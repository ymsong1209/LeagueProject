#ifndef _CUNITSRIPT_H
#define _CUNITSRIPT_H
#include <Engine\CScript.h>
// LoL ���� ������ �÷��̾ ��ȣ�ۿ��� �� �ִ� ��� ������Ʈ

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
    UnitType    m_eUnitType;        // ���� ����
    Faction     m_eFaction;         // ����(����, ���, �߸�)

    float       m_fHP;              // ü��
    float       m_fMaxHP;           // ��ü ü��
    float       m_fAttackPower;     // ���ݷ�
    float       m_fDefencePower;    // ����
    float       m_fAttackSpeed;     // ����(��Ÿ)�ӵ�
    float       m_fAttackRange;     // ����(��Ÿ)��Ÿ�
    float       m_fMoveSpeed;       // �̵��ӵ�

    Vec3        m_vNextPos;
    float       m_fFaceRot;

    bool        m_bUnitDead;      // ������ �׾�����(HP 0 ����)

public:
    virtual void begin() override;
    virtual void tick() override {};

    virtual UnitType GetType() { return m_eUnitType; }
    virtual Faction GetFaction() { return m_eFaction; }

public:
    bool IsUnitDead() { return m_bUnitDead; }

    // ������Ʈ�� PathFinder ������Ʈ�� ���� ��ΰ��� ���� ��, �ش� ��η� �̵�
    bool PathFindMove(float _fSpeed, bool _IsRotation = true);

public:
    float GetCurHP() { return m_fHP; }
    float GetMaxHP() { return m_fMaxHP; }
    void  SetCurHP(float _f) { m_fHP = _f; }         // ���� ü�� = ���ڰ�
    void  SetCurHPVar(float _f) { m_fHP += _f; }     // ���� ü�� += ���ڰ�

    // ��Ÿ �ʿ��� Getter/Setter �Լ� �߰�
    float   GetAttackPower() { return m_fAttackPower; }
    float   GetDefencePower() { return m_fDefencePower; }
    float   GetAttackRange() { return m_fAttackRange; }

    void    SetAttackPower(float _pow) { m_fAttackPower = _pow; }
    void    SetDefencePower(float _pow) { m_fDefencePower = _pow; }
    void    SetAttackRange(float _range) { m_fAttackRange = _range; }

};
#endif // _CUNITSRIPT_H