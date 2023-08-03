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


    // 가장 외곽 타워부터 이전 타워를 부숴야 내부 타워를 부술 수 있기 때문에 타워 넘버링이 필요
    int         m_iTurretNumber;    // 1 ~ 4     
    bool        m_bIsAttacking;

    bool        m_bShowAttackRange;     // 포탑 사거리 보일지 여부

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    void CheckStatus();
    void ChangeAnim();
    bool IsEnemyInRange(UINT _unitID);



};

