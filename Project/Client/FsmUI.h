#pragma once
#include "ComponentUI.h"
class FsmUI :
    public ComponentUI
{
public:
    FsmUI();
    ~FsmUI();

private:

public:
    virtual void init() override;
    virtual int render_update() override;
};

