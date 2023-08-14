#pragma once
#include <Engine\CScript.h>

class CTimedEffect;

class CWorldHPSpawnScript :
    public CScript
{
private:
    vector<tWorldHPInfo> m_vOtherplayerInfo; //�ٸ� �÷��̾���� ����. �ڽ��� ������ �÷��̾��� ������ŭ ���;���.
    CGameObject* m_MyPlayerWorldBar;
    vector<CGameObject*> m_vOtherWorldBar;
    //map���� ó�� �Ϸ�������, m_vOtherplayerInfo���� �Ź� �ٲ�Ƿ� �׳� ���� ���ͷ� ����� ���� �ε��� ��ȣ�� �������°����� ä����.
    CGameObject* m_MyPlayer;
    vector<CGameObject*> m_OtherPlayer;

public:
    virtual void begin() override;
    virtual void tick() override;
    virtual void BeginOverlap(CCollider2D* _Other) override;

    void UISpawn(CGameObject* _PlayerObj,CGameObject* _WorldBarObj);

    void DisplayLastCCEffect(CGameObject* _PlayerObj, const wstring& nickname, Vec2 _DisplayPos);
	// �켱������ ���� CC Ÿ���� ��ȯ�ϴ� �Լ�
    wstring GetHighestPriorityCC(UINT m_eCurCC, wstring _NickName);



public:
    CLONE(CWorldHPSpawnScript);

public:
    CWorldHPSpawnScript();
    ~CWorldHPSpawnScript();
};

