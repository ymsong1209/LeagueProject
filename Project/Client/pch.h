#pragma once

#define WIN32_LEAN_AND_MEAN // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.

#include "framework.h"

#include <Engine\global.h>
#include <Engine\CEngine.h>


// Engine Library
#ifdef _DEBUG
#pragma comment(lib, "Engine//Engine_d")
#else
#pragma comment(lib, "Engine//Engine")
#endif


// Script Library
#ifdef _DEBUG
#pragma comment(lib, "Script//Script_d")
#else
#pragma comment(lib, "Script//Script")
#endif


#ifdef _DEBUG
#pragma comment(lib, "Debug\\ServerCore.lib")
#else
#pragma comment(lib, "Release\\ServerCore.lib")
#endif

#include "CorePch.h"