#pragma once
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <stdlib.h>
#include <iostream>
namespace lv {
	class ShaderCompiler
	{
	public:
		enum LvShaderProgramType {
			Vertex,
			Fragment
		};
		ShaderCompiler() {};
		~ShaderCompiler() {};
		GLuint CompileShader(const char * shaderSrc, const int srcLen, LvShaderProgramType programType/*, char* infoLog, int* infoLogLen*/);
		int ShaderTypeFlag(LvShaderProgramType programType);
	private:

	};
}