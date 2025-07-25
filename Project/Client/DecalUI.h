#pragma once
#include "ComponentUI.h"
class DecalUI :
    public ComponentUI
{
public:
    virtual int render_update() override;

public:

    void ShowDebug();
    void ShowAsLight();
    int ChooseTexture();
    void SetLayerCheck();
    void SetBSType();
    void SelectAlpha();

    void SelectTextureBtn(DWORD_PTR _Key);
public:
    DecalUI();
    ~DecalUI();
};
