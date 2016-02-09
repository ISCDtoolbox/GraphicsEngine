#include "cgl/shader.h"
#include <fstream>

using namespace std;

CglShader::CglShader(){
  mVertex_file_path   = "";
  mFragment_file_path = "";
  mProgramID          = 0;
}


void CglShader::load(std::string vertex_file_path, std::string fragment_file_path, std::string functions_file_path){
  mVertex_file_path   = vertex_file_path;
  mFragment_file_path = fragment_file_path;

  // Create the shaders
  GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
  GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

  // Read the Vertex Shader code from the file
  std::string VertexShaderCode;
  std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
  if(VertexShaderStream.is_open()){
    std::string Line = "";
    while(getline(VertexShaderStream, Line))
      VertexShaderCode += "\n" + Line;
    VertexShaderStream.close();
  }
  else{
    std::cout << "Impossible to open " << vertex_file_path << std::endl;
    exit(-1);
  }

  // Read the Fragment Shader code from the file
  std::string FragmentShaderCode;
  std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
  if(FragmentShaderStream.is_open()){
    std::string Line = "";
    while(getline(FragmentShaderStream, Line))
      FragmentShaderCode += "\n" + Line;
    FragmentShaderStream.close();
  }
  else{
    std::cout << "Impossible to open " << fragment_file_path << std::endl;
    exit(-1);
  }

  //Read the Functions from the functions file
  std::string functionsCode;
  std::ifstream functionsStream(functions_file_path, std::ios::in);
  if(functionsStream.is_open()){
    std::string Line = "";
    while(getline(functionsStream, Line))
      functionsCode += "\n" + Line;
    functionsStream.close();
  }
  else{
    std::cout << "Impossible to open " << functions_file_path << std::endl;
    exit(-1);
  }

  //VertexShaderCode += functionsCode;
  FragmentShaderCode += functionsCode;

  GLint Result = GL_FALSE;
  int InfoLogLength=0;

  // Compile Vertex Shader
  char const * VertexSourcePointer = VertexShaderCode.c_str();
  glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
  glCompileShader(VertexShaderID);
  glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
  glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
  if ( InfoLogLength > 1 ){
    std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
    glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
    printf("%s\n", &VertexShaderErrorMessage[0]);
    exit(-1);
  }
  //std::cout << "Succesfully compiled shader : " << vertex_file_path << std::endl;

  // Compile Fragment Shader
  char const * FragmentSourcePointer = FragmentShaderCode.c_str();
  glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
  glCompileShader(FragmentShaderID);
  glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
  glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
  if ( InfoLogLength > 1 ){
    std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
    glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
    printf("%s\n", &FragmentShaderErrorMessage[0]);
    exit(-1);
  }
  //std::cout << "Succesfully compiled shader : " << fragment_file_path << std::endl;

  // Link the program
  GLuint ProgramID = glCreateProgram();
  glAttachShader(ProgramID, VertexShaderID);
  glAttachShader(ProgramID, FragmentShaderID);
  glLinkProgram(ProgramID);
  // Check the program
  glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
  glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
  if ( InfoLogLength > 1 ){
    std::vector<char> ProgramErrorMessage(InfoLogLength+1);
    glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
    printf("%s\n", &ProgramErrorMessage[0]);
    exit(-1);
  }
  std::cout << "Succesfully linked " << vertex_file_path << std::endl;

  glDetachShader(ProgramID, VertexShaderID);
  glDetachShader(ProgramID, FragmentShaderID);

  glDeleteShader(VertexShaderID);
  glDeleteShader(FragmentShaderID);

  mProgramID = ProgramID;
}


void CglShader::load(string TYPE){
  std::string P = ROOT_PATH + "shaders/";
  if(TYPE=="SIMPLE"){
    load(P + "shader.vert", P + "shader.frag", P + "functions.shader");
  }
  else if(TYPE=="SMOOTH"){
    load(P + "smooth_shader.vert", P + "smooth_shader.frag", P + "functions.shader");
  }
  else if(TYPE=="FLAT"){
    load(P + "flat_shader.vert", P + "flat_shader.frag", P + "functions.shader");
  }
  else if(TYPE=="FRESNEL"){
    load(P + "fresnel_shader.vert", P + "fresnel_shader.frag", P + "functions.shader");
  }
}
