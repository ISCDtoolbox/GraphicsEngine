#include <glic/mesh.h>
extern "C" {
#include <libmesh5.h>
}

#include <glic/canvas.h>

extern CglicCanvas *pcv;

CglicMesh::CglicMesh(char *name)
{
  isMesh   = true;
  Point    *ppt;
  Tria     *pt;
  double   *n,dd;
  float     fp1,fp2,fp3;
  int       k,inm;

  //cout << "\n NAME:" << name << endl;

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
      ppt->c[2] = 0.;
    }
    else
      GmfGetLin(inm,GmfVertices,&ppt->c[0],&ppt->c[1],&ppt->c[2],&ppt->ref);
  }

  //read triangles
  tria.resize(nt+1);
  GmfGotoKwd(inm,GmfTriangles);
  for (k=0; k<nt; k++) {
    pt = &tria[k];
    GmfGetLin(inm,GmfTriangles,&pt->v[0],&pt->v[1],&pt->v[2],&pt->ref);
  }

  normal.resize(np+1);
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
  glGenBuffers( 1,               &meshBuffer);
  glBindBuffer( GL_ARRAY_BUFFER, meshBuffer);
  glBufferData( GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

  //Buffer des indices
  for (int i = 0 ; i < tria.size() ; i++)
    for(int j = 0 ; j < 3 ; j++)
      indices.push_back(tria[i].v[j]-1);
  glGenBuffers(1, &indicesBuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBuffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);


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
  //Buffer des normales
  glGenBuffers( 1,               &normalBuffer);
  glBindBuffer( GL_ARRAY_BUFFER, normalBuffer);
  glBufferData( GL_ARRAY_BUFFER, sizeof(float) * normals.size(), &normals[0], GL_STATIC_DRAW);


  //face_color = glm::vec3(0, 0, 1);
  //edge_color = glm::vec3(0,0,1);

  //TYPE DE RENDU ET SHADER
  nPicking = 3 * tria.size();
}

void CglicMesh::meshInfo(const int& verbose, ostream& outstr)
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

void CglicMesh::getBBOX()
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
  if(pcv->profile.independantScale){
    scaleFactor = localScale;
  }
  //ELSE: C'EST LA SCENE QUI SE CHARGE DENVOYER LE PATE

  //Bounding box buffer
  float cube[] = {
    -0.5, -0.5, -0.5,
     0.5, -0.5, -0.5,
     0.5,  0.5, -0.5,
    -0.5,  0.5, -0.5,
    -0.5, -0.5,  0.5,
     0.5, -0.5,  0.5,
     0.5,  0.5,  0.5,
    -0.5,  0.5,  0.5,
  };
  glGenBuffers(1, &bboxBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, bboxBuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

  //Indices buffer
  GLushort elements[] = {
    0, 1, 2, 3,
    4, 5, 6, 7,
    0, 4, 1, 5, 2, 6, 3, 7
  };
  glGenBuffers(1, &bboxIndBuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bboxIndBuffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  //Freeing ressources
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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

void CglicMesh::shadowsDisplay(){
  if(pcv->profile.displayShadows){
    if(hidden)
      glEnable(GL_BLEND);

    glEnable(GL_CULL_FACE);
    int shaderID = pcv->simpleShader.mProgramID;
    glUseProgram(shaderID);
    int MatrixID = glGetUniformLocation(shaderID, "MVP");
    int colorID  = glGetUniformLocation(shaderID, "COL");

    glEnableVertexAttribArray( 0);
    glBindBuffer(GL_ARRAY_BUFFER, meshBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, ( void*)0);
    glBindAttribLocation(shaderID, 0, "vertex_position");
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBuffer);

    glm::mat4 shadowMVP =  *pPROJ * *pVIEW * *pMODEL *
                           shadowMatrix( glm::vec4(glm::vec3(0,1,0), pcv->profile.bottomDistance - 0.002), glm::vec4(glm::vec3(0,1,0), 0) ) *
                           //shadowMatrix( glm::vec4(*sceneUp, 0.495), glm::vec4(*sceneUp, 0) ) *
                           glm::scale(MODEL, glm::vec3(scaleFactor));

    glUniformMatrix4fv( MatrixID, 1, GL_FALSE, &shadowMVP[0][0]);
    uniformVec3(colorID, 0.08f * face_color);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);




    glDrawElements(GL_TRIANGLES, 3 * tria.size(), GL_UNSIGNED_INT, (void*)0);

    glDisable(GL_BLEND);
    glDisable(GL_CULL_FACE);
  }
}

void CglicMesh::artifactsDisplay(){
  if(!hidden){
    //Initialization
    glm::mat4 MVP = *pPROJ * *pVIEW * *pMODEL * glm::scale(MODEL, glm::vec3(scaleFactor));;
    int shaderID = pcv->simpleShader.mProgramID;
    glUseProgram(shaderID);
    int MatrixID = glGetUniformLocation(shaderID, "MVP");
    int colorID  = glGetUniformLocation(shaderID, "COL");
    glUniformMatrix4fv( MatrixID, 1, GL_FALSE, &MVP[0][0]);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //Mesh buffer binding
    glEnableVertexAttribArray( 0);
    glBindBuffer(GL_ARRAY_BUFFER, meshBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, ( void*)0);
    glBindAttribLocation(shaderID, 0, "vertex_position");
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBuffer);

    //Group identification
    pCglicScene scene = pcv->scene[pcv->window[pcv->winid()].ids];
    if (scene->listGroup.size() > 0){
      for(int i = 0 ; i < scene->listGroup.size() ; i++){
        for(int j = 0 ; j < scene->listGroup[i]->listObject.size() ; j++){
          if(objectID==scene->listGroup[i]->listObject[j]->getID()){
            idGroup = i;
          }
        }
      }
    }
    else
      idGroup = -1;

    glm::vec3 selection_color = ((idGroup==-1)?pcv->profile.sele_color:scene->listGroup[idGroup]->group_color);

    //Contour
    if(isSelected()){
      glLineWidth(10.0);

      uniformVec3(colorID, selection_color);
      glUniformMatrix4fv( MatrixID, 1, GL_FALSE, &MVP[0][0]);

      glDisable(GL_DEPTH_TEST);
      glDrawElements(GL_TRIANGLES, 3 * tria.size(), GL_UNSIGNED_INT, (void*)0);
      glEnable(GL_DEPTH_TEST);

      glLineWidth(1.0);
    }

    //Box
    if(box){
      glBindBuffer(GL_ARRAY_BUFFER, bboxBuffer);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, ( void*)0);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bboxIndBuffer);

      if(isSelected()){
        glLineWidth(2.0);
        uniformVec3(colorID, selection_color);}
      else{
        glLineWidth(1.0);
        uniformVec3(colorID, pcv->profile.idle_color);}

      //glm::mat4 SCALE;
      //if(idGroup==-1)
        glm::mat4 SCALE = glm::scale(MVP, 1.02f * (bbmax - bbmin));
      //else{
        //SCALE = //glm::translate(glm::mat4(1),  scene->listGroup[idGroup]->group_center) *
                //glm::scale(MVP, scene->listGroup[idGroup]->bbmax - scene->listGroup[idGroup]->bbmin);// *
                //glm::translate(glm::mat4(1),  -scene->listGroup[idGroup]->group_center);
      //}
      glUniformMatrix4fv( MatrixID, 1, GL_FALSE, &SCALE[0][0]);

      glEnable(GL_POLYGON_OFFSET_LINE);
      glPolygonOffset(10,0);
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
      glUseProgram(pcv->simpleShader.mProgramID);
      GLuint axeBuffer;
      glGenBuffers(1, &axeBuffer);
      glBindBuffer(GL_ARRAY_BUFFER, axeBuffer);
      glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), &pts[0][0], GL_STATIC_DRAW);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, ( void*)0);
      glBindAttribLocation(pcv->simpleShader.mProgramID, 0, "vertex_position");
      MVP = *pPROJ * *pVIEW * *pMODEL;
      glUniformMatrix4fv( MatrixID, 1, GL_FALSE, &MVP[0][0]);
      if(isConstrainedInRotation())
        uniformVec3(colorID, constrainedRotationAxis);
      else if(isConstrainedInTranslation())
        uniformVec3(colorID, constrainedTranslationAxis);
      glLineWidth(2.0f);
      glDrawArrays(GL_LINES, 0, 2);
      glLineWidth(1.0);
    }

    //Closing and freeing ressources
    glDisable(GL_POLYGON_OFFSET_FILL);
    glDisableVertexAttribArray( 0);
    glDisableVertexAttribArray( 1);
    glUseProgram(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDisable(GL_CULL_FACE);
  }
}

void CglicMesh::display()
{
  if(hidden)
    glEnable(GL_BLEND);

  glEnable(GL_CULL_FACE);

  //Initialization
  glm::mat4 MVP = *pPROJ * *pVIEW * *pMODEL * glm::scale(MODEL, glm::vec3(scaleFactor));;
  int shaderID = ((smooth)?pcv->smoothShader.mProgramID : pcv->simpleShader.mProgramID);
  glUseProgram(shaderID);
  int MatrixID = glGetUniformLocation(shaderID, "MVP");
  int colorID  = glGetUniformLocation(shaderID, "COL");
  int lightID  = glGetUniformLocation(shaderID, "LIGHTPOS");
  glUniformMatrix4fv( MatrixID, 1, GL_FALSE, &MVP[0][0]);
  uniformVec3(lightID, *sceneCam);

  //Mesh buffer binding

  glEnableVertexAttribArray( 0);
  glBindBuffer(              GL_ARRAY_BUFFER, meshBuffer);
  glVertexAttribPointer(     0, 3, GL_FLOAT, GL_FALSE, 0, ( void*)0);
  glBindAttribLocation(      shaderID, 0, "vertex_position");

  //Normal buffer binding
  glEnableVertexAttribArray( 1);
  glBindBuffer(              GL_ARRAY_BUFFER, normalBuffer);
  glVertexAttribPointer(     1, 3, GL_FLOAT, GL_FALSE, 0, ( void*)0);
  glBindAttribLocation(      shaderID, 1, "vertex_normal");

  //Indices buffer binding
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBuffer);

  pCglicScene scene = pcv->scene[pcv->window[pcv->winid()].ids];
  glm::vec3 selection_color = ((idGroup==-1)?pcv->profile.sele_color:scene->listGroup[idGroup]->group_color);

  if(smooth){
    GLuint MID      = glGetUniformLocation(shaderID, "M");
    GLuint VID      = glGetUniformLocation(shaderID, "V");
    glUniformMatrix4fv( MID, 1, GL_FALSE, &MODEL[0][0]);
    glUniformMatrix4fv( VID, 1, GL_FALSE, &(*pVIEW)[0][0]);


    if(isSelected())
      uniformVec3(colorID, 1.0f * selection_color);
    else
      uniformVec3(colorID, face_color);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1.0,1.0);

    glDrawElements(GL_TRIANGLES, 3 * tria.size(), GL_UNSIGNED_INT, (void*)0);
  }
  else{
    uniformVec3(colorID, face_color);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_TRIANGLES, 3 * tria.size(), GL_UNSIGNED_INT, (void*)0);
  }

  if(line){
    glDisable(GL_POLYGON_OFFSET_FILL);
    if(isSelected())
      uniformVec3(colorID, 0.8f * selection_color);
    else
      uniformVec3(colorID, edge_color);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_TRIANGLES, 3 * tria.size(), GL_UNSIGNED_INT, (void*)0);
  }

  //Closing and freeing ressources
  glDisable(GL_POLYGON_OFFSET_FILL);
  glDisableVertexAttribArray( 0);
  glDisableVertexAttribArray( 1);
  glUseProgram(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDisable(GL_BLEND);
  glDisable(GL_CULL_FACE);
}


