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

	// 체력
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

void CUnitScript::GetHit(SkillType _type, CGameObject* _SkillUser)
{
	// 스킬 매니저에서 해당 타입으로 스킬을 검색해 CSkill 클래스를 받아옴.
	CSkill* Skill = CSkillMgr::GetInst()->FindSkill(_type);
	if (Skill != nullptr)
	{
		int SkillLevel = 0;
		
		// 스킬 레벨 받아오기
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
