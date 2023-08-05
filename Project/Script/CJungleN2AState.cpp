#include "pch.h"
#include "CJungleN2AState.h"
#include <Engine/CAnim3D.h>
#include "CJungleMonsterScript.h"
CJungleN2AState::CJungleN2AState()
	: m_bDetectChampion(true)
{
}

CJungleN2AState::~CJungleN2AState()
{
}

void CJungleN2AState::Enter()
{
	m_bDetectChampion = true;
}

void CJungleN2AState::tick()
{
	m_bDetectChampion = false;
	if (GetOwner()->Animator3D()->GetCurAnim()->IsFinish()) {
		GetOwner()->Fsm()->ChangeState(L"Alert");
		return;
	}

	CJungleMonsterScript* script = GetOwner()->GetScript<CJungleMonsterScript>();
	const vector<CGameObject*>& Champions = CLevelMgr::GetInst()->GetCurLevel()->FindLayerByName(L"Champion")->GetObjects();
	Vec3 MonAggroPos = script->GetAggroPos();
	float AggroRadius = script->GetAggroRange();
	for (const CGameObject* Champ : Champions) {
		Vec3 ChampPos = Champ->Transform()->GetRelativePos();
		float distance = sqrt(pow(ChampPos.x - MonAggroPos.x, 2.f) + pow(ChampPos.z - MonAggroPos.z, 2.f));
		// ���� ��׷� ������ è�Ǿ��� �ϳ��� ����
		if (distance < AggroRadius) {
			m_bDetectChampion = true;
			break;
		}
	}
	//Animation ��� �߿� ��� champion�� ��׷� ���� ������ ������
	if (m_bDetectChampion == false) {
		GetOwner()->Fsm()->ChangeState(L"A2N");
	}

	
}

void CJungleN2AState::Exit()
{
	GetOwner()->Animator3D()->GetCurAnim()->Reset();
	m_bDetectChampion = true;
}

