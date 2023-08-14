#include "pch.h"
#include "CJinxBasicAttackMinigunHitEffectScript.h"
#include <Engine/CTimeMgr.h>

CJinxBasicAttackMinigunHitEffectScript::CJinxBasicAttackMinigunHitEffectScript()
	: CScript((UINT)SCRIPT_TYPE::JINXBASICATTACKMINIGUNHITEFFECTSCRIPT)
	, m_fStartTime(0.f)
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
	m_fStartTime += DT;

	if (m_fStartTime >= 2.f)
	{

	}


}

