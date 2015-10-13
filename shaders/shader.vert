#version 120

attribute vec3 vertex_position;
attribute vec3 vertex_normal;
varying vec3 fragmentColor;

uniform mat4 MVP;
uniform vec3 COL;

uniform vec3 FOG_COL;
uniform mat4 V;
uniform mat4 M;
uniform float FOG;

float fog(float begin, float end);

void main(){
  //Position du sommet, dans l'espace de découpe : MVP * position
  gl_Position = MVP * vec4(vertex_position, 1);

  vec3 col = COL;

  if(FOG==1.0f){
    float f        = fog(2.5, 4.0);
    col            = (1-f)*FOG_COL + f*col;
  }

  fragmentColor = col;
}


float fog(float begin, float end){
    float d   = length(( V * M * vec4(vertex_position,1)).xyz);
    float fac;
    //Linear
    //fac = (end - d)/(end - begin);
    //Exponential
    float FogDensity = 1.3;
    if(d>begin)
        fac = 1.0 /exp( ((d-begin) * FogDensity)* ((d-begin) * FogDensity));
    else
        fac = 1;

    fac = clamp( fac, 0.0, 1.0 );
    return fac;
}
