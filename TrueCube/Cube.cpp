#include<iostream>
#include<InitOpenGL.h>
#include<vmath.h>
#include<time.h>
using namespace vmath;

#include "LoadShaders.h"
#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3
#define ONE_ROUND 360
// it is for judging 
int Judge[8][2] = {
	RIGHT, UP,
	RIGHT, UP,
	LEFT, UP,
	LEFT, UP,
	RIGHT, DOWN,
	RIGHT, DOWN,
	LEFT, DOWN,
	LEFT, DOWN
};
int JudgeForUpDownorLeftRight = 1;
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
GLfloat zNear = 1.0, zFar = 5.0;
GLfloat t = 0;
GLint n = 0;

vec3 TranslateArray[8] = {
	vec3(0.0f, 0.0f, 0.0f),
	vec3(-1.0f, 0.0f, 0.0f),
	vec3(-1.0f, -1.0f, 0.0f),
	vec3(0.0f, -1.0f, 0.0f),
	vec3(0.0f, 0.0f, -1.0f),
	vec3(-1.0f, 0.0f, -1.0f),
	vec3(-1.0f, -1.0f, -1.0f),
	vec3(0.0f, -1.0f, -1.0f)
};

// model-view Matrix set
vec3 eye = vec3(2.0, 2.0, 2.0);
vec3 center = vec3(0.0, 0.0, 0.0);
vec3 up = vec3(-1.0, -1.0, 1.0);
mat4 viewMatrix = lookat(eye, center, up);
mat4 modelMatrix;
mat4 mvMatrix;
//projection matrix set
mat4 projMatrix = frustum(theLeft, theRight, bottom, top, zNear, zFar);
//projMatrix = perspective(3.0f, 1.0f, 0.1f, 10.0f);
//projMatrix = ortho(theLeft,theRight,bottom,top,zNear,zFar);

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
		1.0f,1.0f,1.0f,
		0.0f,1.0f,1.0f,
		0.0f,0.0f,1.0f,
		1.0f,0.0f,1.0f,
		1.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

	};

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

	glUseProgram(program);

	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glUniformMatrix4fv(Uniforms.projMatrixLocation, 1, GL_FALSE, projMatrix);
	
	for (int i = 0; i != 8; i++)
	{
		modelMatrix = translate(TranslateArray[i]);
		if (JudgeForUpDownorLeftRight > 0){
			if (Judge[i][0] == RIGHT){
				modelMatrix = rotate(t, vec3(0.0f, 1.0f, 0.0f))*modelMatrix;
			}
			else
			{
				modelMatrix = rotate(-t, vec3(0.0f, 1.0f, 0.0f))*modelMatrix;
			}
		}
		else
		{
			if (Judge[i][1] == UP){
				modelMatrix = rotate(t, vec3(0.0f, 0.0f, 1.0f))*modelMatrix;
			}
			else
			{
				modelMatrix = rotate(-t, vec3(0.0f, 0.0f, 1.0f))*modelMatrix;
			}
		}
		modelMatrix = rotate(t, vec3(0.0, 0.0, 1.0))*modelMatrix;
		mvMatrix = viewMatrix * modelMatrix;
		glUniformMatrix4fv(Uniforms.mvMatrixLocation, 1, GL_FALSE, mvMatrix);
		glDrawElements(GL_TRIANGLE_STRIP, 8, GL_UNSIGNED_SHORT, NULL);
		glDrawElements(GL_TRIANGLE_STRIP, 8, GL_UNSIGNED_SHORT, BUFFER_OFFSET(sizeof(GLushort) * 9));
	}
	glutSwapBuffers();

}

void idleDisplay(void)
{
	t += 1;
	
	display();
	if (t >= ONE_ROUND){
		t = 0;
		n += 1;
		JudgeForUpDownorLeftRight = -JudgeForUpDownorLeftRight;
		std::cout << n << std::endl;
	}
	Sleep(20);
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
