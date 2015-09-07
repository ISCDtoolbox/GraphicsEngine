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
  void gradient(std::vector<float> hei, std::vector<glm::vec3> col);
  void display();
  CglView *view;

private:
  vector<float> grid;
  vector<float> axes;
  vector<float> back;
  vector<float> colors;
  GLuint gridBuffer;
  GLuint axesBuffer;
  GLuint backBuffer;
  GLuint backColorBuffer;
};

#endif
