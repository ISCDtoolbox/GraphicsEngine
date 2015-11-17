/****************************************************************
 *
 * CGL: ICS Graphics Library
 *
 * Original Authors:
 *   J. Foulon, P. Frey
 *
 ****************************************************************/
#ifndef __CGL_MESH_H_
#define __CGL_MESH_H_

#include <cgl/object.h>

#define FLOAT_MAX  1.e20
/*
typedef struct {
  double    c[3];
  int       ref;
} Point;
//typedef Point * pPoint;

typedef struct {
  int       v[3],ref;
} Tria;
//typedef Tria * pTria;

typedef struct {
  double    n[3];
} Normal;
//typedef Normal * pNormal;

typedef struct {
  int inds[2];
} NormalAtVertex;
*/

class CglMesh;
typedef CglMesh* pCglMesh;

enum PrimitiveType{CUBE, SPHERE, CYLINDER};

class CGL_API CglMesh : public CglObject
{
public:

public:
  CglMesh(char *name);
  CglMesh(pCglObject M);
  ~CglMesh(){};

  glm::vec3 getBBMIN(){return center + glm::vec3(glm::inverse(MODEL) * glm::vec4(pGeom->bbmin, 0));};
  glm::vec3 getBBMAX(){return center + glm::vec3(glm::inverse(MODEL) * glm::vec4(pGeom->bbmax, 0));};
  //void      meshInfo(const int& verbose = 0, ostream& outstr = cout);

  void      shadowsDisplay();
  void      displayShadow();
  void      displayReflection();

  void      artifactsDisplay();

  void      display();
};

typedef CglMesh* pCglMesh;

#endif
