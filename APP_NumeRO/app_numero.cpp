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
// In the .cpp, include "cgl.h".h
// Everything should be set up and ready to go!
//

#include <cgl.h>

using namespace std;

int main(int argc, char **argv){
  CglCanvas cv(argc, argv);
  string configDir = "@CMAKE_SOURCE_DIR@/"; 

  if (argc > 1){
    if(argc>2){

      cv.getProfile()->read_configuration_file(configDir + "numero.config");

      //OUVERTURE DE TOUS LES OBJETS
      pCglWindow window = new CglWindow(800, 0, 800, 800);

      pCglSubWindow SW3 = new CglSubWindow(window, CGL_LEFT, 0.3);
      pCglScene  scene3  = new CglScene(CGL_GALERY);
      pCglLight fill3    = new CglLight(0,8,15,    1,1,1,  300.0, false);
      pCglLight side3    = new CglLight(-10,5,-3,  1,1,1,  70.0,  false);
      pCglLight back3    = new CglLight(-10,5,-10, 1,1,1,  50.0,  false);
      scene3->addLight( fill3 );
      scene3->addLight( side3 );
      scene3->addLight( back3 );
      SW3->setScene(   scene3);
      cv.initGLEW();
      for (int i=0; i < argc-1; i++)
        scene3->addObject(new CglMesh(argv[i+1]));
      if(cv.profile.initial_arrangement == CGL_ARRANGEMENT_GRID)
        scene3->place_objects_on_column();
      SW3->loadShaders();

      pCglSubWindow SW4     = new CglSubWindow(window, CGL_RIGHT, 0.7);//400, 0, 400, 800);
      pCglScene  scene4     = new CglScene(CGL_MANIPULATION);
      pCglLight fill4       = new CglLight(0,8,15,    1,1,1,  300.0, false);
      pCglLight side4       = new CglLight(-10,5,-3,  1,1,1,  70.0,  false);
      pCglLight back4       = new CglLight(-10,5,-10, 1,1,1,  50.0,  false);
      scene4->addLight( fill4 );
      scene4->addLight( side4 );
      scene4->addLight( back4 );
      SW4->setScene(scene4);
      cv.initGLEW();
      for (int i=3; i < 5; i++)
        scene4->addObject(new CglMesh(argv[i+1]));
      if(cv.profile.initial_arrangement == CGL_ARRANGEMENT_GRID)
        scene4->place_objects_on_grid();
      SW4->loadShaders();
    }

    else if(argc == 2){
      //FICHIER DE SAUVEGARDE
      if(argv[1] == string("save")){
	
	cv.getProfile()->read_configuration_file(configDir + "numero.config");

	 pCglWindow window = new CglWindow(800, 0, 800, 800);

	 pCglSubWindow SW3 = new CglSubWindow(window, CGL_LEFT, 0.3);
	 pCglScene  scene3  = new CglScene(CGL_GALERY);
	 pCglLight fill3    = new CglLight(0,8,15,    1,1,1,  300.0, false);
	 pCglLight side3    = new CglLight(-10,5,-3,  1,1,1,  70.0,  false);
	 pCglLight back3    = new CglLight(-10,5,-10, 1,1,1,  50.0,  false);
	 scene3->addLight( fill3 );
	 scene3->addLight( side3 );
	 scene3->addLight( back3 );
	 SW3->setScene(   scene3);
	 cv.initGLEW();
	 scene3->load_meshes_from_file("cgl.save_0");
	 SW3->loadShaders();

	 pCglSubWindow SW4     = new CglSubWindow(window, CGL_RIGHT, 0.7);//400, 0, 400, 800);
	 pCglScene  scene4     = new CglScene(CGL_MANIPULATION);
	 pCglLight fill4       = new CglLight(0,8,15,    1,1,1,  300.0, false);
	 pCglLight side4       = new CglLight(-10,5,-3,  1,1,1,  70.0,  false);
	 pCglLight back4       = new CglLight(-10,5,-10, 1,1,1,  50.0,  false);
	 scene4->addLight( fill4 );
	 scene4->addLight( side4 );
	 scene4->addLight( back4 );
	 SW4->setScene(scene4);
	 cv.initGLEW();
	 scene4->load_meshes_from_file("cgl.save_1");
	 SW4->loadShaders();
      }
      else{

	cv.getProfile()->read_configuration_file(configDir + "manip.config");
	//MANIPULATION D'UN SEUL MAILLAGE
	pCglWindow window = new CglWindow(800, 0, 800, 800);
	pCglSubWindow SW3 = new CglSubWindow(window, CGL_FULL);
	pCglScene  scene3  = new CglScene(CGL_MANIPULATION);
	pCglLight fill3    = new CglLight(0,8,15,    1,1,1,  300.0, false);
	pCglLight side3    = new CglLight(5,-5,-3,  1,1,1,  70.0,  false);
	pCglLight back3    = new CglLight(-10,5,-10, 1,1,1,  50.0,  false);
	scene3->addLight( fill3 );
	scene3->addLight( side3 );
	scene3->addLight( back3 );
	SW3->setScene(   scene3);
	cv.initGLEW();
	for (int i=0; i < argc-1; i++)
	  scene3->addObject(new CglMesh(argv[1]));
	SW3->loadShaders();
      }
    }
  }

  else{
    cout << "Please enter Arguments" << endl;
    return 1;
  }

  cv.loop();

  return 0;
}
