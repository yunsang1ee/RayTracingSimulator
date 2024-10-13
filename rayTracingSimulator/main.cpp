#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include <string>
#include "..\\MyEngine_source\\YSapplication.h"
#include "..\\MyEngine_source\\ysInputManager.h"
#include "..\\MyEngine_source\\ysSceneManager.h"
#include <glad/glad.h>

static float kWidth = 800, kHight = 800;
static std::string vertexPath = "vertex.glsl";
static std::string fragmentPath = "fragment.glsl";

ys::Application app;

unsigned int CreateShader(std::string vertexPath, std::string fragmentPath);
std::string readFile(const std::string&);

void FramebufferCallback(GLFWwindow* window, int w, int h);
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void CursorPositionCallback(GLFWwindow* window, double xPos, double yPos);

#define CURRENT_SCENE ys::ShaderScene

int main(int argc, char** argv)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(919);
	
	//initialize GLFW(window utility)
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//init window
	//set window handle
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<> uid(0);
	std::string winName{ "Gradient Example" };
	HWND hWnd{};
	std::string winID;
	do
	{
		hWnd = nullptr;
		winID = winName + std::to_string(uid(rd));
		hWnd = FindWindowA(NULL, winID.c_str());
	} while (hWnd);

	GLFWwindow* window = glfwCreateWindow(kWidth, kHight, "Gradient Example", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetWindowPos(window, 600, 100);
	glfwMakeContextCurrent(window);
	hWnd = FindWindowA(NULL, winID.c_str());
	SetWindowTextA(hWnd, winName.c_str());
	glfwSwapInterval(0);

	//initialize GLAD (func pointer)
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	//shader(glsl) initialize
	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

	auto shaderID = CreateShader(vertexPath, fragmentPath);
	if (shaderID == -1)	return -1;
	app.Init(hWnd, window, shaderID, RECT(0, 0, kWidth, kHight), false);
	glViewport(0, 0, kWidth, kHight);

	//set Scene
	ys::SceneManager::CreateScene<ys::Scene>(std::wstring(L"mainScene"));// issue
	ys::SceneManager::LoadScene(L"mainScene");

	//set callback
	glfwSetFramebufferSizeCallback(window, FramebufferCallback);

	app.setScreen(RECT(0, 0, kWidth, kHight));
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, CursorPositionCallback);
	glfwSetMouseButtonCallback(window, MouseButtonCallback);

	//loop func
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		app.Run();
	}
	app.Release();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

unsigned int CreateShader(std::string vertexPath, std::string fragmentPath)
{
	GLint result;
	GLchar errorLog[512];
	auto vertexShader = glCreateShader(GL_VERTEX_SHADER);
	auto tmp = readFile(vertexPath);
	auto vertexName = tmp.c_str();
	glShaderSource(vertexShader, 1, &vertexName, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		std::cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << std::endl;
		return -1;
	}

	auto fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	tmp = readFile(fragmentPath);
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

	auto shaderID = glCreateProgram();
	glAttachShader(shaderID, vertexShader);
	glAttachShader(shaderID, fragmentShader);

	glLinkProgram(shaderID);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderID, 512, NULL, errorLog);
		std::cerr << "ERROR: shader program 연결 실패\n" << errorLog << std::endl;
		return -1;
	}

	glUseProgram(shaderID);
	return shaderID;
}

std::string readFile(const std::string& path)
{
	std::ifstream file(path);
	std::stringstream buffer; buffer << file.rdbuf();
	return buffer.str();
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	std::cout << key << " " << (char)key << " " << action << std::endl;
	ys::InputManager::setKeyState(key, action & GLFW_REPEAT, action == GLFW_RELEASE);
}

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	std::cout << "Mouse: " << std::string((button == 0) ? "Left" : (button == 1) ? "Right" :
		(button == 2) ? "Middle" : std::to_string(button))
		<< ", Pos: " << ys::InputManager::getMousePosition().x << ", " << ys::InputManager::getMousePosition().y
		<< " " << action << std::endl;
	ys::InputManager::setKeyState(button + 0x80, action & GLFW_REPEAT, action == GLFW_RELEASE);
}
void CursorPositionCallback(GLFWwindow* window, double xPos, double yPos)
{
	ys::InputManager::setMousePosition(xPos, yPos);
}

void FramebufferCallback(GLFWwindow* window, int w, int h)
{
	kWidth = w; kHight = h;
	app.setScreen(RECT(0, 0, kWidth, kHight));
}