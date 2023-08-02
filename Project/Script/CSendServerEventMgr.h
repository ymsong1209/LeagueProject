#pragma once
#include <Engine/CSingleton.h>

class CGameEvent;

//// Event
//struct tServerEvent
//{
//    SERVER_EVENT_TYPE Type;
//    DWORD_PTR   wParam;
//    DWORD_PTR   lParam;
//};

class CSendServerEventMgr :
    public CSingleton<CSendServerEventMgr>
{
    SINGLE(CSendServerEventMgr);

public:
    vector<tServerEvent> m_vecServerSendEvent;

public:
    void AddServerSendEvent(tServerEvent _evn) { m_vecServerSendEvent.push_back(_evn); }

    vector<tServerEvent>& GetVecEvent() { return m_vecServerSendEvent; }
    void ClearServerSendEvent() { m_vecServerSendEvent.clear(); }

    // Hit  : �¾Ҵ�.
    // Anim : �ִϸ��̼��� ����Ǿ���.
    // UseSkill : ��ų�� ����ҰŶ� ����ü�� �����϶�.
    // Despawn : �� ������Ʈ�� ���ֶ�.
    // Dead : �� ���� (KDA ������Ʈ ��, �̴Ͼ�CS)
    // Sound : ����
    void SendHitPacket(UINT64 _skillObjId, UINT64 _hitObjId, UINT64 _useObjId, int _skillLevel, SkillType _skillType);
    void SendAnimPacket(UINT64 _targetId, wstring _animName, bool _repeat, bool _blend, float _blentTime);
    void SendUseSkillPacket(UINT64 _ownerId, UINT64 _targetObjId, int _skillLevel
                            , SkillType _skillType, Vec3 _offsetPos
                            , bool _useMousePos, Vec3 _mousePos, bool _useMouseDir, Vec3 _mouseDir);
    void SendDespawnPacket(UINT64 _objId, float _lifeSpan);
    void SendDeadPacket(SkillType _skillType);
    void SendSoundPacket();
};

