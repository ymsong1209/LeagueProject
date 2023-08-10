#pragma once
#include "CChampionScript.h"



class CJinxScript :
    public CChampionScript
{
public:
    CJinxScript();
    virtual ~CJinxScript();

protected:
    JinxWeaponMode  m_eWeaponMode;  // 무기 모드

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    void SetWeaponMode(JinxWeaponMode _mode) { m_eWeaponMode = _mode; }
    JinxWeaponMode GetWeaponMode() { return m_eWeaponMode; }
};

