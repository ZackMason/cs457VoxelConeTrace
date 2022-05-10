#version 460 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec3 voPos[];
in vec3 voNorm[];
in vec2 voUV[];

out vec3 goPos;
out vec3 goNorm;
out vec2 goUV;

void main() 
{
	vec3 n = abs(cross(voPos[1] - voPos[0], voPos[2] - voPos[0]));	
	float axis = max(max(n.x, n.y), n.z);
	

    if (false)
    {
        for(uint i=0; i<3; i++) 
        {
            goPos = voPos[i];
            goNorm = voNorm[i];
            goUV = voUV[i];
            if (n.z > n.x && n.z > n.y)
                gl_Position = vec4(goPos.x, goPos.y, 0, 1);
            else if (n.x > n.y && n.x > n.z)
                gl_Position = vec4(goPos.y, goPos.z, 0, 1);
            else
                gl_Position = vec4(goPos.x, goPos.z, 0, 1);

            //goPos = gl_Position.xyz;
            EmitVertex();
        }

    } else {
        for(uint i=0; i<3; i++) 
        {
            goPos = voPos[i];
            goNorm = voNorm[i];
            goUV = voUV[i];

            if (axis == n.x) gl_Position = vec4(goPos.zyx, 1);
            else if (axis == n.y) gl_Position = vec4(goPos.xzy, 1);
            else if (axis == n.z) gl_Position = vec4(goPos.xyz, 1);
            
            EmitVertex();
        }
    }

	EndPrimitive();
}