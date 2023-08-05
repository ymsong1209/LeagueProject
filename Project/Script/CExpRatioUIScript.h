#pragma once
#include <Engine\CScript.h>

class CExpRatioUIScript :
    public CScript
{
private:
    float m_fLevelTotalExp; //해당레벨의 총 exp (채워야 하는 exp 량)
    float m_fCurrentExp; //현재 가지고 있는exp : 추후에 챔피언 스크립트에서 받아올 예정

    float m_fCurrentExpRatio; //현재 채운exp 비율 -> 셰이더에 전달해서 비율에 따라 ui출력

public:
    virtual void begin() override;
    virtual void tick() override;
    virtual void BeginOverlap(CCollider2D* _Other) override;

    float GetExpRatio() { return m_fCurrentExpRatio; }
    void SetExpRatio(float _Ratio) { m_fCurrentExpRatio = _Ratio; }

    CLONE(CExpRatioUIScript);
public:
    CExpRatioUIScript();
    ~CExpRatioUIScript();
};

