#pragma once
#include <Engine\CScript.h>

class CWorldHPSpawnScript :
    public CScript
{
private:
    vector<tWorldHPInfo> m_vOtherplayerInfo; //�ٸ� �÷��̾���� ����. �ڽ��� ������ �÷��̾��� ������ŭ ���;���.
    vector<CGameObject*> m_vWorldBar;
    //map���� ó�� �Ϸ�������, m_vOtherplayerInfo���� �Ź� �ٲ�Ƿ� �׳� ���� ���ͷ� ����� ���� �ε��� ��ȣ�� �������°����� ä����.

public:
    virtual void begin() override;
    virtual void tick() override;
    virtual void BeginOverlap(CCollider2D* _Other) override;

public:

    CLONE(CWorldHPSpawnScript);

public:
    CWorldHPSpawnScript();
    ~CWorldHPSpawnScript();
};

