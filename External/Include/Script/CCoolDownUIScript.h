#pragma once
#include <Engine\CScript.h>
#include "CUIScript.h"

class CUnitScript;
class CSkill;

class CCoolDownUIScript :
    public CScript
{
private:
    float m_fCoolRatio;  //현재 스킬 쿨타임 진행시간(0~1 비율값임) 계산해서 셰이더로 넘겨줌 전체 스킬 쿨타임에서 어느정도 진행했는가?    
    
    bool m_bSkillUse;

    float   m_fCooldownTime; //추후에는 챔피언 스크립트에서 정보를 받아올 예정. //해당 스킬 쿨타임 시간값
    float   m_fCurCoolTime; //스킬을 쓰고 남은시간
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

