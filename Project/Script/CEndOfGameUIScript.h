#pragma once
#include <Engine\CScript.h>

class CEndOfGameUIScript :
    public CScript
{
private:
    float m_fCurTimer; //�� hp�� mp (ä���� �ϴ� exp ��)
    float m_fAlpha; // ���� �� �ʱ�ȭ
    float m_fTimer; // �ð� ����� ���� Ÿ�̸� ����
    float m_fDuration; // ���İ��� 0���� 1�� ���ϴ� �� �ɸ��� �ð� (0.8��)

    bool m_bAnimating;
    float animationDuration;
    float animationDistance;

    float m_fAnimTimer;
public:
    virtual void begin() override;
    virtual void tick() override;
    CLONE(CEndOfGameUIScript);
public:
    CEndOfGameUIScript();
    ~CEndOfGameUIScript();
};

