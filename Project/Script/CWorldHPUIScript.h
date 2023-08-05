#pragma once
#include <Engine\CScript.h>
#include <Engine\struct.h>

class CWorldHPUIScript :
    public CScript
{
private:
    float CurrentHP;
    float CurrentMP;

    float TotalHP; //�� hp�� mp 
    float TotalMP; //�� hp�� mp 

    float CurrentHPRatio; //���� hp��mp ���� -> ���̴��� �����ؼ� ������ ���� ui���
    float CurrentMPRatio; //���� hp��mp ���� -> ���̴��� �����ؼ� ������ ���� ui���

public:
    virtual void begin() override;
    virtual void tick() override;
    virtual void BeginOverlap(CCollider2D* _Other) override;

    void SetCurrentHP(float _HP) { CurrentHP = _HP; }
    void SetCurrentMP(float _MP) { CurrentMP = _MP; }

    void SetTotalHP(float _HP) { TotalHP = _HP; }
    void SetTotalMP(float _MP) { TotalMP = _MP; }

    void SetBarInfo(float _TotalHP, float _TotalMP, float _CurrentHP, float _CurrentMP)
    {
        TotalHP = _TotalHP;
        TotalMP = _TotalMP;
        CurrentHP = _CurrentHP;
        CurrentMP = _CurrentMP;
    }

    CLONE(CWorldHPUIScript);

public:
    CWorldHPUIScript();
    ~CWorldHPUIScript();
};

