#include <cgl/light.h>
#include <cgl/material.h>
#include <glm/ext.hpp>

CglLight::CglLight(glm::vec3 pos, glm::vec3 col, glm::vec3 a_d_s_ratio, float pow, float lob, bool cam){
  position            = pos;
  color               = col;
  amb_dif_spe_ratio   = a_d_s_ratio;
  power               = pow;
  specular_lobe       = lob;
  follows_camera      = cam;

  //Création de la matrice d'envoi
  packed[0]           = glm::vec4(pos, 0);
  packed[1]           = glm::vec4(col, 0);
  packed[2]           = glm::vec4(a_d_s_ratio, 0);
  packed[3]           = glm::vec4(pow, lob, float(cam), 0);
}
