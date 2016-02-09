#include <cgl/object.h>
#include <cgl/canvas.h>
extern CglCanvas *pcv;
#include <cgl/scene.h>

#include <algorithm>

extern "C" {
#include <libmesh5.h>
}


//BUFFER OPERATIONS
void    createBuffer(GLuint *pBuffer, std::vector<float> *data){
    glGenBuffers( 1,               pBuffer);
    glBindBuffer( GL_ARRAY_BUFFER, *pBuffer);
    glBufferData( GL_ARRAY_BUFFER, sizeof(float) * data->size(), &(*data)[0], GL_STATIC_DRAW);
}
void    createBuffer(GLuint *pBuffer, std::vector<int> *data){
    glGenBuffers( 1,               pBuffer);
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, *pBuffer);
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * data->size(), &(*data)[0], GL_STATIC_DRAW);
}
void    createBuffer(GLuint *pBuffer, std::vector<short> *data){
    glGenBuffers( 1,               pBuffer);
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, *pBuffer);
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * data->size(), &(*data)[0], GL_STATIC_DRAW);
}
void    bindBuffer(int attrib, int bufferType, GLuint buffer){
    if(attrib!=-1)
        glEnableVertexAttribArray( attrib);
    glBindBuffer( bufferType, buffer);
    if(attrib!=-1)
        glVertexAttribPointer( attrib, 3, GL_FLOAT, GL_FALSE, 0, ( void*)0);
}
void    freeBuffer(){
    for(int i = 0 ; i < 5 ; i++)
        glDisableVertexAttribArray(i);
    glUseProgram(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
int     initProgram(int ID){
    glUseProgram(ID);
    for(int i = 0 ; i < 5 ; i++)
        glEnableVertexAttribArray(5);
    return ID;
}
void    draw(   int ID,         //Shader ID
                int s,          //Number of vertices
                int mBuffer,    //Vertexs BUffer
                int nBuffer,    //Normals Buffer
                int iBuffer,    //Indices Buffer
                int cBuffer     //Colors Buffer
            ){
    //std:: cout << ID << " " << mBuffer << " " << nBuffer << " " << iBuffer << " " << cBuffer << std::endl;
    //freeBuffer();
    //Vertex Buffer
    bindBuffer(0, GL_ARRAY_BUFFER, mBuffer);
    glBindAttribLocation(ID, 0, "vertex_position");
    //Indices Buffer
    bindBuffer(-1, GL_ELEMENT_ARRAY_BUFFER, iBuffer);
    //Normal and Color Buffer
    if(nBuffer!=-1){
        bindBuffer(1, GL_ARRAY_BUFFER, nBuffer);
        glBindAttribLocation(ID, 1, "vertex_normal");
    }
    if(cBuffer != -1){
        //cout << "color = " << cBuffer << endl;
        bindBuffer(2, GL_ARRAY_BUFFER, cBuffer);
        glBindAttribLocation(ID, 2, "vertex_color");
    }

    //Drawing
    glDrawElements(GL_TRIANGLES, s, GL_UNSIGNED_INT, (void*)0);
    freeBuffer();

}
void    uniform(int ID, glm::vec3 v){
  glUniform3f(ID, v.x, v.y, v.z);
}
void    uniform(int ID, glm::mat4 &m){
  glUniformMatrix4fv( ID, 1, GL_FALSE, &m[0][0]);

}
void    uniform(int ID, float f){
  glUniform1f( ID, f);

}



//GEOMETRY OPERATIONS
struct sphereGeom{
     struct vertex { float pos[3];};// float uv[2]; };
    std::vector<vertex> vertices;
    std::vector<int> indices;

    sphereGeom( unsigned int subdiv = 4 ) {
        unsigned int perFaceVertexCount = subdiv * subdiv;
        float invSubDiv = 1.f / float(subdiv-1);

        // allocate the vertex buffer memory
        vertices.resize( subdiv * subdiv * 6);
        // fill the buffer
        for ( unsigned int face = 0; face != 3; ++face ) {
            // helper to redirect the vertex coordinate in the axis
            const struct { unsigned int xIdx, yIdx, zIdx; } faceIndex[] { {0,1,2}, {2,0,1}, {1,2,0} };
            auto const & indirect = faceIndex[face];
            for ( unsigned int v = 0; v < subdiv; ++v ) {
                float texV = float(v) * invSubDiv; // the v texcoord [0..1]
                for ( unsigned int u = 0; u < subdiv; ++u) {
                    float texU = float(u) * invSubDiv; // the u texcoord [0..1]
                    float posU = texU * 2.f - 1.f; // the position first value [-1..1]
                    float posV = texV * 2.f - 1.f; // the position second value [-1..1]
                    float invLen = 1.f / sqrtf( posU*posU + posV*posV + 1.f ); // pythagore
                    vertex result;
                    // set the vertex position
                    result.pos[ indirect.xIdx ] = posU * invLen;
                    result.pos[ indirect.yIdx ] = posV * invLen;
                    result.pos[ indirect.zIdx ] = invLen;
                    // set the vertex texcoord
                    //result.uv[0] = texU;
                    //result.uv[1] = texV;
                    // copy result to the right offset
                    vertices[ perFaceVertexCount * ( 2 * face + 0 ) + u + subdiv * v] = result;
                    // mirror x and z location for the other face
                    result.pos[ indirect.xIdx ] *= -1.f;
                    result.pos[ indirect.zIdx ] *= -1.f;
                    vertices[ perFaceVertexCount * ( 2 * face + 1 ) + u + subdiv * v] = result;
                }
            }
        }
        unsigned int perFaceQuadCount = (subdiv-1)*(subdiv-1); // number of quad per faces
        // start by filling an index buffer for one face, then we will duplicate it for each other face
        std::vector<unsigned int> faceIndices;
        faceIndices.resize( perFaceQuadCount * 2 * 3 ); // one quad is two triangle of three indices.
        for (unsigned int v = 0; v != subdiv - 1; ++v ) {
            for (unsigned int u = 0; u != subdiv - 1; ++u ) {
                // the four vertex corners of the quad
                unsigned int i0 = ( u + 0 ) + ( v + 0 ) * subdiv;
                unsigned int i1 = ( u + 0 ) + ( v + 1 ) * subdiv;
                unsigned int i2 = ( u + 1 ) + ( v + 1 ) * subdiv;
                unsigned int i3 = ( u + 1 ) + ( v + 0 ) * subdiv;

                // fill the two triangles, clockwise
                auto it = std::begin(faceIndices) + ( v * (subdiv-1) + u ) * 6;
                *it++ = i1; *it++ = i0; *it++ = i3;
                *it++ = i1; *it++ = i3; *it++ = i2;
            }
        }
        // allocate memory for the full index buffer
        indices.resize( perFaceQuadCount * 2 * 3 * 6 );
        // for each face, copy the per face index buffer with a index shift to map the correct range of vertices
        for( unsigned int face = 0; face != 6; ++face) {
            unsigned int vertexOffs = face * subdiv * subdiv;
            unsigned int indexOffs = face * 2 * 3 * perFaceQuadCount;
            std::transform( std::begin(faceIndices), std::end(faceIndices)
                , std::begin(indices) + indexOffs
                , [vertexOffs]( unsigned int idx) { return idx+vertexOffs; } );
        }
    }

};

CglGeometry::CglGeometry(GEOMETRY geom, char* file){
    mBuffer = nBuffer = cBuffer = iBuffer = bbmBuffer = bbiBuffer = -1;

    glm::vec3 center(0,0,0);
    glm::vec3 scale(1,1,1);
    glm::vec3 color(1,1,1);
    glm::vec3 pt1(0,0,0);
    glm::vec3 pt2(1,1,1);

    //pMaterial->setColor(glm::vec3(R, G, B));
    //pos = glm::vec3(x,y,z);
    //center = glm::vec3(x,y,z);
    //MODEL[3] = glm::vec4(center,1);

    switch(geom){

    case CGL_CUBE:{
        vertices = std::vector<float>{
            -1.0, -1.0,  1.0,
             1.0, -1.0,  1.0,
             1.0,  1.0,  1.0,
            -1.0,  1.0,  1.0,
            -1.0, -1.0, -1.0,
             1.0, -1.0, -1.0,
             1.0,  1.0, -1.0,
            -1.0,  1.0, -1.0,
        };
        for(int i = 0 ; i < vertices.size() ; i+=3){
            vertices[i+0] = scale.x * vertices[i+0];
            vertices[i+1] = scale.y * vertices[i+1];
            vertices[i+2] = scale.z * vertices[i+2];
        }
        indices = std::vector<int>{
            0, 1, 2,
            2, 3, 0,
            3, 2, 6,
            6, 7, 3,
            7, 6, 5,
            5, 4, 7,
            4, 5, 1,
            1, 0, 4,
            4, 0, 3,
            3, 7, 4,
            1, 5, 6,
            6, 2, 1
        };
        break;
    }
    case CGL_SPHERE:{
        sphereGeom sphere(10);
        indices = sphere.indices;
        for(int i = 0 ; i < sphere.vertices.size() ; i++){
            for(int j = 0 ; j < 3 ; j++)
                vertices.push_back(scale[j] * sphere.vertices[i].pos[j]);}
        normals = vertices;
        break;
    }
    case CGL_CYLINDER:{
        glm::vec3 direction = pt2-pt1;
        glm::vec3 ortho1    = glm::normalize(glm::cross(direction, glm::vec3(0,1,0)));
        glm::vec3 ortho2    = glm::normalize(glm::cross(direction, ortho1));
        //CIRCLES
        std::vector<glm::vec3> circle1, circle2;
        int nb = 20;
        float angleInc = 2*3.14159f / nb;
        float angle = 0;
        for(int i = 0 ; i < nb ; i++){
            circle1.push_back( pt1 +  scale.x*cos(angle)*ortho1 + scale.y*sin(angle)*ortho2 );
            circle2.push_back( pt2 +  scale.x*cos(angle)*ortho1 + scale.y*sin(angle)*ortho2 );
            angle+=angleInc;
        }
        //INDICES
        for(int i = 0 ; i < nb ; i++){
            indices.push_back(i);
            if(i==nb-1)
                indices.push_back(0);
            else
                indices.push_back(i+1);
            indices.push_back(nb + i);
        }
        for(int i = 0 ; i < nb ; i++){
            indices.push_back(nb + i);
            if(i==circle1.size()-1)
                indices.push_back(0);
            else
                indices.push_back(i+1);
            if(i==circle1.size()-1)
                indices.push_back(nb);
            else
                indices.push_back(nb+i+1);
        }
        //VERTICES & NORMALS
        for(int i = 0 ; i < nb ; i++){
            for(int j = 0 ; j < 3 ; j ++){
                vertices.push_back(circle1[i][j]);
                normals.push_back(circle1[i][j] - pt1[j]);
            }
        }
        for(int i = 0 ; i < nb ; i++){
            for(int j = 0 ; j < 3 ; j ++){
                vertices.push_back(circle2[i][j]);
                normals.push_back(circle2[i][j] - pt2[j]);
            }
        }
        break;
    }
    case CGL_MESH:{
        meshFile    = std::string(file);
        cout << "Reading: " << file << " " << endl;

        //Initialisation
        int                     np,nt,nn,dim,ver, nNAtV;
        Point                   *ppt;
        Tria                    *pt;
        double                  *n,dd;
        float                   fp1,fp2,fp3;
        int                     k,inm;
        vector<Point>           point;
        vector<Tria>            tria;
        vector<Normal>          normal;
        vector<NormalAtVertex>  NormalAtVertices;

        //Lecture du .sol
            /*
            std::string solFile   = meshFile.substr(0, meshFile.size()-5) + ".sol";
            cout << solFile << endl << endl;
            int ver2, dim2;
            int inMeshSol = 0;
            inMeshSol = GmfOpenMesh((char*)(solFile.c_str()), GmfRead, &ver2, &dim2);
            if(inMeshSol == 0){cout << "Unable to open sol file" << endl; exit(143);}
            cout << "sol file opened " << (char*)(solFile.c_str()) << endl;
            int type, offset, typtab[GmfMaxTyp];
            int nSol      = GmfStatKwd(inMeshSol, GmfSolAtVertices, &type, &offset, &typtab);
            std::vector<float> values(nSol);
            GmfGotoKwd(inMeshSol, GmfSolAtVertices);
            for(int i = 0 ; i< nSol ; i++){
                double val;
                if ( ver2 == GmfFloat ){
                    GmfGetLin(inMeshSol, GmfSolAtVertices, &values[i]);
                }
                else{
                  GmfGetLin(inMeshSol, GmfSolAtVertices, &val);
                  values[i] = val;
                }
            }
            GmfCloseMesh(inMeshSol);
            //COLORS
            float mini = 1e9;
            float maxi = -1e9;
            for(int i = 0 ; i < values.size() ; i++){
                mini = min(mini, values[i]);
                maxi = max(maxi, values[i]);
            }
            palette = new CglPalette( mini, maxi, CGL_PALETTE_BR );
            palette->setBoundaries(mini, maxi);
            for(int i = 0 ; i < values.size() ; i++){
                glm::vec3 col = palette->getColor(values[i]);
                colors.push_back(col.x);
                colors.push_back(col.y);
                colors.push_back(col.z);
            }
    */

        //Lecture du .mesh
          inm = GmfOpenMesh(file,GmfRead,&ver,&dim);
          cout << file << "/" << ver << "/" << dim << endl;
          if ( !inm ){
            cout << "Unable to open mesh file" << endl;
            exit(0);
          }

          np    = GmfStatKwd(inm, GmfVertices);
          nt    = GmfStatKwd(inm, GmfTriangles);
          nn    = GmfStatKwd(inm, GmfNormals);
          nNAtV = GmfStatKwd(inm, GmfNormalAtVertices);
          if ( !np ){
            cout << "  ** MISSING DATA" << endl;
            exit(0);
          }
          point.resize(np);
          tria.resize(nt);
          normal.resize(np);
          NormalAtVertices.resize(nNAtV + 1);

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
          GmfGotoKwd(inm,GmfTriangles);
          for (k=0; k<nt; k++) {
            pt = &tria[k];
            GmfGetLin(inm,GmfTriangles,&pt->v[0],&pt->v[1],&pt->v[2],&pt->ref);
          }
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

          //VERTICES
          int inv = ((pcv->profile.invertVertical)?-1:1);
          for (int i = 0 ; i < point.size() ; i++){
            vertices.push_back(      point[i].c[0]);
            vertices.push_back(inv * point[i].c[1]);
            vertices.push_back(inv * point[i].c[2]);
          }
          //INDICES
          for (int i = 0 ; i < tria.size() ; i++){
            for(int j = 0 ; j < 3 ; j++)
              indices.push_back(tria[i].v[j]-1);}
          //NORMALS
          if ( nNAtV ) {
            GmfGotoKwd(inm,GmfNormalAtVertices);
            for (k=0; k<nNAtV; k++)
              GmfGetLin(inm,GmfNormalAtVertices,
                        &NormalAtVertices[k].inds[0],
                        &NormalAtVertices[k].inds[1]);
          }
          for(int i = 0 ; i < vertices.size() ; i++){normals.push_back(0.0f);}
          for(int i = 0 ; i < NormalAtVertices.size() - 1 ; i++){
            int indV = NormalAtVertices[i].inds[0] - 1;
            int indN = NormalAtVertices[i].inds[1] - 1;
            normals[3 * indV + 0] =       normal[indN].n[0];
            normals[3 * indV + 1] = inv * normal[indN].n[1];
            normals[3 * indV + 2] = inv * normal[indN].n[2];
          }

        GmfCloseMesh(inm);
        break;
    }
    case CGL_PLANE:{
        std::vector<float> vertices = {-1,0,1,
                                        1,0,1,
                                        1,0,-1,
                                        -1,0,-1
                                        };
        std::vector<int> indices = {0,1,2,
                                    1,2,3
                                    };
        std::vector<float> normals = {  0,1,0,
                                        0,1,0
                                        };
    }
    }

    getBBOX(vertices);
    computeBuffers();
    //A récupérer par l'objet lors de la création
    /*
    center = glm::vec3(x,y,z);
    MODEL[3] = glm::vec4(center,1);
    */
}
void CglGeometry::getBBOX(std::vector<float> &V){
    //Compute bounding box
    bbmin = glm::vec3(FLOAT_MAX);
    bbmax = glm::vec3(-FLOAT_MAX);
    for (int k=0; k<V.size(); k+=3) {
        if ( V[k+0] < bbmin.x ) bbmin.x = V[k+0];
        if ( V[k+0] > bbmax.x ) bbmax.x = V[k+0];
        if ( V[k+1] < bbmin.y ) bbmin.y = V[k+1];
        if ( V[k+1] > bbmax.y ) bbmax.y = V[k+1];
        if ( V[k+2] < bbmin.z ) bbmin.z = V[k+2];
        if ( V[k+2] > bbmax.z ) bbmax.z = V[k+2];
    }

    //Translate mesh to center
    tra = 0.5f * (bbmin + bbmax);
    for (int k=0; k<V.size(); k+=3) {
        V[k+0] -= tra.x;
        V[k+1] -= tra.y;
        V[k+2] -= tra.z;
    }

    //Scale computation
    {
        glm::vec3 size    = bbmax - bbmin;
        float maxDim      = max( max(size.x, size.y) , size.z );
        localScale     = 0.5f / maxDim;
        //Independant scale -> Chaque objet est ramené de sorte que sa plus grande dimension egale 1.
        if(pcv->profile.independantScale){
            scaleFactor = localScale;
        }
    }

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
    std::vector<short> elements{
        0, 1, 2, 3,
        4, 5, 6, 7,
        0, 4, 1, 5, 2, 6, 3, 7
    };
    createBuffer(&bbmBuffer, &cube);
    createBuffer(&bbiBuffer, &elements);
    freeBuffer();
}
void CglGeometry::computeBuffers(){
    createBuffer(&mBuffer, &vertices);
    createBuffer(&iBuffer, &indices);
    nBuffer = -1;
    cBuffer = -1;
    if(normals.size() > 0)
        createBuffer(&nBuffer, &normals);
    if(colors.size() > 0)
        createBuffer(&cBuffer, &colors);
    freeBuffer();
    nTriangles = indices.size()/3;
    cout << mBuffer << " / " << iBuffer << " / " << nBuffer << " / " << cBuffer << "  /  " << nTriangles << endl;
}


// object constructor
CglObject::CglObject(){
  pcv->addObject(this);
  parent=NULL;

  selected = false;
  box      = false;
  line     = false;
  hidden   = false;
  dynamic  = true;

  MODEL  = glm::mat4(1.0f);
  //center = glm::vec3(0.0f);

  //pScene = NULL;
  pGroup = NULL;
  pMaterial   = new CglMaterial(pcv->profile.color(), 0.85, 0.15, 12.0);
}
CglObject::~CglObject(){}

void CglObject::pickingDisplay(){
  if(!hidden){
    int shaderID = pcv->simpleID();
    glUseProgram(shaderID);
    int MatrixID = glGetUniformLocation(shaderID, "MVP");
    int colorID  = glGetUniformLocation(shaderID, "COL");

    glm::mat4 MVP = sPROJ() * sVIEW() * sMODEL() * glm::scale(MODEL, glm::vec3(pGeom->scaleFactor));;

    uniform(MatrixID, MVP);
    uniform(colorID, pickingColor);

    glPolygonMode(GL_FRONT, GL_FILL);
    draw(shaderID, 3*pGeom->nTriangles, pGeom->mBuffer, -1, pGeom->iBuffer);
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
        if(!pGroup)
            rotationCenter = &center;
    }
    else{
        if(!pGroup)
            rotationCenter = parent->getCenterPtr();
    }

    if ((transform.tr != glm::vec3(0.0f)) || (transform.rot != glm::mat4(1.0f))){
        glm::mat4 rot  = glm::mat4(1.0f);
        glm::mat4 ID = glm::mat4(1.0f);

        if(!isSuper())
            MODEL =  glm::translate(ID, *rotationCenter) * transform.rot * glm::translate(ID, -*rotationCenter) * MODEL;

        //if(isMesh)
        //if(!pGroup){
            glm::vec3 sC = sCENTER();
            glm::vec3 bbC = 0.5f * (getBBMAX() + getBBMIN());
            glm::vec3 newMinPos = sC  + glm::vec3(MODEL[3]) + transform.tr + pGeom->scaleFactor * (getBBMIN()-bbC);
            glm::vec3 newMaxPos = sC  + glm::vec3(MODEL[3]) + transform.tr + pGeom->scaleFactor * (getBBMAX()-bbC);

            std::cout << bbC.x << " " << bbC.y << " " << bbC.z << std::endl;

            glm::vec3 trans(0,0,0);
            float s = pScene->getScale();
            glm::vec3 mins = pcv->getScene()->getAxis()->getBBMIN();
            glm::vec3 maxs = pcv->getScene()->getAxis()->getBBMAX();

            if( (newMaxPos.x < sC.x + s * maxs.x ) && (newMinPos.x > sC.x + s * mins.x) )
                trans.x += transform.tr.x;
            //if( (newMaxPos.y < sC.y + s * maxs.y ) && (newMinPos.y > sC.y + s * mins.y) )
            //    trans.y += transform.tr.y;
            if( (newMaxPos.z < sC.z + s * maxs.z ) && (newMinPos.z > sC.z + s * mins.z) )
                trans.z += transform.tr.z;

            MODEL = glm::translate(ID, trans) * MODEL;
            center += trans;// glm::vec3(MODEL[3]);
        //}

        /*else{
            //CHECK IF OK FOR EVERYONE IN THE GROUP
            float trX = trY = trZ = 10000f;
            glm::vec3 groupTr;
            for(int i = 0 ; i < pScene->numObjects() ; i++){
                pCglObject obj = pScene->getObject[i];
                if(obj->getGroup() == pGroup){
                    glm::vec3 bbC = 0.5f * (obj->getBBMAX() + obj->getBBMIN());
                    glm::vec3 newMinPos = glm::vec3(pScene->getMODEL()[3]) + glm::vec3(obj->getMODEL()[3]) + transform.tr + scaleFactor*(obj->getBBMIN()-bbC);
                    glm::vec3 newMaxPos = glm::vec3(pScene->getMODEL()[3]) + glm::vec3(obj->getMODEL()[3]) + transform.tr + scaleFactor*(obj->getBBMAX()-bbC);
                    glm::vec3 trans(0,0,0);
                    float scale = pScene->getScale();
                    glm::vec3 mins = pcv->getScene()->getAxis()->getBBMIN();
                    glm::vec3 maxs = pcv->getScene()->getAxis()->getBBMAX();
                    if( (newMaxPos.x < sMODEL()[3].x + scale * maxs.x ) && (newMinPos.x > sMODEL()[3].x + scale * mins.x) )
                        trans.x = transform.tr.x;
                    if( (newMaxPos.y < sMODEL()[3].y + scale * maxs.y ) && (newMinPos.y > sMODEL()[3].y + scale * mins.y) )
                        trans.y = transform.tr.y;
                    if( (newMaxPos.z < sMODEL()[3].z + scale * maxs.z ) && (newMinPos.z > sMODEL()[3].z + scale * mins.z) )
                        trans.z = transform.tr.z;
                    trX = min(trX, trans.x);
                    trY = min(trY, trans.y);
                    trZ = min(trZ, trans.z);
                }
            }
            groupTr = glm::vec3(trX, trY, trZ);
            for(int i = 0 ; i < pScene->numObjects() ; i++){
                pCglObject obj = pScene->getObject[i];
                if(obj->getGroup() == pGroup){
                    obj->transform.reset();
                    obj->transform.tr = groupTr;
                }
            }
        }*/
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

void CglObject::computeGroup(){
    pCglScene scene = pcv->getScene();
    if (scene->numGroups() > 0){
      for(int i = 0 ; i < scene->numGroups() ; i++)
        for(int j = 0 ; j < scene->getGroup(i)->numObjects() ; j++){
            pCglObject pOBJ = scene->getGroup(i)->getObject(j);
            if( (this==pOBJ) || (this->parent == pOBJ))
                pGroup = scene->getGroup(i);
        }
    }
    else
      pGroup = NULL;
}

void CglObject::setRotationCenter(glm::vec3 &center){rotationCenter = &center;}
void CglObject::setScaleFactor(float sf){pGeom->scaleFactor = sf;}

void CglObject::setMODEL(glm::mat4 M){MODEL = M;}
void CglObject::setCenter(glm::vec3 C){center = C;}
void CglObject::setGroup(pCglGroup G){pGroup = G;}
void CglObject::setFileName(std::string n){pGeom->meshFile = n;}
void CglObject::setColor(glm::vec3 col){pMaterial->setColor(col);}

pCglGroup  CglObject::getGroup(){return pGroup;}
void       CglObject::resetGroup(){pGroup = NULL;}
float      CglObject::getLocalScale(){return pGeom->localScale;}
int        CglObject::getID(){return objectID;}
glm::vec3* CglObject::getCenterPtr(){return &center;}

//Toogle render modes
void CglObject::toogleBBox()   {box = !box;
                                    if(isSuper()){
                                        for(int i = 0 ; i < listPart.size() ; i++){
                                            listPart[i]->toogleBBox();
                                        }
                                    }
                                }
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
