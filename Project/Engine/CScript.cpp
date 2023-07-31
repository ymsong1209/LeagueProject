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

		// NextPos�� ��ȿ�� ���̶��
		if (!isnan(NextPos.x))
		{
			// ���� ��ġ
			Vec3 CurPos = GetOwner()->Transform()->GetRelativePos();

			// ������ ���� ���ϱ�
			Vec3 Dir = (NextPos - CurPos).Normalize();

			Vec3 NewPos = CurPos + (Dir * _fSpeed * EditorDT);

			// Pos �ݿ�
			GetOwner()->Transform()->SetRelativePos(NewPos);

			if (_IsRotation) //���� ������ �ٶ󺸵��� ȸ����Ű�°ͱ��� ��û�ߴٸ� ȸ�� ����
			{
				float targetYaw = atan2f(-Dir.x, -Dir.z);
				targetYaw = fmod(targetYaw + XM_PI, 2 * XM_PI) - XM_PI; // ������ -�� ~ �� �� �ٲٱ�
				float currentYaw = GetOwner()->Transform()->GetRelativeRot().y;
				currentYaw = fmod(currentYaw + XM_PI, 2 * XM_PI) - XM_PI; // ������ -�� ~ �� �� �ٲٱ�

				// ���� ���� ���
				float diff = targetYaw - currentYaw;

				// ���̰� �� ������ ������ �ݴ� �������� ����
				if (diff > XM_PI) 
					targetYaw -= 2 * XM_PI;
				else if (diff < -XM_PI) 
					targetYaw += 2 * XM_PI;

				float lerpFactor = EditorDT * 18.f;

				// Lerp�� �̿��� ���� ȸ�� ������ ��ǥ ȸ�� ������ ����
				float newYaw = currentYaw + (targetYaw - currentYaw) * lerpFactor;


				// ���ο� ȸ�� ������ ����
				GetOwner()->Transform()->SetRelativeRot(Vec3(0.f, newYaw, 0.f));
			}


			// ��ǥ������ �����ߴٸ�
			if ((NewPos - NextPos).Length() < _fSpeed * EditorDT)
			{
				// ���� ��ġ �����϶�� ��û
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