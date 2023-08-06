#include "pch.h"
#include "CUpdateBoneScript.h"

#include <Engine/CAnimator3D.h>
#include <Engine/CRenderMgr.h>
#include <Engine/CKeyMgr.h>

CUpdateBoneScript::CUpdateBoneScript()
	: CScript((UINT)SCRIPT_TYPE::UPDATEBONESCRIPT)
	, m_iTrackIdx(0)
{
}

CUpdateBoneScript::~CUpdateBoneScript()
{
}

void CUpdateBoneScript::SetTrackObject(CGameObject* _Obj)
{
	m_pTrackObj = _Obj;

	if (_Obj->MeshRender() != nullptr || _Obj->Animator3D() != nullptr)
	{
		//assert(false, L"CUpdateBoneScript에서 문제 발생!");
	}

	m_pTrackVertexMem = _Obj->MeshRender()->GetMesh()->GetVtxSysMem();

	if (GetOwner()->Transform() != nullptr)
	{
		GetOwner()->Transform()->SetAnimationFollowingVertexObj(true, m_pTrackObj);
	}

	 
}

void CUpdateBoneScript::tick()
{
	if (CKeyMgr::GetInst()->GetKeyState(KEY::Y) == KEY_STATE::PRESSED)
	{
		m_iTrackIdx += 1;
	}


	if (m_pTrackObj != nullptr)
	{
		CStructuredBuffer* ParentBoneMatrix = m_pTrackObj->Animator3D()->GetFinalBoneMat();

		// t30 레지스터에 최종행렬 데이터(구조버퍼) 바인딩		
		ParentBoneMatrix->UpdateData(29, PIPELINE_STAGE::PS_VERTEX);
		Vtx TrackVertex = m_pTrackVertexMem[m_iTrackIdx];

		int BoneCount = m_pTrackObj->MeshRender()->GetMesh()->GetBoneCount();

		
		//m_iTrackIdx = 200;
		
	 
		Vec4 Pos = Vec4(m_pTrackVertexMem[m_iTrackIdx].vPos, 1.f); // 6
		//Vec4 Tangent = Vec4(m_pTrackVertexMem[m_iTrackIdx].vTangent, 1.f); // 7
		//Vec4 Binormal = Vec4(m_pTrackVertexMem[m_iTrackIdx].vBinormal, 1.f); //8
		//Vec4 Normal = Vec4(m_pTrackVertexMem[m_iTrackIdx].vNormal, 1.f); // 9
		Vec4 Weights = Vec4(m_pTrackVertexMem[m_iTrackIdx].vWeights);   // 10
		Vec4 Indices = Vec4(m_pTrackVertexMem[m_iTrackIdx].vIndices);   // 11


		//Vec4 TackVertexPos = Vec4(m_pTrackVertexMem[m_iTrackIdx].vPos, 0.f);
		int TriggerNumber = 100;


		//m_matWorldScale = XMMatrixIdentity(); // 단위행렬 만들기
		//m_matWorldScale = XMMatrixScaling(m_vRelativeScale.x, m_vRelativeScale.y, m_vRelativeScale.z);


		//m_matWorldRot = XMMatrixIdentity();
		//m_matWorldRot = XMMatrixRotationX(m_vRelativeRot.x);
		//m_matWorldRot *= XMMatrixRotationY(m_vRelativeRot.y);
		//m_matWorldRot *= XMMatrixRotationZ(m_vRelativeRot.z);

		//m_matWorldPos = XMMatrixTranslation(m_vRelativePos.x, m_vRelativePos.y, m_vRelativePos.z);

		//m_matWorld = m_matWorldScale * m_matWorldRot * m_matWorldPos;


		//Vec4 TargetPos = Vec4(m_pTrackObj->Transform()->getrelativepos(), 0.f);

	/*	Matrix WorldMat = m_pTrackObj->Transform()->GetWorldMat();
		Matrix ViewMat = CRenderMgr::GetInst()->GetMainCam()->GetViewMat();
		Matrix ProjMat = CRenderMgr::GetInst()->GetMainCam()->GetProjMat();

		Matrix WVP = m_pTrackObj->Transform()->GetWorldMat() *
		CRenderMgr::GetInst()->GetMainCam()->GetViewMat() *
		CRenderMgr::GetInst()->GetMainCam()->GetProjMat();*/

		//WVP = WVP.Transpose();

		Matrix TargetMatrix = XMMatrixIdentity();
		TargetMatrix = m_pTrackObj->Transform()->GetWorldPosMat();

		GetOwner()->Transform()->SetRelativePos(m_pTrackObj->Transform()->GetRelativePos());

		GetOwner()->MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::INT_10, &BoneCount);
		GetOwner()->MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::INT_11, &TriggerNumber);
		GetOwner()->MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC4_6, &Pos);
		//GetOwner()->MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC4_7, &Tangent);
		//GetOwner()->MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC4_8, &Binormal);
		//GetOwner()->MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC4_9, &Normal);
		GetOwner()->MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC4_10,&Weights);
		GetOwner()->MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC4_11,&Indices);
		//GetOwner()->MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC4_9, &TargetPos);
		GetOwner()->MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::MAT_3, &TargetMatrix);
 
	}
}

void CUpdateBoneScript::begin()
{
}

void CUpdateBoneScript::SaveToLevelFile(FILE* _File)
{
}

void CUpdateBoneScript::LoadFromLevelFile(FILE* _FILE)
{
}

void CUpdateBoneScript::SaveToLevelJsonFile(Value& _objValue, Document::AllocatorType& allocator)
{
}

void CUpdateBoneScript::LoadFromLevelJsonFile(const Value& _componentValue)
{
}



