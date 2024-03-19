#version 430 core

//hardcoded values
int MAX_LIGHT_SOURCES = 3;
vec3 ambientColor = vec3(.15, .15, .15);
float shininess = 64.0f;
vec3 attenuation = vec3(1, .001, .001);
float ballRadius = 3.0f;

in layout(location = 0) vec3 normal;
in layout(location = 1) vec3 pos;
in layout(location = 2) vec2 textureCoordinates;

layout(binding = 0) uniform sampler2D textSampler;

uniform layout(location = 6) vec3 ballPosition;

struct LightSource 
{            
  vec3 color;    
  vec3 pos;  
};

uniform LightSource lightSources[3];

out vec4 color;

float rand(vec2 co) { return fract(sin(dot(co.xy, vec2(12.9898,78.233))) * 43758.5453); }
float dither(vec2 uv) { return (rand(uv)*2.0-1.0) / 256.0; }
vec3 reject(vec3 from, vec3 onto) { return from - onto*dot(from, onto)/dot(onto, onto);}


void main() {
  
  color = texture(textSampler, textureCoordinates);
  //color = vec4(textureCoordinates.xy, 1, 1);
}