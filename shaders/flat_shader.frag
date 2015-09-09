#version 300 es

flat in lowp vec3 fragmentColor;
out lowp vec4 color;

void main(){
  color = vec4(fragmentColor, 0.3);
}

