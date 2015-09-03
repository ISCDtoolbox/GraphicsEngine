/****************************************************************
 *
 * GLIC: ICS Graphics Library
 *
 * Original Authors:
 *   J. Foulon, P. Frey
 *
 ****************************************************************/

#ifndef __GLIC_AXIS_H_
#define __GLIC_AXIS_H_

#include <glic/object.h>
#include <glic/shader.h>
#include <glic/view.h>

class GLIC_API CglicAxis : public CglicObject
{
public:
  CglicAxis();
  ~CglicAxis(){};
  void display();
  CglicView *view;

private:
  vector<float> grid;
  vector<float> axes;
  GLuint gridBuffer;
  GLuint axesBuffer;
};

#endif
