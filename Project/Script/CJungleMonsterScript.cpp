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
	//몬스터가 스폰된 이후에 aggro범위, hitbox생성해야함
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


	if (//공격을 받았음 && 현재 state가 attack state가 아님, && 어그로가 풀려서 돌아가는중이 아님
		m_bTest && 
		GetOwner()->Fsm()->GetCurState() != GetOwner()->Fsm()->FindState(L"Attack") &&
		GetOwner()->Fsm()->GetCurState() != GetOwner()->Fsm()->FindState(L"Return")) {
		GetOwner()->Fsm()->ChangeState(L"Attack");
	}

	//Target이 있는 경우 어그로 체크를 해야함
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

	// 죽음
	if (m_fHP <= 0)
	{
		// 죽음 이벤트 서버에 쏴야함?
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
	//나중에 DT로 바꿔야함
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