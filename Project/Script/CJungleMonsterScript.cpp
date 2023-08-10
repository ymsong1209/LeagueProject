#include "pch.h"
#include "CJungleMonsterScript.h"
#include <Engine\CPathFinder.h>
#include <Engine\CRenderMgr.h>

#include "CGameEvent.h"
#include "CGameEventMgr.h"

#include "CChampionScript.h"
#include "CBasicAttack.h"
#include "CProjectileScript.h"

CJungleMonsterScript::CJungleMonsterScript(UINT ScriptType)
	: CMobScript(ScriptType)
	, m_vSpawnPos()
	, m_pTarget(nullptr)
	, m_bReturnActive(false)
	, m_fMaxReturnTime(3.f)
	, m_fCurReturnTime(0.f)
{
	//���Ͱ� ������ ���Ŀ� aggro����, hitbox�����ؾ���
	m_fAggroRange = 0.f;
	m_fAttackRange = 0.f;
	m_fHP = 1;
	m_fMaxHP = 1;
	m_fAttackPower = 10;
}

CJungleMonsterScript::CJungleMonsterScript()
	:CMobScript((UINT)SCRIPT_TYPE::JUNGLEMONSTERSCRIPT)
	, m_vSpawnPos()
	, m_pTarget(nullptr)
	, m_bReturnActive(false)
	, m_fMaxReturnTime(3.f)
	, m_fCurReturnTime(0.f)
{
	//���Ͱ� ������ ���Ŀ� aggro����, hitbox�����ؾ���
	m_fAggroRange = 0.f;
	m_fAttackRange = 0.f;
	m_fHP = 1;
	m_fMaxHP = 1;
	m_fAttackPower = 10;
}

CJungleMonsterScript::~CJungleMonsterScript()
{
}

void CJungleMonsterScript::GetHit(CGameObject* _target)
{
	if (m_pTarget == nullptr) {
		m_pTarget = _target;
	}
}

void CJungleMonsterScript::begin()
{
	m_eCurCC = CC::CLEAR;
	m_eRestraint = RESTRAINT::DEFAULT;

	GetOwner()->Transform()->SetUseMouseOutline(true);
	m_Skill[0] = new CBasicAttack;
	m_Skill[0]->SetOwnerScript(this);
	GetOwner()->Fsm()->ChangeState(L"Spawn");
}

void CJungleMonsterScript::tick()
{
	CUnitScript::tick();
	if (CLevelMgr::GetInst()->GetCurLevel()->GetState() == LEVEL_STATE::STOP) return;
	if (CheckDeath()) return;
	
	
	if (//������ �޾��� && ���� state�� attack state�� �ƴ�, && ��׷ΰ� Ǯ���� ���ư������� �ƴ�
		m_pTarget && 
		GetOwner()->Fsm()->GetCurState() != GetOwner()->Fsm()->FindState(L"Attack") &&
		GetOwner()->Fsm()->GetCurState() != GetOwner()->Fsm()->FindState(L"Return") &&
		GetOwner()->Fsm()->GetCurState() != GetOwner()->Fsm()->FindState(L"Chase")) {
		GetOwner()->Fsm()->ChangeState(L"Chase");
	}

	//��׷ΰ� Ǯ���� ���ư��� ���̸� �ϴ��� �ڵ�� ���� ���ص���
	if (GetOwner()->Fsm()->GetCurState() == GetOwner()->Fsm()->FindState(L"Return")) return;


	//Target�� �ִ� ��� ��׷� ���� �ۿ� ���۸��� �ִ��� üũ�� �ؾ���
	if (m_pTarget == nullptr) return;
	CheckReturnActive();
	if (m_bReturnActive) {
		CheckReturnTime();
	}
}

void CJungleMonsterScript::BeginOverlap(CCollider2D* _Other)
{
	int a = 0;
	
}

void CJungleMonsterScript::EndOverlap(CCollider2D* _Other)
{
	
}


bool CJungleMonsterScript::CheckDeath()
{
	if (m_bUnitDead) return true;

	// ����
	if (m_fHP <= 0)
	{
		GetOwner()->Fsm()->ChangeState(L"Death");
		return true;
	}
	return false;
}

void CJungleMonsterScript::CheckReturnTime()
{
	//���߿� DT�� �ٲ����
	m_fCurReturnTime += DT;
	
	CUnitScript* Champscript = m_pTarget->GetScript<CUnitScript>();
	
	if (m_fMaxReturnTime < m_fCurReturnTime || Champscript->IsUnitDead()) {
		GetOwner()->Fsm()->ChangeState(L"Return");
		m_pTarget = nullptr;
		m_bReturnActive = false;
		m_fCurReturnTime = 0.f;
	}
}

void CJungleMonsterScript::CheckReturnActive()
{
	if (!m_pTarget) return;

	Vec3 Targetpos = m_pTarget->Transform()->GetRelativePos();
	float distance = sqrt(pow(m_vAggroPos.x - Targetpos.x, 2.f) + pow(m_vAggroPos.z - Targetpos.z, 2.f));
	if (distance > m_fAggroRange && m_bReturnActive == false)
		m_bReturnActive = true;
	else if(distance < m_fAggroRange && m_bReturnActive == true){
		m_bReturnActive = false;
		m_fCurReturnTime = 0.f;
	}
		
}

void CJungleMonsterScript::SaveToLevelFile(FILE* _File)
{
	CMobScript::SaveToLevelFile(_File);
	fwrite(&m_vAggroPos, sizeof(Vec3), 1, _File);
}

void CJungleMonsterScript::LoadFromLevelFile(FILE* _FILE)
{
	CMobScript::LoadFromLevelFile(_FILE);
	fread(&m_vAggroPos, sizeof(Vec3), 1, _FILE);
}

void CJungleMonsterScript::SaveToLevelJsonFile(Value& _objValue, Document::AllocatorType& allocator)
{
	CMobScript::SaveToLevelJsonFile(_objValue, allocator);
	_objValue.AddMember("AggroPos", SaveVec3Json(m_vAggroPos, allocator), allocator);
}

void CJungleMonsterScript::LoadFromLevelJsonFile(const Value& _componentValue)
{
	CMobScript::LoadFromLevelJsonFile(_componentValue);
	m_vAggroPos = LoadVec3Json(_componentValue["AggroPos"]);
}
