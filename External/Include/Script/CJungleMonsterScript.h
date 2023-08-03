#pragma once
#include "CMobScript.h"

enum class JungleType
{
    CRIMSON_RAPTOR,
    RAPTORS,
    MURK_WOLF,
    WOLVES,
    ANCIENT_KRUG,
    KRUG,
    GROMP,
    BLUE,
    RED,
    DRAGON,
};

class CJungleMonsterScript :
    public CMobScript
{
public:
    CJungleMonsterScript();
    ~CJungleMonsterScript();

    CLONE(CJungleMonsterScript);

private:
    JungleType      m_eJungleType;

    Vec3            m_vSpawnPos;
    

public:
    virtual void begin() override;
    virtual void tick() override;
};

