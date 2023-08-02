#ifndef _CUNITSRIPT_H
#define _CUNITSRIPT_H
#include <Engine\CScript.h>
// LoL 게임 내에서 플레이어가 상호작용할 수 있는 모든 오브젝트

enum class UnitType
{
    CHAMPION,
    STRUCTURE,
    MINION,
    JUNGLEMONSTER,
    DRAGON,
    BARON,
};

enum class Faction
{
    NONE,
    RED,
    BLUE,
};


enum class Lane
{
    NONE,
    TOP,
    MID,
    BOTTOM,
};

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
    UnitType    m_eUnitType;        // 유닛 종류
    Faction     m_eFaction;         // 진영(레드, 블루, 중립)

    float       m_fHP;              // 체력
    float       m_fMaxHP;           // 전체 체력
    float       m_fAttackPower;     // 공격력
    float       m_fDefencePower;    // 방어력
    float       m_fAttackSpeed;     // 공격(평타)속도
    float       m_fAttackRange;     // 공격(평타)사거리
    float       m_fMoveSpeed;       // 이동속도

    Vec3        m_vNextPos;
    float       m_fFaceRot;

    bool        m_bUnitDead;      // 유닛이 죽었는지(HP 0 이하)

public:
    virtual void begin() override;
    virtual void tick() override {};

    virtual UnitType GetType() { return m_eUnitType; }
    virtual Faction GetFaction() { return m_eFaction; }

public:
    bool IsUnitDead() { return m_bUnitDead; }

    // 오브젝트의 PathFinder 컴포넌트에 남은 경로값이 있을 때, 해당 경로로 이동
    bool PathFindMove(float _fSpeed, bool _IsRotation = true);

public:
    float GetCurHP() { return m_fHP; }
    float GetMaxHP() { return m_fMaxHP; }
    void  SetCurHP(float _f) { m_fHP = _f; }         // 현재 체력 = 인자값
    void  SetCurHPVar(float _f) { m_fHP += _f; }     // 현재 체력 += 인자값

    // 기타 필요한 Getter/Setter 함수 추가
    float   GetAttackPower() { return m_fAttackPower; }
    float   GetDefencePower() { return m_fDefencePower; }
    float   GetAttackRange() { return m_fAttackRange; }

    void    SetAttackPower(float _pow) { m_fAttackPower = _pow; }
    void    SetDefencePower(float _pow) { m_fDefencePower = _pow; }
    void    SetAttackRange(float _range) { m_fAttackRange = _range; }

};
#endif // _CUNITSRIPT_H