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

CChampionScript::CChampionScript(UINT ScriptType)
	: CUnitScript(ScriptType)
	, m_iLevel(1)
	, m_fExp(0)
	, m_EquippedSpell{}
	, m_fRespawnTime(5)
	, m_eCurCC(CC::CLEAR)
	, m_eRestraint(RESTRAINT::DEFAULT)
	, m_Skill{}
	, m_SkillLevel{}
	, m_bIsAttackingChampion(false)
{
	m_eUnitType = UnitType::CHAMPION;

	// test	
	m_fMaxHP = 5;
}

CChampionScript::CChampionScript()
	: CUnitScript((UINT)SCRIPT_TYPE::CHAMPIONSCRIPT)
{
}

CChampionScript::~CChampionScript()
{
	Safe_Del_Array(m_Skill);
}

void CChampionScript::begin()
{
	CUnitScript::begin();

	// ��ų ���� �ʱ�ȭ
	m_SkillLevel[0] = 1;
	m_SkillLevel[2] = 1;

	
	// ��ȯ�� �ֹ� �迭�� �־��ֱ�
}

void CChampionScript::tick()
{
	if (KEY_TAP(KEY::P))
	{
		m_fHP = 0;
	}

	if (CheckDeath())
		return;

	GetInput();
	CheckStatus();
	Move();
	CheckSkills();

}

void CChampionScript::BeginOverlap(CCollider2D* _Other)
{
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
		m_eRestraint = BLOCK;

		m_fRespawnTime -= EditorDT;
		// ��Ȱ ���ð� ������
		if (m_fRespawnTime <= 0)
		{
			m_fHP = m_fMaxHP;
			m_fRespawnTime = 5;
			m_eRestraint = DEFAULT;

			// ��ã�� ������Ʈ�� ���� ��ΰ��� �ִٸ� Clear
			PathFinder()->ClearPath();

			// �칰�� ������
			GetOwner()->Transform()->SetRelativePos(100.f, 30.f, 100.f);

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
	// ü�� / ���� ���� ����
	m_fHP += 2.0f * DT;
	m_fMP += 5.0f * DT;

	if (m_fHP > m_fMaxHP)
		m_fHP = m_fMaxHP;

	if (m_fMP > m_fMaxMP)
		m_fMP = m_fMaxMP;
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
			if ((m_eRestraint & CAN_MOVE) == 0)
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
			if ((m_eRestraint & CAN_USE_SKILL) == 0 || m_Skill[0]->GetCost() > m_fMP)
				return;

			if (m_Skill[1]->CSkill::Use())
			{
				// ��ų �̺�Ʈ
			}
		}
		if (KEY_TAP(KEY::W))
		{
			// ��ų�� ����� �� ���� ��Ȳ Ȥ�� ������ ������ ��� return
			if ((m_eRestraint & CAN_USE_SKILL) == 0 || m_Skill[1]->GetCost() > m_fMP)
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
			if ((m_eRestraint & CAN_USE_SKILL) == 0 || m_Skill[2]->GetCost() > m_fMP)
				return;

			if (m_Skill[3]->CSkill::Use())
			{
				// E �̺�Ʈ �߻�
				PlayerEEvent* evn = dynamic_cast<PlayerEEvent*>(CGameEventMgr::GetInst()->GetEvent((UINT)GAME_EVENT_TYPE::PLAYER_SKILL_E));
				if (evn != nullptr)
				{
					evn->Clear();
					CGameEventMgr::GetInst()->NotifyEvent(*evn);
				}
			}
		}
		if (KEY_TAP(KEY::R))
		{
			// ��ų�� ����� �� ���� ��Ȳ Ȥ�� ������ ������ ��� return
			if ((m_eRestraint & CAN_USE_SKILL) == 0 || m_Skill[3]->GetCost() > m_fMP)
				return;

			if (m_Skill[4]->CSkill::Use())
			{
				// ��ų �̺�Ʈ
			}
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
	if ((m_eRestraint & CAN_MOVE) == 0)
		return;

	// �̵�
	if (PathFindMove(80, true))
	{
		// �̵� �̺�Ʈ
		MoveEvent* evn = dynamic_cast<MoveEvent*>(CGameEventMgr::GetInst()->GetEvent((UINT)GAME_EVENT_TYPE::PLAYER_MOVE));
		if (evn != nullptr)
		{
			evn->Clear();
			evn->SetPlayerID(GetOwner()->GetID());
			evn->SetTargetPos(m_vNextPos);
			evn->SetFaceRot(m_fFaceRot);

			CGameEventMgr::GetInst()->NotifyEvent(*evn);
		}
	}
	else
	{
		// �̵� ���Ͱ��� NaN -> �̵� �Ұ�, ����
		StopEvent* evn = new StopEvent;
		if (evn != nullptr)
		{
			CGameEventMgr::GetInst()->NotifyEvent(*evn);
		}
	}
}


