#include "pch.h"
#include "CDragonScript.h"
#include <Engine\CPathFinder.h>
#include <Engine\CRenderMgr.h>

#include "CGameEvent.h"
#include "CGameEventMgr.h"

#include "CChampionScript.h"
#include "CBasicAttack.h"


#include "CDragonSpawnState.h"
#include "CDragonIdleState.h"
#include "CDragonNormal2AlertState.h"
#include "CDragonAlertState.h"
#include "CDragonAlert2AggroState.h"
#include "CDragonAggro2AlertState.h"
#include "CDragonAlert2NormalState.h"
#include "CDragonFlyStartState.h"
#include "CDragonChaseState.h"
#include "CDragonAttackState.h"
#include "CDragonReturnState.h"
#include "CDragonLandState.h"
#include "CDragonDeathState.h"
#include "CDragonAggroState.h"

CDragonScript::CDragonScript(UINT ScriptType)
	: CMobScript(ScriptType)
	, m_vSpawnPos()
	, m_pTarget(nullptr)
	, m_bReturnActive(false)
	, m_fMaxReturnTime(3.f)
	, m_fCurReturnTime(0.f)
{
	//몬스터가 스폰된 이후에 aggro범위, hitbox생성해야함
	m_fAggroRange = 0.f;
	m_fAlertRange = 0.f;
	m_fAttackRange = 100.f;
}

CDragonScript::CDragonScript()
	:CMobScript((UINT)SCRIPT_TYPE::DRAGONSCRIPT)
	, m_vSpawnPos()
	, m_pTarget(nullptr)
	, m_bReturnActive(false)
	, m_fMaxReturnTime(3.f)
	, m_fCurReturnTime(0.f)
{
	//몬스터가 스폰된 이후에 aggro범위, hitbox생성해야함
	m_fAggroRange = 0.f;
	m_fAlertRange = 0.f;
	m_fAttackRange = 100.f;
}

CDragonScript::~CDragonScript()
{
}

void CDragonScript::GetHit(CGameObject* _target)
{
	if (m_pTarget == nullptr) {
		m_pTarget = _target;
	}
}

void CDragonScript::begin()
{
	m_fHP = 100;
	m_fMaxHP = 100;
	m_fAttackRange = 30.f;
	GetOwner()->Animator3D()->LoadEveryAnimFromFolder(L"animation\\Elder_Dragon");
	if (GetOwner()->GetComponent(COMPONENT_TYPE::FSM) == nullptr) GetOwner()->AddComponent(new CFsm);
	if (GetOwner()->GetComponent(COMPONENT_TYPE::PATHFINDER) == nullptr) GetOwner()->AddComponent(new CPathFinder);

	AddState();

	GetOwner()->Transform()->SetUseMouseOutline(true);
	m_Skill[0] = new CBasicAttack;
	m_Skill[0]->SetOwnerScript(this);
	GetOwner()->Fsm()->ChangeState(L"Spawn");
}

void CDragonScript::tick()
{
	if (CLevelMgr::GetInst()->GetCurLevel()->GetState() == LEVEL_STATE::STOP) return;
	if (CheckDeath()) return;


	if (//공격을 받았음 && 현재 state가 attack state가 아님, && 어그로가 풀려서 돌아가는중이 아님
		m_pTarget &&
		GetOwner()->Fsm()->GetCurState() != GetOwner()->Fsm()->FindState(L"Attack") &&
		GetOwner()->Fsm()->GetCurState() != GetOwner()->Fsm()->FindState(L"Return") &&
		GetOwner()->Fsm()->GetCurState() != GetOwner()->Fsm()->FindState(L"Chase") &&
		GetOwner()->Fsm()->GetCurState() != GetOwner()->Fsm()->FindState(L"FlyStart") &&
		GetOwner()->Fsm()->GetCurState() != GetOwner()->Fsm()->FindState(L"Land"))
	{
		GetOwner()->Fsm()->ChangeState(L"FlyStart");
	}

	//어그로가 풀려서 돌아가는 중이면 하단의 코드는 진행 안해도됨
	if (GetOwner()->Fsm()->GetCurState() == GetOwner()->Fsm()->FindState(L"Return")) return;


	//Target이 있는 경우 어그로 범위 밖에 드래곤이 있는지 체크를 해야함
	if (m_pTarget == nullptr) return;
	CheckReturnActive();
	if (m_bReturnActive) {
		CheckReturnTime();
	}
}

void CDragonScript::BeginOverlap(CCollider2D* _Other)
{
	/*if (GetOwner()->Fsm()->GetCurState() == GetOwner()->Fsm()->FindState(L"Chase")) {
		if (_Other->GetOwner() == m_pTarget) {
			GetOwner()->Fsm()->ChangeState(L"Attack");
		}
	}*/
}

void CDragonScript::EndOverlap(CCollider2D* _Other)
{
	/*if (m_pTarget) {
		GetOwner()->Fsm()->ChangeState(L"Chase");
	}*/

}


void CDragonScript::AddState()
{

	GetOwner()->Fsm()->AddState(L"Spawn", new CDragonSpawnState);
	GetOwner()->Fsm()->AddState(L"Idle", new CDragonIdleState);
	GetOwner()->Fsm()->AddState(L"Normal2Alert", new CDragonNormal2AlertState);
	GetOwner()->Fsm()->AddState(L"Alert", new CDragonAlertState);
	GetOwner()->Fsm()->AddState(L"Alert2Aggro", new CDragonAlert2AggroState);
	GetOwner()->Fsm()->AddState(L"Aggro2Alert", new CDragonAggro2AlertState);
	GetOwner()->Fsm()->AddState(L"Alert2Normal", new CDragonAlert2NormalState);
	GetOwner()->Fsm()->AddState(L"FlyStart", new CDragonFlyStartState);
	GetOwner()->Fsm()->AddState(L"Chase", new CDragonChaseState);
	GetOwner()->Fsm()->AddState(L"Attack", new CDragonAttackState);
	GetOwner()->Fsm()->AddState(L"Return", new CDragonReturnState);
	GetOwner()->Fsm()->AddState(L"Land", new CDragonLandState);
	GetOwner()->Fsm()->AddState(L"Death", new CDragonDeathState);
	GetOwner()->Fsm()->AddState(L"Aggro", new CDragonAggroState);
}

bool CDragonScript::CheckDeath()
{
	if (m_bUnitDead) return true;

	// 죽음
	if (m_fHP <= 0)
	{
		GetOwner()->Fsm()->ChangeState(L"Death");
		return true;
	}
	return false;
}

void CDragonScript::CheckReturnTime()
{
	//나중에 DT로 바꿔야함
	m_fCurReturnTime += DT;

	CUnitScript* ChampScript = m_pTarget->GetScript<CUnitScript>();

	if (m_fMaxReturnTime < m_fCurReturnTime || ChampScript->IsUnitDead()) {
		GetOwner()->Fsm()->ChangeState(L"Return");
		m_pTarget = nullptr;
		m_bReturnActive = false;
		m_fCurReturnTime = 0.f;
	}
}

void CDragonScript::CheckReturnActive()
{
	if (!m_pTarget) return;

	Vec3 Targetpos = m_pTarget->Transform()->GetRelativePos();
	float distance = sqrt(pow(m_vAlertPos.x - Targetpos.x, 2.f) + pow(m_vAlertPos.z - Targetpos.z, 2.f));
	if (distance > m_fAlertRange && m_bReturnActive == false)
		m_bReturnActive = true;
	else if (distance < m_fAlertRange && m_bReturnActive == true) {
		m_bReturnActive = false;
		m_fCurReturnTime = 0.f;
	}

}

void CDragonScript::SaveToLevelFile(FILE* _File)
{
	CMobScript::SaveToLevelFile(_File);
	fwrite(&m_vAggroPos, sizeof(Vec3), 1, _File);
	fwrite(&m_vAlertPos, sizeof(Vec3), 1, _File);
}

void CDragonScript::LoadFromLevelFile(FILE* _FILE)
{
	CMobScript::LoadFromLevelFile(_FILE);
	fread(&m_vAggroPos, sizeof(Vec3), 1, _FILE);
	fread(&m_vAlertPos, sizeof(Vec3), 1, _FILE);
}

void CDragonScript::SaveToLevelJsonFile(Value& _objValue, Document::AllocatorType& allocator)
{
	CMobScript::SaveToLevelJsonFile(_objValue, allocator);
	_objValue.AddMember("AggroPos", SaveVec3Json(m_vAggroPos, allocator), allocator);
	_objValue.AddMember("AlertPos", SaveVec3Json(m_vAlertPos, allocator), allocator);
}

void CDragonScript::LoadFromLevelJsonFile(const Value& _componentValue)
{
	CMobScript::LoadFromLevelJsonFile(_componentValue);
	m_vAggroPos = LoadVec3Json(_componentValue["AggroPos"]);
	m_vAlertPos = LoadVec3Json(_componentValue["AlertPos"]);
}
