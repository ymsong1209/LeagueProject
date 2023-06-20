#include "pch.h"
#include "CResMgr.h"

#include "CPathMgr.h"

CResMgr::CResMgr()
	: m_Changed(false)
{
}

CResMgr::~CResMgr()
{
}


void CResMgr::tick()
{
	m_Changed = false;
}

Ptr<CTexture> CResMgr::CreateTexture(const wstring& _strKey, UINT _Width, UINT _Height
	, DXGI_FORMAT _pixelformat, UINT _BindFlag, D3D11_USAGE _Usage)
{
	Ptr<CTexture> pTex =  FindRes<CTexture>(_strKey);

	assert(nullptr == pTex);

	pTex = new CTexture(true);
	if (FAILED(pTex->Create(_Width, _Height, _pixelformat, _BindFlag, _Usage)))
	{
		assert(nullptr);
	}

	AddRes<CTexture>(_strKey, pTex);

	return pTex;
}

Ptr<CTexture> CResMgr::CreateTexture(const wstring& _strKey, ComPtr<ID3D11Texture2D> _Tex2D)
{
	Ptr<CTexture> pTex = FindRes<CTexture>(_strKey);

	assert(nullptr == pTex);

	pTex = new CTexture(true);
	if (FAILED(pTex->Create(_Tex2D)))
	{
		assert(nullptr);
	}

	AddRes<CTexture>(_strKey, pTex);

	return pTex;
}

Ptr<CTexture> CResMgr::LoadTexture(const wstring& _strKey, const wstring& _strRelativePath, int _iMapLevel)
{
	CTexture* pRes = FindRes<CTexture>(_strKey).Get();
	if (nullptr != pRes)
	{
		return pRes;
	}

	pRes = new CTexture;
	pRes->SetKey(_strKey);
	pRes->SetRelativePath(_strRelativePath);

	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _strRelativePath;

	if (FAILED(pRes->Load(strFilePath, _iMapLevel)))
	{
		return nullptr;
	}

	m_arrRes[(UINT)RES_TYPE::TEXTURE].insert(make_pair(_strKey, pRes));
	m_Changed = true;

	return pRes;
}

void CResMgr::DeleteRes(RES_TYPE _type, const wstring& _strKey)
{
	map<wstring, Ptr<CRes>>::iterator iter = m_arrRes[(UINT)_type].find(_strKey);

	assert(!(iter == m_arrRes[(UINT)_type].end()));

	m_arrRes[(UINT)_type].erase(iter);	

	m_Changed = true;
}