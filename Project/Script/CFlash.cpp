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
	// ��ų ��Ÿ�� �ʱ�ȭ
	if (!CSkill::Use())
		return false;

	// ���콺 �������� 500.f �Ÿ���ŭ �����̵�
	// è�Ǿ��� ���� �ִ� Face ���⵵ ��������

	//m_OwnerScript->GetOwner()->Transform()->..

	// Flash SFX & VFX ���

	return true;
}
