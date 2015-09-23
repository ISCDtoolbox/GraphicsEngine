#include "cgl/profile.h"
#include "ext.h"


#include <cgl/canvas.h>
extern CglCanvas *pcv;

CglProfile::CglProfile()
{
  //Reading from cmakefile
  path              = EXT_PATH;

  displayAxes       = true;

  displayBottomGrid = false;

  stereo            = false;
  perspective       = true;

  smooth            = false;

  flyingMode        = false;

  invertVertical    = true;

  independantScale  = false;
  globalScale       = true;

  groupRotation     = true;

  fullscreen        = false;

  bottomDistance    = 0.1;
  bottomAngle       = 0.0;//G(float)3.14159/4;

  initial_arrangement = CGL_ARRANGEMENT_GRID;
  camera              = CGL_CAM_UPPER_SPHERE;
  interface           = CGL_INTERFACE_LINEAR;
  buttons             = CGL_BUTTONS_EXTENDED;
  colors              = CGL_COLORS_FLASHY;
  theme               = CGL_THEME_DARK;

  update_colors();
  update_theme();
}

glm::vec3 CglProfile::color(){
  return colorGenerator.generateColor();
}




void CglProfile::switch_interface(){
  int n;
  for( int i = CGL_INTERFACE_LINEAR; i <= CGL_INTERFACE_RADIAL ; i++ ){
    if ( i == colors ){
      if ( i != CGL_INTERFACE_RADIAL )
        n = i+1;
      else
        n = 0;
    }
  }
  interface = static_cast<CGL_INTERFACE>(n);
}





void CglProfile::switch_colors(){
  int n;
  for( int i = CGL_COLORS_FLASHY; i <= CGL_COLORS_WARM ; i++ ){
    if ( i == colors ){
      if ( i != CGL_COLORS_WARM )
        n = i+1;
      else
        n = 0;
    }
  }
  colors = static_cast<CGL_COLORS>(n);
  update_colors();
  update_objects_colors();
}

void CglProfile::update_colors(){
  saturation  = 0.65;
  value       = 1;
  if ( colors == CGL_COLORS_FLASHY ){
    mixFactor   = 0;
    mixColor    = glm::vec3(1);
  }
  else if ( colors == CGL_COLORS_PASTEL ){
    mixFactor   = 0.5;
    mixColor    = glm::vec3(1);
  }
  else if ( colors == CGL_COLORS_WHITES ){
    mixFactor   = 0.9;
    mixColor    = glm::vec3(1);
  }
  else if ( colors == CGL_COLORS_COLD ){
    mixFactor   = 0.7;
    mixColor    = glm::vec3(0.7,0.7,1);
  }
  else if ( colors == CGL_COLORS_WARM ){
    mixFactor   = 0.8;
    mixColor    = glm::vec3(1,0.8,0.2);
  }
}

void CglProfile::update_objects_colors(){
  for(int i = 0 ; i < pcv->getObjectList()->size() ; i++){
    pCglObject obj = (*pcv->getObjectList())[i];
    if (obj->isMeshObject())
      obj->setColor(color());
  }
}





void CglProfile::update_theme(){
  if( (theme == CGL_THEME_DARK) || (theme == CGL_THEME_BLACK) ){
    grid_color = glm::vec3(0.65, 0.65, 0.65);
    idle_color = glm::vec3(0.8,  0.8,  0.8);
    sele_color = glm::vec3(1,    0.6,  0);
    displayReflection = true;
    reflection_mix = glm::vec2(0.2, 0.25);
    displayShadows = false;
    shadow_factor = 0.2;
  }
  if( (theme == CGL_THEME_CLEAR) || (theme == CGL_THEME_WHITE) ){
    grid_color = glm::vec3(0.4,  0.4,  0.4);
    idle_color = glm::vec3(0.15, 0.15, 0.15);
    sele_color = glm::vec3(1,    0.6,  0);
    displayReflection = false;
    reflection_mix = glm::vec2(0.75, 0.25);
    displayShadows = true;
    shadow_factor = 0.9;
  }
  else if ( theme == CGL_THEME_FANCY ){
    grid_color = glm::vec3(0);
    idle_color = glm::vec3(0);
    sele_color    = glm::vec3(1, 0.6, 0);
    displayReflection = true;
    reflection_mix = glm::vec2(0.2, 0.25);
    displayShadows = false;
    shadow_factor = 0.5;
  }
}

void CglProfile::switch_theme(){
  int new_theme;
  for( int t = CGL_THEME_DARK; t <= CGL_THEME_FANCY ; t++ ){
    if ( t == theme ){
      if ( t != CGL_THEME_FANCY )
        new_theme = t+1;
      else
        new_theme = 0;
    }
  }
  theme = static_cast<CGL_THEME>(new_theme);
  update_theme();
}
