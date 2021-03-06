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
#include <cgl/texture.h>
#include <cgl/cube.h>

class CGL_API CglAxis : public CglObject
{
private:
  vector<float> axes;
  vector<float> back;
  vector<float> colors;
  vector<float> mainGrid;
  GLuint axesBuffer;
  GLuint mainGridBuffer;
  GLuint nBuffer;
  glm::vec3 mins, maxs;
  pCglMaterial    material;

public:
  CglAxis();
  ~CglAxis(){};
  glm::vec3 getBBMIN(){return mins;}
  glm::vec3 getBBMAX(){return maxs;}
  void display();
  pCglView view;
  //pCglScene pScene;
};
typedef CglAxis* pCglAxis;


class CGL_API CglBackground : public CglObject{
  private:
    //CglTexture texture;
    void gradient(std::vector<float> hei,       std::vector<glm::vec3> col);
    void gradient(std::vector<glm::vec2> hei, std::vector<glm::vec3> col);
  public:
    CglBackground();
    void display();
};
typedef CglBackground* pCglBackground;

#endif
