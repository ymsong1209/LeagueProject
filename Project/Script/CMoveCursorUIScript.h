#pragma once
#include <Engine\CScript.h>

class CMoveCursorUIScript :
    public CScript
{
private:
    float Time;

public:
    virtual void begin() override;
    virtual void tick() override;
    virtual void BeginOverlap(CCollider2D* _Other) override;


    CLONE(CMoveCursorUIScript);
public:
    CMoveCursorUIScript();
    ~CMoveCursorUIScript();
};

