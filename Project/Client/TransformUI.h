#pragma once
#include "ComponentUI.h"

class CGameObject;

class TransformUI :
    public ComponentUI
{
public:
    bool    b_IsWindowMode;
    float   m_fDragSpeed;

    //=====UI 폰트 넣을때 편하라고 급하게 트랜스폼UI에 구현 대강 글씨띄우는 용도=====
    bool    m_bTestText;
    Vec3    m_TestTextPos;
    float   m_fFontSize;
    char    m_wTextInput[50];
    //===================


    virtual int render_update() override;
    void EditTransform(float* cameraView, float* cameraProjection, float* matrix, bool editTransformDecomposition);

    void RenderGizmo();

public:
    TransformUI();
    ~TransformUI();
};

