#include "pch.h"
#include "CChampionAttackState.h"
#include <Engine/CFsm.h>
#include <Engine/CAnimator3D.h>
#include <Engine/CAnim3D.h>

#include "CChampionScript.h"
#include "CSkill.h"

CChampionAttackState::CChampionAttackState()
	: m_iUserID(-1)
	, m_iTargetID(-1)
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
	// 공격 투사체 생성 (여긴 사실 서버가 방장컴에만 호출해줘야하는것)
	CChampionScript* ChampScript = GetOwnerFSM()->GetOwner()->GetScript<CChampionScript>();
	CSkill* BaseAttack = ChampScript->GetSkill(0);
	
	BaseAttack->SetUserID(m_iUserID);
	BaseAttack->SetTargetID(m_iTargetID);
	
	if (BaseAttack) 
		BaseAttack->Use();

	CUnitState::Enter();
}

void CChampionAttackState::Exit()
{
	// 변수 초기화
	m_iUserID = -1;
	m_iTargetID = -1;

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
