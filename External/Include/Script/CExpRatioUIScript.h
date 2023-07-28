#pragma once
#include <Engine\CScript.h>

class CExpRatioUIScript :
    public CScript
{
private:
    float LevelTotalExp; //�ش緹���� �� exp (ä���� �ϴ� exp ��)
    float CurrentExp; //���� ������ �ִ�exp : ���Ŀ� è�Ǿ� ��ũ��Ʈ���� �޾ƿ� ����

    float CurrentExpRatio; //���� ä��exp ���� -> ���̴��� �����ؼ� ������ ���� ui���

public:
    virtual void begin() override;
    virtual void tick() override;
    virtual void BeginOverlap(CCollider2D* _Other) override;

    float GetExpRatio() { return CurrentExpRatio; }
    void SetExpRatio(float _Ratio) { CurrentExpRatio = _Ratio; }

    CLONE(CExpRatioUIScript);
public:
    CExpRatioUIScript();
    ~CExpRatioUIScript();
};

