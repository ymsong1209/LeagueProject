#pragma once
#include "CComponent.h"

class CLight2D :
    public CComponent
{
private:
    tLightInfo      m_LightInfo;

public:
    void SetLightType(LIGHT_TYPE _Type) { m_LightInfo.LightType = (UINT)_Type; }
    void SetLightDiffuse(Vec3 _vDiffuse) {m_LightInfo.Color.vDiffuse = _vDiffuse; }
    void SetLightAmbient(Vec3 _vAmbient) {m_LightInfo.Color.vAmbient = _vAmbient; }
    void SetRadius(float _Radius) { m_LightInfo.Radius = _Radius; }

    void SetAngle(float _Angle) { m_LightInfo.Angle = _Angle; }
    const float GetAngle() { return m_LightInfo.Angle * 180.f / XM_PI; }

    const tLightInfo& GetLightInfo() { return m_LightInfo; }

public:
    virtual void finaltick() override;

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;
    
    virtual void SaveToLevelJsonFile(Value& _objValue, Document::AllocatorType& allocator)override;
    virtual void LoadFromLevelJsonFile(const Value& _componentValue)override;

    CLONE(CLight2D);
public:
    CLight2D();
    CLight2D(const CLight2D& _other);
    ~CLight2D();
};

