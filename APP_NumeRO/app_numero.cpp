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
  string configFile = "/home/tech/dev/SUSCCGL/APP_NumeRO/numero.config";
  cv.getProfile()->read_configuration_file(configFile);

  if((argc==2) && (string(argv[1])=="cgl.save")){
    //cv.initWindow(idw, ids, 400, 400);
    //cv.load_meshes_from_file("cgl.save", ids);
  }

  else if (argc > 1){



    /*
    ////////////////////////////////////////////////////////////////////////////////////////////////
    //PREMIERE SOUS FENETRE, LA PLUS GRANDE
    ////////////////////////////////////////////////////////////////////////////////////////////////
    pCglWindow window = new CglWindow(0, 0, 800, 800);

    pCglSubWindow SW1 = new CglSubWindow(window, CGL_TOP);
    pCglScene  scene  = new CglScene();
    pCglLight fill    = new CglLight(0,8,15,    1,1,1,  300.0, false);
    pCglLight side    = new CglLight(-10,5,-3,  1,1,1,  70.0,  false);
    pCglLight back    = new CglLight(-10,5,-10, 1,1,1,  50.0,  false);

    scene->addLight( fill );
    scene->addLight( side );
    scene->addLight( back );
    SW1->setScene(   scene);
    cv.initGLEW();

    for (int i=0; i < argc - 1; i++)
      scene->addObject(new CglMesh(argv[i+1]));
    if(cv.profile.initial_arrangement == CGL_ARRANGEMENT_GRID)
      scene->place_objects_on_grid();

    SW1->loadShaders();



    ////////////////////////////////////////////////////////////////////////////////////////////////
    //PREMIERE SOUS FENETRE, LA PLUS GRANDE
    ////////////////////////////////////////////////////////////////////////////////////////////////

    pCglSubWindow SW2     = new CglSubWindow(window, CGL_BOTTOM);//400, 0, 400, 800);
    pCglScene  scene2     = new CglScene();
    pCglLight fill2       = new CglLight(0,8,15,    1,1,1,  300.0, false);
    pCglLight side2       = new CglLight(-10,5,-3,  1,0,0,  70.0,  false);
    pCglLight back2       = new CglLight(-10,5,-10, 0,0,1,  50.0,  false);

    scene2->addLight( fill2 );
    scene2->addLight( side2 );
    scene2->addLight( back2 );
    SW2->setScene(scene2);

    cv.initGLEW();

    for (int i=0; i < 2; i++)
      scene2->addObject(new CglMesh(argv[i+1]));
    if(cv.profile.initial_arrangement == CGL_ARRANGEMENT_GRID)
      scene2->place_objects_on_grid();

    SW2->loadShaders();
    */








    pCglWindow window2 = new CglWindow(800, 0, 800, 800);

    ////////////////////////////////////////////////////////////////////////////////////////////////
    //PREMIERE SOUS FENETRE, LA PLUS GRANDE
    ////////////////////////////////////////////////////////////////////////////////////////////////

    pCglSubWindow SW3 = new CglSubWindow(window2, CGL_LEFT, 0.3);
    pCglScene  scene3  = new CglScene();
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
      scene3->place_objects_on_grid();

    

    SW3->loadShaders();


    ////////////////////////////////////////////////////////////////////////////////////////////////
    //PREMIERE SOUS FENETRE, LA PLUS GRANDE
    ////////////////////////////////////////////////////////////////////////////////////////////////

    pCglSubWindow SW4     = new CglSubWindow(window2, CGL_RIGHT, 0.7);//400, 0, 400, 800);
    pCglScene  scene4     = new CglScene();
    pCglLight fill4       = new CglLight(0,8,15,    1,1,1,  300.0, false);
    pCglLight side4       = new CglLight(-10,5,-3,  1,0.9,0.8,  70.0,  false);
    pCglLight back4       = new CglLight(-10,5,-10, 0.3,0.3,1,  50.0,  false);

    scene4->addLight( fill4 );
    scene4->addLight( side4 );
    scene4->addLight( back4 );
    SW4->setScene(scene4);

    cv.initGLEW();

    for (int i=3; i < 5; i++)
      scene4->addObject(new CglMesh(argv[i+1]));
    if(cv.profile.initial_arrangement == CGL_ARRANGEMENT_GRID)
      scene4->place_objects_on_grid();

    //scene4->addObject( new CglSphere(0,0,0, 1,1,1, 5) );

    SW4->loadShaders();
  }

  else{
    cout << "Invalid arguments" << endl;
    return 1;
  }

  cv.loop();

  return 0;
}
