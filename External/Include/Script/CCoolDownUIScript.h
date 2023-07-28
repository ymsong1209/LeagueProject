#pragma once
#include <Engine\CScript.h>
#include "CUIScript.h"

class CCoolDownUIScript :
    public CScript
{
private:
    float m_dCoolDown;  //현재 스킬 쿨타임 진행시간(0~1 비율값임) 계산해서 셰이더로 넘겨줌 전체 스킬 쿨타임에서 어느정도 진행했는가?    
    bool m_bSkillUse;

    float   m_fcooldownTime; //추후에는 챔피언 스크립트에서 정보를 받아올 예정. //해당 스킬 쿨타임 시간값
    float   m_felapsedTime; //스킬 진행 시간 (스킬을 쓰고 지난시간)
public:
    virtual void begin() override;
    virtual void tick() override;

    CLONE(CCoolDownUIScript);
public:
    CCoolDownUIScript();
    ~CCoolDownUIScript();
};

