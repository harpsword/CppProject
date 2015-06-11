#include<iostream>
#include<InitOpenGL.h>
#include<vmath.h>
#include<time.h>
using namespace vmath;

#include "LoadShaders.h"

GLuint vao, program;
GLuint vbo[2];
GLuint ebo;

struct Uniform
{
	int mvMatrixLocation;
	int projMatrixLocation;
}Uniforms;

GLfloat theLeft =-3.0, theRight = 3.0;
GLfloat bottom = -3.0, top = 3.0;
GLfloat zNear = 0.7, zFar = 3.0;
GLfloat t = 0;
void init(void)
{
	ShaderInfo Shaders[] = {
			{ GL_VERTEX_SHADER, "cube.vert" },
			{ GL_FRAGMENT_SHADER, "cube.frag" },
			{ GL_NONE, NULL }
	};
	program = LoadShaders(Shaders);
	glUseProgram(program);

	Uniforms.mvMatrixLocation = glGetUniformLocation(program, "mv_matrix");
	Uniforms.projMatrixLocation = glGetUniformLocation(program, "proj_matrix");

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	
	GLfloat CubePosition[] = {
		0.5, 0.5, 0.5,
		-0.5, 0.5, 0.5,
		-0.5, -0.5, 0.5,
		0.5, -0.5, 0.5,
		0.5, 0.5, -0.5,
		-0.5, 0.5, -0.5,
		-0.5, -0.5, -0.5,
		0.5, -0.5, -0.5
	};
	/*
	GLfloat CubePosition[] = {
		-1.0f, -1.0f, -1.0f, // triangle 1 : begin
		-1.0f, -1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f, // triangle 1 : end
		1.0f, 1.0f, -1.0f, // triangle 2 : begin
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f, // triangle 2 : end
		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f
	};*/
	
	GLfloat CubeColor[] = {
		0.583f, 0.771f, 0.014f,
		0.609f, 0.115f, 0.436f,
		0.327f, 0.483f, 0.844f,
		0.822f, 0.569f, 0.201f,
		0.435f, 0.602f, 0.223f,
		0.310f, 0.747f, 0.185f,
		0.597f, 0.770f, 0.761f,
		0.559f, 0.436f, 0.730f
	};
	/*
	GLfloat CubeColor[] = {
		0.583f, 0.771f, 0.014f,
		0.609f, 0.115f, 0.436f,
		0.327f, 0.483f, 0.844f,
		0.822f, 0.569f, 0.201f,
		0.435f, 0.602f, 0.223f,
		0.310f, 0.747f, 0.185f,
		0.597f, 0.770f, 0.761f,
		0.559f, 0.436f, 0.730f,
		0.359f, 0.583f, 0.152f,
		0.483f, 0.596f, 0.789f,
		0.559f, 0.861f, 0.639f,
		0.195f, 0.548f, 0.859f,
		0.014f, 0.184f, 0.576f,
		0.771f, 0.328f, 0.970f,
		0.406f, 0.615f, 0.116f,
		0.676f, 0.977f, 0.133f,
		0.971f, 0.572f, 0.833f,
		0.140f, 0.616f, 0.489f,
		0.997f, 0.513f, 0.064f,
		0.945f, 0.719f, 0.592f,
		0.543f, 0.021f, 0.978f,
		0.279f, 0.317f, 0.505f,
		0.167f, 0.620f, 0.077f,
		0.347f, 0.857f, 0.137f,
		0.055f, 0.953f, 0.042f,
		0.714f, 0.505f, 0.345f,
		0.783f, 0.290f, 0.734f,
		0.722f, 0.645f, 0.174f,
		0.302f, 0.455f, 0.848f,
		0.225f, 0.587f, 0.040f,
		0.517f, 0.713f, 0.338f,
		0.053f, 0.959f, 0.120f,
		0.393f, 0.621f, 0.362f,
		0.673f, 0.211f, 0.457f,
		0.820f, 0.883f, 0.371f,
		0.982f, 0.099f, 0.879f
	};*/

	GLushort CubeIndices[] = {
		2,1,3,0,7,4,6,5,
		0xFFFF,
		3,7,2,6,1,5,0,4
	
	};

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(CubeIndices), CubeIndices, GL_STATIC_DRAW);

	glGenBuffers(2, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(CubePosition), CubePosition, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);

	
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(CubeColor), CubeColor, GL_STATIC_DRAW);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(1);

	//std::cout << glGetError() << std::endl;

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}   

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	std::cout << glGetError();

	glUseProgram(program);

	vec3 eye = vec3(0.0,0.0,1.0);
	vec3 center = vec3(0.0,0.0,0.0);
	vec3 up = vec3(1.0,0.0,0.0);
	mat4 viewMatrix = lookat(eye, center, up);
	mat4 modelMatrix = rotate(t,vec3(0.0,0.0,1.0))*translate(0.0f,1.0f,0.0f);
	mat4 mvMatrix = viewMatrix * modelMatrix;
	glUniformMatrix4fv(Uniforms.mvMatrixLocation, 1, GL_FALSE, mvMatrix);

	mat4 projMatrix = frustum(theLeft, theRight, bottom, top, zNear, zFar);
	//projMatrix = perspective(3.0f, 1.0f, 0.1f, 10.0f);
	//projMatrix = ortho(theLeft,theRight,bottom,top,zNear,zFar);
	glUniformMatrix4fv(Uniforms.projMatrixLocation, 1, GL_FALSE, projMatrix);
	
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	std::cout << glGetError() << std::endl;
#if USE_PRIMITIVE_RESTART
	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(0xFFFF);
	glDrawElements(GL_TRIANGLE_STRIP, 17, GL_UNSIGNED_SHORT, NULL);
#else
	glDrawElements(GL_TRIANGLE_STRIP, 8, GL_UNSIGNED_SHORT, NULL);
	glDrawElements(GL_TRIANGLE_STRIP, 8, GL_UNSIGNED_SHORT, BUFFER_OFFSET(sizeof(GLushort) * 9));
#endif
	std::cout << glGetError() << std::endl;

	glutSwapBuffers();

}

void idleDisplay(void)
{
	t += 0.01;
	display();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE);
	glutInitWindowSize(512, 512);
	glutInitWindowPosition(200, 200);
	std::cout << time(0)<<std::endl;
	glutCreateWindow(argv[0]);
	if (glewInit()){
		std::cerr << "Error" << std::endl;
		exit(EXIT_FAILURE);
	}

	init();
	glutDisplayFunc(display);
	glutIdleFunc(idleDisplay);
	glutMainLoop();
}
