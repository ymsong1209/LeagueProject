#include "pch.h"
#include "CUnitScript.h"
#include "CSkillMgr.h"
#include "CSkill.h"

#include "CChampionScript.h"
#include "CTimedEffect.h"

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
	, m_fMoveSpeedFactor(1)
	, m_bUnitDead(false)
	, m_ChampType(ChampionType::NONE)
{
}

CUnitScript::CUnitScript()
	:CScript((UINT)SCRIPT_TYPE::UNITSCRIPT)
{
}

CUnitScript::~CUnitScript()
{
	Safe_Del_Array(m_Skill);
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

void CUnitScript::tick()
{
	CheckTimedEffect();
	CheckCC();
}

void CUnitScript::CheckTimedEffect()
{
	for (auto it = m_TimedEffectList.begin(); it != m_TimedEffectList.end(); )
	{
		// ���� �ð��� ���� ȿ�� ����
		if ((*it)->isFinished())
		{
			delete* it; 
			it = m_TimedEffectList.erase(it);  // ����Ʈ���� ����
		}
		else
		{
			(*it)->tick();
			it++;
		}
	}
}

void CUnitScript::CheckCC()
{
	// �ɷ��ִ� CC�⿡ ���� �ൿ ���� ����

	if ((m_eCurCC & CC::SLOW) != 0)  // õõ�� ������
	{
		// �̵��ӵ� ����
		m_fMoveSpeedFactor = 0.5f;
	}
	else
	{
		m_fMoveSpeedFactor = 1.f;
	}

	if ((m_eCurCC & CC::SILENCE) != 0) // ħ�� ����
	{
		RestrictAction(RESTRAINT::CAN_USE_SKILL);	// ��ų ��� �Ұ�
	}

	if ((m_eCurCC & CC::ROOT) != 0) // �ӹ� ����
	{
		RestrictAction(RESTRAINT::CAN_MOVE);	// ������ �Ұ�
	}

	if ((m_eCurCC & CC::STUN) != 0) // ���� ����
	{
		m_eRestraint = RESTRAINT::BLOCK; // ��� �ൿ ����
	}

	if ((m_eCurCC & CC::AIRBORNE) != 0) // ��� ����
	{
		m_eRestraint = RESTRAINT::BLOCK; // ��� �ൿ ����
	}
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

			float Speed = _fSpeed * m_fMoveSpeedFactor;

			Vec3 NewPos = CurPos + (Dir * Speed * EditorDT);

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

void CUnitScript::GetHit(SkillType _type, CGameObject* _SkillTarget, CGameObject* _SkillUser, int _SkillLevel)
{
	// ��ų �Ŵ������� �ش� Ÿ������ ��ų�� �˻��� CSkill Ŭ������ �޾ƿ�.
	CSkill* Skill = CSkillMgr::GetInst()->FindSkill(_type);
	if (Skill != nullptr)
	{
		CUnitScript* TargetScript = _SkillTarget->GetScript<CUnitScript>();

		Skill->GetHit(_SkillUser->GetScript<CUnitScript>(), TargetScript, _SkillLevel);
	}
}

void CUnitScript::RestrictAction(RESTRAINT restriction)
{
	m_eRestraint = static_cast<RESTRAINT>(m_eRestraint & ~restriction);
}

void CUnitScript::ApplyCC(CC _ccType)
{
	m_eCurCC = static_cast<CC>(static_cast<int>(m_eCurCC) | static_cast<int>(_ccType));
}

void CUnitScript::RemoveCC(CC _ccType)
{
	m_eCurCC = static_cast<CC>(static_cast<int>(m_eCurCC) & ~static_cast<int>(_ccType));
}
