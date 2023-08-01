#pragma once
#include "CMobScript.h"

enum class JungleType
{
    DEFAULT,//Default�� �ȵ�
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

    bool            m_bReturnActive;        //�ٽ� spawn�������� ���ư�������
    float           m_fMaxReturnTime;       //��׷� ���� �ۿ��� target�� ���ʵ��� ������ ������
    float           m_fCurReturnTime;          //��׷� ���� �ۿ��� target�� ���ʵ��� �־�����


public:
    bool    CheckDeath();
    // ��׷� ���� �ۿ��� target�� �ִ��� Ȯ��
    bool    CheckReturnActive();
    // ��׷� ���� �ۿ��� target�� ���� ���� �ִ��� Ȯ��
    void    CheckReturnTime();
    
   


public:
    virtual void begin() override;
    virtual void tick() override;

public:
    CJungleMonsterScript(JungleType _type);
    ~CJungleMonsterScript();

    CLONE(CJungleMonsterScript);
};

