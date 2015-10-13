#ifndef MATERIAL_H
#define MATERIAL_H

#include <cgl/defs.h>

class CglMaterial{
private:
    glm::vec3  color;
    float      ambientRatio;
    float      diffuseRatio;
    float      specularLobe;

  public:
    //CglMaterial();
    CglMaterial(glm::vec3 col, float dif, float amb, float speLob);
    float      getAmbient(){ return ambientRatio;}
    float      getDiffuse(){ return diffuseRatio * (1.0f - ambientRatio);}
    float      getSpecular(){return (1.0f - ambientRatio) * (1.0f - diffuseRatio); }
    float      getLobe(){    return specularLobe;}
    glm::vec3  getColor(){   return color;}
    void       setColor(glm::vec3 col){color = col;};
};

typedef CglMaterial* pCglMaterial;

#endif // MATERIAL_H
