/****************************************************************
 *
 * CGL: ICS Graphics Library
 *
 * Original Authors:
 *   J. Foulon, P. Frey
 *
 ****************************************************************/
#ifndef __CGL_SCENE_H_
#define __CGL_SCENE_H_

#include "defs.h"
#include <cgl/object.h>
#include <cgl/view.h>
#include <cgl/axis.h>
#include <cgl/group.h>

class CGL_API CglScene
{
public:
  std::vector<pCglObject> listObject;
  std::vector<pCglGroup>  listGroup;
  CglTransform            transform;
  CglAxis                 *axis;
  CglView                 *view;

  float globalScale;
  glm::vec3 center;

public:
  glm::vec3 m_look, m_cam, m_up, m_look_offset;
  glm::vec3 m_right;

  glm::mat4 MODEL;
  glm::mat4 VIEW;
  glm::mat4 PROJ;

public:
  bool selected;
  int  ids;

  CglScene();
  virtual ~CglScene();
  void display();
  void addObject(pCglObject object);
  void applyTransformation();
  void saveTransformations();
  void update_matrices();
  void debug();
  int  getPickedObjectID(int x, int y);
  void reOrderObjects(int picked);
  void resetAll();
  void undoLast();

  bool isSelected();
  void select();
  void unSelect();

  void toogleFlyingMode();

  glm::vec2 cursorOrigin;

protected:
  virtual void cglInit();
};

typedef CglScene* pCglScene;


#endif
