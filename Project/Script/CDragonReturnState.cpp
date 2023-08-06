#include "pch.h"
#include "CDragonReturnState.h"
#include "CDragonScript.h"
#include <Engine/CAnim3D.h>
CDragonReturnState::CDragonReturnState()
{
}

CDragonReturnState::~CDragonReturnState()
{
}


void CDragonReturnState::Enter()
{
	CDragonScript* script = GetOwner()->GetScript<CDragonScript>();
	Vec3 SpawnPos = script->GetSpawnPos();
	GetOwner()->PathFinder()->FindPath(SpawnPos);
	float MaxHP = script->GetMaxHP();
	script->SetCurHP(MaxHP);
}

void CDragonReturnState::tick()
{

	CDragonScript* script = GetOwner()->GetScript<CDragonScript>();

	float MaxHP = script->GetMaxHP();
	script->SetCurHP(MaxHP);

	Vec3 targetpos = script->GetSpawnPos();
	Vec3 CurPos = GetOwner()->Transform()->GetRelativePos();
	float distance = sqrt(pow(targetpos.x - CurPos.x, 2.f) + pow(targetpos.z - CurPos.z, 2.f));

	//Spawn���� ��ó�� ��
	if (distance < 3.f) {
		GetOwner()->Fsm()->ChangeState(L"Land");
	}
	else {
		//GetOwner()->PathFinder()->FindPath(targetpos);
		script->PathFindMove(75.f, true);
	}
}

void CDragonReturnState::Exit()
{
	CDragonScript* script = GetOwner()->GetScript<CDragonScript>();
	GetOwner()->Transform()->SetRelativePos(script->GetSpawnPos());
	GetOwner()->Transform()->SetRelativeRot(script->GetSpawnRot());
	GetOwner()->Animator3D()->GetCurAnim()->Reset();
}

void CDragonReturnState::HandleEvent(CGameEvent& event)
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
