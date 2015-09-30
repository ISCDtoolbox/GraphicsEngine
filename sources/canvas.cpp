#include <sstream>
#include <iterator>
#include <set>
#include <fstream>

#include <cgl/mesh.h>

#include <cgl/canvas.h>
using namespace std;
CglCanvas *pcv;

//Used for parsing save file
template<typename T> std::vector<T> split(const std::string& line) {
    std::istringstream is(line);
    return std::vector<T>(std::istream_iterator<T>(is), std::istream_iterator<T>());
}

///////////////////////////////////////////////////////////////////////////
//Constructor

CglCanvas::CglCanvas(int argc, char **argv, string config_file){
  pcv = this;
  glutInit(&argc, argv);

  if(pcv->profile.stereo)
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STEREO | GLUT_MULTISAMPLE);
  else
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);

  cglSetLights();

  pcv->profile.read_configuration_file(config_file);
}


///////////////////////////////////////////////////////////////////////////
//Windows, scene ... creation

int CglCanvas::cglWindow(int x, int y, int w, int h){
  window.push_back(CglWindow(x,y,w,h));
  return window.size()-1;
}

int CglCanvas::cglScene(){
  scene.push_back(new CglScene());
  scene[scene.size()-1]->setID(scene.size()-1);
  cout << "scene.size()-1: " << scene.size()-1 << endl;
  return scene.size()-1;
}

int CglCanvas::cglObject(pCglObject obj){
  object.push_back(obj);
  return object.size() - 1;
}

void CglCanvas::cglSetLights(){
  light.push_back(CglLight(glm::vec3(10,1,10),   glm::vec3(1,1,0.9),   glm::vec3(0.2, 0.65, 0.15), 200.0, 5.0, true));   //FILL
  light.push_back(CglLight(glm::vec3(-10,-10,1), glm::vec3(1,0.9,0.9), glm::vec3(0.2, 0.65, 0.15), 50.0,  5.0, true));   //SIDE
  light.push_back(CglLight(glm::vec3(1,1,-10),   glm::vec3(1,1,0.9),   glm::vec3(0.2, 0.45, 0.35), 100.0, 2.0, true));   //BACK
}

void CglCanvas::cglSetScene(int ids, int idw){
  window[idw].ids = ids;
  scene[ids]->setView(&(window[idw].view));
}

void CglCanvas::cglSetObject(int ido, int ids){
  scene[ids]->addObject(object[ido]);
}

void CglCanvas::initWindow(int &idw, int &ids, int width, int height){
  idw = cglWindow(0,0,width,height);
  ids = cglScene();
  cglSetScene(ids, idw);
  getWindow(idw)->show();
  cout << "Window ID = " << idw << endl;
  cout << "Scene  ID = " << ids << endl;

  getWindow(idw)->cglAddLight(getLight(0));
  getWindow(idw)->cglAddLight(getLight(1));
  getWindow(idw)->cglAddLight(getLight(2));

#ifndef __APPLE__
  glewExperimental = GL_TRUE;
  GLenum err = glewInit();
  if (err != GLEW_OK){
    cout << "glewinit impossible" << endl;
    exit(1);
  }
  if (!GLEW_VERSION_2_1){
    cout << "Version 2.1 pas supportée" << endl;
    exit(1);
  }
#endif
}


///////////////////////////////////////////////////////////////////////////
//Accessors

pCglLight     CglCanvas::getLight( int lightID){  return &(light[lightID]);}
pCglScene     CglCanvas::getScene( int sceneID){  return scene[sceneID]; }
pCglWindow    CglCanvas::getWindow(int windowID){ return &(window[windowID]);}
pCglScene     CglCanvas::getScene(){     return scene[window[winid()].ids]; }
pCglWindow    CglCanvas::getWindow(){    return &(window[winid()]);}

pCglInterface CglCanvas::getInterface(){
  if(profile.interface == CGL_INTERFACE_LINEAR)
    return &linearInterface;
  else if(profile.interface == CGL_INTERFACE_RADIAL)
    return &radialInterface;
}

pCglMouse     CglCanvas::getMouse(){     return &mice;}
int           CglCanvas::simpleID(){     return simpleShader.mProgramID;}
int           CglCanvas::smoothID(){     return smoothShader.mProgramID;}
int           CglCanvas::flatID(){       return flatShader.mProgramID;}

void       CglCanvas::centerMouse(){
  mice.lastPassivePos = mice.lastPos = glm::vec2(pcv->getScene()->getView()->width/2, pcv->getScene()->getView()->height/2);
}
int CglCanvas::winid(){
  int idw = glutGetWindow();
  for (int i=0; i<window.size(); i++) {
    if ( window[i].m_id == idw )  return(i);
  }
  return(0);
}


///////////////////////////////////////////////////////////////////////////
//Virtual methods

void CglCanvas::reshape(int w, int h){
  int  idw = winid();
  window[idw].view.reshape(w, h);
}

void CglCanvas::display(){
  int  idw = winid();
  window[idw].display();
  window[idw].view.setView();
}


///////////////////////////////////////////////////////////////////////////
//Glut wraps

void CglCanvas::reshapeWrap(int w, int h) {
  pcv->reshape(w, h);
}

void CglCanvas::displayWrap(){
  pcv->display();
}

void CglCanvas::motionWrap(int x, int y){
  pcv->mice.motion(x,y);
  glutPostRedisplay();
}

void CglCanvas::passiveMotionWrap(int x, int y){
  pcv->mice.passiveMotion(x,y);
  glutPostRedisplay();
}

void CglCanvas::mouseWrap(int b, int s, int x, int y){
  pcv->mice.mouse(b, s, x, y);
  glutPostRedisplay();
}

void CglCanvas::keyWrap(unsigned char key, int x, int y){
  pcv->keyboard.keyboard(key, x, y);
  glutPostRedisplay();
}

void CglCanvas::keyUpWrap(unsigned char key, int x, int y){
  pcv->keyboard.keyboardUp(key, x, y);
  glutPostRedisplay();
}

void CglCanvas::specialWrap(int key, int x, int y){
  pcv->keyboard.special(key, x, y);
  glutPostRedisplay();
}


void CglCanvas::cglMainLoop(){
  simpleShader.load("SIMPLE");
  smoothShader.load("SMOOTH");
  flatShader.load("FLAT");

  linearInterface.init(0.9);
  radialInterface.init(glm::vec2(0,0), 0.17);

  mice.setGUI();
  glutMainLoop();
}



void CglCanvas::load_meshes_from_file(string fileName, int idSce){
  vector<string>    names;
  vector<glm::mat4> mats;
  vector<glm::vec3> centers;
  vector<int>       groups;
  vector<glm::vec4> matrices;
  string line;
  int lineNumber = -1;
  int numLines = 7;
  int numberMeshes = 0;

  //Lecture du fichier de sauvegarde
  ifstream saveFile(fileName);
  if (saveFile.is_open()){
    while ( getline (saveFile,line) ){
      if (lineNumber==-1)
        numberMeshes = atoi(line.c_str());
      if (lineNumber%numLines == 0)
        names.push_back(line);
      for(int i = 1 ; i < 5 ; i++){
        if(lineNumber%numLines == i){
          glm::vec4 ROW;
          vector<float> values = split<float>(line);
          for(int j = 0 ; j < 4 ; j++){
            ROW[j] = values[j];
          }
          matrices.push_back(ROW);
        }
      }
      if(lineNumber%numLines == 5){
        centers.push_back(glm::vec3(0.0f));
        vector<double> values = split<double>(line);
        for(int i = 0 ; i < 3 ; i++){
          centers[centers.size()-1][i] = values[i];
        }
      }
      if((lineNumber%numLines == 6) && (lineNumber!=-1))
        groups.push_back(atoi(line.c_str()));
      lineNumber++;
    }
    saveFile.close();
  }
  else{
    cout << "Unable to open file";
    exit(0);
  }

  //Réajustement des matrices des matrices
  for(int i = 0 ; i < matrices.size() ; i++){
    if(i%4 == 0)
      mats.push_back(glm::mat4(1.0f));
    mats[i/4][i%4] = matrices[i];
  }

  //Upload des propriétés du maillage
  vector<CglMesh*> mesh;
  for (int i=0; i < numberMeshes; i++){
    char *N = (char*)names[i].c_str();
    mesh.push_back(new CglMesh(N));
    mesh[i]->meshInfo(0);
    int idObj = cglObject(mesh[i]);
    cglSetObject(idObj, idSce);
    mesh[i]->setCenter(centers[i]);
    mesh[i]->setMODEL(mats[i]);
    //Création des groupes
    pCglScene scene = getScene(getWindow(winid())->ids);
    set<int> indGroups(groups.begin(), groups.end());
    for (set<int>::iterator i = indGroups.begin(); i != indGroups.end(); i++) {
      std::vector<pCglObject> objectsToGroup;
      for(int j = 0 ; j < scene->numObjects() ; j++){
        if((groups[j]==*i) && (groups[j]!=-1)){
          objectsToGroup.push_back(scene->getObject(j));
        }
      }
      if(objectsToGroup.size()>1){
        scene->getGroupList()->push_back(new CglGroup(objectsToGroup));
      }
    }
    mesh[i]->setFileName(names[i]);
  }
}


