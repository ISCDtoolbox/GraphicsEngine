#include <cgl/mesh.h>
extern "C" {
#include <libmesh5.h>
}

#include <cgl/canvas.h>
extern CglCanvas *pcv;

CglMesh::CglMesh(pCglObject M){
    pGeom = new CglGeometry(*(M->pGeom));
    pGeom->computeBuffers();
}

CglMesh::CglMesh(char *name){
    pGeom = new CglGeometry(CGL_MESH, name);
}
/*
void CglMesh::shadowsDisplay(){
    if(pcv->profile.displayShadows && !hidden){
        displayShadow();
    }
    else if(pcv->profile.displayReflection){
        displayReflection();
    }
}*/
void CglMesh::shadowsDisplay(){
    if((pcv->profile.ground==CGL_GROUND_SHADOWS) && !hidden){
        displayShadow();
    }
    else if((pcv->profile.ground==CGL_GROUND_REFLECTION)){
        displayReflection();
    }
}
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
void CglMesh::displayShadow(){
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    int shaderID        = initProgram(pcv->simpleID());
    int MatrixID        = glGetUniformLocation(shaderID, "MVP");
    int colorID         = glGetUniformLocation(shaderID, "COL");

    glm::mat4 shadowMVP =   sPROJ() * sVIEW() * sMODEL() *
                            shadowMatrix( glm::vec4(glm::vec3(0,1,0), pcv->profile.bottomDistance + sMODEL()[3].y + 0.002), glm::vec4(glm::vec3(0,1,0), 0) ) *
                            glm::scale(MODEL, glm::vec3(scaleFactor));

    uniform(MatrixID,   shadowMVP);
    uniform(colorID,    glm::vec3(0.2));

    if(pcv->profile.displayBottomGrid)
        glEnable(GL_STENCIL_TEST);
    draw(shaderID, pGeom->nTriangles, pGeom->mBuffer, pGeom->nBuffer, pGeom->iBuffer, pGeom->cBuffer);
    if(pcv->profile.displayBottomGrid)
        glDisable(GL_STENCIL_TEST);
    freeBuffer();
    glDisable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}
void CglMesh::displayReflection(){
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    int shaderID    = initProgram( ((pcv->profile.smooth)?pcv->smoothID():pcv->flatID()) );
    int MatrixID    = glGetUniformLocation(shaderID, "MVP");
    int colorID     = glGetUniformLocation(shaderID, "COL");
    int MID         = glGetUniformLocation(shaderID, "M");

    //Calcul de la couleur
    glm::vec3 selection_color   = ((!pGroup)?pcv->profile.sele_color:pGroup->getColor());
    glm::vec3 color             = ((isSelected())?selection_color:pMaterial->getColor());
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

    if(pcv->profile.displayBottomGrid)
        glEnable(GL_STENCIL_TEST);
    draw(shaderID, pGeom->nTriangles, pGeom->mBuffer, pGeom->nBuffer, pGeom->iBuffer, pGeom->cBuffer);
    if(pcv->profile.displayBottomGrid)
        glDisable(GL_STENCIL_TEST);

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
        glm::vec3 selection_color   = ((!pGroup)?pcv->profile.sele_color:pGroup->getColor());
        computeGroup();

        //Contour
        if(isSelected()){
            glLineWidth(10.0);
            glDisable(GL_DEPTH_TEST);
            uniform(colorID,  selection_color);
            uniform(MatrixID, MVP);
            draw(shaderID, pGeom->nTriangles, pGeom->mBuffer, -1, pGeom->iBuffer);
            glEnable(GL_DEPTH_TEST);
            glLineWidth(1.0);
        }

        //Box
        if(box){
            glEnable(GL_POLYGON_OFFSET_LINE);
            glPolygonOffset(10,0);
            glm::mat4 SCALE = glm::scale(MVP, 1.02f * (pGeom->bbmax - pGeom->bbmin));
            glm::vec3 color = ((isSelected())?selection_color:pcv->profile.idle_color);
            uniform( MatrixID, SCALE);
            uniform(colorID, color);
            glLineWidth(((isSelected())?2.0:1.0));
            bindBuffer(0, GL_ARRAY_BUFFER, pGeom->bbmBuffer);
            bindBuffer(-1, GL_ELEMENT_ARRAY_BUFFER, pGeom->bbiBuffer);
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
    }
}

void CglMesh::display(){
    if(!hidden){
        glEnable(GL_CULL_FACE);
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(1.0,1.0);
        int shaderID                = initProgram(((pcv->profile.smooth)? pcv->smoothID() : pcv->flatID()));
        if(line)
            shaderID = initProgram(pcv->simpleID());
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
        glm::vec3 selection_color   = ((!pGroup)?pcv->profile.sele_color:pGroup->getColor());
        glm::vec3 color             = ((isSelected())?1.0f * selection_color:pMaterial->getColor());
        std::vector<pCglLight> lights = pcv->getSubWindow()->getScene()->getLights();

        uniform( MatrixID, MVP);
        uniform( MID, M);
        uniform( VID, V);
        uniform( colorID, color);
        if(line)
            uniform(colorID, glm::vec3(1,1,1));
        uniform( fill_light_ID, *(lights[0]->getLightMatrix(pMaterial)));
        uniform( side_light_ID, *(lights[1]->getLightMatrix(pMaterial)));
        uniform( back_light_ID, *(lights[2]->getLightMatrix(pMaterial)));

        enableFog(shaderID);
        glPolygonMode(GL_FRONT, GL_FILL);
        draw(shaderID, pGeom->nTriangles, pGeom->mBuffer, pGeom->nBuffer, pGeom->iBuffer, pGeom->cBuffer);
        glDisable(GL_POLYGON_OFFSET_FILL);

        //Wireframe
        if(line){
            shaderID = initProgram(pcv->flatID());
            MatrixID = glGetUniformLocation(shaderID, "MVP");
            colorID  = glGetUniformLocation(shaderID, "COL");
            color = ((isSelected())?0.6f * selection_color:0.6f * pMaterial->getColor());
            uniform( MatrixID, MVP);
            uniform( colorID, color);
            glDisable(GL_POLYGON_OFFSET_FILL);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            draw(shaderID, pGeom->nTriangles, pGeom->mBuffer, -1, pGeom->iBuffer);
        }

        freeBuffer();
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDisable(GL_CULL_FACE);
    }
}



