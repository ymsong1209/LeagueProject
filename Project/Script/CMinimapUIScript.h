#pragma once
#include <Engine\CScript.h>

class CMinimapUIScript :
    public CScript
{
private:
    CGameObject* Minimap;
    CGameObject* Player1;
    CGameObject* Player2;
    CGameObject* Player3;
    CGameObject* Player4;

public:
    virtual void begin() override;
    virtual void tick() override;
    virtual void BeginOverlap(CCollider2D* _Other) override;


    CLONE(CMinimapUIScript);
public:
    CMinimapUIScript();
    ~CMinimapUIScript();
};

