#include "pch.h"
#include "CFlash.h"
#include <Engine\CScript.h>

CFlash::CFlash()
{
	m_fCoolDown = 180.f;
	m_iLevel = 1;
}

CFlash::~CFlash()
{
}

void CFlash::tick()
{
	CSkill::tick();
}

bool CFlash::Use()
{
	// 스킬 쿨타임 초기화
	if (!CSkill::Use())
		return false;

	// 마우스 방향으로 500.f 거리만큼 순간이동
	// 챔피언이 보고 있는 Face 방향도 돌려야함

	//m_OwnerScript->GetOwner()->Transform()->..

	// Flash SFX & VFX 재생

	return true;
}
