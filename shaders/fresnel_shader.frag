#version 120
#extension GL_EXT_gpu_shader4 : enable

varying vec3 vertex_pos;
varying vec3 vertex_nor;

uniform mat4 MVP;
uniform vec3 COL;
uniform mat4 FILL;
uniform mat4 SIDE;
uniform mat4 BACK;
uniform mat4 M;
uniform mat4 V;
uniform vec3 FOG_COL;
uniform float FOG;
uniform float GRID;

vec3  smoothLight(mat4 light_matrix, vec3 mater_color);
float fog(float begin, float end);
vec3  fog(vec3 col){
    if(FOG==1.0f){
        vec3 newCol;
        float f        = fog(2.5, 4.0);
        newCol         = (1-f)*FOG_COL + f*col;
        return newCol;
    }
    else
        return col;
}


void main(){
  vec3 Position_worldspace = (M * vec4(vertex_pos,1)).xyz;
  
  vec3 GRIDCOL;
  float alpha;
  if((int(10*vertex_pos.x + 10) + int(10*vertex_pos.y + 10) + int(10*vertex_pos.z + 10)) %2 < 1){GRIDCOL = COL;alpha=0.35;}
  else{GRIDCOL = vec3(1,1,1);alpha=0.35;}
  if(GRID==0.0f)
    alpha = 1;
  vec3 col = smoothLight(FILL, GRIDCOL) + smoothLight(SIDE, GRIDCOL) + smoothLight(BACK, GRIDCOL);
  vec3 finalCol = fog(col);
  //gl_FragColor  = vec4(finalCol, 0.0);
  
  gl_FragColor = vec4(col,alpha);
  
  /*
  //vec3 pos = FILL[0].xyz;
  float distance = length( (MVP * vec4(vertex_pos,1)).xyz);
  vec3 vertex_position_cameraspace = ( V * M * vec4(vertex_pos,1)).xyz;
  vec3 eye_direction_cameraspace = vec3(0,0,0) - vertex_position_cameraspace;
  vec3 light_position_cameraspace = ( V * vec4(0,0,0,1)).xyz;
  vec3 light_direction_cameraspace = eye_direction_cameraspace;
  vec3 normal_cameraspace = ( V * M * vec4(vertex_nor,0)).xyz;

  vec3 n = normalize( normal_cameraspace );
  vec3 l = normalize( light_direction_cameraspace );
  float cosTheta = clamp( dot( n,l ), 0.,1. );
  
  //gl_FragColor  = vec4(cosTheta, cosTheta, cosTheta, 0.0);
  //gl_FragColor  = vec4(vec3(clamp(dot(l, vertex_nor), 0., 1.)), 0.0);
  float fresnel = pow(1.0 - cosTheta, 2.0);
  //gl_FragColor = vec4(fresnel * col, 0);
  */
  
  
  
}


float fog(float begin, float end){
    float d   = length(( V * M * vec4(vertex_pos,1)).xyz);
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

vec3 smoothLight(mat4 light_matrix, vec3 mater_color){
  vec3  light_posit     = light_matrix[0].xyz;
  vec3  light_color     = light_matrix[1].xyz;
  vec3  mix_ratio       = light_matrix[2].xyz;
  float light_power     = light_matrix[3].x;
  float lobe_size       = light_matrix[3].y;
  bool  camera_anchored = bool(light_matrix[3].z);

  float distance = length(light_posit - (MVP * vec4(vertex_pos,1)).xyz);
  vec3 vertex_position_cameraspace = ( V * M * vec4(vertex_pos,1)).xyz;
  vec3 eye_direction_cameraspace = vec3(0,0,0) - vertex_position_cameraspace;
  vec3 light_position_cameraspace = ( V * vec4(light_posit,1)).xyz;

  //WORLD OR CAMERA?
  vec3 light_direction_cameraspace;
  if(camera_anchored)
    light_direction_cameraspace = light_posit + eye_direction_cameraspace;
  else
    light_direction_cameraspace = light_position_cameraspace + eye_direction_cameraspace;

  vec3 normal_cameraspace = ( V * M * vec4(vertex_nor,0)).xyz;
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





