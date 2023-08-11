#pragma once
#include <Engine\CScript.h>
#include "CUIScript.h"

class CUnitScript;
class CSkill;

class CCoolDownUIScript :
    public CScript
{
private:
    float m_fCoolRatio;  //���� ��ų ��Ÿ�� ����ð�(0~1 ��������) ����ؼ� ���̴��� �Ѱ��� ��ü ��ų ��Ÿ�ӿ��� ������� �����ߴ°�?    
    
    bool m_bSkillUse;

    float   m_fCooldownTime; //���Ŀ��� è�Ǿ� ��ũ��Ʈ���� ������ �޾ƿ� ����. //�ش� ��ų ��Ÿ�� �ð���
    float   m_fCurCoolTime; //��ų�� ���� �����ð�
    COOL_DOWN_TYPE    m_SkillType;
    ChampionType    m_ChampType;

    CUnitScript* m_PlayerUnitScript;
    CSkill* m_SkillQ;
    CSkill* m_SkillW;
    CSkill* m_SkillE;
    CSkill* m_SkillR;

public:
    virtual void begin() override;
    virtual void tick() override;

    void SetChampType(ChampionType _ChampType) { m_ChampType = _ChampType; }

    CLONE(CCoolDownUIScript);
public:
    CCoolDownUIScript(COOL_DOWN_TYPE _SkillType);
    CCoolDownUIScript();
    ~CCoolDownUIScript();
};

