#version 450 core

in vec2 UV;
in vec3 normals0;

out vec3 color;

uniform sampler2D texture0;

vec3 lightDirection = vec3(0.0, -1.0, 1.0);
vec3 lightColor = vec3(1.0, 1.0, 0.5);
void main()
{
	gl_FragColor = texture2D(texture0, UV)
	 * clamp(dot(-lightDirection, normals0), 0.0, 1.0);
}