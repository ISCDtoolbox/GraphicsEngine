#include <cgl/canvas.h>
#include <cgl/window.h>

extern CglCanvas *pcv;
CglWindow        *WIN;
CglSubWindow     *SUBWIN;

void updateWINPointer(){
    int currentWindow = glutGetWindow();
    std::vector<pCglWindow> windows = pcv->getWindows();
    for(int i = 0 ; i < windows.size() ; i++)
        if (windows[i]->ID == currentWindow)
            WIN = windows[i];
}

void updateSUBWINPointer(){
    int currentWindow = glutGetWindow();
    std::vector<pCglSubWindow> subWindows = WIN->subWindows;
    for(int j = 0 ; j < subWindows.size() ; j++){
        if(subWindows[j]->ID == currentWindow)
            SUBWIN = subWindows[j];
    }
}


CglWindow::CglWindow(){}
CglWindow::CglWindow(int x, int y, int w, int h){
    pcv->addWindow(    this);
    position         = glm::vec2(x,y);
    size             = glm::vec2(w,h);
    title            = "Window " + to_string(pcv->getWindows().size());



    glutInitWindowPosition( position.x, position.y);
    glutInitWindowSize(     size.x, size.y);
    ID = glutCreateWindow(  title.c_str());
    glutSetWindow(          ID);



    WIN = this;



    // Function callbacks with wrapper functions
    glutIdleFunc(   WIN->idleWrap);
    glutReshapeFunc(this->reshapeWrap);
    //glutDisplayFunc(this->displayWrap);
    cout << "Main Window ID = " << ID << endl;
}
CglWindow::~CglWindow(){}

void CglWindow::reshape(int w, int h){
    size = glm::vec2(w,h);
    for(int i = 0 ; i < subWindows.size() ; i++)
        subWindows[i]->reshape(w,h);
}
void CglWindow::reshapeWrap(int w, int h){
    updateWINPointer();
    WIN->reshape(w, h);
    //glutPostWindowRedisplay(WIN->ID);
    glutPostRedisplay();
}
void CglWindow::displayWrap(){
    //updateWINPointer();
    //glutSetWindow(WIN);
    //WIN->reshape(w, h);
    //WIN->getLinearInterface()->display();
    //pcv->getInterface()->display();
    //glutPushWindow();
    //glutPostRedisplay();
}
void CglWindow::display(){
    //updateWINPointer();
    //WIN->reshape(w, h);
    //pcv->getInterface()->display();
}


void CglWindow::idleWrap(){
    WIN->FPS.frame++;
	WIN->FPS.time=glutGet(GLUT_ELAPSED_TIME);
	if (WIN->FPS.time - WIN->FPS.lastTime > 1000) {
		float fps = WIN->FPS.frame * 1000.0 / (WIN->FPS.time - WIN->FPS.lastTime);
	 	WIN->FPS.lastTime = WIN->FPS.time;
		WIN->FPS.frame = 0;
		cout << "FPS: " << fps << ",   \tUnit = ";
		pcv->getSubWindow()->pScene->debug();
		cout << " m." << endl;
		//pcv->PROF.print();
	}
}

pCglSubWindow CglWindow::getSubWindow(){
    updateSUBWINPointer();
    return SUBWIN;
}









void CglSubWindow::loadShaders(){
    simpleShader.load("SIMPLE");
    smoothShader.load("SMOOTH");
    flatShader.load("FLAT");
    fresnelShader.load("FRESNEL");
}

void CglSubWindow::display(){
    bool debug = false;
    if(debug){
        cout << " - Active Subwindow        = " << ID << " vs " << glutGetWindow() << endl;
        cout << " - Active Subwindow adress = " << this << endl;
        cout << " -- SUBWIN variable  = " << SUBWIN << endl;
        cout << " -- SubScene  adress = " << getScene() << endl;
        cout << endl;
    }

    if(pcv->profile.stereo){
        displayBuffer(GL_BACK_LEFT);
        displayBuffer(GL_BACK_RIGHT);
    }
    else
        displayBuffer(GL_BACK_LEFT);

    glutSwapBuffers();
}

void CglSubWindow::displayBuffer(int buffer){
  float offset    = ( (pcv->profile.stereo) ? pView->m_eyesep / 2 : 0.0f );
  int stereo      = ( (buffer == GL_BACK_LEFT) ? -1 : 1 );


    glDrawBuffer(buffer);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  pView->camOffset = stereo * offset;
  pScene->display();

  pcv->getInterface()->display();
}

void CglSubWindow::compute_window(int w, int h){
    //Etape de redimensionnement
    int W = parent->size.x;
    int H = parent->size.y;
    if (location==CGL_LEFT){
        position = glm::vec2(0,0);
        size     = glm::vec2(ratio * W, H);
    }
    if(location==CGL_RIGHT){
        position = glm::vec2((1-ratio) * W,0);
        size     = glm::vec2(ratio * W, H);
    }
    if(location==CGL_TOP){
        position = glm::vec2(0,0);
        size     = glm::vec2(W, ratio * H);
    }
    if(location==CGL_BOTTOM){
        position = glm::vec2(0,(1-ratio) * H);
        size     = glm::vec2(W,ratio * H);
    }
    if(location==CGL_FULL){
        position = glm::vec2(0,0);
        size     = glm::vec2(W,H);
    }
    if(location==CGL_CUSTOM){}

    if (size.x<10)
        size.x = 10;
    if (size.y < 100)
        size.y = 100;
    pView->reshape(size.x, size.y);
}

void CglSubWindow::reshapeWrap(int w, int h){
    updateSUBWINPointer();
    SUBWIN->reshape(w, h);
    glutPostWindowRedisplay(SUBWIN->ID);

}
void CglSubWindow::reshape(int w, int h){
    compute_window(     w,h);
    glutSetWindow(      ID);
    glutPositionWindow( position.x, position.y);
    glutReshapeWindow(  size.x, size.y);
    pView->reshape(     size.x, size.y);
}

void CglSubWindow::displayWrap(){
    updateSUBWINPointer();
    SUBWIN->display();
    //glutPostWindowRedisplay(SUBWIN->ID);
}

void CglSubWindow::setCallbacks(){
    SUBWIN = this;
    // Function callbacks with wrapper functions
    glutReshapeFunc(      this->reshapeWrap);
    glutDisplayFunc(      this->displayWrap);

    glutMouseFunc(        pcv->mouseWrap);
    glutKeyboardFunc(     pcv->keyWrap);
    glutKeyboardUpFunc(   pcv->keyUpWrap);
    glutMotionFunc(       pcv->motionWrap);
    glutPassiveMotionFunc(pcv->passiveMotionWrap);
    glutSpecialFunc(      pcv->specialWrap);
}


CglSubWindow::CglSubWindow(pCglWindow window, CGL_SUBWINDOW loc, float R){
    location         = loc;
    parent           = window;
    ratio            = R;

    parent->subWindows.push_back(this);
    pScene           = NULL;
    pView            = new CglView();

    pLinearInterface = new CglLinearInterface();
    pRadialInterface = new CglRadialInterface();
    pLinearInterface->init(0.9);
    //pRadialInterface->init(glm::vec2(0.0, 0.0), 0.17);

    if(pcv->profile.stereo)
        glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STEREO | GLUT_MULTISAMPLE  | GLUT_STENCIL);
      else
        glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE | GLUT_STENCIL);

    glutInitWindowPosition(position.x, position.y);
    glutInitWindowSize(size.x, size.y);

    compute_window(parent->size.x, parent->size.y);
    ID       = glutCreateSubWindow(window->ID, position.x, position.y, size.x, size.y);

    glutSetWindow(ID);
    setCallbacks();
    cout << "SubWindow " << ID << " created." << endl;

}




