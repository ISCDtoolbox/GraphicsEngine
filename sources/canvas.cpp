#include <cgl/mesh.h>
#include <cgl/canvas.h>
using namespace std;
CglCanvas *pcv;


///////////////////////////////////////////////////////////////////////////
//Constructor

CglCanvas::CglCanvas(int argc, char **argv){
  pcv = this;
  glutInit(&argc, argv);
  cout << "Canvas created" << endl;
}

void CglCanvas::initGLEW(){
#ifndef __APPLE__
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK){
        cout << "glewinit impossible" << endl;
        exit(1);
    }
    if (!GLEW_VERSION_2_1){
        cout << "Version 2.1 pas supportée" << endl;
        exit(1);
    }
#endif
}


///////////////////////////////////////////////////////////////////////////
//Accessors

pCglLight     CglCanvas::getLight( int lightID){  return lights[lightID];}
pCglScene     CglCanvas::getScene( int sceneID){  return scenes[sceneID]; }
pCglScene     CglCanvas::getScene(){
    return getSubWindow()->getScene();
}


pCglInterface CglCanvas::getInterface(){
  if(profile.interface == CGL_INTERFACE_LINEAR)
    return getSubWindow()->getLinearInterface();//linearInterface;
  else if(profile.interface == CGL_INTERFACE_RADIAL)
    return getSubWindow()->getRadialInterface();//&radialInterface;
}

void       CglCanvas::centerMouse(){
  mice.lastPassivePos = mice.lastPos = glm::vec2(pcv->getScene()->getView()->width/2, pcv->getScene()->getView()->height/2);
}

pCglWindow CglCanvas::getWindow(){
    int idw = glutGetWindow();
    for (int i=0; i<windows.size(); i++){
        for(int j = 0 ; j < windows[i]->subWindows.size() ; j++)
            if ( windows[i]->subWindows[j]->ID == idw )
                return windows[i];
    }
    for (int i=0; i<windows.size(); i++)
        if ( windows[i]->ID == idw )
            return windows[i];
    cout << "No active window found for ID = " << glutGetWindow() << endl;// exit(-1);
    exit(-1);
}

pCglSubWindow CglCanvas::getSubWindow(){
    int idw = glutGetWindow();
    for (int i=0; i<windows.size(); i++)
        for(int j = 0 ; j < windows[i]->subWindows.size() ; j++)
            if ( windows[i]->subWindows[j]->ID == idw )
                return windows[i]->subWindows[j];
    //cout << "No active sub window found for ID = " << glutGetWindow() << endl;// exit(-1);
    return NULL;
    //exit(-2);
}

//pCglWindow CglCanvas::getWindow(int windowID){ return windows[windowID];}


///////////////////////////////////////////////////////////////////////////
//Glut wraps

/*

void CglCanvas::reshapeWrap(int w, int h) {
  pcv->reshape(w, h);
}
void CglCanvas::reshape(int w, int h){
    if (getWindow()!=NULL)
        getWindow()->reshape(w, h);
}


void CglCanvas::displayWrap(){
  pcv->display();
}
void CglCanvas::display(){
    //cout << "Displaying canvas" << endl;
    //getWindow()->display();
  //for(int i = 0 ; i < windows.size() ; i++){
    //glutSetWindow(getWindow()->ID);
    //windows[i]->display();
  //}
  //cout << "active win = " << getWindow()->ID << endl;
}
*/


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



void CglCanvas::loop(){
    glutMainLoop();
}






