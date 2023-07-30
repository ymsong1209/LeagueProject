#pragma once
#include "CUnitScript.h"

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


    float                   m_fRespawnTime;     // 부활 대기시간

    CC                      m_eCurCC;
    RESTRAINT               m_eRestraint;

    CSkill*                 m_Skill[5];
    SUMMONERS_SPELL*        m_EquippedSpell;    // 장착 소환사 주문(2칸 배열)


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

