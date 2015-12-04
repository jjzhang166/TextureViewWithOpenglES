#include "com_example_textureviewwithopengles_RenderCode.h"
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "math3d.h"
#include "MyLog.h"
#define VERTEX_DATA 0
#define NORMAL_DATA 1
#define TEXTURE_DATA 2
#define INDEX_DATA 3
#define COLOR_DATA 4

#define VERTEX_BUFFER_NUM 5
#define VERTEX_ARRAY_OBJECT_NUM 2
#define TEXTURE_BUFFER_NUM 2
#define SQUARE_NUM 1

#define TEXTURE_WIDTH 512
#define TEXTURE_HEIGHT 512

#define PIXEL_BUFFER_SIZE TEXTURE_WIDTH*TEXTURE_HEIGHT//786432 //PIXEL_BUFFER_SIZE=TEXTURE_WIDTH*TEXTURE_HEIGHT
#define ORTHO_RANGE 1
#define SCALE_NUM 100

GLuint VBO[VERTEX_BUFFER_NUM];
GLuint VAO[VERTEX_ARRAY_OBJECT_NUM];
GLuint VBO_SCALE[VERTEX_BUFFER_NUM];
GLuint TO;
GLuint PIXEL_BUFFER[PIXEL_BUFFER_SIZE];
GLfloat SQUARE_TRANSLATE[3];
GLuint vertexShader;
GLuint fragmentShader;
GLuint shaderProgram;
GLchar* fragmentShaderSource =
		"void main() {\n"
		"   gl_FragColor = vec4(1,1,0,1);\n"
		"}\0";
GLchar* vertexShaderSource =
		"attribute vec4 position;\n"
		"attribute vec4 normal;\n"
		"attribute vec2 texCoords;\n"
		"uniform vec4 light;\n"
		"uniform mat4 normalMatrix;"
		"uniform mat4 mvMatrix;\n"
		"uniform mat4 projMatrix;\n"
		"void main(void) {\n"
		"gl_Position=projMatrix*mvMatrix*vec4(position.xyz,1);\n"
		"}\0";
GLint locRed;
GLint locMvMat;
GLint locProjMat;
GLint locTextureImg;
GLint locNormalMatrix;
GLint locLight;
M3DMatrix44f m_proj;
M3DMatrix44f m_mv;

JNIEXPORT void JNICALL Java_com_example_textureviewwithopengles_RenderCode_InitGL(JNIEnv *env, jclass thiz) {
	for (int i = 0; i < PIXEL_BUFFER_SIZE; ++i) {
		PIXEL_BUFFER[i] = 0xFF00FF00;
	}
	GLfloat pVerts[24][3] = { { -1, -1, 1 }, { -1, -1, 1 }, { -1, -1, 1 }, //0,1,2
			{ 1, -1, 1 }, { 1, -1, 1 }, { 1, -1, 1 }, //3,4,5
			{ 1, 1, 1 }, { 1, 1, 1 }, { 1, 1, 1 }, //6,7,8
			{ -1, 1, 1 }, { -1, 1, 1 }, { -1, 1, 1 }, //9,10,11
			{ -1, -1, -1 }, { -1, -1, -1 }, { -1, -1, -1 }, //12,13,14
			{ 1, -1, -1 }, { 1, -1, -1 }, { 1, -1, -1 }, //15,16,17
			{ 1, 1, -1 }, { 1, 1, -1 }, { 1, 1, -1 }, //18,19,20
			{ -1, 1, -1 }, { -1, 1, -1 }, { -1, 1, -1 } //21,22,23
	};
	GLfloat pColors[24][4] = { { 1, 0.5, 0, 1 }, { 1, 0.5, 0, 1 }, { 1, 0.5, 0, 1 }, { 1, 0.5, 0, 1 }, { 1, 0.5, 0, 1 }, { 1, 0.5, 0, 1 }, { 1, 0.5, 0, 1 }, { 1, 0.5, 0, 1 }, { 1, 0.5, 0, 1 }, { 1, 0.5, 0, 1 }, { 1, 0.5, 0, 1 }, { 1, 0.5, 0, 1 }, { 1, 0.5, 0, 1 }, { 1, 0.5, 0, 1 }, { 1, 0.5, 0, 1 }, { 1, 0.5, 0, 1 }, { 1, 0.5, 0, 1 }, { 1, 0.5, 0, 1 }, { 1, 0.5, 0, 1 }, { 1, 0.5, 0, 1 }, { 1, 0.5, 0, 1 }, { 1, 0.5, 0, 1 }, { 1, 0.5, 0, 1 }, { 1, 0.5, 0, 1 } };
	GLfloat pNorms[24][3] = { { 0, 0, 1 }, //0
			{ -1, 0, 0 }, //1
			{ 0, -1, 0 }, //2
			{ 0, 0, 1 }, //3
			{ 1, 0, 0 }, //4
			{ 0, -1, 0 }, //5
			{ 0, 0, 1 }, //6
			{ 1, 0, 0 }, //7
			{ 0, 1, 0 }, //8
			{ 0, 0, 1 }, //9
			{ -1, 0, 0 }, //10
			{ 0, 1, 0 }, //11
			{ 0, 0, -1 }, //12
			{ -1, 0, 0 }, //13
			{ 0, -1, 0 }, //14
			{ 0, 0, -1 }, //15
			{ 1, 0, 0 }, //16
			{ 0, -1, 0 }, //17
			{ 0, 0, -1 }, //18
			{ 1, 0, 0 }, //19
			{ 0, 1, 0 }, //20
			{ 0, 0, -1 }, //21
			{ -1, 0, 0 }, //22
			{ 0, 1, 0 }, //23
			};
	GLfloat pTexCoords[24][2] = { { 0, 0 }, //0
			{ 0, 0 }, //1
			{ 0, 0 }, //2
			{ 1, 0 }, //3
			{ 0, 0 }, //4
			{ 1, 0 }, //5
			{ 1, 1 }, //6
			{ 0, 1 }, //7
			{ 1, 0 }, //8
			{ 0, 1 }, //9
			{ 0, 1 }, //10
			{ 0, 0 }, //11
			{ 0, 0 }, //12
			{ 1, 0 }, //13
			{ 0, 1 }, //14
			{ 1, 0 }, //15
			{ 1, 0 }, //16
			{ 1, 1 }, //17
			{ 1, 1 }, //18
			{ 1, 1 }, //19
			{ 1, 1 }, //20
			{ 0, 1 }, //21
			{ 1, 1 }, //22
			{ 0, 1 } //23
	};
	GLushort pIndexes[36] = { 0, 3, 9, 3, 6, 9, 12, 21, 15, 15, 21, 18, 1, 10, 13, 13, 10, 22, 4, 16, 19, 4, 19, 7, 11, 8, 23, 8, 20, 23, 2, 14, 5, 5, 14, 17 };
	GLfloat vLight[4] = { 0, 0, -1, 0 };
	//  glGenVertexArrays(2,VAO);
	//  glBindVertexArray(VAO[0]);
	glGenBuffers(VERTEX_BUFFER_NUM, VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[VERTEX_DATA]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 24 * 3, pVerts, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[NORMAL_DATA]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 24 * 3, pNorms, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[TEXTURE_DATA]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 24 * 2, pTexCoords, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[COLOR_DATA]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 24 * 4, pColors, GL_STATIC_DRAW);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO[INDEX_DATA]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * 36, pIndexes, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	//glBindVertexArrayOES(0);

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(vertexShader);
	glCompileShader(fragmentShader);
	//////////////////
	GLint testValue=0;
	glGetShaderiv(vertexShader,GL_COMPILE_STATUS,&testValue);
	if(testValue==GL_FALSE)
	{
		LOG("SHADER","vertexShader compile failed",__LINE__);
	}
    glGetShaderiv(fragmentShader,GL_COMPILE_STATUS,&testValue);
    if(testValue==GL_FALSE)
    {
    	LOG("SHADER","fragmentShader compile failed",__LINE__);
    }
	//////////////////
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glBindAttribLocation(shaderProgram, 0, "position");
	glBindAttribLocation(shaderProgram, 1, "normal");
	glBindAttribLocation(shaderProgram, 2, "texCoords");
	glLinkProgram(shaderProgram);

	locRed = glGetUniformLocation(shaderProgram, "red");
	locMvMat = glGetUniformLocation(shaderProgram, "mvMatrix");
	locProjMat = glGetUniformLocation(shaderProgram, "projMatrix");
	locTextureImg = glGetUniformLocation(shaderProgram, "textureImg");
	locNormalMatrix = glGetUniformLocation(shaderProgram, "normalMatrix");
	locLight = glGetUniformLocation(shaderProgram, "light");

	glUseProgram(shaderProgram);
	glGenTextures(1, &TO);

	glBindTexture(GL_TEXTURE_2D, TO);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, PIXEL_BUFFER);
	//glTexImage2D(GL_TEXTURE_2D,0, GL_RGBA,image.width(),image.height(), 0,GL_RGBA,GL_UNSIGNED_BYTE,image.constBits());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//    glTexParameteri(GL_TEXTURE_2D,GL_GENERATE_MIPMAP,GL_FALSE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindTexture(GL_TEXTURE_2D, 0);
	glClearColor(0.5, 0.1, 0.2, 1);
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_FRONT);
	glEnable(GL_CULL_FACE);
	//glDisable(GL_CULL_FACE);
	glUniform1i(locTextureImg, 0);
	glUniform4fv(locLight, 1, vLight);
	m3dLoadIdentity44(m_mv);
}

JNIEXPORT void JNICALL Java_com_example_textureviewwithopengles_RenderCode_ResizeGL(JNIEnv *env, jclass thiz, jint width, jint height) {
	 if(width<=0)
	        width=1;
	    if(height<=0)
	        height=1;
	    glViewport(0, 0,width, height);
	    m3dLoadIdentity44(m_proj);
	    //m3dMakePerspectiveMatrix(m_proj,15.0f,(GLfloat)width/(GLfloat)height,1.0f,100.0f);
	    m3dMakeOrthographicMatrix(m_proj,-2,2,-2,2,-100,100);
	    //m_proj.ortho(-2,2,-2,2,1,100);
	    glUniformMatrix4fv(locProjMat,1,GL_FALSE,m_proj);

}

JNIEXPORT void JNICALL Java_com_example_textureviewwithopengles_RenderCode_PaintGL(JNIEnv *env, jclass thiz) {
	  static float tempRed=0;
	    static float delta=0.001;
	    static float rotateDegree=0;
	    if(tempRed>1)
	    {
	        delta=-0.001;
	    }
	    else
	    {
	        if(tempRed<0)
	        {
	            delta=0.001;
	        }
	    }
	    glBindTexture(GL_TEXTURE_2D, TO);
	    tempRed+=delta;
	    m3dLoadIdentity44(m_mv);
	    m3dTranslationMatrix44(m_mv,0,0,-15);
	    m3dRotationMatrix44(m_mv,rotateDegree,1,1,0);
	    rotateDegree+=0.001;
	    glUniformMatrix4fv(locMvMat,1,GL_FALSE,m_mv);
	    glUniformMatrix4fv(locNormalMatrix,1,GL_FALSE,m_mv);
	    //glUniform1f(locRed,tempRed);
	    glClear(GL_COLOR_BUFFER_BIT);
	    glClearColor(0,tempRed,0,1);
	    //glBindVertexArray(VAO[0]);
	    glDrawElements(GL_TRIANGLES,36,GL_UNSIGNED_SHORT,0);
}
