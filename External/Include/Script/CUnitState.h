#pragma once
#include <Engine/CState.h>

#include "IGameEventListener.h"
#include "CGameEvent.h"
#include "CGameEventMgr.h"

class CUnitState :
    public CState
{
public:
    CUnitState();
    virtual ~CUnitState();

    class CUnitScriptEventListener
        : public IGameEventListener
    {
    private:
        CUnitState* m_state;

    public:
        CUnitScriptEventListener(CUnitState* _state)
            : m_state(_state)
        {
        }

        virtual void onEvent(CGameEvent& event) override { m_state->HandleEvent(event); }
    };

private:
    CUnitScriptEventListener* m_Listener;

public:
    CUnitScriptEventListener* GetListener() { return m_Listener; }
    virtual void HandleEvent(CGameEvent& event) {};

    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    void SendAnimPacket(wstring _animName, bool _repeat, bool _blend, float _blentTime);
};

