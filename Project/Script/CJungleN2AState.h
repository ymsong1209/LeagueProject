#pragma once
#include "CUnitState.h"
// Idle -> Alert animation�����ϴ� state
// n2a animation�� ���� alert animation�� ���� ���, Enter���� �ٷ� changestate�� alert�� �ϱ� �ٶ�
// tick�� ��ӹ޾Ƽ� �������� ����ξ����
class CJungleN2AState :
    public CUnitState
{

private:
    bool        m_bDetectChampion;
public:
    virtual void Enter();
    virtual void tick();
    virtual void Exit();
public:
    //void HandleEvent(CGameEvent& event) override;
public:
    CJungleN2AState();
    ~CJungleN2AState();
};
