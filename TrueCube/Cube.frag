#version 430 core

in vec3 fragmentColor;
in vec2 uv;

uniform sampler2D textureSampler;

out vec4 fColor;
void main()
{

	//fColor = vec4(fragmentColor,1.0);
	fColor = texture(textureSampler,uv);
}