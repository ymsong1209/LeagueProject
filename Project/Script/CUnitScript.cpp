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
		// NextPos�� ��ȿ�� ���̶��
		else
		{
			// ���� ��ġ
			Vec3 CurPos = GetOwner()->Transform()->GetRelativePos();

			// ������ ���� ���ϱ�
			Vec3 Dir = (NextPos - CurPos).Normalize();

			Vec3 NewPos = CurPos + (Dir * _fSpeed * EditorDT);

			m_vNextPos = NewPos;

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

				m_fFaceRot = newYaw;

				// ���ο� ȸ�� ������ ����
				GetOwner()->Transform()->SetRelativeRot(Vec3(0.f, newYaw, 0.f));
			}

			// ��ǥ������ �����ߴٸ�
			if ((NewPos - NextPos).Length() < _fSpeed * EditorDT)
			{
				// ���� ��ġ �����϶�� ��û
				GetOwner()->PathFinder()->FindNextPath();
			}
			return true;
		}
	}
}
