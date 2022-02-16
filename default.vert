#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;
layout (location = 2) in vec2 aUV;

out vec3 color;
out vec2 uv;

uniform float time;

void main()
{
	float x = aPos.x + 0.5 * sin(time) + 0.2 * sin(time * 2.0 + aPos.x * 2.2 + aPos.y * 1.3) + 0.2 * cos(time * 2.0 + aPos.x * 2.2 + aPos.y * 1.3);
	float y = aPos.y + 0.5 * sin(time) + 0.2 * sin(time * 2.0 + aPos.y * 2.2 + aPos.x * 1.3) + 0.2 * cos(time * 2.0 + aPos.y * 2.2 + aPos.x * 1.3);
	gl_Position = vec4(x * 1.0, y * 1.0, aPos.z, 1.0);
	color = aCol;
	uv = aUV;
}