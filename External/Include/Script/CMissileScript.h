#pragma once
#include <Engine\CScript.h>

class CMissileScript :
    public CScript
{
private:
    Vec3        m_vDir;     // �̵� ����
    float       m_fSpeed;   // �̵� �ӷ�

public:
    void SetSpeed(float _Speed) { m_fSpeed = _Speed; }

public:
    virtual void tick() override;

    CLONE(CMissileScript);
public:
    CMissileScript();
    ~CMissileScript();
};

