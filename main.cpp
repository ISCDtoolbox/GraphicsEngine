#include <iostream>
#include <cgl.h>
#include <cstdlib>

using namespace std;


double frand_a_b(double a, double b){
  return ( rand()/(double)RAND_MAX ) * (b-a) + a;
}

void InitGlew(){
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

int main(int argc, char **argv){

  int   idw,  ids,  ido;

  CglCanvas cv(argc,argv);

  if (argc <=1){

    idw = cv.cglWindow(0,0,1000,1000);
    //cout << "\t Id window: " << idw << endl;
    ids = cv.cglScene();
    //cout << "\t Id scene: " << ids << endl;
    cv.cglSetScene(ids, idw);



    cv.window[idw].show();
    InitGlew();


    //cv.window[idw].cglAddLight(&cv.light[0]);
    //cv.window[idw].cglAddLight(&cv.light[1]);
    //cv.window[idw].cglAddLight(&cv.light[2]);

    ido = cv.cglObject(new CglCube(0., 0.2, 0.7, 1.));
    cv.cglSetObject(ido, ids);

    ido = cv.cglObject(new CglSphere(0., 0.2, 0., 1.5));
    //ido = cv.cglObject(new CglCube(0.7, 0.2, 0., 1.));
    cv.cglSetObject(ido, ids);

    //for (int io = 0; io < 5; io++){
    // ido = cv.cglObject(new CglCube(frand_a_b(0,1), frand_a_b(0,1), 1.0, frand_a_b(0.,1.)));
    //  cv.cglSetObject(ido, ids);
    //}



    ido = cv.cglObject(new CglAxis());
    cv.cglSetObject(ido, ids);

    //ido = cv.cglObject(new CglSphere(0.1));
    //cv.cglSetObject(ido, ids);


    //idw = cv.cglWindow(600,600,400,400);
    //cout << "Id window: " << idw << endl;
    //ids = cv.cglScene();

    //cout << "Id scene: " << ids << endl;
    //cv.cglSetScene(ids, idw);
    //cv.window[idw].show();

    //ido = cv.cglObject(new CglCube(0.7, 0.2, 0., 1.));

    //cv.cglSetObject(ido, ids);
    //cv.window[idw].cglAddLight(&cv.light[1]);

    //ido = cv.cglObject(new CglCube());
    //cv.cglSetObject(ido, ids);
  }

  else
  {
    //Première fenêtre
    idw = cv.cglWindow(0,0,800,800);
    ids = cv.cglScene();
    cv.cglSetScene(ids, idw);
    cv.window[idw].show();

    InitGlew();

    cout << idw << " " << ids << endl;
    vector<CglMesh*> mesh;
    for (int i=0; i < argc - 1; i++){
      cout << "i: " << i << endl;
      mesh.push_back(new CglMesh(argv[i+1]));
      mesh[i]->meshInfo(0);
      ido = cv.cglObject(mesh[i]);
      cv.cglSetObject(ido, ids);
    }

    /*
    //Seconde fenetre
    idw = cv.cglWindow(1000, 0, 1000, 1000);
    cv.cglSetScene(ids, idw);
    cv.window[idw].show();
    for (int i=0; i < mesh.size(); i++){
      cout << "i: " << i << endl;
      ido = i;
      cv.cglSetObject(ido, ids);
    }
    */
  }

  cv.cglMainLoop();

  return(EXIT_SUCCESS);
}
