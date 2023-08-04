#include "pch.h"
#include "CJungleIdleState.h"
#include "CJungleMonsterScript.h"
#include <Engine/CAnim3D.h>

CJungleIdleState::CJungleIdleState()
{
}

CJungleIdleState::~CJungleIdleState()
{
}

void CJungleIdleState::Enter()
{
}

void CJungleIdleState::tick()
{
	CJungleMonsterScript* script = GetOwner()->GetScript<CJungleMonsterScript>();
	const vector<CGameObject*>& Champions = CLevelMgr::GetInst()->GetCurLevel()->FindLayerByName(L"Champion")->GetObjects();
	Vec3 MonAggroPos = script->GetAggroPos();
	float AggroRadius = script->GetAggroRange();
	for (const CGameObject* Champ : Champions) {
		Vec3 ChampPos = Champ->Transform()->GetRelativePos();
		float distance = sqrt(pow(ChampPos.x - MonAggroPos.x, 2.f) + pow(ChampPos.z - MonAggroPos.z, 2.f));
		if (distance < AggroRadius) {
			GetOwner()->Fsm()->ChangeState(L"N2A");
			break;
		}
	}
}

void CJungleIdleState::Exit()
{
	GetOwner()->Animator3D()->GetCurAnim()->Reset();
}


