#pragma once
#include "CComputeShader.h"

class CStructuredBuffer;

class CFogOfWarShader :
	public CComputeShader
{
private:
	CStructuredBuffer* m_pRayBuffer;				// ���̸� ��� Object�� ������ �� Buffer�� ������                (t16)
	CStructuredBuffer* m_pWallBuffer;				// �浹 �˻縦 �����ؾ��ϴ� Object�� ������ �� Buffer�� ������   (t17)
	CStructuredBuffer* m_pOutputBuffer;				// OutputBuffer / ReadWrite                                      (u0)

	int				   m_iSourceLightCount;			// Ray�� ������ Source �� ����                                   (g_int_0)
	int				   m_iSourceLightPerRay;		// �Ƹ� 360�� ������ ( Source�ϳ��� ���� ������ Ray�� ����)      (g_int_1)
	int                m_iColliderVecCount;         // Compute Shader�� �������ִ� Collider���� �� ����              (g_int_2)

public:
	void SetSourceLightCount(int _iSourceLightCount) { m_iSourceLightCount = _iSourceLightCount; m_Const.arrInt[0] = _iSourceLightCount; }
	void SetSourceLightPerRay(int _iSourceLightPerRay) { m_iSourceLightPerRay = _iSourceLightPerRay; m_Const.arrInt[1] = _iSourceLightPerRay; }
	void SetColliderVecCount(int _iColliderVecCount) { m_iColliderVecCount = _iColliderVecCount; m_Const.arrInt[2] = _iColliderVecCount; }

	void SetRayBuffer(CStructuredBuffer* _buffer) { m_pRayBuffer = _buffer; }
	void SetWallBuffer(CStructuredBuffer* _buffer) { m_pWallBuffer = _buffer; }
	void SetOutputBuffer(CStructuredBuffer* _buffer) { m_pOutputBuffer = _buffer;  }



public:
	virtual void UpdateData();
	virtual void Clear();

public:
	CFogOfWarShader(UINT _iGroupPerThreadX, UINT _iGroupPerThreadY, UINT _iGroupPerThreadZ);
	virtual ~CFogOfWarShader();
};

 

 