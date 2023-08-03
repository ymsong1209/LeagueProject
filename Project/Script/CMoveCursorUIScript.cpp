#include "pch.h"
#include "CMoveCursorUIScript.h"

void CMoveCursorUIScript::begin()
{
	Vec4 funcValue = Vec4(-2, 0, 0, 0);
	MeshRender()->SetTexMovingStyle(MovTexType::OUTPUT, MovTexMoveType::VERTICAL);
	MeshRender()->SetFuncValue(MovTexType::OUTPUT, funcValue);
}

void CMoveCursorUIScript::tick()
{
	Time += DT;
	if (Time >= 0.3f)
	{
		DestroyObject(GetOwner());
	}
}

void CMoveCursorUIScript::BeginOverlap(CCollider2D* _Other)
{
}

CMoveCursorUIScript::CMoveCursorUIScript()
	:CScript((UINT)SCRIPT_TYPE::MOVECURSORUISCRIPT)
{
}

CMoveCursorUIScript::~CMoveCursorUIScript()
{
}
