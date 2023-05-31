#pragma once
#include "CScript.h"

class CPlayerScript :
    public CScript
{
private:
    float       m_fSpeed;

public:
    virtual void begin() override;
    virtual void tick() override;


    virtual void BeginOverlap(CCollider2D* _Other) override;


private:
    void Shoot();

    CLONE(CPlayerScript);
public:
    CPlayerScript();
    ~CPlayerScript();
};

