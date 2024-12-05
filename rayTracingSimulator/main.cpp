#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include <string>

#include"..\\MyEngine_source\\ysImgui_Manager.h"
#include "..\\MyEngine_source\\YSapplication.h"
#include "..\\MyEngine_source\\ysInputManager.h"
#include "..\\MyEngine_source\\ysSceneManager.h"
#include <glad/glad.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_opengl3_loader.h"
#include "PlanetaryScene.h"
#include <ysResources.h>
#include <ysShader.h>

static float kWidth = 2320, kHight = 1080;

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
	if (!glfwInit())
	{ 
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	

	GLFWwindow* window = glfwCreateWindow(kWidth, kHight, "Gradient Example", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetWindowPos(window, 100, 100);
	glfwMakeContextCurrent(window);

	//initialize GLAD (func pointer)
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	HWND hWnd = glfwGetWin32Window(window);
	if (hWnd == NULL)
	{ 
		std::cerr << "Failed to get Win32 window handle" << std::endl;
		return -1;
	} 
	SetWindowTextA(hWnd, "Gradient Example");
	glfwSwapInterval(0);

	//init window
//set window handle
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<> uid(0);
	std::string winName{ "Gradient Example" };
	//HWND hWnd{};
	std::string winID;
	do
	{
		hWnd = nullptr;
		winID = winName + std::to_string(uid(rd));
		hWnd = FindWindowA(NULL, winID.c_str());
	} while (hWnd); // 이거 순서 조금 바꿨어요 아래에서 해줌 - 대원


	ys::Imgui_Manager::Get_Imgui_Manager()->Init(window);


	glGetError();
	// Clear any previous errors 
	const GLubyte* version = glGetString(GL_VERSION);
	if (version == NULL)
	{
		std::cerr << "Failed to get OpenGL version" << std::endl;
		GLenum err = glGetError();
		std::cerr << "OpenGL error occurred: " << err << std::endl;
		return -1; 
	}

	ys::Resources::Load<ys::graphics::Shader>(L"vc", L"vc");

	//shader(glsl) initialize
	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
	app.Init(hWnd, window, RECT(0, 0, kWidth, kHight), false);
	glViewport(0, 0, kWidth, kHight);

	//set Scene
	ys::SceneManager::CreateScene<ys::PlanetaryScene>(std::wstring(L"mainScene"));// issue
	ys::SceneManager::LoadScene(L"mainScene");

	//set callback
	glfwSetFramebufferSizeCallback(window, FramebufferCallback);

	app.setScreen(RECT(0, 0, kWidth, kHight));
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, CursorPositionCallback);
	glfwSetMouseButtonCallback(window, MouseButtonCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

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

std::string readFile(const std::string& path)
{
	std::ifstream file(path);
	std::stringstream buffer; buffer << file.rdbuf();
	return buffer.str();
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods); // aka. On GUI textBox -> On Flag
	if (!ImGui::GetIO().WantCaptureKeyboard) // And Down Flag
	{ 
		// ImGui가 입력을 캡처하지 않을 때 애플리케이션 입력 처리 
		if (65 <= key && key <= 90) 
		{
			if ((mods & GLFW_MOD_CAPS_LOCK))//TODO: ON CAPS_LOCK -> SHIFT FLAG REVERSE		
			{
				mods ^= GLFW_MOD_SHIFT;
			}
			if (!(mods & GLFW_MOD_SHIFT))
			{
				key += 32;
			}
		}
		std::cout << key << " " << (char)key << " " << action << ' ' << std::bitset<8>(mods) << std::endl;
		ys::InputManager::setKeyState(key, action & GLFW_REPEAT, action == GLFW_RELEASE);
	}
}

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
	if (!ImGui::GetIO().WantCaptureMouse) 
	{ 
		// ImGui가 입력을 캡처하지 않을 때 애플리케이션 입력 처리
		std::cout << "Mouse: " << std::string((button == 0) ? "Left" : (button == 1) ? "Right" : (button == 2) ? "Middle" : std::to_string(button)) << ", Pos: " << ys::InputManager::getMousePosition().x << ", " << ys::InputManager::getMousePosition().y 
			<< " " << action << std::endl;
		ys::InputManager::setKeyState(button + 0x80, action & GLFW_REPEAT, action == GLFW_RELEASE); 
	}
}
void CursorPositionCallback(GLFWwindow* window, double xPos, double yPos)
{
	ImGui_ImplGlfw_CursorPosCallback(window, xPos, yPos);
	if (!ImGui::GetIO().WantCaptureMouse) 
	{
		// ImGui가 입력을 캡처하지 않을 때 애플리케이션 입력 처리
		ys::InputManager::setMousePosition(xPos, yPos);
	}
}

void FramebufferCallback(GLFWwindow* window, int w, int h)
{
	kWidth = w; kHight = h;
	app.setScreen(RECT(0, 0, kWidth, kHight));
}