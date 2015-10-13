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

private:
    glm::vec2   position, size;
    std::string title;
    int         id;
    pCglScene   pScene;
    int         ids;

public:
    std::vector<pCglLight>  light;
    CglView                 view;

    public:
    CglWindow();
	CglWindow(int x, int y, int w, int h);
	~CglWindow();
    int cglAddLight(pCglLight li);

    void reshape(int w, int h);
    pCglView getView(){return &view;}
    void display();
    void resize(int w, int h);
	void cglMainLoop();

public:
    int       getID(){   return id;}
    pCglScene getScene(){return pScene;}

private:
	void displayBuffer(int buffer);

};

typedef CglWindow * pCglWindow;

#endif

