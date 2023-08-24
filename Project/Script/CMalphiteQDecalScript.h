#pragma once
#include <Engine/CScript.h>
class CMalphiteQDecalScript :
    public CScript
{
private:
    float       m_fTime;
    float       m_bDeathActivated;

    //¸»ÆÄ º»Ã¼ position
    Vec3        m_vSpawnPos;

    //¸»ÆÄ ±¼··¼è position
    Vec3        m_vTargetPos;
public:
    virtual void begin() override;
    virtual void tick() override;

    CLONE(CMalphiteQDecalScript)

public:
    void DeleteDecal();
    void SetSpawnPos(Vec3 _pos) { m_vSpawnPos = _pos; }
    void SetTargetPos(Vec3 _pos) { m_vTargetPos = _pos; }
public:
    CMalphiteQDecalScript();
    ~CMalphiteQDecalScript();
};

