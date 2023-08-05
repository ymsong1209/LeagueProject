#pragma once
#include "CStructureScript.h"
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

    vector<CGameObject*>    m_vecCollidingMinion;
    vector<CGameObject*>    m_vecCollidingChampion;
    CGameObject*            m_pTarget;

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _Other) override;
    virtual void OnOverlap(CCollider2D* _Other) override;
    virtual void EndOverlap(CCollider2D* _Other) override;


public:
    void CheckStatus();
    void ChangeAnim();
    bool IsEnemyInRange(UINT _unitID);



};

