#include "cgl/profile.h"
#include "ext.h"

//Config file reading
#include <iostream>
#include <iomanip>
#include <string>
#include <exception>
#include <fstream>
#include <vector>
#include <algorithm>


#include <cgl/canvas.h>
extern CglCanvas *pcv;

CglProfile::CglProfile()
{
  path              = EXT_PATH;
  read_configuration_file("");
  update_colors();
  update_theme();
}


void CglProfile::read_configuration_file(string fileName){

  //Fix defaults parameters
  //Fixed variables
  invertVertical    = false;
  displayAxes       = false;
  independantScale  = false;
  globalScale       = false;
  bottomDistance    = 0.1;
  bottomAngle       = 0.0;
  //Modifiable variables
  displayBottomGrid = false;
  stereo            = false;
  perspective       = false;
  smooth            = false;
  flyingMode        = false;
  fullscreen        = false;
  //Default parameters
  initial_arrangement = CGL_ARRANGEMENT_GRID;
  camera              = CGL_CAM_UPPER_SPHERE;
  interface           = CGL_INTERFACE_LINEAR;
  buttons             = CGL_BUTTONS_EXTENDED;
  colors              = CGL_COLORS_FLASHY;
  theme               = CGL_THEME_DARK;
  ground              = CGL_GROUND_SHADOWS;

  if(fileName != ""){
    std::ifstream in;
    in.open(fileName);
    std::string str;

    if(!in.is_open()){
      cerr << "Invalid conf file: " + fileName << endl;
      exit(1);
    }

    while(!in.eof()) {
      while(getline(in,str)) {
	std::string::size_type begin = str.find_first_not_of(" \f\t\v");
	//Skips blank lines
	if(begin == std::string::npos)
	  continue;
	//Skips #
	if(std::string("#").find(str[begin]) != std::string::npos)
	  continue;
	std::string firstWord;
	try {
	  firstWord = str.substr(0,str.find(" "));
	}
	catch(std::exception& e) {
	  firstWord = str.erase(str.find_first_of(" "),str.find_first_not_of(" "));
	}
	std::transform(firstWord.begin(),firstWord.end(),firstWord.begin(), ::toupper);

	//Enums
	if(firstWord == "INITIAL_ARRANGEMENT")
	  initial_arrangement = static_cast<CGL_ARRANGEMENT>(stoi(str.substr(str.find(" ")+1,str.length())));
	if(firstWord == "CAMERA")
	  camera              = static_cast<CGL_CAM>(stoi(str.substr(str.find(" ")+1,str.length())));
	if(firstWord == "INTERFACE")
	  interface           = static_cast<CGL_INTERFACE>(stoi(str.substr(str.find(" ")+1,str.length())));
	if(firstWord == "BUTTONS")
	  buttons             = static_cast<CGL_BUTTONS>(stoi(str.substr(str.find(" ")+1,str.length())));
	if(firstWord == "COLORS")
	  colors              = static_cast<CGL_COLORS>(stoi(str.substr(str.find(" ")+1,str.length())));
	if(firstWord == "THEME")
	  theme               = static_cast<CGL_THEME>(stoi(str.substr(str.find(" ")+1,str.length())));
	//if(firstWord == "GROUND")
	//	ground              = static_cast<CGL_GROUND>(stoi(str.substr(str.find(" ")+1,str.length())));

	//Fixed variables
	if(firstWord == "INVERT_VERTICAL")
	  invertVertical = true;
	if(firstWord == "DISPLAY_AXES")
	  displayAxes    = true;
	if(firstWord == "INDEPENDANT_SCALE")
	  independantScale = true;
	if(firstWord == "GLOBAL_SCALE")
	  globalScale = true;
	if(firstWord == "STEREO")
	  stereo = true;
	if(firstWord == "BOTTOM_DISTANCE")
	  bottomDistance = stof(str.substr(str.find(" ")+1,str.length()));
	if(firstWord == "BOTTOM_ANGLE")
	  bottomAngle    = stof(str.substr(str.find(" ")+1,str.length()));

	//Start up, launching application
	if(firstWord == "PERSPECTIVE")
	  perspective = true;
	if(firstWord == "BOTTOM_GRID")
	  displayBottomGrid = true;
	if(firstWord == "SMOOTH")
	  smooth = true;
	if(firstWord == "FULLSCREEN")
	  fullscreen = true;
	if(firstWord == "FLYING_MODE")
	  flyingMode = true;
      }
    }
  }
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
  //Theme update
  if( (theme == CGL_THEME_DARK) || (theme == CGL_THEME_BLACK) ){
    grid_color = glm::vec3(0.65, 0.65, 0.65);
    idle_color = glm::vec3(0.8,  0.8,  0.8);
    sele_color = glm::vec3(1,    0.6,  0);
    ground     = CGL_GROUND_REFLECTION;
  }
  if( (theme == CGL_THEME_CLEAR) || (theme == CGL_THEME_WHITE) ){
    grid_color = glm::vec3(0.4,  0.4,  0.4);
    idle_color = glm::vec3(0.15, 0.15, 0.15);
    sele_color = glm::vec3(1,    0.6,  0);
    ground         = CGL_GROUND_SHADOWS;
  }

  //Ground update
  if( ground == CGL_GROUND_SHADOWS ){
    displayReflection = false;
    displayShadows    = true;
    shadow_factor = 0.9;
  }
  else if ( ground == CGL_GROUND_REFLECTION ){
    displayReflection = true;
    displayShadows    = false;
    reflection_mix = glm::vec2(0.2, 0.25);
  }
  else if ( ground == CGL_GROUND_NONE ){
    displayReflection = false;
    displayShadows    = false;
  }
}

void CglProfile::switch_theme(){
  int new_theme;
  for( int t = CGL_THEME_DARK; t <= CGL_THEME_WHITE ; t++ ){
    if ( t == theme ){
      if ( t != CGL_THEME_WHITE )
        new_theme = t+1;
      else
        new_theme = 0;
    }
  }
  theme = static_cast<CGL_THEME>(new_theme);
  update_theme();
}
