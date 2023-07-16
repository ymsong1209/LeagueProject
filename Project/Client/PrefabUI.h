#pragma once
#include "ResUI.h"

class CGameObject;
class CCamera;

class PrefabUI :
    public ResUI
{
private:
    Vec4            m_vMousePos;
    Vec4            m_vConvertedMousePos;
    CCamera*        m_pEditorCam;
    int             m_iSpawnLayerIdx;

    bool            m_bClickToSpawn;
    bool            m_bIsTransformAbsolute;
    Vec3            m_vClickedPosition;
    CGameObject*    m_pClonedObject;

public:
    virtual void tick() override;
    virtual int render_update() override;

    void SelectLayer();
    void SpawnPrefab();

public:
    PrefabUI();
    ~PrefabUI();
};


