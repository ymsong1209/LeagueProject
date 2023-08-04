#pragma once
#include <Engine/CState.h>

#include "IGameEventListener.h"
#include "CGameEvent.h"
#include "CGameEventMgr.h"
#include <Engine/components.h>

class CUnitState :
    public CState
{
public:
    CUnitState();
    CUnitState(const CUnitState& other);
    virtual ~CUnitState();

    CLONE(CUnitState);

    class CUnitScriptEventListener
        : public IGameEventListener
    {
        friend class CUnitState;

    private:
        CUnitState* m_state;

    public:
        CUnitScriptEventListener(CUnitState* _state)
            : m_state(_state)
        {
        }

        CUnitScriptEventListener(const CUnitScriptEventListener& other)
            : IGameEventListener(other)
            , m_state(other.m_state)
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

};

