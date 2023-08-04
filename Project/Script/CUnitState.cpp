#include "pch.h"
#include "CUnitState.h"

CUnitState::CUnitState()
	: m_Listener(new CUnitScriptEventListener(this))
{
	CGameEventMgr::GetInst()->AddListener(m_Listener);
}


CUnitState::~CUnitState()
{
}

void CUnitState::tick()
{
}

void CUnitState::Enter()
{
	CState::Enter();
}

void CUnitState::Exit()
{
	CState::Exit();
}
