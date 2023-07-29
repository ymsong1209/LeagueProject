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
    virtual void render(UINT _iSubset) override;

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    virtual void SaveToLevelJsonFile(Value& _objValue, Document::AllocatorType& allocator)override;
    virtual void LoadFromLevelJsonFile(const Value& _componentValue)override;

    CLONE(CSkyBox);
public:
    CSkyBox();
    CSkyBox(const CSkyBox& _other);
    ~CSkyBox();
};

