#pragma once
#include "UI.h"

class CLevel;

class LevelUI :
    public UI
{
private:
    vector<wstring>  m_vecLevelName;
    UINT             m_LeftLayer;
    UINT             m_RightLayer;

public:

    virtual int render_update() override;
    void SelectLayer();

public:
    LevelUI();
    ~LevelUI();

    friend class ContentUI;
};

