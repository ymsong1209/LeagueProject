#pragma once
#include "CUnitScript.h"



    
//// 소환사 주문(임시)
//enum class SUMMONERS_SPELL
//{
//    FLASH,      // 점멸
//    HEAL,       // 회복
//    GHOST,      // 유체화
//    IGNITE,     // 점화
//    SMITE,      // 강타
//    EXHAUST,    // 탈진
//    CLEANSE,    // 정화
//};



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
    float                   m_fExp;             // 경험치

    float                   m_fRespawnTime;     // 부활 대기시간
   
    SUMMONERS_SPELL*        m_EquippedSpell;    // 장착 소환사 주문(2칸 배열)

    bool                    m_bIsInsideEnemyTurretRange;
    bool                    m_bIsAttackingChampion;


 // =========== Script   ============
public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _Other) override;
    virtual void EndOverlap(CCollider2D* _Other) override;

    // ============ Champion =============
public:
    bool    CheckDeath();
    void    CheckStatus();  // 현재 상태 확인
    void    GetInput();     // 입력 받기
    void    CheckSkills();  // 스킬 체크
    void    Move();
    bool    IsAttackingChampion() { return m_bIsAttackingChampion; }
    bool    IsInsideEnemyTurretRange() { return m_bIsInsideEnemyTurretRange; }
    float    GetExp() { return m_fExp; }


    
    // =========== Skill     ==============
public:
    virtual void Skill1() {};
    virtual void Skill2() {};
    virtual void Skill3() {};
    virtual void Skill4() {};


public:
    int     GetLevel() { return m_iLevel; }

    void SetInsideEnemyTurretRange(bool _b) { m_bIsInsideEnemyTurretRange = _b; }
    void SetAttackingChampion(bool _b) { m_bIsAttackingChampion = _b; }
};
