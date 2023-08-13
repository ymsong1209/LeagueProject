#include "pch.h"
#include "CUnitScript.h"
#include "CSkillMgr.h"
#include "CSkill.h"

#include "CChampionScript.h"
#include "CTimedEffect.h"

CUnitScript::CUnitScript(UINT ScriptType)
	: CScript(ScriptType)
	, m_fHP(0)
	, m_fMaxHP(50)
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
	, m_iLevel(1)
{
}

CUnitScript::CUnitScript()
	:CScript((UINT)SCRIPT_TYPE::UNITSCRIPT)
	, m_fHP(0)
	, m_fMaxHP(50)
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
	, m_iLevel(1)
{
}

CUnitScript::~CUnitScript()
{
	Safe_Del_Array(m_Skill);
}

void CUnitScript::begin()
{
	// 체력
	m_fHP = m_fMaxHP;
	m_fMP = m_fMaxMP;

	// CC & Restraint
	m_eCurCC = CC::NO_CC;
	m_eRestraint = RESTRAINT::NO_RESTRAINT;

}

void CUnitScript::tick()
{
	CheckTimedEffect();
	CheckCC();

	// 가짜는 방장이 아닌 컴에서 플레이어를 제외한 모든 오브젝트
	// 방장 컴에서는 방장과 오브젝트를 제외한 모든 플레이어
	// 이 Unit이 본인 플레이어면 안함. -> 즉 본인플레이어 아니면 다 해야함.
	// 이 프로그램이 방장 클라인데, 이 Unit은 방장이 아니면 안함. -> 즉 방장컴 objects들은 안함.(진짜니까)



	if (CSendServerEventMgr::GetInst()->GetMyPlayer() == nullptr) return;

	if (CSendServerEventMgr::GetInst()->GetMyPlayer()->GetScript<CUnitScript>()->IsHost()) // 방장일 경우 // 방장 외 챔피언은 보간 (그외X)
	{
		if ((m_eUnitType == UnitType::CHAMPION) && !m_bHost)
		{
			Vec3 vCurPos = Transform()->GetRelativePos();
			float duration = 0.08f; // 1/10초

			if (m_bRcvMove && vCurPos != m_vMovePos)
			{
				// 목적지까지의 거리와 방향을 계산
				Vec3 direction = m_vMovePos - vCurPos;
				float totalDistance = direction.Length();
				direction.Normalize();

				// 이동해야 할 거리를 계산
				float moveDistance = totalDistance * (DT / duration);

				// 새 위치를 계산
				Vec3 NewPos = vCurPos + direction * moveDistance;
				Transform()->SetRelativePos(NewPos);

				// 회전값을 계산 (간단한 예시로 회전 방향은 m_vMoveDir을 사용)
				Vec3 currentRot = Transform()->GetRelativeRot(); // 현재 오일러 각
				Vec3 targetRot = m_vMoveDir; // 목표 오일러 각 (예시로 사용)
				Vec3 NewRot = currentRot + (targetRot - currentRot) * (DT / duration);
				Transform()->SetRelativeRot(NewRot);

				// 목적지에 도달했는지 확인
				if ((NewPos - m_vMovePos).Length() <= moveDistance)
				{
					m_bRcvMove = false;
				}
			}
		}
	}
	else // 방장이 아닐경우
	{
		if (CSendServerEventMgr::GetInst()->GetMyPlayer() != GetOwner())
		{
			Vec3 vCurPos = Transform()->GetRelativePos();
			float duration = 0.08f; // 1/10초

			if (m_bRcvMove && vCurPos != m_vMovePos)
			{
				// 목적지까지의 거리와 방향을 계산
				Vec3 direction = m_vMovePos - vCurPos;
				float totalDistance = direction.Length();
				direction.Normalize();

				// 이동해야 할 거리를 계산
				float moveDistance = totalDistance * (DT / duration);

				// 새 위치를 계산
				Vec3 NewPos = vCurPos + direction * moveDistance;
				Transform()->SetRelativePos(NewPos);

				// 회전값을 계산 (간단한 예시로 회전 방향은 m_vMoveDir을 사용)
				Vec3 currentRot = Transform()->GetRelativeRot(); // 현재 오일러 각
				Vec3 targetRot = m_vMoveDir; // 목표 오일러 각 (예시로 사용)
				Vec3 NewRot = currentRot + (targetRot - currentRot) * (DT / duration);
				Transform()->SetRelativeRot(NewRot);

				// 목적지에 도달했는지 확인
				if ((NewPos - m_vMovePos).Length() <= moveDistance)
				{
					m_bRcvMove = false;
				}
			}
		}
	}
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
	// 
	// 초기화: 모든 제약 상태 제거
	m_fMoveSpeedFactor = 1.f;
	RemoveRestraint(RESTRAINT::CANNOT_SKILL);
	RemoveRestraint(RESTRAINT::CANNOT_MOVE);
	RemoveRestraint(RESTRAINT::BLOCK);

	// SLOW
	if ((m_eCurCC & CC::SLOW) != 0)
	{
		m_fMoveSpeedFactor = 0.5f;
	}

	// 침묵 상태
	if ((m_eCurCC & CC::SILENCE) != 0)
	{
		ApplyRestraint(RESTRAINT::CANNOT_SKILL);
	}

	// 속박 상태
	if ((m_eCurCC & CC::ROOT) != 0)
	{
		ApplyRestraint(RESTRAINT::CANNOT_MOVE); 
	}

	// 스턴 상태
	if ((m_eCurCC & CC::STUN) != 0)
	{
		ApplyRestraint(RESTRAINT::BLOCK);
	}
	
	// 에어본 상태
	if ((m_eCurCC & CC::AIRBORNE) != 0) 
	{
		if (m_bAirBorneActive == false) 
		{
			GetOwner()->PathFinder()->ClearPath();
			Vec3 CurPos = GetOwner()->Transform()->GetRelativePos();
			m_vAirBorneStartPos = CurPos;
			GetOwner()->Transform()->SetRelativePos(Vec3(CurPos.x, CurPos.y + 50.f, CurPos.z));
				
			m_bAirBorneActive = true;
		}
		else 
		{
			Vec3 CurPos = GetOwner()->Transform()->GetRelativePos();
			m_fAirBorneVelocity -= 2.f * DT;
			GetOwner()->Transform()->SetRelativePos(Vec3(CurPos.x, CurPos.y + m_fAirBorneVelocity, CurPos.z));
			if (CurPos.y + m_fAirBorneVelocity < m_vAirBorneStartPos.y)
			{
				GetOwner()->Transform()->SetRelativePos(Vec3(CurPos.x, m_vAirBorneStartPos.y, CurPos.z));
			}
		}
		
		ApplyRestraint(RESTRAINT::BLOCK); // 모든 행동 제약
	}
	else if(m_bAirBorneActive)
	{
		m_bAirBorneActive = false;
		m_fAirBorneVelocity = 0.f;
	}
}

void CUnitScript::SetCurCC(CC _cc)
{
	m_eCurCC = (UINT)_cc;
}

void CUnitScript::ApplyCC(CC _ccType)
{
	m_eCurCC |= _ccType;
}

void CUnitScript::RemoveCC(CC _ccType)
{
	m_eCurCC &= ~_ccType;
}

void CUnitScript::ApplyRestraint(RESTRAINT restraint)
{
	m_eRestraint |= restraint;
}

void CUnitScript::RemoveRestraint(RESTRAINT restraint)
{
	m_eRestraint &= ~restraint;
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


void CUnitScript::SaveToLevelFile(FILE* _File)
{
	SaveWString(m_strNickname, _File);
	fwrite(&m_eUnitType, sizeof(UnitType), 1, _File);
	fwrite(&m_eFaction, sizeof(Faction), 1, _File);

	fwrite(&m_eCurCC, sizeof(CC), 1, _File);
	fwrite(&m_eRestraint, sizeof(RESTRAINT), 1, _File);

	fwrite(&m_fHP, sizeof(float), 1, _File);
	fwrite(&m_fMaxHP, sizeof(float), 1, _File);
	fwrite(&m_fMP, sizeof(float), 1, _File);
	fwrite(&m_fMaxMP, sizeof(float), 1, _File);

	fwrite(&m_fAttackPower, sizeof(float), 1, _File);
	fwrite(&m_fDefencePower, sizeof(float), 1, _File);

	fwrite(&m_fAttackSpeed, sizeof(float), 1, _File);
	fwrite(&m_fAttackRange, sizeof(float), 1, _File);
	fwrite(&m_fMoveSpeed, sizeof(float), 1, _File);
	fwrite(&m_fMoveSpeedFactor, sizeof(float), 1, _File);
}

void CUnitScript::LoadFromLevelFile(FILE* _File)
{
	LoadWString(m_strNickname, _File);
	fread(&m_eUnitType, sizeof(UnitType), 1, _File);
	fread(&m_eFaction, sizeof(Faction), 1, _File);

	fread(&m_eCurCC, sizeof(CC), 1, _File);
	fread(&m_eRestraint, sizeof(RESTRAINT), 1, _File);

	fread(&m_fHP, sizeof(float), 1, _File);
	fread(&m_fMaxHP, sizeof(float), 1, _File);
	fread(&m_fMP, sizeof(float), 1, _File);
	fread(&m_fMaxMP, sizeof(float), 1, _File);

	fread(&m_fAttackPower, sizeof(float), 1, _File);
	fread(&m_fDefencePower, sizeof(float), 1, _File);

	fread(&m_fAttackSpeed, sizeof(float), 1, _File);
	fread(&m_fAttackRange, sizeof(float), 1, _File);
	fread(&m_fMoveSpeed, sizeof(float), 1, _File);
	fread(&m_fMoveSpeedFactor, sizeof(float), 1, _File);
}

void CUnitScript::SaveToLevelJsonFile(Value& _objValue, Document::AllocatorType& allocator)
{
	_objValue.AddMember("NickName", SaveWStringJson(m_strNickname, allocator), allocator);
	_objValue.AddMember("UnitType", (UINT)m_eUnitType, allocator);
	_objValue.AddMember("Faction", (UINT)m_eFaction, allocator);

	_objValue.AddMember("CurCC", (UINT)m_eCurCC, allocator);
	_objValue.AddMember("Restraint", (UINT)m_eRestraint, allocator);

	_objValue.AddMember("HP", m_fHP, allocator);
	_objValue.AddMember("MaxHP", m_fMaxHP, allocator);
	_objValue.AddMember("MP", m_fMP, allocator);
	_objValue.AddMember("MaxMP", m_fMaxMP, allocator);

	_objValue.AddMember("AttackPower", m_fAttackPower, allocator);
	_objValue.AddMember("DefencePower", m_fDefencePower, allocator);

	_objValue.AddMember("AttackSpeed", m_fAttackSpeed, allocator);
	_objValue.AddMember("AttackRange", m_fAttackRange, allocator);
	_objValue.AddMember("MoveSpeed", m_fMoveSpeed, allocator);
	_objValue.AddMember("MoveSpeedFactor", m_fMoveSpeedFactor, allocator);
}

void CUnitScript::LoadFromLevelJsonFile(const Value& _componentValue)
{
	const Value& objNameValue = _componentValue["NickName"];
	if (objNameValue.IsString())
	{
		std::wstring objName = StrToWStr(objNameValue.GetString());
		m_strNickname = objName;
	}
	m_eUnitType = (UnitType)_componentValue["UnitType"].GetUint();
	m_eFaction = (Faction)_componentValue["Faction"].GetUint();

	m_eCurCC = (CC)_componentValue["CurCC"].GetUint();
	m_eRestraint = (RESTRAINT)_componentValue["Restraint"].GetUint();
	
	m_fHP = _componentValue["HP"].GetFloat();
	m_fMaxHP = _componentValue["MaxHP"].GetFloat();
	m_fMP = _componentValue["MP"].GetFloat();
	m_fMaxMP = _componentValue["MaxMP"].GetFloat();

	m_fAttackPower = _componentValue["AttackPower"].GetFloat();
	m_fDefencePower = _componentValue["DefencePower"].GetFloat();

	m_fAttackSpeed = _componentValue["AttackSpeed"].GetFloat();
	m_fAttackRange = _componentValue["AttackRange"].GetFloat();
	m_fMoveSpeed = _componentValue["MoveSpeed"].GetFloat();
	m_fMoveSpeedFactor = _componentValue["MoveSpeedFactor"].GetFloat();
}
