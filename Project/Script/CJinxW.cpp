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
	// ��ų ��Ÿ�� �ʱ�ȭ
	if (!CSkill::Use())
		return false;

	// W ����ü �߻�
	// ¡ũ���� ���� ����ĸ� �߻��Ͽ� ó�� ���� ������ ���� ���ظ� ������, 2�� ���� ��ȭ��Ű�� ��ġ�� �巯���ϴ�.

}
