#include "cgl/profile.h"
#include "ext.h"


#include <cgl/canvas.h>
extern CglCanvas *pcv;

CglProfile::CglProfile()
{
  //Reading from cmakefile
  path              = EXT_PATH;

  displayAxes       = true;
  displayAxesLabels = false;
  displayBackgroundGradient = true;

  displayShadows    = true;
  displayBottomGrid = false;
  smooth            = false;

  displayReflection = true;

  stereo            = false;
  perspective       = true;

  classicalMode     = true;
  accumulatedMode   = false;
  flyingMode        = false;

  invertVertical    = true;

  independantScale  = false;
  globalScale       = true;

  groupRotation     = true;

  bottomDistance    = 0.1;
  bottomAngle       = 0.0;//G(float)3.14159/4;

  saturation     = 0.65;
  value          = 1;
  mixWithWhite   = 1;

  theme      = CGL_THEME_DARK;
  update_theme();
}

CglProfile::~CglProfile(){}

glm::vec3 CglProfile::color(){
  return colorGenerator.generateColor();
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
    displayReflection = false;
    reflection_mix = glm::vec2(0.2, 0.25);
    displayShadows = true;
    shadow_factor = 0.5;
  }
}

void CglProfile::switch_theme(){
  //cout << "Old theme = " << pcv->profile.theme << endl;
  int new_theme;
  for( int t = CGL_THEME_DARK; t <= CGL_THEME_FANCY ; t++ ){
    if ( t == pcv->profile.theme ){
      if ( t != CGL_THEME_FANCY )
        new_theme = t+1;
      else
        new_theme = 0;
    }
  }
  pcv->profile.theme = static_cast<CGL_THEME>(new_theme);
  update_theme();
}
