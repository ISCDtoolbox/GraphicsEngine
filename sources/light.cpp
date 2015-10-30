#include <cgl/light.h>
#include <cgl/material.h>
#include <glm/ext.hpp>

#include <cgl/canvas.h>
extern CglCanvas *pcv;

CglLight::CglLight(glm::vec3 pos, glm::vec3 col, float pow, bool cam){
  position            = pos;
  color               = col;
  power               = pow;
  follows_camera      = cam;
  pcv->addLight(this);
}
CglLight::CglLight(float x,float y,float z, float r,float g,float b, float pow, bool cam){
  position            = glm::vec3(x,y,z);
  color               = glm::vec3(r,g,b);
  power               = pow;
  follows_camera      = cam;
  pcv->addLight(this);
}

glm::mat4* CglLight::getLightMatrix(pCglMaterial material){
    //Création de la matrice d'envoi
    packed[0]           = glm::vec4(position, 0);
    packed[1]           = glm::vec4(color, 0);
    packed[2]           = glm::vec4(material->getAmbient(), material->getDiffuse(), material->getSpecular(), 0);
    packed[3]           = glm::vec4(power, material->getLobe(), float(follows_camera), 0);
    return &packed;
}

void CglLight::rotate(float angle){
    glm::vec3 axis;
    if(follows_camera)
        axis = glm::vec3(0,0,1);
    else
        axis = glm::vec3(0,1,0);
    position = glm::rotate(position, angle, axis);
}
