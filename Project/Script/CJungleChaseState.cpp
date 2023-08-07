#include "pch.h"
#include "CJungleChaseState.h"
#include "CJungleMonsterScript.h"

CJungleChaseState::CJungleChaseState()
	:m_fTime(0.f)
{
}

CJungleChaseState::~CJungleChaseState()
{
}


void CJungleChaseState::Enter()
{

}

void CJungleChaseState::tick()
{
	CJungleMonsterScript* script = GetOwner()->GetScript<CJungleMonsterScript>();
	//Ÿ���� ���� �ȵǾ��µ� chasestate�� ����
	if (script->GetTarget() == nullptr)
		assert(nullptr);

	Vec3 targetpos = script->GetTarget()->Transform()->GetRelativePos();
	Vec3 CurPos = GetOwner()->Transform()->GetRelativePos();
	float AttackRange = script->GetAttackRange();
	float distance = sqrt(pow(targetpos.x - CurPos.x, 2.f) + pow(targetpos.z - CurPos.z, 2.f));

	//���� ��Ÿ� ���� ����
	if (distance < AttackRange) {
		GetOwner()->Fsm()->ChangeState(L"Attack");
	}
	else{
		m_fTime += DT;
		if (m_fTime > 0.01f) {
			GetOwner()->PathFinder()->FindPath(targetpos);
			m_fTime = 0.f;
		}
		script->PathFindMove(70.f, true);
	}

}

void CJungleChaseState::Exit()
{
	GetOwner()->Animator3D()->Reset();
}

void CJungleChaseState::HandleEvent(CGameEvent& event)
{
	if (!IsActive())
		return;

	if (event.GetType() == GAME_EVENT_TYPE::GET_HIT) {
		GetHitEvent* HitEvent = dynamic_cast<GetHitEvent*>(&event);

		// ���� Ÿ���� ������ ��쿡�� �̺�Ʈ�� ����
		if (HitEvent->GetTargetObj() == GetOwner())
		{
			CGameObject* SkillUser = HitEvent->GetUserObj();
			CGameObject* SkillTarget = HitEvent->GetTargetObj();
			SkillType skilltype = HitEvent->GetSkillType();
			int	skillLevel = HitEvent->GetSkillLevel();

			GetOwnerFSM()->GetOwner()->GetScript<CUnitScript>()->GetHit(skilltype, SkillTarget, SkillUser, skillLevel);
		}
	}

}
