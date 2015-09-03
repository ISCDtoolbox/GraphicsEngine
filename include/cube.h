/****************************************************************
 *
 * GLIC: ICS Graphics Library
 *
 * Original Authors:
 *   J. Foulon, P. Frey
 *
 ****************************************************************/
#ifndef __GLIC_CUBE_H_
#define __GLIC_CUBE_H_

#include <glic/object.h>

class GLIC_API CglicCube : public CglicObject
{
public:
  CglicCube(double cr=0., double cg=0., double cb=0.,double rr=1);
  void display();
  ~CglicCube();

private:
  double r;
  int colorTest[4];
};

#endif
