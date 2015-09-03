/****************************************************************
 *
 * GLIC: ICS Graphics Library
 *
 * Original Authors:
 *   J. Foulon, P. Frey
 *
 ****************************************************************/
#ifndef __GLIC_MOUSE_H_
#define __GLIC_MOUSE_H_

#include "defs.h"

class GLIC_API CglicMouse
{
public:
  glm::vec2 lastPos;
  glm::vec2 lastPassivePos;
private:
  glm::vec2 currPos;
  bool arcball;

  //enum   TmouseType {TM_NONE, TM_SHIFT, TM_ALT, TM_CTRL};
  //TmouseType  m_key;
  bool   m_button[3];
  glm::vec3  m_pos;
  //unsigned int m_tm;

  glm::vec3 projsph(glm::vec2 diff);

  bool isPressed, isReleased;

public:
  CglicMouse();
  virtual ~CglicMouse(){};
  void motion(int x, int y);
  void passiveMotion(int x, int y);
  void mouse(int b, int s, int x, int y);
  void transform();
};


#endif
