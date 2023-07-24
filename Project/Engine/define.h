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

#define MAX_LAYER 32
#define NaN std::numeric_limits<float>::quiet_NaN()

#define SINGLE(type) private: type(); ~type(); friend class CSingleton<type>;



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
	TRANSFORM,		// ��ġ, ũ��, ȸ��
	COLLIDER2D,		// 2���� �浹
	COLLIDER3D,		// 3���� �浹
	ANIMATOR2D,		// Sprite Animation
	ANIMATOR3D,		// Bone Sknning Animation
	LIGHT2D,		// 2���� ����
	LIGHT3D,		// 3���� ����
	CAMERA,			// Camera
	FSM,
	PATHFINDER,		// ��ã��

	// render
	MESHRENDER,		// �⺻���� ������
	PARTICLESYSTEM, // ���� ������
	TILEMAP,		// 2���� Ÿ��
	SKYBOX,			// �ϴ�
	LANDSCAPE,		// 3���� ����
	DECAL,			// ���� ������
		
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

	MESH,			// ����
	TEXTURE,		// �̹���
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
	ALPHA_BLEND,	// Alpha ��� 
	ONE_ONE,		// 1:1 ȥ��
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
	DOMAIN_OPAQUE,		// ������ ������Ʈ
	DOMAIN_MASK,		// ������, ����
	DOMAIN_TRANSPARENT,	// ������
	DOMAIN_POSTPROCESS, // �� ó��
	DOMAIN_UI,
	DOMAIN_CONTOURPAINT, // �ƿ����� ������
	DOMAIN_UNDEFINED,	// ����
};


enum class EVENT_TYPE
{
	CREATE_OBJECT,	// wParam : GameObject, lParam : Layer Index
	DELETE_OBJECT,  // wParam : GameObject

	ADD_CHILD,

	DELETE_RESOURCE,	// wParam : RES_TYPE, lParam : Resource Adress

	LEVEL_CHANGE,	
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


enum class PLAYER_STATE // ¡ũ�� �̵� �׽�Ʈ�� Run, Idle ���� �߰� (���߿� fsm ���� �� ����� ����)
{
	RUN,
	IDLE,
};