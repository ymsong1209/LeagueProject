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
	// 부모인 CSkill의 tick은 기본적으로 매 프레임 쿨타임을 계산해주기 때문에 
	// 자식 클래스에서 꼭 먼저 호출해줘야합니다.
	// 그 외에, 기본 지속효과(사용 시간 동안 공격력 상승 등)가 있는 스킬의 경우
	// 관련 함수는 tick에 적어주세요.
}

bool CSkill::Use()
{
	// 스킬 레벨이 0이거나 쿨타임이 다 차지 않은 경우 사용 불가
	if (m_iLevel == 0 || m_fCurCoolDown > 0.0f)
	{
		return false;
	}
	else
	{
		// 쿨타임 초기화
		m_fCurCoolDown = m_fCoolDown;

		return true;
	}

	// 부모인 CSkill의 Use는 사용할 때마다 쿨타임을 초기화해주고 
	// Skill 레벨이 0일 때와 스킬 쿨타임이 다 차지 않았을 때에 스킬을 사용할 수 없게 합니다.
	// 따라서 꼭 자식 클래스에서 먼저 호출해줘야합니다!
	// (스킬 사용 불가 상태 / 마나 부족 등의 경우는 ChampionScript에서 예외처리 해줍니다.)
	// 여기서 각 스킬의 사용 효과(ex. 투사체 생성)를 정의합니다.


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
	Vec3 MousePos = result.vCrossPoint;	// 마우스 좌표

	return MousePos;
}

Vec3 CSkill::GetMouseDir()
{
	CCamera* MainCam = CRenderMgr::GetInst()->GetMainCam();
	tRay ray = MainCam->GetRay();

	CGameObject* Map = CUR_LEVEL->FindObjectByName(L"LoLMapCollider");
	IntersectResult result = MainCam->IsCollidingBtwRayRect(ray, Map);
	Vec3 TargetPos = result.vCrossPoint;	// 마우스 좌표

	// 두 좌표 사이의 방향을 구함
	Vec3 MyPos = m_UserObj->Transform()->GetRelativePos();

	Vec3 MouseDir = (TargetPos - MyPos).Normalize();
	return MouseDir;
}

