#pragma once
#include "CRes.h"
class CGameObject;

//PrefabObject는 CLevelSaveJson이 아니라 CLevelSave함수 사용합니다.
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
