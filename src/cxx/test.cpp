//#include <stdio.h>
//#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <streambuf>

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "LvShader.h"
#include "math.h"

using namespace std;
using namespace lv;

int main()
{
	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	//glfwWindowHint(GLFW_CONTEXT_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL

	GLFWwindow* window;
	window = glfwCreateWindow(1024, 768, "test", NULL, NULL);

	if (window == nullptr) {
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	GLuint vertexArrayID;
	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	static const GLfloat vBuffer[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f,  1.0f, 0.0f,
	};

	GLuint vBufferId;
	glGenBuffers(1, &vBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, vBufferId);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vBuffer), vBuffer, GL_STATIC_DRAW);

	std::ifstream ifs;

	ifs.open("Shader/testVertex.shader");
	string vsSrc((std::istreambuf_iterator<char>(ifs)),
		std::istreambuf_iterator<char>());
	ifs.close();

	LvShaderSource* vs = new LvShaderSource(vsSrc.c_str(), ShaderType::Vertex);
	string log;
	if (!vs->Compile(&log)) {
		printf(log.c_str());
	}
	

	ifs.open("Shader/testFragment.shader");

	string fsSrc((std::istreambuf_iterator<char>(ifs)),
		std::istreambuf_iterator<char>());
	ifs.close();

	LvShaderSource* fs = new LvShaderSource(fsSrc.c_str(), ShaderType::Fragment);
	if (fs->Compile(&log)) {
		printf(log.c_str());
	}


	uint32 program = glCreateProgram();
	glAttachShader(program, vs->shader());
	glAttachShader(program, fs->shader());
	glLinkProgram(program);

	int32 linkResult = -1;
	int32 infoLen = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &linkResult);
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);

	if (infoLen > 0) 
	{
		char* log = (char*)malloc((infoLen + 1) * sizeof(char));
		glGetProgramInfoLog(program, infoLen, nullptr,log);
		printf("%s", log);
		free(log);
	}

	delete vs;
	delete fs;


	GLuint vec3ID = glGetUniformLocation(program, "inputColor");

	do {
		// Clear the screen. It's not mentioned before Tutorial 02, but it can cause flickering, so it's there nonetheless.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(program);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexArrayID);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glUniform3f(vec3ID, 1.0f, 0.0f, 1.0f);

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
		glDisableVertexAttribArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	return 0;
}