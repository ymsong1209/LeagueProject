#pragma once
#include "CBasicAttack.h"
class CDragonBasicAttack :
	public CBasicAttack
{
public:
	CDragonBasicAttack();
	~CDragonBasicAttack();

private:

public:
	virtual void tick() override;
	virtual bool Use() override;

	virtual void GetHit(CUnitScript* _UserScript, CUnitScript* _TargetScript, int _skillLevel);
};


