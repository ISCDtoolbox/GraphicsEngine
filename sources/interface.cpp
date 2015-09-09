#include "cgl/interface.h"

#include <cgl/canvas.h>
extern CglCanvas *pcv;

////////////////////////////////////////////////////////////////
//Button class

//Constructor
CglButton::CglButton(glm::vec2 s, glm::vec2 c, glm::vec3 col, string texturePath){
  size    = s;
  center  = c;
  color   = col;
  mins    = glm::vec2(c.x - s.x/2, c.y - s.y/2);
  maxs    = glm::vec2(c.x + s.x/2, c.y + s.y/2);
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

  glBegin(GL_QUAD_STRIP);
    glColor3f(color.x, color.y, color.z);
    glTexCoord2f(0.0, 1.0);   glVertex3f(center.x - size.x/2, center.y - size.y/2, z);
    glTexCoord2f(1.0, 1.0);   glVertex3f(center.x + size.x/2, center.y - size.y/2, z);
    glTexCoord2f(0.0, 0.0);   glVertex3f(center.x - size.x/2, center.y + size.y/2, z);
    glTexCoord2f(1.0, 0.0);   glVertex3f(center.x + size.x/2, center.y + size.y/2, z);
  glEnd();

  glDisable(GL_BLEND);
  glDisable(GL_TEXTURE_2D);
  glEnable(GL_DEPTH_TEST);
}


////////////////////////////////////////////////////////////////
//Interface class
//<a href="https://icons8.com/color-icons">Flat color icons by Icons8</a>

//Constructor
void CglInterface::init(int nb){
  isMouseOnPanel = false;
  float space = 2.0 / (nb + 1) ;

  std::vector<string> icons =  {"icon-exit.png",
                                "icon-info.png",
                                "icon-blank.png",
                                "icon-OK.png",
                                "icon-parameters.png"};
  for(int i = 0 ; i < nb ; i++){
    pCglButton b = new CglButton(glm::vec2(0.15,0.15),
                                glm::vec2(0.8, (i+1) * space - 1),
                                glm::vec3(1,1,1),
                                pcv->profile.path + "icons/" + icons[i]);
    buttons.push_back(b);
  }
}

//Display
void CglInterface::display(){
  glm::vec2 pos = pcv->getMouse()->getCursorPosition();
  int panelBorder = 650;
  if (pos.x > panelBorder){
    for(int i = 0 ; i < buttons.size() ; i++){
      buttons[i]->display();
    }
  }
}
