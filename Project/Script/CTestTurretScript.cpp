#include "pch.h"
#include "CTestTurretScript.h"

#include <Engine/CKeyMgr.h>
#include <Engine/CLevelMgr.h>

CTestTurretScript::CTestTurretScript()
	: CScript((UINT)SCRIPT_TYPE::TESTTURRETSCRIPT)
{
}

CTestTurretScript::~CTestTurretScript()
{
}


void CTestTurretScript::tick()
{

	if (CKeyMgr::GetInst()->GetKeyState(KEY::F) == KEY_STATE::TAP)
	{
		CGameObject* Blaze = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"CircleFast");

		CGameObject* CloneObj = Blaze->Clone();

		SpawnGameObject(CloneObj, Vec3(0.f, 0.f, 0.f), 0);
	}
}

void CTestTurretScript::begin()
{
}

