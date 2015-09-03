/****************************************************************
 *
 * CGL: ICS Graphics Library
 *
 * Original Authors:
 *   J. Foulon, P. Frey
 *
 ****************************************************************/
#ifndef __CGL_SPHERE_H_
#define __CGL_SPHERE_H_

#include <cgl/object.h>

class CGL_API CglSphere : public CglObject
{
private:
  double r;
public:
  CglSphere(double cr=0., double cg=0., double cb=0.,double rr=1.);
  void display();
  ~CglSphere();
};

#endif