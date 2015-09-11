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
#include <cgl/profile.h>
#include <cgl/shader.h>
#include <cgl/interface.h>

class CGL_API CglCanvas
{

/////////////////////////////////////////////////////
//Attributes

public:
  CglProfile profile;

private:

  std::vector<CglLight>    light;
  std::vector<pCglScene>   scene;
  std::vector<CglWindow>   window;
  std::vector<pCglObject>  object;
  CglMouse                 mice;
  CglKeyboard              keyboard;
  CglShader                simpleShader, smoothShader, flatShader;
  CglRadialInterface       interface;
  //CglLinearInterface       interface;


/////////////////////////////////////////////////////
//Methods

public:

  //Constructors and destructors
  CglCanvas(){};
  CglCanvas(int argc, char **argv);
  virtual ~CglCanvas(){};

  //Scenes, windows... creation
  int  cglWindow(int x, int y, int w, int h);
  int  cglScene();
  int  cglObject(pCglObject obj);
  void cglSetLights();
  void cglSetScene(int ids, int idw);
  void cglSetObject(int ido, int ids);

  //Accessors
  pCglLight     getLight( int lightID);
  pCglScene     getScene( int sceneID);
  pCglWindow    getWindow(int windowID);
  pCglScene     getScene();
  pCglWindow    getWindow();
  pCglInterface getInterface();
  pCglMouse     getMouse();
  int           simpleID();
  int           smoothID();
  int           flatID();
  void          centerMouse();
  int           winid();

  //Glut wrap functions
  static void reshapeWrap(int w, int h);
  static void displayWrap();
  static void mouseWrap(int b, int s, int x, int y);
  static void motionWrap(int x, int y);
  static void passiveMotionWrap(int x, int y);
  static void keyWrap(unsigned char key,int x,int y);
  static void keyUpWrap(unsigned char key,int x,int y);
  static void specialWrap(int key, int x, int y);

  //Main loop
  void cglMainLoop();


/////////////////////////////////////////////////////
//Private virtual methods, which need an instance of canvas

private:
  virtual void reshape(int w, int h);
  virtual void display();

};


#endif
