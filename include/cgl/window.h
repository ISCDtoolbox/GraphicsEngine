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


struct FrameCounter{
    int frame;
    int time;
    int lastTime;
    FrameCounter(){lastTime = 0; frame = 0;}
};


class CGL_API CglWindow{
public:
    std::vector<pCglSubWindow>  subWindows;
    glm::vec2                   position;
    glm::vec2                   size;
    std::string                 title;
    int                         ID;
    FrameCounter                FPS;

public:
    CglWindow();
	CglWindow(    int x, int y, int w, int h);
	~CglWindow();
    pCglSubWindow getSubWindow();
    void          reshape(     int w, int h);
    static void   reshapeWrap( int w, int h);
    void          display();
    static void   displayWrap();
    static void   idleWrap();
};


class CglSubWindow{
public:
    CGL_SUBWINDOW               location;
    pCglWindow                  parent;
    pCglScene                   pScene;
    pCglView                    pView;
    pCglLinearInterface         pLinearInterface;
    pCglRadialInterface         pRadialInterface;
    CglShader                   simpleShader, smoothShader, flatShader;
    int                         ID;
    glm::vec2                   position;
    glm::vec2                   size;
    float                       ratio;

public:
    CglSubWindow(pCglWindow window, CGL_SUBWINDOW loc, float R = 0.5);
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
};


/*
class CGL_API CglWindow{
public:
    std::vector<pCglSubWindow>  subWindows;
    pCglLinearInterface         pLinearInterface;
    glm::vec2                   position;
    glm::vec2                   size;
    std::string                 title;
    int                         ID;
    FrameCounter                FPS;

public:
    CglWindow();
	CglWindow(    int x, int y, int w, int h);
	~CglWindow();
    pCglSubWindow getSubWindow();
    void          reshape(     int w, int h);
    static void   reshapeWrap( int w, int h);
    static void   idleWrap();

};*/


#endif
