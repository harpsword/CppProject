#version 430 core
layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vColor;
out vec3 fragmentColor;
uniform mat4 proj_matrix;
uniform mat4 mv_matrix;

void main()
{

	vec4 position = vec4(vPosition,1.0);
	gl_Position = proj_matrix * mv_matrix * position;
	//gl_Position = proj_matrix * position;
	fragmentColor = vColor;
}