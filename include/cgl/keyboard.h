/****************************************************************
 *
 * CGL: ICS Graphics Library
 *
 * Original Authors:
 *   J. Foulon, P. Frey
 *
 ****************************************************************/
#ifndef __CGL_KEYBOARD_H_
#define __CGL_KEYBOARD_H_

#include "defs.h"

class CGL_API CglKeyboard
{
public:
  CglKeyboard(){selection=-1; lastKey='*';};
  virtual ~CglKeyboard(){};
  void keyboard(unsigned char key, int x, int y);
  void keyboardUp(unsigned char key, int x, int y);
  void special(unsigned char key, int x, int y);

private:
  unsigned char lastKey;
  int selection;
  void keyColor(unsigned char key,int x,int y);
};

#endif
