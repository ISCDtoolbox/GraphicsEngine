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

enum OBJECT_TYPE{SUPER, SIMPLE, MESH};
struct ClipBoard{
  OBJECT_TYPE object_type;
  glm::vec3 center;
  glm::mat4 MODEL;
  char* fileName;
  ClipBoard(OBJECT_TYPE& T, char* F, glm::vec3 C, glm::mat4 M){
    object_type = T;
    fileName = F;
    center = C;
    MODEL = M;
  }
};

struct Profiler{
    int bind, free, bindDraw, draw, uni;
    int setScale, applyTransformation, computeGroup, objApplyTransformation, clear;
    int shadows, display, artifacts, axis, interface;
    Profiler();
    void print();
};

class CGL_API CglCanvas
{
/////////////////////////////////////////////////////
//Attributes

public:
  CglProfile    profile;
  std::vector<ClipBoard> clipboard;
  Profiler      PROF;

private:
  std::vector<pCglLight>    lights;
  std::vector<pCglScene>    scenes;
  std::vector<pCglWindow>   windows;
  std::vector<pCglObject>   objects;

  CglMouse                  mice;
  CglKeyboard               keyboard;
  //CglShader                 simpleShader, smoothShader, flatShader;


/////////////////////////////////////////////////////
//Methods

public:

  //Constructors and destructors
  CglCanvas(){};
  CglCanvas(int argc, char **argv);
  virtual ~CglCanvas(){};

  pCglProfile getProfile(){return &profile;}


  //Rajouts automatiques
  void addWindow(pCglWindow win){windows.push_back(win);}
  void addScene( pCglScene  sce){scenes.push_back( sce);}
  void addObject(pCglObject obj){objects.push_back(obj);}
  void addLight( pCglLight  lig){lights.push_back( lig);}

  std::vector<pCglWindow> getWindows(){return windows;}
  std::vector<pCglScene>  getScenes(){ return scenes;}
  std::vector<pCglObject> getObjects(){return objects;}
  std::vector<pCglLight>  getLights(){ return lights;}


  //Scenes, windows... creation
  pCglWindow cglWindow(int x, int y, int w, int h);
  pCglScene  cglScene();

  void initGLEW();


  //Accessors
  pCglLight     getLight( int lightID);
  pCglScene     getScene( int sceneID);
  //pCglSubWindow getWindow(int windowID);
  pCglScene     getScene();
  pCglInterface getInterface();
  pCglMouse     getMouse(){    return &mice;}
  pCglKeyboard  getKeyboard(){ return &keyboard;}

  pCglWindow    getWindow();
  pCglSubWindow getSubWindow();

  std::vector<pCglObject>* getObjectList(){return &objects;}
  std::vector<pCglLight>*  getLightsList(){return &lights;}

  int           simpleID(){return getSubWindow()->simpleID();}
  int           smoothID(){return getSubWindow()->smoothID();}
  int           flatID(){  return getSubWindow()->flatID();}
  void          centerMouse();
  //int           winid();

  //Glut wrap functions

  //static void reshapeWrap(int w, int h);
  //static void displayWrap();
  static void mouseWrap(int b, int s, int x, int y);
  static void motionWrap(int x, int y);
  static void passiveMotionWrap(int x, int y);
  static void keyWrap(unsigned char key,int x,int y);
  static void keyUpWrap(unsigned char key,int x,int y);
  static void specialWrap(int key, int x, int y);

  //Main loop
  void loop();


/////////////////////////////////////////////////////
//Private virtual methods, which need an instance of canvas

private:
  //virtual void reshape(int w, int h);
  //virtual void display();

};


#endif
