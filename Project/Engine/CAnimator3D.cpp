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
	, m_pCurAnim(nullptr) // 현재 재생중인 Animation
	, m_bRepeat(false)  // 반복
	, b_HavePrevAnim(false)
	, b_UseMotionBlend(true)
	, f_BlendTime(0.2f)
	, f_BlendRatio(0.f)
	, f_IsAnimBlendState(false)
	, i_PrevFrameIdx(0)
{
	m_pBoneFinalMatBuffer = new CStructuredBuffer;
}

CAnimator3D::CAnimator3D(const CAnimator3D& _origin) //복사생성자는 안했어요!
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

			if (m_pCurAnim->b_StartEndLerp) //애님 끝과 시작지점 블렌딩을 할경우 매 애님이 끝나면 f_IsAnimBlendState를 true로.
			{
				i_PrevFrameIdx = GetCurAnim3D()->GetFrameIdx();
				f_IsAnimBlendState = 1.f;
				f_BlendRatio = 0.f;
			}
		}

		m_pCurAnim->finaltick();
	}

	if (f_IsAnimBlendState == 1.f) //블렌딩을 해야한다면.
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

		//여기서 현재 애니메이션을 가져와서 그에대한 본정보를 얻어야함

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


		//---애님 블렌딩------
		pUpdateShader->SetIsAnimBlend(f_IsAnimBlendState);
		//애님 블렌드에 대한 값을 전달해줌. f_IsAnimBlendState: 블렌드를 하는가, 

		pUpdateShader->SetAnimBlendRatio(f_BlendRatio);
		//f_BlendRatio 블렌딩 비율 

		pUpdateShader->SetPrevAnimFrameIdx(i_PrevFrameIdx);
		//i_PrevFrameIdx 블렌딩을 할 이전프레임의 인덱스 정보


		// 업데이트 쉐이더 실행
		pUpdateShader->Execute();

		m_pCurAnim->m_bFinalMatUpdate = true;

	}


	// t30 레지스터에 최종행렬 데이터(구조버퍼) 바인딩		
	m_pBoneFinalMatBuffer->UpdateData(30, PIPELINE_STAGE::PS_VERTEX);
}

void CAnimator3D::AnimBlend()
{
	f_BlendRatio += EditorDT / f_BlendTime;

	// m_fRatio가 1을 초과하지 않도록 한다.
	if (f_BlendRatio >= 1.0f) {
		f_BlendRatio = 0.f;
		f_IsAnimBlendState = 0.f; //f_IsAnimBlendState 블렌딩 상태를 안하는것으로 바꿈
	}
	//비율이 1이되었을땐 이제 블렌딩을 안해주면 됨

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

		pMtrl->SetAnim3D(false); // Animation Mesh 알리기
		pMtrl->SetBoneCount(0);
	}
}

void CAnimator3D::Play(const wstring& _strName, bool _bRepeat, bool _ReSetPlay, bool _StartEndLerp)
{
	if (b_HavePrevAnim && b_UseMotionBlend) //이전에 플레이된 애님이 있다면(최초로 실행된 애님이 아니라면)
	{
		f_BlendRatio = 0.f;
		f_IsAnimBlendState = 1.f; // true

		i_PrevFrameIdx = GetCurAnim3D()->GetFrameIdx();
		//블렌딩 해줄 이전 프레임값 세팅 (플레이 해줄때 뿐만아니라 애니메이션이 끝났을때도 다른함수에서 이 변수에 값세팅중)
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
	assert(pClipAnim); //찾으려는 클립애님이 없다면 assert

	int FrameLength = pClipAnim->GetVecClip()->at(0).iFrameLength; // 일단은 0번 클립으로 급하게 했지만 수정이 필요할것 같습니다.

	if (_StartFrame < 0)//StartFrame값이 0이하면 조정
		_StartFrame = 0;

	if (_EndFrame > FrameLength)  //EndFrame값이 최대 프레임 길이보다 크다면 최대 프레임값으로 조정
		_EndFrame = FrameLength;

	CAnim3D* pAnim = new CAnim3D; //애니메이션을 만들고 필요한 값들을 세팅
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
	assert(pClipAnim); //찾으려는 클립애님이 없다면 assert

	double TimeLength = pClipAnim->GetVecClip()->at(0).dTimeLength; // 일단은 0번 클립으로 급하게 했지만 수정이 필요할것 같습니다.

	if (_StartTime < 0) //StartFrame값이 0이하면 조정
		_StartTime = 0;
	if (_EndTime > TimeLength) //EndFrame값이 최대 프레임 길이보다 크다면 최대 프레임값으로 조정
		_EndTime = TimeLength;

	CAnim3D* pAnim = new CAnim3D;  //애니메이션을 만들고 필요한 값들을 세팅
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

