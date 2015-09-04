#version 120

attribute vec3 vertex_position;
attribute vec3 vertex_normal;
varying vec3 fragmentColor;

uniform mat4 MVP;
uniform vec3 COL;
uniform vec3 LIGHTPOS;

uniform mat4 M;//MODEL
uniform mat4 V;//VIEW


vec3 light(vec3 light_color, vec3 mater_color, vec3 mix_ratio, vec3 light_posit, bool camera_space, float light_power, float lobe_size);

void main(){
  gl_Position = MVP * vec4(vertex_position, 1);
  vec3 Position_worldspace = (M * vec4(vertex_position,1)).xyz;

  bool camera_anchored = true;
  bool world_anchored  = false;

  vec3 fill_light = light(vec3(1,1,0.7),
                          COL,
                          vec3(0.2, 0.75, 0.05),
                          vec3(4,2,10),
                          camera_anchored,
                          100.0,
                          5.0);
  vec3 side_light = light(vec3(1,0.9,0.9),
                          COL,
                          vec3(0.2, 0.75, 0.05),
                          vec3(-10,2,7),
                          camera_anchored,
                          50.0,
                          5.0);
  vec3 back_light = light(vec3(0.9,0.9,1),
                          COL,
                          vec3(0.2, 0.65, 0.15),
                          vec3(1,1,-10),
                          camera_anchored,
                          100.0,
                          5.0);

  vec3 mix = 0.9 * vec3(1, 1, 2);
  fragmentColor = mix.x * fill_light +
                  mix.y * side_light +
                  mix.z * back_light
                  ;
}



vec3 light(vec3 light_color, vec3 mater_color, vec3 mix_ratio, vec3 light_posit, bool camera_anchored, float light_power, float lobe_size){

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





