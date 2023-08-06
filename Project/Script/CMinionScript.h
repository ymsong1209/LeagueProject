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
    CGameObject*    m_pTarget;          // ����Ÿ��

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

