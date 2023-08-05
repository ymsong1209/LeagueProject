#pragma once
#include <Engine\CScript.h>

class CSkill;

class CUnitScript :
    public CScript
{
protected:
    CUnitScript(UINT ScriptType);

public:
    CUnitScript();
    virtual ~CUnitScript();

    CLONE(CUnitScript);

protected:
    UINT64                  m_iServerID;        // 서버에게 부여받은 ID
    wstring                 m_strNickname;      // 닉네임
    bool                    m_bHost;            // 방장인지 여부
   
    UnitType                m_eUnitType;        // 유닛 종류
    Faction                 m_eFaction;         // 진영(레드, 블루, 중립)

    CC                      m_eCurCC;
    RESTRAINT               m_eRestraint;
        
    CSkill*                 m_Skill[5];         // 타워/몹은 오직 0번 스킬(평타)만 있음!
    int                     m_SkillLevel[5];    // 타워/몹은 오직 0번 스킬 1렙 고정

    float                   m_fHP;              // 체력
    float                   m_fMaxHP;           // 전체 체력
    float                   m_fMP;
    float                   m_fMaxMP;

    float                   m_fAttackPower;     // 공격력
    float                   m_fDefencePower;    // 방어력


    float                   m_fAttackSpeed;     // 공격(평타)속도
    float                   m_fAttackRange;     // 공격(평타)사거리
    float                   m_fMoveSpeed;       // 이동속도

    Vec3                    m_vNextPos;
    float                   m_fFaceRot;

    bool                    m_bUnitDead;      // 유닛이 죽었는지(HP 0 이하)

public:
    virtual void begin() override;
    virtual void tick() override {};

    virtual UnitType GetType() { return m_eUnitType; }
    virtual Faction GetFaction() { return m_eFaction; }

public:
    bool IsUnitDead() { return m_bUnitDead; }
    void SetUnitDead(bool _dead) { m_bUnitDead = _dead; }

    // 오브젝트의 PathFinder 컴포넌트에 남은 경로값이 있을 때, 해당 경로로 이동
    bool PathFindMove(float _fSpeed, bool _IsRotation = true);

public:
    float GetCurHP() { return m_fHP; }
    float GetMaxHP() { return m_fMaxHP; }
    void  SetCurHP(float _f) { m_fHP = _f; }         // 현재 체력 = 인자값
    void  SetCurHPVar(float _f) { m_fHP += _f; }     // 현재 체력 += 인자값
    float GetMaxHP() const { return m_fMaxHP; }

    float GetCurMP() { return m_fMP; }
    void  SetCurMP(float _f) { m_fMP = _f; }
    void  SetCurMPVar(float _f) { m_fMP += _f; }
    float GetMaxMP() const { return m_fMaxMP; }

    UINT64           GetServerID() { return m_iServerID; }
    wstring          GetNickname() const { return m_strNickname; }
    bool             IsHost() const { return m_bHost; }
    UnitType         GetUnitType() const { return m_eUnitType; }
    Faction          GetFaction() const { return m_eFaction; }

    void             SetServerID(unsigned __int64 serverID) { m_iServerID = serverID; }
    void             SetNickname(wstring nickname) { m_strNickname = nickname; }
    void             SetHost(bool isHost) { m_bHost = isHost; }
    void             SetFaction(Faction faction) { m_eFaction = faction; }

    // 기타 필요한 Getter/Setter 함수 추가
    float  GetAttackPower() { return m_fAttackPower; }
    float  GetDefencePower() { return m_fDefencePower; }
    float   GetAttackRange() { return m_fAttackRange; }

    void   SetAttackPower(float _attackPower) { m_fAttackPower = _attackPower; }
    void   SetDefencePower(float _defencePower) { m_fDefencePower = _defencePower; }
    void    SetAttackRange(float _range) { m_fAttackRange = _range; }

    float   GetFaceRot() { return m_fFaceRot; }

    // 비동기
    void GetHit(SkillType _type, CGameObject* _SkillTarget, CGameObject* _SkillUser, int _SkillLevel);


    CSkill* GetSkill(int _i) { if (_i < 0 || _i >= 5) return nullptr; return m_Skill[_i]; }
    int     GetSkillLevel(int _i) { return m_SkillLevel[_i]; }
};
