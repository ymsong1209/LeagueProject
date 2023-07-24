#include "pch.h"
#include "CJinxW.h"

CJinxW::CJinxW()
{
	m_strSkillName = L"Zap!";
	m_fCoolDown = 4.f;
	m_iMaxLevel = 5;
	m_fCost = 50.f;
}

CJinxW::~CJinxW()
{
}

void CJinxW::tick()
{
	CSkill::tick();
}

bool CJinxW::Use()
{
	// 스킬 쿨타임 초기화
	if (!CSkill::Use())
		return false;

	// W 투사체 발사
	// 징크스가 전기 충격파를 발사하여 처음 맞힌 적에게 물리 피해를 입히고, 2초 동안 둔화시키며 위치를 드러냅니다.

}
