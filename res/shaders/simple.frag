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
in layout(location = 3) vec3 tangent;
in layout(location = 4) vec3 bitangent;

layout(binding = 1) uniform sampler2D diffuseSampler;
layout(binding = 2) uniform sampler2D normalSampler;

uniform layout(location = 6) vec3 ballPosition;
uniform layout(location = 7) bool useTexture;

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
  color = vec4(ambientColor,1);
  vec3 N = normalize(normal);

  // apply the TBN matrix, along with normals of the normal map (if possible)
  if(useTexture){
    N = normalize(2.0 * texture(normalSampler, textureCoordinates).xyz - 1.0);
    N = normalize(mat3(tangent, bitangent, normal) * N);
  }

  vec3 V = normalize(-pos);

  for (int i = 0; i < MAX_LIGHT_SOURCES; ++i) 
  {
    vec3 L = lightSources[i].pos - pos;
    vec3 B = ballPosition - pos;

    float shadowFactor = 1.0f;
    if(dot(L,L) > dot(B,B) && dot(L,B) >= 0)
    {
      float l = length(reject(B, L));
      //hard shadows
      //shadowFactor = l > ballRadius ? 1 : 0;
      //soft shadows (parabolic interpolation at the edge of the shadow)
      shadowFactor = clamp(-0.5 + 0.5 * ((l*l)/(ballRadius*ballRadius)), 0, 1);
    }

    L = normalize(L);

    // calculation of diffuse and specular factors
    float diff = max(dot(N, L), 0.0);
    float specular = 0.0;
    if(diff > 0.0) {
      vec3 R = reflect(-L, N);
      specular = pow(max(dot(R, V), 0.0), shininess);
    }

    // calculation of attenuation
    float dist = length(lightSources[i].pos - pos);
    float att = 1 / dot(attenuation, vec3(1, dist, dist*dist));

    // final per light addition of shadow, attenuation, diffuse and specular components
    color += shadowFactor * att * vec4(ambientColor + diff * lightSources[i].color + specular * vec3(1), 0);
  } 

  // apply dithering 
  color += vec4(vec3(dither(textureCoordinates)), 1.0);

  // apply diffuse texture
  if (useTexture){
    color *= texture(diffuseSampler, textureCoordinates);
  }
}