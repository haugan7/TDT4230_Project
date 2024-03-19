#version 430 core

in layout(location = 0) vec3 position;
in layout(location = 1) vec3 normal_in;
in layout(location = 2) vec2 textureCoordinates_in;
in layout(location = 3) vec3 tangent_in;
in layout(location = 4) vec3 bitangent_in;

uniform layout(location = 3) mat4 modelview;
uniform layout(location = 4) mat4 projection;
uniform layout(location = 5) mat3 normal;

out layout(location = 0) vec3 normal_out;
out layout(location = 1) vec3 vertex_out;
out layout(location = 2) vec2 textureCoordinates_out;
out layout(location = 3) vec3 tangent_out;
out layout(location = 4) vec3 bitangent_out;

void main()
{
    normal_out = normalize(normal * normal_in);
    vec4 v4 = modelview * vec4(position, 1.0);
    vertex_out = vec3(v4) / v4.w;
    gl_Position = projection * v4;
    textureCoordinates_out = textureCoordinates_in;
    tangent_out = tangent_in;
    bitangent_out = bitangent_in;
}