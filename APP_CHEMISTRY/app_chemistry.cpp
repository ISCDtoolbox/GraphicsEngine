// Installation procedure:
//
// Download SUscCGL via github
// In SUscCGL main directory:
//   - mkdir build
//   - cd build
//   - cmake ..
//   - sudo make install
//
// Link SUscCGL include files in CMakeLists.txt
// Link against libSUscCGL.so
// In the .cpp, include "cgl.h".
// Everything should be set up and ready to go!
//

#include <cgl.h>

using namespace std;

int main(int argc, char **argv){
  CglCanvas cv(argc, argv);
  string configFile = "/home/tech/dev/SUscCGL_V2/APP_CHEMISTRY/chemistry.config"; // Remplacer avec "at"CMAKE_SOURCE_DIR"at"
  cv.getProfile()->read_configuration_file(configFile);

    pCglWindow window2 = new CglWindow(800, 0, 800, 800);

    ////////////////////////////////////////////////////////////////////////////////////////////////
    //PREMIERE SOUS FENETRE, LA PLUS GRANDE
    ////////////////////////////////////////////////////////////////////////////////////////////////

    pCglSubWindow SW3 = new CglSubWindow(window2, CGL_FULL);
    pCglScene  scene3  = new CglScene();
    pCglLight fill3    = new CglLight(0,8,15,    1,1,1,  300.0, false);
    pCglLight side3    = new CglLight(-10,5,-3,  1,1,1,  70.0,  false);
    pCglLight back3    = new CglLight(-10,5,-10, 1,1,1,  50.0,  false);

    scene3->addLight( fill3 );
    scene3->addLight( side3 );
    scene3->addLight( back3 );
    SW3->setScene(   scene3);
    cv.initGLEW();

    //scene3->addObject(new CglCube(  0,0.12,0));
    //scene3->addObject(new CglSphere(0,0,0.12));
    //scene3->addObject(new CglCylinder(  -0.025,-0.025,-0.025,   -0.025,-0.025,0.025));

    pCglSuperObject OBJ = new CglSuperObject();
    scene3->addObject(OBJ);

    std::vector<pCglSphere> atoms;
    float space = 0.05;
    float begin = -0.05;
    int dim     = 3;

    for(int i = 0 ; i < dim ; i++)
      for(int j = 0 ; j < dim ; j++)
	for(int k = 0 ; k < dim ; k++)
	  atoms.push_back(new CglSphere(begin + space * i,
					begin + space * j,
					begin + space * k));

    for(int i = 0 ; i < atoms.size() ; i++)
      OBJ->addPart(atoms[i]);

    std::vector<pCglCylinder> links;
    for(int i = 0 ; i < atoms.size() ; i++){
      for(int j = 0 ; j < atoms.size() ; j++){
	if( (i!=j) && ((i+j)%11==0) )
	  links.push_back(new CglCylinder(atoms[i], atoms[j]));
      }
    }

    for(int i = 0 ; i < links.size() ; i++)
      OBJ->addPart(links[i]);
    
    SW3->loadShaders();

  cv.loop();

  return 0;
}
