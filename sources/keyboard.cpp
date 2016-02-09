#include <fstream>
#include <string>

#include <cgl/keyboard.h>
#include <cgl/cube.h>

#include <cgl/canvas.h>
extern CglCanvas *pcv;

void setTranslation(glm::vec3 tr, int &state){
  pCglScene scene = pcv->getScene();;
  for (unsigned int iObj = 0; iObj < scene->numObjects() ; iObj++){
    pCglObject obj = scene->getObject(iObj);
    if (obj->isSelected()){
      obj->transform.setTranslation(tr);
      state = 1;
    }
  }
  if (state == 0)
    scene->getTransform()->setTranslation(tr);
}

void CglKeyboard::special(unsigned char key, int x, int y)
{
  pCglScene scene = pcv->getScene();;

  if(key!=lastKey){
    if (scene->isSelected()){
      scene->saveTransformations();
    }
    for (unsigned int i = 0; i < scene->numObjects() ; i++){
      CglObject *obj = scene->getObject(i);
      if (obj->isSelected()){
        //if(( !obj->isConstrainedInRotation()) && (!obj->isConstrainedInTranslation()) )
          obj->saveTransformations();
      }
    }
  }


  if(!pcv->profile.flyingMode){
    glm::vec3 moveX = 0.0025f * scene->getRight();
    glm::vec3 moveZ = 0.0025f * glm::vec3(0,1,0);
    int state = 0;
    switch (key) {
      case GLUT_KEY_LEFT:
        setTranslation(-moveX, state);
        break;
      case GLUT_KEY_RIGHT:
        setTranslation(moveX, state);
        break;
      case GLUT_KEY_DOWN:
        setTranslation(-moveZ, state);
        break;
      case GLUT_KEY_UP:
        setTranslation(moveZ, state);
        break;
      default:
        break;
    }
  }
  else{
    float speed;
    if (glutGetModifiers() && GLUT_ACTIVE_CTRL )
      speed = 0.002f;
    else
      speed = 0.015f;

    switch (key) {
      case GLUT_KEY_LEFT:
	scene->setCam( scene->getCam() - speed * scene->getRight() );
        break;
      case GLUT_KEY_RIGHT:
	scene->setCam( scene->getCam() + speed * scene->getRight() );
        break;
      case GLUT_KEY_DOWN:
	scene->setCam( scene->getCam() - speed * glm::normalize(scene->getLook()) );
        break;
      case GLUT_KEY_UP:
	scene->setCam( scene->getCam() + speed * glm::normalize(scene->getLook()) );
        break;
      default:
        break;
    }
  }

  lastKey = key;
}


void quit(pCglScene scene){
    exit(0);
}

void CglKeyboard::keyboard(unsigned char key, int x, int y)
{
    pCglScene scene = pcv->getScene();

    //Custom actions
    for(int i = 0 ; i < customActions.size() ; i++){
        if(key == customKeys[i]){
            (*customActions[i])(scene);
        }
    }


  // QUIT
  if ( key == 'q' || key == 27 ){
    exit(0);}

  // ZOOM
  if(((key == 'z') || (key == 'Z')) && (lastKey!='t') && (lastKey!='r')){
    if ((glm::length(scene->getCam())>0.05) && (key=='z'))
      scene->getView()->zoom *= 0.95;
    else if( (glm::length(scene->getCam())<10) && (key=='Z') )
      scene->getView()->zoom *= 1.05;
  }

  // BB
  if (key == 'b' ){
    for (unsigned int i = 0; i < scene->numObjects() ; i++)
      scene->getObject(i)->toogleBBox();
  }

  // Wireframe
  if (key == 'm' ){
    for (unsigned int i = 0; i < scene->numObjects() ; i++)
      scene->getObject(i)->toogleMesh();
  }

  //Reset Transformations
  if (key == 'e'){
    if(scene->isSelected()){
      scene->undoLast();
    }
    for(int i = 0 ; i < scene->numObjects() ; i++){
      pCglObject obj = scene->getObject(i);
      if(obj->isSelected()){
        obj->undoLast();
      }
    }
  }
  if (key == 'E'){
    scene->resetAll();
  }

  //Constrained translation
  if(lastKey=='t'){
    cout << "constrain in Translation" << endl;
    for(int i = 0 ; i < scene->numObjects() ; i++){
      pCglObject obj = scene->getObject(i);
      if(obj->isSelected()){
        glm::vec3 constAxis;
        if(key=='x')
          constAxis = glm::vec3(1,0,0);
        else if(key=='y')
          constAxis = glm::vec3(0,1,0);
        else if(key=='z')
          constAxis = glm::vec3(0,0,1);
        if(constAxis!=glm::vec3(0))
          obj->constrainTranslation(constAxis);
      }
    }
  }
  //Constrained Rotation
  if(lastKey=='r'){
    cout << "constrain in Rotation" << endl;
    for(int i = 0 ; i < scene->numObjects() ; i++){
      pCglObject obj = scene->getObject(i);
      if(obj->isSelected()){
        glm::vec3 constAxis;
        if(key=='x')
          constAxis = glm::vec3(1,0,0);
        else if(key=='y')
          constAxis = glm::vec3(0,1,0);
        else if(key=='z')
          constAxis = glm::vec3(0,0,1);
        if(constAxis!=glm::vec3(0))
          obj->constrainRotation(constAxis);
      }
    }
  }
  //Saving the model
  if((lastKey == 'r') || (lastKey == 't')){
    if((key=='x') || (key=='y') || (key=='z')){
      for(int i = 0 ; i < scene->numObjects() ; i++){
        pCglObject obj = scene->getObject(i);
        if(obj->isSelected()){
          obj->saveTransformations();
        }
      }
    }
  }

  //Group objects
  if (key == 'g'){
    std::vector<pCglObject> objectsToGroup;
    for(int i = 0 ; i < scene->numObjects() ; i++){
      if(scene->getObject(i)->isSelected()){
        objectsToGroup.push_back(scene->getObject(i));
      }
    }
    if(objectsToGroup.size()>1){
      scene->getGroupList()->push_back(new CglGroup(objectsToGroup));
    }
  }
  //Ungroup
  if (key == 'G'){
    for(int iG = 0 ; iG < scene->numGroups() ; iG++){
        pCglGroup G = scene->getGroup(iG);
      if(G->isSelected()){
        for(int iO = 0 ; iO < G->numObjects() ; iO++){
          G->getObject(iO)->unSelect();
          G->getObject(iO)->resetGroup();
        }
        scene->getGroupList()->erase(scene->getGroupList()->begin() + iG);
        G->unGroup();
      }
    }
  }

  //Hide
  if (key == 'h'){
    for(int i = 0 ; i < scene->numObjects() ; i++){
      pCglObject obj = scene->getObject(i);
      if(obj->isSelected()){
        obj->hide();
        obj->unSelect();
        scene->select();
      }
    }
  }
  if (key == 'H'){
    for(int i = 0 ; i < scene->numObjects() ; i++){
      if(scene->getObject(i)->isHidden()){
        scene->getObject(i)->unHide();
      }
    }
  }

  //Camera Presets
  if (key == '1'){
    scene->setCam(glm::vec3(0,0,1));}
  if (key == '3'){
    scene->setCam(glm::vec3(1,0,0));}
  if ((key=='7') && (!pcv->profile.flyingMode)){
    scene->setCam(glm::vec3(0,1,0));
    scene->setUp(glm::vec3(0,0,-1));
  }
  if ((key == '1') || (key == '3')){
    scene->setUp(glm::vec3(0,1,0));}

  //Ortho view
  if (key == '5'){
    pcv->profile.perspective = !pcv->profile.perspective;}

  //Save
  if(key == 's'){
    ofstream saveFile;
    saveFile.open("cgl.save_" + std::to_string(scene->scene_type) );
    int numberMeshes = 0;
    for(int i = 0 ; i < scene->numObjects() ; i++)
        numberMeshes++;

    saveFile << numberMeshes << endl;

    std::vector<pCglGroup> WrittenGroups;
    std::vector<int> groupIds;
    for(int i = 0 ; i < scene->numObjects() ; i++){
        bool written = false;
        int groupID = 0;
        for(int j = 0 ; j < WrittenGroups.size() ; j++){
            if (scene->getObject(i)->getGroup() == WrittenGroups[j])
                written = true;
        }
        if(!written)
            WrittenGroups.push_back(scene->getObject(i)->getGroup());
    }

    for(int i = 0 ; i < scene->numObjects() ; i++){
        pCglObject obj = scene->getObject(i);
        glm::mat4 M = obj->getMODEL();
        saveFile << obj->pGeom->meshFile << endl;
        for(int i = 0 ; i < 4 ; i++)
          saveFile << float(M[i][0]) << " " << float(M[i][1]) << " " << float(M[i][2]) << " " << float(M[i][3]) << endl;
        glm::vec3 c = *(obj->getCenterPtr());
        saveFile << c.x << " " << c.y << " " << c.z << endl;

        if (obj->getGroup() == NULL)
            saveFile << 0 << endl;
        else{
            for(int j = 0 ; j < WrittenGroups.size() ; j++){
                if (obj->getGroup() == WrittenGroups[j]){
                    saveFile << j+1 << endl;
                }
            }
        }
    }
    saveFile.close();
    cout << "Scene saved!!!" << endl;
  }

  //Flying mode
  if(key == 'f'){
    if( !( glm::normalize(scene->getCam()) == glm::vec3(0,1,0) ) )
      scene->toogleFlyingMode();
  }

  //Theme
  if(key=='B'){
    pcv->profile.switch_theme();
    pcv->getInterface()->updateTextures();
  }
  //Colors
  //if(key=='C'){
  //  pcv->profile.switch_colors();
  //}
  if(key=='c'){
    pcv->profile.update_objects_colors();
  }

    //Fullscreen
    if(key=='F'){
        glutSetWindow(pcv->getWindow()->ID);
        pcv->profile.fullscreen = !pcv->profile.fullscreen;
        if (pcv->profile.fullscreen)
            glutFullScreen();
        else{
            glutReshapeWindow(pcv->getWindow()->size.x, pcv->getWindow()->size.y);//getScene()->getView()->height
        }
    }

  //Lights rotation
  if(key=='R'){
    std::vector<pCglLight> *lights = pcv->getLightsList();
    for(int i = 0 ; i < lights->size() ; i++){
        (*lights)[i]->rotate(0.05f);
    }
  }


    //Copy
    if(key=='C'){
        pcv->clipboard.erase(pcv->clipboard.begin(), pcv->clipboard.end());
        for(int i = 0 ; i < scene->numObjects() ; i++){
            pCglObject obj = scene->getObject(i);
            if(obj->dynamic){
                if(obj->isSelected()){
                    OBJECT_TYPE typ;
                    if(obj->isSuper()){typ = SUPER;}
                    else{typ = SIMPLE;}
                    ClipBoard clip(typ,
                                   (char*)(obj->pGeom->meshFile).c_str(),
                                   obj->getCenter(),
                                   obj->getMODEL());
                    pcv->clipboard.push_back(clip);
                    obj->unSelect();
                }
            }
        }
    }
    //Cut
    if(key=='X'){
        pcv->clipboard.erase(pcv->clipboard.begin(), pcv->clipboard.end());
        std::vector<pCglObject> objectsToRemove;
        for(int i = 0 ; i < scene->numObjects() ; i++){
            pCglObject obj = scene->getObject(i);
            if(obj->dynamic){
                if(obj->isSelected()){
                    OBJECT_TYPE typ;
                    if(obj->isSuper()){typ = SUPER;}
                    else{typ = SIMPLE;}
                    ClipBoard clip(typ,
                                   (char*)(obj->pGeom->meshFile).c_str(),
                                   obj->getCenter(),
                                   obj->getMODEL());
                    pcv->clipboard.push_back(clip);
                    objectsToRemove.push_back(obj);
                }
            }
        }
        for(int i = 0 ; i < objectsToRemove.size() ; i++)
            scene->removeObject(objectsToRemove[i]);
    }
    //Paste
    if(key == 'V'){
        for(int i = 0 ; i < pcv->clipboard.size() ; i++){
            //New place for object
            glm::vec3 plane(0, -pcv->profile.bottomDistance, 0);
            glm::vec3 planeNormal(0,1,0);
            glm::vec3 ray   = pcv->getScene()->getRayVector(x,y);
            bool intersects;
            glm::vec3 inter = pcv->getScene()->intersect(ray, plane, planeNormal, intersects);
            glm::vec3 newC;
            if(intersects)
                newC = inter;

            OBJECT_TYPE object_type = pcv->clipboard[i].object_type;

            scene->unSelect();
            pCglObject obj = NULL;
            int nT = 0;
            for(int iS = 0 ; iS < pcv->getScenes().size() ; iS++){
                for(int iO = 0 ; iO < pcv->getScene(iS)->getObjectList()->size() ; iO++){
                    pCglObject pOBJ = pcv->getScene(iS)->getObject(iO);
                    if((char*)(pOBJ->pGeom->meshFile).c_str() == pcv->clipboard[i].fileName){
                        obj = new CglMesh(pOBJ);
                        scene->addObject(obj);
                        obj->setMODEL(glm::translate(/*pcv->clipboard[i].MODEL*/glm::mat4(1), glm::vec3(newC.x, 0, newC.z)));
                        obj->setCenter(glm::vec3(newC.x, 0, newC.z));//pcv->clipboard[i].center);
                        obj->select();
                        break;
                    }
                }
            }

        }
        pcv->clipboard.erase(pcv->clipboard.begin(), pcv->clipboard.end());
    }
    //Suppr
    if(key == 'D'){
        std::vector<pCglObject> objectsToRemove;
        for(int i = 0 ; i < scene->numObjects() ; i++)
            if(scene->getObject(i)->isSelected())
                objectsToRemove.push_back(scene->getObject(i));
        for(int i = 0 ; i < objectsToRemove.size() ; i++)
            scene->removeObject(objectsToRemove[i]);
    }

    //Speed
    if(key == 'J'){
        cout << pcv->profile.speed << endl;
        if(pcv->profile.speed == 1){
            pcv->profile.speed = 0.1;
        }
        else{
            pcv->profile.speed = 1;
        }
    }

  lastKey = key;
}

void CglKeyboard::keyboardUp(unsigned char key, int x, int y){}

