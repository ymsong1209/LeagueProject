#include "pch.h"
#include "CChampionAttackState.h"
#include <Engine/CFsm.h>
#include <Engine/CAnimator3D.h>
#include <Engine/CAnim3D.h>

#include "CChampionScript.h"
#include "CSkill.h"
#include "CSendServerEventMgr.h"

CChampionAttackState::CChampionAttackState()
	: m_UserObj(nullptr)
	, m_TargetObj(nullptr)
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
	/*
	// ==== 서버에게 평타 이벤트 Send  ==== //
	SkillInfo* skillInfo = new SkillInfo;
	//skillInfo->SkillId = 아직 알 수 없음
	skillInfo->OwnerId = m_iUserObj->GetScript<CUnitScript>()->GetServerID();	 // 본인 
	skillInfo->TargetId = m_iTargetObj->GetScript<CUnitScript>()->GetServerID(); //	피격자
	skillInfo->SkillLevel = 1;	// 평타라서 의미 없음, 무조건 1 
	//skillInfo->SkillLevel = m_iUserObj->GetScript<CChampionScript>()->GetSkillLevel(0);
	skillInfo->skillType = SkillType::BASIC_ATTACK;
	//skillInfo->offsetPos = Vec3(0,0,0); 추후 Bone 추적기능 활성시 하위 state에서 변경 

	tServerEvent serverEvn = {};
	serverEvn.Type = SERVER_EVENT_TYPE::SKILL_PROJECTILE_PACKET;
	serverEvn.wParam = (DWORD_PTR)skillInfo;
	//serverEvn.lParam 
	CSendServerEventMgr::GetInst()->AddServerSendEvent(serverEvn);
	*/
	CSkill* BasicAttack = GetOwner()->GetScript<CChampionScript>()->GetSkill(0);
	BasicAttack->SetUserObj(m_UserObj);
	BasicAttack->SetTargetObj(m_TargetObj);
	BasicAttack->SetOwnerScript(GetOwner()->GetScript<CChampionScript>());
	
	BasicAttack->Use();

	// 서버에게 기본 공격 사용한다는 신호 보냄
	//CSendServerEventMgr::GetInst()->SendUseSkillPacket(
	//	m_UserObj->GetScript<CUnitScript>()->GetServerID(),
	//	m_TargetObj->GetScript<CUnitScript>()->GetServerID(),
	//	1,
	//	SkillType::BASIC_ATTACK, 
	//	Vec3(0, 0, 0), 
	//	false,
	//	Vec3(0, 0, 0),
	//	false,
	//	Vec3(0, 0, 0));

	CUnitState::Enter();
}

void CChampionAttackState::Exit()
{
	// 변수 초기화
	m_UserObj = nullptr;
	m_TargetObj = nullptr;

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
