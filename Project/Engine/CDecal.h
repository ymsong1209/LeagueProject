#pragma once
#include "CRenderComponent.h"

class CDecal :
    public CRenderComponent
{
private:
    Ptr<CTexture>   m_DecalTex;
    int             m_Light;
    Vec4            m_vLightDiffuse;
    bool            m_bShowDebug;   //Imgui���� DebugShape�� �����ְ� ���� check
    int             m_iLayerCheck;  //��� layer�� ĥ�� ������ ��Ʈ�����ڷ� ����, shader���� ��Ʈ�����ڸ� �и��� ��ĥ�� �Ѵ�.
    BS_TYPE         m_BSType;      


public:
    void SetAsLight(bool _bLight) { m_Light = _bLight; }
    void SetOutputTexture(Ptr<CTexture> _Tex) { m_DecalTex = _Tex; }
    void SetShowDebug(bool _Debugbool) { m_bShowDebug = _Debugbool; }
    void SetLightDiffuse(Vec4& _diffuse) { m_vLightDiffuse = _diffuse; }
    void SetDecalTex(Ptr<CTexture> _tex) { m_DecalTex = _tex; }
    void SetLayerCheck(int _ilayer) { m_iLayerCheck = _ilayer; };
    void SetBSType(BS_TYPE _bs) { m_BSType = _bs; }

    const int& GetLayerCheck() { return m_iLayerCheck; }
    const bool& GetShowAsLight() { return m_Light; }
    const bool& GetShowDebug() { return m_bShowDebug; }
    const Vec4& GetLightDiffuse() { return m_vLightDiffuse; }
    Ptr<CTexture> GetDecalTex() { return m_DecalTex; }
    const BS_TYPE& GetBSType() { return m_BSType; }

    virtual void finaltick() override;
    virtual void render() override;
    virtual void render(UINT _iSubset) override;

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;
   
    virtual void SaveToLevelJsonFile(Value& _objValue, Document::AllocatorType& allocator)override;
    virtual void LoadFromLevelJsonFile(const Value& _componentValue)override;

    CLONE(CDecal);

public:
    CDecal();
    CDecal(const CDecal& _other);
    ~CDecal();
};