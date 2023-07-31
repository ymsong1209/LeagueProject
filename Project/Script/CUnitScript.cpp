#include "pch.h"
#include "CUnitScript.h"
#include "CSkillMgr.h"
#include "CSkill.h"

#include "CChampionScript.h"

CUnitScript::CUnitScript(UINT ScriptType)
	: CScript(ScriptType)
	, m_fHP(0)
	, m_fMaxHP(0)
	, m_fAttackPower(0)
	, m_fDefencePower(0)
	, m_fAttackSpeed(0)
	, m_fAttackRange(0)
	, m_fMoveSpeed(0)
	, m_vNextPos{}
	, m_fFaceRot(0)
	, m_bUnitDead(false)
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
	// FSM
	if (GetOwner()->Fsm() == nullptr)
		return;
	GetOwner()->Fsm()->ChangeState(L"Idle");

	// ü��
	m_fHP = m_fMaxHP;

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

void CUnitScript::GetHit(SkillType _type, CGameObject* _SkillUser)
{
	// ��ų �Ŵ������� �ش� Ÿ������ ��ų�� �˻��� CSkill Ŭ������ �޾ƿ�.
	CSkill* Skill = CSkillMgr::GetInst()->FindSkill(_type);
	if (Skill != nullptr)
	{
		int SkillLevel = 0;
		
		// ��ų ���� �޾ƿ���
		switch (_type)
		{
		case SkillType::BASIC_ATTACK:
			SkillLevel = 1;
			break;
		case SkillType::JINX_Q:
			_SkillUser->GetScript<CChampionScript>()->GetSkillLevel(1);
			break;
		case SkillType::JINX_W:
			_SkillUser->GetScript<CChampionScript>()->GetSkillLevel(2);
			break;
		case SkillType::JINX_E:
			_SkillUser->GetScript<CChampionScript>()->GetSkillLevel(3);
			break;
		case SkillType::JINX_R:
			_SkillUser->GetScript<CChampionScript>()->GetSkillLevel(4);
			break;
		case SkillType::DARIUS_Q:
			_SkillUser->GetScript<CChampionScript>()->GetSkillLevel(1);
			break;
		case SkillType::DARIUS_W:
			_SkillUser->GetScript<CChampionScript>()->GetSkillLevel(2);
			break;
		case SkillType::DARIUS_E:
			_SkillUser->GetScript<CChampionScript>()->GetSkillLevel(3);
			break;
		case SkillType::DARIUS_R:
			_SkillUser->GetScript<CChampionScript>()->GetSkillLevel(4);
			break;
		}
			
		Skill->GetHit(_SkillUser->GetScript<CUnitScript>(), this, SkillLevel);
	}
}
