#pragma once
#include "CComputeShader.h"

class CStructuredBuffer;

// 해당 컴퓨트 쉐이더는 CRenderMgr.cpp 
// CalcRayForFor() 내부에서 Execute 된다. 

class CFogFilterShader :
    public CComputeShader
{
private:
	CStructuredBuffer*  m_pCalcedFogInfo;       // 계산되어 온 값 (t16)
	Ptr<CTexture>       m_pFogFilterMap;		// FogFilterShader 결과

	int					m_iCntObjectsWithSight; // 시야를 가지는 오브젝트 개수
	int					m_iCntRayPerObject;		// 오브젝트가 쏠 레이의 개수(약 1000개)

public:
	void SetCalcedFogInfo(CStructuredBuffer* _buffer) { m_pCalcedFogInfo = _buffer; }
	void SetFogFilterMap(Ptr<CTexture> _pTex) { m_pFogFilterMap = _pTex; }
	void SetCountObject(int _count) { m_iCntObjectsWithSight = _count; }
	void SetCountRayPerObj(int _rayCount) { m_iCntRayPerObject = _rayCount; }
public:
	virtual void UpdateData();
	virtual void Clear();

public:
	CFogFilterShader(UINT _iGroupPerThreadX, UINT _iGroupPerThreadY, UINT _iGroupPerThreadZ);
	virtual ~CFogFilterShader();
};



