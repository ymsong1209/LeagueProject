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
    //Level을 json으로 저장
    void SaveLevel();
    //levelui로 인해 사용 안하는 코드
    void LoadLevel();
    void CreateEmptyObject();
    void AddComponent(COMPONENT_TYPE _type);
    void AddScript(const wstring& _strScriptName);

    void CreateMaterial();

public:
    MenuUI();
    ~MenuUI();
};

