#pragma once
#include <Engine\CScript.h>

class CTurretHPUIScript :
    public CScript
{
private:
    float m_fCurHP; //총 hp나 mp (채워야 하는 exp 량)
    float m_fTotalHP; //현재 가지고 있는hp나 mp : 추후에 챔피언 스크립트에서 받아올 예정
    float m_fRatio; //현재 hp나mp 비율 -> 셰이더에 전달해서 비율에 따라 ui출력

    Vec3 m_vOffsetBarPos;
    Vec2 m_vOffsetFontPos;
public:
    virtual void begin() override;
    virtual void tick() override;
    virtual void BeginOverlap(CCollider2D* _Other) override;

    CLONE(CTurretHPUIScript);

public:
    CTurretHPUIScript();
    ~CTurretHPUIScript();
};

