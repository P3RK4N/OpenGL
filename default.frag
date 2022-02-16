#version 460 core

in vec3 color;
in vec2 uv;

out vec4 frag;

uniform sampler2D tex0;

void main()
{
	frag = texture(tex0, uv);
}