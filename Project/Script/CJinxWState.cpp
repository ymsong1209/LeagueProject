#include "pch.h"
#include "CJinxWState.h"
#include "CChampionScript.h"
#include "CSkill.h"

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
	CChampionSkillState::Enter();
	// ¡ũ�� w ���ڴٰ� ������ ��û
	//SkillInfo* skillInfo = new SkillInfo();
	//skillInfo->OwnerId = GetOwner()->GetScript<CChampionScript>()->GetServerID();
	//skillInfo->TargetId = -1;
	//
	//skillInfo->UseMouseDir = true;
	//skillInfo->MouseDir.x = GetMouseDir().x;
	//skillInfo->MouseDir.y = GetMouseDir().y;
	//skillInfo->MouseDir.z = GetMouseDir().z;
	//
	//skillInfo->SkillLevel = 1;//GetOwner()->GetScript<CChampionScript>()->GetSkillLevel(2);
	//skillInfo->skillType = SkillType::JINX_W;
	//
	//tServerEvent serverEvn = {};
	//serverEvn.Type = SERVER_EVENT_TYPE::SKILL_PROJECTILE_PACKET;
	//serverEvn.wParam = (DWORD_PTR)skillInfo;
	////serverEvn.lParam 
	//CSendServerEventMgr::GetInst()->AddServerSendEvent(serverEvn);

	CSkill* JinxW = GetOwner()->GetScript<CChampionScript>()->GetSkill(2);
	JinxW->SetUserObj(m_UserObj);
	JinxW->SetTargetObj(m_TargetObj);
	JinxW->SetOwnerScript(GetOwner()->GetScript<CChampionScript>());

	JinxW->Use();

	
}

void CJinxWState::Exit()
{
	CChampionSkillState::Exit();
}

void CJinxWState::HandleEvent(CGameEvent& event)
{
}
