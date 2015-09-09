#include <fstream>

#include <cgl/keyboard.h>
#include <cgl/cube.h>

#include <cgl/canvas.h>
extern CglCanvas *pcv;

void CglKeyboard::keyColor(unsigned char key,int x,int y) {
  pcv->winid();
}

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
    if (glutGetModifiers() && GLUT_ACTIVE_CTRL)
      speed = 0.001f;
    else
      speed = 0.015f;
    switch (key) {
      case GLUT_KEY_LEFT:
        scene->getTransform()->setTranslation(-speed * scene->getRight());
        break;
      case GLUT_KEY_RIGHT:
        scene->getTransform()->setTranslation(speed * scene->getRight());
        break;
      case GLUT_KEY_DOWN:
        scene->getTransform()->setTranslation(-speed * glm::normalize(scene->getLook()));
        break;
      case GLUT_KEY_UP:
        scene->getTransform()->setTranslation(speed * glm::normalize(scene->getLook()));
        break;
      default:
        break;
    }
  }

  lastKey = key;
}

void CglKeyboard::keyboard(unsigned char key, int x, int y)
{
  pCglScene scene = pcv->getScene();;

  // QUIT
  if ( key == 'q' || key == 27 ){
    exit(0);}

  // ZOOM
  if(((key == 'z') || (key == 'Z')) && (lastKey!='t') && (lastKey!='r')){
    if ((glm::length(scene->getCam())>0.15) && (key=='z'))
      scene->getView()->zoom *= 0.95;
    else if( (glm::length(scene->getCam())<1.75) && (key=='Z') )
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
      if(scene->getGroup(iG)->isSelected()){
        for(int iO = 0 ; iO < scene->getGroup(iG)->numObjects() ; iO++){
          scene->getGroup(iG)->listObject[iO]->unSelect();
          scene->getGroup(iG)->listObject[iO]->resetGroupID();
        }
        scene->getGroupList()->erase(scene->getGroupList()->begin() + iG);
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
    saveFile.open("cgl.save");
    int numberMeshes = 0;
    for(int i = 0 ; i < scene->numObjects() ; i++)
      if(scene->getObject(i)->isMeshObject())
        numberMeshes++;
    saveFile << numberMeshes << endl;
    for(int i = 0 ; i < scene->numObjects() ; i++){
      if(scene->getObject(i)->isMeshObject()){
        pCglObject obj = scene->getObject(i);
        glm::mat4 M = obj->getMODEL();
        saveFile << obj->meshFile << endl;
        for(int i = 0 ; i < 4 ; i++)
          saveFile << float(M[i][0]) << " " << float(M[i][1]) << " " << float(M[i][2]) << " " << float(M[i][3]) << endl;
        glm::vec3 c = *(obj->getCenterPtr());
        saveFile << c.x << " " << c.y << " " << c.z << endl;
        saveFile << obj->getGroupID() << endl;
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
    pcv->profile.dark_theme = !pcv->profile.dark_theme;
    pcv->profile.update_theme();
  }

  lastKey = key;

}

void CglKeyboard::keyboardUp(unsigned char key, int x, int y){}

