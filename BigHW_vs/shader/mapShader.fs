#version 330 core
out vec4 FragColor;
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform sampler2D Texture;

uniform float Distance;
uniform float maxDistance;
uniform vec3 viewPos;


void main()
{
   vec3 result = vec3(texture(Texture, TexCoords));
   vec3 norm = normalize(Normal);
   vec3 viewDir = normalize(viewPos - FragPos);

   float fogFactor = (maxDistance - Distance/2) / (maxDistance - 0);
   vec3 fogColor = vec3(0.94f, 0.70f, 0.16f); // Set fog color (adjust as needed)
   vec3 finalColor = mix(fogColor, result, fogFactor);


   FragColor = vec4(finalColor, 1.0f);
}