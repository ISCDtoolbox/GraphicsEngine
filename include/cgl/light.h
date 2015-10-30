
/****************************************************************
 *
 * CGL: ICS Graphics Library
 *
 * Original Authors:
 *   J. Foulon, P. Frey
 *
 ****************************************************************/
#ifndef __CGL_LIGHT_H_
#define __CGL_LIGHT_H_

#include <cgl/material.h>
#include <cgl/object.h>


class CGL_API CglLight{
  private:
    glm::vec3 position;
    glm::vec3 color;
    glm::vec3 amb_dif_spe_ratio;
    float     power;
    float     specular_lobe;
    bool      follows_camera;
    glm::mat4 packed;

  public:
    CglLight(glm::vec3 pos, glm::vec3 col, float pow, bool cam);
    CglLight(float x,float y,float z, float r,float g,float b, float pow, bool cam);
    glm::mat4* getLightMatrix(pCglMaterial material);
    void      rotate(float angle);
};

typedef CglLight * pCglLight;

#endif
