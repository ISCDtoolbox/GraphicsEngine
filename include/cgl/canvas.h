/****************************************************************
 *
 * CGL: ICS Graphics Library
 *
 * Original Authors:
 *   J. Foulon, P. Frey
 *
 ****************************************************************/
#ifndef __CGL_CORE_H_
#define __CGL_CORE_H_

#include "defs.h"
#include <cgl/light.h>
#include <cgl/mouse.h>
#include <cgl/keyboard.h>
#include <cgl/scene.h>
#include <cgl/window.h>
#include <cgl/transform.h>
#include <cgl/profile.h>
#include <cgl/shader.h>

class CGL_API CglCanvas
{
public:
  std::vector<CglLight>    light;
  std::vector<pCglScene>   scene;
  std::vector<CglWindow>   window;
  std::vector<pCglObject>  object;
  CglMouse    mice;
  CglKeyboard keyboard;
  CglProfile  profile;
  SHADER        simpleShader, smoothShader;

  //private:
  int winid();

public:
  // graphic engine constructor + destructor
  CglCanvas(){};
  CglCanvas(int argc, char **argv);
  virtual ~CglCanvas(){};

  int  cglWindow(int x, int y, int w, int h);
  int  cglScene();
  void cglSetScene(int ids, int idw);
  void cglLight();

  int cglObject(pCglObject obj);
  void cglSetObject(int ido, int ids);

  int cglPickObject(int x, int y);

private:
  virtual void reshape(int w, int h);
  virtual void display();

public:
  static void reshapeWrap(int w, int h);
  static void displayWrap();
  static void mouseWrap(int b, int s, int x, int y);
  static void motionWrap(int x, int y);
  static void passiveMotionWrap(int x, int y);
  static void keyWrap(unsigned char key,int x,int y);
  static void keyUpWrap(unsigned char key,int x,int y);
  static void specialWrap(int key, int x, int y);

  void cglMainLoop();
};


#endif
