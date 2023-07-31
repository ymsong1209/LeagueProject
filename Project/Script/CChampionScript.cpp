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

		// �ƹ��͵� ���ϴ� ����
		m_eRestraint = BLOCK;

		m_fRespawnTime -= DT;
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
				// ���� �̺�Ʈ �߻�
				BaseAttackEvent* evn = dynamic_cast<BaseAttackEvent*>(CGameEventMgr::GetInst()->GetEvent((UINT)GAME_EVENT_TYPE::PLAYER_BASE_ATTACK));
				if (evn != nullptr)
				{
					evn->Clear();
					evn->SetUserID(GetOwner()->GetID());
					evn->SetTargetID(UnitScript->GetOwner()->GetID());

					CGameEventMgr::GetInst()->NotifyEvent(*evn);
				}

				// �������� ���� �̺�Ʈ
				SkillInfo* skillInfo = new SkillInfo;
				skillInfo->OwnerId = GetServerID();
				skillInfo->TargetId = UnitScript->GetServerID();
				skillInfo->SkillLevel = 1;
				skillInfo->skillType = SkillType::BASIC_ATTACK;

				tServerEvent serverEvn = {};
				serverEvn.Type = SERVER_EVENT_TYPE::SKILL_PROJECTILE_PACKET;
				serverEvn.wParam = (DWORD_PTR)skillInfo;
				//serverEvn.lParam 
				CSendServerEventMgr::GetInst()->AddServerSendEvent(serverEvn);
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

			CGameObject* Map = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"LoLMapCollider");
			IntersectResult result = MainCam->IsCollidingBtwRayRect(ray, Map);
			Vec3 TargetPos = result.vCrossPoint;	// Ŭ�� ��ǥ
			PathFinder()->FindPath(TargetPos);
		}


		if (KEY_TAP(KEY::Q))
		{
			// ��ų�� ����� �� ���� ��Ȳ Ȥ�� ������ ������ ��� return
			if ((m_eRestraint & CAN_USE_SKILL) == 0 || m_Skill[0]->GetCost() > m_fMP)
				return;

			if (m_Skill[0]->Use())
			{
				// ��ų �̺�Ʈ
			}
		}
		if (KEY_TAP(KEY::W))
		{
			// ��ų�� ����� �� ���� ��Ȳ Ȥ�� ������ ������ ��� return
			if ((m_eRestraint & CAN_USE_SKILL) == 0 || m_Skill[1]->GetCost() > m_fMP)
				return;

			if (m_Skill[1]->Use())
			{
				// ��ų �̺�Ʈ
			}
		}
		if (KEY_TAP(KEY::E))
		{
			// ��ų�� ����� �� ���� ��Ȳ Ȥ�� ������ ������ ��� return
			if ((m_eRestraint & CAN_USE_SKILL) == 0 || m_Skill[2]->GetCost() > m_fMP)
				return;

			if (m_Skill[2]->Use())
			{
				// ��ų �̺�Ʈ
			}
		}
		if (KEY_TAP(KEY::R))
		{
			// ��ų�� ����� �� ���� ��Ȳ Ȥ�� ������ ������ ��� return
			if ((m_eRestraint & CAN_USE_SKILL) == 0 || m_Skill[3]->GetCost() > m_fMP)
				return;

			if (m_Skill[3]->Use())
			{
				// ��ų �̺�Ʈ
			}
		}

		// ��ȯ�� �ֹ�
	}
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

void CChampionScript::GetHit(CSkill* _skill)
{
	// ��ų �Ŵ��� ��� ��ų ���� ������ �� ��ų�� �

	// ������ ����
	CGameObject* User = CUR_LEVEL->FindObjectByID(_skill->GetUserID());
	CUnitScript* UserScript = User->GetScript<CUnitScript>();

	float	UserAttackPower = UserScript->GetAttackPower();

	// �̿�

}
