#pragma once
#include <Engine\CScript.h>
class CMonsterHPBarUIScript :
    public CScript
{
private:

public:
    virtual void begin() override;
    virtual void tick() override;
    virtual void BeginOverlap(CCollider2D* _Other) override;

public:
    CLONE(CMonsterHPBarUIScript);

public:
    CMonsterHPBarUIScript();
    ~CMonsterHPBarUIScript();
};

