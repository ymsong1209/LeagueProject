#include "pch.h"
#include "CInhibitorBrokenState.h"
#include "CInhibitorScript.h"

#include <Engine/CAnim3D.h>
#include <Engine/CAnimator3D.h>
#include  <Engine/CFsm.h>
#include <Engine/CTimeMgr.h>

CInhibitorBrokenState::CInhibitorBrokenState()
	: m_fRespawnTime(0)
{
}

CInhibitorBrokenState::~CInhibitorBrokenState()
{
}

void CInhibitorBrokenState::tick()
{
	// RespawnTime이 지나면 Spawn 상태로 전환
	m_fRespawnTime -= DT;

	if(m_fRespawnTime <=0)
		GetOwnerFSM()->ChangeState(L"Spawn");
}

void CInhibitorBrokenState::Enter()
{
	// 재질설정 필요(destroy..)

	// RespawnTime  지정
	m_fRespawnTime = GetOwnerFSM()->GetOwner()->GetScript<CInhibitorScript>()
					->GetRespawnTime();
		
	CUnitState::Enter();
}

void CInhibitorBrokenState::Exit()
{
	CUnitState::Exit();
}
