#pragma once
#include <Engine\CScript.h>

class CMinionHPRatioScript :
    public CScript
{
private:
    CGameObject* BarPanel;
    CGameObject* HPPanel;

public:
    virtual void begin() override;
    virtual void tick() override;
    virtual void BeginOverlap(CCollider2D* _Other) override;


    //void UISpawn(CGameObject* _PlayerObj, CGameObject* _WorldBarObj);
public:
    CLONE(CMinionHPRatioScript);

public:
    CMinionHPRatioScript();
    ~CMinionHPRatioScript();
};

