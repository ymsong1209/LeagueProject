#pragma once
#include <Engine\CScript.h>
#include "CUIScript.h"

class CCoolDownUIScript :
    public CScript
{
private:
    float m_dCoolDown;  //���� ��ų ��Ÿ�� ����ð�(0~1 ��������) ����ؼ� ���̴��� �Ѱ��� ��ü ��ų ��Ÿ�ӿ��� ������� �����ߴ°�?    
    bool m_bSkillUse;

    float   m_fcooldownTime; //���Ŀ��� è�Ǿ� ��ũ��Ʈ���� ������ �޾ƿ� ����. //�ش� ��ų ��Ÿ�� �ð���
    float   m_felapsedTime; //��ų ���� �ð� (��ų�� ���� �����ð�)
public:
    virtual void begin() override;
    virtual void tick() override;

    CLONE(CCoolDownUIScript);
public:
    CCoolDownUIScript();
    ~CCoolDownUIScript();
};

