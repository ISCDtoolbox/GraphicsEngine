/****************************************************************
*
* CGL: ICS Graphics Library
*
* Original Authors:
*   J. Foulon, P. Frey
*
****************************************************************/

#ifndef __CGL_WINDOW_H_
#define __CGL_WINDOW_H_

#include "defs.h"
#include <cgl/scene.h>
#include <cgl/view.h>

class   CglWindow;
class   CglSubWindow;

typedef CglWindow*     pCglWindow;
typedef CglSubWindow*  pCglSubWindow;

enum    CGL_SUBWINDOW{ CGL_FULL, CGL_LEFT, CGL_RIGHT, CGL_TOP, CGL_BOTTOM, CGL_CUSTOM };



////////////////////////////////////////////////////////////////////////:
//Classe CGLSUBWINDOW
// Needs a parent window to work...
// Allows for screen partitionning
//..

class CglSubWindow{
public:
    CGL_SUBWINDOW               location;
    pCglWindow                  parent;
    pCglScene                   pScene;
    pCglView                    pView;
    pCglRadialInterface         pRadialInterface;
    pCglLinearInterface         pLinearInterface;
    CglShader                   simpleShader, smoothShader, flatShader;
    int                         ID;
    glm::vec2                   position;
    glm::vec2                   size;

public:
    CglSubWindow(pCglWindow window, CGL_SUBWINDOW loc);                   //FULL
	CglSubWindow(pCglWindow window, int x, int y, int w, int h);          //CUSTOM
	~CglSubWindow();

    //pCglSubWindow getSubWindow();
    pCglScene     getScene(){             return pScene;}
	void          setScene(pCglScene sc){ pScene = sc; sc->setView(pView); sc->setWindowID(ID);}
	void          setView( pCglView vi){  pView  = vi;}
	pCglInterface getLinearInterface(){   return pLinearInterface;}
	pCglInterface getRadialInterface(){   return pRadialInterface;}

    void          display();
    void          displayBuffer(int buffer);
    void          compute_window(int w, int h);
    void          reshape(int w, int h);

    void          loadShaders();
    int           simpleID(){             return simpleShader.mProgramID;}
    int           smoothID(){             return smoothShader.mProgramID;}
    int           flatID(){               return flatShader.mProgramID;}

    void          setCallbacks();
    static void   displayWrap();
    static void   reshapeWrap(       int w, int h);

    static void mouseWrap(         int b, int s, int x, int y);
    static void motionWrap(        int x, int y);
    static void passiveMotionWrap( int x, int y);
    static void keyWrap(           unsigned char key,int x,int y);
    static void keyUpWrap(         unsigned char key,int x,int y);
    static void specialWrap(       int key, int x, int y);

    static void idle();
    void        idleCallback();
};



////////////////////////////////////////////////////////////////////////:
//Classe CGLWINDOW
// "Physical window"
// Contains subwindows, which are displayed when needed
//..

class CGL_API CglWindow{
public:
    std::vector<pCglSubWindow>  subWindows;
    glm::vec2                   position;
    glm::vec2                   size;
    std::string                 title;
    int                         ID;

public:
    CglWindow();
	CglWindow(    int x, int y, int w, int h);
	~CglWindow();

    pCglSubWindow getSubWindow();
    void          reshape(     int w, int h);
    static void   reshapeWrap( int w, int h);
	void          display();
    static void   displayWrap();
};



#endif
