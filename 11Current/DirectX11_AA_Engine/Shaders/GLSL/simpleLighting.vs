#version 450 core

in vec3 pos;
in vec2 uv;
in vec3 normals;

uniform mat4 modelView;
uniform mat4 projection;

uniform vec3 lightPosition;
uniform vec3 cameraPos;
uniform vec3 ambientLight;

out VS_OUT
{
	vec2 UV;
	vec3 normals0;
	vec3 lightDirection;
	vec3 cameraDirection;
	vec3 ambient;
	float dist;
}vs_out;

void main()
{
	gl_Position = projection * modelView * vec4(pos, 1.0);
	vs_out.UV = uv;
	vs_out.dist = distance(lightPosition, pos);
	//lightPosition = (modelView * vec4(lightPosition, 1.0)).xyz;
	vs_out.lightDirection = normalize(lightPosition - pos);
	vs_out.cameraDirection = normalize(cameraPos - pos);
	vs_out.normals0 = (modelView * vec4(normals, 0.0)).xyz;
	vs_out.ambient = vec4(ambientLight, 1.0).xyz;
}