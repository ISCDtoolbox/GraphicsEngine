#include <glic/axis.h>
#include <glic/canvas.h>
extern CglicCanvas *pcv;

CglicAxis::CglicAxis(){
  //Grid creation
  float size       = 2;
  float resolution = 20;
  float dash_size = size/resolution;
  std::vector<glm::vec3> tGrid;
  for(float x = -size/2 ; x <= size/2 ; x+=dash_size){
    for(float z = -size/2 ; z <= size/2 ; z+=dash_size){
      tGrid.push_back(glm::vec3(x-dash_size/2, 0., z));
      tGrid.push_back(glm::vec3(x+dash_size/2, 0., z));
      tGrid.push_back(glm::vec3(x, 0., z-dash_size/2));
      tGrid.push_back(glm::vec3(x, 0., z+dash_size/2));
    }
  }
  for(int i = 0 ; i < tGrid.size() ; i++){
    tGrid[i] = glm::rotate( glm::angleAxis(pcv->profile.bottomAngle, glm::vec3(0, 1, 0)), glm::vec3(tGrid[i]));
    tGrid[i] = glm::vec3(glm::translate(glm::mat4(1),glm::vec3(0,-pcv->profile.bottomDistance,0)) * glm::vec4(tGrid[i], 1));
    for(int j = 0 ; j < 3 ; j++)
      grid.push_back(tGrid[i][j]);
  }
  glGenBuffers( 1,               &gridBuffer);
  glBindBuffer( GL_ARRAY_BUFFER, gridBuffer);
  glBufferData( GL_ARRAY_BUFFER, sizeof(float) * grid.size(), &grid[0], GL_STATIC_DRAW);

  //Axes creation
  std::vector<glm::vec3> tAxes = {glm::vec3(0,0,0),
                                  glm::vec3(1,0,0),
                                  glm::vec3(0,1,0),
                                  glm::vec3(0,0,0),
                                  glm::vec3(0,0,1),
                                  glm::vec3(0,0,0)};
  for(int i = 0 ; i < tAxes.size() ; i++)
    for(int j = 0 ; j < 3 ; j++)
      axes.push_back(0.4*tAxes[i][j]);
  glGenBuffers( 1,               &axesBuffer);
  glBindBuffer( GL_ARRAY_BUFFER, axesBuffer);
  glBufferData( GL_ARRAY_BUFFER, sizeof(float) * axes.size(), &axes[0], GL_STATIC_DRAW);
}

void CglicAxis::display()
{
  //Initialization
  glUseProgram(pcv->simpleShader.mProgramID);
  glEnableVertexAttribArray( 0);
  GLuint MatrixID = glGetUniformLocation(pcv->simpleShader.mProgramID, "MVP");
  GLuint colorID  = glGetUniformLocation(pcv->simpleShader.mProgramID, "COL");

  //GRID
  if(pcv->profile.displayBottomGrid){
    glm::mat4 MVP = glm::translate( *pPROJ * *pVIEW * MODEL, center);
    glUniformMatrix4fv( MatrixID, 1, GL_FALSE, &MVP[0][0]);
    glLineWidth(1.0);
    glBindBuffer(              GL_ARRAY_BUFFER, gridBuffer);
    glVertexAttribPointer(     0, 3, GL_FLOAT, GL_FALSE, 0, ( void*)0);
    glBindAttribLocation(      pcv->simpleShader.mProgramID, 0, "vertex_position");
    uniformVec3(colorID, pcv->profile.grid_color);
    glPolygonMode(GL_FRONT, GL_LINE);
    glDrawArrays(GL_LINES, 0, grid.size()/3);
  }

  //Axes
  if(pcv->profile.displayAxes){
    pCglicScene scene = pcv->scene[pcv->window[pcv->winid()].ids];
    //glDisable(GL_DEPTH_TEST);
    glDepthFunc(GL_ALWAYS);
    glViewport(0,0,150,150);
    glm::mat4 neutralProj = glm::perspective(70.0, view->ratio, view->m_znear, view->m_zfar);
    glm::mat4 MVP = glm::scale(glm::translate(neutralProj * *pVIEW * MODEL, glm::vec3(0)/*-*sceneCenter*/), glm::vec3(view->zoom));

    glUniformMatrix4fv( MatrixID, 1, GL_FALSE, &MVP[0][0]);
    glLineWidth(2.0);
    glBindBuffer(              GL_ARRAY_BUFFER, axesBuffer);
    glVertexAttribPointer(     0, 3, GL_FLOAT, GL_FALSE, 0, ( void*)0);
    glBindAttribLocation(      pcv->simpleShader.mProgramID, 0, "vertex_position");
    glPolygonMode(GL_FRONT, GL_LINE);
    //X
    uniformVec3(colorID, glm::vec3(1,0,0));
    glDrawArrays(GL_LINES, 0, 2);
    //Y
    uniformVec3(colorID, glm::vec3(0,1,0));
    glDrawArrays(GL_LINES, 2, 4);
    //Z
    uniformVec3(colorID, glm::vec3(0,0,1));
    glDrawArrays(GL_LINES, 4, 6);
  }

  //Ressources freeing
  //glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  view->reshape(view->width,view->height);
  glDisableVertexAttribArray(0);
  glLineWidth(1.0);
  glUseProgram(0);
  glPolygonMode(GL_FRONT, GL_FILL);
}
