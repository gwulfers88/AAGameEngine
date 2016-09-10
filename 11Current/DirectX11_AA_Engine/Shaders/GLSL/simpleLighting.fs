#version 450 core

in VS_OUT
{
	vec2 UV;
	vec3 normals0;
	vec3 lightDirection;
	vec3 cameraDirection;
	vec4 ambient;
	float dist;
}fs_in;

uniform sampler2D texture0;

out vec3 color;

void main()
{
	// Attenuation
	float attC = 100;
	float attLinear = 0.005;
	float attQuadratic = 0.005;
	
	float att0 = 1 / (attC) + (attLinear * fs_in.dist) + (attQuadratic * fs_in.dist * fs_in.dist);

	// Diffuse
	vec4 lightColor = vec4(1.0, 1.0, 1.0, 1.0);
	float brightness = clamp(dot(fs_in.lightDirection, fs_in.normals0), 0.0, 1.0);
	vec4 diffuseLight = vec4(brightness, brightness, brightness, 0.0);

	// Specular
	vec3 reflectedLight = reflect(fs_in.lightDirection, fs_in.normals0);
	float spec = clamp(dot(reflectedLight, fs_in.cameraDirection), 0.0, 1.0);
	spec = pow(spec, 128.0);
	vec4 specularLight = vec4(spec, 0.0, 0.0, 1.0);

	//texture2D(texture0, fs_in.UV) * 
	gl_FragColor = (lightColor)
	 * (diffuseLight + fs_in.ambient + specularLight);
}