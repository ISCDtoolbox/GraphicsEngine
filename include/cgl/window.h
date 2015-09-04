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


class CGL_API CglWindow
{
private:
	int    m_wpos[2],m_wsiz[2];   // window position + size
	int    m_vpos[2],m_vsiz[2];   // viewport position + size
  double m_mold[16],m_mnew[16];


public:
  std::vector<pCglLight>  light;
	int   m_id;
  int ids;
	CglView    view;

	CglWindow();
	CglWindow(int x, int y, int w, int h);
	virtual ~CglWindow();

  int cglAddLight(pCglLight li);

  void reshape(int w, int h);

	void show();
	void display();
	void resize(int w, int h);
  void title(const char *title);
	void cglMainLoop();
};

typedef CglWindow * pCglWindow;

#endif

