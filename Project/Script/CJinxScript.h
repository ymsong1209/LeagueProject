#pragma once
#include "CChampionScript.h"

enum JinxWeaponMode
{
    MINIGUN,            // ���ݼӵ�++
    ROCKET_LAUNCHER,    // ��Ÿ�++ ���ݼӵ�-- ��������
};

class CJinxScript :
    public CChampionScript
{
public:
    CJinxScript();
    virtual ~CJinxScript();

private:
    JinxWeaponMode  m_eWeaponMode;  // ���� ���

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    void SetWeaponMode(JinxWeaponMode _mode) { m_eWeaponMode = _mode; }
    JinxWeaponMode GetWeaponMode() { return m_eWeaponMode; }
};

