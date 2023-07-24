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
    CFsm*        m_pOwnerFSM;   // 본인을 소유한 FSM 컴포넌트
    bool         m_bActive;     // 활성화 여부

public:
    //State는 본인 상태로 전환됐을 때 할 일(tick), 진입과 퇴장 함수를 필수로 가져야한다
    virtual void tick() {};
    virtual void Enter();
    virtual void Exit();

public:
    CFsm* GetOwnerFSM() { return m_pOwnerFSM; }
    CGameObject* GetOwner();
    bool    IsActive() { return m_bActive; }
};

