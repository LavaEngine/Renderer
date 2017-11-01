#include "LvShaderCompiler.h"

namespace lv {
	GLuint lv::ShaderCompiler::CompileShader(const char * shaderSrc, const int srcLen, LvShaderProgramType programType/*, char* infoLog, int* infoLogLen*/)
	{
		//GLuint result = 0;

		int programTypID = ShaderTypeFlag(programType);
		if (programTypID == -1)
			return 0;

		GLuint shaderID = glCreateShader(programTypID);
		if (shaderID == 0)
			return shaderID;

		glShaderSource(shaderID, 1, &shaderSrc, nullptr);
		glCompileShader(shaderID);

		GLint result = GL_FALSE;
		GLint infoLogLen = 0;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLen);

		if (infoLogLen > 0)
		{
			char* infoLog = (char*)malloc(sizeof(char)* (infoLogLen + 1));
			glGetShaderInfoLog(shaderID, infoLogLen, nullptr, infoLog);
			printf("%s", infoLog);
		}
		return shaderID;
	}

	int lv::ShaderCompiler::ShaderTypeFlag(LvShaderProgramType programType)
	{
		switch (programType)
		{
		case LvShaderProgramType::Vertex:
			return GL_VERTEX_SHADER;
		case LvShaderProgramType::Fragment:
			return GL_FRAGMENT_SHADER;
		default:
			return -1;
		}
	}
}