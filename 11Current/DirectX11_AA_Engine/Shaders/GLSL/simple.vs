#version 450 core

in vec3 pos;

uniform mat4 modelView;
uniform mat4 projection;

out vec4 Color;

void main()
{
	gl_Position = projection * modelView * vec4(pos, 1.0);
	Color = vec4(pos, 1.0) * 2;
}