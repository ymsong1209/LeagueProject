#pragma once
#include "ComponentUI.h"
class Animator3DUI :
    public ComponentUI
{
public:
    virtual int render_update() override;

public:
    void LoadAnim3D();
    wstring BrowseFolder();
    void LoadAnimFromFolder();
    void SelectPlayMode();
    void SetRepeat();
    void SetBlendTime();
    void SetSpeed();
    void SelectAnimation();
    void DeleteCurrentAnimation();
    void DeleteEveryAnim();
    void MainAnimationInfo();


public:
    Animator3DUI();
    ~Animator3DUI();
};
