#pragma once
#include <Engine\CScript.h>

class CBushScript :
    public CScript
{
private:
   

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider3D* _Other) override;
    virtual void OnOverlap(CCollider3D* _Other) override;
    virtual void EndOverlap(CCollider3D* _Other) override;
public:

    CLONE(CBushScript);
public:
    CBushScript();
    ~CBushScript();
};
