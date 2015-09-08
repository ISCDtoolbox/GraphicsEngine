#include <cgl/canvas.h>
#include <cgl/window.h>

extern CglCanvas *pcv;


CglWindow::CglWindow(): m_id(-1) /*scene(NULL)*/
{
}


CglWindow::CglWindow(int x, int y, int w, int h)
{
  cout << " - [create window " << w << " " << h << "]" << endl;
  this->m_wpos[0] = x;
  this->m_wpos[1] = y;
  this->m_wsiz[0] = w;
  this->m_wsiz[1] = h;
  //view.setPos(0.0, 0.0, 1.0);
}


CglWindow::~CglWindow()
{
  cout << " - [destroy window]" << endl;
}


void CglWindow::show()
{
  bool VBOs = false;

  cout << " - [open window]" << endl;

  glutInitWindowPosition(m_wpos[0], m_wpos[1]);
  glutInitWindowSize(m_wsiz[0], m_wsiz[1]);
  m_id = glutCreateWindow("essai");

  // Function callbacks with wrapper functions
  glutReshapeFunc(pcv->reshapeWrap);
  glutDisplayFunc(pcv->displayWrap);
  glutMouseFunc(pcv->mouseWrap);
  glutKeyboardFunc(pcv->keyWrap);
  glutKeyboardUpFunc(pcv->keyUpWrap);
  glutMotionFunc(pcv->motionWrap);
  glutPassiveMotionFunc(pcv->passiveMotionWrap);
  glutSpecialFunc(pcv->specialWrap);

  /* basic openGL calls */
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  //glPolygonOffset(1.0, 1.0 / (float)0x10000);
  //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  //glShadeModel(GL_SMOOTH);
  //glDisable(GL_NORMALIZE);
  //glDisable(GL_LINE_SMOOTH);
  //glDisable(GL_POINT_SMOOTH);
  //glEnable(GL_DITHER);
  //glDisable(GL_CULL_FACE);

  //view.setView();
}


void CglWindow::display()
{
  pCglScene scene = pcv->getScene(ids);
  glm::vec3 col = pcv->profile.back_color;

  //STEREO
  if(pcv->profile.stereo){
    //LEFT
    glDrawBuffer(GL_BACK_LEFT);
    glClearColor(col.x, col.y, col.z, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    scene->getView()->camOffset = scene->getView()->m_eyesep / 2;
    scene->display();
    //RIGHT
    glDrawBuffer(GL_BACK_RIGHT);
    glClearColor(col.x, col.y, col.z, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    scene->getView()->camOffset = -scene->getView()->m_eyesep / 2;
    scene->display();
  }

  else{
    glDrawBuffer(GL_BACK_RIGHT);
    glClearColor(col.x, col.y, col.z, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    scene->getView()->camOffset = 0.0f;
    scene->display();
  }


  glutSwapBuffers();
}

int CglWindow::cglAddLight(pCglLight li)
{
  light.push_back(li);
  return light.size() - 1;
}


