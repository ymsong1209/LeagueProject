#pragma once
#include "CComputeShader.h"

class CStructuredBuffer;

// �ش� ��ǻƮ ���̴��� CRenderMgr.cpp 
// CalcRayForFor() ���ο��� Execute �ȴ�. 

class CFogFilterShader :
    public CComputeShader
{
private:
	CStructuredBuffer*  m_pCalcedFogInfo;       // ���Ǿ� �� �� (t16)
	Ptr<CTexture>       m_pFogFilterMap;		// FogFilterShader ���

	int					m_iCntObjectsWithSight; // �þ߸� ������ ������Ʈ ����
	int					m_iCntRayPerObject;		// ������Ʈ�� �� ������ ����(�� 1000��)

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



