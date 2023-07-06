#pragma once
#include "ComponentUI.h"

#include <Engine\CScript.h>

class ScriptUI :
    public ComponentUI
{
private:
    CScript*    m_pTargetScript;
    string      m_strScriptName;
    string      m_strTexDesc;
public:
    virtual int render_update() override;

public:
    void SetScript(CScript* _Script);
    void SelectTexture(DWORD_PTR _Key);
public:
    ScriptUI();
    ~ScriptUI();
};

