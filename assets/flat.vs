#version 460 core

layout ( location = 0 ) in vec3 aPos;
layout ( location = 1 ) in vec3 aNormal;
layout ( location = 2 ) in vec2 aUV;

//out vec2 voUV;
//out vec3 voNorm;
out vec3 voPos;
//out vec3 voViewPos;
//out vec3 voViewNorm;

uniform mat4 uVP;
uniform mat4 uM;
uniform mat4 uV;

void main()
{
    vec3 modelPos = (uM * vec4(aPos,1.)).xyz;
    voPos = modelPos;
   // voViewPos = (uV * vec4(modelPos,1.)).xyz;

    vec4 mvp_pos = (uVP*vec4(modelPos,1));

  //  voNorm = mat3(transpose(inverse(uM))) * aNormal;
//    voNorm = normalize(voNorm);

   // voViewNorm = (uV * vec4(voNorm,0.)).xyz;
   // voViewNorm = normalize(voViewNorm); 
    gl_Position = mvp_pos;
    //voUV = aUV;
}