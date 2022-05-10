#version 460 core

out vec4 FragColor;

in vec3 voPos;
in vec3 voClipPos;
in vec3 voNorm;
in vec2 voUV;

uniform vec3 light_dir;

uniform float uDiffuseAngle; // auto [1, 1, 90] 90
uniform float uSpecularAngle; // auto [1, 1, 90] 5
uniform float uShadowAngle; // auto [1, 1, 90] 2

uniform float uDiffuseStepScale; // auto [0.1, 0.1, 1] 0.5
uniform float uSpecularStepScale; // auto [0.1, 0.1, 1]
uniform float uShadowStepScale; // auto [0.1, 0.1, 1] 0.4
uniform float uStartDist; // export [0.001, 0.001, 0.01] 0.01

uniform bool uDiffuse; // auto 1
uniform bool uSpecular; // auto 1
uniform bool uDiffuseBounce; // auto 1
uniform bool uSpecBounce; // auto 0
uniform bool uShadowTrace; // auto 1
uniform bool uHDDiffuseBounce; // auto 1
uniform bool uMultiplyShadow; // export 1
uniform bool uGammaCorrect; // export 1
uniform bool uReinhard; // export 1
uniform float uExposure; // export [0.0, 0.1, 3.0] 1

uniform float uAmbient; // export 0.01
uniform float uSpecularPower; // export [1, 1, 32] 32

uniform float uDiffuseBounceStrength; // export [0.1, 0.1, 5.0] 1.0

uniform vec4 uLightColor; // export

uniform vec3 uCamPos;

layout(location = 0) uniform sampler2D uTexture1;
layout(location = 1) uniform sampler3D uVoxelMap;

float voxel_size = textureSize(uVoxelMap, 0).x;

vec4 sample_scene(vec3 pos, float radius);
vec3 cone_trace(vec3 pos, vec3 dir, float deg, float step_scale);
float shadow_trace(vec3 pos, vec3 dir, float deg);
vec3 sample_diffuse_bounce(vec3 pos, vec3 norm);

void main() {
    vec3 l = normalize(light_dir);

    vec3 color = vec3(0.0);
    vec3 view_dir = normalize(voPos - uCamPos);
    vec3 n = normalize(voNorm);
    vec3 recflected = normalize(reflect(view_dir, n));

    float kD = max(dot(n, l), 0.0);
    float kS = max(0, dot(recflected, light_dir));

    vec4 albedo = texture(uTexture1, voUV);

    if (albedo.a < 0.51) discard;

 
    if(uDiffuse)
        color += albedo.rgb * kD;

    if (uSpecular)
        color += uLightColor.rgb * pow(kS, uSpecularPower);

    if(uShadowTrace)
    {
        vec3 shadow_start_pos = voClipPos + n * uStartDist;

        if (uMultiplyShadow)
            color = max(vec3(0.0),  color * vec3(shadow_trace(shadow_start_pos, l, uShadowAngle)) );
        else
            color = max(vec3(0.0),  color - vec3(1.0 - shadow_trace(shadow_start_pos, l, uShadowAngle)) );
    }

    if(uDiffuseBounce)
        color += sample_diffuse_bounce(voClipPos, n) * uDiffuseBounceStrength;// * 
    
    if(uSpecBounce)
        color += cone_trace(voClipPos, normalize(reflect(view_dir, n)), uSpecularAngle, uSpecularStepScale);
    
    
    color += uAmbient * albedo.rgb;

    //color /= 2.0;
    
    if (uReinhard)
        color.rgb = color / (color + vec3(1.0)); 
    else if(uExposure >= 0.1)
        color.rgb = vec3(1.0) - exp(-color * uExposure);

    if (uGammaCorrect)
        color.rgb = pow(color.rgb, vec3(1.0 / 2.2));


    FragColor.rgb = color.rgb;
    FragColor.a = albedo.a;
}

float shadow_trace(vec3 pos, vec3 dir, float deg)
{
    float a = deg * 3.14159 / 180.;
    float d = uStartDist;
    float occ = 0.0;
    dir = normalize(dir);

    while(d < 1.0 && occ < 1.0)
    {
        float diam = d * 2.0 * tan(a/2.0);
        vec3 cone_pos = d * dir + pos;

        vec4 sample_color = sample_scene(cone_pos, diam);
        float density = sqrt(d) * sample_color.a * 2.0; // empty voxels have alpha of zero

        //occ += (1.0-occ)*density;
        occ += density;

        d += diam * uShadowStepScale;
    }
    
    return clamp(1.0 - occ, 0.0, 1.0);
}

vec3 cone_trace(vec3 pos, vec3 dir, float deg, float step_scale)
{
    float a = deg * 3.14159 / 180.;
    float d = uStartDist;
    vec3 color = vec3(0.0);
    float occ = 0.0;
    dir = normalize(dir);

    while(d < 1.0 && occ < 1.0)
    {
        float diam = d * 2.0 * tan(a/2.0);
        vec3 cone_pos = d * dir + pos;

        vec4 sample_color = sample_scene(cone_pos, diam * 0.5);
        color += sample_color.rgb;
        occ += sample_color.a; // empty voxels have alpha of zero

        d += diam * step_scale;
    }
    
    return pow(color, vec3(2.2));
}

vec3 sample_diffuse_bounce(vec3 pos, vec3 norm)
{
    vec3 tangent = normalize(cross(norm, vec3(1,0,0)));
    if (any(isnan(tangent))) tangent = normalize(cross(norm, vec3(0,0,1)));
    vec3 bitangent = normalize(cross(norm, tangent));
    vec3 bt = normalize(tangent + bitangent);
    vec3 tn = normalize(tangent + norm);

    pos += norm * 0.005;
    vec3 color = vec3(0);

    float diffuse_angle = uDiffuseAngle;

    color += cone_trace(pos, norm, diffuse_angle, uDiffuseStepScale);
    color += cone_trace(pos, tangent, diffuse_angle, uDiffuseStepScale);
    color += cone_trace(pos, -tangent, diffuse_angle, uDiffuseStepScale);
    color += cone_trace(pos, bitangent, diffuse_angle, uDiffuseStepScale);
    color += cone_trace(pos, -bitangent, diffuse_angle, uDiffuseStepScale);
    
    if (!uHDDiffuseBounce)
        return color / 5.0; // normalize result
    
    color += cone_trace(pos, -bt, diffuse_angle, uDiffuseStepScale);
    color += cone_trace(pos, bt, diffuse_angle, uDiffuseStepScale);
    color += cone_trace(pos, tn, diffuse_angle, uDiffuseStepScale);
    color += cone_trace(pos, -tn, diffuse_angle, uDiffuseStepScale);

    return color / 9.0; // normalize result

}

// as described in 
// https://research.nvidia.com/sites/default/files/pubs/2011-09_Interactive-Indirect-Illumination/GIVoxels-pg2011-authors.pdf
vec4 sample_scene(vec3 pos, float radius)
{
    float mip_level = min(log2(radius * voxel_size), 7.0);

    pos = pos * 0.5 + 0.5;
    return textureLod(uVoxelMap, pos, int(mip_level));
}

