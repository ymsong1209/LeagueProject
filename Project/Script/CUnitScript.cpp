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

	// 체력
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
		// 지속 시간이 끝난 효과 제거
		if ((*it)->isFinished())
		{
			delete* it; 
			it = m_TimedEffectList.erase(it);  // 리스트에서 제거
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
	// 걸려있는 CC기에 따라 행동 제약 변경

	if ((m_eCurCC & CC::SLOW) != 0)  // 천천히 움직임
	{
		// 이동속도 감소
		m_fMoveSpeedFactor = 0.5f;
	}
	else
	{
		m_fMoveSpeedFactor = 1.f;
	}

	if ((m_eCurCC & CC::SILENCE) != 0) // 침묵 상태
	{
		RestrictAction(RESTRAINT::CAN_USE_SKILL);	// 스킬 사용 불가
	}

	if ((m_eCurCC & CC::ROOT) != 0) // 속박 상태
	{
		RestrictAction(RESTRAINT::CAN_MOVE);	// 움직임 불가
	}

	if ((m_eCurCC & CC::STUN) != 0) // 스턴 상태
	{
		m_eRestraint = RESTRAINT::BLOCK; // 모든 행동 제약
	}

	if ((m_eCurCC & CC::AIRBORNE) != 0) // 에어본 상태
	{
		m_eRestraint = RESTRAINT::BLOCK; // 모든 행동 제약
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
		// NextPos가 유효한 값이라면
		else
		{
			// 현재 위치
			Vec3 CurPos = GetOwner()->Transform()->GetRelativePos();

			// 가야할 방향 구하기
			Vec3 Dir = (NextPos - CurPos).Normalize();

			float Speed = _fSpeed * m_fMoveSpeedFactor;

			Vec3 NewPos = CurPos + (Dir * Speed * EditorDT);

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

void CUnitScript::GetHit(SkillType _type, CGameObject* _SkillTarget, CGameObject* _SkillUser, int _SkillLevel)
{
	// 스킬 매니저에서 해당 타입으로 스킬을 검색해 CSkill 클래스를 받아옴.
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
