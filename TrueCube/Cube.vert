#version 430 core
layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vColor;
layout(location = 2) in vec2 tc;

out vec2 uv;
out vec3 fragmentColor;

uniform mat4 proj_matrix;
uniform mat4 mv_matrix;

void main()
{
	vec4 position = vec4(vPosition,1);
	gl_Position = proj_matrix * mv_matrix * position;
	//gl_Position = proj_matrix * position;

	fragmentColor = vColor;
	uv = tc;
}