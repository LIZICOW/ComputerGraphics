#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords; 

out vec2 TexCoords;
out vec4 ParticleColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 offset;
uniform vec4 color;
uniform float scale;

void main()
{
    float s = scale * 0.001f;
    TexCoords = texCoords;
    ParticleColor = color;
    gl_Position = projection * view * model * vec4((position * s) + offset, 1.0);
}