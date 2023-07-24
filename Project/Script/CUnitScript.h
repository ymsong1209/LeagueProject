#pragma once
#include <Engine\CScript.h>

// LoL ���� ������ �÷��̾ ��ȣ�ۿ��� �� �ִ� ��� ������Ʈ

class CUnitScript :
    public CScript
{
protected:
    CUnitScript(UINT ScriptType);

public:
    CUnitScript();
    ~CUnitScript();

    CLONE(CUnitScript);

protected:
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

public:
    bool IsUnitDead() { return m_bUnitDead; }

    // ������Ʈ�� PathFinder ������Ʈ�� ���� ��ΰ��� ���� ��, �ش� ��η� �̵�
    bool PathFindMove(float _fSpeed, bool _IsRotation = true);

public:
    float GetCurHP() { return m_fHP; }
    void  SetCurHP(float _f) { m_fHP = _f; }         // ���� ü�� = ���ڰ�
    void  SetCurHPVar(float _f) { m_fHP += _f; }     // ���� ü�� += ���ڰ�

    // ��Ÿ �ʿ��� Getter/Setter �Լ� �߰�

};

