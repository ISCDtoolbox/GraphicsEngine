#include <cgl/canvas.h>
#include <cgl/mouse.h>
#include <cgl/interface.h>
extern CglCanvas *pcv;

CglMouse::CglMouse(){
  memset((void*)this, 0, sizeof(*this));
  m_button[0] = m_button[1] = m_button[2] = false;
  //m_key = TM_NONE;
  isPressed      = isReleased = false;
  arcball        = false;
  currPos        = glm::vec2(0);
  lastPos        = glm::vec2(0);
  lastPassivePos = glm::vec2(0);
  m_pos          = glm::vec3(0);
}

//void CglMouse::setGUI(){
//  buttons = pcv->getInterface()->getButtonList();
//}

glm::vec3 get_arcball_vector(glm::vec2 cursor) {
  int W = pcv->getSubWindow()->pView->width;
  int H = pcv->getSubWindow()->pView->height;
  glm::vec3 P = glm::vec3(1.0*cursor.x/W*2 - 1.0,
                          1.0*cursor.y/H*2 - 1.0,
                          0);
  P.y = -P.y;
  float OP_squared = P.x*P.x + P.y*P.y;
  if (OP_squared <= 1*1)
    P.z = sqrt(1*1 - OP_squared);  // Pythagore
  else
    P = glm::normalize(P);         // nearest point
  return P;
}

void CglMouse::motion(int x, int y)
{
  pCglScene scene         = pcv->getScene();
  bool unactive_interface = !pcv->getInterface()->isActive();
  currPos                 = glm::vec2(x,y);
  bool mvt                = (currPos != lastPos);
  glm::vec2 d             = currPos - lastPos;
  scene->onDrag(x,y);

  if ( mvt && unactive_interface ){
    if(m_button[0])
      scene->onLeftDrag(x,y);//d.x, d.y);
    if(m_button[1])
      scene->onMiddleDrag(x, y);
    if(m_button[2])
      scene->onRightDrag(x,y);
  }

  lastPos = currPos;
}


void CglMouse::passiveMotion(int x, int y){
  bool insideWindow = (x<pcv->getWindow()->size.x) &&
                      (x>0) &&
                      (y<pcv->getWindow()->size.y) &&
                      (y>0);
  if(insideWindow){
  pCglScene scene = pcv->getScene();
  //cout << "Scene adress = " << scene << endl;
  glm::vec2 currPassivePos(x,y);

  if (currPassivePos != lastPassivePos) {
    glm::vec2 d = currPassivePos - lastPassivePos;
    lastPassivePos = currPassivePos;

    if(pcv->profile.flyingMode){
      glm::mat4 LOOKROT = glm::mat4(    glm::angleAxis(0.002f * d.x, glm::vec3(0,1,0)) *
                                        glm::angleAxis(0.002f * d.y, scene->getRight()));
      if (scene->isSelected())
        scene->getTransform()->setRotation(LOOKROT);
    glutWarpPointer(scene->getView()->width/2, scene->getView()->height/2);
    }


    //else{
      for (unsigned int i = 0; i < scene->getObjectList()->size(); i++){
        pCglObject obj = scene->getObject(i);
        if (obj->isSelected()){
          if(obj->isConstrainedInRotation())
            obj->setConstrainedRotation(5.0f * d.y);
          else if(obj->isConstrainedInTranslation())
            obj->setConstrainedTranslation(1.0f * (d.y));
        }
      }
      //}

  }
  currPos = glm::vec2(x,y);

  //Check hovered buttons

  pcv->getInterface()->checkHoveredButtons(x, y);
  }
}


void CglMouse::mouse(int b, int s, int x, int y)
{
  pCglScene scene = pcv->getScene();
  isPressed  = (s == GLUT_DOWN);
  isReleased = (s == GLUT_UP  );

  if(isReleased)
    scene->onRelease(x,y);
  if(isPressed)
    scene->onClick(x,y);

  m_button[0] = m_button[1] = m_button[2] = false;
  switch(b){
    case GLUT_LEFT_BUTTON:
      m_button[0] = ((GLUT_DOWN==s)?1:0);
      if(s == GLUT_UP)
	scene->onLeftRelease(x, y);
      if(s == GLUT_DOWN)
	scene->onLeftClick(x, y);
      break;
    case GLUT_MIDDLE_BUTTON:
      m_button[1] = ((GLUT_DOWN==s)?1:0);
      if(s == GLUT_UP)
	scene->onMiddleRelease(x, y);
      if(s == GLUT_DOWN)
	scene->onMiddleClick(x, y);
      break;
    case GLUT_RIGHT_BUTTON:
      m_button[2] = ((GLUT_DOWN==s)?1:0);
      if(s == GLUT_UP)
	scene->onRightRelease(x, y);
      if(s == GLUT_DOWN)
	scene->onRightClick(x, y);
      break;
  }

  lastPos = currPos = glm::vec2(x,y);
  pcv->getInterface()->checkClickedButtons(b, s, x, y);
}



