#pragma once
#include "CUnitScript.h"



    
//// ��ȯ�� �ֹ�(�ӽ�)
//enum class SUMMONERS_SPELL
//{
//    FLASH,      // ����
//    HEAL,       // ȸ��
//    GHOST,      // ��üȭ
//    IGNITE,     // ��ȭ
//    SMITE,      // ��Ÿ
//    EXHAUST,    // Ż��
//    CLEANSE,    // ��ȭ
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
    int                     m_iLevel;           // ����
    float                   m_fExp;             // ����ġ

    float                   m_fRespawnTime;     // ��Ȱ ���ð�
   
    SUMMONERS_SPELL*        m_EquippedSpell;    // ���� ��ȯ�� �ֹ�(2ĭ �迭)

    bool                    m_bIsAttackingChampion;


 // =========== Script   ============
public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _Other) override;

    // ============ Champion =============
public:
    bool    CheckDeath();
    void    CheckStatus();  // ���� ���� Ȯ��
    void    GetInput();     // �Է� �ޱ�
    void    CheckSkills();  // ��ų üũ
    void    Move();
    bool    IsAttackingChampion() { return m_bIsAttackingChampion; }
    float    GetExp() { return m_fExp; }


    
    // =========== Skill     ==============
public:
    virtual void Skill1() {};
    virtual void Skill2() {};
    virtual void Skill3() {};
    virtual void Skill4() {};


public:
    int     GetLevel() { return m_iLevel; }
};
