#pragma once
#include <Engine\CScript.h>

class CSkill;
class CTimedEffect;

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

    int                     m_iLevel;           // 레벨

    float                   m_fHP;              // 체력
    float                   m_fMaxHP;           // 전체 체력
    float                   m_fMP;
    float                   m_fMaxMP;

    float                   m_fAttackPower;     // 공격력
    float                   m_fDefencePower;    // 방어력


    float                   m_fAttackSpeed;     // 공격(평타)속도
    float                   m_fAttackRange;     // 공격(평타)사거리
    float                   m_fMoveSpeed;       // 이동속도
    float                   m_fMoveSpeedFactor; // 이동속도 변화율

    Vec3                    m_vNextPos;
    float                   m_fFaceRot;

    ChampionType            m_ChampType; 
    Lane                    m_eLane;
    bool                    m_bUnitDead;        // 유닛이 죽었는지(HP 0 이하)
    vector<CTimedEffect*>   m_TimedEffectList;  // 지속딜 / CC기 관련 리스트

    bool                    m_bRcvMove;
    Vec3                    m_vMovePos;
    float                   m_fT;                // 보간 파라미터

public:
    void    SetRcvMove(bool _bool) { m_bRcvMove = _bool; }
    void    SetMovePos(Vec3 _vec3) { m_vMovePos = _vec3; }
   
    virtual void begin() override;
    virtual void tick() override;
    virtual void BeginOverlap(CCollider2D* _collider) override;

    virtual UnitType GetType() { return m_eUnitType; }
    virtual Faction GetFaction() { return m_eFaction; }

public:
    bool IsUnitDead() { return m_bUnitDead; }
    void SetUnitDead(bool _dead) { m_bUnitDead = _dead; }

    void CheckTimedEffect();
    void CheckCC();

    // 오브젝트의 PathFinder 컴포넌트에 남은 경로값이 있을 때, 해당 경로로 이동
    bool PathFindMove(float _fSpeed, bool _IsRotation = true);

public:
    void SetChampType(ChampionType _Type) { m_ChampType = _Type; }
    ChampionType GetChampType() { return m_ChampType; }

    void SetLane(Lane _lane) { m_eLane = _lane; }
    Lane GetLane() { return m_eLane; }

    void  SetCurHP(float _f) { m_fHP = _f; }         // 현재 체력 = 인자값
    void  SetCurHPVar(float _f) { m_fHP += _f; }     // 현재 체력 += 인자값
    void  SetMaxHP(float _f) { m_fMaxHP = _f; }

    void  SetCurMP(float _f) { m_fMP = _f; }
    void  SetCurMPVar(float _f) { m_fMP += _f; }
    void  SetMaxMP(float _f) { m_fMaxMP = _f; }

    void  SetCC(CC _cc) { m_eCurCC = _cc; }
    void  SetLevel(int _level) { m_iLevel = _level; }

    float GetCurHP() { return m_fHP; }
    float GetMaxHP() { return m_fMaxHP; }
    //float GetMaxHP() const { return m_fMaxHP; }

    float GetCurMP() { return m_fMP; }
    float GetMaxMP() const { return m_fMaxMP; }

    CC    GetCC() { return m_eCurCC; }
    int   GetLevel() { return m_iLevel; }

    UINT64           GetServerID() { return m_iServerID; }
    wstring          GetNickname() const { return m_strNickname; }
    bool             IsHost() const { return m_bHost; }
    void             SetUnitType(UnitType _type) { m_eUnitType = _type; }
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
    float   GetMoveSpeed() { return m_fMoveSpeed; }

    // 비동기
    void GetHit(SkillType _type, CGameObject* _SkillTarget, CGameObject* _SkillUser, int _SkillLevel);
    void RestrictAction(RESTRAINT restriction);
    void ApplyCC(CC _ccType);
    void RemoveCC(CC _ccType);

    void AddTimedEffect(CTimedEffect* _effect) { m_TimedEffectList.push_back(_effect); }

    CSkill* GetSkill(int _i) { if (_i < 0 || _i >= 5) return nullptr; return m_Skill[_i]; }
    int     GetSkillLevel(int _i) { return m_SkillLevel[_i]; }

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _FILE) override;

    virtual void SaveToLevelJsonFile(Value& _objValue, Document::AllocatorType& allocator)override;
    virtual void LoadFromLevelJsonFile(const Value& _componentValue)override;
};
