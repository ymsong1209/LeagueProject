#include "pch.h"
#include "CMalphiteScript.h"

#include "CMalphiteIdleState.h"
#include "CMalphiteWalkState.h"
#include "CMalphiteDeathState.h"
#include "CMalphiteRespawnState.h"
#include "CMalphiteAttackState.h"
#include "CMalphiteQState.h"
#include "CMalphiteWState.h"
#include "CMalphiteEState.h"
#include "CMalphiteRState.h"

#include "CBasicAttack.h"
#include "CMalphiteQ.h"
#include "CMalphiteW.h"
#include "CMalphiteE.h"
#include "CMalphiteR.h"

#include <Engine\CPathFinder.h>
#include <Engine\CRenderMgr.h>
#include "CAttackRangeScript.h"


CMalphiteScript::CMalphiteScript()
	:CChampionScript((UINT)MALPHITESCRIPT)
{
	m_fMaxHP = 50;
	m_fMaxMP = 50;
	m_fHP = 50;
	m_fMP = 50;
	//m_fAttackPower = 5.f;
	m_fAttackPower = 10.f;
	m_fMoveSpeed = 100.f;
}

CMalphiteScript::~CMalphiteScript()
{
}

void CMalphiteScript::begin()
{
	// FSM�� State �߰�
	GetOwner()->Fsm()->AddState(L"Idle", new CMalphiteIdleState);
	GetOwner()->Fsm()->AddState(L"Walk", new CMalphiteWalkState);
	GetOwner()->Fsm()->AddState(L"Death", new CMalphiteDeathState);
	GetOwner()->Fsm()->AddState(L"Respawn", new CMalphiteRespawnState);
	GetOwner()->Fsm()->AddState(L"Attack", new CMalphiteAttackState);
	GetOwner()->Fsm()->AddState(L"Recall", new CMalphiteRespawnState);
	GetOwner()->Fsm()->AddState(L"Q", new CMalphiteQState);
	GetOwner()->Fsm()->AddState(L"W", new CMalphiteWState);
	GetOwner()->Fsm()->AddState(L"E", new CMalphiteEState);
	GetOwner()->Fsm()->AddState(L"R", new CMalphiteRState);

	// Skill�� Jinx Skill �߰�
	m_Skill[0] = new CBasicAttack;
	m_Skill[0]->SetOwnerScript(this);
	m_Skill[1] = new CMalphiteQ;
	m_Skill[1]->SetOwnerScript(this);
	m_Skill[2] = new CMalphiteW;
	m_Skill[2]->SetOwnerScript(this);
	m_Skill[3] = new CMalphiteE;
	m_Skill[3]->SetOwnerScript(this);
	m_Skill[4] = new CMalphiteR;
	m_Skill[4]->SetOwnerScript(this);

	// Spell�� ��ȯ�� �ֹ� �߰�


	CChampionScript::begin();
}

void CMalphiteScript::tick()
{
	CChampionScript::tick();

	m_fAttackAnimCoolTime += DT;
	if (m_bWActive) {
		m_fWActiveTime += DT;
		if (m_fWActiveTime > 4.f) {
			m_bWActive = false;
			m_fWActiveTime = 0.f;

			m_fAttackPower = 10.f;

			//������ �ٲ���
			UINT64 targetId = GetOwner()->GetScript<CUnitScript>()->GetServerID();
			Ptr<CTexture> normaltex = CResMgr::GetInst()->FindRes<CTexture>(L"texture\\FBXTexture\\malphite.dds");
			if (normaltex == nullptr) assert(nullptr);
			GetOwner()->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, normaltex);
			CSendServerEventMgr::GetInst()->SendMtrlPacket(targetId, 0, TEX_0, L"texture\\FBXTexture\\malphite.dds");
		}
	}
	
}

void CMalphiteScript::GetInput()
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


		CCamera* MainCam = CRenderMgr::GetInst()->GetMainCam();
		tRay ray = MainCam->GetRay();

		// ���� ���콺 ray�� �浹 ���� ������Ʈ�� �ִ��� Ȯ��
		vector<CGameObject*>& MouseOverlapObj = MainCam->GetMouseOverlapObj();
		if (MouseOverlapObj.size() >= 1)
		{
			CGameObject* Unit = MouseOverlapObj[0];
			if (Unit == this->GetOwner()) return;

			CUnitScript* UnitScript = Unit->GetScript<CUnitScript>();

			// �ش� ������ �׾��ٸ� return
			if (UnitScript->IsUnitDead())
				return;

			if (UnitScript->GetFaction() == GetFaction()) return;

			if (m_Skill[1]->CSkill::Use())
			{
				// Q�̺�Ʈ �߻�
				PlayerQEvent* evn = dynamic_cast<PlayerQEvent*>(CGameEventMgr::GetInst()->GetEvent((UINT)GAME_EVENT_TYPE::PLAYER_SKILL_Q));
				if (evn != nullptr)
				{
					evn->Clear();
					evn->SetUserObj(GetOwner());
					evn->SetTargetObj(Unit);
					CGameEventMgr::GetInst()->NotifyEvent(*evn);
				}
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

	// ��ȯ�� �ֹ�
}
