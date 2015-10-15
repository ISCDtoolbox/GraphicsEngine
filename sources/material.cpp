#include <cgl/material.h>

CglMaterial::CglMaterial(glm::vec3 col, float dif, float amb, float speLob){
    color        = col;
    diffuseRatio = dif;
    specularLobe = speLob;
    ambientRatio = amb;
}
