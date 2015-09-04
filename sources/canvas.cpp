#include <cgl/canvas.h>

CglCanvas *pcv;

CglCanvas::CglCanvas(int argc, char **argv){
  pcv = this;

  glutInit(&argc, argv);

  if(pcv->profile.stereo)
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STEREO);
  else
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

  cglLighting();

  //Construct profile from a configuration file
  profile = CglProfile();
}


int CglCanvas::cglWindow(int x, int y, int w, int h){
  window.push_back(CglWindow(x,y,w,h));
  return window.size()-1;
}


int CglCanvas::cglScene(){
  scene.push_back(new CglScene());
  scene[scene.size()-1]->ids = scene.size()-1;
  cout << "scene.size()-1: " << scene.size()-1 << endl;
  return scene.size()-1;
}

void CglCanvas::cglSetScene(int ids, int idw){
  window[idw].ids = ids;
  scene[ids]->view = &window[idw].view;
}


void CglCanvas::cglLighting(){
  light.push_back(CglLight(glm::vec3(10,1,10),  glm::vec3(1,1,0.9),   glm::vec3(0.2, 0.65, 0.15), 200.0, 5.0, true));   //FILL
  light.push_back(CglLight(glm::vec3(-10,-10,1), glm::vec3(1,0.9,0.9), glm::vec3(0.2, 0.65, 0.15), 50.0,  5.0, true));   //SIDE
  light.push_back(CglLight(glm::vec3(1,1,-10), glm::vec3(1,1,0.9),   glm::vec3(0.2, 0.45, 0.35), 100.0, 2.0, true));   //BACK
}

int CglCanvas::cglObject(pCglObject obj){
  object.push_back(obj);
  return object.size() - 1;
}

void CglCanvas::cglSetObject(int ido, int ids){
  scene[ids]->addObject(object[ido]);
}

int CglCanvas::winid(){
  int idw = glutGetWindow();
  for (int i=0; i<window.size(); i++) {
    if ( window[i].m_id == idw )  return(i);
  }
  return(0);
}

void CglCanvas::reshape(int w, int h){
  int  idw = winid();
  window[idw].view.reshape(w, h);
}

void CglCanvas::reshapeWrap(int w, int h) {
  pcv->reshape(w, h);
}

void CglCanvas::display(){
  int  idw = winid();
  window[idw].display();
  window[idw].view.setView();
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
  glutMainLoop();
}

