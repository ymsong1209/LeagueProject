#pragma once
#include "CComponent.h"

#include "CMesh.h"
#include "CMaterial.h"
#include "ptr.h"

class CLight3D :
    public CComponent
{
private:
    tLightInfo      m_LightInfo;

    Ptr<CMesh>      m_VolumeMesh;
    Ptr<CMaterial>  m_LightMtrl;

    UINT            m_iLightIdx;

    CGameObject*    m_pLightCam;    // 광원 시점용 카메라, Component안에 GameObject가 있으므로 finaltick_module을 사용

    bool            m_bShowDebug;

public:
    void SetLightType(LIGHT_TYPE _Type);

    void SetLightDiffuse(Vec3 _vDiffuse) { m_LightInfo.Color.vDiffuse = _vDiffuse; }
    void SetLightSpecular(Vec3 _vSpec) { m_LightInfo.Color.vSpecular = _vSpec; }
    void SetLightAmbient(Vec3 _vAmbient) { m_LightInfo.Color.vAmbient = _vAmbient; }
    void SetRadius(float _Radius) { m_LightInfo.Radius = _Radius; }
    void SetAngle(float _Angle) { m_LightInfo.Angle = _Angle * XM_PI / 180.f; }
    void SetInnerAngle(float _Angle) { m_LightInfo.InnerAngle = _Angle * XM_PI / 180.f; }
    void SetShowDebug(bool _Debugbool) { m_bShowDebug = _Debugbool; }
    void SetLightDirection(Vec3 _vDir);


    const tLightInfo& GetLightInfo() { return m_LightInfo; }
    const float GetAngle() { return m_LightInfo.Angle * 180.f / XM_PI; }
    const float GetInnerAngle() { return m_LightInfo.InnerAngle * 180.f / XM_PI; }
    const bool  GetShowDebug() { return m_bShowDebug; }
    Vec3 GetLightDirection() { return m_LightInfo.vWorldDir; }

public:
    virtual void finaltick() override;
    void render();
    void render_depthmap();

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    virtual void SaveToLevelJsonFile(Value& _objValue, Document::AllocatorType& allocator)override;
    virtual void LoadFromLevelJsonFile(const Value& _componentValue)override;

    CLONE(CLight3D);
public:
    CLight3D();
    CLight3D(const CLight3D& _other);
    ~CLight3D();
};

