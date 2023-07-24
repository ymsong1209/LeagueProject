#pragma once
#include "UI.h"
#include <Engine\CMaterial.h>
#include <Engine\CShader.h>

class CreateMaterialUI :
    public UI
{
private:
    bool        SetWindowSize;
    Ptr<CMaterial>  CulMaterial;
    Ptr<CGraphicsShader>    CulShader;
    TEX_PARAM       m_eSelected;

public:
    virtual void finaltick() override;
    virtual int render_update() override;

    char MtrlName[50];
    int    TexNum;

public:
    CreateMaterialUI();
    ~CreateMaterialUI();

    void SelectShader(DWORD_PTR _Key);
    void SelectTexture(DWORD_PTR _Key);

};

