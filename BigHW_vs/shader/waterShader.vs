#version 330 core

struct Wave{
    float s;
    float w;
    float l;
    vec2 D;
};

layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform int waveCnt;
uniform float time;
out vec3 FragPos;
out vec3 normal;

uniform Wave wave[20];
void main()
{
    vec3 pos = aPos;
    vec2 u = vec2(aPos.x, aPos.z);
    for(int i = 0;i < waveCnt; i++){
        float k = 2 * 3.14 / wave[i].l;
        float A = wave[i].s / k;
        float value = k * dot(wave[i].D, u) - wave[i].w * time;
        pos.x += wave[i].D.x * A * cos(value);
        pos.y += A * sin(value);
        pos.z += wave[i].D.y * A * cos(value);
    }

    for(int i = 0;i < waveCnt; i++){
        float k = 2 * 3.14 / wave[i].l;
        float A = wave[i].s / k;        
        float value = k * dot(wave[i].D, u) - wave[i].w * time;
        normal.x = 1 - wave[i].D.x * wave[i].D.x * A * k * sin(value);
        normal.y = wave[i].D.x * A * k * cos(value);
        normal.z = -wave[i].D.x * wave[i].D.y * A * k * sin(value);
    }

    normal = mat3(transpose(inverse(model))) * normal;
    FragPos = vec3(model * vec4(pos, 1.0f));
    gl_Position = projection * view * model * vec4(pos, 1.0);
}
