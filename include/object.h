/****************************************************************
 *
 * GLIC: ICS Graphics Library
 *
 * Original Authors:
 *   J. Foulon, P. Frey
 *
 ****************************************************************/
#ifndef __GLIC_OBJECT_H_
#define __GLIC_OBJECT_H_

#include "defs.h"
#include <glic/transform.h>

class GLIC_API CglicObject
{
  public:
    CglicTransform transform;
    bool isMesh;

  protected:
    //Matrices and vectors
    glm::mat4 MODEL;
    glm::mat4 *pVIEW;
    glm::mat4 *pPROJ;
    glm::vec3 center;
    glm::vec3 *rotationCenter;
    float     localScale;
    float     scaleFactor;
    //Scene parameters
    glm::vec3 *sceneCenter;
    glm::vec3 *sceneUp;
    glm::vec3 *sceneCam;
    glm::mat4 *pMODEL;
    //Colors
    glm::vec3 face_color, edge_color;
    glm::vec3 pickingColor;
    //Buffers
    GLuint meshBuffer;
    GLuint indicesBuffer;
    //Render parameters & selection
    int objectID;
    int nPicking;
    int idGroup;
    bool selected;
    bool box;
    bool line;
    bool smooth;
    bool hidden;
    //Constrained movements
    bool      isRotationConstrained,   isTranslationConstrained;
    glm::vec3 constrainedRotationAxis, constrainedTranslationAxis;

  //Public methods
  public:
    CglicObject();
    virtual ~CglicObject();
    void linkSceneParameters(glm::mat4 *MODEL, glm::mat4 *VIEW, glm::mat4 *PROJ, glm::vec3 *Center, glm::vec3 *Up, glm::vec3 *Cam, int ID);
    void setRotationCenter(glm::vec3 &center);
    void setScaleFactor(float sf);

    virtual void display(){};
    virtual void artifactsDisplay(){};
    virtual void shadowsDisplay(){};
    //virtual void pickingDisplay(){};
    void pickingDisplay();

    void applyTransformation();
    void saveTransformations();
    void undoLast();
    void resetAll();
    void uniformVec3(int ID, glm::vec3 v);

  //Public Accessors
  public:
    virtual glm::vec3 getBBMIN(){};
    virtual glm::vec3 getBBMAX(){};
    int   getID();
    float getLocalScale();
    void  resetGroupID();
    glm::vec3* getCenterPtr();
    //Selection & picking
    bool  isPicked(int ID);
    bool  isHidden();
    void  hide();
    void  unHide();
    bool  isSelected();
    void  toogleSelected();
    void  select();
    void  unSelect();
    //render modes
    void  toogleBBox();
    void  toogleMesh();
    void  toogleSmooth();
    //Constrained movements
    bool  isConstrainedInRotation();
    bool  isConstrainedInTranslation();
    void  constrainRotation(glm::vec3 axis);
    void  constrainTranslation(glm::vec3 axis);
    void  setConstrainedRotation(float angle);
    void  setConstrainedTranslation(float tr);
    void  unConstrain();

protected:
  virtual void glicInit();
};

typedef CglicObject* pCglicObject;

#endif
