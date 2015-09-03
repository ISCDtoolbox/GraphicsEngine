/****************************************************************
 *
 * CGL: ICS Graphics Library
 *
 * Original Authors:
 *   J. Foulon, P. Frey
 *
 ****************************************************************/

#ifndef __CGL_AXIS_H_
#define __CGL_AXIS_H_

#include <cgl/object.h>
#include <cgl/shader.h>
#include <cgl/view.h>

class CGL_API CglAxis : public CglObject
{
public:
  CglAxis();
  ~CglAxis(){};
  void display();
  CglView *view;

private:
  vector<float> grid;
  vector<float> axes;
  GLuint gridBuffer;
  GLuint axesBuffer;
};

#endif
