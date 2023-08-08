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
    MinionType      m_eMinionType;      // �̴Ͼ� Ÿ��
    Lane            m_eLane;            // �̴Ͼ� ����(ž/�̵�/��)

    vector<Vec3>    m_vecWayPoint;      // ���� �� ��������Ʈ
    int             m_iWayPointIdx;     // �� ��° ��������Ʈ�� ���ؾ��ϴ���

    bool            m_bChasing;         // �߰� ���ΰ�
    CGameObject* m_pTarget;          // ����Ÿ��

    float           m_fCurAttackCoolTime;
    float           m_fAggroTime;


public:
    virtual void begin() override;
    virtual void tick() override;

public:
    Lane        GetLane() { return m_eLane; }
    MinionType  GetMinionType() { return m_eMinionType; }
    int         GetWayPointIdx() { return m_iWayPointIdx; }
    Vec3        GetWayPoint() { return m_vecWayPoint[m_iWayPointIdx]; }
    float       GetAggroTime() { return m_fAggroTime; }

    CGameObject* GetTarget() { return m_pTarget; }

    bool        IsChasing() { return m_bChasing; }
    bool        CanAttack() { return m_fCurAttackCoolTime >= m_fAttackSpeed; }
    void        SetLane(Lane _lane) { m_eLane = (Lane)_lane; }
    void        SetMinionType(MinionType _type) { m_eMinionType = (MinionType)_type; }

public:
    void        Move();

    void        FindTarget();
    void        AttackCoolDown();
    bool        IsTargetValid(CGameObject* _Obj);
    bool        IsTargetInRange(CGameObject* _Obj);
    bool        IsAtWayPoint();
    void        ResetAttackCoolTime() { m_fCurAttackCoolTime = 0; }
};

