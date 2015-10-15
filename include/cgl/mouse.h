/****************************************************************
 *
 * CGL: ICS Graphics Library
 *
 * Original Authors:
 *   J. Foulon, P. Frey
 *
 ****************************************************************/
#ifndef __CGL_MOUSE_H_
#define __CGL_MOUSE_H_

#include "defs.h"
#include "interface.h"

class CGL_API CglMouse
{
public:
  glm::vec2 lastPos;
  glm::vec2 lastPassivePos;

private:
  //std::vector<pCglButton> *buttons;
  glm::vec2 currPos;
  bool arcball;
  //enum   TmouseType {TM_NONE, TM_SHIFT, TM_ALT, TM_CTRL};
  //TmouseType  m_key;
  bool   m_button[3];
  glm::vec3  m_pos;
  //unsigned int m_tm;
  bool isPressed, isReleased;

public:
  CglMouse();
  //void setGUI();
  virtual ~CglMouse(){};
  void motion(int x, int y);
  void passiveMotion(int x, int y);
  void mouse(int b, int s, int x, int y);
  void checkButtons(int b, int s, int x, int y);
  glm::vec3 projsph(glm::vec2 diff);
  glm::vec2 getCursorPosition(){return currPos;}
  glm::vec3 getMouseButtons(){return glm::vec3(m_button[0], m_button[1], m_button[2]);}
};

typedef CglMouse* pCglMouse;

#endif
