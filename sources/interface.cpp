#include "cgl/interface.h"

#include <cgl/canvas.h>
extern CglCanvas *pcv;

////////////////////////////////////////////////////////////////
//Button class

//Constructor
CglButton::CglButton(float s, glm::vec2 c, glm::vec3 col, string texturePath){
  size    = s;
  center  = c;
  color   = col;
  mins    = glm::vec2(c.x - s/2, c.y - s/2);
  maxs    = glm::vec2(c.x + s/2, c.y + s/2);
  //texture.loadBMP(texturePath);
  texture.loadPNG(texturePath);
}

//Display
void CglButton::display(){
  glEnable(GL_BLEND);
  glDisable(GL_DEPTH_TEST);
  glUseProgram(0);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texture.getID());

  float z = 1;
  float ratio = pcv->getScene()->getView()->ratio;

  glBegin(GL_QUAD_STRIP);
    glColor3f(color.x, color.y, color.z);
    glTexCoord2f(0.0, 1.0);   glVertex3f(center.x - (size/2)/ratio, -center.y - size/2, z);
    glTexCoord2f(1.0, 1.0);   glVertex3f(center.x + (size/2)/ratio, -center.y - size/2, z);
    glTexCoord2f(0.0, 0.0);   glVertex3f(center.x - (size/2)/ratio, -center.y + size/2, z);
    glTexCoord2f(1.0, 0.0);   glVertex3f(center.x + (size/2)/ratio, -center.y + size/2, z);
  glEnd();

  glDisable(GL_BLEND);
  glDisable(GL_TEXTURE_2D);
  glEnable(GL_DEPTH_TEST);
}


////////////////////////////////////////////////////////////////
//Interface class
//<a href="https://icons8.com/color-icons">Flat color icons by Icons8</a>



////////////////////////////////////////////////////////////////
//Linear interface

void CglLinearInterface::init(int nb, float off){
  isMouseOnPanel = false;
  float space = 2.0 / (nb + 1) ;
  std::vector<string> icons =  {"icon-info.png",
                                "icon-blank.png",
                                "icon-OK.png",
                                "icon-parameters.png",
                                "icon-exit.png"};
  for(int i = 0 ; i < nb ; i++){
    pCglButton b = new CglButton(0.15,
                                glm::vec2(off, (i+1) * space - 1),
                                glm::vec3(1,1,1),
                                pcv->profile.path + "icons/" + icons[i]);
    buttons.push_back(b);
  }
  offset = off;
}

void CglLinearInterface::display(){
  glm::vec2 pos = pcv->getMouse()->getCursorPosition();
  int panelBorder = (1 + offset - (1-offset)) * pcv->getScene()->getView()->width/2;
  //cout << panelBorder << "/" << pos.x << "/" << pos.y << endl;
  isMouseOnPanel = ((pos.x > panelBorder) ? true:false);
  if(isMouseOnPanel){
    for(int i = 0 ; i < buttons.size() ; i++){
      buttons[i]->display();
    }
  }
}


////////////////////////////////////////////////////////////////
//Radial interface

void CglRadialInterface::init(glm::vec2 cen, int nb, float rad){
  isMouseOnPanel = false;
  center = cen;
  std::vector<string> icons =  {"icon-info.png",
                                "icon-blank.png",
                                "icon-OK.png",
                                "icon-parameters.png",
                                "icon-exit.png"};
  for(int i = 0 ; i < nb ; i++){
    float angle    = 3.14159 / 2.0  +  i * (2.0 * 3.14159)/nb;
    glm::vec2  pos = rad * glm::vec2(cos(angle), sin(angle));
    pCglButton b   = new CglButton(0.15,
                                  glm::vec2(center.x + pos.x, -center.y - pos.y),
                                  glm::vec3(1,1,1),
                                  pcv->profile.path + "icons/" + icons[i]);
    buttons.push_back(b);
  }
  radius = rad;
}

void CglRadialInterface::display(){
  glm::vec2 mousePos = pcv->getMouse()->getCursorPosition();
  float w            = pcv->getScene()->getView()->width;
  float h            = pcv->getScene()->getView()->height;
  glm::vec2 pos      = glm::vec2( (mousePos.x - w/2)/(w/2), (mousePos.y - h/2)/(h/2) );
  float distance     = glm::length(center - pos);
  isMouseOnPanel     = ((distance < radius + 0.15) ? true:false);
  //cout << isMouseOnPanel << endl;
  if (isMouseOnPanel){
    for(int i = 0 ; i < buttons.size() ; i++){
      buttons[i]->display();
    }
  }
}
