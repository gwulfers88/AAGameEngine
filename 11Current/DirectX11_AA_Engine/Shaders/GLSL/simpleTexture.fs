#version 450 core

in vec2 UV;

uniform sampler2D texture0;

void main()
{
	gl_FragColor = texture2D(texture0, UV);
}