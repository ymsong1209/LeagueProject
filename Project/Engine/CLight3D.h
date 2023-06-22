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

    CGameObject*    m_pLightCam;    // 광원 시점용 카메라

public:
    void SetLightType(LIGHT_TYPE _Type);

    void SetLightDiffuse(Vec3 _vDiffuse) { m_LightInfo.Color.vDiffuse = _vDiffuse; }
    void SetLightSpecular(Vec3 _vSpec) { m_LightInfo.Color.vSpecular = _vSpec; }
    void SetLightAmbient(Vec3 _vAmbient) { m_LightInfo.Color.vAmbient = _vAmbient; }
    void SetRadius(float _Radius) { m_LightInfo.Radius = _Radius; }
    void SetAngle(float _Angle) { m_LightInfo.Angle = _Angle; }
    void SetLightDirection(Vec3 _vDir);


    const tLightInfo& GetLightInfo() { return m_LightInfo; }
    Vec3 GetLightColor() { return m_LightInfo.Color.vDiffuse; }
    LIGHT_TYPE GetLightType() { return (LIGHT_TYPE)m_LightInfo.LightType; }
    float GetRadius(float _fRadius) { return m_LightInfo.Radius; }
    float GetAngle(float _fAngle) { return m_LightInfo.Angle; }
    Vec3 GetLightDirection() { return m_LightInfo.vWorldDir; }

public:
    virtual void finaltick() override;
    void render();
    void render_depthmap();

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CLight3D);
public:
    CLight3D();
    CLight3D(const CLight3D& _other);
    ~CLight3D();
};

