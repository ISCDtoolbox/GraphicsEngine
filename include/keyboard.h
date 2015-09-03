/****************************************************************
 *
 * GLIC: ICS Graphics Library
 *
 * Original Authors:
 *   J. Foulon, P. Frey
 *
 ****************************************************************/
#ifndef __GLIC_KEYBOARD_H_
#define __GLIC_KEYBOARD_H_

#include "defs.h"

class GLIC_API CglicKeyboard
{
public:
  CglicKeyboard(){selection=-1; lastKey='*';};
  virtual ~CglicKeyboard(){};
  void keyboard(unsigned char key, int x, int y);
  void keyboardUp(unsigned char key, int x, int y);
  void special(unsigned char key, int x, int y);

private:
  unsigned char lastKey;
  int selection;
  void keyColor(unsigned char key,int x,int y);
};

#endif
