#version 460 core
out vec4 FragColor;

//in vec2 voUV;
//in vec3 voNorm;
in vec3 voPos;
//in vec3 voViewPos;
//in vec3 voViewNorm;
//in mat4 voView;

uniform vec4 uColor1; // auto

void main()
{
	vec3 color =  uColor1.rgb;
	FragColor.rgb = color;
	FragColor.a = 1.0f;
}