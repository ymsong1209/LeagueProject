#pragma once
#include "ComponentUI.h"

class LandScapeUI :
    public ComponentUI
{

public:
    virtual int render_update() override;


public:

    void SetFace();
    void HeightMapDropCheck();
    void BrushTexDropCheck();
    void SetBrushScale();
    void SetLandScapeMod();

    void SelectHeightMap(DWORD_PTR _Key);
    void SelectBrushTex(DWORD_PTR _Key);
    void SelectRSType();
public:
    LandScapeUI();
    ~LandScapeUI();
};