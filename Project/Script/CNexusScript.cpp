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

	// 미니언 생성 주기마다 미니언 생성
	m_fTime += DT;

	if (m_fTime >= m_fMinionSpawnRate)
	{
		// 기준 시간 초기화
		m_fTime = 0;

		// 미니언 생성
		SpawnMinions();
	}
}

void CNexusScript::CheckStatus()
{
	// 4번 포탑이 전부 Dead 상태여야 Attack 가능
}

void CNexusScript::SpawnMinions()
{
	if (!m_bSuperMinionSpawnMode)
	{
		// 각 Lane당 미니언 Spawn
		// 
	}
}
