#include <cgl/object.h>
#include <cgl/canvas.h>
extern CglCanvas *pcv;

#include <cgl/scene.h>



//BUFFER OPERATIONS
void CglObject::createBuffer(GLuint *pBuffer, std::vector<float> *data){
    glGenBuffers( 1,               pBuffer);
    glBindBuffer( GL_ARRAY_BUFFER, *pBuffer);
    glBufferData( GL_ARRAY_BUFFER, sizeof(float) * data->size(), &(*data)[0], GL_STATIC_DRAW);
}
void CglObject::createBuffer(GLuint *pBuffer, std::vector<int> *data){
    glGenBuffers( 1,               pBuffer);
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, *pBuffer);
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * data->size(), &(*data)[0], GL_STATIC_DRAW);
}
void CglObject::createBuffer(GLuint *pBuffer, std::vector<short> *data){
    glGenBuffers( 1,               pBuffer);
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, *pBuffer);
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * data->size(), &(*data)[0], GL_STATIC_DRAW);
}
void CglObject::bindBuffer(int attrib, int bufferType, GLuint buffer){
    int t0 = glutGet(GLUT_ELAPSED_TIME);
    if(attrib!=-1)
        glEnableVertexAttribArray( attrib);
    glBindBuffer( bufferType, buffer);
    if(attrib!=-1)
        glVertexAttribPointer( attrib, 3, GL_FLOAT, GL_FALSE, 0, ( void*)0);
    int t1 = glutGet(GLUT_ELAPSED_TIME);
    pcv->PROF.bind += t1 - t0;
}
void CglObject::freeBuffer(){
    int t0 = glutGet(GLUT_ELAPSED_TIME);
    for(int i = 0 ; i < 5 ; i++)
        glDisableVertexAttribArray(i);
    glUseProgram(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    int t1 = glutGet(GLUT_ELAPSED_TIME);
    pcv->PROF.free += t1 - t0;
}
int CglObject::initProgram(int ID){
    glUseProgram(ID);
    for(int i = 0 ; i < 5 ; i++)
        glEnableVertexAttribArray(5);
    return ID;
}
void CglObject::draw(   int ID,       //Shader ID
                        int s,        //Number of vertices
                        int mBuffer,  //Vertexs BUffer
                        int nBuffer,  //Normals Buffer
                        int iBuffer   //Indices Buffer
                    ){

    //Vertex Buffer
    bindBuffer(0, GL_ARRAY_BUFFER, mBuffer);
    glBindAttribLocation(ID, 0, "vertex_position");
    //Normal Buffer
    if(nBuffer!=-1){
        bindBuffer(1, GL_ARRAY_BUFFER, nBuffer);
        glBindAttribLocation(ID, 1, "vertex_normal");
    }
    //Indices Buffer
    if(iBuffer!=-1){
        bindBuffer(-1, GL_ELEMENT_ARRAY_BUFFER, iBuffer);
    }
    int t0 = glutGet(GLUT_ELAPSED_TIME);
    //Drawing
    glDrawElements(GL_TRIANGLES, s, GL_UNSIGNED_INT, (void*)0);
    int t2 = glutGet(GLUT_ELAPSED_TIME);
    pcv->PROF.draw += t2 - t0;
}


void CglObject::uniform(int ID, glm::vec3 v){
    int t0 = glutGet(GLUT_ELAPSED_TIME);
  glUniform3f(ID, v.x, v.y, v.z);
  int t1 = glutGet(GLUT_ELAPSED_TIME);
    pcv->PROF.uni += t1 - t0;
}
void CglObject::uniform(int ID, glm::mat4 &m){
    int t0 = glutGet(GLUT_ELAPSED_TIME);
  glUniformMatrix4fv( ID, 1, GL_FALSE, &m[0][0]);
  int t1 = glutGet(GLUT_ELAPSED_TIME);
    pcv->PROF.uni += t1 - t0;
}
void CglObject::uniform(int ID, float f){
    int t0 = glutGet(GLUT_ELAPSED_TIME);
  glUniform1f( ID, f);
  int t1 = glutGet(GLUT_ELAPSED_TIME);
    pcv->PROF.uni += t1 - t0;
}






// object constructor
CglObject::CglObject()
{
  pcv->addObject(this);
  parent=NULL;

  selected = false;
  box      = false;
  line     = false;
  hidden   = false;
  isMesh   = false;

  MODEL  = glm::mat4(1.0f);
  //center = glm::vec3(0.0f);

  //pScene = NULL;
  material   = new CglMaterial(pcv->profile.color(), 0.85, 0.15, 12.0);
  idGroup    = -1;
  localScale = 1;
}
CglObject::~CglObject(){}

void CglObject::pickingDisplay(){
  if(!hidden){
    int shaderID = pcv->simpleID();
    glUseProgram(shaderID);
    int MatrixID = glGetUniformLocation(shaderID, "MVP");
    int colorID  = glGetUniformLocation(shaderID, "COL");

    glm::mat4 MVP = sPROJ() * sVIEW() * sMODEL() * glm::scale(MODEL, glm::vec3(scaleFactor));;

    uniform(MatrixID, MVP);
    uniform(colorID, pickingColor);

    glPolygonMode(GL_FRONT, GL_FILL);
    draw(shaderID, nPicking, meshBuffer, -1, indicesBuffer);

  }
}

void CglObject::enableFog(int ID){
    GLuint FOGID    = glGetUniformLocation(ID, "FOG");
    GLuint FOGCOLID = glGetUniformLocation(ID, "FOG_COL");
    bool fog          = (pcv->profile.theme==CGL_THEME_BLACK)||(pcv->profile.theme==CGL_THEME_WHITE);
    uniform(FOGID, float(fog));
    if(fog){
        glm::vec3 fogColor;
        if(pcv->profile.theme == CGL_THEME_BLACK)
            fogColor        = glm::vec3(0.1);
        if(pcv->profile.theme == CGL_THEME_WHITE)
            fogColor        = glm::vec3(0.9);
        uniform(FOGCOLID, fogColor);
    }
}
void CglObject::disableFog(int ID){
    GLuint FOGID    = glGetUniformLocation(ID, "FOG");
    uniform(FOGID, 0.0f);
}

void CglObject::cglInit(){}

void CglObject::applyTransformation(){
    if(parent==NULL){
        if(idGroup==-1)
            rotationCenter = &center;
    }
    else{
        if(idGroup==-1)
            rotationCenter = parent->getCenterPtr();
    }

    if ((transform.tr != glm::vec3(0.0f)) || (transform.rot != glm::mat4(1.0f))){
        glm::mat4 rot  = glm::mat4(1.0f);
        glm::mat4 ID = glm::mat4(1.0f);



        if(!isSuper())
            MODEL =  glm::translate(ID, *rotationCenter) * transform.rot * glm::translate(ID, -*rotationCenter) * MODEL;

        //if(isMesh)
        glm::vec3 bbC = 0.5f * (getBBMAX() + getBBMIN());
        glm::vec3 newMinPos = glm::vec3(pScene->getMODEL()[3]) + glm::vec3(MODEL[3]) + transform.tr + scaleFactor*(getBBMIN()-bbC);
        glm::vec3 newMaxPos = glm::vec3(pScene->getMODEL()[3]) + glm::vec3(MODEL[3]) + transform.tr + scaleFactor*(getBBMAX()-bbC);
        //if(isSelected()){cout << scaleFactor << endl;}
        glm::vec3 trans(0,0,0);
        if( (newMaxPos.x < transform.xBounds[1] ) && (newMinPos.x > transform.xBounds[0]) )
            trans.x += transform.tr.x;
        if( (newMaxPos.y < transform.yBounds[1] ) && (newMinPos.y > transform.yBounds[0]) )
            trans.y += transform.tr.y;
        if( (newMaxPos.z < transform.zBounds[1] ) && (newMinPos.z > transform.zBounds[0]) )
            trans.z += transform.tr.z;

        MODEL = glm::translate(ID, trans) * MODEL;
        center += trans;// glm::vec3(MODEL[3]);
        transform.reset();
    }
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

void CglObject::computeGroupID(){
    pCglScene scene = pcv->getScene();
    if (scene->numGroups() > 0){
      for(int i = 0 ; i < scene->numGroups() ; i++)
        for(int j = 0 ; j < scene->getGroup(i)->numObjects() ; j++){
            pCglObject pOBJ = scene->getGroup(i)->getObject(j);
            if( (this==pOBJ) || (this->parent == pOBJ))
                idGroup = i;
        }
    }
    else
      idGroup = -1;
}

void CglObject::setRotationCenter(glm::vec3 &center){rotationCenter = &center;}
void CglObject::setScaleFactor(float sf){scaleFactor = sf;}

void CglObject::setMODEL(glm::mat4 M){MODEL = M;}
void CglObject::setCenter(glm::vec3 C){center = C;}
void CglObject::setGroupID(int id){idGroup = id;}
void CglObject::setFileName(std::string n){meshFile = n;}
void CglObject::setColor(glm::vec3 col){material->setColor(col);}

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


glm::vec3 CglObject::sCENTER(){return pScene->getCenter();}
glm::vec3 CglObject::sUP(){    return pScene->getUp();}
glm::vec3 CglObject::sCAM(){   return pScene->getCam();}
glm::mat4 CglObject::sMODEL(){ return pScene->getMODEL();}
glm::mat4 CglObject::sVIEW(){  return pScene->getVIEW();}
glm::mat4 CglObject::sPROJ(){  return pScene->getPROJ();}
