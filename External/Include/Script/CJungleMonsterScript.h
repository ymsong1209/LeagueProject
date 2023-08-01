#pragma once
#include "CMobScript.h"

enum class JungleType
{
    DEFAULT,//Default면 안됨
    RAZORBEAK,
    RAZORBEAK_MINI,
    MURK_WOLF,
    WOLVES,
    KRUG,
    KRUG_MINI,
    GROMP,
    BLUE,
    RED,
    DRAGON,
    BARON,
    END
};

class CJungleMonsterScript :
    public CMobScript
{
private:
    JungleType      m_eJungleType;
    Vec3            m_vSpawnPos;

    CGameObject*    m_pTarget;

    bool            m_bReturnActive;        //다시 spawn지점으로 돌아갈것인지
    float           m_fMaxReturnTime;       //어그로 범위 밖에서 target이 몇초동안 있을수 있을지
    float           m_fCurReturnTime;          //어그로 범위 밖에서 target이 몇초동안 있었는지


public:
    bool    CheckDeath();
    // 어그로 범위 밖에서 target이 있는지 확인
    bool    CheckReturnActive();
    // 어그로 범위 밖에서 target이 몇초 동안 있는지 확인
    void    CheckReturnTime();
    
   


public:
    virtual void begin() override;
    virtual void tick() override;

public:
    CJungleMonsterScript(JungleType _type);
    ~CJungleMonsterScript();

    CLONE(CJungleMonsterScript);
};

