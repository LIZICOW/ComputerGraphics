#version 330 core

out vec4 FragColor;
uniform vec3 deepWaterColor;
uniform vec3 shallowWaterColor;
uniform vec3 viewPos;
uniform vec3 lightDir;

in vec3 FragPos;
in vec3 normal;
uniform samplerCube skybox;
uniform sampler2D tesst;
uniform float maxDistance;

float dis(vec3 x, vec3 y){
    return sqrt((x.x - y.x) * (x.x - y.x) + (x.y - y.y) * (x.y - y.y) + (x.z - y.z) * (x.z - y.z));
}

void main(){
    vec3 n = normalize(normal);
    vec3 eyeVec = normalize(viewPos - vec3(FragPos));
    vec3 halfwayDir = normalize(lightDir + eyeVec);
    vec3 reflectVec = normalize(2 * dot(eyeVec, n) * n - eyeVec);

    // Incident angle, reflection angle and transmission(refraction) angle
    float thetaI = acos(dot(eyeVec, n));
    float thetaR = acos(dot(reflectVec, n));
    float thetaT = asin(0.75 * sin(thetaI));

    // The reflectivity factor, 1-reflectivity is the refraction factor
    float reflectivity;
    if (abs(thetaI) >= 0.000001) {
        float t1 = sin(thetaT - thetaI), t2 = sin(thetaT + thetaI);
        float t3 = tan(thetaT - thetaI), t4 = tan(thetaT + thetaI);
        reflectivity = 0.5 * (t1*t1/(t2*t2) + t3*t3/(t4*t4));
        if (reflectivity > 1.0) reflectivity = 1.0;
    } else {
        reflectivity = 0;
    }

    // Reflection color component
    vec4 r = texture(tesst, vec2(reflectVec));

    // Transmission color component
    vec4 t = vec4(deepWaterColor, 1.0);

    // Calculate Fresnel Reflection and Refraction
    vec3 result = vec3(reflectivity * r + (1 - reflectivity) * t);

    float fogFactor = (maxDistance - dis(viewPos, vec3(FragPos))/2) / (maxDistance - 0);
    vec3 fogColor = vec3(0.94f, 0.70f, 0.16f); // Set fog color (adjust as needed)
    vec3 finalColor = mix(fogColor, result, fogFactor);

    FragColor = vec4(finalColor, 1.0f);

}