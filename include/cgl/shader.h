#ifndef SHADERHEADERDEF
#define SHADERHEADERDEF

#include "defs.h"

class CglShader
{
private:
  std::string vertex_file_path;
  std::string fragment_file_path;

public:
  GLuint      mProgramID;
  CglShader();
  void load(std::string vert_file_path, std::string frag_file_path);
  void load(std::string TYPE);
};

typedef CglShader* pCglShader;

#endif
