#pragma once
#include "CComputeShader.h"

class CStructuredBuffer;

class CFogOfWarShader :
    public CComputeShader
{
private:
	CStructuredBuffer* m_vColliderWorldMatrixBuffer;
	CStructuredBuffer* m_vEssentialInfoBuffer;
	CStructuredBuffer* m_pOutputBuffer;				// u0
public:
	virtual void UpdateData();
	virtual void Clear();

public:
	CFogOfWarShader(UINT _iGroupPerThreadX, UINT _iGroupPerThreadY, UINT _iGroupPerThreadZ);
	virtual ~CFogOfWarShader();
};

 

 