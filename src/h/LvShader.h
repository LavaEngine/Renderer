#pragma once
#ifndef LV_SHADER
#define LV_SHADER
#include "LvDefines.h"
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <stdlib.h>
#include <iostream>
#include <vector>
using namespace lv;

namespace lv {
	enum ShaderType {
		Vertex,
		Fragment
	};

	class LvShaderSource // a piece of single shader program
	{
	public:
		LvShaderSource();
		LvShaderSource(const char *sourcePtr, ShaderType shaderType);
		~LvShaderSource();
		void SetProgramText(char* sourcePtr);
		void ClearProgramText();
		void ReleaseShader();
		bool Compile(std::string *log = nullptr);
	public:
		inline void SetType(ShaderType type) { _type = type; }
		inline std::string* source() const { return _source; }
		inline ShaderType type() const { return _type; }
		inline bool isAvailable() const { return _isAvailable; }
		inline uint32 shader() const { return _shader; }
	private:
		int32 ShaderTypeFlag(ShaderType programType);
	private:
		std::string *_source;
		ShaderType _type;
		uint32 _shader;
		bool _isAvailable;
	};

	class LvShader
	{
	public:
		LvShader();
		~LvShader();
	private:
		bool _isAvailable;
	};
}
#endif // !LV_SHADER