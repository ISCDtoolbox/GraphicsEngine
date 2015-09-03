#include <glic/canvas.h>
#include <glic/keyboard.h>
#include <glic/cube.h>

extern CglicCanvas *pcv;

void CglicKeyboard::keyColor(unsigned char key,int x,int y) {
  pcv->winid();
}

void setTranslation(glm::vec3 tr, int &state){
  pCglicScene scene = pcv->scene[pcv->window[pcv->winid()].ids];
  for (unsigned int iObj = 0; iObj < scene->listObject.size(); iObj++){
    pCglicObject obj = scene->listObject[iObj];
    if (obj->isSelected()){
      obj->transform.setTranslation(tr);
      state = 1;
    }
  }
  if (state == 0)
    scene->transform.setTranslation(tr);
}

void CglicKeyboard::special(unsigned char key, int x, int y)
{
  pCglicScene scene = pcv->scene[pcv->window[pcv->winid()].ids];

  if(key!=lastKey){
    if (scene->isSelected()){
      scene->saveTransformations();
    }
    for (unsigned int i = 0; i < scene->listObject.size(); i++){
      CglicObject *obj = scene->listObject[i];
      if (obj->isSelected()){
        //if(( !obj->isConstrainedInRotation()) && (!obj->isConstrainedInTranslation()) )
          obj->saveTransformations();
      }
    }
  }


  if(!pcv->profile.flyingMode){
    glm::vec3 moveX = 0.0025f * scene->m_right;
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
        scene->transform.setTranslation(-speed * scene->m_right);
        break;
      case GLUT_KEY_RIGHT:
        scene->transform.setTranslation(speed * scene->m_right);
        break;
      case GLUT_KEY_DOWN:
        scene->transform.setTranslation(-speed * glm::normalize(scene->m_look));
        break;
      case GLUT_KEY_UP:
        scene->transform.setTranslation(speed * glm::normalize(scene->m_look));
        break;
      default:
        break;
    }
  }

  lastKey = key;
}

void CglicKeyboard::keyboard(unsigned char key, int x, int y)
{
  pCglicScene scene = pcv->scene[pcv->window[pcv->winid()].ids];

  // QUIT
  if ( key == 'q' || key == 27 ){
    exit(0);}

  // ZOOM
  if(((key == 'z') || (key == 'Z')) && (lastKey!='t') && (lastKey!='r')){
    if ((glm::length(scene->m_cam)>0.15) && (key=='z'))
      scene->view->zoom *= 0.95;
    else if( (glm::length(scene->m_cam)<1.75) && (key=='Z') )
      scene->view->zoom *= 1.05;
  }

  // BB
  if (key == 'b' ){
    for (unsigned int i = 0; i < scene->listObject.size(); i++)
      scene->listObject[i]->toogleBBox();
  }

  // Wireframe
  if (key == 'm' ){
    for (unsigned int i = 0; i < scene->listObject.size(); i++)
      scene->listObject[i]->toogleMesh();
  }

  //Smooth to flat shading
  //if (key == 'f'){
  //  for (unsigned int i = 0; i < scene->listObject.size(); i++)
  //    if( scene->listObject[i]->isSelected())
  //      scene->listObject[i]->toogleSmooth();
  //}

  //Reset Transformations
  if (key == 'e'){
    if(scene->isSelected()){
      scene->undoLast();
    }
    for(int i = 0 ; i < scene->listObject.size() ; i++){
      pCglicObject obj = scene->listObject[i];
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
    for(int i = 0 ; i < scene->listObject.size() ; i++){
      pCglicObject obj = scene->listObject[i];
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
    for(int i = 0 ; i < scene->listObject.size() ; i++){
      pCglicObject obj = scene->listObject[i];
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
      for(int i = 0 ; i < scene->listObject.size() ; i++){
        pCglicObject obj = scene->listObject[i];
        if(obj->isSelected()){
          obj->saveTransformations();
        }
      }
    }
  }

  //Group objects
  if (key == 'g'){
    std::vector<pCglicObject> objectsToGroup;
    for(int i = 0 ; i < scene->listObject.size() ; i++){
      if(scene->listObject[i]->isSelected()){
        objectsToGroup.push_back(scene->listObject[i]);
      }
    }
    if(objectsToGroup.size()>1){
      scene->listGroup.push_back(new CglicGroup(objectsToGroup));
    }
  }
  //Ungroup
  if (key == 'G'){
    for(int iG = 0 ; iG < scene->listGroup.size() ; iG++){
      if(scene->listGroup[iG]->isSelected()){
        for(int iO = 0 ; iO < scene->listGroup[iG]->listObject.size() ; iO++){
          scene->listGroup[iG]->listObject[iO]->unSelect();
          scene->listGroup[iG]->listObject[iO]->resetGroupID();
        }
        scene->listGroup.erase(scene->listGroup.begin() + iG);
      }
    }
  }

  //Hide
  if (key == 'h'){
    for(int i = 0 ; i < scene->listObject.size() ; i++){
      if(scene->listObject[i]->isSelected()){
        scene->listObject[i]->hide();
        scene->listObject[i]->unSelect();
        scene->select();
      }
    }
  }
  if (key == 'H'){
    for(int i = 0 ; i < scene->listObject.size() ; i++){
      if(scene->listObject[i]->isHidden()){
        scene->listObject[i]->unHide();
      }
    }
  }

  //Camera Presets
  if (key == '1'){
    scene->m_cam = glm::vec3(0,0,1);}
  if (key == '3'){
    scene->m_cam = glm::vec3(1,0,0);}
  if ((key=='7') && (!pcv->profile.flyingMode)){
    scene->m_cam = glm::vec3(0,1,0);
    scene->m_up  = glm::vec3(0,0,-1);
  }
  if ((key == '1') || (key == '3')){
    scene->m_up = glm::vec3(0,1,0);}

  //Ortho view
  if (key == '5'){
    pcv->profile.perspective = !pcv->profile.perspective;}

  //Save
  if(key == 's'){
    for(int i = 0 ; i < scene->listObject.size() ; i++){
      //Ecrire MODEL et nom du fichier dans un file
    }
  }

  //Flying mode
  if(key == 'f'){
    bool camAbove = false;
    if(glm::normalize(scene->m_cam) == glm::vec3(0,1,0) )
      camAbove = true;
    if(!camAbove)
      scene->toogleFlyingMode();
  }

  lastKey = key;

}

void CglicKeyboard::keyboardUp(unsigned char key, int x, int y){}

