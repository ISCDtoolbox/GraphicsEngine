#include <cgl/view.h>
#include "glm/gtx/string_cast.hpp"
#include <cgl/canvas.h>
extern CglCanvas *pcv;


CglView::CglView()
{
  m_stereo = false;
  m_eyesep = 0.027;
  setPersp(70.0, 0.001, 15.0);
  zoom     = 1.0f;
  camOffset= 0.0f;
  ratio    = 1;
}

void CglView::setPersp(double fovy, double znear, double zfar)
{
  m_fovy  = fovy;
  m_znear = znear;
  m_zfar  = zfar;
}


void CglView::reshape(int w, int h)
{
  //cout << "   -- reshapeView " << w << " " << h << endl;
  glViewport(0,0,(GLsizei)w,(GLsizei)h);
  ratio = (double)w/h;
  width  = w;
  height = h;
  pCglScene scene = pcv->getScene();
  //scene->cursorOrigin = glm::vec2(width/2, height/2);
}
