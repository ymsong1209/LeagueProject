#pragma once
#include <Engine\CScript.h>

class CMouseCursorUIScript :
    public CScript
{
private:
    CGameObject* Mouse;
    bool    m_bOnlyMouseCursorOn;
    CGameObject* MoveArrow;

public:
    virtual void begin() override;
    virtual void tick() override;
    virtual void BeginOverlap(CCollider2D* _Other) override;


    CLONE(CMouseCursorUIScript);
public:
    CMouseCursorUIScript();
    ~CMouseCursorUIScript();
};

