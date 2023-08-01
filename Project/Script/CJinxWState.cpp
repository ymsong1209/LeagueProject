#include "pch.h"
#include "CJinxWState.h"
#include "CChampionScript.h"

CJinxWState::CJinxWState()
{
	SetName(L"W");
}

CJinxWState::~CJinxWState()
{
}

void CJinxWState::tick()
{
	CChampionSkillState::tick();
}

void CJinxWState::Enter()
{
	// 징크스 w 쓰겠다고 서버에 요청
	SkillInfo* skillInfo = new SkillInfo;
	skillInfo->OwnerId = GetOwner()->GetScript<CChampionScript>()->GetServerID();
	skillInfo->TargetId = -1;
	skillInfo->SkillLevel = GetOwner()->GetScript<CChampionScript>()->GetSkillLevel(2);
	skillInfo->skillType = SkillType::JINX_W;

	tServerEvent serverEvn = {};
	serverEvn.Type = SERVER_EVENT_TYPE::SKILL_PROJECTILE_PACKET;
	serverEvn.wParam = (DWORD_PTR)skillInfo;
	//serverEvn.lParam 
	CSendServerEventMgr::GetInst()->AddServerSendEvent(serverEvn);


	CChampionSkillState::Enter();
}

void CJinxWState::Exit()
{
	CChampionSkillState::Exit();
}

void CJinxWState::HandleEvent(CGameEvent& event)
{
}
