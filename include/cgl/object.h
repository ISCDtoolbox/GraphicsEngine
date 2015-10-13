/****************************************************************
 *
 * CGL: ICS Graphics Library
 *
 * Original Authors:
 *   J. Foulon, P. Frey
 *
 ****************************************************************/
#ifndef __CGL_OBJECT_H_
#define __CGL_OBJECT_H_

#include "defs.h"
#include <cgl/transform.h>
#include <cgl/material.h>

class CGL_API CglObject
{
  public:
    CglTransform transform;
    string meshFile;

  protected:
    pCglMaterial material;
    //Matrices and vectors
    glm::mat4 MODEL;
    glm::mat4 *pVIEW;
    glm::mat4 *pPROJ;
    glm::vec3 center;//
    glm::vec3 *rotationCenter;//
    float     localScale;
    float     scaleFactor;
    bool isMesh;//
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

    bool hidden;
    //Constrained movements
    bool      isRotationConstrained,   isTranslationConstrained;
    glm::vec3 constrainedRotationAxis, constrainedTranslationAxis;

  //Public methods
  public:
    CglObject();
    virtual ~CglObject();
    void linkSceneParameters(glm::mat4 *MODEL, glm::mat4 *VIEW, glm::mat4 *PROJ, glm::vec3 *Center, glm::vec3 *Up, glm::vec3 *Cam, int ID);
    void setRotationCenter(glm::vec3 &center);
    void setScaleFactor(float sf);

    void setColor(glm::vec3 col);

    void setMODEL(glm::mat4 M);
    void setCenter(glm::vec3 C);
    void setGroupID(int id);
    void setFileName(std::string n);

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
    bool  isMeshObject(){return isMesh;}
    glm::mat4 getMODEL(){return MODEL;}
    glm::vec3 getRotationCenter(){return *rotationCenter;}

    int   getGroupID();
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
    //Constrained movements
    bool  isConstrainedInRotation();
    bool  isConstrainedInTranslation();
    void  constrainRotation(glm::vec3 axis);
    void  constrainTranslation(glm::vec3 axis);
    void  setConstrainedRotation(float angle);
    void  setConstrainedTranslation(float tr);
    void  unConstrain();

protected:
  virtual void cglInit();
};

typedef CglObject* pCglObject;

#endif
