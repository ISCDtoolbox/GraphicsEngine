#version 120

attribute vec3 vertex_position;
varying vec3   vertex_pos;

uniform mat4 MVP;

void main(){
  vec4 pos = MVP * vec4(vertex_position, 1);
  gl_Position = pos;
  vertex_pos = vertex_position;
}



