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
#include <cgl/mesh.h>
#include <cgl/view.h>
#include <cgl/axis.h>
#include <cgl/group.h>
#include <cgl/light.h>

class CGL_API CglScene
{

/////////////////////////////////////////////////////
//Attributes

private:

  //Instances of cgl objects
  std::vector<pCglObject> listObject;
  std::vector<pCglGroup>  listGroup;
  CglTransform            transform;
  pCglAxis                axis;
  pCglBackground          background;
  pCglView                view;
  int                     windowID;

  std::vector<pCglLight>  lights;


  //View parameters
  glm::vec3               m_look, m_cam, m_up, m_right;
  glm::vec3               m_look_offset;
  glm::mat4               MODEL, VIEW, PROJ;
  glm::vec3               center;

  //Properties
  bool                    selected;
  float                   globalScale;

/////////////////////////////////////////////////////
//Methods

public:

    void setWindowID(int ID){windowID = ID;}
    int addLight(pCglLight li);
    std::vector<pCglLight> getLights(){return lights;}

  //Constructors
  CglScene();
  virtual ~CglScene();
  void load_meshes_from_file(string fileName);

  //Accessors and setters for Canvas objects
  void                     addObject(pCglObject object);
  std::vector<pCglObject>* getObjectList(){return &listObject;}
  std::vector<pCglGroup>*  getGroupList(){ return &listGroup;}
  int                      numObjects(){return listObject.size();}
  int                      numGroups(){ return listGroup.size();}
  pCglObject               getObject(int indObject){ return listObject[indObject]; }
  pCglGroup                getGroup( int indGroup){  return listGroup[indGroup]; }
  pCglAxis                 getAxis(){return axis;}
  pCglView                 getView(){return view;}
  void                     setView(pCglView newView){view = newView;}
  pCglBackground           getBackground(){return background;}
  pCglTransform            getTransform(){return &transform;}

  //Accessors for matrices
  glm::mat4                 getMODEL(){ return MODEL;}
  glm::mat4                 getVIEW(){  return VIEW;}
  glm::mat4                 getPROJ(){  return PROJ;}
  glm::vec3                 getCenter(){return center;}

  //Accessors and setters for camera parameters
  glm::vec3                getRight(){  return m_right;}
  glm::vec3                getLook(){   return m_look;}
  glm::vec3                getCam(){    return m_cam;}
  glm::vec3                getUp(){     return m_up;}
  void                     setCam(glm::vec3 newCam){m_cam = newCam;}
  void                     setUp( glm::vec3 newUp){ m_up  = newUp;}
  void                     setLook(glm::vec3 newL){ m_look= newL;}
  void                     toogleFlyingMode();

  //Transformations
  void                     applyTransformation();
  void                     saveTransformations();
  void                     update_matrices();
  void                     undoLast();
  void                     resetAll();
  void                     place_objects_on_grid();
  void                     save();

  //Picking
  pCglObject               getPicked(int x, int y);
  void                     onPick(bool ctrl, pCglObject obj);
  void                     reOrderObjects(pCglObject obj);
  glm::vec3                getRayVector(int x, int y);

  //Mouse actions
  glm::vec2                lastDrag;
  glm::vec3                intersect(glm::vec3 ray,
				     glm::vec3 plane,
				     glm::vec3 planeNormal,
				     bool      &intersects);
  float                    orientedAngle(glm::vec3 pt1,
					 glm::vec3 pt2,
					 glm::vec3 c,
					 glm::vec3 ax);
  void                     onDrag(         int x, int y);
  void                     onLeftDrag(     int x, int y);
  void                     onMiddleDrag(   int x, int y);
  void                     onRightDrag(    int x, int y);
  void                     onClick(        int x, int y);
  void                     onLeftClick(    int x, int y);
  void                     onMiddleClick(  int x, int y);
  void                     onRightClick(   int x, int y);
  void                     onRelease(      int x, int y);
  void                     onLeftRelease(  int x, int y);
  void                     onMiddleRelease(int x, int y);
  void                     onRightRelease( int x, int y);


  //ID and selection
  bool                     isSelected();
  void                     select();
  void                     unSelect();

  //Display
  void                     display();
  void                     debug();


/////////////////////////////////////////////////////
//Init

protected:
  virtual void cglInit();

};

typedef CglScene* pCglScene;


#endif
