#pragma once
#include <Engine\CScript.h>

class CExpRatioUIScript :
    public CScript
{
private:
    float m_fLevelTotalExp; //�ش緹���� �� exp (ä���� �ϴ� exp ��)
    float m_fCurrentExp; //���� ������ �ִ�exp : ���Ŀ� è�Ǿ� ��ũ��Ʈ���� �޾ƿ� ����

    float m_fCurrentExpRatio; //���� ä��exp ���� -> ���̴��� �����ؼ� ������ ���� ui���

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

