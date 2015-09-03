/****************************************************************
 *
 * GLIC: ICS Graphics Library
 *
 * Original Authors:
 *   J. Foulon, P. Frey
 *
 ****************************************************************/
#ifndef __GLIC_CORE_H_
#define __GLIC_CORE_H_

#include "defs.h"
#include <glic/light.h>
#include <glic/mouse.h>
#include <glic/keyboard.h>
#include <glic/scene.h>
#include <glic/window.h>
#include <glic/transform.h>
#include <glic/profile.h>
#include <glic/shader.h>

class GLIC_API CglicCanvas
{
public:
  std::vector<CglicLight>    light;
  std::vector<pCglicScene>   scene;
  std::vector<CglicWindow>   window;
  std::vector<pCglicObject>  object;
  CglicMouse    mice;
  CglicKeyboard keyboard;
  CglicProfile  profile;
  SHADER        simpleShader, smoothShader;

  //private:
  int winid();

public:
  // graphic engine constructor + destructor
  CglicCanvas(){};
  CglicCanvas(int argc, char **argv);
  virtual ~CglicCanvas(){};

  int  glicWindow(int x, int y, int w, int h);
  int  glicScene();
  void glicSetScene(int ids, int idw);
  void glicLight();

  int glicObject(pCglicObject obj);
  void glicSetObject(int ido, int ids);

  int glicPickObject(int x, int y);

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

  void glicMainLoop();
};


#endif
