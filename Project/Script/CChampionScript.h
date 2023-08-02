#pragma once
#include "CUnitScript.h"

#ifndef CHAMPION_ENUM_DEFINED
#define CHAMPION_ENUM_DEFINED

// 군중 제어기
enum class CC
{
    NONE = 0,
    SLOW = 1 << 0,
    SILENCE = 1 << 1,
    ROOT = 1 << 2,
    STUN = 1 << 3,
    AIRBORNE = 1 << 4,
};

// 행동 제약
enum RESTRAINT
{
    CAN_MOVE = 1 << 0,
    CAN_ATTACK = 1 << 1,
    CAN_USE_SKILL = 1 << 2,

    DEFAULT = CAN_MOVE | CAN_ATTACK | CAN_USE_SKILL,
    BLOCK = 0,
};

// 소환사 주문(임시)
enum class SUMMONERS_SPELL
{
    FLASH,      // 점멸
    HEAL,       // 회복
    GHOST,      // 유체화
    IGNITE,     // 점화
    SMITE,      // 강타
    EXHAUST,    // 탈진
    CLEANSE,    // 정화
};
#endif

#ifndef CHAMPION_CLASS_DEFINED
#define CHAMPION_CLASS_DEFINED


class CSkill;

class CChampionScript :
    public CUnitScript
{
public:
    CChampionScript(UINT ScriptType);

    CChampionScript();
    virtual ~CChampionScript();

    CLONE(CChampionScript);

protected:
    int                     m_iLevel;           // 레벨
    float                   m_fExp;             // 경험치

    float                   m_fMana;            // 마나
    float                   m_fMaxMana;         // 전체 마나

    float                   m_fRespawnTime;     // 부활 대기시간

    CC                      m_eCurCC;
    RESTRAINT               m_eRestraint;

    CSkill*                 m_Skill[5];
    SUMMONERS_SPELL*        m_EquippedSpell;    // 장착 소환사 주문(2칸 배열)

    bool                    m_bIsAttackingChampion;


 // =========== Script   ============
public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _Other) override;

    // ============ Champion =============
public:
    bool    CheckDeath();
    void    CheckStatus();  // 현재 상태 확인
    void    GetInput();     // 입력 받기
    void    CheckSkills();  // 스킬 체크
    void    Move();
    bool    IsAttackingChampion() { return m_bIsAttackingChampion; }
    


    // 비동기
    void    GetHit(CSkill* _skill);       // 피격시 
    
    // =========== Skill     ==============
public:
    virtual void Skill1() {};
    virtual void Skill2() {};
    virtual void Skill3() {};
    virtual void Skill4() {};


public:
    int GetLevel() { return m_iLevel; }
    float GetDefencePower() { return m_fDefencePower; }
    CSkill* GetSkill(int _i) { if (_i < 0 || _i >= 5) return nullptr; m_Skill[_i]; }
};
#endif
