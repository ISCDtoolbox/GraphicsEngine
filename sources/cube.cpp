#include <cgl/cube.h>

#include "cgl/canvas.h"
extern CglCanvas *pcv;

#include <algorithm>
/*
std::vector<Point> tovecs3(std::vector<float> V0){
    std::vector<Point> V1;
    for(int i = 0 ; i < V0.size() ; i+=3){
        Point P;
        for(int j = 0 ; j < 3 ; j++)
            P.c[j] = V0[i+j];
        V1.push_back(P);
    }
    return V1;
}

struct sphereGeom
{
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










CglCube::CglCube(float x, float y, float z,
                 float R, float G, float B){

    size = 0.01f;
    pMaterial->setColor(glm::vec3(R, G, B));
    pos = glm::vec3(x,y,z);

    std::vector<float> cube{
        -1.0, -1.0,  1.0,
         1.0, -1.0,  1.0,
         1.0,  1.0,  1.0,
        -1.0,  1.0,  1.0,
        -1.0, -1.0, -1.0,
         1.0, -1.0, -1.0,
         1.0,  1.0, -1.0,
        -1.0,  1.0, -1.0,
    };
    for(int i = 0 ; i < cube.size() ; i++)
        cube[i] = size * cube[i];

    vector<Point> V = tovecs3(cube);
    getBBOX(V);

    std::vector<int> elements{
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

    nTriangles = 3*elements.size();

    center = glm::vec3(x,y,z);
    MODEL[3] = glm::vec4(center,1);

    normalBuffer = -1;
    createBuffer(&meshBuffer, &cube);
    createBuffer(&indicesBuffer, &elements);
    freeBuffer();
}
CglCube::~CglCube(){}


CglSphere::CglSphere(float x, float y, float z,
                     float R, float G, float B,
                     float S){
    size = S;
    pMaterial->setColor(glm::vec3(R, G, B));
    pos = glm::vec3(x,y,z);

    sphereGeom sphere(10);
    std::vector<int> elements = sphere.indices;
    std::vector<float> cube;
    for(int i = 0 ; i < sphere.vertices.size() ; i++){
        cube.push_back(size * sphere.vertices[i].pos[0]);
        cube.push_back(size * sphere.vertices[i].pos[1]);
        cube.push_back(size * sphere.vertices[i].pos[2]);
    }

    nTriangles = 3*elements.size();

    vector<Point> V = tovecs3(cube);
    getBBOX(V);

    center = glm::vec3(x,y,z);
    MODEL[3] = glm::vec4(center,1);

    normalBuffer = -1;
    createBuffer(&normalBuffer, &cube);
    createBuffer(&meshBuffer, &cube);
    createBuffer(&indicesBuffer, &elements);
    freeBuffer();
}
CglSphere::~CglSphere(){}


CglCylinder::CglCylinder(float x,  float y,  float z,
                         float x2, float y2, float z2,
                         float R, float G, float B){
    init(x,y,z, x2,y2,z2);
}

CglCylinder::CglCylinder(pCglPrimitive obj1, pCglPrimitive obj2,
                         float R, float G, float B){
    glm::vec3 c1 = obj1->getPos();
    glm::vec3 c2 = obj2->getPos();
    init(c1.x, c1.y, c1.z, c2.x, c2.y, c2.z, R, G, B);
}

void CglCylinder::init(float x,  float y,  float z,
                       float x2, float y2, float z2,
                       float R, float G, float B){

    size = 0.003f;
    pMaterial->setColor(glm::vec3(R, G, B));
    glm::vec3 pt1(x,y,z);
    glm::vec3 pt2(x2,y2,z2);
    glm::vec3 direction = pt2-pt1;
    glm::vec3 ortho1    = glm::normalize(glm::cross(direction, glm::vec3(0,1,0)));
    glm::vec3 ortho2    = glm::normalize(glm::cross(direction, ortho1));

    //MAKE CIRCLES IN POINTS
    std::vector<glm::vec3> circle1, circle2;
    int nb = 20;
    float angleInc = 2*3.14159f / nb;
    float angle = 0;



    for(int i = 0 ; i < nb ; i++){
        circle1.push_back( pt1 +  size * (cos(angle)*ortho1 + sin(angle)*ortho2) );
        circle2.push_back( pt2 +  size * (cos(angle)*ortho1 + sin(angle)*ortho2) );
        angle+=angleInc;
    }

    std::vector<int> indices;
    //Partie haute des faces
    for(int i = 0 ; i < nb ; i++){
        indices.push_back(i);
        if(i==nb-1)
            indices.push_back(0);
        else
            indices.push_back(i+1);
        indices.push_back(nb + i);
    }
    //Partie basse des faces
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

    std::vector<float> cube;
    std::vector<float> normals;
    for(int i = 0 ; i < nb ; i++){
        for(int j = 0 ; j < 3 ; j ++){
            cube.push_back(circle1[i][j]);
            normals.push_back(circle1[i][j] - pt1[j]);
        }
    }
    for(int i = 0 ; i < nb ; i++){
        for(int j = 0 ; j < 3 ; j ++){
            cube.push_back(circle2[i][j]);
            normals.push_back(circle2[i][j] - pt2[j]);
        }
    }

    vector<Point> V = tovecs3(cube);
    getBBOX(V);

    std::vector<int> elements = indices;
    nTriangles = 3*elements.size();
    //center = 0.5f * (pt1 + pt2);
    MODEL[3] = glm::vec4(center,1);
    normalBuffer = -1;
    createBuffer(&normalBuffer, &normals);
    createBuffer(&meshBuffer, &cube);
    createBuffer(&indicesBuffer, &elements);
    freeBuffer();
}
CglCylinder::~CglCylinder(){}

*/
