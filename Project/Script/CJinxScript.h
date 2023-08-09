#pragma once
#include "CChampionScript.h"

enum JinxWeaponMode
{
    MINIGUN,            // 공격속도++
    ROCKET_LAUNCHER,    // 사거리++ 공격속도-- 범위피해
};

class CJinxScript :
    public CChampionScript
{
public:
    CJinxScript();
    virtual ~CJinxScript();

private:
    JinxWeaponMode  m_eWeaponMode;  // 무기 모드

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    void SetWeaponMode(JinxWeaponMode _mode) { m_eWeaponMode = _mode; }
    JinxWeaponMode GetWeaponMode() { return m_eWeaponMode; }
};

