#include <cgl/canvas.h>
#include <cgl/window.h>
extern CglCanvas *pcv;
CglWindow *WIN;

CglWindow::CglWindow(){
    pScene  = NULL;
    pView   = NULL;
}
CglWindow::CglWindow(int x, int y, int w, int h){
    WIN = this;
    pScene           = NULL;
    pView            = NULL;
    pLinearInterface = new CglLinearInterface();
    pRadialInterface = new CglRadialInterface();

    pcv->addWindow(this);
    pLinearInterface->init(0.9);
    //pRadialInterface->init(glm::vec2(0.0, 0.0), 0.17);

    position = glm::vec2(x,y);
    size     = glm::vec2(w,h);
    title    = "Window " + to_string(pcv->getWindows().size());

    if(pcv->profile.stereo)
        glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STEREO | GLUT_MULTISAMPLE);
      else
        glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glutInitWindowPosition(position.x, position.y);
    glutInitWindowSize(size.x, size.y);
    ID = glutCreateWindow(title.c_str());
    glutSetWindow(ID);

    // Function callbacks with wrapper functions
    glutReshapeFunc(pcv->reshapeWrap);
    glutDisplayFunc(WIN->displayWrap);
    glutMouseFunc(pcv->mouseWrap);
    glutKeyboardFunc(pcv->keyWrap);
    glutKeyboardUpFunc(pcv->keyUpWrap);
    glutMotionFunc(pcv->motionWrap);
    glutPassiveMotionFunc(pcv->passiveMotionWrap);
    glutSpecialFunc(pcv->specialWrap);
}
CglWindow::~CglWindow(){}

void CglWindow::loadShaders(){
    simpleShader.load("SIMPLE");
    smoothShader.load("SMOOTH");
    flatShader.load("FLAT");
}

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
  float offset    = ( (pcv->profile.stereo) ? pView->m_eyesep / 2 : 0.0f );
  int stereo      = ( (buffer == GL_BACK_LEFT) ? -1 : 1 );

  glDrawBuffer(buffer);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  pView->camOffset = stereo * offset;
  pScene->display();
}






void CglWindow::reshape(int w, int h){
  pView->reshape(w, h);
}
void CglWindow::reshapeWrap(int w, int h) {
  pcv->getWindow()->reshape(w, h);
}

void CglWindow::displayWrap(){
  pcv->getWindow()->display();

  //for(int i = 0 ; i < pcv->getWindows().size() ; i++){
    //glutSetWindow(pcv->getWindow(i)->ID);
    //pcv->getWindow(i)->display();
  //}
  //cout << "active win = " << pcv->getWindow()->ID << endl;
  //WIN->display();
}

void CglWindow::motionWrap(int x, int y){
  pcv->getMouse()->motion(x,y);
  glutPostRedisplay();
}

void CglWindow::passiveMotionWrap(int x, int y){
  pcv->getMouse()->passiveMotion(x,y);
  glutPostRedisplay();
}

void CglWindow::mouseWrap(int b, int s, int x, int y){
  pcv->getMouse()->mouse(b, s, x, y);
  glutPostRedisplay();
}

void CglWindow::keyWrap(unsigned char key, int x, int y){
  pcv->getKeyboard()->keyboard(key, x, y);
  glutPostRedisplay();
}

void CglWindow::keyUpWrap(unsigned char key, int x, int y){
  pcv->getKeyboard()->keyboardUp(key, x, y);
  glutPostRedisplay();
}

void CglWindow::specialWrap(int key, int x, int y){
  pcv->getKeyboard()->special(key, x, y);
  glutPostRedisplay();
}








CglSubWindow::CglSubWindow(pCglWindow window, int x, int y, int w, int h){
    WIN = this;
    pScene           = NULL;
    pView            = NULL;
    pLinearInterface = new CglLinearInterface();
    pRadialInterface = new CglRadialInterface();

    pcv->addWindow(this);
    pLinearInterface->init(0.9);
    //pRadialInterface->init(glm::vec2(0.0, 0.0), 0.17);

    position = glm::vec2(x,y);
    size     = glm::vec2(w,h);
    title    = "Window " + to_string(pcv->getWindows().size());

    if(pcv->profile.stereo)
        glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STEREO | GLUT_MULTISAMPLE);
      else
        glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glutInitWindowPosition(position.x, position.y);
    glutInitWindowSize(size.x, size.y);
    ID = glutCreateSubWindow(window->ID, position.x, position.y, size.x, size.y);
    glutSetWindow(ID);

    // Function callbacks with wrapper functions
    glutReshapeFunc(pcv->reshapeWrap);
    glutDisplayFunc(WIN->displayWrap);
    glutMouseFunc(pcv->mouseWrap);
    glutKeyboardFunc(pcv->keyWrap);
    glutKeyboardUpFunc(pcv->keyUpWrap);
    glutMotionFunc(pcv->motionWrap);
    glutPassiveMotionFunc(pcv->passiveMotionWrap);
    glutSpecialFunc(pcv->specialWrap);
}



