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
    tWorldScoreInfo WorldInfo; //�ʿ��� ������ �ִٰ� ����.
    tMyPlayerInfo MyInfo; //�ʿ��� ������ �ִٰ� ����.

    float PlayTime; //�׽�Ʈ��
    CLONE(CScorePanelScript);
public:
    CScorePanelScript();
    ~CScorePanelScript();
};

