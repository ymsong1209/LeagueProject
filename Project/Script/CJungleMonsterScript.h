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
    float           m_fCurReturnTime;       //��׷� ���� �ۿ��� target�� ���ʵ��� �־�����

    bool            m_bTest;                //���� �޾Ҵ��� Ȱ��ȭ���ִ� testcode,


public:
    bool    CheckDeath();
    // ��׷� ���� �ۿ��� target�� �ִ��� Ȯ��
    void    CheckReturnActive();
    // ��׷� ���� �ۿ��� target�� ���� ���� �ִ��� Ȯ��
    void    CheckReturnTime();

    void    SetSpawnPos(Vec3 _pos) { m_vSpawnPos = _pos; }
    
   


public:
    virtual void begin() override;
    virtual void tick() override;

    //Overlap�Լ��� ���� ��Ÿ� �ȿ� ���������� �����ϴ� �뵵��
    virtual void BeginOverlap(CCollider2D* _Other) override;
    virtual void EndOverlap(CCollider2D* _Other) override;

public:
    CJungleMonsterScript(JungleType _type);
    ~CJungleMonsterScript();

    CLONE(CJungleMonsterScript);
};

