#version 460 core
out vec4 FragColor;

in vec3 voPos;
in vec3 voNorm;
in vec2 voUV;

const float PI = 3.14159;

uniform sampler2D uTexture1;

const vec3 light = (vec3(0,15,10));

void main()
{
    vec3 light_dir = -normalize(voPos - light);
    vec3 color = texture(uTexture1,voUV).rgb;

    vec3 norm = normalize(voNorm);    

    float NdotL = max(dot(voNorm,light_dir),0.);

    FragColor = vec4(vec3(color),1.);
    //FragColor.rgb = norm;
	FragColor.a = texture(uTexture1, voUV).a;
}



