#include "pch.h"
#include "CUnitScript.h"

CUnitScript::CUnitScript(UINT ScriptType)
	: CScript(ScriptType)
{
}

CUnitScript::CUnitScript()
	:CScript((UINT)SCRIPT_TYPE::UNITSCRIPT)
{
}

CUnitScript::~CUnitScript()
{
}

void CUnitScript::begin()
{
}

bool CUnitScript::PathFindMove(float _fSpeed, bool _IsRotation)
{
	if (GetOwner()->PathFinder() == nullptr)
		return false;
	else
	{
		Vec3 NextPos = GetOwner()->PathFinder()->GetNextPos();

		if (isnan(NextPos.x))
			return false;
		// NextPos가 유효한 값이라면
		else
		{
			// 현재 위치
			Vec3 CurPos = GetOwner()->Transform()->GetRelativePos();

			// 가야할 방향 구하기
			Vec3 Dir = (NextPos - CurPos).Normalize();

			Vec3 NewPos = CurPos + (Dir * _fSpeed * EditorDT);

			m_vNextPos = NewPos;

			// Pos 반영
			GetOwner()->Transform()->SetRelativePos(NewPos);

			if (_IsRotation) //진행 방향을 바라보도록 회전시키는것까지 요청했다면 회전 진행
			{
				float targetYaw = atan2f(-Dir.x, -Dir.z);
				targetYaw = fmod(targetYaw + XM_PI, 2 * XM_PI) - XM_PI; // 범위를 -π ~ π 로 바꾸기
				float currentYaw = GetOwner()->Transform()->GetRelativeRot().y;
				currentYaw = fmod(currentYaw + XM_PI, 2 * XM_PI) - XM_PI; // 범위를 -π ~ π 로 바꾸기

				// 각도 차이 계산
				float diff = targetYaw - currentYaw;

				// 차이가 π를 넘으면 각도를 반대 방향으로 보간
				if (diff > XM_PI)
					targetYaw -= 2 * XM_PI;
				else if (diff < -XM_PI)
					targetYaw += 2 * XM_PI;

				float lerpFactor = EditorDT * 18.f;

				// Lerp를 이용해 현재 회전 각도와 목표 회전 각도를 보간
				float newYaw = currentYaw + (targetYaw - currentYaw) * lerpFactor;

				m_fFaceRot = newYaw;

				// 새로운 회전 각도를 적용
				GetOwner()->Transform()->SetRelativeRot(Vec3(0.f, newYaw, 0.f));
			}

			// 목표지점에 도착했다면
			if ((NewPos - NextPos).Length() < _fSpeed * EditorDT)
			{
				// 다음 위치 갱신하라고 요청
				GetOwner()->PathFinder()->FindNextPath();
			}
			return true;
		}
	}
}
