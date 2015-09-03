/****************************************************************
 *
 * CGL: ICS Graphics Library
 *
 * Original Authors:
 *   J. Foulon, P. Frey
 *
 ****************************************************************/
#ifndef __CGL_CUBE_H_
#define __CGL_CUBE_H_

#include <cgl/object.h>

class CGL_API CglCube : public CglObject
{
public:
  CglCube(double cr=0., double cg=0., double cb=0.,double rr=1);
  void display();
  ~CglCube();

private:
  double r;
  int colorTest[4];
};

#endif
