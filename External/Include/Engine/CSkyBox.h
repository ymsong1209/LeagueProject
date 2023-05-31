#pragma once
#include "CRenderComponent.h"

enum class SKYBOX_TYPE
{
    SPHERE,
    CUBE,
};


class CSkyBox :
    public CRenderComponent
{
private:
    SKYBOX_TYPE     m_Type;
    Ptr<CTexture>   m_SkyTex;


public:
    void SetType(SKYBOX_TYPE _Type);
    void SetSkyTexture(Ptr<CTexture> _SkyTex) { m_SkyTex = _SkyTex;}


public:
    virtual void finaltick() override;
    virtual void render() override;

    CLONE(CSkyBox);
public:
    CSkyBox();
    ~CSkyBox();
};

