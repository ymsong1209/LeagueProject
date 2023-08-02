#pragma once
#include <Engine\CScript.h>

class CHpMpRatioUIScript :
    public CScript
{
private:
    float m_fTotal; //�� hp�� mp (ä���� �ϴ� exp ��)
    float m_fCurrent; //���� ������ �ִ�hp�� mp : ���Ŀ� è�Ǿ� ��ũ��Ʈ���� �޾ƿ� ����

    float m_fCurrentRatio; //���� hp��mp ���� -> ���̴��� �����ؼ� ������ ���� ui���
    BARTYPE BarType;

public:
    virtual void begin() override;
    virtual void tick() override;
    virtual void BeginOverlap(CCollider2D* _Other) override;

    CLONE(CHpMpRatioUIScript);
public:
    CHpMpRatioUIScript(BARTYPE _Type);
    CHpMpRatioUIScript();
    ~CHpMpRatioUIScript();
};

