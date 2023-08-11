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
    float                   m_fMaxExp;          // 최대 경험치

    int                     m_iKillCount;
    int                     m_iDeathCount;
    int                     m_iAssistCount;     // 안씀

    int                     m_iSkillLevelUpPoint;   // 레벨 업 시 스킬 레벨을 올릴 수 있는 포인트

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
    virtual void    GetInput();     // 입력 받기
    void    CheckSkills();  // 스킬 체크
    void    Move();
    bool    IsAttackingChampion() { return m_bIsAttackingChampion; }
    bool    IsInsideEnemyTurretRange() { return m_bIsInsideEnemyTurretRange; }
    
public:
    float   GetExp() { return m_fExp; }
    float   GetMaxExp() { return m_fMaxExp; }

    int     GetSkillLevelUpPoint() { return m_iSkillLevelUpPoint; }

    void SetInsideEnemyTurretRange(bool _b) { m_bIsInsideEnemyTurretRange = _b; }
    void SetAttackingChampion(bool _b) { m_bIsAttackingChampion = _b; }
};
