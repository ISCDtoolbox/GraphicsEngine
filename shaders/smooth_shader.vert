#version 120

attribute vec3 vertex_position;
attribute vec3 vertex_normal;
varying vec3 fragmentColor;

uniform mat4 MVP;
uniform vec3 COL;

//Lights
uniform mat4 FILL;
uniform mat4 SIDE;
uniform mat4 BACK;

uniform mat4 M;
uniform mat4 V;




vec3 light(mat4 light_matrix, vec3 mater_color);

void main(){
  gl_Position = MVP * vec4(vertex_position, 1);
  vec3 Position_worldspace = (M * vec4(vertex_position,1)).xyz;

  bool camera_anchored = true;
  bool world_anchored  = false;

  vec3 fill_light = light(FILL, COL);
  vec3 side_light = light(SIDE, COL);
  vec3 back_light = light(BACK, COL);

  vec3 mix = 0.9 * vec3(1, 1, 1);
  fragmentColor = mix.x * fill_light
                  + mix.y * side_light
                  + mix.z * back_light
                  ;
}



vec3 light(mat4 light_matrix, vec3 mater_color){
  vec3  light_posit     = light_matrix[0].xyz;
  vec3  light_color     = light_matrix[1].xyz;
  vec3  mix_ratio       = light_matrix[2].xyz;
  float light_power     = light_matrix[3].x;
  float lobe_size       = light_matrix[3].y;
  bool  camera_anchored = bool(light_matrix[3].z);

  float distance = length(light_posit - gl_Position.xyz);
  vec3 vertex_position_cameraspace = ( V * M * vec4(vertex_position,1)).xyz;
  vec3 eye_direction_cameraspace = vec3(0,0,0) - vertex_position_cameraspace;
  vec3 light_position_cameraspace = ( V * vec4(light_posit,1)).xyz;

  //WORLD OR CAMERA?
  vec3 light_direction_cameraspace;
  if(camera_anchored)
    light_direction_cameraspace = light_posit + eye_direction_cameraspace;
  else
    light_direction_cameraspace = light_position_cameraspace + eye_direction_cameraspace;

  vec3 normal_cameraspace = ( V * M * vec4(vertex_normal,0)).xyz;
  vec3 n = normalize( normal_cameraspace );
  vec3 l = normalize( light_direction_cameraspace );
  float cosTheta = clamp( dot( n,l ), 0.,1. );

  vec3 E = normalize(eye_direction_cameraspace);
  vec3 R = reflect(-l,n);
  float cosAlpha = clamp( dot( E,R ), 0.,1. );

  vec3 ambi_color = 0.1 *  mater_color;
  vec3 diff_color = mater_color;
  vec3 spec_color = 0.5 * (mater_color + vec3(1.0, 1.0, 1.0));
  vec3 color = mix_ratio.x * ambi_color
             + mix_ratio.y * diff_color * light_color * light_power * cosTheta        / (distance*distance)
             + mix_ratio.z * spec_color * light_color * light_power * pow(cosAlpha,lobe_size) / (distance*distance)
             ;

  return color;
}





