#pragma once
#include "ComponentUI.h"

class CGameObject;

class TransformUI :
    public ComponentUI
{
public:
    bool    b_IsWindowMode;

    virtual int render_update() override;
    void EditTransform(float* cameraView, float* cameraProjection, float* matrix, bool editTransformDecomposition);

    void RenderGizmo();

public:
    TransformUI();
    ~TransformUI();
};

