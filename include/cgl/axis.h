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
private:
  vector<float> mainGrid;
  vector<float> secondaryGrid;
  vector<float> axes;
  vector<float> back;
  vector<float> colors;
  GLuint mainGridBuffer;
  GLuint secondaryGridBuffer;
  GLuint axesBuffer;

public:
  CglAxis();
  ~CglAxis(){};
  void display();
  pCglView view;
};
typedef CglAxis* pCglAxis;


class CGL_API CglBackground : public CglObject{
  private:
    void gradient(std::vector<float> hei, std::vector<glm::vec3> col);
    void gradient(std::vector<glm::vec2> hei, std::vector<glm::vec3> col);
  public:
    void display();
};
typedef CglBackground* pCglBackground;

#endif
