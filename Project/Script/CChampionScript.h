#pragma once
#include "CUnitScript.h"

// ���� �����
enum CC
{
    NONE_CC = 0,
    SLOW_CC = 1 << 0,
    SILENCE_CC = 1 << 1,
    ROOT_CC = 1 << 2,
    STUN_CC = 1 << 3,
    AIRBORNE_CC = 1 << 4,
};

// �ൿ ����
enum RESTRAINT
{
    CAN_MOVE = 1 << 0,
    CAN_ATTACK = 1 << 1,
    CAN_USE_SKILL = 1 << 2,

    DEFAULT = CAN_MOVE | CAN_ATTACK | CAN_USE_SKILL,
    BLOCK = 0,
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
    int                     m_iLevel;           // ����
    float                   m_fExp;             // ����ġ

    float                   m_fMana;            // ����
    float                   m_fMaxMana;         // ��ü ����

    float                   m_fRespawnTime;     // ��Ȱ ���ð�

    CC                      m_eCurCC;
    RESTRAINT               m_eRestraint;

    CSkill*                 m_Skill[5];
    SUMMONERS_SPELL*        m_EquippedSpell;    // ���� ��ȯ�� �ֹ�(2ĭ �迭)

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


    // �񵿱�
    void    GetHit(CSkill* _skill);       // �ǰݽ� 
    
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
