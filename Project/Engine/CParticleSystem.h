#pragma once
#include "CRenderComponent.h"

#include "ptr.h"
#include "CParticleUpdateShader.h"

class CStructuredBuffer;

class CParticleSystem :
    public CRenderComponent
{
private:
    CStructuredBuffer*          m_ParticleBuffer;
    CStructuredBuffer*          m_RWBuffer;
    CStructuredBuffer*          m_ModuleDataBuffer;
    
    tParticleModule             m_ModuleData;
    Ptr<CParticleUpdateShader>  m_UpdateCS;
    Ptr<CTexture>               pParticleTex;

    float                       m_AccTime;

public:
    tParticleModule& GetModuleData() { return m_ModuleData; }
    void             SetModuleData(tParticleModule _data) { m_ModuleData = _data; }
    Ptr<CTexture>    GetParticleTexture() { return pParticleTex; }
    void             SetParticleTexture(Ptr<CTexture> _tex) { pParticleTex = _tex; }
    void             SetParticleBufferSize();

public:
    virtual void finaltick() override;
    virtual void render() override;

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CParticleSystem);
public:
    CParticleSystem();
    CParticleSystem(const CParticleSystem& _other);
    ~CParticleSystem();
};

