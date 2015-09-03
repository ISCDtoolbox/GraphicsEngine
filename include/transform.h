/****************************************************************
 *
 * GLIC: ICS Graphics Library
 *
 * Original Authors:
 *   J. Foulon, P. Frey
 *
 ****************************************************************/
#ifndef __GLIC_TRANSFORM_H_
#define __GLIC_TRANSFORM_H_

#include "defs.h"

class GLIC_API CglicTransform
{
public:
  glm::vec3 tr;
  glm::mat4 rot;

  //Retour en arrière
  std::vector<glm::mat4> lastMatrices;
  std::vector<glm::vec3> lastUps;
  std::vector<glm::vec3> lastCams;

  CglicTransform(){};
  ~CglicTransform(){};
  void setRotation(glm::mat4 M);
  void setTranslation(glm::vec3 translation);
  void print();
  void reset();
};

#endif
