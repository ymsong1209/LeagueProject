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


public:
    void SetLightType(LIGHT_TYPE _Type);
    void SetLightDirection(Vec3 _vDir) { m_LightInfo.vWorldDir = _vDir; }
    void SetLightDiffuse(Vec3 _vDiffuse) { m_LightInfo.Color.vDiffuse = _vDiffuse; }
    void SetLightSpecular(Vec3 _vSpec) { m_LightInfo.Color.vSpecular = _vSpec; }
    void SetLightAmbient(Vec3 _vAmbient) { m_LightInfo.Color.vAmbient = _vAmbient; }
    void SetRadius(float _Radius) { m_LightInfo.Radius = _Radius; }
    void SetAngle(float _Angle) { m_LightInfo.Angle = _Angle; }

    const tLightInfo& GetLightInfo() { return m_LightInfo; }

public:
    virtual void finaltick() override;
    void render();

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CLight3D);
public:
    CLight3D();
    ~CLight3D();
};

