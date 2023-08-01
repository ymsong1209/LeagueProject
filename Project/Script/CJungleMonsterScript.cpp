#include "pch.h"
#include "CJungleMonsterScript.h"
#include <Engine\CPathFinder.h>
#include <Engine\CRenderMgr.h>

#include "CGameEvent.h"
#include "CGameEventMgr.h"

#include "CChampionScript.h"

CJungleMonsterScript::CJungleMonsterScript(JungleType _type)
	:CMobScript((UINT)SCRIPT_TYPE::JUNGLEMONSTERSCRIPT)
	, m_eJungleType(_type)
	, m_fMaxReturnTime(3.f)
	, m_fCurReturnTime(0.f)
	, m_bTest(false)
{
	//���Ͱ� ������ ���Ŀ� aggro����, hitbox�����ؾ���
	m_fAggroRange = 0.f;
}

CJungleMonsterScript::~CJungleMonsterScript()
{
}


void CJungleMonsterScript::begin()
{
	GetOwner()->Fsm()->ChangeState(L"Spawn");
}

void CJungleMonsterScript::tick()
{
	if (CheckDeath()) return;
	if (KEY_TAP(KEY::P)) {
		m_bTest = !m_bTest;
	}


	if (//������ �޾��� && ���� state�� attack state�� �ƴ�, && ��׷ΰ� Ǯ���� ���ư������� �ƴ�
		m_bTest && 
		GetOwner()->Fsm()->GetCurState() != GetOwner()->Fsm()->FindState(L"Attack") &&
		GetOwner()->Fsm()->GetCurState() != GetOwner()->Fsm()->FindState(L"Return")) {
		GetOwner()->Fsm()->ChangeState(L"Attack");
	}

	//Target�� �ִ� ��� ��׷� üũ�� �ؾ���
	if (m_pTarget == nullptr) return;
	CheckReturnActive();
	if (m_bReturnActive) {
		CheckReturnTime();
	}
}

void CJungleMonsterScript::BeginOverlap(CCollider2D* _Other)
{
	if (GetOwner()->Fsm()->GetCurState() == GetOwner()->Fsm()->FindState(L"Chase")) {
		if (_Other->GetOwner() == m_pTarget) {
			GetOwner()->Fsm()->ChangeState(L"Attack");
		}
	}
}

void CJungleMonsterScript::EndOverlap(CCollider2D* _Other)
{
	if (m_pTarget) {
		GetOwner()->Fsm()->ChangeState(L"Chase");
	}

}


bool CJungleMonsterScript::CheckDeath()
{
	if (m_bUnitDead) return true;

	// ����
	if (m_fHP <= 0)
	{
		// ���� �̺�Ʈ ������ ������?
		DeathEvent* evn = dynamic_cast<DeathEvent*>(CGameEventMgr::GetInst()->GetEvent((UINT)GAME_EVENT_TYPE::PLAYER_KILL_MOB));
		if (evn != nullptr)
		{
			CGameEventMgr::GetInst()->NotifyEvent(*evn);
		}
		GetOwner()->Fsm()->ChangeState(L"Dead");
	}
	return false;
}

void CJungleMonsterScript::CheckReturnTime()
{
	//���߿� DT�� �ٲ����
	m_fCurReturnTime += EditorDT;
	
	CChampionScript* ChampScript = m_pTarget->GetScript<CChampionScript>();
	
	if (m_fMaxReturnTime < m_fCurReturnTime || ChampScript->IsUnitDead()) {
		GetOwner()->Fsm()->ChangeState(L"Return");
	}
}

void CJungleMonsterScript::CheckReturnActive()
{
	if (!m_pTarget) return;

	Vec3 Targetpos = m_pTarget->Transform()->GetRelativePos();
	float distance = sqrt(pow(m_vSpawnPos.x - Targetpos.x, 2.f) + pow(m_vSpawnPos.z - Targetpos.z, 2.f));
	if (distance > m_fAggroRange && !m_bReturnActive)
		m_bReturnActive = true;
	else {
		m_bReturnActive = false;
		m_fCurReturnTime = 0.f;
	}
		
}