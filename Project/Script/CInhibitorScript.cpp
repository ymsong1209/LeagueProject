#include "pch.h"
#include "CInhibitorScript.h"

CInhibitorScript::CInhibitorScript()
	:CStructureScript((UINT)SCRIPT_TYPE::INHIBITORSCRIPT)
{
}

CInhibitorScript::~CInhibitorScript()
{
}

void CInhibitorScript::begin()
{
	CStructureScript::begin();

	// FSM
	if (GetOwner()->Fsm() == nullptr)
		return;
	GetOwner()->Fsm()->ChangeState(L"Spawn");
}

void CInhibitorScript::tick()
{
}

void CInhibitorScript::CheckStatus()
{
	// 공격 가능한지 여부 체크

	// 포탑 레이어에서 모든 포탑 오브젝트를 가져와서
	// 본인 Lane의 모든 포탑이 Dead 상태인지 확인
}
