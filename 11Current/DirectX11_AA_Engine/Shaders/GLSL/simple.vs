#version 450 core

in vec3 pos;
in vec2 uv;
in vec3 normals;

uniform mat4 modelView;
uniform mat4 projection;

out vec2 UV;
out vec3 normals0;

void main()
{
	gl_Position = projection * modelView * vec4(pos, 1.0);
	UV = uv;
	normals0 = (modelView * vec4(normals, 0.0)).xyz;
}