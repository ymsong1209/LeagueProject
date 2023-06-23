#pragma once

class CGameObjectEx;

class CEditorObjMgr
	: public CSingleton<CEditorObjMgr>
{
	SINGLE(CEditorObjMgr);
private:
	vector<CGameObjectEx*>	m_vecEditorObj;

	CGameObjectEx*			m_DebugShape[(UINT)SHAPE_TYPE::END];
	vector<tDebugShapeInfo> m_DebugShapeInfo;

	CGameObjectEx* m_DebugBounding;
	vector<tDebugBoundingInfo>  m_vecBoundingInfo;

public:
	void CreateFrustumDebugMesh();

	void init();
	void progress();

private:
	void tick();
	void render();
};

