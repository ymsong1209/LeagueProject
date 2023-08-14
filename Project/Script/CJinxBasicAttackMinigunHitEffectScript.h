#pragma once
#include  <Engine/CScript.h>
class CJinxBasicAttackMinigunHitEffectScript :
    public CScript
{
private:
    CGameObject* SpawnObject;
    float m_fStartTime;
    bool  m_bSecontHitEffectSpawned;
    bool  m_bThirdHitEffectSpawned;


public:
    virtual void begin() override;
    virtual void tick() override;
    CLONE(CJinxBasicAttackMinigunHitEffectScript);


public:
    CJinxBasicAttackMinigunHitEffectScript();
    ~CJinxBasicAttackMinigunHitEffectScript();

};

 