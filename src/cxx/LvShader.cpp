#include "LvShader.h"
namespace lv {

	LvShaderSource::LvShaderSource()
		:_type(ShaderType::Vertex), _isAvailable(false),_shader(0)
	{
		this->_source = new std::string();
	}

	LvShaderSource::LvShaderSource(const char* sourcePtr, ShaderType shaderType)
		: _type(shaderType), _isAvailable(false), _shader(0)
	{
		this->_source = new std::string(sourcePtr);
	}

	LvShaderSource::~LvShaderSource() {
		ReleaseShader();
		delete _source;
	}

	void LvShaderSource::SetProgramText(char * sourcePtr)
	{
		_source->assign(sourcePtr);
	}

	void LvShaderSource::ClearProgramText()
	{
		if (_source != nullptr)
			_source->clear();
	}

	void LvShaderSource::ReleaseShader()
	{
		if (_shader != 0) {
			glDeleteShader(_shader);
		}
		_isAvailable = false;
	}

	bool LvShaderSource::Compile(std::string * log)
	{
		if (_source == nullptr || _source->size() == 0)
			return false;

		int programTypID = ShaderTypeFlag(this->_type);
		if (programTypID == -1)
			return false;

		// compile opengl shader:
		GLuint shaderID = glCreateShader(programTypID);
		if (shaderID == 0)
			return false;

		const char * srcPtr = _source->c_str();

		glShaderSource(shaderID, 1, &srcPtr, nullptr);
		glCompileShader(shaderID);

		GLint result = GL_FALSE;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);

		if (result == GL_FALSE)
		{
			GLint infoLogLen = 0;
			glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLen);
			if (log != nullptr) {
				char* logPtr = (char*)malloc(sizeof(char)* (infoLogLen + 1));
				glGetShaderInfoLog(shaderID, infoLogLen, nullptr, logPtr);
				log->assign(logPtr);
				free(logPtr);
			}
			glDeleteShader(shaderID);
			shaderID = 0;
		}
		_shader = shaderID;

		_isAvailable = _shader != 0 ? true : false;
		return _isAvailable;
	}

	int32 lv::LvShaderSource::ShaderTypeFlag(ShaderType programType)
	{
		switch (programType)
		{
		case ShaderType::Vertex:
			return GL_VERTEX_SHADER;
		case ShaderType::Fragment:
			return GL_FRAGMENT_SHADER;
		default:
			return -1;
		}
	}

	LvShader::LvShader() :_isAvailable(false)
	{
	}

	LvShader::~LvShader()
	{
	}
}