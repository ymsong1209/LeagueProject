#pragma once
#include "CComputeShader.h"

class CStructuredBuffer;

class CFogFilterShader :
    public CComputeShader
{
private:
	CStructuredBuffer*  m_pCalcedFogInfo;       // 계산되어 온 값 (t16)
	Ptr<CTexture>       m_pFogFilterMap;		


	//CStructuredBuffer*  m_pOutputFogFilterMap;  // 반환할 맵 (u0)


	//int                 m_iWidth;   
	//int                 m_iHeight; 

	int					m_iCntObjectsWithSight; // 시야를 가지는 오브젝트 개수
	int					m_iCntRayPerObject;		// 오브젝트가 쏠 레이의 개수(최대360)

public:
	void SetCalcedFogInfo(CStructuredBuffer* _buffer) { m_pCalcedFogInfo = _buffer; }

	void SetFogFilterMap(Ptr<CTexture> _pTex) { m_pFogFilterMap = _pTex; }
	//void SetFogFilterMap(CStructuredBuffer* _buffer, int _iWidth, int _iHeight) 
	//{ 
	//	m_pHeightMap = _buffer;
	//	m_iWidth = _iWidth;
	//	m_iHeight = _iHeight;
	//}

	void SetCountObject(int _count) { m_iCntObjectsWithSight = _count; }
	void SetCountRayPerObj(int _rayCount) { m_iCntRayPerObject = _rayCount; }
public:
	virtual void UpdateData();
	virtual void Clear();

public:
	CFogFilterShader(UINT _iGroupPerThreadX, UINT _iGroupPerThreadY, UINT _iGroupPerThreadZ);
	virtual ~CFogFilterShader();
};



