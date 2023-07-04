#pragma once
#include "ComponentUI.h"

class Animator3DUI :
    public ComponentUI
{
public:
    virtual int render_update() override;
    void wstringTostring(wstring _wstring, char* _Buff, size_t _BufferSize);

public:
    Animator3DUI();
    ~Animator3DUI();
};

