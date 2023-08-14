#include "pch.h"
#include "CJinxRocketBasicAttack.h"

CJinxRocketBasicAttack::CJinxRocketBasicAttack()
{
    m_strSkillName = L"Jinx_Rocket_Basic_Attack";
    m_fCoolDown = 0.f;
    m_iLevel = 1;
}

CJinxRocketBasicAttack::~CJinxRocketBasicAttack()
{
}

void CJinxRocketBasicAttack::tick()
{
}

bool CJinxRocketBasicAttack::Use()
{
	// �������� �⺻ ���� ��� ��ȣ�� ����
	CSendServerEventMgr::GetInst()->SendUseSkillPacket(
		m_UserObj->GetScript<CUnitScript>()->GetServerID(),
		m_TargetObj->GetScript<CUnitScript>()->GetServerID(),
		1,				// �⺻ ������ ������ ������ 1
		SkillType::JINX_ROCKET_BASIC_ATTACK,
		Vec3(0, 0, 0),
		m_iProjectileCount,
		false,
		Vec3(0, 0, 0),
		false,
		Vec3(0, 0, 0));

    return false;
}

void CJinxRocketBasicAttack::GetHit(CUnitScript* _UserScript, CUnitScript* _TargetScript, int _skillLevel)
{
	CBasicAttack::GetHit(_UserScript, _TargetScript, _skillLevel);
}
