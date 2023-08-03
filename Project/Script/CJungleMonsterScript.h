#pragma once
#include "CMobScript.h"

enum class JungleType
{
    DEFAULT,//Default면 안됨
    RAZORBEAK,
    RAZORBEAK_MINI,
    MURK_WOLF,
    MURK_WOLF_MINI,
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
protected:
    JungleType      m_eJungleType;
    Vec3            m_vSpawnPos;   
    Vec3            m_vSpawnRot;

    CGameObject*    m_pTarget;

    bool            m_bReturnActive;        //다시 spawn지점으로 돌아갈것인지
    float           m_fMaxReturnTime;       //어그로 범위 밖에서 target이 몇초동안 있을수 있을지
    float           m_fCurReturnTime;       //어그로 범위 밖에서 target이 몇초동안 있었는지

    bool            m_bTest;                //공격 받았는지 활성화해주는 testcode,


public:
    bool    CheckDeath();
    // 어그로 범위 밖에서 target이 있는지 확인
    void    CheckReturnActive();
    // 어그로 범위 밖에서 target이 몇초 동안 있는지 확인
    void    CheckReturnTime();

    void    SetSpawnPos(Vec3 _pos) { m_vSpawnPos = _pos; }
    Vec3    GetSpawnPos() { return m_vSpawnPos; }
    void    SetSpawnRot(Vec3 _rot) { m_vSpawnRot = _rot; }
    Vec3    GetSpawnRot() { return m_vSpawnRot; }
   
    CGameObject* GetTarget() { return m_pTarget; }

public:
    virtual void begin() override;
    virtual void tick() override;

    //Overlap함수는 공격 사거리 안에 들어와있을때 공격하는 용도임
    virtual void BeginOverlap(CCollider2D* _Other) override;
    virtual void EndOverlap(CCollider2D* _Other) override;

public:
    CJungleMonsterScript(UINT ScriptType);
    CJungleMonsterScript();
    ~CJungleMonsterScript();

    CLONE(CJungleMonsterScript);
};

