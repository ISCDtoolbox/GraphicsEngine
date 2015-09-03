/****************************************************************
 *
 * GLIC: ICS Graphics Library
 *
 * Original Authors:
 *   J. Foulon, P. Frey
 *
 ****************************************************************/
#ifndef __GLIC_SCENE_H_
#define __GLIC_SCENE_H_

#include "defs.h"
#include <glic/object.h>
#include <glic/view.h>
#include <glic/axis.h>
#include <glic/group.h>

class GLIC_API CglicScene
{
public:
  std::vector<pCglicObject> listObject;
  std::vector<pCglicGroup>  listGroup;
  CglicTransform            transform;
  CglicAxis                 *axis;
  CglicView                 *view;

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

  CglicScene();
  virtual ~CglicScene();
  void display();
  void addObject(pCglicObject object);
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
  virtual void glicInit();
};

typedef CglicScene* pCglicScene;


#endif
