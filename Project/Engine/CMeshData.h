#pragma once
#include "CRes.h"

#include "Ptr.h"
#include "CMaterial.h"
#include "CMesh.h"


class CMeshData :
	public CRes
{
	Ptr<CMesh>				m_pMesh;
	vector<Ptr<CMaterial>>	m_vecMtrl;


public:
	static CMeshData* LoadFromFBX(const wstring& _strFilePath);

	virtual int Save(const wstring& _strFilePath) override;
	virtual int Load(const wstring& _strFilePath) override;

	CGameObject* Instantiate();

	const Ptr<CMesh>& GetMesh() { return m_pMesh; }
	const vector<Ptr<CMaterial>>& GetMaterial() { return m_vecMtrl; }

	CLONE_DISABLE(CMeshData)
public:
	CMeshData(bool _bEngine = false);
	virtual ~CMeshData();
};
