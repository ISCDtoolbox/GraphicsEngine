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

    if(pcv->profile.stereo)
        glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STEREO | GLUT_MULTISAMPLE);
      else
        glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);

    glutInitWindowPosition( position.x, position.y);
    glutInitWindowSize(     size.x, size.y);
    ID = glutCreateWindow(  title.c_str());
    glutSetWindow(          ID);

    //pLinearInterface->init( 0.9);
    //pRadialInterface->init(glm::vec2(0.0, 0.0), 0.17);

    // Function callbacks with wrapper functions

    glutReshapeFunc(        this->reshapeWrap);
    glutDisplayFunc(        this->displayWrap);
    WIN = this;


    glutMouseFunc(          pcv->mouseWrap);
    glutKeyboardFunc(       pcv->keyWrap);
    glutKeyboardUpFunc(     pcv->keyUpWrap);
    glutMotionFunc(         pcv->motionWrap);
    glutPassiveMotionFunc(  pcv->passiveMotionWrap);
    glutSpecialFunc(        pcv->specialWrap);


    cout << "Main Window ID = " << ID << endl;
}
CglWindow::~CglWindow(){}

void CglWindow::reshape(int w, int h){
    size = glm::vec2(w,h);
    for(int i = 0 ; i < subWindows.size() ; i++){
        //subWindows[i]->display();
        subWindows[i]->reshape(w,h);

    }

    //cout << ID << ": " << size.x << "/" << size.y << " - " << position.x << "/" << position.y << endl;
}
void CglWindow::reshapeWrap(int w, int h){
    updateWINPointer();
    WIN->reshape(w, h);
    glutPostWindowRedisplay(WIN->ID);
    glutPostRedisplay();
}

void CglWindow::displayWrap(){
    updateWINPointer();
    //cout << "Displaying main window: " << glutGetWindow() << endl;
    WIN->display();
    glutPostWindowRedisplay(WIN->ID);
}
void CglWindow::display(){

    //cout << "-- " << glutGetWindow() << " / " << ID << endl;
    //for(int i = 0 ; i < subWindows.size() ; i ++)
    //    subWindows[i]->display();

    //Seulement l'active
    //if(getSubWindow()!=NULL)
    //    getSubWindow()->display();
    //Toutes les fenêtres
    //SUBWIN->reshape(size.x, size.y);
}

pCglSubWindow CglWindow::getSubWindow(){
    updateSUBWINPointer();
    /*
    int idw = glutGetWindow();
    for (int i=0; i<subWindows.size(); i++)
        if ( subWindows[i]->ID == idw )
            return subWindows[i];
    cout << "No active sub window for ID = " << idw << endl;
    return NULL;
    */
    return SUBWIN;
    //exit(-1);
}


void CglSubWindow::loadShaders(){
    simpleShader.load("SIMPLE");
    smoothShader.load("SMOOTH");
    flatShader.load("FLAT");
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
}

void CglSubWindow::compute_window(int w, int h){
    //Etape de redimensionnement
    int W = parent->size.x;
    int H = parent->size.y;
    if (location==CGL_LEFT){
        position = glm::vec2(0,0);
        size     = glm::vec2(W/2, H);
    }
    if(location==CGL_RIGHT){
        position = glm::vec2(W/2,0);
        size     = glm::vec2(W/2, H);
    }
    if(location==CGL_TOP){
        position = glm::vec2(0,0);
        size     = glm::vec2(W, H/2);
    }
    if(location==CGL_BOTTOM){
        position = glm::vec2(0,H/2);
        size     = glm::vec2(W,H/2);
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
    compute_window(w,h);
    glutSetWindow(ID);
    glutPositionWindow(position.x, position.y);
    glutReshapeWindow(size.x, size.y);
    pView->reshape(size.x, size.y);
}

void CglSubWindow::displayWrap(){
    updateSUBWINPointer();
    SUBWIN->display();
    glutPostWindowRedisplay(SUBWIN->ID);

}



/*
void CglSubWindow::motionWrap(int x, int y){
    updateSUBWINPointer();
    pcv->getMouse()->motion(x,y);
    glutPostWindowRedisplay(SUBWIN->ID);
}

void CglSubWindow::passiveMotionWrap(int x, int y){
    updateSUBWINPointer();
    pcv->getMouse()->passiveMotion(x,y);
    glutPostWindowRedisplay(SUBWIN->ID);
}

void CglSubWindow::mouseWrap(int b, int s, int x, int y){
    updateSUBWINPointer();
    pcv->getMouse()->mouse(b, s, x, y);
    glutPostWindowRedisplay(SUBWIN->ID);
}

void CglSubWindow::keyWrap(unsigned char key, int x, int y){
    updateSUBWINPointer();
    pcv->getKeyboard()->keyboard(key, x, y);
    glutPostWindowRedisplay(SUBWIN->ID);
}

void CglSubWindow::keyUpWrap(unsigned char key, int x, int y){
    updateSUBWINPointer();
    pcv->getKeyboard()->keyboardUp(key, x, y);
    glutPostWindowRedisplay(SUBWIN->ID);
}

void CglSubWindow::specialWrap(int key, int x, int y){
    updateSUBWINPointer();
    pcv->getKeyboard()->special(key, x, y);
    glutPostWindowRedisplay(SUBWIN->ID);
}



void CglSubWindow::idle(){
    SUBWIN->idleCallback();
}
void CglSubWindow::idleCallback(){

    glutSetWindow(parent->ID);
    glutPostRedisplay();
    int cur = glutGetWindow();

    std::vector<pCglSubWindow> SUBS = parent->subWindows;
    for(int i = 0 ; i < SUBS.size() ; i ++){
        pCglSubWindow sub = SUBS[i];
        if(sub->ID!=cur){
            glutSetWindow(sub->ID);
            glutPostRedisplay();
        }
    }

    glutSetWindow(cur);

}
*/



void CglSubWindow::setCallbacks(){
    SUBWIN = this;
    // Function callbacks with wrapper functions
    glutReshapeFunc(      this->reshapeWrap);
    glutDisplayFunc(      this->displayWrap);
    //glutIdleFunc(       this->idle);


    glutMouseFunc(        pcv->mouseWrap);
    glutKeyboardFunc(     pcv->keyWrap);
    glutKeyboardUpFunc(   pcv->keyUpWrap);
    glutMotionFunc(       pcv->motionWrap);
    glutPassiveMotionFunc(pcv->passiveMotionWrap);
    glutSpecialFunc(      pcv->specialWrap);


}


CglSubWindow::CglSubWindow(pCglWindow window, CGL_SUBWINDOW loc){
    location         = loc;
    parent           = window;
    parent->subWindows.push_back(this);

    pScene           = NULL;
    pView            = new CglView();
    pLinearInterface = new CglLinearInterface();
    pRadialInterface = new CglRadialInterface();
    pLinearInterface->init(0.9);
    //pRadialInterface->init(glm::vec2(0.0, 0.0), 0.17);
    if(pcv->profile.stereo)
        glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STEREO | GLUT_MULTISAMPLE);
      else
        glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);

    glutInitWindowPosition(position.x, position.y);
    glutInitWindowSize(size.x, size.y);

    compute_window(parent->size.x, parent->size.y);
    ID       = glutCreateSubWindow(window->ID, position.x, position.y, size.x, size.y);

    glutSetWindow(ID);
    setCallbacks();
    cout << "SubWindow " << ID << " created." << endl;
}




CglSubWindow::CglSubWindow(pCglWindow window, int x, int y, int w, int h){
    location         = CGL_CUSTOM;
    parent           = window;

    parent->subWindows.push_back(this);
    SUBWIN           = this;
    pScene           = NULL;
    pView            = new CglView();
    pLinearInterface = new CglLinearInterface();
    pRadialInterface = new CglRadialInterface();

    pLinearInterface->init(0.9);
    //pRadialInterface->init(glm::vec2(0.0, 0.0), 0.17);

    if( pcv->profile.stereo )
        glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STEREO | GLUT_MULTISAMPLE);
      else
        glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);

    position = glm::vec2(x,y);
    size     = glm::vec2(w,h);

    reshape(parent->size.x, parent->size.y);

    glutInitWindowPosition(position.x, position.y);
    glutInitWindowSize(size.x, size.y);
    ID       = glutCreateSubWindow(window->ID, position.x, position.y, size.x, size.y);
    glutSetWindow(ID);

    setCallbacks();
}



