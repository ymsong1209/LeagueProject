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
	if (m_ProjectileScript)
		delete m_ProjectileScript;

	/*for (int i = 0; i < m_vecSkillObj.size(); i++)
	{
		if (m_vecSkillObj[i] != nullptr)
			delete m_vecSkillObj[i].Get();
	}*/
}

void CSkill::tick()
{
	if (m_fCurCoolDown > 0.0f)
			m_fCurCoolDown -= DT;
	else
		m_fCurCoolDown = 0.0f;
	
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
		return true;
	}

	// �θ��� CSkill�� Use�� ����� ������ ��Ÿ���� �ʱ�ȭ���ְ� 
	// Skill ������ 0�� ���� ��ų ��Ÿ���� �� ���� �ʾ��� ���� ��ų�� ����� �� ���� �մϴ�.
	// ���� �� �ڽ� Ŭ�������� ���� ȣ��������մϴ�!
	// (��ų ��� �Ұ� ���� / ���� ���� ���� ���� ChampionScript���� ����ó�� ���ݴϴ�.)
	// ���⼭ �� ��ų�� ��� ȿ��(ex. ����ü ����)�� �����մϴ�.


}

vector<CGameObject*> CSkill::GetProjectile()
{
	vector<CGameObject*> vecProj = {};

	for (int i = 0; i < m_iProjectileCount; i++)
	{
		vecProj.push_back(m_vecSkillObj[i]->Instantiate());	
	}

	return vecProj;
}

CGameObject* CSkill::GetSkillHitEffect()
{
	if (m_SkillHitEffect == nullptr)
		return nullptr;

	return m_SkillHitEffect->Instantiate();
}

void CSkill::GetHit(CUnitScript* _UserScript, CUnitScript* _TargetScript, int _skillLevel)
{
	// ��ų�� �� ����� è�Ǿ�, �´� ����� è�Ǿ��� ���
	if (_UserScript->GetUnitType() == UnitType::CHAMPION && _TargetScript->GetUnitType() == UnitType::CHAMPION())
	{
		// ��ų �� ����� ���� ��� ��ž ���ο� �ִٸ�
		if (static_cast<CChampionScript*>(_UserScript)->IsInsideEnemyTurretRange())
		{
			// è�Ǿ� ������ �ɼ� true
			static_cast<CChampionScript*>(_UserScript)->SetAttackingChampion(true);
		}
	}
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

