#pragma once
#include "ComponentUI.h"

class MeshRenderUI :
    public ComponentUI
{

public:
    virtual int render_update() override;


public:
    void SelectMesh(DWORD_PTR _Key);
    void SelectMaterial(DWORD_PTR _Key);

public:
    void SetTexMovingTypeAndValue(int& _Target, Vec4& _FuncValue,  int _RandNum);


public:
    MeshRenderUI();
    ~MeshRenderUI();
};

