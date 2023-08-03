#pragma once
#include "CStructureScript.h"
class CTurretScript :
    public CStructureScript
{
public:
    CTurretScript();
    ~CTurretScript();

    CLONE(CTurretScript);

private:
    UINT        m_iAttackTargetID;


    // ���� �ܰ� Ÿ������ ���� Ÿ���� �ν��� ���� Ÿ���� �μ� �� �ֱ� ������ Ÿ�� �ѹ����� �ʿ�
    int         m_iTurretNumber;    // 1 ~ 4     
    bool        m_bIsAttacking;

    bool        m_bShowAttackRange;     // ��ž ��Ÿ� ������ ����

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    void CheckStatus();
    void ChangeAnim();
    bool IsEnemyInRange(UINT _unitID);



};

