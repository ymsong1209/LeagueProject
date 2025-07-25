#pragma once
#include "CEntity.h"


class CComponent;
class CTransform;
class CMeshRender;
class CCollider2D;
class CCollider3D;
class CAnimator2D;
class CAnimator3D;
class CLight2D;
class CLight3D;
class CCamera;
class CFsm;
class CPathFinder;
class CRenderComponent;
class CScript;
class CTileMap;
class CParticleSystem;
class CSkyBox;
class CDecal;
class CLandScape;
class CScript;

#define GET_COMPONENT(Type, TYPE) C##Type* Type() const { return (C##Type*)m_arrCom[(UINT)COMPONENT_TYPE::TYPE]; }



class CGameObject :
    public CEntity
{
private:
    CComponent*             m_arrCom[(UINT)COMPONENT_TYPE::END];
    CRenderComponent*       m_RenderCom;//GameObject는 RenderComponent가 하나여야만 한다.
    vector<CScript*>        m_vecScript;//Script가 여러개일수도 있음

    CGameObject*            m_Parent;
    vector<CGameObject*>    m_vecChild;

    int                     m_iLayerIdx;
    bool                    m_bDead;
    float                   m_LifeTime;
    float                   m_CurLifeTime;
    bool                    m_bLifeSpan;




public:
    void begin();       // 레벨이 시작될 때 호출 or 시작 된 레벨에 합류할 때
    void RegisterObject(); //Layer에 자기자신 등록
    void tick();        
    virtual void finaltick();
    virtual void finaltick_module();
    void render();



public:
    void AddComponent(CComponent* _Component);
    void AddChild(CGameObject* _Object);

    void ChangeLayer(int _NewLayerIdx);
    void ChangeLayer(const wstring& _LayerName);

    CComponent* GetComponent(COMPONENT_TYPE _ComType) { return m_arrCom[(UINT)_ComType]; }
    const vector<CGameObject*>& GetChild() { return m_vecChild; }
    CGameObject* FindChildObjByName(wstring _name);


    CGameObject* GetParent() const { return m_Parent; }

    GET_COMPONENT(Transform, TRANSFORM);
    GET_COMPONENT(MeshRender, MESHRENDER);
    GET_COMPONENT(Camera, CAMERA);
    GET_COMPONENT(Collider2D, COLLIDER2D);
    GET_COMPONENT(Collider3D, COLLIDER3D);
    GET_COMPONENT(Light2D, LIGHT2D);
    GET_COMPONENT(Light3D, LIGHT3D);
    GET_COMPONENT(TileMap, TILEMAP);
    GET_COMPONENT(Animator2D, ANIMATOR2D);
    GET_COMPONENT(Animator3D, ANIMATOR3D);
    GET_COMPONENT(ParticleSystem, PARTICLESYSTEM);
    GET_COMPONENT(SkyBox, SKYBOX);
    GET_COMPONENT(Decal, DECAL);
    GET_COMPONENT(LandScape, LANDSCAPE);
    GET_COMPONENT(Fsm, FSM);
    GET_COMPONENT(PathFinder, PATHFINDER);

    CRenderComponent* GetRenderComponent() const {  return m_RenderCom; }


    int GetLayerIndex() { return m_iLayerIdx; }

    template<typename T>
    T* GetScript();

    const vector<CScript*>& GetScripts() { return m_vecScript; }

    void SetLifeSpan(float _fTime)
    {
        if (m_bLifeSpan)
            return;

        m_LifeTime = _fTime;
        m_bLifeSpan = true;
    }

    bool IsDead() { return m_bDead; }
    bool IsAncestor(CGameObject* _Target);

    float GetLifeSpan() { return m_LifeTime; }
    float GetCurLifeSpan() { return m_CurLifeTime; }

private:
    void DisconnectFromParent();
    void ChangeToChildType();
    void AddParentList();


    CLONE(CGameObject)    
public:
    CGameObject();
    CGameObject(const CGameObject& _Other);
    ~CGameObject();

    friend class CLayer;
    friend class CEventMgr;
};

template<typename T>
inline T* CGameObject::GetScript()
{
    for (size_t i = 0; i < m_vecScript.size(); ++i)
    {
        T* pScript = dynamic_cast<T*> (m_vecScript[i]);
        if (nullptr != pScript)
            return pScript;
    }

    return nullptr;
}
