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


enum PrimitiveType{CUBE, SPHERE, CYLINDER};

class CGL_API CglMesh : public CglObject
{
private:
  //Pas nécessaire de les conserver en mémoire si uniquement du display, on peut garder le système de buffer
  //vector<Point>             point;
  //vector<Tria>              tria;
  //vector<Normal>            normal;
  //vector<NormalAtVertex>    NormalAtVertices;
  GLuint    bboxBuffer;        //idem
  GLuint    bboxIndBuffer;     //idem
  glm::vec3 bbmin, bbmax;
  glm::vec3 tra;

public:
  CglMesh(char *name);
  CglMesh(){isMesh = true;};
  ~CglMesh(){};

  void      getBBOX(std::vector<Point> &p);
  glm::vec3 getBBMIN(){return center + glm::vec3(glm::inverse(MODEL) * glm::vec4(bbmin, 0));};
  glm::vec3 getBBMAX(){return center + glm::vec3(glm::inverse(MODEL) * glm::vec4(bbmax, 0));};
  //void      meshInfo(const int& verbose = 0, ostream& outstr = cout);

  void      shadowsDisplay();
  void      displayShadow();
  void      displayReflection();

  void      artifactsDisplay();

  void      display();
};

typedef CglMesh* pCglMesh;

#endif
