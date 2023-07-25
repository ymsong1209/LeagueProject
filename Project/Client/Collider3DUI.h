#pragma once
#include "ComponentUI.h"
class Collider3DUI :
    public ComponentUI
{
public:
    virtual int render_update() override;

public:
    void SetWall();
    void ChooseColliderType();
public:
    Collider3DUI();
    ~Collider3DUI();
};