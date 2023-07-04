#pragma once
#include "CComputeShader.h"

class CStructuredBuffer;

class CAnimation3DShader :
	public CComputeShader
{
private:
	CStructuredBuffer* m_pFrameDataBuffer;			// t13
	CStructuredBuffer* m_pOffsetMatBuffer;			// t14 
	CStructuredBuffer* m_pOutputBuffer;				// u0

public:
	// g_int_0 : BonCount, g_int_1 : Frame Index
	void SetBoneCount(int _iBoneCount) { m_Const.arrInt[0] = _iBoneCount; }
	void SetFrameIndex(int _iFrameIdx) { m_Const.arrInt[1] = _iFrameIdx; }
	void SetNextFrameIdx(int _iFrameIdx) { m_Const.arrInt[2] = _iFrameIdx; }
	void SetFrameRatio(float _fFrameRatio) { m_Const.arrFloat[0] = _fFrameRatio; }
	void SetFrameDataBuffer(CStructuredBuffer* _buffer) { m_pFrameDataBuffer = _buffer; }
	void SetOffsetMatBuffer(CStructuredBuffer* _buffer) { m_pOffsetMatBuffer = _buffer; }
	void SetOutputBuffer(CStructuredBuffer* _buffer) { m_pOutputBuffer = _buffer; }

	//-----애님 블렌드-----
	void SetIsAnimBlend(float _IsAnimBlend) { m_Const.arrFloat[1] = _IsAnimBlend; }
	//왜 bool값이 아니라 float로 한것이냐면.. int가 이미 4개를 다써서 그냥 float값으로 구별함.
	// 1.f일경우 true, 0.f일경우 false

	void SetAnimBlendRatio(float _fBlendRatio) { m_Const.arrFloat[2] = _fBlendRatio; } //블렌딩 비율
	void SetPrevAnimFrameIdx(int _fPrevAnimFramIdx) { m_Const.arrInt[3] = _fPrevAnimFramIdx; } //블렌딩을 같이할 이전프레임 인덱스
	//------------------



public:
	virtual void UpdateData();
	virtual void Clear();

public:
	CAnimation3DShader(UINT _iGroupPerThreadX, UINT _iGroupPerThreadY, UINT _iGroupPerThreadZ);
	virtual ~CAnimation3DShader();
};

