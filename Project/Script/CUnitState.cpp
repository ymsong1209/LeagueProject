#include "pch.h"
#include "CUnitState.h"
#include <Engine\CFsm.h>
#include <Engine\CRenderMgr.h>
#include "CUnitScript.h"
#include "CSendServerEventMgr.h"
#include "CChampionScript.h"

CUnitState::CUnitState()
	: m_Listener(new CUnitScriptEventListener(this))
{
	CGameEventMgr::GetInst()->AddListener(m_Listener);
}

CUnitState::CUnitState(const CUnitState& other)
		: CState(other)
		, m_Listener(new CUnitScriptEventListener(*other.m_Listener))
{
		m_Listener->m_state = this;
}

CUnitState::~CUnitState()
{
	CGameEventMgr::GetInst()->RemoveListener(m_Listener);
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


