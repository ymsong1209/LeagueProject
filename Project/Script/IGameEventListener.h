#pragma once

class CGameEvent;

class IGameEventListener
{
public:
    virtual ~IGameEventListener() = default;

    virtual void onEvent(CGameEvent& event) = 0;
};

