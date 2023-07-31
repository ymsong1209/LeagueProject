#pragma once


class CUnitScript;
class CPrefab;
#include <Engine\ptr.h>

class CSkill
{
public:
	CSkill();
	virtual	~CSkill();

protected:
	wstring					m_strSkillName;	// 스킬명

	float					m_fCoolDown;	// 쿨타임
	float					m_fCurCoolDown;	// 현재 쿨타임

	int						m_iUserID;		// 사용자 ID
	int						m_iTargetID;	// 타겟 ID

	SkillType				m_eSkillType;	// 스킬 타입

	float					m_fSkillRange;	// 스킬 사거리
	//float					m_fDamage;		// 스킬 기본 데미지
	float					m_fCost;        // 사용 자원(마나 등)

	int						m_iLevel;		// 스킬 레벨
	int						m_iMaxLevel;	// 최대 레벨 (QWE : 5, R : 3)

	CUnitScript*			m_OwnerScript;	// 챔피언 스크립트

	vector<Ptr<CPrefab>>			m_vecSkillObj;	// 스킬 사용 시 생성할 투사체 등

	// ======== 도트딜 / 상태이상 관련 ========== 
	bool					m_bIsDot;		// 지속하여 데미지/상태이상을 거는지 여부
	float					m_fDotTime;		// 지속시킬 시간
	int						m_iTickCount;	// 지속시간 동안 몇번에 나누어 데미지를 줄 지
	float					m_fDotDamage;	// 도트 데미지
	CC						m_eCC;			// 지속시킬 CC기

public:
	virtual void tick() = 0;		// 쿨타임 계산해줌. 필수!
	virtual bool Use() = 0;			// 각 스킬마다 다른 사용 효과 작성. 필수!
	
	virtual	void GetHit(CUnitScript* _UserScript, CUnitScript* _TargetScript) {};	// 투사체 등이 맞았을 때 호출되는 함수

public:
	// 챔피언 스크립트 생성자에서 스킬 생성 후 Owner 등록
	void SetOwnerScript(CUnitScript* _script) { m_OwnerScript = _script; }

	// 스킬 사용가능 여부 (쿨타임만)
	bool	IsReady() { return m_fCurCoolDown <= 0.0f; }

	wstring GetSkillName() { return m_strSkillName; }

	SkillType GetSkillType() { return m_eSkillType; }
	int		GetUserID() { return m_iUserID; }
	int		GetTargetID() { return m_iTargetID; }

	void SetUserID(int _i) { m_iUserID = _i; }
	void SetTargetID(int _i) { m_iTargetID = _i; }

	float	GetCurCoolDown() { return m_fCurCoolDown; }
	float	GetCoolDown() { return m_fCoolDown; }
	float	GetSkillRange() { return m_fSkillRange; }
	float	GetCost() { return m_fCost; }

	int		GetLevel() { return m_iLevel; }

	void	SetProjectileObj(Ptr<CPrefab> _prefab) { m_vecSkillObj.push_back(_prefab); }

};

