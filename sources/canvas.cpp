#include <cgl/canvas.h>

CglCanvas *pcv;

CglCanvas::CglCanvas(int argc, char **argv)
{
  pcv = this;

  glutInit(&argc, argv);

  if(pcv->profile.stereo)
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STEREO);
  else
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

  cglLight();

  //Construct profile from a configuration file
  profile = CglProfile();
}


int CglCanvas::cglWindow(int x, int y, int w, int h)
{
  window.push_back(CglWindow(x,y,w,h));
  return window.size()-1;
}


int CglCanvas::cglScene()
{
  scene.push_back(new CglScene());
  scene[scene.size()-1]->ids = scene.size()-1;
  cout << "scene.size()-1: " << scene.size()-1 << endl;
  return scene.size()-1;
}

void CglCanvas::cglSetScene(int ids, int idw)
{
  window[idw].ids = ids;
  scene[ids]->view = &window[idw].view;
}


void CglCanvas::cglLight()
{
  //cout << "   Sun " << endl;
   light.push_back(CglLight(0));
   light[0].setPos(0., 0., 1., 1.);
   //light[0].setCol(CglMaterial::TC_DIF, 1., 0., 0., 0.5);
   //cout << "   Sun  end" << endl;

  //cout << "   Light 1 " << endl;
  light.push_back(CglLight(0));
  light[0].setPos(-1.0, 0., 0.5, 1.);
  //light[0].setCol(CglMaterial::TC_DIF, 1., 0., 0., 0.5);
  //cout << "   Light 1  end" << endl;

  //cout << "   Light 2 " << endl;
  light.push_back(CglLight(1));
  //cout << "   Light 2 bis" << endl;
  light[1].setPos(1.0, 0., 0.5, 1.);
  //light[1].setCol(CglMaterial::TC_DIF,0., 1., 0., 0.5);

  //cout << "   Light 3 " << endl;
  light.push_back(CglLight(2));
  //cout << "   Light 3 bis" << endl;
  light[2].setPos(0., 1., 0.5, 1.);
  //light[2].setCol(CglMaterial::TC_DIF, 0., 0., 1., 1.);
}

int CglCanvas::cglObject(pCglObject obj)
{
  //cout << " [define CglObject]" << endl;
  object.push_back(obj);
  return object.size() - 1;
}

void CglCanvas::cglSetObject(int ido, int ids)
{
  //cout << " [add Object n." << ido << " to scene n." << ids << "]" << endl;
  scene[ids]->addObject(object[ido]);
}

int CglCanvas::cglPickObject(int x, int y)
{
  //cout << " [pick Object ] " << endl;

  return 0;
}


int CglCanvas::winid()
{
  int idw = glutGetWindow();
  for (int i=0; i<window.size(); i++) {
    if ( window[i].m_id == idw )  return(i);
  }
  return(0);
}


void CglCanvas::reshape(int w, int h)
{
  //cout << " [reshapeGL]" << endl;
  int  idw = winid();
  window[idw].view.reshape(w, h);
}

void CglCanvas::reshapeWrap(int w, int h) {
  //cout << " [reshapeWrap]" << endl;
  pcv->reshape(w, h);
}


void CglCanvas::display()
{
  //cout << " [display CglCanvas]" << endl;
  int  idw = winid();
  window[idw].display();
  window[idw].view.setView();
}

void CglCanvas::displayWrap()
{
  //cout << " [displayWrap]" << endl;
  pcv->display();
}


void CglCanvas::motionWrap(int x, int y)
{
  //cout << " [motionWrap]" << endl;
  pcv->mice.motion(x,y);
  glutPostRedisplay();
}

void CglCanvas::passiveMotionWrap(int x, int y)
{
  //cout << " [motionWrap]" << endl;
  pcv->mice.passiveMotion(x,y);
  glutPostRedisplay();
}


void CglCanvas::mouseWrap(int b, int s, int x, int y)
{
  //cout << " [mouseWrap]" << endl;
  pcv->mice.mouse(b, s, x, y);
  glutPostRedisplay();
}


void CglCanvas::keyWrap(unsigned char key, int x, int y)
{
  //cout << " [keyboardWrap]" << endl;
  pcv->keyboard.keyboard(key, x, y);
  glutPostRedisplay();
}
void CglCanvas::keyUpWrap(unsigned char key, int x, int y)
{
  //cout << " [keyboardWrap]" << endl;
  pcv->keyboard.keyboardUp(key, x, y);
  glutPostRedisplay();
}

void CglCanvas::specialWrap(int key, int x, int y)
{
  //cout << " [specialWrap]" << endl;
  pcv->keyboard.special(key, x, y);
  glutPostRedisplay();
}


void CglCanvas::cglMainLoop()
{
  simpleShader.load("shaders/shader.vert", "shaders/shader.frag");
  smoothShader.load("shaders/smooth_shader.vert", "shaders/smooth_shader.frag");
  glutMainLoop();
}

