#version 330

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 norm;
layout (location = 2) in vec2 tex;
layout (location = 3) in vec3 color;

out vec3 Color;
out vec2 texCoord;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
uniform float time;

uniform float amplitude;
uniform vec2 waveDirection;
uniform float waveLength;
uniform float waveSpeed;
uniform float steepness;

void main()
{
	float PI = 3.14159265359;
	float PI2 = 6.28318530718;

	mat4 modelMatrix =  model;

	vec3 vPos = pos;
	float w = PI2 / waveLength;
	float pSpeed = 2 * waveSpeed / waveLength;
	float q = steepness / (w * amplitude);
	
	vec2 dir = normalize(waveDirection);

	float a = dot(vec2(pos.x, pos.y),dir) * w + time * pSpeed;

	vPos.x = pos.x + q * amplitude * dir.x * cos(a);
	vPos.y = pos.y + q * amplitude * dir.y * cos(a);
	vPos.z = - amplitude * sin(a);

	gl_Position = projection * view * modelMatrix * vec4(vPos, 1.0);
	Color = color;
	
	texCoord = tex;

	// Normal = mat3(transpose(inverse(modelMatrix))) * norm;
	
	Normal.x = - dir.x * w * amplitude * cos(a);
	Normal.y = - dir.y * w * amplitude * cos(a);
	Normal.z = 1 - q * w * amplitude * sin(a);

	FragPos = (modelMatrix * vec4(vPos, 1.0)).xyz;
}