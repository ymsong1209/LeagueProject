#pragma once
#include <Engine\CScript.h>

class CScorePanelScript :
    public CScript
{
private:

public:
    virtual void begin() override;
    virtual void tick() override;
    virtual void BeginOverlap(CCollider2D* _Other) override;
    tWorldScoreInfo WorldInfo; //필요한 정보가 있다고 가정.
    tMyPlayerInfo MyInfo; //필요한 정보가 있다고 가정.

    float PlayTime; //테스트용
    CLONE(CScorePanelScript);
public:
    CScorePanelScript();
    ~CScorePanelScript();
};

