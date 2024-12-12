#include "ysShader.h"
#include <fstream>
#include <sstream>
#include <iostream>

ys::graphics::Shader::Shader()
	: Resource(enums::ResourceType::Shader), isComputeShader(false)
{
}

ys::graphics::Shader::~Shader()
{
	glDeleteProgram(shaderID);
}

HRESULT ys::graphics::Shader::Load(const std::wstring& path)
{
	std::wistringstream stream(path);
	std::wstring fileName;
	std::getline(stream, fileName, L',');

	shaderID = glCreateProgram();

	std::wstring stage;
	while (std::getline(stream, stage, L','))
		if (!Create(stage, fileName)) return S_FALSE;

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

bool ys::graphics::Shader::Create(const std::wstring stage, const std::wstring& fileName)
{
	if (stage == L"VS")
		if (CreateVertexShader(fileName) == -1)
			return false;
		else
			glAttachShader(shaderID, vertexShader);
	else if (stage == L"HS")
		;
	else if (stage == L"GS")
		;
	else if (stage == L"DS")
		;
	else if (stage == L"FS")
		if (CreateFragmentShader(fileName) == -1)
			return false;
		else
			glAttachShader(shaderID, fragmentShader);
	else if (stage == L"CS")
		if (CreateComputeShader(fileName) == -1)
			return false;
		else
			glAttachShader(shaderID, computeShader);
	else if (stage == L"ALL")
		;
	else
		return false;

	return true;
}

GLuint ys::graphics::Shader::CreateVertexShader(std::wstring path)
{
	GLint result;
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
		GLchar errorLog[512];
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		std::cerr << "ERROR: vertexShader 컴파일 실패\n" << errorLog << std::endl;
		return -1;
	}
	return vertexShader;
}

GLuint ys::graphics::Shader::CreateFragmentShader(std::wstring path)
{
	GLint result;
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
		GLchar errorLog[512];
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		std::cerr << "ERROR: fragmentShader 컴파일 실패\n" << errorLog << std::endl;
		return -1;
	}
	return fragmentShader;
}

GLuint ys::graphics::Shader::CreateComputeShader(std::wstring path)
{
	GLint result;
	isComputeShader = true;
	computeShader = glCreateShader(GL_COMPUTE_SHADER);
	std::ifstream file(L"..\\ShaderSource\\" + path + L"CS.glsl");
	std::stringstream buffer; buffer << file.rdbuf();
	auto tmp = buffer.str();
	auto computeName = tmp.c_str();
	glShaderSource(computeShader, 1, &computeName, NULL);
	glCompileShader(computeShader);
	glGetShaderiv(computeShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		GLchar errorLog[512];
		glGetShaderInfoLog(computeShader, 512, NULL, errorLog);
		std::cerr << "ERROR: computeShader 컴파일 실패\n" << errorLog << std::endl;
		return -1;
	}
	glGenBuffers(1, &dispatchIndirectBuffer);
	glBindBuffer(GL_DISPATCH_INDIRECT_BUFFER, dispatchIndirectBuffer);

	GLuint dispatchParams[9] = { 8, 8, 1, 16, 16, 1, 32, 32, 1 };
	glBufferData(GL_DISPATCH_INDIRECT_BUFFER, sizeof(dispatchParams), dispatchParams, GL_DYNAMIC_DRAW);

	return computeShader;
}