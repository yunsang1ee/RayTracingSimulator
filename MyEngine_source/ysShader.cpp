#include "ysShader.h"
#include <fstream>
#include <sstream>
#include <iostream>

ys::graphics::Shader::Shader()
	: Resource(enums::ResourceType::Shader) 
{
}

ys::graphics::Shader::~Shader()
{
	glDeleteProgram(shaderID);
}

HRESULT ys::graphics::Shader::Load(const std::wstring& path)
{
	size_t fineNameBeginOffset = path.rfind(L"\\") + 1;
	size_t fineNameEndOffset = path.length() - fineNameBeginOffset;
	const std::wstring fileName(path.substr(fineNameBeginOffset, fineNameEndOffset));

	shaderID = glCreateProgram();

	if (!Create(ShaderStage::VS, fileName)) 
		return S_FALSE;
	else 
		glAttachShader(shaderID, vertexShader);

	if (!Create(ShaderStage::FS, fileName))
		return S_FALSE;
	else
		glAttachShader(shaderID, fragmentShader);

	glLinkProgram(shaderID);

	GLint result;
	GLchar errorLog[512];
	glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderID, 512, NULL, errorLog);
		std::cerr << "ERROR: shader program 연결 실패\n" << errorLog << std::endl;
		return S_FALSE;
	}

	return S_OK;
}

bool ys::graphics::Shader::Create(const ys::graphics::ShaderStage stage, const std::wstring& fileName)
{
	switch (stage)
	{
	case ys::graphics::ShaderStage::VS:
		if (CreateVertexShader(fileName) == -1) return false;
		break;
	case ys::graphics::ShaderStage::HS:
		break;
	case ys::graphics::ShaderStage::DS:
		break;
	case ys::graphics::ShaderStage::GS:
		break;
	case ys::graphics::ShaderStage::FS:
		if (CreateFragmentShader(fileName) == -1) return false;
		break;
	case ys::graphics::ShaderStage::CS:
		break;
	case ys::graphics::ShaderStage::All:
		break;
	case ys::graphics::ShaderStage::End:
		break;
	default:
		break;
	}

	return true;
}

GLuint ys::graphics::Shader::CreateVertexShader(std::wstring path)
{
	GLint result;
	GLchar errorLog[512];
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	std::ifstream file(L"..\\ShaderSource\\" + path + L"VS.glsl");
	std::stringstream buffer; buffer << file.rdbuf();
	auto tmp = buffer.str();
	auto vertexName = tmp.c_str();
	glShaderSource(vertexShader, 1, &vertexName, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		std::cerr << "ERROR: vertexShader 컴파일 실패\n" << errorLog << std::endl;
		return -1;
	}
	return vertexShader;
}

GLuint ys::graphics::Shader::CreateFragmentShader(std::wstring path)
{
	GLint result;
	GLchar errorLog[512];
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	std::ifstream file(L"..\\ShaderSource\\" + path + L"FS.glsl");
	std::stringstream buffer; buffer << file.rdbuf();
	auto tmp = buffer.str();
	auto fragmentName = tmp.c_str();
	glShaderSource(fragmentShader, 1, &fragmentName, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		std::cerr << "ERROR: fragmentShader 컴파일 실패\n" << errorLog << std::endl;
		return -1;
	}
	return fragmentShader;
}