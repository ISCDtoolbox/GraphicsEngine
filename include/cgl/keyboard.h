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

class CglScene;
typedef CglScene* pCglScene;

typedef void (*pAction)(pCglScene);

class CGL_API CglKeyboard
{
public:
  CglKeyboard(){selection=-1; lastKey='*';};
  virtual ~CglKeyboard(){};
  void keyboard(unsigned char key, int x, int y);
  void keyboardUp(unsigned char key, int x, int y);
  void special(unsigned char key, int x, int y);

  void addAction(unsigned char key, pAction func){
    customActions.push_back(func);
    customKeys.push_back(key);
  }

private:
    std::vector<pAction>        customActions;
    std::vector<unsigned char>  customKeys;
    unsigned char               lastKey;
    int                         selection;
  void keyColor(unsigned char key,int x,int y);
};

typedef CglKeyboard* pCglKeyboard;

#endif
