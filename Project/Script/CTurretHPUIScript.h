#pragma once
#include <Engine\CScript.h>

class CTurretHPUIScript :
    public CScript
{
private:
    float m_fCurHP; //�� hp�� mp (ä���� �ϴ� exp ��)
    float m_fTotalHP; //���� ������ �ִ�hp�� mp : ���Ŀ� è�Ǿ� ��ũ��Ʈ���� �޾ƿ� ����
    float m_fRatio; //���� hp��mp ���� -> ���̴��� �����ؼ� ������ ���� ui���

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

