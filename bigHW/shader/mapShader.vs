#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float deltaTime;

out vec2 TexCoords;

void main()
{
    vec3 pos = vec3(aPos.x - deltaTime, aPos.y, aPos.z);
    gl_Position = projection * view * model * vec4(pos, 1.0f);
    TexCoords = vec2(aTexCoords.x, aTexCoords.y);
}