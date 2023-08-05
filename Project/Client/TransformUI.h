#pragma once
#include "ComponentUI.h"

class CGameObject;

class TransformUI :
    public ComponentUI
{
public:
    bool    b_IsWindowMode;
    float   m_fDragSpeed;

    //=====UI ��Ʈ ������ ���϶�� ���ϰ� Ʈ������UI�� ���� �밭 �۾����� �뵵=====
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

