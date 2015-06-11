#version 430 core

in vec3 fragmentColor;
out vec4 fColor;

void main()
{

	fColor = vec4(fragmentColor,1.0f);
}