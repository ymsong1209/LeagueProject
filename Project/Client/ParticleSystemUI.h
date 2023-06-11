#pragma once
#include "ComponentUI.h"
#include <Engine/ptr.h>
#include <Engine/CTexture.h>

class TreeUI;

class ParticleSystemUI :
    public ComponentUI
{

private:

    tParticleModule       m_ModuleData;
    bool		          m_bModuleCheck[(UINT)PARTICLE_MODULE::END];
    bool                  m_bModuleShow[(UINT)PARTICLE_MODULE::END];
    bool                  renderbool[5];
    Ptr<CTexture>         ParticleTex;

private:
    void ParticleImageLoad();
    void UpdateParticleModule();


public:
    virtual int render_update() override;

public:
    ParticleSystemUI();
    ~ParticleSystemUI();
};

