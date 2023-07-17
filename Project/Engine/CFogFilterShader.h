#pragma once
#include "CComputeShader.h"

class CStructuredBuffer;

class CFogFilterShader :
    public CComputeShader
{
private:
	CStructuredBuffer*  m_pCalcedFogInfo;      // ���Ǿ� �� �� (t16)
	CStructuredBuffer*  m_pOutputFogFilterMap; // ��ȯ�� �� (u0)

	int                 m_iWidth;   // (g_int_0)
	int                 m_iHeight;  // (g_int_1)

	int					m_iCntObjectsWithSight; // �þ߸� ������ ������Ʈ ����
	int					m_iCntRayPerObject;	 // ������Ʈ�� �� ������ ����(�ִ�360)

public:
	void SetCalcedFogInfo(CStructuredBuffer* _buffer) { m_pCalcedFogInfo = _buffer; }
	void SetFogFilterMap(CStructuredBuffer* _buffer, int _iWidth, int _iHeight) 
	{ 
		m_pOutputFogFilterMap = _buffer; 
		m_iWidth = _iWidth;
		m_iHeight = _iHeight;
	}
	void SetCountObject(int _count) { m_iCntObjectsWithSight = _count; }
	void SetCountRayPerObj(int _rayCount) { m_iCntRayPerObject = _rayCount; }
public:
	virtual void UpdateData();
	virtual void Clear();

public:
	CFogFilterShader(UINT _iGroupPerThreadX, UINT _iGroupPerThreadY, UINT _iGroupPerThreadZ);
	virtual ~CFogFilterShader();
};



