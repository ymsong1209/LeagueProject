#include "pch.h"
#include "CInhibitorScript.h"

#include "CInhibitorBrokenState.h"
#include "CInhibitorIdleState.h"
#include "CInhibitorSpawnState.h"
#include "CInhibitorRespawnState.h"

CInhibitorScript::CInhibitorScript()
	:CStructureScript((UINT)SCRIPT_TYPE::INHIBITORSCRIPT)
	, m_vecTurrets{}
	, m_fRespawnTime(5)
{
	m_fMaxHP = 30.f;
}

CInhibitorScript::~CInhibitorScript()
{
	
}

void CInhibitorScript::begin()
{
	CStructureScript::begin();

	// FSM�� State �߰�
	if (GetOwner()->GetComponent(COMPONENT_TYPE::FSM) == nullptr) GetOwner()->AddComponent(new CFsm);
	GetOwner()->Fsm()->AddState(L"Idle", new CInhibitorIdleState);
	GetOwner()->Fsm()->AddState(L"Broken", new CInhibitorBrokenState);
	GetOwner()->Fsm()->AddState(L"Spawn", new CInhibitorSpawnState);
	GetOwner()->Fsm()->AddState(L"Respawn", new CInhibitorRespawnState);

	GetOwner()->Fsm()->ChangeState(L"Spawn");

	// ���� / ������ ���� ��ž�� ��ž ���Ϳ� ����Ѵ�.
	vector<CGameObject*> StructureLayer = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(3)->GetParentObject();

	for (int i = 0; i < StructureLayer.size(); i++)
	{
		CUnitScript* Structure = StructureLayer[i]->GetScript<CUnitScript>();
		CUnitScript* Inhibitor = GetOwner()->GetScript<CUnitScript>();

		if (Structure->GetFaction() == Inhibitor->GetFaction() && Structure->GetLane() == Inhibitor->GetLane())
		{
			if (Structure == Inhibitor)
				continue;

			m_vecTurrets.push_back(StructureLayer[i]);
		}
	}

	// ���ݹ��� �ʰ� m_bUnitDeadó��
	m_bUnitDead = true;
}

void CInhibitorScript::tick()
{
	CheckStatus();


	//�ӽ� �ڵ�
	m_vecTurrets.clear();

	// ���� / ������ ���� ��ž�� ��ž ���Ϳ� ����Ѵ�.
	vector<CGameObject*> StructureLayer = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(3)->GetParentObject();

	for (int i = 0; i < StructureLayer.size(); i++)
	{
		CUnitScript* Structure = StructureLayer[i]->GetScript<CUnitScript>();
		CUnitScript* Inhibitor = GetOwner()->GetScript<CUnitScript>();

		if (Structure->GetFaction() == Inhibitor->GetFaction() && Structure->GetLane() == Inhibitor->GetLane())
		{
			if (Structure == Inhibitor)
				continue;

			m_vecTurrets.push_back(StructureLayer[i]);
		}
	}
}

void CInhibitorScript::CheckStatus()
{
	// ���� �������� ���� üũ
	// ��ž�� �ı��Ǿ��ٸ� ������ ���� �Ұ�

	if (m_vecTurrets.size() < 3)
		return;

	if (GetOwner()->Fsm()->GetCurState()->GetName() == L"Broken"
		|| GetOwner()->Fsm()->GetCurState()->GetName() == L"Respawn")
		return;

	// ��ž ���̾�� ��� ��ž ������Ʈ�� �����ͼ�
	// ���� Lane�� ��� ��ž�� Dead �������� Ȯ��
	for (int i = 0; i < 3; i++)
	{
		CUnitScript* TurretScript = m_vecTurrets[i]->GetScript<CUnitScript>();
		if (!TurretScript->IsUnitDead())
			break;

		// ������ Turret���� dead���, dead���� ����
		if (i == 2)
			m_bUnitDead = false;
	}
}
