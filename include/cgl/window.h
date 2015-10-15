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

class CGL_API CglWindow{

public:
    pCglScene           pScene;
    pCglView            pView;
    pCglRadialInterface pRadialInterface;
    pCglLinearInterface pLinearInterface;

    glm::vec2   position, size;
    std::string title;
    int         ID;


    CglShader     simpleShader, smoothShader, flatShader;
    int           simpleID(){     return simpleShader.mProgramID;}
    int           smoothID(){     return smoothShader.mProgramID;}
    int           flatID(){       return flatShader.mProgramID;}

public:
    CglWindow();
	CglWindow(int x, int y, int w, int h);
	~CglWindow();

    void reshape(int w, int h);
    void display();
	void cglMainLoop();

    pCglScene getScene(){return pScene;}

	void setScene(pCglScene sc){  pScene = sc; sc->setView(pView); sc->setWindowID(ID);}
	void setView(pCglView vi){    pView  = vi;}

	pCglInterface getLinearInterface(){return pLinearInterface;}
	pCglInterface getRadialInterface(){return pRadialInterface;}

    void loadShaders();

	//Glut wrap functions
    static void reshapeWrap(int w, int h);
    static void displayWrap();
    static void mouseWrap(int b, int s, int x, int y);
    static void motionWrap(int x, int y);
    static void passiveMotionWrap(int x, int y);
    static void keyWrap(unsigned char key,int x,int y);
    static void keyUpWrap(unsigned char key,int x,int y);
    static void specialWrap(int key, int x, int y);

protected:
	void displayBuffer(int buffer);
};

typedef CglWindow * pCglWindow;


class CglSubWindow : public CglWindow{
public:
    CglSubWindow();
	CglSubWindow(pCglWindow window, int x, int y, int w, int h);
	~CglSubWindow();
};
typedef CglSubWindow* pCglSubWindow;




#endif

