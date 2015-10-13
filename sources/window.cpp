#include <cgl/canvas.h>
#include <cgl/window.h>
extern CglCanvas *pcv;

CglWindow::CglWindow(){
    id   = -1;
    pScene  = NULL;
}
CglWindow::CglWindow(int x, int y, int w, int h){
    position = glm::vec2(x,y);
    size     = glm::vec2(w,h);
    title    = "Window " + to_string(id);

    glutInitWindowPosition(position.x, position.y);
    glutInitWindowSize(size.x, size.y);
    id       = glutCreateWindow(title.c_str());

    // Function callbacks with wrapper functions
    glutReshapeFunc(pcv->reshapeWrap);
    glutDisplayFunc(pcv->displayWrap);
    glutMouseFunc(pcv->mouseWrap);
    glutKeyboardFunc(pcv->keyWrap);
    glutKeyboardUpFunc(pcv->keyUpWrap);
    glutMotionFunc(pcv->motionWrap);
    glutPassiveMotionFunc(pcv->passiveMotionWrap);
    glutSpecialFunc(pcv->specialWrap);
}
CglWindow::~CglWindow(){}


void CglWindow::display(){
  if(pcv->profile.stereo){
    displayBuffer(GL_BACK_LEFT);
    displayBuffer(GL_BACK_RIGHT);
  }
  else
    displayBuffer(GL_BACK_LEFT);
  glutSwapBuffers();
}

void CglWindow::displayBuffer(int buffer){
  pCglScene scene = pcv->getScene(ids);
  float offset    = ( (pcv->profile.stereo) ? scene->getView()->m_eyesep / 2 : 0.0f );
  int stereo      = ( (buffer == GL_BACK_LEFT) ? -1 : 1 );

  glDrawBuffer(buffer);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  scene->getView()->camOffset = stereo * offset;
  scene->display();
}



int CglWindow::cglAddLight(pCglLight li){
  light.push_back(li);
  return light.size() - 1;
}


