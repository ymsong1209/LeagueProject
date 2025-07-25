#include "pch.h"
#include "global.h"

tTransform g_transform = {};
tGlobal GlobalData = {};



extern const char* RES_TYPE_STR[(UINT)RES_TYPE::END] =
{
	"MESHDATA",
	"MATERIAL",
	"PREFAB",
	"MESH",
	"TEXTURE",
	"SOUND",
	"GRAPHICS_SHADER",
	"COMPUTE_SHADER"
};

extern const wchar_t* RES_TYPE_WSTR[(UINT)RES_TYPE::END] =
{
	L"MESHDATA",
	L"MATERIAL",
	L"PREFAB",
	L"MESH",
	L"TEXTURE",
	L"SOUND",
	L"GRAPHICS_SHADER",
	L"COMPUTE_SHADER"
};



extern const char* COMPONENT_TYPE_STR[(UINT)COMPONENT_TYPE::END] =
{
	"TRANSFORM",
	"COLLIDER2D",
	"COLLIDER3D",
	"ANIMATOR2D",
	"ANIMATOR3D",
	"LIGHT2D",
	"LIGHT3D",
	"CAMERA",
	"FSM",
	"PATHFINDER",
	"MESHRENDER",
	"PARTICLESYSTEM",
	"TILEMAP",
	"SKYBOX",
	"LANDSCAPE",
	"DECAL",
};

extern const wchar_t* COMPONENT_TYPE_WSTR[(UINT)COMPONENT_TYPE::END] =
{
	L"TRANSFORM",
	L"COLLIDER2D",
	L"COLLIDER3D",
	L"ANIMATOR2D",
	L"ANIMATOR3D",
	L"LIGHT2D",
	L"LIGHT3D",
	L"CAMERA",
	L"FSM",
	L"PATHFINDER",
	L"MESHRENDER",
	L"PARTICLESYSTEM",
	L"TILEMAP",
	L"SKYBOX",
	L"LANDSCAPE",
	L"DECAL",
};


extern const wchar_t* SKILL_TYPE_WSTR[(UINT)SkillNum::END] =
{
	L"Q",
	L"W",
	L"E",
	L"R",
	L"PASSIVE",
};



