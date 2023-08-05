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


    // 가장 외곽 타워부터 이전 타워를 부숴야 내부 타워를 부술 수 있기 때문에 타워 넘버링이 필요
    int         m_iTurretNumber;    // 1 ~ 4     
    bool        m_bIsAttacking;

    bool        m_bShowAttackRange;     // 포탑 사거리 보일지 여부
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

