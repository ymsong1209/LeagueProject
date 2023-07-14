#pragma once
#include "CRes.h"
class CGameObject;

class CPrefab :
	public CRes
{
private:
	CGameObject* m_ProtoObj;
public:
	CGameObject* Instantiate();

public:
	virtual int Load(const wstring& _strRelativePath) { SetRelativePath(_strRelativePath); return S_OK; }
public:
	virtual int Save(const wstring& _strRelativePath) { return S_OK; }


public:
	void RegisterProtoObject(CGameObject* _Proto);
	CGameObject* GetProtoObject() { return m_ProtoObj; }

public:
	CPrefab();
	~CPrefab();
};
