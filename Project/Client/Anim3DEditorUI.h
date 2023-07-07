#pragma once
#include "UI.h"

class CGameObject;
class ComponentUI;
class CTexture;
class CAnim3D;

struct Point;

class Anim3DEditorUI :
    public UI
{
private:
    int                 m_iEditMode;
    CGameObject* m_pTestObject;
    bool                m_bRepeat;
    float               m_fBlendTime;

    int                 m_iStartFrm;
    int                 m_iEndFrm;

    CAnim3D* m_pBaseAnim;
    CAnim3D* m_pBlendAnim;

public:
    virtual void tick() override;
    virtual int render_update() override;

public:

    bool FindTestObject();
    void SpawnTestObject();
    void LoadFBX();
    void LoadMeshdata();
    void LoadAnim3D();
    wstring BrowseFolder();
    void LoadAnimFromFolder();
    void SelectEditMode();
    void SelectPlayMode();
    void SetRepeat();
    void SetRepeatBlendTime();
    void SetBlendTime();
    void SelectMainAnimation();
    void DeleteCurrentAnimation();
    void DeleteEveryAnim();
    void SaveEveryAnim();
    void MainAnimationInfo();
    void SetCustomAnim();
    void SaveCustomAnim();

    void SelectBlendTestPlayMode();
    void SelectBaseAnimation();
    void SelectBlendAnimation();
    void BlendAnim();
public:
    Anim3DEditorUI();
    ~Anim3DEditorUI();
};

