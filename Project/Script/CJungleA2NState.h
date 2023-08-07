#pragma once
#include "CUnitState.h"
//Alert To Normal State
//�ִϸ��̼��� ���� ��쿡 �׳� �ٷ� enter�� changestate idle�� ���ָ� �ȴ�.
//JungleA2NState�� ��ӹ��� MonsterA2N������ tick�� ��ӹް� ������ ����θ� ��.
class CJungleA2NState :
    public CUnitState
{
public:
    virtual void Enter();
    virtual void tick();
    virtual void Exit();
public:
    void HandleEvent(CGameEvent& event) override;

public:
    CJungleA2NState();
    ~CJungleA2NState();
};

