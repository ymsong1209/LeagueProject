#include "pch.h"
#include "CJinxBasicAttackMinigunHitEffectScript.h"
#include <Engine/CTimeMgr.h>

CJinxBasicAttackMinigunHitEffectScript::CJinxBasicAttackMinigunHitEffectScript()
	: CScript((UINT)SCRIPT_TYPE::JINXBASICATTACKMINIGUNHITEFFECTSCRIPT)
	, m_fPassedTime(0.f)
	, m_bSecontHitEffectSpawned(false)
	, m_bThirdHitEffectSpawned(false)
{
}

CJinxBasicAttackMinigunHitEffectScript::~CJinxBasicAttackMinigunHitEffectScript()
{
}

void CJinxBasicAttackMinigunHitEffectScript::begin()
{
	//m_pSpawnObject = 
}

void CJinxBasicAttackMinigunHitEffectScript::tick()
{
	m_fPassedTime += DT;

	if (m_bSecontHitEffectSpawned == false && m_fPassedTime >= 1.f)
	{
		m_bSecontHitEffectSpawned = true;
	}

	if (m_bThirdHitEffectSpawned == false && m_fPassedTime >= 2.f)
	{
		m_bThirdHitEffectSpawned = true;
	}

	if (m_fPassedTime >= 3.f)
	{
		DeleteObject(GetOwner());
	}
}

