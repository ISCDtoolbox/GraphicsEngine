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
  string configFile = "/home/foulonj/dev/SUscCGL_V2/APP_NumeRO/numero.config";
  cv.getProfile()->read_configuration_file(configFile);

  if((argc==2) && (string(argv[1])=="cgl.save")){
    //cv.initWindow(idw, ids, 400, 400);
    //cv.load_meshes_from_file("cgl.save", ids);
  }

  else if (argc > 1){  
    pCglWindow window = new CglWindow(0, 0, 800, 800);   
    pCglScene  scene  = new CglScene();
    pCglView   view   = new CglView();
    pCglLight fill    = new CglLight(0,8,15,    1,1,1,  300.0, false);
    pCglLight side    = new CglLight(-10,5,-3,  1,1,1,  70.0,  false);
    pCglLight back    = new CglLight(-10,5,-10, 1,1,1,  50.0,  false);
    //lights.push_back(CglLight(glm::vec3(0,0,10),  glm::vec3(1), 70.0,  true));   //FILL
    //lights.push_back(CglLight(glm::vec3(-10,0,2), glm::vec3(1), 40.0,  true));   //SIDE
    //lights.push_back(CglLight(glm::vec3(3,3,-10), glm::vec3(1), 30.0,  true));   //BACK
    
    window->setView( view);
    scene->addLight( fill );
    scene->addLight( side );
    scene->addLight( back );
    window->setScene(scene);

    cv.initGLEW();
    
    for (int i=0; i < argc - 1; i++)
      scene->addObject(new CglMesh(argv[i+1]));
    if(cv.profile.initial_arrangement == CGL_ARRANGEMENT_GRID)
      scene->place_objects_on_grid();

    //scene->init();
    //window->init();
    window->loadShaders();

    
    pCglWindow subWindow1 = new CglSubWindow(window, 0, 0, 300, 300);   
    pCglScene  scene2     = new CglScene();
    pCglView   view2      = new CglView();
    pCglLight fill2       = new CglLight(0,8,15,    1,1,1,  300.0, false);
    pCglLight side2       = new CglLight(-10,5,-3,  1,0,0,  70.0,  false);
    pCglLight back2       = new CglLight(-10,5,-10, 0,0,1,  50.0,  false);
    //lights.push_back(CglLight(glm::vec3(0,0,10),  glm::vec3(1), 70.0,  true));   //FILL
    //lights.push_back(CglLight(glm::vec3(-10,0,2), glm::vec3(1), 40.0,  true));   //SIDE
    //lights.push_back(CglLight(glm::vec3(3,3,-10), glm::vec3(1), 30.0,  true));   //BACK
    subWindow1->setView( view2);
    scene2->addLight( fill2 );
    scene2->addLight( side2 );
    scene2->addLight( back2 );
    subWindow1->setScene(scene2);
    
    cv.initGLEW();
    
    for (int i=0; i < 2; i++)
      scene2->addObject(new CglMesh(argv[i+1]));
    if(cv.profile.initial_arrangement == CGL_ARRANGEMENT_GRID)
      scene2->place_objects_on_grid();

    subWindow1->loadShaders();
    
  }
  
  else{ 
    cout << "Invalid arguments" << endl;
    return 1;
  }
  

  cv.cglMainLoop();

  return 0;
}
