#include "pch.h"
#include "CNexusScript.h"

#include "CNexusBrokenState.h"
#include "CNexusIdleState.h"
#include "CNexusSpawnState.h"



CNexusScript::CNexusScript()
	:CStructureScript((UINT)SCRIPT_TYPE::NEXUSSCRIPT)
	, m_vecTurrets{}
{
}

CNexusScript::~CNexusScript()
{
}

void CNexusScript::begin()
{
	CStructureScript::begin();

	// FSM�� State �߰�
	if (GetOwner()->GetComponent(COMPONENT_TYPE::FSM) == nullptr) GetOwner()->AddComponent(new CFsm);
	GetOwner()->Fsm()->AddState(L"Idle", new CNexusIdleState);
	GetOwner()->Fsm()->AddState(L"Broken", new CNexusBrokenState);
	GetOwner()->Fsm()->AddState(L"Spawn", new CNexusSpawnState);

	GetOwner()->Fsm()->ChangeState(L"Spawn");

	m_bUnitDead = true;
}

void CNexusScript::tick()
{
	CStructureScript::tick();

	//�ӽ� �ڵ�
	m_vecTurrets.clear();

	// ������ ���� Lane�� None�� ��ž(������ �� ��ž)�� ��ž ���Ϳ� ����Ѵ�.
	vector<CGameObject*> StructureLayer = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(3)->GetParentObject();

	for (int i = 0; i < StructureLayer.size(); i++)
	{
		CUnitScript* Structure = StructureLayer[i]->GetScript<CUnitScript>();
		CUnitScript* Nexus = GetOwner()->GetScript<CUnitScript>();

		if (Structure->GetFaction() == Nexus->GetFaction() && Structure->GetLane() == Lane::NONE)
		{
			if (Structure->GetUnitType() != UnitType::TURRET)
				continue;

			m_vecTurrets.push_back(StructureLayer[i]);
		}
	}

	CheckStatus();
}

void CNexusScript::CheckStatus()
{
	// 4�� ��ž�� ���� Dead ���¿��� Attack ����
	if (m_vecTurrets.size() < 2)
		return;

	// ��ž ���̾�� ��� ��ž ������Ʈ�� �����ͼ�
	// ���� Lane�� ��� ��ž�� Dead �������� Ȯ��
	for (int i = 0; i < 2; i++)
	{
		CUnitScript* TurretScript = m_vecTurrets[i]->GetScript<CUnitScript>();
		if (!TurretScript->IsUnitDead())
			break;

		// ������ Turret���� dead���, dead���� ����
		if (i == 1)
			m_bUnitDead = false;
	}
}

