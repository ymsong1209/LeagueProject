#pragma once
#include "CMobScript.h"


class CDragonScript :
    public CMobScript
{
private:
    Vec3            m_vAggroPos;
    Vec3            m_vAlertPos;
    Vec3            m_vSpawnPos;
    Vec3            m_vSpawnRot;
    float           m_fAlertRange;          //Dragon�� alertrange������ ������ ��׷ΰ� Ǯ��.

    CGameObject*    m_pTarget;

    bool            m_bReturnActive;        //�ٽ� spawn�������� ���ư�������
    float           m_fMaxReturnTime;       //��׷� ���� �ۿ��� target�� ���ʵ��� ������ ������
    float           m_fCurReturnTime;       //��׷� ���� �ۿ��� target�� ���ʵ��� �־�����


public:
    void    AddState();

    bool    CheckDeath();
    // ��׷� ���� �ۿ��� target�� �ִ��� Ȯ��
    void    CheckReturnActive();
    // ��׷� ���� �ۿ��� target�� ���� ���� �ִ��� Ȯ��
    void    CheckReturnTime();

    void    SetSpawnPos(Vec3 _pos) { m_vSpawnPos = _pos; }
    Vec3    GetSpawnPos() { return m_vSpawnPos; }
    void    SetSpawnRot(Vec3 _rot) { m_vSpawnRot = _rot; }
    Vec3    GetSpawnRot() { return m_vSpawnRot; }
    void    SetAggroPos(Vec3 _pos) { m_vAggroPos = _pos; }
    Vec3    GetAggroPos() { return m_vAggroPos; }
    void    SetAlertPos(Vec3 _pos) { m_vAlertPos = _pos; }
    Vec3    GetAlertPos() { return m_vAlertPos; }
    void    SetAlertRange(float _range) { m_fAlertRange = _range; }
    float   GetAlertRange() { return m_fAlertRange; }

    CGameObject* GetTarget() { return m_pTarget; }

    void GetHit(CGameObject* _target);
    void ReleaseTarget() { m_pTarget = nullptr; }

public:
    virtual void begin() override;
    virtual void tick() override;

    //Overlap�Լ��� ���� ��Ÿ� �ȿ� ���������� �����ϴ� �뵵��
    virtual void BeginOverlap(CCollider2D* _Other) override;
    virtual void EndOverlap(CCollider2D* _Other) override;

public:
    CDragonScript(UINT ScriptType);
    CDragonScript();
    ~CDragonScript();

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _FILE) override;

    virtual void SaveToLevelJsonFile(Value& _objValue, Document::AllocatorType& allocator) override;
    virtual void LoadFromLevelJsonFile(const Value& _componentValue) override;

    CLONE(CDragonScript);
};

