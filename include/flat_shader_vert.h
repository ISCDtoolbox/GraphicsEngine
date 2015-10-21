// header file generated by txt2h, 2003-2006 by ScottManDeath
#ifndef  TXT_HEADER_flat_vert_code
#define  TXT_HEADER_flat_vert_code


const char flat_vert_code [] =
"#version 120\n"
"\n"
"attribute vec3 vertex_position;\n"
"varying vec3   vertex_pos;\n"
"\n"
"uniform mat4 MVP;\n"
"\n"
"void main(){\n"
"  vec4 pos = MVP * vec4(vertex_position, 1);\n"
"  gl_Position = pos;\n"
"  vertex_pos = vertex_position;\n"
"}\n"
"\n"
"\n"
"\n"
;


#endif  // #ifdef TXT_HEADER_flat_vert_code