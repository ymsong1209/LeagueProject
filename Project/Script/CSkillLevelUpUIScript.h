#pragma once
#include <Engine\CScript.h>

class CSkillLevelUpUIScript :
    public CScript
{
private:
    int     m_iSkillPoint;
    CGameObject* m_UIBackPanel;

    CGameObject* Q_LvUpObj;
    CGameObject* W_LvUpObj;
    CGameObject* E_LvUpObj;
    CGameObject* R_LvUpObj;

    int m_iQLevel;
    int m_iWLevel;
    int m_iELevel;
    int m_iRLevel;

public:
    virtual void begin() override;
    virtual void tick() override;
    virtual void BeginOverlap(CCollider2D* _Other) override;

    void SetBackPanel(CGameObject* _Obj) { m_UIBackPanel = _Obj; }
    void Set_QWER_SkillLvUpObj(CGameObject* _Q, CGameObject* _W, CGameObject* _E, CGameObject* _R)
    {
        Q_LvUpObj = _Q; W_LvUpObj = _W; E_LvUpObj = _E; R_LvUpObj = _R;
    }

public:
    CLONE(CSkillLevelUpUIScript);

public:
    CSkillLevelUpUIScript();
    ~CSkillLevelUpUIScript();
};

