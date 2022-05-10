#version 460 core

layout ( location = 0 ) in vec3 aPos;
layout ( location = 1 ) in vec3 aNormal;
layout ( location = 2 ) in vec2 aUV;

out vec2 voUV;
out vec3 voNorm;
out vec3 voPos;
out vec3 voClipPos;

uniform mat4 uVP;
uniform mat4 uM;
uniform mat4 uV;
uniform mat4 uSceneScale;

void main()
{
    voUV = aUV;
 
    vec3 modelPos = (uM * vec4(aPos,1.)).xyz;
    voPos = modelPos;

    vec4 mvp_pos = (uVP*vec4(modelPos,1));

    voNorm = mat3(transpose(inverse(uM))) * aNormal;
    voNorm = normalize(voNorm);

    voClipPos = (uSceneScale * vec4(aPos, 1.0)).xyz;
    gl_Position = mvp_pos;
}