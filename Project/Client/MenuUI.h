#pragma once
#include "UI.h"


class MenuUI :
    public UI
{
private:


public:
    virtual void finaltick() override;
    virtual int render_update() override;


private:
    void SaveLevel();
    void LoadLevel();
    void CreateEmptyObject();
    void AddComponent(COMPONENT_TYPE _type);
    void AddScript(const wstring& _strScriptName);

    void CreateMaterial();

public:
    MenuUI();
    ~MenuUI();
};

