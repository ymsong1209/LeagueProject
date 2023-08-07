#include "pch.h"
#include "CJungleDeathState.h"
#include <Engine/CAnim3D.h>
#include "CJungleMonsterScript.h"
CJungleDeathState::CJungleDeathState()
	:m_bPacketSend(false)
{
}

CJungleDeathState::~CJungleDeathState()
{
}

void CJungleDeathState::Enter()
{
	CUnitState::Enter();
	CJungleMonsterScript* script = GetOwner()->GetScript<CJungleMonsterScript>();
	script->SetUnitDead(true);
}

void CJungleDeathState::tick()
{
	if (GetOwner()->Animator3D()->GetCurAnim()->IsFinish() && m_bPacketSend == false) {
		CJungleMonsterScript* script = GetOwner()->GetScript<CJungleMonsterScript>();
		CSendServerEventMgr::GetInst()->SendDespawnPacket(script->GetServerID(), 0.f);
		m_bPacketSend = true;
	}
}

void CJungleDeathState::Exit()
{
	CUnitState::Exit();
}


