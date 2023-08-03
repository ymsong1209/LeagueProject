#include "pch.h"
#include "CJungleAlertState.h"
#include <Engine/CAnim3D.h>
#include "CJungleMonsterScript.h"
CJungleAlertState::CJungleAlertState()
	: m_bDetectChampion(true)
{
}

CJungleAlertState::~CJungleAlertState()
{
}

void CJungleAlertState::Enter()
{
	m_bDetectChampion = true;
}


void CJungleAlertState::tick()
{
	m_bDetectChampion = false;
	
	CJungleMonsterScript* script = GetOwner()->GetScript<CJungleMonsterScript>();
	const vector<CGameObject*>& Champions = CLevelMgr::GetInst()->GetCurLevel()->FindLayerByName(L"Champion")->GetObjects();
	Vec3 MonSpawnPos = script->GetSpawnPos();
	float AggroRadius = script->GetAggroRange();
	for (const CGameObject* Champ : Champions) {
		Vec3 ChampPos = Champ->Transform()->GetRelativePos();
		float distance = sqrt(pow(ChampPos.x - MonSpawnPos.x, 2.f) + pow(ChampPos.z - MonSpawnPos.z, 2.f));
		// 아직 어그로 범위에 챔피언이 하나라도 있음
		if (distance < AggroRadius) {
			m_bDetectChampion = true;
			break;
		}
	}
	//모든 champion이 어그로 범위 밖으로 나갔음
	if (m_bDetectChampion == false) {
		GetOwner()->Fsm()->ChangeState(L"A2N");
	}
}



void CJungleAlertState::Exit()
{
	GetOwner()->Animator3D()->GetCurAnim()->Reset();
	m_bDetectChampion = true;
}