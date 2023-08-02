#include "pch.h"
#include "CSkill.h"
#include <Engine\CTimeMgr.h>
#include <Engine\CPrefab.h>
#include <Engine\CGameObject.h>
#include <Engine\CRenderMgr.h>
#include <Engine\CCamera.h>
#include <Engine\CTransform.h>

CSkill::CSkill()
	: m_strSkillName{}
	, m_fCurCoolDown(0.0f)
	, m_iLevel(1)
	, m_OwnerScript(nullptr)
	, m_vecSkillObj{}
{
}

CSkill::~CSkill()
{
}

void CSkill::tick()
{
	if (m_fCurCoolDown > 0.0f)
	{
		if (m_fCurCoolDown <= 0.0f)
			m_fCurCoolDown = 0.0f;
		else
			m_fCurCoolDown -= DT;
	}
	// �θ��� CSkill�� tick�� �⺻������ �� ������ ��Ÿ���� ������ֱ� ������ 
	// �ڽ� Ŭ�������� �� ���� ȣ��������մϴ�.
	// �� �ܿ�, �⺻ ����ȿ��(��� �ð� ���� ���ݷ� ��� ��)�� �ִ� ��ų�� ���
	// ���� �Լ��� tick�� �����ּ���.
}

bool CSkill::Use()
{
	// ��ų ������ 0�̰ų� ��Ÿ���� �� ���� ���� ��� ��� �Ұ�
	if (m_iLevel == 0 || m_fCurCoolDown > 0.0f)
	{
		return false;
	}
	else
	{
		// ��Ÿ�� �ʱ�ȭ
		m_fCurCoolDown = m_fCoolDown;

		return true;
	}

	// �θ��� CSkill�� Use�� ����� ������ ��Ÿ���� �ʱ�ȭ���ְ� 
	// Skill ������ 0�� ���� ��ų ��Ÿ���� �� ���� �ʾ��� ���� ��ų�� ����� �� ���� �մϴ�.
	// ���� �� �ڽ� Ŭ�������� ���� ȣ��������մϴ�!
	// (��ų ��� �Ұ� ���� / ���� ���� ���� ���� ChampionScript���� ����ó�� ���ݴϴ�.)
	// ���⼭ �� ��ų�� ��� ȿ��(ex. ����ü ����)�� �����մϴ�.


}

vector<CGameObject*>& CSkill::GetProjectile()
{
	vector<CGameObject*> vecProj = {};

	for (int i = 0; i < m_iProjectileCount; i++)
	{
		vecProj.push_back(m_vecSkillObj[i]->Instantiate());	
	}

	return vecProj;
}
Vec3 CSkill::GetMousePos()
{
	CCamera* MainCam = CRenderMgr::GetInst()->GetMainCam();
	tRay ray = MainCam->GetRay();

	CGameObject* Map = CUR_LEVEL->FindObjectByName(L"LoLMapCollider");
	IntersectResult result = MainCam->IsCollidingBtwRayRect(ray, Map);
	Vec3 MousePos = result.vCrossPoint;	// ���콺 ��ǥ

	return MousePos;
}

Vec3 CSkill::GetMouseDir()
{
	CCamera* MainCam = CRenderMgr::GetInst()->GetMainCam();
	tRay ray = MainCam->GetRay();

	CGameObject* Map = CUR_LEVEL->FindObjectByName(L"LoLMapCollider");
	IntersectResult result = MainCam->IsCollidingBtwRayRect(ray, Map);
	Vec3 TargetPos = result.vCrossPoint;	// ���콺 ��ǥ

	// �� ��ǥ ������ ������ ����
	Vec3 MyPos = m_UserObj->Transform()->GetRelativePos();

	Vec3 MouseDir = (TargetPos - MyPos).Normalize();
	return MouseDir;
}

