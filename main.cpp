#include <iostream>
#include <cgl.h>
#include <cstdlib>
#include <fstream>

#include <sstream>
#include <iterator>

using namespace std;

//Used for parsing save file
template<typename T> std::vector<T> split(const std::string& line) {
    std::istringstream is(line);
    return std::vector<T>(std::istream_iterator<T>(is), std::istream_iterator<T>());
}

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

  //Loading mechanisms
  //~/dev/cgl/data/silene/*.mesh
  //cgl.save
  else if((argc==2) && (string(argv[1])=="cgl.save")){
    vector<string>    names;
    vector<glm::mat4> mats;
    vector<glm::vec3> centers;
    vector<int>       groups;
    string line;
    int lineNumber = -1;
    int numLines = 7;
    int numberMeshes = 0;

    ifstream saveFile("cgl.save");

    if (saveFile.is_open()){
      while ( getline (saveFile,line) ){
        cout << lineNumber << endl;

        //Enregistrement du nombre de maillages
        if (lineNumber==-1)
          numberMeshes = atoi(line.c_str());

        //Enregistrement des noms de fichiers
        if (lineNumber%numLines == 0)
          names.push_back(line);

        //Enregistrement des matrices
        if (lineNumber%numLines == 1){
          cout << "create mat:  " << mats.size() << endl;
          cout << line << endl;
          mats.push_back(glm::mat4(1.0f));
          cout << "matCreation: " << mats.size() << endl;
        }
        for(int i = 1 ; i < 5 ; i++){
          if(lineNumber%numLines == i){
            //cout << i << endl;
            vector<float> values = split<float>(line);
            for(int j = 0 ; j < 4 ; j++){
              mats[mats.size() - 1][i][j] = values[j];
            }
          }
        }

        //Enregistrement des centres
        if(lineNumber%numLines == 5){
          cout << "ooo" << endl;
          centers.push_back(glm::vec3(0.0f));
          cout << "eee" << endl;
          cout << line << endl;
          vector<double> values = split<double>(line);
          cout << "aaa" << endl;
          for(int i = 0 ; i < 3 ; i++){
            centers[centers.size()-1][i] = values[i];
          }
          cout << "iii" << endl;
        }

        //Enregistrement des idGroups
        if((lineNumber%numLines == 6) && (lineNumber!=-1))
          groups.push_back(atoi(line.c_str()));


        lineNumber++;
      }
      saveFile.close();
    }
    else
      cout << "Unable to open file";

    cout << numberMeshes << endl;
    for(int i = 0 ; i < names.size() ; i++)
        cout << names[i] << endl;

    for(int i = 0 ; i < centers.size() ; i++)
      cout << centers[i].x << " " << centers[i].y << " " << centers[i].z << endl;

    for(int i = 0 ; i < mats.size() ; i++){
      for(int j = 0 ; j < 4 ; j ++){
        cout << mats[i][j][0] << " " << mats[i][j][1] << " " << mats[i][j][2] << " " << mats[i][j][3] << endl;
      }
      cout << endl;
    }
    exit(143);
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
