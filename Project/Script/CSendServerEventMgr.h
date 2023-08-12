#pragma once
#include <Engine/CSingleton.h>

class CGameObject;

class CSendServerEventMgr :
    public CSingleton<CSendServerEventMgr>
{
    SINGLE(CSendServerEventMgr);

public:
    vector<tServerEvent> m_vecServerSendEvent;  // Server에게 보내는 이벤트
    vector<tServerEvent> m_vecUISendEvent;      // UI에게 보내는 이벤트 (킬로그)

    // UI용
    CGameObject*         m_myPlayerObj;
    int                  m_myKillCnt;
    int                  m_myDeathCnt;
    int                  m_myCSCnt;

    int                  m_RedScore;
    int                  m_BlueScore;

    vector<CGameObject*>     m_vecAllPlayer;


public:
    void SetMyPlayer(CGameObject* _myPlayerObj) { m_myPlayerObj = _myPlayerObj; }
    void SetVecAllPlyer(vector<CGameObject*> _allPlayer) { m_vecAllPlayer = _allPlayer; }
    
    void AddMyKillCnt(int _cnt) { m_myKillCnt += _cnt; }
    void AddMyDeathCnt(int _cnt) { m_myDeathCnt += _cnt; }
    void AddMyCSCnt(int _cnt) { m_myCSCnt += _cnt; }
    void AddRedScore(int _cnt) { m_RedScore += _cnt; }
    void AddBlueScore(int _cnt) { m_BlueScore += _cnt; }

    vector<CGameObject*> GetVecAllPlayer() { return m_vecAllPlayer; }
    CGameObject* GetMyPlayer() { return m_myPlayerObj; }

    int GetMyKillCnt() { return m_myKillCnt; }
    int GetMyDeathCnt() { return m_myDeathCnt; }
    int GetMyCSCnt() { return m_myCSCnt; }
    int GetRedScore() { return m_RedScore; }
    int GetBlueScore() { return m_BlueScore; }

    
    // ServerSendEvent 용
    void AddServerSendEvent(tServerEvent _evn) { m_vecServerSendEvent.push_back(_evn); }
    vector<tServerEvent>& GetVecEvent() { return m_vecServerSendEvent; }
    void ClearServerSendEvent() { m_vecServerSendEvent.clear(); }


    // UISendEvent 용
    void AddUISendEvent(tServerEvent _evn) { m_vecUISendEvent.push_back(_evn); }
    vector<tServerEvent>& GetUIEvent() { return m_vecUISendEvent; }
    void ClearUISendEvent() { m_vecUISendEvent.clear(); }


    // Hit  : 맞았다.
    // Anim : 애니메이션이 변경되었다.
    // UseSkill : 스킬을 사용할거라 투사체를 생성하라.
    // Despawn : 이 오브젝트를 없애라.
    // KDACS : 얘 죽음 (KDA 업데이트 용, 미니언CS)
    // Sound : 사운드 play
    // Mtrl  : 재질에 Texparam set 
    void SendHitPacket(UINT64 _skillObjId, UINT64 _hitObjId, UINT64 _useObjId, int _skillLevel, SkillType _skillType);
    void SendAnimPacket(UINT64 _targetId, wstring _animName, bool _repeat, bool _bRepeatBlend,  bool _bUseBlend, float _blentTime, float _animSpeed = 1.f);
    void SendUseSkillPacket(UINT64 _ownerId, UINT64 _targetObjId, int _skillLevel
                            , SkillType _skillType, Vec3 _offsetPos, int _projectileCount
                            , bool _useMousePos, Vec3 _mousePos, bool _useMouseDir, Vec3 _mouseDir);
    void SendDespawnPacket(UINT64 _objId, float _lifeSpan);
    void SendKDACSPacket(UINT64 _killerId, UINT64 _victimId, UnitType _deadObjUnitType);
    //Faction None : 모두가 들어야하는 사운드
    void SendSoundPacket(wstring _soundName, int _loopCount, float _fVolume, bool _bOverlap, float _fRange, Vec3 _soundPos, Faction _faction = Faction::NONE);
    
    //settexparam할때 보내는 패킷
    ///안씀. 아래 참조 void SendMtrlPacket(UINT64 _objId, int _mtrlIndex, TEX_PARAM _texParam, wstring _TexName);

    // TexParam 예시 : GetOwner()->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, normaltex);  Ptr<CTexture> normaltex = CResMgr::GetInst()->FindRes<CTexture>(L"texture\\FBXTexture\\malphite.dds");
    // SetMtrl 예시  : GetOwner()->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\turret_rubble_Rubble_red.mtrl"), 0);
    void SendSetTexParamPacket(UINT64 _objId, int _mtrlIndex, TEX_PARAM _texParam, wstring _TexName); 
    void SendSetMtrlPacket(UINT64 _objId, int _mtrlIndex, wstring _mtrlName);
};

