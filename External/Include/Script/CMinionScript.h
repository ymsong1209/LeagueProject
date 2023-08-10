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
    vector<Vec3>    m_vecWayPoint;      // 라인 별 웨이포인트
    int             m_iWayPointIdx;     // 몇 번째 웨이포인트로 향해야하는지

    CGameObject*    m_pTarget;          // 공격타겟

    float           m_fCurAttackCoolTime;
    float           m_fAggroTime;


public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void OnOverlap(CCollider2D* _collider) override;
public:
    int         GetWayPointIdx() { return m_iWayPointIdx; }
    Vec3        GetWayPoint() { if (m_vecWayPoint.size() == 0) return Vec3(NaN, NaN, NaN);  return m_vecWayPoint[m_iWayPointIdx]; }

    float       GetAggroTime() { return m_fAggroTime; }
    CGameObject* GetTarget() { return m_pTarget; }

    bool        CanAttack() { return m_fCurAttackCoolTime >= m_fAttackSpeed; }


public:
    void        Move();

    void        FindTarget();
    void        AttackCoolDown();
    bool        IsTargetValid(CGameObject* _Obj);
    bool        IsTargetInRange(CGameObject* _Obj);
    bool        IsAtWayPoint();
    void        ResetAttackCoolTime() { m_fCurAttackCoolTime = 0; }
    void        SetTargetNull() { m_pTarget = nullptr; }
};

