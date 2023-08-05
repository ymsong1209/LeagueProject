#pragma once
#include <Engine/CSingleton.h>

class CGameObject;

class CSendServerEventMgr :
    public CSingleton<CSendServerEventMgr>
{
    SINGLE(CSendServerEventMgr);

public:
    vector<tServerEvent> m_vecServerSendEvent;

    // UI��
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

    
    
    void AddServerSendEvent(tServerEvent _evn) { m_vecServerSendEvent.push_back(_evn); }

    vector<tServerEvent>& GetVecEvent() { return m_vecServerSendEvent; }
    void ClearServerSendEvent() { m_vecServerSendEvent.clear(); }


    // Hit  : �¾Ҵ�.
    // Anim : �ִϸ��̼��� ����Ǿ���.
    // UseSkill : ��ų�� ����ҰŶ� ����ü�� �����϶�.
    // Despawn : �� ������Ʈ�� ���ֶ�.
    // KDACS : �� ���� (KDA ������Ʈ ��, �̴Ͼ�CS)
    // Sound : ����
    void SendHitPacket(UINT64 _skillObjId, UINT64 _hitObjId, UINT64 _useObjId, int _skillLevel, SkillType _skillType);
    void SendAnimPacket(UINT64 _targetId, wstring _animName, bool _repeat, bool _blend, float _blentTime);
    void SendUseSkillPacket(UINT64 _ownerId, UINT64 _targetObjId, int _skillLevel
                            , SkillType _skillType, Vec3 _offsetPos, int _projectileCount
                            , bool _useMousePos, Vec3 _mousePos, bool _useMouseDir, Vec3 _mouseDir);
    void SendDespawnPacket(UINT64 _objId, float _lifeSpan);
    void SendKDACSPacket(UINT64 _killerId, UINT64 _victimId, UnitType _deadObjUnitType);
    void SendSoundPacket();
};

