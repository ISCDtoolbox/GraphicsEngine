#include <cgl/axis.h>
#include <cgl/canvas.h>
extern CglCanvas *pcv;

CglAxis::CglAxis(){
    float size       = 10;

    //Main grid
    float dash_size = 0.2;
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
        mainGrid.push_back(tGrid[i][j]);
    }

    //Secondary grid
    dash_size = 0.05;
    std::vector<glm::vec3> tsGrid;
    for(float x = -size/2 ; x <= size/2 ; x+=dash_size){
        for(float z = -size/2 ; z <= size/2 ; z+=dash_size){
            tsGrid.push_back(glm::vec3(x-dash_size/2, 0., z));
            tsGrid.push_back(glm::vec3(x+dash_size/2, 0., z));
            tsGrid.push_back(glm::vec3(x, 0., z-dash_size/2));
            tsGrid.push_back(glm::vec3(x, 0., z+dash_size/2));
        }
    }
    for(int i = 0 ; i < tsGrid.size() ; i++){
        tsGrid[i] = glm::rotate( glm::angleAxis(pcv->profile.bottomAngle, glm::vec3(0, 1, 0)), glm::vec3(tsGrid[i]));
        tsGrid[i] = glm::vec3(glm::translate(glm::mat4(1),glm::vec3(0,-pcv->profile.bottomDistance,0)) * glm::vec4(tsGrid[i], 1));
        for(int j = 0 ; j < 3 ; j++)
        secondaryGrid.push_back(tsGrid[i][j]);
    }

    //Axes
    std::vector<glm::vec3> tAxes = {glm::vec3(0,0,0),
                                    glm::vec3(1,0,0),
                                    glm::vec3(0,1,0),
                                    glm::vec3(0,0,0),
                                    glm::vec3(0,0,1),
                                    glm::vec3(0,0,0)};
    for(int i = 0 ; i < tAxes.size() ; i++)
        for(int j = 0 ; j < 3 ; j++)
        axes.push_back(0.4*tAxes[i][j]);

    //Buffers creation
    createBuffer(&mainGridBuffer,       &mainGrid);
    createBuffer(&secondaryGridBuffer,  &secondaryGrid);
    createBuffer(&axesBuffer,           &axes);
}


void CglAxis::display(){
    int shaderID = initProgram(pcv->simpleID());
    glPolygonMode(GL_FRONT, GL_LINE);

    GLuint MatrixID = glGetUniformLocation(shaderID, "MVP");
    GLuint colorID  = glGetUniformLocation(shaderID, "COL");
    GLuint MID      = glGetUniformLocation(shaderID, "M");
    GLuint VID      = glGetUniformLocation(shaderID, "V");

    //GRID
    if(pcv->profile.displayBottomGrid){
        enableFog(shaderID);

        glm::mat4 VIEW  = sVIEW();
        glm::mat4 MVP   = glm::translate( sPROJ() * VIEW * MODEL, center);

        uniform(MatrixID,   MVP);
        uniform(MID,        MODEL);
        uniform(VID,        VIEW);
        uniform(colorID,    pcv->profile.grid_color);

        glLineWidth(1.0);
        bindBuffer(0, GL_ARRAY_BUFFER, secondaryGridBuffer);
        glBindAttribLocation( shaderID, 0, "vertex_position");
        glDrawArrays(GL_LINES, 0, secondaryGrid.size()/3);

        glLineWidth(2.0);
        bindBuffer(0, GL_ARRAY_BUFFER, mainGridBuffer);
        glBindAttribLocation( shaderID, 0, "vertex_position");
        glDrawArrays(GL_LINES, 0, mainGrid.size()/3);

        disableFog(shaderID);
    }

    //Axes
    if(pcv->profile.displayAxes){
        glDisable(GL_DEPTH_TEST);
        glViewport(0,0,view->width/6,view->width/6);

        glm::mat4 NEUTRAL   = glm::mat4(glm::perspective(70.0, 1.0, view->m_znear, view->m_zfar)) * sVIEW() * MODEL;
        glm::mat4 M         = glm::scale(NEUTRAL, glm::vec3(view->zoom));
        uniform(MatrixID, M);

        bindBuffer(0, GL_ARRAY_BUFFER, axesBuffer);
        glBindAttribLocation( shaderID, 0, "vertex_position");

        glLineWidth(2.0);
        uniform(colorID, glm::vec3(1,0,0));
        glDrawArrays(GL_LINES, 0, 2);
        uniform(colorID, glm::vec3(0,1,0));
        glDrawArrays(GL_LINES, 2, 4);
        uniform(colorID, glm::vec3(0,0,1));
        glDrawArrays(GL_LINES, 4, 6);

        view->reshape(view->width,view->height);
        glEnable(GL_DEPTH_TEST);
    }

    glLineWidth(1.0);
    glPolygonMode(GL_FRONT, GL_FILL);
    freeBuffer();
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
    gradient_colors  = {glm::vec3(0.9), glm::vec3(0.9)};
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

