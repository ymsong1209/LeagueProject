#include "pch.h"
#include "CJungleDeathState.h"
#include <Engine/CAnim3D.h>
#include "CJungleMonsterScript.h"
CJungleDeathState::CJungleDeathState()
{
}

CJungleDeathState::~CJungleDeathState()
{
}

void CJungleDeathState::Enter()
{
	CJungleMonsterScript* script = GetOwner()->GetScript<CJungleMonsterScript>();
	script->SetUnitDead(true);
}

void CJungleDeathState::tick()
{
	if (GetOwner()->Animator3D()->GetCurAnim()->IsFinish()) {
		// 죽음 이벤트 서버에 쏴야함?
		//DestroyObject(GetOwner())
		// 
		//DeathEvent* evn = dynamic_cast<DeathEvent*>(CGameEventMgr::GetInst()->GetEvent((UINT)GAME_EVENT_TYPE::PLAYER_KILL_MOB));
		//if (evn != nullptr)
		//{
		//	CGameEventMgr::GetInst()->NotifyEvent(*evn);
		//}
	}
}

void CJungleDeathState::Exit()
{
}


