#include <cgl/scene.h>

#include <cgl/canvas.h>
extern CglCanvas *pcv;

#include <sstream>
#include <iterator>
#include <set>
#include <fstream>

#include <glm/gtx/intersect.hpp>
#include <glm/gtx/vector_angle.hpp>

//Used for parsing save file
template<typename T> std::vector<T> split(const std::string& line) {
    std::istringstream is(line);
    return std::vector<T>(std::istream_iterator<T>(is), std::istream_iterator<T>());
}

// object constructor
CglScene::CglScene(){
  pcv->addScene(this);
  //:transform()
  selected = true;
  m_cam    = glm::normalize(glm::vec3(1,1,1));
  m_look   = -m_cam;
  m_up     = glm::normalize(glm::vec3(-1, 1., -1));
  m_right  = glm::cross(m_look, m_up);
  center   = glm::vec3(0,0,0);
  VIEW     = glm::lookAt(m_cam, m_look, m_up);
  globalScale = 100000.0f;//For use of minimums later
  background  = new CglBackground();
  cout << "Scene created with Adress = " << this << endl;
}
CglScene::~CglScene(){
    delete background;
}


void CglScene::addObject(pCglObject object)
{
  //Ajout de l'objet à la scène
  listObject.push_back(object);
  object->setScene(this, numObjects());

  //Création des axes
  if(numObjects()==1){
    axis = new CglAxis();
    axis->setScene(this, 0);
    axis->view = view;
  }

  globalScale = min(globalScale, object->getLocalScale());
}
void CglScene::removeObject(pCglObject object)
{
  for(int i = 0 ; i < numObjects() ; i++)
    if(object == listObject[i])
        listObject.erase(listObject.begin() + i);
}


void CglScene::display(){
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
    glEnable( GL_MULTISAMPLE );

    if(pcv->profile.globalScale){
        for(int i = 0 ; i < numObjects() ; i++){
            listObject[i]->setScaleFactor(globalScale);
        }
    }

    update_matrices();
    applyTransformation();

    for (int i = 0; i < listGroup.size(); i++)
        listGroup[i]->compute();

    for (int iObj = 0; iObj < numObjects(); iObj++)
        listObject[iObj]->applyTransformation();

    background->display();

    //applyTransformation();

    for (int iObj = 0; iObj < numObjects(); iObj++)
        if(!listObject[iObj]->isHidden())
            listObject[iObj]->shadowsDisplay();
    for (int iObj = 0; iObj < numObjects(); iObj++)
        if(listObject[iObj]->isHidden())
            listObject[iObj]->shadowsDisplay();

    //Display de l'ensemble des artefacts
    for (int iObj = 0; iObj < numObjects(); iObj++)
        listObject[iObj]->artifactsDisplay();

    //Display des meshes
    for (int iObj = 0; iObj < numObjects(); iObj++)
        if(!listObject[iObj]->isHidden())
            listObject[iObj]->display();

    axis->applyTransformation();
    axis->display();

    glClear(GL_DEPTH_BUFFER_BIT);

    //pcv->getInterface()->display();
    //debug();
}


pCglObject CglScene::getPicked(int x, int y){
    unsigned char pixel[3];
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT,viewport);
    glFlush();
    for(int i = 0 ; i < numObjects() ; i++)
        listObject[i]->pickingDisplay();
    glFlush();
    glReadPixels(x,viewport[3]-y,1,1,GL_RGB,GL_UNSIGNED_BYTE,(void *)pixel);
    //On récupère l'indice de l'objet pické
    for(int i = 0 ; i < listObject.size() ; i++){
        pCglObject obj = listObject[i];
        if (obj->isPicked(pixel[0])){
            return obj;
        }
    }
    for(int i = 0 ; i < listObject.size() ; i++){
        pCglObject obj = listObject[i];
        if(obj->isSuper()){
            for(int j = 0 ; j < obj->listPart.size() ; j++){
                pCglObject part = obj->listPart[j];
                if (part->isPicked(pixel[0])){
                    return part;
                }
            }
        }
    }
    return NULL;
}

void CglScene::onPick(bool ctrl, pCglObject obj){
  bool match = false;
  int  nObjs = listObject.size();
  if (obj!=NULL)
    match = true;

  //Si on ne picke pas, on déselectionne tout et on sélectionne la scène
  if(!match){
    select();
    for(int i = 0 ; i < nObjs ; i++)
      listObject[i]->unSelect();
  }

  if(match){
    //On change l'état de sélection de l'objet pické
    cout << "Object " << obj->getID() << " last selection = " << obj->isSelected() << endl;
    if(obj->parent)
        obj->parent->toogleSelected();
    else
        obj->toogleSelected();
    cout << "Object " << obj->getID() << " selection =  " << obj->isSelected() << endl;

    //Unselect other objects
    if(!ctrl){
        for(int i = 0 ; i < nObjs ; i++){
            pCglObject objI = listObject[i];

            if(obj->parent==NULL){
                if(objI!=obj)
                    objI->unSelect();}
            if(obj->parent!=NULL){
                if(objI != obj->parent)
                    objI->unSelect();}
        }
    }

    //Si un selectionné, deselectionne la scène
    bool someSelected = false;
    for(int i = 0 ; i < nObjs ; i++)
        if(listObject[i]->isSelected()){
            someSelected = true;
            cout << "Object " << listObject[i]->getID() << " is selected "<< endl;}
    if(someSelected)
      unSelect();
    else
      select();

    //On met le dernier objet pické au dessus de tous les autres si il est sélectionné
    if(obj->isSelected())
        reOrderObjects(obj);
  }//End match
}

void CglScene::save(){
  if (isSelected())
    saveTransformations();
  for (unsigned int i = 0; i < listObject.size() ; i++){
    pCglObject obj = listObject[i];
    if (obj->isSelected()){
      //if((!obj->isConstrainedInRotation()) && (!obj->isConstrainedInTranslation()))
      obj->saveTransformations();
    }
  }
}



//Dragging
void CglScene::onDrag(int x, int y){}

float CglScene::orientedAngle(glm::vec3 pt1,
			      glm::vec3 pt2,
			      glm::vec3 c,
			      glm::vec3 ax){
  float a = glm::orientedAngle( glm::normalize(glm::vec3(c.x - pt1.x, c.y, c.z - pt1.z)),
                                glm::normalize(glm::vec3(c.x - pt2.x, c.y, c.z - pt2.z)),
                                ax
                              );
  return a;
}

void CglScene::onLeftDrag(int x, int y){
  glm::vec3 planeNormal(0,1,0);
  bool intersects, intersectsLast;
  glm::vec3 ray, lastRay;
  glm::vec3 inter, lastInter;

  //POUR LA SCENE
  glm::mat4 ROT(1), ID(1);
  float dx = x - lastDrag.x;
  float dy = y - lastDrag.y;
  float f = 0.01f;
  if(pcv->profile.camera == CGL_CAM_UPPER_SPHERE)
    ROT = glm::mat4(  glm::angleAxis(f*dy, glm::normalize(m_right))  *  glm::angleAxis(f*dx, glm::vec3(0,1,0)) );
  else if(pcv->profile.camera == CGL_CAM_FULL_SPHERE)
    ROT = glm::mat4( glm::angleAxis(f*dy, m_right) * glm::angleAxis(f*dx, m_up) );
  if (isSelected())
    transform.setRotation(ROT);

  //POUR LES OBJETS
  for (unsigned int i = 0; i < listObject.size() ; i++){
    pCglObject obj  = listObject[i];
    if ( (obj->isSelected()) /*&& (obj->getGroup()==NULL)*/ ){
      glm::vec3 c     = glm::vec3(obj->getRotationCenter()) + glm::vec3(MODEL[3]);
      planeNormal     = -c + m_cam;
      glm::vec3 axis  = glm::vec3(0,1,0);
      ray             = getRayVector(x,y);
      inter           = intersect(ray, c, planeNormal, intersects);
      lastRay         = getRayVector(lastDrag.x, lastDrag.y);
      lastInter       = intersect(lastRay, c, planeNormal, intersectsLast);
      float     angle = orientedAngle(lastInter, inter, c, axis);
      glm::mat4 tot   = glm::mat4(glm::angleAxis(4.0f * angle, axis));
      listObject[i]->transform.setRotation( tot );
    }
  }
  lastDrag = glm::vec2(x,y);
}

glm::vec3 CglScene::intersect(glm::vec3 ray,
			      glm::vec3 plane,
			      glm::vec3 planeNormal,
			      bool      &intersects){
  float      intersectDist;
  glm::vec3  intersection;
  intersects = glm::intersectRayPlane(m_cam,
				      ray,
				      plane,
				      planeNormal,
				      intersectDist);
  if(intersects)
    intersection = m_cam + intersectDist * ray;
  return intersection;
}

void CglScene::onMiddleDrag(int x, int y){
  glm::vec3 plane(0, -pcv->profile.bottomDistance, 0);
  glm::vec3 planeNormal(0,1,0);
  bool intersects, intersectsLast;
  glm::vec3 ray, lastRay;
  glm::vec3 inter, lastInter;

  ray       = getRayVector(x,y);
  inter     = intersect(ray, plane, planeNormal, intersects);
  //cout << inter.x << " " << inter.y << " " << inter.z << endl;

  lastRay   = getRayVector(lastDrag.x, lastDrag.y);
  lastInter = intersect(lastRay, plane, planeNormal, intersectsLast);

  if(!isSelected()){
    if(intersects)
      for(int i = 0 ; i < listObject.size() ; i++)
	if (listObject[i]->isSelected())
	  listObject[i]->transform.setTranslation(inter - lastInter);}
  else{
    if(intersects)
      transform.setTranslation(inter - lastInter);}

  lastDrag = glm::vec2(x,y);
}
void CglScene::onRightDrag( int x, int y){}

//Release
void CglScene::onRelease(int x, int y){
  for (unsigned int i = 0; i < listObject.size(); i++){
    CglObject *obj = listObject[i];
    if ( (obj->isConstrainedInRotation()) || ((obj->isConstrainedInTranslation())) )
      obj->unConstrain();
  }
}
void CglScene::onLeftRelease(  int x, int y){}
void CglScene::onMiddleRelease(int x, int y){}
void CglScene::onRightRelease( int x, int y){
  bool  ctrl = ((glutGetModifiers() && GLUT_ACTIVE_CTRL) ? 1:0);
  if( !pcv->profile.flyingMode ){
    pCglObject pickedObject = getPicked(x, y);
    if(pickedObject)
        cout << "ID = " << pickedObject->getID() << endl;
    onPick(ctrl, pickedObject);
  }
}

//Clicks
void CglScene::onClick(      int x, int y){lastDrag = glm::vec2(x,y);}
void CglScene::onLeftClick(  int x, int y){save();}
void CglScene::onMiddleClick(int x, int y){save();}
void CglScene::onRightClick( int x, int y){}




void CglScene::reOrderObjects(pCglObject obj){
    for(int i = 0 ; i < listObject.size() ; i++){
        if(listObject[i] == obj){
            listObject.insert(listObject.begin(), listObject[i]);
            listObject.erase(listObject.begin() + i + 1);
        }
    }
}

void CglScene::toogleFlyingMode(){
  pcv->profile.displayAxes = ((pcv->profile.flyingMode)?1:0);
  pcv->centerMouse();

  //FROM FLYING TO NORMAL
  if(pcv->profile.flyingMode){
    m_cam *= 1.0f * glm::length(m_cam);
    m_look = -m_cam;
    glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);
    m_right = glm::normalize(glm::vec3(m_cam.z, 0, -m_cam.x));
    m_up = glm::cross(m_right, m_look);
  }
  //FROM NORMAL TO FLYING
  else{
    glutSetCursor(GLUT_CURSOR_NONE);
    glutWarpPointer(view->width/2, view->height/2);
    m_look = -m_cam;
  }

  pcv->profile.flyingMode = !pcv->profile.flyingMode;
  select();
  transform.reset();
}

void CglScene::applyTransformation()
{
  //FLYING MODE
  if(pcv->profile.flyingMode){
    m_cam   +=  transform.tr;
    m_look  =   glm::normalize(glm::vec3(glm::inverse(transform.rot) * glm::vec4(m_look,0)));
    m_up    =   glm::normalize(glm::vec3(glm::inverse(transform.rot) * glm::vec4(m_up,1)));

    VIEW = glm::lookAt(m_cam + view->camOffset * m_right,
                       m_cam + view->camOffset * m_right + m_look,
                       m_up);
    m_right = glm::cross(m_look,m_up);
  }

  //CLASSICAL MODE
  else if (pcv->profile.camera == CGL_CAM_UPPER_SPHERE){
    MODEL = glm::translate(MODEL, transform.tr);
    m_cam   =  view->zoom * glm::normalize(glm::vec3(glm::inverse(transform.rot) * glm::vec4(m_cam,1)));
    if (m_cam.y< 0){
      m_cam.y = 0;
      m_up = glm::normalize(glm::vec3(0,m_up.y,0));
    }
    else{
      m_up    = glm::normalize(glm::vec3(glm::inverse(transform.rot) * glm::vec4(m_up,1)));
    }
    m_look = -m_cam;
    VIEW = glm::lookAt(m_cam + view->camOffset * m_right, m_look, m_up);
    m_right = glm::cross(m_look,m_up);
  }

  //ACCUMULATED MODE
  else if(pcv->profile.camera == CGL_CAM_FULL_SPHERE){
    MODEL = glm::translate(MODEL, transform.tr);
    m_cam   =  view->zoom * glm::normalize(glm::vec3(glm::inverse(transform.rot) * glm::vec4(m_cam,1)));
    m_right = glm::normalize(glm::vec3(glm::inverse(transform.rot) * glm::vec4(m_right,1)));
    m_look  = -m_cam;//glm::normalize(glm::vec3(glm::inverse(transform.rot) * glm::vec4(m_look,1)));//-m_cam;
    m_up    = glm::cross(m_right, m_look);
    //m_up    = glm::normalize(glm::vec3(glm::inverse(transform.rot) * glm::vec4(m_up,1)));
    VIEW    = glm::lookAt(m_cam + view->camOffset * m_right, m_look, m_up);
    //m_right = glm::cross(m_look,m_up);
  }

  transform.reset();
}

void CglScene::update_matrices()
{
  //if(!pcv->profile.flyingMode)
  VIEW = glm::lookAt(m_cam + view->camOffset * m_right, m_look, m_up);

  if(pcv->profile.perspective)
    PROJ = glm::perspective(view->m_fovy, view->ratio, view->m_znear, view->m_zfar);
  else
    PROJ = glm::ortho(view->zoom * -view->ratio*0.5,
                      view->zoom * view->ratio*0.5,
                     -0.5 * view->zoom,
                      0.5 * view->zoom,
                      view->m_znear,
                      view->m_zfar);
}

void CglScene::saveTransformations(){
  transform.lastMatrices.push_back(MODEL);
  transform.lastUps.push_back(m_up);
  transform.lastCams.push_back(m_cam);
}

void CglScene::undoLast(){
  if(transform.lastMatrices.size()>0){
    MODEL = transform.lastMatrices.back();
    center = glm::vec3(glm::vec4(MODEL[3]));
    m_up = transform.lastUps.back();
    m_cam = transform.lastCams.back();
    transform.lastMatrices.pop_back();
    transform.lastUps.pop_back();
    transform.lastCams.pop_back();
  }
}

void CglScene::resetAll(){
  while(transform.lastMatrices.size()>0)
    undoLast();
  for(int i = 0 ; i < numObjects() ; i++)
    listObject[i]->resetAll();
  m_cam = glm::vec3(1,1,1);
  m_up = glm::normalize(glm::vec3(-1, 1., -1));
  view->zoom = 1.0f;
}


void CglScene::debug(){
  //Vectors

    cout << 1./globalScale * 0.2;

  //cout << "cam   = " << m_cam.x   << " " << m_cam.y   << " " << m_cam.z   << endl;
  //cout << "look  = " << m_look.x  << " " << m_look.y  << " " << m_look.z  << endl;
  //cout << "up    = " << m_up.x    << " " << m_up.y    << " " << m_up.z    << endl;
  //cout << "right = " << m_right.x << " " << m_right.y << " " << m_right.z << endl;
  //cout << "center = " << center.x << " " << center.y << " " << center.z << endl;

  /*
  //Matrices
  cout << endl;
  cout << " MODEL " << endl;
  CglObject obj = *listObject[0];
  for(int i = 0 ; i < 4 ; i++)
    cout << obj.MODEL[i][0] << " " << obj.MODEL[i][1] << " " << obj.MODEL[i][2] << " " << obj.MODEL[i][3] << endl;
  cout << endl;
  cout << " VIEW " << endl;
  for(int i = 0 ; i < 4 ; i++)
    cout << VIEW[i][0] << " " << VIEW [i][1] << " " << VIEW [i][2] << " " << VIEW [i][3] << endl;
  cout << endl;
  cout << " PROJ " << endl;
  for(int i = 0 ; i < 4 ; i++)
    cout << PROJ[i][0] << " " << PROJ[i][1] << " " << PROJ[i][2] << " " << PROJ[i][3] << endl;
  cout << endl;
  */
}


void CglScene::cglInit()
{
  //  glEnable(GL_DEPTH_TEST);	// Active le test de profondeur
  // 	glEnable(GL_LIGHTING);	// Active l'éclairage
  // 	glEnable(GL_LIGHT0);
}

bool CglScene::isSelected(){return selected;}
void CglScene::select(){  selected = true;}
void CglScene::unSelect(){selected = false;}

//Placement des objets sur une grille
void CglScene::place_objects_on_grid(){
  int nM = 0;
  for(int i = 0 ; i < listObject.size() ; i++)
    if(listObject[i]->isMeshObject())
      nM++;

  int nRow = int(sqrt(nM));
  int nCol = nM/nRow;
  if(nRow * nCol != nM)
    nCol++;
  float dist = 0.35;
  float offX = ((nCol%2==0)? 0.5 * dist*nCol/2.0f : dist*nCol/2.0f - dist/2.0f );
  float offZ = ((nRow%2==0)? 0.5 * dist*nRow/2.0f : dist*nRow/2.0f - dist/2.0f );

  for(int i = 0 ; i < listObject.size() ; i++){
    if(listObject[i]->isMeshObject()){
      glm::vec3 c = glm::vec3( -offX + dist * (i%nCol), 0, -offZ + dist * (i/nCol));
      glm::mat4 M = glm::mat4(1);
      M[3][0] = c.x;
      M[3][1] = c.y;
      M[3][2] = c.z;
      listObject[i]->setCenter(c);
      listObject[i]->setMODEL(M);
    }
  }
}

glm::vec3 CglScene::getRayVector(int x, int y){
  //Mouse position to [-1,1]
  int       w = view->width;
  int       h = view->height;

  glm::vec2 window_position(x,y);
  glm::vec2 normalized_window_position((x/(w*0.5f) - 1) , 1.0f - y/(h*0.5f));

  //Ray end points
  float     fov             = view->m_fovy;
  float     tan             = tanf(fov * 0.5f);
  glm::vec2 fov_coordinates = tan * normalized_window_position;
  float     near            = view->m_znear;
  float     far             = view->m_zfar;


  glm::vec3 near_point(fov_coordinates.x * near, fov_coordinates.y * near, -near);
  glm::vec3 far_point( fov_coordinates.x * far,  fov_coordinates.y * far,  -far);

  //World coordinates
  glm::mat4 inv             = glm::inverse(VIEW);
  near_point                = m_cam + glm::vec3( inv * glm::vec4(near_point, 1) );
  far_point                 = m_cam + glm::vec3( inv * glm::vec4(far_point,  1) );
  glm::vec3 ray             = glm::normalize(far_point - near_point);

  return ray;
}



void CglScene::load_meshes_from_file(string fileName){
  vector<string>    names;
  vector<glm::mat4> mats;
  vector<glm::vec3> centers;
  vector<int>       groups;
  vector<glm::vec4> matrices;
  string line;
  int lineNumber = -1;
  int numLines = 7;
  int numberMeshes = 0;

  //Lecture du fichier de sauvegarde
  ifstream saveFile(fileName);
  if (saveFile.is_open()){
    while ( getline (saveFile,line) ){
      if (lineNumber==-1)
        numberMeshes = atoi(line.c_str());
      if (lineNumber%numLines == 0)
        names.push_back(line);
      for(int i = 1 ; i < 5 ; i++){
        if(lineNumber%numLines == i){
          glm::vec4 ROW;
          vector<float> values = split<float>(line);
          for(int j = 0 ; j < 4 ; j++){
            ROW[j] = values[j];
          }
          matrices.push_back(ROW);
        }
      }
      if(lineNumber%numLines == 5){
        centers.push_back(glm::vec3(0.0f));
        vector<double> values = split<double>(line);
        for(int i = 0 ; i < 3 ; i++){
          centers[centers.size()-1][i] = values[i];
        }
      }
      if((lineNumber%numLines == 6) && (lineNumber!=-1))
        groups.push_back(atoi(line.c_str()));
      lineNumber++;
    }
    saveFile.close();
  }
  else{
    cout << "Unable to open file";
    exit(0);
  }

  //Réajustement des matrices des matrices
  for(int i = 0 ; i < matrices.size() ; i++){
    if(i%4 == 0)
      mats.push_back(glm::mat4(1.0f));
    mats[i/4][i%4] = matrices[i];
  }

  //Upload des propriétés du maillage
  vector<pCglMesh> mesh;
  for (int i=0; i < numberMeshes; i++){
    char *N = (char*)names[i].c_str();
    mesh.push_back(new CglMesh(N));
    //mesh[i]->meshInfo(0);
    //int idObj = cglObject(mesh[i]);
    mesh[i]->setCenter(centers[i]);
    mesh[i]->setMODEL(mats[i]);
    addObject(mesh[i]);

    //Création des groupes
    set<int> indGroups(groups.begin(), groups.end());
    for (set<int>::iterator i = indGroups.begin(); i != indGroups.end(); i++) {
      std::vector<pCglObject> objectsToGroup;
      for(int j = 0 ; j < numObjects() ; j++){
        if((groups[j]==*i) && (groups[j]!=-1)){
          objectsToGroup.push_back(getObject(j));
        }
      }
      if(objectsToGroup.size()>1){
        getGroupList()->push_back(new CglGroup(objectsToGroup));
      }
    }
    mesh[i]->setFileName(names[i]);
  }
}



int CglScene::addLight(pCglLight li){
  lights.push_back(li);
  return lights.size() - 1;
}
