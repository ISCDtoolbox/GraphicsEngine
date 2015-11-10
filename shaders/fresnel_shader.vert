#version 120
#extension GL_EXT_gpu_shader4 : enable

attribute vec3 vertex_position;
attribute vec3 vertex_normal;

varying vec3 vertex_pos;
varying vec3 vertex_nor;

uniform mat4 MVP;
uniform mat4 M;

void main(){
  vertex_pos  = vertex_position;
  vertex_nor  = vertex_normal;
  gl_Position = MVP * vec4(vertex_pos, 1);
}
