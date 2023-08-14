#include "pch.h"
#include "CNexusScript.h"

CNexusScript::CNexusScript()
	:CStructureScript((UINT)SCRIPT_TYPE::NEXUSSCRIPT)
	, m_fTime(0)
	, m_fMinionSpawnRate(90.f)
	, m_bSuperMinionSpawnMode(false)
{
}

CNexusScript::~CNexusScript()
{
}

void CNexusScript::begin()
{
	CStructureScript::begin();
}

void CNexusScript::tick()
{
	CStructureScript::tick();

	// �̴Ͼ� ���� �ֱ⸶�� �̴Ͼ� ����
	m_fTime += DT;

	if (m_fTime >= m_fMinionSpawnRate)
	{
		// ���� �ð� �ʱ�ȭ
		m_fTime = 0;

		// �̴Ͼ� ����
		SpawnMinions();
	}
}

void CNexusScript::CheckStatus()
{
	// 4�� ��ž�� ���� Dead ���¿��� Attack ����
}

void CNexusScript::SpawnMinions()
{
	if (!m_bSuperMinionSpawnMode)
	{
		// �� Lane�� �̴Ͼ� Spawn
		// 
	}
}
