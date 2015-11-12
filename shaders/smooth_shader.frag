#version 120

varying vec3 vertex_pos;
varying vec3 vertex_nor;
varying vec3 vertex_col;

uniform mat4 MVP;
uniform vec3 COL;
uniform mat4 FILL;
uniform mat4 SIDE;
uniform mat4 BACK;
uniform mat4 M;
uniform mat4 V;
uniform vec3 FOG_COL;
uniform float FOG;

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

float snoise(vec3 v);
float fbm(vec3 val, float initFreq, float gain, int octaves, float lacunarity);


void main(){
  vec3 Position_worldspace = (M * vec4(vertex_pos,1)).xyz;
  vec3 color 	= COL;
  //color 	= vertex_col;
  vec3 col = smoothLight(FILL, color) + smoothLight(SIDE, color) + smoothLight(BACK, color);
  //gl_FragColor = vec4(col, 0.3);

  //Second try for noise
  //float N       = fbm(vertex_pos, 0.07f, 0.75, 8, 1.9f);//snoise(10 * vertex_pos);
  //float F       = 0.8*(N-0.5) + 0.5;
  //vec3 finalCol = fog( F*col + (1-F)*0.9*col );
  vec3 finalCol = fog(col);
  gl_FragColor  = vec4(finalCol, 0.0);// * vec4(col,0.3);//vec4(noise3D, 0.3);
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




float fbm(vec3 val, float initFreq, float gain, int octaves, float lacunarity){
    float total = 0.0f;
    float frequency = 1.0f/initFreq;
    float amplitude = gain;
    for (int i = 0; i < octaves; i++){
        total += snoise(val * frequency) * amplitude;
        frequency *= lacunarity;
        amplitude *= gain;
    }
    return total;
}




//
// Description : Array and textureless GLSL 2D/3D/4D simplex
//               noise functions.
//      Author : Ian McEwan, Ashima Arts.
//  Maintainer : ijm
//     Lastmod : 20110822 (ijm)
//     License : Copyright (C) 2011 Ashima Arts. All rights reserved.
//               Distributed under the MIT License. See LICENSE file.
//               https://github.com/ashima/webgl-noise
//

vec3 mod289(vec3 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec4 mod289(vec4 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec4 permute(vec4 x) {
     return mod289(((x*34.0)+1.0)*x);
}

vec4 taylorInvSqrt(vec4 r)
{
  return 1.79284291400159 - 0.85373472095314 * r;
}

float snoise(vec3 v)
  {
  const vec2  C = vec2(1.0/6.0, 1.0/3.0) ;
  const vec4  D = vec4(0.0, 0.5, 1.0, 2.0);

// First corner
  vec3 i  = floor(v + dot(v, C.yyy) );
  vec3 x0 =   v - i + dot(i, C.xxx) ;

// Other corners
  vec3 g = step(x0.yzx, x0.xyz);
  vec3 l = 1.0 - g;
  vec3 i1 = min( g.xyz, l.zxy );
  vec3 i2 = max( g.xyz, l.zxy );

  //   x0 = x0 - 0.0 + 0.0 * C.xxx;
  //   x1 = x0 - i1  + 1.0 * C.xxx;
  //   x2 = x0 - i2  + 2.0 * C.xxx;
  //   x3 = x0 - 1.0 + 3.0 * C.xxx;
  vec3 x1 = x0 - i1 + C.xxx;
  vec3 x2 = x0 - i2 + C.yyy; // 2.0*C.x = 1/3 = C.y
  vec3 x3 = x0 - D.yyy;      // -1.0+3.0*C.x = -0.5 = -D.y

// Permutations
  i = mod289(i);
  vec4 p = permute( permute( permute(
             i.z + vec4(0.0, i1.z, i2.z, 1.0 ))
           + i.y + vec4(0.0, i1.y, i2.y, 1.0 ))
           + i.x + vec4(0.0, i1.x, i2.x, 1.0 ));

// Gradients: 7x7 points over a square, mapped onto an octahedron.
// The ring size 17*17 = 289 is close to a multiple of 49 (49*6 = 294)
  float n_ = 0.142857142857; // 1.0/7.0
  vec3  ns = n_ * D.wyz - D.xzx;

  vec4 j = p - 49.0 * floor(p * ns.z * ns.z);  //  mod(p,7*7)

  vec4 x_ = floor(j * ns.z);
  vec4 y_ = floor(j - 7.0 * x_ );    // mod(j,N)

  vec4 x = x_ *ns.x + ns.yyyy;
  vec4 y = y_ *ns.x + ns.yyyy;
  vec4 h = 1.0 - abs(x) - abs(y);

  vec4 b0 = vec4( x.xy, y.xy );
  vec4 b1 = vec4( x.zw, y.zw );

  //vec4 s0 = vec4(lessThan(b0,0.0))*2.0 - 1.0;
  //vec4 s1 = vec4(lessThan(b1,0.0))*2.0 - 1.0;
  vec4 s0 = floor(b0)*2.0 + 1.0;
  vec4 s1 = floor(b1)*2.0 + 1.0;
  vec4 sh = -step(h, vec4(0.0));

  vec4 a0 = b0.xzyw + s0.xzyw*sh.xxyy ;
  vec4 a1 = b1.xzyw + s1.xzyw*sh.zzww ;

  vec3 p0 = vec3(a0.xy,h.x);
  vec3 p1 = vec3(a0.zw,h.y);
  vec3 p2 = vec3(a1.xy,h.z);
  vec3 p3 = vec3(a1.zw,h.w);

//Normalise gradients
  vec4 norm = taylorInvSqrt(vec4(dot(p0,p0), dot(p1,p1), dot(p2, p2), dot(p3,p3)));
  p0 *= norm.x;
  p1 *= norm.y;
  p2 *= norm.z;
  p3 *= norm.w;

// Mix final noise value
  vec4 m = max(0.6 - vec4(dot(x0,x0), dot(x1,x1), dot(x2,x2), dot(x3,x3)), 0.0);
  m = m * m;
  return 42.0 * dot( m*m, vec4( dot(p0,x0), dot(p1,x1),
                                dot(p2,x2), dot(p3,x3) ) );
  }









