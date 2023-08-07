#pragma once
#include <Engine\CScript.h>

class CMinionHPBarPosScript :
    public CScript
{
private:

public:
    virtual void begin() override;
    virtual void tick() override;
    virtual void BeginOverlap(CCollider2D* _Other) override;


public:
    CLONE(CMinionHPBarPosScript);

public:
    CMinionHPBarPosScript();
    ~CMinionHPBarPosScript();
};

