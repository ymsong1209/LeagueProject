#pragma once
#include "CStructureScript.h"
#include "CAttackRangeScript.h"

class CTurretScript :
    public CStructureScript
{
public:
    CTurretScript();
    virtual ~CTurretScript();

    CLONE(CTurretScript);

private:
    UINT        m_iAttackTargetID;


    // ���� �ܰ� Ÿ������ ���� Ÿ���� �ν��� ���� Ÿ���� �μ� �� �ֱ� ������ Ÿ�� �ѹ����� �ʿ�
    int         m_iTurretNumber;    // 1 ~ 4     
    bool        m_bIsAttacking;

    bool        m_bShowAttackRange;     // ��ž ��Ÿ� ������ ����
    float       m_fAttackCoolTime;

    CAttackRangeScript*     m_AttackRangeScript;
    CGameObject*            m_pAttackTarget;

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _Other) override;
    virtual void OnOverlap(CCollider2D* _Other) override;
    virtual void EndOverlap(CCollider2D* _Other) override;


private:
    void CheckStatus();
    void ChangeAnim();
    void Attack();

    void SelectTarget();
    bool IsValidTarget(CGameObject* _obj);



};

