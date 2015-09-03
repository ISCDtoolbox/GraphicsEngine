#ifndef SHADERHEADERDEF
#define SHADERHEADERDEF

#include "defs.h"

class SHADER
{
private:
  std::string mVertex_file_path;
  std::string mFragment_file_path;

public:
  GLuint      mProgramID;
  SHADER();
  void load(std::string vert_path, std::string frag_path);
  bool ReadProgram(string& s, const string& name);
  bool CompileShader(GLuint hso, string& src);
};

#endif
