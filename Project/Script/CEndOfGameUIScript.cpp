#include "pch.h"
#include "CEndOfGameUIScript.h"
#include <Engine\CRenderMgr.h>
#include <Engine\CCamera.h>

void CEndOfGameUIScript::begin()
{
}

void CEndOfGameUIScript::tick()
{
	m_fAnimTimer += DT;
	if (m_fAnimTimer >= 4.f)
	{
		m_fTimer += DT; // 델타 타임을 누적하여 경과 시간을 계산

		if (m_bAnimating)
		{
			float progress = min(m_fTimer / animationDuration, 1.f); // 0.0 ~ 1.0 사이의 진행도
			float easeOutProgress = 1 - pow(1 - progress, 2); // ease-out 효과 (시작은 빠르고 점점 느려짐)

			Vec3 Pos = Vec3(0.f, 0.f, 0.f);
			Vec3 PlayerPos = Vec3(Pos.x, Pos.y, 1.f);

			Vec3 YOffsetPos = Vec3(0.f, animationDistance * easeOutProgress, 0.f);
			Vec3 FinalPos = PlayerPos + YOffsetPos;

			Transform()->SetRelativePos(FinalPos);

			if (m_fTimer >= animationDuration) {
				m_bAnimating = false;
			}
		}


		if (m_fTimer <= m_fDuration) {
			float progress = m_fTimer / m_fDuration; // 경과 시간을 기반으로 0~1 사이의 진행도를 계산
			float easeOutProgress = 1 - pow(1 - progress, 2); // ease-out 효과 적용
			m_fAlpha = easeOutProgress; // 계산된 보간 값을 알파 값에 적용
		}
		else {
			m_fAlpha = 1.0f; // 최종 알파 값 설정
		}

		int i = 1;
		MeshRender()->GetDynamicMaterial(0)->SetScalarParam(INT_0, &i);
		MeshRender()->GetDynamicMaterial(0)->SetScalarParam(FLOAT_0, &m_fAlpha);
	}

	else
	{
		m_fAlpha = 0.f;
		int i = 1;
		MeshRender()->GetDynamicMaterial(0)->SetScalarParam(INT_0, &i);
		MeshRender()->GetDynamicMaterial(0)->SetScalarParam(FLOAT_0, &m_fAlpha);
	}
	
}

CEndOfGameUIScript::CEndOfGameUIScript()
	:CScript((UINT)SCRIPT_TYPE::ENDOFGAMEUISCRIPT)
	, m_fCurTimer(0.f)
	, m_fAlpha(0.f)
	, m_fTimer(0.f)
	, m_fDuration(1.2f)
	, m_bAnimating(true)
	, animationDuration(1.2f)
	, animationDistance(40.f)
{
}

CEndOfGameUIScript::~CEndOfGameUIScript()
{
}
