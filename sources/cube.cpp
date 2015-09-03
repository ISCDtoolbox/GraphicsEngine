#include <glic/cube.h>

CglicCube::CglicCube(double cr, double cg, double cb, double rr):r(rr)
{
  //cout << "  --- - [create CglicCube]" << endl;
  //color[0]=cr;
  //color[1]=cg;
  //color[2]=cb;
}

CglicCube::~CglicCube()
{
  //cout << "  --- - [destroy CglicCube]" << endl;
}

void CglicCube::display()
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
