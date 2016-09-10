#version 450 core

in vec3 pos;
in vec2 uv;

uniform mat4 modelView;
uniform mat4 projection;

out vec2 UV;

void main()
{
	gl_Position = projection * modelView * vec4(pos, 1.0);
	UV = uv;
}