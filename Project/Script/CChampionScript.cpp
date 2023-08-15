#include "pch.h"
#include "CChampionScript.h"
#include  <algorithm>

#include <Engine\CPathFinder.h>
#include <Engine\CRenderMgr.h>

#include "CGameEvent.h"
#include "CGameEventMgr.h"

#include "CSkill.h"
#include "CAttackRangeScript.h"
#include "CSendServerEventMgr.h"

#define MAX_SKILL_LEVEL 5
#define MAX_ULT_LEVEL 3

CChampionScript::CChampionScript(UINT ScriptType)
	: CUnitScript(ScriptType)
	, m_fExp(0)
	, m_EquippedSpell{}
	, m_fRespawnTime(5)
	, m_bIsAttackingChampion(false)
{
	m_eUnitType = UnitType::CHAMPION;

	m_iDropGold = 300;
	m_fDropExp = 30.f;

	m_fMaxExp = 50.f;
}

CChampionScript::CChampionScript()
	: CUnitScript((UINT)SCRIPT_TYPE::CHAMPIONSCRIPT)
{
}

CChampionScript::~CChampionScript()
{
	
}

void CChampionScript::begin()
{
	CUnitScript::begin();

	// FSM
	if (GetOwner()->Fsm() == nullptr)
		return;
	GetOwner()->Fsm()->ChangeState(L"Idle");

	// ��ų ���� �ʱ�ȭ
	m_SkillLevel[0] = 1;
	m_SkillLevel[1] = 1;
	m_SkillLevel[2] = 1;
	m_SkillLevel[3] = 1;
	m_SkillLevel[4] = 1;
	/// ��ȯ�� �ֹ� �迭�� �־��ֱ�
}

void CChampionScript::tick()
{
	CUnitScript::tick();

	if (KEY_TAP(KEY::P))
	{
		m_fHP = 0;
	}

	CheckSkills();
	if (CheckDeath())
		return;
	
	GetInput();
	CheckStatus();
	Move();

}

void CChampionScript::BeginOverlap(CCollider2D* _Other)
{
	// ��Ÿ��� �ε�ģ�� �ƴ϶�� return
	if (_Other->GetOwner()->GetScript<CAttackRangeScript>() == nullptr) return;

	// �θ� ��ž�̸�, ������ �ٸ� ��ž�� ���
	if (_Other->GetOwner()->GetParent()->GetScript<CUnitScript>()->GetUnitType() == UnitType::TURRET
		&& _Other->GetOwner()->GetParent()->GetScript<CUnitScript>()->GetFaction() != GetFaction())
	{
		m_bIsInsideEnemyTurretRange = true;
	}
}

void CChampionScript::EndOverlap(CCollider2D* _Other)
{
	// ��Ÿ��� �ε�ģ�� �ƴ϶�� return
	if (_Other->GetOwner()->GetScript<CAttackRangeScript>() == nullptr) return;

	// �θ� ��ž�̸�, ������ �ٸ� ��ž�� ���
	if (_Other->GetOwner()->GetParent()->GetScript<CUnitScript>()->GetUnitType() == UnitType::TURRET
		&& _Other->GetOwner()->GetParent()->GetScript<CUnitScript>()->GetFaction() != GetFaction())
	{
		m_bIsInsideEnemyTurretRange = false;
		m_bIsAttackingChampion = false;
	}
}

bool CChampionScript::CheckDeath()
{
	// ����
	if (m_fHP <= 0)
	{
		// ���� �̺�Ʈ
		DeathEvent* evn = dynamic_cast<DeathEvent*>(CGameEventMgr::GetInst()->GetEvent((UINT)GAME_EVENT_TYPE::PLAYER_DEATH));
		if (evn != nullptr)
		{
			CGameEventMgr::GetInst()->NotifyEvent(*evn);
		}
		
		// ���� üũ
		m_bUnitDead = true;

		// �ƹ��͵� ���ϴ� ����
		m_eRestraint |= RESTRAINT::BLOCK;

		m_fRespawnTime -= EditorDT;
		// ��Ȱ ���ð� ������
		if (m_fRespawnTime <= 0)
		{
			m_fHP = m_fMaxHP;
			m_bUnitDead = false;
			m_fRespawnTime = 5;
			m_eCurCC = CC::NO_CC;
			m_eRestraint = RESTRAINT::NO_RESTRAINT;

			// ��ã�� ������Ʈ�� ���� ��ΰ��� �ִٸ� Clear
			PathFinder()->ClearPath();

			// �칰�� ������
			if(m_eFaction == Faction::BLUE)
				GetOwner()->Transform()->SetRelativePos(100.f, 30.f, 100.f);
			else if(m_eFaction == Faction::RED)
				GetOwner()->Transform()->SetRelativePos(2083.0f, 30.0f, 2137.0f);

			// ��Ȱ �̺�Ʈ
			RespawnEvent* evn = dynamic_cast<RespawnEvent*>(CGameEventMgr::GetInst()->GetEvent((UINT)GAME_EVENT_TYPE::PLAYER_RESPAWN));
			if (evn != nullptr)
			{
				CGameEventMgr::GetInst()->NotifyEvent(*evn);
			}

			return false;
		}
		return true;
	}
	return false;

}

void CChampionScript::CheckStatus()
{
	m_fHP += 2.0f * DT;
	m_fMP += 5.0f * DT;

	m_fHP = clamp(m_fHP, 0.f, m_fMaxHP);
	m_fMP = clamp(m_fMP, 0.f, m_fMaxMP);
	
	if (m_fExp >= m_fMaxExp)
	{
		LevelUp();
	}

}

void CChampionScript::GetInput()
{
	// ���� �Է��� Ȯ���մϴ�.

	if (KEY_TAP(KEY::RBTN))
	{
		CCamera* MainCam = CRenderMgr::GetInst()->GetMainCam();
		tRay ray = MainCam->GetRay();

		// ���� ���콺 ray�� �浹 ���� ������Ʈ�� �ִ��� Ȯ��
		vector<CGameObject*>& MouseOverlapObj = MainCam->GetMouseOverlapObj();
		if (MouseOverlapObj.size() >= 1)
		{
			CGameObject* Unit = MouseOverlapObj[0];
			if (Unit == this->GetOwner()) return;
			CUnitScript* UnitScript = Unit->GetScript<CUnitScript>();

			// ������Ʈ�� ���� è�Ǿ��� ��Ÿ� ���� �ִ��� Ȯ��
			CGameObject* AttackRange = GetOwner()->FindChildObjByName(L"AttackRange");
			CAttackRangeScript* AttackRangeScript = AttackRange->GetScript<CAttackRangeScript>();
			vector<CGameObject*> UnitinRange = AttackRangeScript->GetUnitsInRange();

			auto it = find(UnitinRange.begin(), UnitinRange.end(), Unit);

			// ��Ÿ� ���� ����
			if (it != UnitinRange.end())
			{
				// �ش� ������ �׾��ٸ� return
				if (UnitScript->IsUnitDead())
					return;

				// ���� �̺�Ʈ �߻�
				BasicAttackEvent* evn = dynamic_cast<BasicAttackEvent*>(CGameEventMgr::GetInst()->GetEvent((UINT)GAME_EVENT_TYPE::PLAYER_BASIC_ATTACK));
				if (evn != nullptr)
				{
					evn->Clear();
					evn->SetUserObj(GetOwner());
					evn->SetTargetObj(UnitScript->GetOwner());

					CGameEventMgr::GetInst()->NotifyEvent(*evn);
				}


			}

			// ��Ÿ� ���� ����
			else
			{
				// ��Ÿ� ���� ���� ������ �̵�
			}
		}
		else
		{
			// �� ��(���� Ŭ���� ���)

			// ������ �� ���� ��Ȳ�� ��� return
			if ((m_eRestraint & RESTRAINT::CANNOT_MOVE) != 0)
				return;

			CGameObject* Map = CLevelMgr::GetInst()->GetCurLevel()->FindParentObjectByName(L"LoLMapCollider");
			IntersectResult result = MainCam->IsCollidingBtwRayRect(ray, Map);
			Vec3 TargetPos = result.vCrossPoint;	// Ŭ�� ��ǥ
			PathFinder()->FindPath(TargetPos);
		}
	}

	if (KEY_TAP(KEY::Q))
	{
		// ��ų�� ����� �� ���� ��Ȳ Ȥ�� ������ ������ ��� return
		if ((m_eRestraint & RESTRAINT::CANNOT_SKILL) != 0 || m_Skill[1]->GetCost() > m_fMP)
			return;

		if (m_Skill[1]->CSkill::Use())
		{
			// Q�̺�Ʈ �߻�
			PlayerQEvent* evn = dynamic_cast<PlayerQEvent*>(CGameEventMgr::GetInst()->GetEvent((UINT)GAME_EVENT_TYPE::PLAYER_SKILL_Q));
			if (evn != nullptr)
			{
				evn->Clear();
				evn->SetUserObj(GetOwner());
				evn->SetTargetObj(nullptr);
				CGameEventMgr::GetInst()->NotifyEvent(*evn);
			}
		}
	}
	if (KEY_TAP(KEY::W))
	{
		// ��ų�� ����� �� ���� ��Ȳ Ȥ�� ������ ������ ��� return
		if ((m_eRestraint & RESTRAINT::CANNOT_SKILL) != 0 || m_Skill[2]->GetCost() > m_fMP)
			return;

		if (m_Skill[2]->CSkill::Use())
		{
			// W �̺�Ʈ �߻�
			PlayerWEvent* evn = dynamic_cast<PlayerWEvent*>(CGameEventMgr::GetInst()->GetEvent((UINT)GAME_EVENT_TYPE::PLAYER_SKILL_W));
			if (evn != nullptr)
			{
				evn->Clear();
				evn->SetUserObj(GetOwner());
				evn->SetTargetObj(nullptr);
				CGameEventMgr::GetInst()->NotifyEvent(*evn);
			}
		}
	}
	if (KEY_TAP(KEY::E))
	{
		// ��ų�� ����� �� ���� ��Ȳ Ȥ�� ������ ������ ��� return
		if ((m_eRestraint & RESTRAINT::CANNOT_SKILL) != 0 || m_Skill[3]->GetCost() > m_fMP)
			return;

		if (m_Skill[3]->CSkill::Use())
		{
			// E �̺�Ʈ �߻�
			PlayerEEvent* evn = dynamic_cast<PlayerEEvent*>(CGameEventMgr::GetInst()->GetEvent((UINT)GAME_EVENT_TYPE::PLAYER_SKILL_E));
			if (evn != nullptr)
			{
				evn->Clear();
				evn->SetUserObj(GetOwner());
				evn->SetTargetObj(nullptr);
				CGameEventMgr::GetInst()->NotifyEvent(*evn);
			}
		}
	}
	if (KEY_TAP(KEY::R))
	{
		// ��ų�� ����� �� ���� ��Ȳ Ȥ�� ������ ������ ��� return
		if ((m_eRestraint & RESTRAINT::CANNOT_SKILL) != 0 || m_Skill[4]->GetCost() > m_fMP)
			return;

		if (m_Skill[4]->CSkill::Use())
		{
			// R �̺�Ʈ �߻�
			PlayerREvent* evn = dynamic_cast<PlayerREvent*>(CGameEventMgr::GetInst()->GetEvent((UINT)GAME_EVENT_TYPE::PLAYER_SKILL_R));
			if (evn != nullptr)
			{
				evn->Clear();
				evn->SetUserObj(GetOwner());
				evn->SetTargetObj(nullptr);
				CGameEventMgr::GetInst()->NotifyEvent(*evn);
			}
		}
	}
	if (KEY_TAP(KEY::_1))
	{
		// ��ų ����Ʈ�� ���ų� �̹� ��ų ������ Max�� ��� return
		if (m_iSkillLevelUpPoint <= 0 || m_SkillLevel[1] >= MAX_SKILL_LEVEL)
			return;
		
		m_SkillLevel[1] += 1;
		m_iSkillLevelUpPoint -= 1;
	}
	if (KEY_TAP(KEY::_2))
	{
		// ��ų ����Ʈ�� ���ų� �̹� ��ų ������ Max�� ��� return
		if (m_iSkillLevelUpPoint <= 0 || m_SkillLevel[2] >= MAX_SKILL_LEVEL)
			return;

		m_SkillLevel[2] += 1;
		m_iSkillLevelUpPoint -= 1;
	}
	if (KEY_TAP(KEY::_3))
	{
		// ��ų ����Ʈ�� ���ų� �̹� ��ų ������ Max�� ��� return
		if (m_iSkillLevelUpPoint <= 0 || m_SkillLevel[3] >= MAX_SKILL_LEVEL)
			return;

		m_SkillLevel[3] += 1;
		m_iSkillLevelUpPoint -= 1;
	}
	if (KEY_TAP(KEY::_4))
	{
		// ��ų ����Ʈ�� ���ų� �̹� ��ų ������ Max�� ��� return
		if (m_iSkillLevelUpPoint <= 0 || m_SkillLevel[4] >= MAX_ULT_LEVEL)
			return;

		m_SkillLevel[4] += 1;
		m_iSkillLevelUpPoint -= 1;
	}


	// ��ȯ�� �ֹ�

}
void CChampionScript::CheckSkills()
{
	
	for (int i = 0; i < 5; i++)
	{
		if (m_Skill[i] == nullptr)
			continue;

		m_Skill[i]->tick();
	}
}

void CChampionScript::Move()
{
	// ������ �� ���� ��Ȳ�� ��� return
	if ((m_eRestraint & RESTRAINT::CANNOT_MOVE) != 0)
		return;

	// �̵�
	if (PathFindMove(m_fMoveSpeed, true))
	{
		// �̵� �̺�Ʈ
		MoveEvent* evn = dynamic_cast<MoveEvent*>(CGameEventMgr::GetInst()->GetEvent((UINT)GAME_EVENT_TYPE::PLAYER_MOVE));
		if (evn != nullptr)
		{
			CGameEventMgr::GetInst()->NotifyEvent(*evn);
		}
	}
	else
	{
		// �̵� ���Ͱ��� NaN -> �̵� �Ұ�, ����
		StopEvent* evn = dynamic_cast<StopEvent*>(CGameEventMgr::GetInst()->GetEvent((UINT)GAME_EVENT_TYPE::PLAYER_STOP));;
		if (evn != nullptr)
		{
			CGameEventMgr::GetInst()->NotifyEvent(*evn);
		}
	}
}

void CChampionScript::LevelUp()
{
	m_iLevel++;

	//������ ����Ʈ �� ��ƼŬ

	// �ʰ��� ��ŭ exp�� ����� ��ų ����Ʈ �߰�
	m_fExp = m_fExp - m_fMaxExp;
	m_iSkillLevelUpPoint++;
}


