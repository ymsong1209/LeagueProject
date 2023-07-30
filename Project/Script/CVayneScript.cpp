#pragma once
#include "pch.h"
#include "CVayneScript.h"
#include <Engine/CKeyMgr.h>
#include <Engine/CAnimator3D.h>
#include <Engine/CCamera.h>
#include <Engine/CRenderMgr.h>


//#include "CVayneIdleState.h"
//#include "CVayneWalkState.h"
//#include "CVayneDeathState.h"
//#include "CVayneRespawnState.h"
//
//#include "VayneQ.h"
//#include "VayneW.h"

CVayneScript::CVayneScript()
	:CChampionScript((UINT)VAYNESCRIPT)
{
	m_fMaxHP = 5;
}

CVayneScript::~CVayneScript()
{
}

void CVayneScript::begin()
{
	//// FSM에 State추가
	//GetOwner()->Fsm()->AddState(L"Idle", new CVayneIdleState);
	//GetOwner()->Fsm()->AddState(L"Walk", new CVayneWalkState);
	//GetOwner()->Fsm()->AddState(L"Death", new CVayneDeathState);
	//GetOwner()->Fsm()->AddState(L"Respawn", new CVayneRespawnState);

	//// Skill에 Vayne Skill 추가
	//m_Skill[1] = new CVayneQ;
	//m_Skill[1]->SetOwnerScript(this);
	//m_Skill[2] = new CVayneW;
	//m_Skill[2]->SetOwnerScript(this);

	// Spell에 소환사 주문 추가

	CChampionScript::begin();
} 

void CVayneScript::tick()
{
	//CChampionScript::tick();

	if (KEY_TAP(KEY::F))
	{
		CGameObject* pObj = nullptr;
		pObj = new CGameObject;
		pObj->SetName(L"VayneTestFromScript");

		pObj->AddComponent(new CTransform);
		pObj->AddComponent(new CMeshRender);

		pObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		pObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
		pObj->Transform()->SetRelativeScale(Vec3(200.f, 200.f, 0.f));
		pObj->Transform()->SetRelativeRot(Vec3(XM_PI / 2.f, 0.f, 0.f));

		pObj->MeshRender()->GetDynamicMaterial(0);
		pObj->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\Fighter.bmp"));

		SpawnGameObject(pObj, Vec3(100.f, 100.f, 100.f), 0);


		CStructuredBuffer* FinalBoneMat =  GetOwner()->Animator3D()->GetFinalBoneMat();
		//CStructuredBuffer* BoneMat = GetOwner()->MeshRender()->GetMesh()->GetBoneFrameDataBuffer();
		int BoneCount = GetOwner()->MeshRender()->GetMesh()->GetBoneCount();
		Vtx* VertexInfo = GetOwner()->MeshRender()->GetMesh()->GetVtxSysMem();		// Vertex의 정보가 모두 들어있음?
		int* IndexInfo_0 = GetOwner()->MeshRender()->GetMesh()->GetIdxSysMem(0);
	


		for (int i = 0; i < 3; ++i)
		{
			int temp = IndexInfo_0[i];

			Vtx Vertex = VertexInfo[IndexInfo_0[i]];

			Skinning(Vertex.vPos, Vertex.vTangent, Vertex.vBinormal, Vertex.vNormal, Vertex.vWeights, Vertex.vIndices, 0, FinalBoneMat, BoneCount);



	/*		Matrix WorldMat = GetOwner()->Transform()->GetWorldMat();
			Matrix ViewMat = CRenderMgr::GetInst()->GetMainCam()->GetViewMat();
			Matrix ProjMat = CRenderMgr::GetInst()->GetMainCam()->GetProjMat();*/


			// 웓드로 정점을 보내버려야함 ( World * View * Projection 적용)
			Matrix WVP =  GetOwner()->Transform()->GetWorldMat() *
			CRenderMgr::GetInst()->GetMainCam()->GetViewMat() *
			CRenderMgr::GetInst()->GetMainCam()->GetProjMat();


			Vec4 FinalPos = XMVector3TransformCoord(Vertex.vPos, WVP);



			CGameObject* MeshObj = nullptr;
			MeshObj = new CGameObject;

			MeshObj->AddComponent(new CTransform);
			MeshObj->AddComponent(new CMeshRender);

			MeshObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh"));

		 

			MeshObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"), 0);
			MeshObj->Transform()->SetRelativeScale(Vec3(10.f, 10.f, 10.f));


			SpawnGameObject(MeshObj, Vec3(FinalPos.x, FinalPos.y, FinalPos.z), 0);
			
		}

	 
	}



	// 징크스의 공격력 / 사거리 / 공격속도는 Q스킬의 추가 공격력을 포함한다.


		//CJinxQ* JinxQ = dynamic_cast<CJinxQ*>(m_Skill[0]);
		//if (JinxQ != nullptr)
		//{
		//	m_fAttackPower += JinxQ->GetSkillDamage();
		//	m_fAttackSpeed += JinxQ->GetSkillAttackSpeed();
		//	m_fAttackRange += JinxQ->GetSkillAttackRange();
		//}

}

void CVayneScript::Skinning(Vec3& _vPos, Vec3& _vTangent, Vec3& _vBinormal, Vec3& _vNormal, Vec4& _vWeight, Vec4& _vIndices, int _iRowIdx, CStructuredBuffer* _arrBoneMat, int _iBoneCount)
{
	tSkinningInfo info;

	if (_iRowIdx == -1)
		return;

	for (int i = 0; i < 4; ++i)
	{
		if (0.f == _vWeight[i])
			continue;

		Matrix matBone = GetBoneMat((int)(_vIndices[i]), _iRowIdx, _arrBoneMat, _iBoneCount);

		Matrix a;
		Matrix b;
		Matrix c = a * b;

		Vec4 Temp;

		//info.vPos += 
		Temp =(XMVector3TransformCoord(_vPos, matBone) * _vWeight[i]);
		info.vPos = Vec3(Temp.x, Temp.y, Temp.z);

		Temp = (XMVector3TransformNormal(_vTangent, matBone) * _vWeight[i]);
		info.vTangent = Vec3(Temp.x, Temp.y, Temp.z);

		Temp = (XMVector3TransformNormal(_vBinormal, matBone) * _vWeight[i]);
		info.vBinormal = Vec3(Temp.x, Temp.y, Temp.z);

		Temp = (XMVector3TransformNormal(_vNormal, matBone) * _vWeight[i]);
		info.vNormal = Vec3(Temp.x, Temp.y, Temp.z);

 
	}

	_vPos = info.vPos;
	_vTangent =  (info.vTangent).Normalize();
	_vBinormal = (info.vBinormal).Normalize();
	_vNormal = (info.vNormal).Normalize();
}

Matrix CVayneScript::GetBoneMat(int _iBoneIdx, int _iRowIdx,  CStructuredBuffer* _arrBoneMat, int _iBoneCount)
{
	UINT bufferSize = _arrBoneMat->GetBufferSize();

 

	int a = bufferSize / sizeof(Matrix);

	Matrix* data = new Matrix[bufferSize / sizeof(Matrix)];

	_arrBoneMat->GetData((void*)data);
 

	Matrix TargetData = data[(_iBoneCount * _iRowIdx) + _iBoneIdx];

	delete[] data;

	return TargetData;
}

////WeightMap 구조화버퍼 저장
//fwrite(&m_iWeightWidth, sizeof(UINT), 1, _File);
//fwrite(&m_iWeightHeight, sizeof(UINT), 1, _File);
//fwrite(&m_iWeightIdx, sizeof(UINT), 1, _File);
//
//UINT bufferSize = m_pWeightMapBuffer->GetBufferSize();
//tWeight_4* data = new tWeight_4[bufferSize / sizeof(tWeight_4)];
//m_pWeightMapBuffer->GetData((void*)data);
//size_t numItems = bufferSize / sizeof(tWeight_4);
//fwrite(&numItems, sizeof(size_t), 1, _File);
//fwrite(data, sizeof(tWeight_4), numItems, _File);
//
//delete[] data;


//matrix GetBoneMat(int _iBoneIdx, int _iRowIdx)
//{
//	return g_arrBoneMat[(g_iBoneCount * _iRowIdx) + _iBoneIdx];
//}


 

 
