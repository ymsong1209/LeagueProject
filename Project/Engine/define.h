#pragma once


#define DEVICE  CDevice::GetInst()->GetDevice()
#define CONTEXT CDevice::GetInst()->GetDeviceContext()

#define CLONE(type) public: virtual type* Clone() { return new type(*this); }
#define CLONE_DISABLE(type) public: virtual type* Clone() { return nullptr; assert(nullptr); }

#define KEY_TAP(Key) CKeyMgr::GetInst()->GetKeyState(Key) == KEY_STATE::TAP		
#define KEY_RELEASE(Key) CKeyMgr::GetInst()->GetKeyState(Key) == KEY_STATE::RELEASE
#define KEY_PRESSED(Key) CKeyMgr::GetInst()->GetKeyState(Key) == KEY_STATE::PRESSED
#define KEY_NONE(Key) CKeyMgr::GetInst()->GetKeyState(Key) == KEY_STATE::NONE

#define DT CTimeMgr::GetInst()->GetDeltaTime()
#define EditorDT CTimeMgr::GetInst()->GetEditorDeltaTime()

#define CUR_LEVEL CLevelMgr::GetInst()->GetCurLevel()
#define UICamera CRenderMgr::GetInst()->GetCamerafromIdx(1)

#define MAX_LAYER 32
#define NaN std::numeric_limits<float>::quiet_NaN()

#define SINGLE(type) private: type(); ~type(); friend class CSingleton<type>;
#define FONT_RGBA(r, g, b, a) (((((BYTE)a << 24 ) | (BYTE)b << 16) | (BYTE)g << 8) | (BYTE)r)

enum class MRT_TYPE
{
	SWAPCHAIN,
	DEFERRED,
	DECAL,
	LIGHT,
	SHADOW,
	FOGOFWAR,
	END,
};


enum class COMPONENT_TYPE
{
	// update
	TRANSFORM,		// 위치, 크기, 회전
	COLLIDER2D,		// 2차원 충돌
	COLLIDER3D,		// 3차원 충돌
	ANIMATOR2D,		// Sprite Animation
	ANIMATOR3D,		// Bone Sknning Animation
	LIGHT2D,		// 2차원 광원
	LIGHT3D,		// 3차원 광원
	CAMERA,			// Camera
	FSM,
	PATHFINDER,		// 길찾기

	// render
	MESHRENDER,		// 기본적인 렌더링
	PARTICLESYSTEM, // 입자 렌더링
	TILEMAP,		// 2차원 타일
	SKYBOX,			// 하늘
	LANDSCAPE,		// 3차원 지형
	DECAL,			// 내부 렌더링

	END,

	// custom
	SCRIPT,
};

extern const char* COMPONENT_TYPE_STR[(UINT)COMPONENT_TYPE::END];
extern const wchar_t* COMPONENT_TYPE_WSTR[(UINT)COMPONENT_TYPE::END];


enum class RES_TYPE
{
	MESHDATA,
	MATERIAL,
	PREFAB,

	MESH,			// 형태
	TEXTURE,		// 이미지
	SOUND,

	GRAPHICS_SHADER,
	COMPUTE_SHADER,

	END,
};

extern const char* RES_TYPE_STR[(UINT)RES_TYPE::END];
extern const wchar_t* RES_TYPE_WSTR[(UINT)RES_TYPE::END];




enum class CB_TYPE
{
	TRANSFORM,	// b0
	MATERIAL,	// b1
	GLOBAL,		// b2
	MESHRENDER, // b3
	END,
};


enum SCALAR_PARAM
{
	INT_0,
	INT_1,
	INT_2,
	INT_3,
	INT_4,
	INT_5,
	INT_6,
	INT_7,
	INT_8,
	INT_9,
	INT_10,
	INT_11,

	FLOAT_0,
	FLOAT_1,
	FLOAT_2,
	FLOAT_3,
	FLOAT_4,
	FLOAT_5,
	FLOAT_6,
	FLOAT_7,
	FLOAT_8,
	FLOAT_9,
	FLOAT_10,
	FLOAT_11,

	VEC2_0,
	VEC2_1,
	VEC2_2,
	VEC2_3,
	VEC2_4,
	VEC2_5,
	VEC2_6,
	VEC2_7,
	VEC2_8,
	VEC2_9,
	VEC2_10,
	VEC2_11,

	VEC4_0,
	VEC4_1,
	VEC4_2,
	VEC4_3,
	VEC4_4,
	VEC4_5,
	VEC4_6,
	VEC4_7,
	VEC4_8,
	VEC4_9,
	VEC4_10,
	VEC4_11,

	MAT_0,
	MAT_1,
	MAT_2,
	MAT_3,
};

enum TEX_PARAM
{
	TEX_0,
	TEX_1,
	TEX_2,
	TEX_3,
	TEX_4,
	TEX_5,
	TEX_6,
	TEX_7,

	TEX_CUBE_0,
	TEX_CUBE_1,

	TEX_ARR_0,
	TEX_ARR_1,

	TEX_END,
};

enum MESH_TEX_PARAM
{
	PUNCTURE,
	ADDITIVE,
	MESH_TEX_END,
};

enum PIPELINE_STAGE
{
	PS_VERTEX = 0x01,
	PS_HULL = 0x02,
	PS_DOMAIN = 0x04,
	PS_GEOMETRY = 0x08,
	PS_PIXEL = 0x10,

	PS_ALL_STAGES = PS_VERTEX | PS_HULL | PS_DOMAIN | PS_GEOMETRY | PS_PIXEL,
};

enum class RS_TYPE
{
	CULL_BACK,
	CULL_FRONT,
	CULL_NONE,
	WIRE_FRAME,
	END,
};

enum class DS_TYPE
{
	LESS,
	LESS_EQUAL,
	GREATER,
	GREATER_EQUAL,
	NO_WRITE,			// LESS, DepthWrite X
	NO_TEST_NO_WRITE,	// Test X, DepthWrite X

	FRONT_CHECK,

	END,
};


enum class BS_TYPE
{
	DEFAULT,		// No Blending
	MASK,			// Alpha Coverage
	ALPHA_BLEND,	// Alpha 계수 
	ONE_ONE,		// 1:1 혼합
	END,
};





enum class DIR_TYPE
{
	RIGHT,
	UP,
	FRONT,
};

enum class PROJ_TYPE
{
	ORTHOGRAPHIC,
	PERSPECTIVE,
};

enum class SHADER_DOMAIN
{
	// Deferred
	DOMAIN_DEFERRED,
	DOMAIN_DECAL,

	// Light
	DOMAIN_LIGHT,

	// SwapChain(Foward)
	DOMAIN_OPAQUE,		// 불투명 오브젝트
	DOMAIN_MASK,		// 불투명, 투명
	DOMAIN_TRANSPARENT,	// 반투명
	DOMAIN_POSTPROCESS, // 후 처리
	DOMAIN_UI,
	DOMAIN_CONTOURPAINT, // 아웃라인 페인팅

	DOMAIN_UI_OPAQUE,
	DOMAIN_UI_MASK,
	DOMAIN_UI_TRANSPARENT,
	DOMAIN_UNDEFINED,	// 미정
};


enum class EVENT_TYPE
{
	CREATE_OBJECT,	// wParam : GameObject, lParam : Layer Index
	DELETE_OBJECT,  // wParam : GameObject

	ADD_CHILD,

	DELETE_RESOURCE,	// wParam : RES_TYPE, lParam : Resource Adress

	LEVEL_CHANGE,
	//스크립트 시점에서 오브젝트의 특정 정보를 변경해줄경우 인스펙터에 반영이 안됨 (스크립트에서 오브젝트 이름변경 등..)
	//그래서 이벤트매니저에 ui 리로드 이벤트 추가
	INSPECTOR_RELOAD,
	LAYER_CHANGE,		// wParam : 바꿀 오브젝트 lParam : 바꿀 레이어 번호
	EXCEPT_LAYER_UI,
};


enum class SHAPE_TYPE
{
	RECT,
	CIRCLE,
	CUBE,
	SPHERE,
	ICECREAM,
	FRUSTUM,
	END,
};


enum class COLLIDER2D_TYPE
{
	RECT,
	CIRCLE,
};

enum class COLLIDER3D_TYPE
{
	SPHERE,
	CUBE,
};


enum class LIGHT_TYPE
{
	DIRECTIONAL,
	POINT,
	SPOT,
};

enum class LEVEL_STATE
{
	PLAY,
	PAUSE,
	STOP,
};

enum class SB_TYPE
{
	READ_ONLY,
	READ_WRITE,
};

enum class PARTICLE_MODULE
{
	PARTICLE_SPAWN,
	COLOR_CHANGE,
	SCALE_CHANGE,
	ADD_VELOCITY,

	DRAG,
	NOISE_FORCE,
	RENDER,
	GRAVITY,
	END,
};

enum class LANDSCAPE_MOD
{
	HEIGHT_MAP,
	SPLAT,
	NONE,
};


enum class PLAYER_STATE // 징크스 이동 테스트용 Run, Idle 상태 추가 (나중에 fsm 도입 후 사라질 예정)
{
	RUN,
	IDLE,
};


//UI이미지용
enum class SkillNum
{
	Q,
	W,
	E,
	R,
	PASSIVE,
	END,
}; //이 이름을 조합하여 자동으로 머터리얼 찾아오므로 함부로 변경 x!!

extern const wchar_t* SKILL_TYPE_WSTR[(UINT)SkillNum::END];

enum class BARTYPE
{
	HP,
	MP,
	END,
};



enum class FONT_TYPE
{
	RIX_KOR_M,
	RIX_KOR_L,
	RIX_KOR_B,
	RIX_KOR_EB,
};


enum class FONT_DOMAIN
{
	MAINCAM,
	OPAQE,
	MASK,
	TRANS,
};


enum class COOL_DOWN_TYPE
{
	Q,
	W,
	E,
	R,
	SPELL_D,
	SPELL_F,
	PASSIVE,
};