#include "pch.h"
#include "CPlayerCSUIScript.h"
#include <Engine\CRenderMgr.h>
#include <Engine\CCamera.h>

void CPlayerCSUIScript::begin()
{
}

void CPlayerCSUIScript::tick()
{
	/*m_fTimer += DT;
	if (m_fTimer >= 1.5f)
		DestroyObject(GetOwner());*/

	if (m_bAnimating) {
		m_fTimer += DT;

		float progress = min(m_fTimer / animationDuration, 1.f); // 0.0 ~ 1.0 사이의 진행도
		float easeOutProgress = 1 - pow(1 - progress, 2); // ease-out 효과 (시작은 빠르고 점점 느려짐)

		Vec3 Pos = UICamera->GetMainPlayerUICamPos();
		Vec3 PlayerPos = Vec3(Pos.x, Pos.y, 1.f);

		Vec3 YOffsetPos = Vec3(0.f, 157.f + animationDistance * easeOutProgress, 0.f);
		Vec3 FinalPos = PlayerPos + YOffsetPos;

		Transform()->SetRelativePos(FinalPos);

		if (m_fTimer >= animationDuration) {
			m_bAnimating = false;
			DestroyObject(GetOwner());
		}
	}
}

CPlayerCSUIScript::CPlayerCSUIScript()
	:CScript((UINT)SCRIPT_TYPE::PLAYERCSUISCRIPT)
	, m_bAnimating(true)
	,animationDuration(0.5f)
	,animationDistance(30.f)
{

}

CPlayerCSUIScript::~CPlayerCSUIScript()
{
}
