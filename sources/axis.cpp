#include <cgl/axis.h>
#include <cgl/canvas.h>
extern CglCanvas *pcv;

CglAxis::CglAxis(){
  //Grid creation
  float size       = 5;
  float resolution = 100;
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

void CglAxis::display()
{
  //Initialization
  glUseProgram(pcv->simpleID());
  glEnableVertexAttribArray( 0);
  GLuint MatrixID = glGetUniformLocation(pcv->simpleID(), "MVP");
  GLuint colorID  = glGetUniformLocation(pcv->simpleID(), "COL");

  //GRID
  if(pcv->profile.displayBottomGrid){
    glm::mat4 MVP = glm::translate( *pPROJ * *pVIEW * MODEL, center);
    glUniformMatrix4fv( MatrixID, 1, GL_FALSE, &MVP[0][0]);
    glLineWidth(1.0);
    glBindBuffer(              GL_ARRAY_BUFFER, gridBuffer);
    glVertexAttribPointer(     0, 3, GL_FLOAT, GL_FALSE, 0, ( void*)0);
    glBindAttribLocation(      pcv->simpleID(), 0, "vertex_position");
    uniformVec3(colorID, pcv->profile.grid_color);
    glPolygonMode(GL_FRONT, GL_LINE);
    glDrawArrays(GL_LINES, 0, grid.size()/3);
  }

  //Axes
  if(pcv->profile.displayAxes){
    pCglScene scene = pcv->getScene();;
    //glDisable(GL_DEPTH_TEST);
    glViewport(0,0,view->width/6,view->width/6);
    glm::mat4 neutralProj = glm::perspective(70.0, 1.0, view->m_znear, view->m_zfar);
    glm::mat4 MVP = glm::scale(neutralProj * *pVIEW * MODEL, glm::vec3(view->zoom));

    glUniformMatrix4fv( MatrixID, 1, GL_FALSE, &MVP[0][0]);
    glLineWidth(2.0);
    glBindBuffer(              GL_ARRAY_BUFFER, axesBuffer);
    glVertexAttribPointer(     0, 3, GL_FLOAT, GL_FALSE, 0, ( void*)0);
    glBindAttribLocation(      pcv->simpleID(), 0, "vertex_position");
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
    //glEnable(GL_DEPTH_TEST);
  }

  //Ressources freeing
  glDepthFunc(GL_LEQUAL);
  view->reshape(view->width,view->height);
  glDisableVertexAttribArray(0);
  glLineWidth(1.0);
  glUseProgram(0);
  glPolygonMode(GL_FRONT, GL_FILL);
}



void CglBackground::display(){
  //Background gradient
  std::vector<float>     gradient_heights;
  std::vector<glm::vec3> gradient_colors;
  CGL_THEME theme = pcv->profile.theme;

  if(theme == CGL_THEME_DARK){
    gradient_colors  = {glm::vec3(0.0), glm::vec3(0.25), glm::vec3(0.65)};
    gradient_heights = {-1, 0.25, 1};
  }
  else if (theme == CGL_THEME_BLACK){
    gradient_colors  = {glm::vec3(0.1), glm::vec3(0.1)};
    gradient_heights = {-1, 1};
  }
  else if (theme == CGL_THEME_CLEAR){
    gradient_colors  = {glm::vec3(1), glm::vec3(1), glm::vec3(0.5, 0.5, 0.65)};
    gradient_heights = {-1, -0.2, 1};
  }
  else if (theme == CGL_THEME_WHITE){
    gradient_colors  = {glm::vec3(1), glm::vec3(1)};
    gradient_heights = {-1, 1};
  }

  glUseProgram(0);
  glDisable(GL_DEPTH_TEST);
  glShadeModel(GL_SMOOTH);
  gradient(gradient_heights, gradient_colors);
  glEnable(GL_DEPTH_TEST);
}

void CglBackground::gradient(std::vector<float> hei, std::vector<glm::vec3> col){
  if(hei.size()!=col.size())
    exit(122);
  glBegin(GL_QUAD_STRIP);
  int nb = hei.size();
  float depth = 0;
  for(int i = 0 ; i < nb ; i++){
    glColor3f(col[i].x, col[i].y, col[i].z);
    glVertex2f(-1, hei[i]);
    glVertex2f( 1, hei[i]);
  }
  glEnd();
}

void CglBackground::gradient(std::vector<glm::vec2> hei, std::vector<glm::vec3> col){
  if(hei.size()!=col.size())
    exit(122);
  glBegin(GL_QUAD_STRIP);
  int nb = hei.size();
  float depth = 1;
  for(int i = 0 ; i < nb ; i++){
    glColor3f(col[i].x, col[i].y, col[i].z);
    glVertex3f(-1, hei[i].x, depth);
    glVertex3f( 1, hei[i].y, depth);
  }
  glEnd();
}

