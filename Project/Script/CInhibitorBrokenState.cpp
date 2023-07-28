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
	// RespawnTime�� ������ Spawn ���·� ��ȯ
	m_fRespawnTime -= DT;

	if(m_fRespawnTime <=0)
		GetOwnerFSM()->ChangeState(L"Spawn");
}

void CInhibitorBrokenState::Enter()
{
	// �������� �ʿ�(destroy..)

	// RespawnTime  ����
	m_fRespawnTime = GetOwnerFSM()->GetOwner()->GetScript<CInhibitorScript>()
					->GetRespawnTime();
		
	CUnitState::Enter();
}

void CInhibitorBrokenState::Exit()
{
	CUnitState::Exit();
}
