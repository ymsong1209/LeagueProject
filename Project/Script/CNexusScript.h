#pragma once
#include "CStructureScript.h"
class CNexusScript :
	public CStructureScript
{

public:
	CNexusScript();
	~CNexusScript();

private:
	vector<CGameObject*>    m_vecTurrets;

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	void CheckStatus();

};

