#include <cgl/canvas.h>
CglCanvas *pcv;


///////////////////////////////////////////////////////////////////////////
//Constructor

CglCanvas::CglCanvas(int argc, char **argv){
  pcv = this;
  glutInit(&argc, argv);

  if(pcv->profile.stereo)
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STEREO | GLUT_MULTISAMPLE);
  else
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);

  cglSetLights();
  profile = CglProfile();
}


///////////////////////////////////////////////////////////////////////////
//Windows, scene ... creation

int CglCanvas::cglWindow(int x, int y, int w, int h){
  window.push_back(CglWindow(x,y,w,h));
  return window.size()-1;
}

int CglCanvas::cglScene(){
  scene.push_back(new CglScene());
  scene[scene.size()-1]->setID(scene.size()-1);
  cout << "scene.size()-1: " << scene.size()-1 << endl;
  return scene.size()-1;
}

int CglCanvas::cglObject(pCglObject obj){
  object.push_back(obj);
  return object.size() - 1;
}

void CglCanvas::cglSetLights(){
  light.push_back(CglLight(glm::vec3(10,1,10),   glm::vec3(1,1,0.9),   glm::vec3(0.2, 0.65, 0.15), 200.0, 5.0, true));   //FILL
  light.push_back(CglLight(glm::vec3(-10,-10,1), glm::vec3(1,0.9,0.9), glm::vec3(0.2, 0.65, 0.15), 50.0,  5.0, true));   //SIDE
  light.push_back(CglLight(glm::vec3(1,1,-10),   glm::vec3(1,1,0.9),   glm::vec3(0.2, 0.45, 0.35), 100.0, 2.0, true));   //BACK
}

void CglCanvas::cglSetScene(int ids, int idw){
  window[idw].ids = ids;
  scene[ids]->setView(&(window[idw].view));
}

void CglCanvas::cglSetObject(int ido, int ids){
  scene[ids]->addObject(object[ido]);
}


///////////////////////////////////////////////////////////////////////////
//Accessors

pCglLight     CglCanvas::getLight( int lightID){  return &(light[lightID]);}
pCglScene     CglCanvas::getScene( int sceneID){  return scene[sceneID]; }
pCglWindow    CglCanvas::getWindow(int windowID){ return &(window[windowID]);}
pCglScene     CglCanvas::getScene(){     return scene[window[winid()].ids]; }
pCglWindow    CglCanvas::getWindow(){    return &(window[winid()]);}
pCglInterface CglCanvas::getInterface(){ return &interface;}
pCglMouse     CglCanvas::getMouse(){     return &mice;}
int           CglCanvas::simpleID(){     return simpleShader.mProgramID;}
int           CglCanvas::smoothID(){     return smoothShader.mProgramID;}
int           CglCanvas::flatID(){       return flatShader.mProgramID;}

void       CglCanvas::centerMouse(){
  mice.lastPassivePos = mice.lastPos = glm::vec2(pcv->getScene()->getView()->width/2, pcv->getScene()->getView()->height/2);
}
int CglCanvas::winid(){
  int idw = glutGetWindow();
  for (int i=0; i<window.size(); i++) {
    if ( window[i].m_id == idw )  return(i);
  }
  return(0);
}


///////////////////////////////////////////////////////////////////////////
//Virtual methods

void CglCanvas::reshape(int w, int h){
  int  idw = winid();
  window[idw].view.reshape(w, h);
}

void CglCanvas::display(){
  int  idw = winid();
  window[idw].display();
  window[idw].view.setView();
}


///////////////////////////////////////////////////////////////////////////
//Glut wraps

void CglCanvas::reshapeWrap(int w, int h) {
  pcv->reshape(w, h);
}

void CglCanvas::displayWrap(){
  pcv->display();
}

void CglCanvas::motionWrap(int x, int y){
  pcv->mice.motion(x,y);
  glutPostRedisplay();
}

void CglCanvas::passiveMotionWrap(int x, int y){
  pcv->mice.passiveMotion(x,y);
  glutPostRedisplay();
}

void CglCanvas::mouseWrap(int b, int s, int x, int y){
  pcv->mice.mouse(b, s, x, y);
  glutPostRedisplay();
}

void CglCanvas::keyWrap(unsigned char key, int x, int y){
  pcv->keyboard.keyboard(key, x, y);
  glutPostRedisplay();
}

void CglCanvas::keyUpWrap(unsigned char key, int x, int y){
  pcv->keyboard.keyboardUp(key, x, y);
  glutPostRedisplay();
}

void CglCanvas::specialWrap(int key, int x, int y){
  pcv->keyboard.special(key, x, y);
  glutPostRedisplay();
}


void CglCanvas::cglMainLoop(){
  simpleShader.load("SIMPLE");
  smoothShader.load("SMOOTH");
  flatShader.load("FLAT");
  interface.init(5);
  mice.setGUI();
  glutMainLoop();
}

