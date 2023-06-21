#pragma once
#include "CRenderComponent.h"

enum class eTexMovingStyle
{
    NONE,
    HORIZONTAL,
    VERTICAL,
    LINEAR,
    PARABOLA,
    SIN,
    COS,
    END,
};

enum class eTargetTexture
{
    OUTPUT,
    PUNCTURE,
    END,
};

struct MovingStruct
{
    eTargetTexture   TargetTex;
    eTexMovingStyle  MovingStyle;
    Vec4            FuncValue;
    Vec2            PreviousPos;
};


class CMeshRender :
    public CRenderComponent
{
public:
    vector<MovingStruct>    MovingVec;

public:
    virtual void finaltick() override;
    virtual void render() override;

    CLONE(CMeshRender);
public:
    CMeshRender();
    ~CMeshRender();
};

