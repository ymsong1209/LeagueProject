#include "pch.h"
#include "CChampionAttackState.h"
#include <Engine/CFsm.h>
#include <Engine/CAnimator3D.h>
#include <Engine/CAnim3D.h>

#include "CChampionScript.h"
#include "CSkill.h"
#include "CSendServerEventMgr.h"

CChampionAttackState::CChampionAttackState()
	: m_iUserObj(nullptr)
	, m_iTargetObj(nullptr)
{
}

CChampionAttackState::~CChampionAttackState()
{
}

void CChampionAttackState::tick()
{
	// 애니메이션이 끝날 경우, Idle로 전환
	if(GetOwnerFSM()->GetOwner()->Animator3D()->GetCurAnim()->IsFinish())
		GetOwnerFSM()->ChangeState(L"Idle");
}

void CChampionAttackState::Enter()
{
	// ==== 서버에게 평타 이벤트 Send  ==== //
	SkillInfo* skillInfo = new SkillInfo;
	skillInfo->OwnerId = GetOwnerFSM()->GetOwner()->GetScript<CUnitScript>()->GetServerID();
	skillInfo->TargetId = m_iTargetObj->GetScript<CUnitScript>()->GetServerID();
	skillInfo->SkillLevel = 1;
	skillInfo->skillType = SkillType::BASIC_ATTACK;

	tServerEvent serverEvn = {};
	serverEvn.Type = SERVER_EVENT_TYPE::SKILL_PROJECTILE_PACKET;
	serverEvn.wParam = (DWORD_PTR)skillInfo;
	//serverEvn.lParam 
	CSendServerEventMgr::GetInst()->AddServerSendEvent(serverEvn);

	// 서버 없을 때 호출할 코드
	//CChampionScript* ChampScript = GetOwnerFSM()->GetOwner()->GetScript<CChampionScript>();
	//CSkill* BaseAttack = ChampScript->GetSkill(0);
	//
	//BaseAttack->SetUserID(m_iUserID);
	//BaseAttack->SetTargetID(m_iTargetID);
	//
	//if (BaseAttack) 
	//	BaseAttack->Use();

	CUnitState::Enter();
}

void CChampionAttackState::Exit()
{
	// 변수 초기화
	m_iUserObj = nullptr;
	m_iTargetObj = nullptr;

	CUnitState::Exit();
}

void CChampionAttackState::HandleEvent(CGameEvent& event)
{
	if (!IsActive())
		return;

	switch (event.GetType())
	{
	case GAME_EVENT_TYPE::PLAYER_DEATH:
		GetOwnerFSM()->ChangeState(L"Death");
		break;
	}
}
