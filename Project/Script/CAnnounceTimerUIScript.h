#pragma once
#include <Engine\CScript.h>


class CAnnounceTimerUIScript :
    public CScript
{
private:
    float m_fCurTimer; //�� hp�� mp (ä���� �ϴ� exp ��)
    float m_fAlpha; // ���� �� �ʱ�ȭ
    float m_fTimer; // �ð� ����� ���� Ÿ�̸� ����
    float m_fDuration; // ���İ��� 0���� 1�� ���ϴ� �� �ɸ��� �ð� (0.8��)

public:
    virtual void begin() override;
    virtual void tick() override;
    CLONE(CAnnounceTimerUIScript);
public:
    CAnnounceTimerUIScript();
    ~CAnnounceTimerUIScript();
};

