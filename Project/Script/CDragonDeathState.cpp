#include "pch.h"
#include "CDragonDeathState.h"
#include "CDragonScript.h"
#include <Engine/CAnim3D.h>
CDragonDeathState::CDragonDeathState()
	: m_bPacketSend(false)
{
}

CDragonDeathState::~CDragonDeathState()
{
}


void CDragonDeathState::Enter()
{
	CUnitState::Enter();
	m_bPacketSend = false;
	GetOwner()->Animator3D()->PlayOnce(L"Elder_Dragon\\Death", true, 0.5f);
	UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
	CSendServerEventMgr::GetInst()->SendAnimPacket(targetId, L"Elder_Dragon\\Death", false, false, true, 5.f);
	CDragonScript* script = GetOwner()->GetScript<CDragonScript>();
	script->SetUnitDead(true);
}

void CDragonDeathState::tick()
{
	if (GetOwner()->Animator3D()->GetCurAnim()->IsFinish() && m_bPacketSend == false) {
		CDragonScript* script = GetOwner()->GetScript<CDragonScript>();
		CSendServerEventMgr::GetInst()->SendDespawnPacket(script->GetServerID(), 0.f);
		m_bPacketSend = true;
	}
}

void CDragonDeathState::Exit()
{
	CUnitState::Exit();
}



