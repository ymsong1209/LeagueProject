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
	Vec3 vCurPos = Transform()->GetRelativePos();
	float duration = 0.125f; // 1/8��
	if (m_bRcvMove && vCurPos != m_vMovePos)
	{
		if (m_fT == 0) // �̵��� ó�� ���۵Ǹ� t�� �ʱ�ȭ
		{
			m_fT = DT / 0.125f;
		}
		else // �׷��� ������ t�� ������Ʈ
		{
			m_fT += DT / 0.125f;
		}

		if (m_fT > 1) m_fT = 1;

		Vec3 NewPos = vCurPos + (m_vMovePos - vCurPos) * m_fT;
		Transform()->SetRelativePos(NewPos);

		if (m_fT >= 1)
		{
			m_bRcvMove = false;
			m_fT = 0; // ���� �̵��� ���� t�� 0���� �ʱ�ȭ
		}
	}
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
