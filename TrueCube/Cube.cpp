#include<iostream>
#include<InitOpenGL.h>
#include<vmath.h>
#include<time.h>

#include<GL\GLAUX.H>
#pragma comment(lib,"glaux.lib")

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
GLuint vbo[2],vboTc;
GLuint tex[8];
GLuint ebo;

struct Uniform
{
	int mvMatrixLocation;
	int projMatrixLocation;
}Uniforms;


GLfloat theLeft =-3.0, theRight = 3.0;
GLfloat bottom = -3.0, top = 3.0;
GLfloat zNear = 0.5, zFar = 4.0;
GLfloat t = 0; //旋转的控制变量
GLint n = 0; // 表示已经过了几个周期
GLfloat aboutSize = 1.2; // 大小系数，1.0表示原大小，变大则代表变大。。。

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

AUX_RGBImageRec *LoadBMP(char *Filename)				// Loads A Bitmap Image
{
	FILE *File = NULL;									// File Handle

	if (!Filename)										// Make Sure A Filename Was Given
	{
		return NULL;									// If Not Return NULL
	}

	File = fopen(Filename, "r");							// Check To See If The File Exists

	if (File)											// Does The File Exist?
	{
		fclose(File);									// Close The Handle
		return auxDIBImageLoadA(Filename);				// Load The Bitmap And Return A Pointer
	}
	return NULL;										// If Load Failed Return NULL
}

GLuint LoadGLTextures(char *filename,GLuint texture)									// Load Bitmaps And Convert To Textures
{
	int Status = FALSE;									// Status Indicator

	AUX_RGBImageRec *TextureImage[1];					// Create Storage Space For The Texture

	memset(TextureImage, 0, sizeof(void *) * 1);           	// Set The Pointer To NULL

	// Load The Bitmap, Check For Errors, If Bitmap's Not Found Quit
	if (TextureImage[0] = LoadBMP(filename))
	{
		Status = TRUE;									// Set The Status To TRUE

		glGenTextures(1, &texture);					// Create The Texture

		// Typical Texture Generation Using Data From The Bitmap
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	if (TextureImage[0])									// If Texture Exists
	{
		if (TextureImage[0]->data)							// If Texture Image Exists
		{
			free(TextureImage[0]->data);					// Free The Texture Image Memory
		}

		free(TextureImage[0]);								// Free The Image Structure
	}

	return texture;						// Return The Status
}

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
	/* glDrawElement 函数绘图 时只需要8个顶点的坐标，因为使用了索引
	GLfloat CubePosition[] = {
		1.0f,1.0f,1.0f,
		0.0f,1.0f,1.0f,
		0.0f,0.0f,1.0f,
		1.0f,0.0f,1.0f,
		1.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

	};*/
	
	//采用了glDrawArrays 函数绘图后，需要将每一个三角形图元的顶点坐标写清楚，所以比较长，而且耗费的内存更多
	GLfloat CubePosition[] = {
		0.0f, 0.0f, 0.0f, 
		1.0f, 0.0f, 0.0f, 
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 
		0.0f, 1.0f, 0.0f, 
		0.0f, 0.0f, 0.0f, 

		
		0.0f, 0.0f, 1.0f, 
		1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 
		1.0f, 1.0f, 1.0f, 
		0.0f, 1.0f, 1.0f, 
		0.0f, 0.0f, 1.0f, 

		0.0f, 1.0f, 1.0f, 
		0.0f, 1.0f, 0.0f, 
		0.0f, 0.0f, 0.0f, 
		0.0f, 0.0f, 0.0f, 
		0.0f, 0.0f, 1.0f, 
		0.0f, 1.0f, 1.0f,

		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 
		1.0f, 0.0f, 0.0f, 
		1.0f, 0.0f, 0.0f, 
		1.0f, 0.0f, 1.0f, 
		1.0f, 1.0f, 1.0f,

		0.0f, 0.0f, 0.0f, 
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 1.0f, 
		1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 
		0.0f, 0.0f, 0.0f, 

		0.0f, 1.0f, 0.0f, 
		1.0f, 1.0f, 0.0f, 
		1.0f, 1.0f, 1.0f, 
		1.0f, 1.0f, 1.0f, 
		0.0f, 1.0f, 1.0f, 
		0.0f, 1.0f, 0.0f, 
	};
	/*
	//原来没有使用纹理时采用的颜色
	GLfloat CubeColor[] = {
		0.583f, 0.771f, 0.014f,
		0.609f, 0.115f, 0.436f,
		0.327f, 0.483f, 0.844f,
		0.822f, 0.569f, 0.201f,
		0.435f, 0.602f, 0.223f,
		0.310f, 0.747f, 0.185f,
		0.597f, 0.770f, 0.761f,
		0.559f, 0.436f, 0.730f
	};*/

	GLfloat CubeTextureCoord[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,

		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,

		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,

		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,

		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,

		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f
	};
	/* glDrawElements 的索引数组
	GLushort CubeIndices[] = {
		2,1,3,0,7,4,6,5,
		0xFFFF,
		3,7,2,6,1,5,0,4
	};
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(CubeIndices), CubeIndices, GL_STATIC_DRAW);
	*/
	// 点的坐标
	glGenBuffers(2, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(CubePosition), CubePosition, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);
	/*
	//原来没有使用纹理时采用的颜色
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(CubeColor), CubeColor, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(1);
	*/
	// 纹理坐标
	glGenBuffers(1, &vboTc);
	glBindBuffer(GL_ARRAY_BUFFER, vboTc);
	glBufferData(GL_ARRAY_BUFFER, sizeof(CubeTextureCoord), CubeTextureCoord, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(2);
	// ------------- 
	// Texture 纹理
	glEnable(GL_TEXTURE_2D);
	tex[0] = LoadGLTextures("1.bmp", tex[0]);
	tex[1] = LoadGLTextures("2.bmp", tex[1]);
	tex[2] = LoadGLTextures("3.bmp", tex[2]);
	tex[3] = LoadGLTextures("4.bmp", tex[3]);
	tex[4] = LoadGLTextures("5.bmp", tex[4]);
	tex[5] = LoadGLTextures("6.bmp", tex[5]);
	tex[6] = LoadGLTextures("7.bmp", tex[6]);
	tex[7] = LoadGLTextures("8.bmp", tex[7]);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}   

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glUseProgram(program);

	glBindVertexArray(vao);
	// 之前没有加纹理时采用了 glDrawElement 函数绘图
	// 比较省内存，但是由于不知道如何为每个顶点设定不同的纹理坐标，所以又重新采用了 glDrawArrays 函数绘图
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	//glActiveTexture(GL_TEXTURE0);
	
	//每次调整投影矩阵
	projMatrix = frustum(theLeft, theRight, bottom, top, zNear, zFar);
	glUniformMatrix4fv(Uniforms.projMatrixLocation, 1, GL_FALSE, projMatrix);
	
	for (int i = 0; i != 8; i++)
	{
		glUniform1i(glGetUniformLocation(program, "textureSampler"), 0);
		glBindTexture(GL_TEXTURE_2D, tex[i]);
		modelMatrix = scale(aboutSize,aboutSize,aboutSize);
		modelMatrix = translate(aboutSize*TranslateArray[i])*modelMatrix;
		if (JudgeForUpDownorLeftRight > 0){
			if (Judge[i][0] == RIGHT){
				modelMatrix = rotate(-t, vec3(1.0f, 0.0f, 0.0f))*rotate(t, vec3(0.0f, 1.0f, 0.0f))*modelMatrix;
			}
			else
			{
				modelMatrix = rotate(-t, vec3(1.0f, 0.0f, 0.0f))*rotate(-t, vec3(0.0f, 1.0f, 0.0f))*modelMatrix;
			}
		}
		else
		{
			if (Judge[i][1] ==UP){
				modelMatrix = rotate(-t, vec3(1.0f, 0.0f, 0.0f))* rotate(t, vec3(0.0f, 0.0f, 1.0f))*modelMatrix;
			}
			else
			{
				modelMatrix = rotate(-t, vec3(1.0f, 0.0f, 0.0f))* rotate(-t, vec3(0.0f, 0.0f, 1.0f))*modelMatrix;
			}
		}
		modelMatrix = rotate(t, vec3(0.0, 0.0, 1.0))*modelMatrix;
		mvMatrix = viewMatrix * modelMatrix;
		glUniformMatrix4fv(Uniforms.mvMatrixLocation, 1, GL_FALSE, mvMatrix);
		glDrawArrays(GL_TRIANGLES, 0, 3 * 12);
		//glDrawElements(GL_TRIANGLE_STRIP, 8, GL_UNSIGNED_SHORT, NULL);
		//glDrawElements(GL_TRIANGLE_STRIP, 8, GL_UNSIGNED_SHORT, BUFFER_OFFSET(sizeof(GLushort) * 9));

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
		std::cout <<"Round:"<< n << std::endl;
	}
	Sleep(20);
}

void keyBoardReflect(unsigned char key, int x,int y)
{
	switch ( key)
	{
	case 'i':
		aboutSize += 0.1;
		std::cout << "Size:" << aboutSize << std::endl;
		break;
	case 'd':
		aboutSize -= 0.1;
		std::cout << "Size:" << aboutSize << std::endl;
		break;
	case 'F':
		zFar += 0.1;
		std::cout << "zFar:" << zFar << std::endl;
		break;
	case 'N':
		zFar -= 0.1;
		if (zFar < 0){
			zFar = 0;
		}
		std::cout << "zFar:" << zFar << std::endl;
		break;
	case 'f':
		zNear += 0.1;
		if (zNear < 0){
			zNear = 0;
		}
		std::cout << "zNear:" << zNear << std::endl;
		break;
	case 'n':
		zNear -= 0.1;
		if (zNear < 0){
			zNear = 0;
		}
		std::cout << "zNear:" << zNear << std::endl;
		break;
	default:
		break;
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE);
	glutInitWindowSize(768, 768);
	glutInitWindowPosition(200, 200);
	glutCreateWindow(argv[0]);
	if (glewInit()){
		std::cerr << "Error" << std::endl;
		exit(EXIT_FAILURE);
	}

	init();
	glutDisplayFunc(display);
	glutIdleFunc(idleDisplay);
	glutKeyboardFunc(keyBoardReflect);
	glutMainLoop();
}
