#include "glic/shader.h"
#include <fstream>

using namespace std;

SHADER::SHADER(){
  mVertex_file_path   = "";
  mFragment_file_path = "";
  mProgramID          = 0;
}

// Reads the shader source code from the file into a string.
bool SHADER::ReadProgram(string& s, const string& name)
{
  ifstream ist(name.c_str());
  if (!ist.good())
    return false;
  s.clear();
  while(ist.good()) {
    string line;
    getline(ist, line);
    s += line;
    s += "\n";
  }
  return true;
}

// Compiles the shader, returning false on errors.
bool SHADER::CompileShader(GLuint hso, string& src)
{
  const char* str = src.c_str();
  GLint len = (int)src.length();
  glShaderSource(hso, 1, &str, &len);
  glCompileShader(hso);
  GLint compiled;
  glGetShaderiv(hso, GL_COMPILE_STATUS, &compiled);
  if (!compiled) {
    cerr << " could not compile shader:" << endl;
    char log[2048];
    glGetShaderInfoLog(hso, 2047, NULL, log);
    cerr << log << endl;
    return false;
  }
  src.clear();
  //cout << src << endl;
  return true;
}



void SHADER::load(string vert_path, string frag_path){
  // Crée les shaders
  mVertex_file_path   = vert_path;
  mFragment_file_path = frag_path;
  GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
  GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
  GLint Result = GL_FALSE;
  int InfoLogLength;

  // Lit le code du vertex shader à partir du fichier
  std::string VertexShaderCode;
  std::ifstream VertexShaderStream(mVertex_file_path.c_str(), std::ios::in);
  if(VertexShaderStream.is_open()){
    std::string Line = "";
    while(getline(VertexShaderStream, Line))
      VertexShaderCode += "\n" + Line;
    VertexShaderStream.close();
  }
  else{exit(144);}

  // Lit le code du fragment shader à partir du fichier
  std::string FragmentShaderCode;
  std::ifstream FragmentShaderStream(mFragment_file_path.c_str(), std::ios::in);
  if(FragmentShaderStream.is_open()){
    std::string Line = "";
    while(getline(FragmentShaderStream, Line))
      FragmentShaderCode += "\n" + Line;
    FragmentShaderStream.close();
  }
  else{exit(145);}

  // Compile le vertex shader
  printf("Compiling shader : %s\n", mVertex_file_path.c_str());
  char const * VertexSourcePointer = VertexShaderCode.c_str();
  glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
  glCompileShader(VertexShaderID);
  // Vérifie
  glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
  glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
  std::vector<char> VertexShaderErrorMessage(InfoLogLength);
  glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
  fprintf(stdout, "%s\n", &VertexShaderErrorMessage[0]);

  // Compile le fragment shader
  printf("Compiling shader : %s\n", mFragment_file_path.c_str());
  char const * FragmentSourcePointer = FragmentShaderCode.c_str();
  glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
  glCompileShader(FragmentShaderID);
  // Vérifie
  glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
  glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
  std::vector<char> FragmentShaderErrorMessage(InfoLogLength);
  glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
  fprintf(stdout, "%s\n", &FragmentShaderErrorMessage[0]);

  // Lit le programme
  fprintf(stdout, "Linking program\n");
  GLuint ProgramID = glCreateProgram();
  glAttachShader(ProgramID, VertexShaderID);
  glAttachShader(ProgramID, FragmentShaderID);
  glLinkProgram(ProgramID);

  // Vérifie le programme
  glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
  glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
  std::vector<char> ProgramErrorMessage( std::max(InfoLogLength, int(1)) );
  glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
  fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);

  glDeleteShader(VertexShaderID);
  glDeleteShader(FragmentShaderID);
  mProgramID = ProgramID;
}
