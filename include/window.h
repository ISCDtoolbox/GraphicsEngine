/****************************************************************
*
* GLIC: ICS Graphics Library
*
* Original Authors:
*   J. Foulon, P. Frey
*
****************************************************************/
#ifndef __GLIC_WINDOW_H_
#define __GLIC_WINDOW_H_

#include "defs.h"
#include <glic/scene.h>
#include <glic/view.h>


class GLIC_API CglicWindow
{
private:
	int    m_wpos[2],m_wsiz[2];   // window position + size
	int    m_vpos[2],m_vsiz[2];   // viewport position + size
  double m_mold[16],m_mnew[16];
  std::vector<pCglicLight>  light;
  
public:
	int   m_id;
  int ids;
	CglicView    view;

	CglicWindow();
	CglicWindow(int x, int y, int w, int h);
	virtual ~CglicWindow();

  int glicAddLight(pCglicLight li);
  void activateLight();
  
  void reshape(int w, int h);

	void show();
	void display();
	void resize(int w, int h);
  void title(const char *title);
	void glicMainLoop();
};

typedef CglicWindow * pCglicWindow;

#endif

