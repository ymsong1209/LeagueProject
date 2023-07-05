#include "pch.h"
#include "CAnimator2D.h"

#include "CAnim2D.h"
#include "CMeshRender.h"

#include "CTransform.h"
#include "ptr.h"
#include "CMaterial.h"

CAnimator2D::CAnimator2D()
	: CComponent(COMPONENT_TYPE::ANIMATOR2D)
	, m_pCurAnim(nullptr)
	, m_bRepeat(false)
	, m_bPause(false)
{
	m_tOriginalTransform.OriginalPos = Vec3(0.f, 0.f, 0.f);
	m_tOriginalTransform.OriginalScale = Vec3(1.f, 1.f, 1.f);
	m_tOriginalTransform.OriginalRot = Vec3(0.f, 0.f, 0.f);
}

CAnimator2D::CAnimator2D(const CAnimator2D& _Other)
	: CComponent(_Other)
	, m_pCurAnim(_Other.m_pCurAnim)
	, m_bRepeat(_Other.m_bRepeat)
{
	//_other.m_mapAnim�� ��ü���� �ݺ����� ���� ���� �����Ͽ� ������ �ʿ� �־��ش�.
	map<wstring, CAnim2D*>::const_iterator iter = _Other.m_mapAnim.begin();
	for (; iter != _Other.m_mapAnim.end(); iter++)
	{
		//�ִϸ��̼� ������ Ŭ��
		CAnim2D* pAnim = iter->second->Clone();
		//�ִϸ��̼��� ���� �ִϸ����͸� �ڽ����� ����
		pAnim->m_pOwner = this;
		//�ִϸ��̼� ������ ���� ���� ����
		pAnim->Reset();
		//������ �ִϸ��̼� �ʿ� ���� ���� �ִϸ��̼� ����
		m_mapAnim.insert(make_pair(iter->first, pAnim));
	}

	//���簡 ������ ���� �ִϸ����Ϳ� ���� �ִϸ��̼��� �����Ų��
	if (m_pCurAnim != nullptr)
		Play(_Other.m_pCurAnim->GetName(), m_bRepeat);
}

CAnimator2D::~CAnimator2D()
{
	Safe_Del_Map(m_mapAnim);
}

void CAnimator2D::finaltick()
{
	if (nullptr != m_pCurAnim)
	{
		if (m_bRepeat && m_pCurAnim->IsFinish())
		{
			m_pCurAnim->Reset();
		}

		// �ش� �ִϸ��̼��� DynamicTransform �ɼ��� �����ִٸ�
		if (m_pCurAnim->m_bDynamicTransform)
		{
			// �����Ӻ��� ����� DynamicTransform���� ������ ���� Transform �����Ͽ� ����
			const tAnim2DFrm& frm = m_pCurAnim->GetCurFrame();

			Vec3 UpdatePos = m_tOriginalTransform.OriginalPos + frm.DynamicPos;
			Vec3 UpdateScale = m_tOriginalTransform.OriginalScale * frm.DynamicScale;
			Vec3 UpdateRot = m_tOriginalTransform.OriginalRot + frm.DynamicRot;

			// ����� Transform�� �ݿ�
			Transform()->SetRelativePos(UpdatePos);
			Transform()->SetRelativeScale(UpdateScale);
			Transform()->SetRelativeRot(UpdateRot);
		}

		if (!m_bPause)
			m_pCurAnim->finaltick();

	}
}

void CAnimator2D::UpdateData()
{
	if (m_pCurAnim == nullptr)
		return;

	Ptr<CMaterial> pMtrl = MeshRender()->GetMaterial();

	const tAnim2DFrm& frm = m_pCurAnim->GetCurFrame();
	Vec2 vBackSizeUV = m_pCurAnim->GetBackSizeUV();

	int iAnimUse = 1;
	pMtrl->SetScalarParam(INT_0, &iAnimUse);
	pMtrl->SetScalarParam(VEC2_0, &frm.LeftTopUV);
	pMtrl->SetScalarParam(VEC2_1, &frm.SliceUV);
	pMtrl->SetScalarParam(VEC2_2, &frm.OffsetUV);
	pMtrl->SetScalarParam(VEC2_3, &vBackSizeUV);

	pMtrl->SetTexParam(TEX_0, m_pCurAnim->GetAtlasTex());
}

void CAnimator2D::Clear()
{
	Ptr<CMaterial> pMtrl = MeshRender()->GetMaterial();

	int iAnimUse = 0;
	pMtrl->SetScalarParam(INT_0, &iAnimUse);

	Ptr<CTexture> pTex = nullptr;
	pMtrl->SetTexParam(TEX_0, pTex);
}

void CAnimator2D::Play(const wstring& _strName, bool _bRepeat)
{
	CAnim2D* pAnim = FindAnim(_strName);
	assert(pAnim);

	m_pCurAnim = pAnim;
	m_bRepeat = _bRepeat;
}

CAnim2D* CAnimator2D::FindAnim(const wstring& _strName)
{
	map<wstring, CAnim2D*>::iterator iter = m_mapAnim.find(_strName);

	if (iter == m_mapAnim.end())
	{
		return nullptr;
	}

	return iter->second;
}

void CAnimator2D::AddAnim(CAnim2D* _CurAnim)
{
	_CurAnim->m_pOwner = this;
	m_mapAnim.insert(make_pair(_CurAnim->GetName(), _CurAnim));
	SetCurAnim(_CurAnim);
}

void CAnimator2D::CreateAnimation(const wstring& _strAnimName
	, Ptr<CTexture> _AtlasTex, Vec2 _vLeftTop, Vec2 _vSlice, Vec2 _vBackSize
	, int _FrameCount, int _FPS)
{
	CAnim2D* pAnim = new CAnim2D;
	pAnim->Create(_strAnimName, _AtlasTex, _vLeftTop, _vSlice, _vBackSize, _FrameCount, _FPS);

	pAnim->m_pOwner = this;
	m_mapAnim.insert(make_pair(_strAnimName, pAnim));
}

void CAnimator2D::SetOriginalTransform()
{
	m_tOriginalTransform.OriginalPos = Transform()->GetRelativePos();
	m_tOriginalTransform.OriginalScale = Transform()->GetRelativeScale();
	m_tOriginalTransform.OriginalRot = Transform()->GetRelativeRot();
}

void CAnimator2D::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_bRepeat, sizeof(bool), 1, _File);

	size_t AnimCount = m_mapAnim.size();
	fwrite(&AnimCount, sizeof(size_t), 1, _File);

	for (const auto& pair : m_mapAnim)
	{
		pair.second->SaveToLevelFile(_File);
	}

	wstring strCurAnimName;
	if (nullptr != m_pCurAnim)
	{
		strCurAnimName = m_pCurAnim->GetName();
	}
	SaveWString(strCurAnimName, _File);
}

void CAnimator2D::LoadFromLevelFile(FILE* _File)
{
	fread(&m_bRepeat, sizeof(bool), 1, _File);

	size_t AnimCount = 0;
	fread(&AnimCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < AnimCount; ++i)
	{
		CAnim2D* pNewAnim = new CAnim2D;
		pNewAnim->LoadFromLevelFile(_File);

		m_mapAnim.insert(make_pair(pNewAnim->GetName(), pNewAnim));
		pNewAnim->m_pOwner = this;
	}

	wstring strCurAnimName;
	LoadWString(strCurAnimName, _File);

	m_pCurAnim = FindAnim(strCurAnimName);
}
