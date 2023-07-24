#pragma once
#include "CEntity.h"

class CFsm;
class CGameObject;

class CState :
    public CEntity
{
public:
    CState();
    virtual ~CState();

    friend class CFsm;

public:
    CLONE(CState);

private:
    CFsm*        m_pOwnerFSM;   // ������ ������ FSM ������Ʈ
    bool         m_bActive;     // Ȱ��ȭ ����

public:
    //State�� ���� ���·� ��ȯ���� �� �� ��(tick), ���԰� ���� �Լ��� �ʼ��� �������Ѵ�
    virtual void tick() {};
    virtual void Enter();
    virtual void Exit();

public:
    CFsm* GetOwnerFSM() { return m_pOwnerFSM; }
    CGameObject* GetOwner();
    bool    IsActive() { return m_bActive; }
};

