#pragma once
#include "CStructureScript.h"
class CNexusScript :
	public CStructureScript
{

public:
	CNexusScript();
	~CNexusScript();

private:
	float		m_fTime;				// ���� �ð�
	float		m_fMinionSpawnRate;		// �̴Ͼ� ���� �ֱ�

	bool		m_bSuperMinionSpawnMode;

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	void CheckStatus();
	void SpawnMinions();

	void SetSuperMinionSpawnMode(bool _b) { m_bSuperMinionSpawnMode = _b; }


};

