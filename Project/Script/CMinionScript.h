#pragma once
#include "CMobScript.h"

class CMinionScript :
    public CMobScript
{
public:
    CMinionScript();
    ~CMinionScript();

    CLONE(CMinionScript);

protected:
    MinionType      m_eMinionType;      // 미니언 타입
    Lane            m_eLane;            // 미니언 라인(탑/미드/봇)
    vector<Vec3>    m_vecWayPoint;      // 라인 별 웨이포인트
    int             m_iWayPointIdx;     // 몇 번째 웨이포인트로 향해야하는지

    bool            m_bChasing;         // 추격 중인가
    CGameObject*    m_pTarget;          // 공격타겟

    float           m_curAttackCoolTime;


public:
    virtual void begin() override;
    virtual void tick() override;

public:
    Lane        GetLane() { return m_eLane; }
    MinionType  GetMinionType() { return m_eMinionType; }
    int         GetWayPointIdx() { return m_iWayPointIdx; }
    Vec3        GetWayPoint() { return m_vecWayPoint[m_iWayPointIdx]; }

    CGameObject* GetTarget() { return m_pTarget; }

    bool        IsChasing() { return m_bChasing; }
    bool        CanAttack() { return m_curAttackCoolTime >= m_fAttackSpeed; }

public:
    void        Move();

    void        SelectTarget();
    void        AttackCoolDown();
    bool        IsTargetValid(CGameObject* _Obj);
    bool        IsTargetInRange(CGameObject* _Obj);
    bool        IsAtWayPoint();
};

