#version 460 core

out vec4 FragColor;

in vec3 goPos;
in vec3 goNorm;
in vec2 goUV;

uniform vec3 light_dir = normalize(vec3(10,20,30));

//uniform vec3 uCamPos;

layout(location = 0) uniform sampler2D uTexture1;
layout(RGBA8, location = 1) uniform image3D uVoxelMap;

void main() {
    float ambient = 0.1;
    vec3 color = vec3(ambient);

    vec3 l = normalize(light_dir);

    vec3 n = normalize(goNorm);
    float kD = max(dot(n, l ), 0.0);
    color += vec3(kD);
    color *= texture(uTexture1, goUV).rgb;

    ivec3 textureSize = imageSize(uVoxelMap);
    
    // [-1, 1] -> [-.5, 0.5] -> [0, 1]
    vec3 pos = goPos * vec3(0.5) + vec3(0.5);
    ivec3 iPos = ivec3(pos * textureSize);

    //color = vec3(1,0,1);
    imageStore(uVoxelMap, iPos, vec4(color, 1.0));
}