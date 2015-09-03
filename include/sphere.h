/****************************************************************
 *
 * GLIC: ICS Graphics Library
 *
 * Original Authors:
 *   J. Foulon, P. Frey
 *
 ****************************************************************/
#ifndef __GLIC_SPHERE_H_
#define __GLIC_SPHERE_H_

#include <glic/object.h>

class GLIC_API CglicSphere : public CglicObject
{
private:
  double r;
public:
  CglicSphere(double cr=0., double cg=0., double cb=0.,double rr=1.);
  void display();
  ~CglicSphere();
};

#endif