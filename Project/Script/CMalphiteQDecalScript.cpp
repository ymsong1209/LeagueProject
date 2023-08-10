#include "pch.h"
#include "CMalphiteQDecalScript.h"
#include "CUnitScript.h"



CMalphiteQDecalScript::CMalphiteQDecalScript()
	:CScript((UINT)SCRIPT_TYPE::MALPHITEQDECALSCRIPT)
	, m_bDeathActivated(false)
{
}

CMalphiteQDecalScript::~CMalphiteQDecalScript()
{
}


void CMalphiteQDecalScript::begin()
{
	
}

void CMalphiteQDecalScript::tick()
{
	if (m_bDeathActivated) {
		m_fTime += DT;

		GetOwner()->Decal()->SetAlpha(1.f - m_fTime / 3.f);
	}
	else {
		GetOwner()->Transform()->SetRelativePos(Vec3((m_vSpawnPos.x + m_vTargetPos.x) / 2.f, 0.f, (m_vSpawnPos.z + m_vTargetPos.z) / 2.f));
		float distance = sqrt( pow((m_vSpawnPos.x - m_vTargetPos.x), 2.f) + 
								pow((m_vSpawnPos.z - m_vTargetPos.z), 2.f));
		GetOwner()->Transform()->SetRelativeScale(Vec3(distance, 100.f, 40.f));
	}
}

void CMalphiteQDecalScript::DeleteDecal()
{
	m_bDeathActivated = true;
	SetLifeSpan(3.f);
}
