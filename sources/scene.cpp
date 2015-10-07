#include <cgl/scene.h>
#include <cgl/canvas.h>
extern CglCanvas *pcv;

#include <glm/gtx/intersect.hpp>
#include <glm/gtx/vector_angle.hpp>

// object constructor
CglScene::CglScene():transform(){
  selected = true;
  m_cam = glm::normalize(glm::vec3(1,1,1));
  m_look = -m_cam;
  m_up = glm::normalize(glm::vec3(-1, 1., -1));
  m_right = glm::cross(m_look, m_up);
  center = glm::vec3(0,0,0);
  VIEW = glm::lookAt(m_cam, m_look, m_up);
  globalScale = 100000.0f;//For use of minimums later
  background = new CglBackground();
}
CglScene::~CglScene(){}


void CglScene::addObject(pCglObject object)
{
  //Ajout de l'objet à la scène
  listObject.push_back(object);
  object->linkSceneParameters(&MODEL, &VIEW, &PROJ, &center, &m_up, &m_cam, numObjects()+100);

  //Création des axes
  if(numObjects()==1){
    axis = new CglAxis();
    axis->linkSceneParameters(&MODEL, &VIEW, &PROJ, &center, &m_up, &m_cam, 0);
    axis->view = view;
  }

  globalScale = min(globalScale, object->getLocalScale());
}


void CglScene::display()
{
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
  axis->applyTransformation();
  axis->display();

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
  for (int iObj = 0; iObj < numObjects(); iObj++)
    if(listObject[iObj]->isHidden())
      listObject[iObj]->display();



  //GUI
  pcv->getInterface()->display();

  glClear(GL_DEPTH_BUFFER_BIT);

  //debug();
}


int CglScene::getPickedID(int x, int y){
  unsigned char pixel[3];
  GLint viewport[4];
  glGetIntegerv(GL_VIEWPORT,viewport);
  glFlush();
  for(int i = 0 ; i < numObjects() ; i++)
    listObject[i]->pickingDisplay();
  glFlush();
  glReadPixels(x,viewport[3]-y,1,1,GL_RGB,GL_UNSIGNED_BYTE,(void *)pixel);

  int indPicked = -1;
  //On récupère l'indice de l'objet pické
  for(int i = 0 ; i < listObject.size() ; i++)
    if (listObject[i]->isPicked(pixel[0]))
      indPicked = i;
  return indPicked;
}

void CglScene::onPick(bool ctrl, int ind){
  bool match = false;
  int  nObjs = listObject.size();
  if (ind!=-1)
    match = true;

  //Si on ne picke pas, on déselectionne tout et on sélectionne la scène
  if(!match){
    select();
    for(int i = 0 ; i < nObjs ; i++)
      listObject[i]->unSelect();
  }

  if(match){
    //On change l'état de sélection de l'objet pické
    listObject[ind]->toogleSelected();

    if(!ctrl){
      //On déselectionne tous les autres objets
      for(int i = 0 ; i < nObjs ; i++)
	if(i!=ind)
	  listObject[i]->unSelect();
    }

    //Si un selectionné, deselectionne la scène
    bool someSelected = false;
    for(int i = 0 ; i < nObjs ; i++)
      if(listObject[i]->isSelected())
	someSelected = true;
    if(someSelected)
      unSelect();
    else
      select();

    //On met le dernier objet pické au dessus de tous les autres si il est sélectionné
    if(listObject[ind]->isSelected())
      reOrderObjects(ind);
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
  float a = glm::orientedAngle(glm::normalize(glm::vec3(pt1.x - c.x,
							c.y,
							pt1.z - c.z)),
			       glm::normalize(glm::vec3(pt2.x - c.x,
							c.y,
							pt2.z - c.z)),
			       ax);
  return 3.0f * a;
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
    if ( (obj->isSelected()) /*&& (obj->getGroupID()==-1)*/ ){
      glm::vec3 c     = glm::vec3(obj->getRotationCenter()) + glm::vec3(MODEL[3]);
      //planeNormal     = -c + m_cam;
      glm::vec3 axis  = glm::vec3(0,1,0);
      ray             = getRayVector(x,y);
      inter           = intersect(ray, c, planeNormal, intersects);
      lastRay         = getRayVector(lastDrag.x, lastDrag.y);
      lastInter       = intersect(lastRay, c, planeNormal, intersectsLast);
      float     angle = orientedAngle(lastInter, inter, c, axis);
      glm::mat4 ROT   = glm::mat4(glm::angleAxis(angle, axis));
      listObject[i]->transform.setRotation(ROT);
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

  lastRay   = getRayVector(lastDrag.x, lastDrag.y);
  lastInter = intersect(lastRay, plane, planeNormal, intersectsLast);

  //cout << "curr: " << inter.x <<     "/" << inter.y <<     "/" << inter.z << endl;
  //cout << "last: " << lastInter.x << "/" << lastInter.y << "/" << lastInter.z << endl;

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
    int IndPicked = getPickedID(x, y);
    onPick(ctrl, IndPicked);
  }
}

//Clicks
void CglScene::onClick(      int x, int y){lastDrag = glm::vec2(x,y);}
void CglScene::onLeftClick(  int x, int y){save();}
void CglScene::onMiddleClick(int x, int y){save();}
void CglScene::onRightClick( int x, int y){}




void CglScene::reOrderObjects(int picked){
  listObject.insert(listObject.begin(), listObject[picked]);
  listObject.erase(listObject.begin() + picked + 1);
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
    m_cam   +=   transform.tr;
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
    m_look  = -m_cam;
    VIEW = glm::lookAt(m_cam + view->camOffset * m_right, m_look, m_up);
    m_right = glm::cross(m_look,m_up);
  }

  //ACCUMULATED MODE
  else if(pcv->profile.camera == CGL_CAM_FULL_SPHERE){
    MODEL = glm::translate(MODEL, transform.tr);
    m_cam   =  view->zoom * glm::normalize(glm::vec3(glm::inverse(transform.rot) * glm::vec4(m_cam,1)));
    m_right = glm::normalize(glm::vec3(glm::inverse(transform.rot) * glm::vec4(m_right,1)));
    m_look  = -m_cam;//glm::normalize(glm::vec3(glm::inverse(transform.rot) * glm::vec4(m_look,1)));//-m_cam;
    m_up = glm::cross(m_right, m_look);
    //m_up    = glm::normalize(glm::vec3(glm::inverse(transform.rot) * glm::vec4(m_up,1)));
    VIEW = glm::lookAt(m_cam + view->camOffset * m_right, m_look, m_up);
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

  cout << "cam   = " << m_cam.x   << " " << m_cam.y   << " " << m_cam.z   << endl;
  cout << "look  = " << m_look.x  << " " << m_look.y  << " " << m_look.z  << endl;
  cout << "up    = " << m_up.x    << " " << m_up.y    << " " << m_up.z    << endl;
  cout << "right = " << m_right.x << " " << m_right.y << " " << m_right.z << endl;
  cout << "center = " << center.x << " " << center.y << " " << center.z << endl;

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
  glm::vec2 normalized_window_position( (x/(w*0.5f) - 1)/float(w/h), 1.0f - y/(h*0.5f));

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
