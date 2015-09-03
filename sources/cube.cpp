#include <cgl/cube.h>

CglCube::CglCube(double cr, double cg, double cb, double rr):r(rr)
{
  //cout << "  --- - [create CglCube]" << endl;
  //color[0]=cr;
  //color[1]=cg;
  //color[2]=cb;
}

CglCube::~CglCube()
{
  //cout << "  --- - [destroy CglCube]" << endl;
}

void CglCube::display()
{

  //if (state == TO_SEL ){
  //  glMaterialfv(GL_FRONT, GL_DIFFUSE, glm::value_ptr(mat_diffuse));
  //}
  //else
  //  glColor3f(color.r, color.g, color.b);


  glutSolidCube(0.2*r);
  glColor3f(1., 1., 1.);
  if (box)
    glutWireCube(r);

}
