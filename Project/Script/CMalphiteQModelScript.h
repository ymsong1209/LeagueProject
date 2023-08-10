#pragma once
#include <Engine/CScript.h>
class CMalphiteQModelScript :
    public CScript
{
private:
    
    Vec3        m_vSpawnPos;
    Vec3        m_vTargetPos;
public:
    virtual void begin() override;
    virtual void tick() override;

    CLONE(CMalphiteQModelScript)

public:
    void SetSpawnPos(Vec3 _pos) { m_vSpawnPos = _pos; }
    void SetTargetPos(Vec3 _pos) { m_vTargetPos = _pos; }
    void SetPos(Vec3 _pos) { GetOwner()->Transform()->SetRelativePos(_pos); }
public:
    CMalphiteQModelScript();
    ~CMalphiteQModelScript();
};

