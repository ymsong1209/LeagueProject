#pragma once
#include <Engine/CSingleton.h>

class CGameObject;

class CSendServerEventMgr :
    public CSingleton<CSendServerEventMgr>
{
    SINGLE(CSendServerEventMgr);

public:
    vector<tServerEvent> m_vecServerSendEvent;
    CGameObject*         m_myPlayerObj;

public:
    void SetMyPlayer(CGameObject* _myPlayerObj) { m_myPlayerObj = _myPlayerObj; }
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
    void SendKDACSPacket(UINT64 _killerId, UnitType _deadObjUnitType);
    void SendSoundPacket();
};

