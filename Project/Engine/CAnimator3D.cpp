#include "pch.h"

#include "CAnimator3D.h"
#include "CAnim3D.h"

#include "CTimeMgr.h"
#include "CMeshRender.h"
#include "CStructuredBuffer.h"
#include "CResMgr.h"

#include "CAnimation3DShader.h"

#include "CKeyMgr.h"


CAnimator3D::CAnimator3D()
	: CComponent(COMPONENT_TYPE::ANIMATOR3D)
	, m_pCurAnim(nullptr) // ���� ������� Animation
	, m_bRepeat(false)  // �ݺ�
	, b_HavePrevAnim(false)
	, b_UseMotionBlend(true)
	, f_BlendTime(0.2f)
	, f_BlendRatio(0.f)
	, f_IsAnimBlendState(false)
	, i_PrevFrameIdx(0)
{
	m_pBoneFinalMatBuffer = new CStructuredBuffer;
}

CAnimator3D::CAnimator3D(const CAnimator3D& _origin) //��������ڴ� ���߾��!
	: CComponent(COMPONENT_TYPE::ANIMATOR3D)
{
	m_pBoneFinalMatBuffer = new CStructuredBuffer;
}

CAnimator3D::~CAnimator3D()
{
	if (nullptr != m_pBoneFinalMatBuffer)
		delete m_pBoneFinalMatBuffer;

	Safe_Del_Map(m_mapAnim);
}


void CAnimator3D::finaltick()
{
	if (nullptr != m_pCurAnim)
	{
		if (m_bRepeat && m_pCurAnim->IsFinish())
		{
			m_pCurAnim->Reset();

			if (m_pCurAnim->b_StartEndLerp) //�ִ� ���� �������� ������ �Ұ�� �� �ִ��� ������ f_IsAnimBlendState�� true��.
			{
				i_PrevFrameIdx = GetCurAnim3D()->GetFrameIdx();
				f_IsAnimBlendState = 1.f;
				f_BlendRatio = 0.f;
			}
		}

		m_pCurAnim->finaltick();
	}

	if (f_IsAnimBlendState == 1.f) //������ �ؾ��Ѵٸ�.
	{
		AnimBlend();
	}
}

void CAnimator3D::UpdateData()
{
	if (!m_pCurAnim->m_bFinalMatUpdate)
	{
		// Animation3D Update Compute Shader
		CAnimation3DShader* pUpdateShader = (CAnimation3DShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"Animation3DUpdateCS").Get();

		//���⼭ ���� �ִϸ��̼��� �����ͼ� �׿����� �������� ������

		// Bone Data
		Ptr<CMesh> pMesh = MeshRender()->GetMesh();
		check_mesh(pMesh);

		pUpdateShader->SetFrameDataBuffer(pMesh->GetBoneFrameDataBuffer());
		pUpdateShader->SetOffsetMatBuffer(pMesh->GetBoneOffsetBuffer());
		pUpdateShader->SetOutputBuffer(m_pBoneFinalMatBuffer);

		UINT iBoneCount = (UINT)m_pCurAnim->m_pVecBones->size();
		pUpdateShader->SetBoneCount(iBoneCount);
		pUpdateShader->SetFrameIndex(m_pCurAnim->m_iFrameIdx);
		pUpdateShader->SetNextFrameIdx(m_pCurAnim->m_iNextFrameIdx);
		pUpdateShader->SetFrameRatio(m_pCurAnim->m_fRatio);


		//---�ִ� ����------
		pUpdateShader->SetIsAnimBlend(f_IsAnimBlendState);
		//�ִ� ���忡 ���� ���� ��������. f_IsAnimBlendState: ���带 �ϴ°�, 

		pUpdateShader->SetAnimBlendRatio(f_BlendRatio);
		//f_BlendRatio ���� ���� 

		pUpdateShader->SetPrevAnimFrameIdx(i_PrevFrameIdx);
		//i_PrevFrameIdx ������ �� ������������ �ε��� ����


		// ������Ʈ ���̴� ����
		pUpdateShader->Execute();

		m_pCurAnim->m_bFinalMatUpdate = true;

	}


	// t30 �������Ϳ� ������� ������(��������) ���ε�		
	m_pBoneFinalMatBuffer->UpdateData(30, PIPELINE_STAGE::PS_VERTEX);
}

void CAnimator3D::AnimBlend()
{
	f_BlendRatio += EditorDT / f_BlendTime;

	// m_fRatio�� 1�� �ʰ����� �ʵ��� �Ѵ�.
	if (f_BlendRatio >= 1.0f) {
		f_BlendRatio = 0.f;
		f_IsAnimBlendState = 0.f; //f_IsAnimBlendState ���� ���¸� ���ϴ°����� �ٲ�
	}
	//������ 1�̵Ǿ����� ���� ������ �����ָ� ��

}

void CAnimator3D::ClearData()
{
	m_pBoneFinalMatBuffer->Clear();

	UINT iMtrlCount = MeshRender()->GetMtrlCount();
	Ptr<CMaterial> pMtrl = nullptr;
	for (UINT i = 0; i < iMtrlCount; ++i)
	{
		pMtrl = MeshRender()->GetSharedMaterial(i);
		if (nullptr == pMtrl)
			continue;

		pMtrl->SetAnim3D(false); // Animation Mesh �˸���
		pMtrl->SetBoneCount(0);
	}
}

void CAnimator3D::Play(const wstring& _strName, bool _bRepeat, bool _ReSetPlay, bool _StartEndLerp)
{
	if (b_HavePrevAnim && b_UseMotionBlend) //������ �÷��̵� �ִ��� �ִٸ�(���ʷ� ����� �ִ��� �ƴ϶��)
	{
		f_BlendRatio = 0.f;
		f_IsAnimBlendState = 1.f; // true

		i_PrevFrameIdx = GetCurAnim3D()->GetFrameIdx();
		//���� ���� ���� �����Ӱ� ���� (�÷��� ���ٶ� �Ӹ��ƴ϶� �ִϸ��̼��� ���������� �ٸ��Լ����� �� ������ ��������)
	}

	CAnim3D* pAnim = FindAnim(_strName);
	assert(pAnim);

	m_pCurAnim = pAnim;
	m_bRepeat = _bRepeat;
	b_HavePrevAnim = true;

	if (_StartEndLerp)
	{
		pAnim->b_StartEndLerp = true;
	}

	if (_ReSetPlay == true)
		pAnim->Reset();
}

CAnim3D* CAnimator3D::FindAnim(const wstring& _strName)
{
	map<wstring, CAnim3D*>::iterator iter = m_mapAnim.find(_strName);

	if (iter == m_mapAnim.end())
	{
		return nullptr;
	}

	return iter->second;
}

void CAnimator3D::SetAnimClip(const vector<tMTAnimClip>* _vecAnimClip, wstring _strAnimName, const vector<tMTBone>* _vecBones)
{
	CAnim3D* pAnim = new CAnim3D;
	pAnim->SetAnimClip(_vecAnimClip);
	pAnim->SetName(_strAnimName);

	pAnim->m_pVecBones = _vecBones;
	pAnim->m_vecFinalBoneMat.resize(pAnim->m_pVecBones->size());

	pAnim->m_pOwner = this;
	m_mapAnim.insert(make_pair(_strAnimName, pAnim));
}




void CAnimator3D::check_mesh(Ptr<CMesh> _pMesh)
{
	UINT iBoneCount = _pMesh->GetBoneCount();
	if (m_pBoneFinalMatBuffer->GetElementCount() != iBoneCount)
	{
		m_pBoneFinalMatBuffer->Create(sizeof(Matrix), iBoneCount, SB_TYPE::READ_WRITE, false, nullptr);
	}
}

void CAnimator3D::SaveToLevelFile(FILE* _pFile)
{
}

void CAnimator3D::LoadFromLevelFile(FILE* _pFile)
{
}


void CAnimator3D::CreateClipToAnimF(int _StartFrame, int _EndFrame, wstring _ClipName, wstring _SetAnimName)
{
	CAnim3D* pClipAnim = FindAnim(_ClipName);
	assert(pClipAnim); //ã������ Ŭ���ִ��� ���ٸ� assert

	int FrameLength = pClipAnim->GetVecClip()->at(0).iFrameLength; // �ϴ��� 0�� Ŭ������ ���ϰ� ������ ������ �ʿ��Ұ� �����ϴ�.

	if (_StartFrame < 0)//StartFrame���� 0���ϸ� ����
		_StartFrame = 0;

	if (_EndFrame > FrameLength)  //EndFrame���� �ִ� ������ ���̺��� ũ�ٸ� �ִ� �����Ӱ����� ����
		_EndFrame = FrameLength;

	CAnim3D* pAnim = new CAnim3D; //�ִϸ��̼��� ����� �ʿ��� ������ ����
	pAnim->SetAnimClip(pClipAnim->GetVecClip());
	pAnim->SetName(_SetAnimName);

	pAnim->m_pVecBones = pClipAnim->GetBones();
	pAnim->m_vecFinalBoneMat.resize(pAnim->m_pVecBones->size());

	pAnim->m_pOwner = this;

	pAnim->SetStartFrame(_StartFrame);
	pAnim->SetEndFrame(_EndFrame);
	m_mapAnim.insert(make_pair(_SetAnimName, pAnim));
}


void CAnimator3D::CreateClipToAnimT(double _StartTime, double _EndTime, wstring _ClipName, wstring _SetAnimName)
{
	CAnim3D* pClipAnim = FindAnim(_ClipName);
	assert(pClipAnim); //ã������ Ŭ���ִ��� ���ٸ� assert

	double TimeLength = pClipAnim->GetVecClip()->at(0).dTimeLength; // �ϴ��� 0�� Ŭ������ ���ϰ� ������ ������ �ʿ��Ұ� �����ϴ�.

	if (_StartTime < 0) //StartFrame���� 0���ϸ� ����
		_StartTime = 0;
	if (_EndTime > TimeLength) //EndFrame���� �ִ� ������ ���̺��� ũ�ٸ� �ִ� �����Ӱ����� ����
		_EndTime = TimeLength;

	CAnim3D* pAnim = new CAnim3D;  //�ִϸ��̼��� ����� �ʿ��� ������ ����
	pAnim->SetAnimClip(pClipAnim->GetVecClip());
	pAnim->SetName(_SetAnimName);
	pAnim->SetClipName(_ClipName);

	pAnim->m_pVecBones = pClipAnim->GetBones();
	pAnim->m_vecFinalBoneMat.resize(pAnim->m_pVecBones->size());
	pAnim->m_pOwner = this;

	pAnim->SetStartTime(_StartTime);
	pAnim->SetEndTime(_EndTime);
	m_mapAnim.insert(make_pair(_SetAnimName, pAnim));
}

