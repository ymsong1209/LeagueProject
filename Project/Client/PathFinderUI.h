#pragma once
#include "ComponentUI.h"

class PathFinderUI :
    public ComponentUI
{
public:
    virtual int render_update() override;


public:
    PathFinderUI();
    ~PathFinderUI();
};




