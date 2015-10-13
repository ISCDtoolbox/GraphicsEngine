#include <cgl/material.h>
/*
CglMaterial::CglMaterial(){
    color = glm::vec3(0.8,0.8,0.8);
    diffuseRatio = 0.8;
    specularLobe = 0.5;
    ambientRatio = 0.1;
}
*/

CglMaterial::CglMaterial(glm::vec3 col, float dif, float amb, float speLob){
    color        = col;
    diffuseRatio = dif;
    specularLobe = speLob;
    ambientRatio = amb;
}
