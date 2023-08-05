#pragma once
#include <Engine\CScript.h>

class CSkill;

class CUnitScript :
    public CScript
{
protected:
    CUnitScript(UINT ScriptType);

public:
    CUnitScript();
    virtual ~CUnitScript();

    CLONE(CUnitScript);

protected:
    UINT64                  m_iServerID;        // �������� �ο����� ID
    wstring                 m_strNickname;      // �г���
    bool                    m_bHost;            // �������� ����
   
    UnitType                m_eUnitType;        // ���� ����
    Faction                 m_eFaction;         // ����(����, ���, �߸�)

    CC                      m_eCurCC;
    RESTRAINT               m_eRestraint;
        
    CSkill*                 m_Skill[5];         // Ÿ��/���� ���� 0�� ��ų(��Ÿ)�� ����!
    int                     m_SkillLevel[5];    // Ÿ��/���� ���� 0�� ��ų 1�� ����

    float                   m_fHP;              // ü��
    float                   m_fMaxHP;           // ��ü ü��
    float                   m_fMP;
    float                   m_fMaxMP;

    float                   m_fAttackPower;     // ���ݷ�
    float                   m_fDefencePower;    // ����


    float                   m_fAttackSpeed;     // ����(��Ÿ)�ӵ�
    float                   m_fAttackRange;     // ����(��Ÿ)��Ÿ�
    float                   m_fMoveSpeed;       // �̵��ӵ�

    Vec3                    m_vNextPos;
    float                   m_fFaceRot;

    bool                    m_bUnitDead;      // ������ �׾�����(HP 0 ����)

public:
    virtual void begin() override;
    virtual void tick() override {};

    virtual UnitType GetType() { return m_eUnitType; }
    virtual Faction GetFaction() { return m_eFaction; }

public:
    bool IsUnitDead() { return m_bUnitDead; }
    void SetUnitDead(bool _dead) { m_bUnitDead = _dead; }

    // ������Ʈ�� PathFinder ������Ʈ�� ���� ��ΰ��� ���� ��, �ش� ��η� �̵�
    bool PathFindMove(float _fSpeed, bool _IsRotation = true);

public:
    float GetCurHP() { return m_fHP; }
    float GetMaxHP() { return m_fMaxHP; }
    void  SetCurHP(float _f) { m_fHP = _f; }         // ���� ü�� = ���ڰ�
    void  SetCurHPVar(float _f) { m_fHP += _f; }     // ���� ü�� += ���ڰ�
    float GetMaxHP() const { return m_fMaxHP; }

    float GetCurMP() { return m_fMP; }
    void  SetCurMP(float _f) { m_fMP = _f; }
    void  SetCurMPVar(float _f) { m_fMP += _f; }
    float GetMaxMP() const { return m_fMaxMP; }

    UINT64           GetServerID() { return m_iServerID; }
    wstring          GetNickname() const { return m_strNickname; }
    bool             IsHost() const { return m_bHost; }
    UnitType         GetUnitType() const { return m_eUnitType; }
    Faction          GetFaction() const { return m_eFaction; }

    void             SetServerID(unsigned __int64 serverID) { m_iServerID = serverID; }
    void             SetNickname(wstring nickname) { m_strNickname = nickname; }
    void             SetHost(bool isHost) { m_bHost = isHost; }
    void             SetFaction(Faction faction) { m_eFaction = faction; }

    // ��Ÿ �ʿ��� Getter/Setter �Լ� �߰�
    float  GetAttackPower() { return m_fAttackPower; }
    float  GetDefencePower() { return m_fDefencePower; }
    float   GetAttackRange() { return m_fAttackRange; }

    void   SetAttackPower(float _attackPower) { m_fAttackPower = _attackPower; }
    void   SetDefencePower(float _defencePower) { m_fDefencePower = _defencePower; }
    void    SetAttackRange(float _range) { m_fAttackRange = _range; }

    float   GetFaceRot() { return m_fFaceRot; }

    // �񵿱�
    void GetHit(SkillType _type, CGameObject* _SkillTarget, CGameObject* _SkillUser, int _SkillLevel);


    CSkill* GetSkill(int _i) { if (_i < 0 || _i >= 5) return nullptr; return m_Skill[_i]; }
    int     GetSkillLevel(int _i) { return m_SkillLevel[_i]; }
};
