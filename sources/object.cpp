#include <cgl/object.h>
#include <cgl/canvas.h>
extern CglCanvas *pcv;

void CglObject::uniformVec3(int ID, glm::vec3 v){
  glUniform3f(ID, v.x, v.y, v.z);
}

// object constructor
CglObject::CglObject()
{
  pcv->addObject(this);

  selected = false;
  box      = false;
  line     = false;
  hidden   = false;
  isMesh   = false;

  MODEL  = glm::mat4(1.0f);
  center = glm::vec3(0.0f);
  pPROJ  = NULL;
  pVIEW  = NULL;

  face_color = pcv->profile.color();
  material   = new CglMaterial(face_color, 0.85, 0.15, 12.0);
  edge_color = 0.8f * face_color;
  idGroup    = -1;
  localScale = 1;
}

CglObject::~CglObject(){}

void CglObject::linkSceneParameters(glm::mat4 *MODEL, glm::mat4 *VIEW, glm::mat4 *PROJ, glm::vec3 *Center, glm::vec3 *Up, glm::vec3 *Cam, int ID){
  pPROJ        = PROJ;
  pVIEW        = VIEW;
  pMODEL       = MODEL;
  sceneCenter  = Center;
  sceneUp      = Up;
  sceneCam     = Cam;
  objectID     = ID;
  pickingColor = glm::vec3(objectID/255.0f, objectID/255.0f, 0);
}

void CglObject::pickingDisplay(){

  if(!hidden){
    int shaderID = pcv->simpleID();
    glUseProgram(shaderID);
    int MatrixID = glGetUniformLocation(shaderID, "MVP");
    int colorID  = glGetUniformLocation(shaderID, "COL");
    glm::mat4 MVP = *pPROJ * *pVIEW * *pMODEL * glm::scale(MODEL, glm::vec3(scaleFactor));;
    glUniformMatrix4fv( MatrixID, 1, GL_FALSE, &MVP[0][0]);
    //Mesh buffer binding
    glEnableVertexAttribArray( 0);
    glBindBuffer(              GL_ARRAY_BUFFER, meshBuffer);
    glVertexAttribPointer(     0, 3, GL_FLOAT, GL_FALSE, 0, ( void*)0);
    glBindAttribLocation(      shaderID, 0, "vertex_position");
    //Indices buffer binding
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBuffer);
    //Faces
    uniformVec3(colorID, pickingColor);
    glPolygonMode(GL_FRONT, GL_FILL);
    glDrawElements(GL_TRIANGLES, nPicking, GL_UNSIGNED_INT, (void*)0);
  }
}

void CglObject::cglInit()
{}

void CglObject::applyTransformation()
{
  glm::mat4 ID = glm::mat4(1.0f);
  if(idGroup==-1)
    rotationCenter = &center;
  MODEL =  glm::translate(ID, *rotationCenter) * transform.rot * glm::translate(ID, -*rotationCenter) * MODEL;
  if(isMesh)
    MODEL = glm::translate(ID, transform.tr) * MODEL;
  center += transform.tr;// glm::vec3(MODEL[3]);
  transform.reset();
}
void CglObject::saveTransformations(){
  transform.lastMatrices.push_back(MODEL);
}
void CglObject::undoLast(){
  if(transform.lastMatrices.size()>0){
    MODEL = transform.lastMatrices.back();
    center = glm::vec3(glm::vec4(MODEL[3]));
    transform.lastMatrices.pop_back();
  }
}
void CglObject::resetAll(){
  while(transform.lastMatrices.size()>0)
    undoLast();
}


void CglObject::setRotationCenter(glm::vec3 &center){rotationCenter = &center;}
void CglObject::setScaleFactor(float sf){scaleFactor = sf;}

void CglObject::setMODEL(glm::mat4 M){MODEL = M;}
void CglObject::setCenter(glm::vec3 C){center = C;}
void CglObject::setGroupID(int id){idGroup = id;}
void CglObject::setFileName(std::string n){meshFile = n;}
void CglObject::setColor(glm::vec3 col){face_color = col; edge_color = 0.5f * col;}

int        CglObject::getGroupID(){return idGroup;}
void       CglObject::resetGroupID(){idGroup = -1;}
float      CglObject::getLocalScale(){return localScale;}
int        CglObject::getID(){return objectID;}
glm::vec3* CglObject::getCenterPtr(){return &center;}

//Toogle render modes
void CglObject::toogleBBox()   {box        = !box;}
void CglObject::toogleMesh()   {line       = !line;}

//Selection accessors
bool CglObject::isPicked(int ID){return objectID == ID;}
bool CglObject::isHidden(){return hidden;}
void CglObject::hide(){    hidden = true;}
void CglObject::unHide(){  hidden = false;}
bool CglObject::isSelected(){return selected;}
void CglObject::toogleSelected(){    selected = !selected;}
void CglObject::select(){            selected = true;}
void CglObject::unSelect(){          selected = false;}

//Constrained movements accessors
bool CglObject::isConstrainedInRotation(){return isRotationConstrained;}
bool CglObject::isConstrainedInTranslation(){return isTranslationConstrained;};
void CglObject::constrainRotation(glm::vec3 axis){
  isRotationConstrained = true;
  constrainedRotationAxis = axis;
}
void CglObject::constrainTranslation(glm::vec3 axis){
  isTranslationConstrained = true;
  constrainedTranslationAxis = axis;
}
void CglObject::unConstrain(){
  isRotationConstrained = isTranslationConstrained = false;
  constrainedRotationAxis = constrainedTranslationAxis = glm::vec3(0);
};
void CglObject::setConstrainedRotation(float angle){transform.setRotation(glm::rotate(glm::mat4(1), angle, constrainedRotationAxis));}
void CglObject::setConstrainedTranslation(float tr){transform.tr += tr * constrainedTranslationAxis;}
