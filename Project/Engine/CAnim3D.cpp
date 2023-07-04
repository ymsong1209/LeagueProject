#include "pch.h"
#include "CAnim3D.h"

#include "CTimeMgr.h"

CAnim3D::CAnim3D()
	: m_pVecBones(nullptr)
	, m_pVecClip(nullptr)
	, m_iCurClip(0)
	, m_dClipAnimCurTime(0.)
	, m_iFrameCount(30)
	, m_iFrameIdx(0)
	, m_iNextFrameIdx(0)
	, m_fRatio(0.f)
	, m_bFinish(false)
	, f_AnimSpeed(1.f)

{
}

CAnim3D::~CAnim3D()
{
}

void CAnim3D::finaltick()
{
	m_iFrameLength = m_iEndFrame - m_iStartFrame;
	m_dTimeLength = m_dEndTime - m_dStartTime;

	m_dClipAnimCurTime = 0.f;
	// ���� ������� Clip �� �ð��� �����Ѵ�.
	m_vecClipUpdateTime[m_iCurClip] += EditorDT * f_AnimSpeed; //���ǵ� ���� ���� �ӵ� ������ �Բ���

	m_PlayTime = m_vecClipUpdateTime[m_iCurClip];

	m_dClipAnimCurTime = m_dStartTime + m_vecClipUpdateTime[m_iCurClip];

	if (m_dClipAnimCurTime >= m_dEndTime)
	{
		m_bFinish = true;
	}


	//�ʴ� 30��¥��

	// ���� ������ �ε��� ���ϱ�
	double dFrameIdx = m_dClipAnimCurTime * (double)m_iFrameCount;
	m_iFrameIdx = (int)(dFrameIdx);

	// ���� ������ �ε���
	if (m_iFrameIdx >= m_pVecClip->at(0).iFrameLength - 1)
		m_iNextFrameIdx = m_iFrameIdx;	// ���̸� ���� �ε����� ����
	else
		m_iNextFrameIdx = m_iFrameIdx + 1;

	// �����Ӱ��� �ð��� ���� ������ �����ش�.
	m_fRatio = (float)(dFrameIdx - (double)m_iFrameIdx);


	// ��ǻƮ ���̴� ���꿩��
	m_bFinalMatUpdate = false;
}

void CAnim3D::SetAnimClip(const vector<tMTAnimClip>* _vecAnimClip)
{
	m_pVecClip = _vecAnimClip;
	m_vecClipUpdateTime.resize(m_pVecClip->size());
}

void CAnim3D::SaveToLevelFile(FILE* _File)
{
}

void CAnim3D::LoadFromLevelFile(FILE* _File)
{
}


