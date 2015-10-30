#include <cgl/mesh.h>
extern "C" {
#include <libmesh5.h>
}

#include <cgl/canvas.h>
extern CglCanvas *pcv;



CglMesh::CglMesh(char *name)
{
  isMesh   = true;
  Point    *ppt;
  Tria     *pt;
  double   *n,dd;
  float     fp1,fp2,fp3;
  int       k,inm;
  meshFile  = std::string(name);
  cout << "Reading: " << name << " " << endl;

  //Lecture du .sol
  std::string solFile   = meshFile.substr(0, meshFile.size()-5) + ".sol";
  cout << solFile << endl << endl;
  int ver2, dim2;
  int inMeshSol = GmfOpenMesh((char*)(solFile.c_str()), GmfRead, &ver2, &dim2);
  if(!inMeshSol){
        exit(143);
  }

  cout << "sol file opened" << endl;
  int nSol      = GmfStatKwd(inMeshSol, GmfISolAtVertices);
  cout << "size of solution = " << nSol << endl;



  inm = GmfOpenMesh(name,GmfRead,&ver,&dim);
  if ( !inm ){
    //cout << "  ** FILE NOT FOUND.\n";
    exit(0);
  }

  np    = GmfStatKwd(inm, GmfVertices);
  nt    = GmfStatKwd(inm, GmfTriangles);
  nn    = GmfStatKwd(inm, GmfNormals);
  //Normals At vertices
  nNAtV = GmfStatKwd(inm, GmfNormalAtVertices);

  if ( !np ){
    cout << "  ** MISSING DATA\n";
    exit(0);
  }

  point.resize(np);
  GmfGotoKwd(inm,GmfVertices);
  for (k=0; k<np; k++){
    ppt = &point[k];
    if ( ver == GmfFloat ){
      GmfGetLin(inm,GmfVertices,&fp1,&fp2,&fp3,&ppt->ref);
      ppt->c[0] = fp1;
      ppt->c[1] = fp2;
      ppt->c[2] = fp3;
    }
    else
      GmfGetLin(inm,GmfVertices,&ppt->c[0],&ppt->c[1],&ppt->c[2],&ppt->ref);
  }

  //read triangles
  tria.resize(nt);
  GmfGotoKwd(inm,GmfTriangles);
  for (k=0; k<nt; k++) {
    pt = &tria[k];
    GmfGetLin(inm,GmfTriangles,&pt->v[0],&pt->v[1],&pt->v[2],&pt->ref);
  }

  normal.resize(np);
  if ( nn ) {
    GmfGotoKwd(inm,GmfNormals);
    for (k=0; k<nn; k++) {
      if ( ver == GmfFloat ) {
        GmfGetLin(inm,GmfNormals,&fp1,&fp2,&fp3);
        normal[k].n[0] = fp1;
        normal[k].n[1] = fp2;
        normal[k].n[2] = fp3;
      }
      else
        GmfGetLin(inm,GmfNormals,&normal[k].n[0],&normal[k].n[1],&normal[k].n[2]);
      n  = normal[k].n;
      dd = 1.0 / sqrt(n[0]*n[0] + n[1]*n[1] + n[2]*n[2]);
      n[0] *= dd;
      n[1] *= dd;
      n[2] *= dd;
    }
  }

  //Calcul de la bounding box
  getBBOX();

  //Préparation des buffers
  std::vector<float> vertices;
  std::vector<int>   indices;
  int inv = ((pcv->profile.invertVertical)?-1:1);

  //Buffer des vertices
  for (int i = 0 ; i < point.size() ; i++){
    vertices.push_back(      point[i].c[0]);
    vertices.push_back(inv * point[i].c[1]);
    vertices.push_back(inv * point[i].c[2]);
  }

  //Buffer des indices
  for (int i = 0 ; i < tria.size() ; i++)
    for(int j = 0 ; j < 3 ; j++)
      indices.push_back(tria[i].v[j]-1);




  //Lecture des NormalsAtVertices pour faire les normales
  NormalAtVertices.resize(nNAtV + 1);
  //Lecture du .mesh
  if ( nNAtV ) {
    GmfGotoKwd(inm,GmfNormalAtVertices);
    for (k=0; k<nNAtV; k++)
      GmfGetLin(inm,GmfNormalAtVertices,
                &NormalAtVertices[k].inds[0],
                &NormalAtVertices[k].inds[1]);
  }
  //Initialisation des normals à 0 pour les vertices n'ayant pas de normales
  std::vector<float> normals;
  for(int i = 0 ; i < vertices.size() ; i++)
    normals.push_back(0.0f);
  //Remplissage du vector 1D "normals", envoyé plus tard aux buffers
  for(int i = 0 ; i < NormalAtVertices.size() - 1 ; i++){
    int indV = NormalAtVertices[i].inds[0] - 1;
    int indN = NormalAtVertices[i].inds[1] - 1;
    normals[3 * indV + 0] =       normal[indN].n[0];
    normals[3 * indV + 1] = inv * normal[indN].n[1];
    normals[3 * indV + 2] = inv * normal[indN].n[2];
  }

    createBuffer(&meshBuffer,       &vertices);
    createBuffer(&indicesBuffer,    &indices);
    createBuffer(&normalBuffer,     &normals);

  //TYPE DE RENDU ET SHADER
  nPicking = 3 * tria.size();
}

void CglMesh::meshInfo(const int& verbose, ostream& outstr)
{
  //cout << " \t\t MeshInfo \n" << endl;
  cout << "np: " << np << ", nt: " << nt << ", nn:" << nn << ", dim: " << dim << ", ver: " << ver << endl;

  if (verbose){
    cout << "Points" << endl;
    for (int i = 0; i < np; i++)
      cout << point[i].c[0] << ", " << point[i].c[1] << ", " << point[i].c[2] << ", " << point[i].ref << endl;
    cout << "Triangles" << endl;
    for (int i = 0; i < nt; i++)
      cout << tria[i].v[0] << ", " << tria[i].v[1] << ", " << tria[i].v[2] << ", " << tria[i].ref << endl;
    cout << "Normals" << endl;
    for (int i = 0; i < nn; i++)
      cout << normal[i].n[0] << ", " << normal[i].n[1] << ", " << normal[i].n[2] << endl;
  }
}

void CglMesh::getBBOX()
{
  //Init
  Point     *p0;
  bbmin = glm::vec3(FLOAT_MAX);
  bbmax = glm::vec3(-FLOAT_MAX);

  //Compute bounding box
  for (int k=0; k<np; k++) {
    p0 = &point[k];
    if ( p0->c[0] < bbmin.x ) bbmin.x = p0->c[0];
    if ( p0->c[0] > bbmax.x ) bbmax.x = p0->c[0];
    if ( p0->c[1] < bbmin.y ) bbmin.y = p0->c[1];
    if ( p0->c[1] > bbmax.y ) bbmax.y = p0->c[1];
    if ( p0->c[2] < bbmin.z ) bbmin.z = p0->c[2];
    if ( p0->c[2] > bbmax.z ) bbmax.z = p0->c[2];
  }

  //Translate mesh to center
  tra = 0.5f * (bbmin + bbmax);
  for (int k=0; k<np; k++) {
    p0 = &point[k];
    p0->c[0] -= tra.x;
    p0->c[1] -= tra.y;
    p0->c[2] -= tra.z;
  }

  glm::vec3 size = bbmax - bbmin;
  localScale     = 0.5f * 1.0f / (max( max(size.x, size.y) , size.z ));
  //Independant scale -> Chaque objet est ramené de sorte que sa plus grande dimension egale 1.
  if(pcv->profile.independantScale){
    scaleFactor = localScale;
  }
  //ELSE: C'EST LA SCENE QUI SE CHARGE DENVOYER LE PATE

  //Bounding box buffer
  std::vector<float> cube{
    -0.5, -0.5, -0.5,
     0.5, -0.5, -0.5,
     0.5,  0.5, -0.5,
    -0.5,  0.5, -0.5,
    -0.5, -0.5,  0.5,
     0.5, -0.5,  0.5,
     0.5,  0.5,  0.5,
    -0.5,  0.5,  0.5
  };

  //Indices buffer
  std::vector<short> elements{
    0, 1, 2, 3,
    4, 5, 6, 7,
    0, 4, 1, 5, 2, 6, 3, 7
  };

  createBuffer(&bboxBuffer, &cube);
  createBuffer(&bboxIndBuffer, &elements);
  freeBuffer();
}


//Light[3] = 0 si directionelle
glm::mat4 shadowMatrix(glm::vec4 ground, glm::vec4 light){
    float  dot;
    glm::mat4 shadowMat;
    dot = ground[0] * light[0] +
          ground[1] * light[1] +
          ground[2] * light[2] +
          ground[3] * light[3];
    shadowMat[0][0] = dot - light[0] * ground[0];
    shadowMat[1][0] = 0.0 - light[0] * ground[1];
    shadowMat[2][0] = 0.0 - light[0] * ground[2];
    shadowMat[3][0] = 0.0 - light[0] * ground[3];
    shadowMat[0][1] = 0.0 - light[1] * ground[0];
    shadowMat[1][1] = dot - light[1] * ground[1];
    shadowMat[2][1] = 0.0 - light[1] * ground[2];
    shadowMat[3][1] = 0.0 - light[1] * ground[3];
    shadowMat[0][2] = 0.0 - light[2] * ground[0];
    shadowMat[1][2] = 0.0 - light[2] * ground[1];
    shadowMat[2][2] = dot - light[2] * ground[2];
    shadowMat[3][2] = 0.0 - light[2] * ground[3];
    shadowMat[0][3] = 0.0 - light[3] * ground[0];
    shadowMat[1][3] = 0.0 - light[3] * ground[1];
    shadowMat[2][3] = 0.0 - light[3] * ground[2];
    shadowMat[3][3] = dot - light[3] * ground[3];
    return shadowMat;
}


void CglMesh::shadowsDisplay(){
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    if(pcv->profile.displayShadows && !hidden){
        int shaderID        = initProgram(pcv->simpleID());
        int MatrixID        = glGetUniformLocation(shaderID, "MVP");
        int colorID         = glGetUniformLocation(shaderID, "COL");

        glm::mat4 shadowMVP =  sPROJ() * sVIEW() * sMODEL() *
                               shadowMatrix( glm::vec4(glm::vec3(0,1,0), pcv->profile.bottomDistance + sMODEL()[3].y - 0.002), glm::vec4(glm::vec3(0,1,0), 0) ) *
                               glm::scale(MODEL, glm::vec3(scaleFactor));

        uniform(MatrixID,   shadowMVP);
        uniform(colorID,    glm::vec3(0.2));

        draw(shaderID, 3*tria.size(), meshBuffer, -1, indicesBuffer);
    }

    //Réflection
    if(pcv->profile.displayReflection){
        int shaderID    = initProgram( ((pcv->profile.smooth)?pcv->smoothID():pcv->flatID()) );
        int MatrixID    = glGetUniformLocation(shaderID, "MVP");
        int colorID     = glGetUniformLocation(shaderID, "COL");
        int MID         = glGetUniformLocation(shaderID, "M");

        //Calcul de la couleur
        glm::vec3 selection_color   = ((idGroup==-1)?pcv->profile.sele_color:pcv->getScene()->getGroup(idGroup)->getColor());
        glm::vec3 color             = ((isSelected())?selection_color:face_color);
        glm::vec2 mix               = ((hidden)?glm::vec2(0,1):pcv->profile.reflection_mix);

        //Calcul de MVP
        glm::mat4 ID(1);
        glm::vec3 cc            =  glm::vec3(sMODEL()[3]);
        glm::mat4 refMODEL      =  glm::translate(ID, glm::vec3(-cc.x, - pcv->profile.bottomDistance - cc.y, -cc.z)) *
                                   glm::scale(ID, glm::vec3(1,-1,1)) *
                                   glm::translate(ID, glm::vec3(cc.x, + pcv->profile.bottomDistance + cc.y, cc.z)) *
                                   glm::scale(MODEL, glm::vec3(scaleFactor));
        glm::mat4 reflection    =  sPROJ() * sVIEW() * sMODEL() * refMODEL;
        glm::mat4 M             = glm::translate(refMODEL, glm::vec3(sMODEL()[3]));

        //Envoi des uniformes
        uniform(colorID, (glm::vec3(mix.x) + mix.y * color));
        uniform( MatrixID, reflection);
        uniform(MID, M);

        draw(shaderID, 3*tria.size(), meshBuffer, normalBuffer, indicesBuffer);
    }

    freeBuffer();
    glDisable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

void CglMesh::artifactsDisplay(){
  if(!hidden){
    int shaderID                = initProgram(pcv->simpleID());
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    int MatrixID                = glGetUniformLocation(shaderID, "MVP");
    int colorID                 = glGetUniformLocation(shaderID, "COL");

    glm::mat4 MVP               = sPROJ() * sVIEW() * sMODEL() * glm::scale(MODEL, glm::vec3(scaleFactor));
    pCglScene scene             = pcv->getScene();
    glm::vec3 selection_color   = ((idGroup==-1)?pcv->profile.sele_color:scene->getGroup(idGroup)->getColor());
    computeGroupID();

    //Contour
    if(isSelected()){
        glLineWidth(10.0);
        glDisable(GL_DEPTH_TEST);

        uniform(colorID,  selection_color);
        uniform(MatrixID, MVP);

        draw(shaderID, 3*tria.size(), meshBuffer, -1, indicesBuffer);

        glEnable(GL_DEPTH_TEST);
        glLineWidth(1.0);
    }

    //Box
    if(box){
        glEnable(GL_POLYGON_OFFSET_LINE);
        glPolygonOffset(10,0);

        glm::mat4 SCALE = glm::scale(MVP, 1.02f * (bbmax - bbmin));
        glm::vec3 color = ((isSelected())?selection_color:pcv->profile.idle_color);

        uniform( MatrixID, SCALE);
        uniform(colorID, color);

        glLineWidth(((isSelected())?2.0:1.0));

        bindBuffer(0, GL_ARRAY_BUFFER, bboxBuffer);
        bindBuffer(-1, GL_ELEMENT_ARRAY_BUFFER, bboxIndBuffer);
        glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, 0);
        glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, (GLvoid*)(4*sizeof(GLushort)));
        glDrawElements(GL_LINES, 8, GL_UNSIGNED_SHORT, (GLvoid*)(8*sizeof(GLushort)));

        glDisable(GL_POLYGON_OFFSET_LINE);
        glLineWidth(1.0);
    }

    //Axes avec contraintes
    if((isConstrainedInRotation()) || (isConstrainedInTranslation())){
      std::vector<glm::vec3> pts;
      if(isConstrainedInRotation()){
        pts.push_back(-10.0f * constrainedRotationAxis + *rotationCenter);
        pts.push_back( 10.0f * constrainedRotationAxis + *rotationCenter);
      }
      else if(isConstrainedInTranslation()){
        pts.push_back(-10.0f * constrainedTranslationAxis + *rotationCenter);
        pts.push_back( 10.0f * constrainedTranslationAxis + *rotationCenter);
      }
      std::vector<float> line;
      for(int i = 0 ; i < pts.size(); i++)
            for(int j = 0 ; j < 3 ; j++)
                line.push_back(pts[i][j]);

        GLuint axeBuffer;
        createBuffer(&axeBuffer, &line);


      MVP = sPROJ() * sVIEW() * sMODEL();

      uniform( MatrixID, MVP);
      if(isConstrainedInRotation())
        uniform(colorID, constrainedRotationAxis);
      else if(isConstrainedInTranslation())
        uniform(colorID, constrainedTranslationAxis);

      glLineWidth(2.0f);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, ( void*)0);
      glBindAttribLocation(shaderID, 0, "vertex_position");
      glDrawArrays(GL_LINES, 0, 2);
      glLineWidth(1.0);
    }

    freeBuffer();
    //glDisable(GL_CULL_FACE);
  }
}

void CglMesh::display(){

    if(!hidden){
        glEnable(GL_CULL_FACE);
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(1.0,1.0);

        int shaderID                = initProgram(((pcv->profile.smooth)? pcv->smoothID() : pcv->flatID()));

        int MatrixID                = glGetUniformLocation(shaderID, "MVP");
        int colorID                 = glGetUniformLocation(shaderID, "COL");
        GLuint MID                  = glGetUniformLocation(shaderID, "M");
        GLuint VID                  = glGetUniformLocation(shaderID, "V");
        int fill_light_ID           = glGetUniformLocation(shaderID, "FILL");
        int side_light_ID           = glGetUniformLocation(shaderID, "SIDE");
        int back_light_ID           = glGetUniformLocation(shaderID, "BACK");

        glm::mat4 MVP               = sPROJ() * sVIEW() * sMODEL() * glm::scale(MODEL, glm::vec3(scaleFactor));
        glm::mat4 M                 = glm::translate(MODEL, glm::vec3(sMODEL()[3]));
        glm::mat4 V                 = sVIEW();
        glm::vec3 selection_color   = ((idGroup==-1)?pcv->profile.sele_color:pcv->getScene()->getGroup(idGroup)->getColor());
        glm::vec3 color             = ((isSelected())?1.0f * selection_color:face_color);
        std::vector<pCglLight> lights = pcv->getSubWindow()->getScene()->getLights();

        uniform( MatrixID, MVP);
        uniform( MID, M);
        uniform( VID, V);
        uniform( colorID, color);
        uniform( fill_light_ID, *(lights[0]->getLightMatrix(material)));
        uniform( side_light_ID, *(lights[1]->getLightMatrix(material)));
        uniform( back_light_ID, *(lights[2]->getLightMatrix(material)));
        enableFog(shaderID);

        glPolygonMode(GL_FRONT, GL_FILL);
        draw(shaderID, 3*tria.size(), meshBuffer, normalBuffer, indicesBuffer);
        glDisable(GL_POLYGON_OFFSET_FILL);

        //Wireframe
        if(line){

            shaderID = initProgram(pcv->smoothID());
            MatrixID = glGetUniformLocation(shaderID, "MVP");
            colorID  = glGetUniformLocation(shaderID, "COL");

            color = ((isSelected())?0.6f * selection_color:0.6f * face_color);

            uniform( MatrixID, MVP);
            uniform( colorID, color);

            glDisable(GL_POLYGON_OFFSET_FILL);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            draw(shaderID, 3*tria.size(), meshBuffer, -1, indicesBuffer);
      }

      //Closing and freeing ressources

      freeBuffer();
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      glDisable(GL_CULL_FACE);
    }
}



