#include "pch.h"
#include "CScript.h"


CScript::CScript(UINT _ScriptType)
	: CComponent(COMPONENT_TYPE::SCRIPT)	
	, m_iScriptType(_ScriptType)
{
}

CScript::~CScript()
{
}

void CScript::AddScriptParam(SCRIPT_PARAM eParam, void* _pData, const string& _Desc)
{
	m_vecParam.push_back(tScriptParam{ eParam , _pData , _Desc });
}

void CScript::AddScriptTexParam(Ptr<CTexture>& _tex, const string& _Desc)
{
	m_vecTexParam.push_back(tScriptTexParam{ _tex,_Desc });
}



void CScript::PathFindMove(float _fSpeed, bool _IsRotation)
{
	if (GetOwner()->PathFinder() != nullptr)
	{
		Vec3 NextPos = GetOwner()->PathFinder()->GetNextPos();

		// NextPos가 유효한 값이라면
		if (!isnan(NextPos.x))
		{
			// 현재 위치
			Vec3 CurPos = GetOwner()->Transform()->GetRelativePos();

			// 가야할 방향 구하기
			Vec3 Dir = (NextPos - CurPos).Normalize();

			Vec3 NewPos = CurPos + (Dir * _fSpeed * EditorDT);

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


				// 새로운 회전 각도를 적용
				GetOwner()->Transform()->SetRelativeRot(Vec3(0.f, newYaw, 0.f));
			}


			// 목표지점에 도착했다면
			if ((NewPos - NextPos).Length() < _fSpeed * EditorDT)
			{
				// 다음 위치 갱신하라고 요청
				GetOwner()->PathFinder()->FindNextPath();
			}
		}
	}
}



void CScript::SetScriptTexParam(Ptr<CTexture> pCurTex, const string& _Desc)
{
	for (size_t i = 0; i < m_vecTexParam.size(); ++i) {
		if (m_vecTexParam[i].strDesc == _Desc) {
			m_vecTexParam[i].tex = pCurTex;
		}
	}
}