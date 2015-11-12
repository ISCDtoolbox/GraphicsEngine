#version 120
#extension GL_EXT_gpu_shader4 : enable

attribute vec3 vertex_position;
attribute vec3 vertex_normal;
//attribute vec3 vertex_color;

varying vec3 vertex_pos;
varying vec3 vertex_nor;
//varying vec3 vertex_col;

uniform mat4 MVP;
uniform mat4 M;

void main(){
  vertex_pos  = vertex_position;
  vertex_nor  = vertex_normal;
  //vertex_col = vertex_color; //Se baser la dessus pour le calcul de COL
  gl_Position = MVP * vec4(vertex_pos, 1);
}
