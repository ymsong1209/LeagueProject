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
	// �ִϸ��̼��� ���� ���, Idle�� ��ȯ
	if(GetOwnerFSM()->GetOwner()->Animator3D()->GetCurAnim()->IsFinish())
		GetOwnerFSM()->ChangeState(L"Idle");
}

void CChampionAttackState::Enter()
{
	// ���� ����ü ���� (���� ��� ������ �����Ŀ��� ȣ��������ϴ°�)
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
	// ���� �ʱ�ȭ
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
