#pragma once
#include <Engine\CScript.h>

class CTimedEffect;

class CWorldHPSpawnScript :
    public CScript
{
private:
    vector<tWorldHPInfo> m_vOtherplayerInfo; //다른 플레이어들의 정보. 자신을 제외한 플레이어의 개수만큼 들어와야함.
    CGameObject* m_MyPlayerWorldBar;
    vector<CGameObject*> m_vOtherWorldBar;
    //map으로 처리 하려했으나, m_vOtherplayerInfo값은 매번 바뀌므로 그냥 각자 벡터로 만들고 같은 인덱스 번호를 가져오는것으로 채택함.
    CGameObject* m_MyPlayer;
    vector<CGameObject*> m_OtherPlayer;

public:
    virtual void begin() override;
    virtual void tick() override;
    virtual void BeginOverlap(CCollider2D* _Other) override;

    void UISpawn(CGameObject* _PlayerObj,CGameObject* _WorldBarObj);

    void DisplayLastCCEffect(CGameObject* _PlayerObj, const wstring& nickname, Vec2 _DisplayPos);
	// 우선순위에 따라 CC 타입을 반환하는 함수
    wstring GetHighestPriorityCC(UINT m_eCurCC, wstring _NickName);



public:
    CLONE(CWorldHPSpawnScript);

public:
    CWorldHPSpawnScript();
    ~CWorldHPSpawnScript();
};

