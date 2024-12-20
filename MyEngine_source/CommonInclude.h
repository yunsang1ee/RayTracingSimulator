#pragma once
#include <Windows.h>

#include <glad/glad.h>
//#include <gl/freeglut.h>
//#include <gl/freeglut_ext.h>
//#include <gl/freeglut_std.h>

#include <GLFW/glfw3.h>
#ifdef _WIN32
#ifndef GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#endif
#include <GLFW/glfw3native.h>
#endif
#pragma comment(lib, "glfw3.lib")


#include <vector>
#include <string>
#include <map>
#include <bitset>
#include <utility>
#include <functional>

#include <random>

#include "ysEnums.h"
#include <glm/glm.hpp>
#include <glm/fwd.hpp>
#include <glm/ext.hpp>






//#ifdef _DEBUG
//#pragma comment(lib, "..\\FMOD_SoundSystem\\lib\\Debug\\fmodL_vc.lib")
//#pragma comment(lib, "..\\FMOD_SoundSystem\\lib\\Debug\\fmodstudioL_vc.lib")
//#else
//#pragma comment(lib, "..\\FMOD_SoundSystem\\lib\\Release\\fmod_vc.lib")
//#pragma comment(lib, "..\\FMOD_SoundSystem\\lib\\Release\\fmodstudio_vc.lib")
//#endif // _DEBUG
//#include "..\\FMOD_SoundSystem\\inc\\fmod_studio.hpp"
//#include "..\\FMOD_SoundSystem\\inc\\fmod.hpp"
//#include "..\\FMOD_SoundSystem\\inc\\fmod_common.h"
//#include "..\\FMOD_SoundSystem\\inc\\fmod_codec.h"

#include <mmsystem.h>
#include <dinput.h>
#pragma comment(lib, "Msimg32.lib")
#pragma comment(lib, "winmm.lib")

#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")