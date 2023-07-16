#include "pch.h"
#include "CTransform.h"

#include "CDevice.h"
#include "CConstBuffer.h"

CTransform::CTransform()
	: CComponent(COMPONENT_TYPE::TRANSFORM)
	, m_vRelativeScale(Vec3(1.f, 1.f, 1.f))
	, m_bAbsolute(false)	
	, m_vRelativeDir{
		  Vec3(1.f, 0.f, 0.f)
		, Vec3(0.f, 1.f, 0.f)
		, Vec3(0.f, 0.f, 1.f)}	
	, m_fGizmoBounding_Radius(150.f)
	, m_bGizmoObjExcept(false)
	, m_bIsShootingRay(false)
	, m_iRayCount(20)
	, m_fRayRange(20)
{
	SetName(L"Transform");
}

CTransform::CTransform(const CTransform& _other)
	: CComponent(COMPONENT_TYPE::TRANSFORM)
	, m_vRelativeScale(_other.m_vRelativeScale)
	, m_vRelativePos(_other.m_vRelativePos)
	, m_vRelativeRot(_other.m_vRelativeRot)
	, m_bAbsolute(_other.m_bAbsolute)
	, m_fGizmoBounding_Radius(_other.m_fGizmoBounding_Radius)
	, m_bGizmoObjExcept(_other.m_bGizmoObjExcept)
	, m_bIsShootingRay(_other.m_bIsShootingRay)
	, m_iRayCount(_other.m_iRayCount)
	, m_fRayRange(_other.m_fRayRange)
{
	SetName(L"Transform");
}

CTransform::~CTransform()
{
}


void CTransform::finaltick()
{
	m_matWorldScale = XMMatrixIdentity(); //단위행렬 만들기
	m_matWorldScale = XMMatrixScaling(m_vRelativeScale.x, m_vRelativeScale.y, m_vRelativeScale.z);


	m_matWorldRot = XMMatrixIdentity();
	m_matWorldRot = XMMatrixRotationX(m_vRelativeRot.x);
	m_matWorldRot *= XMMatrixRotationY(m_vRelativeRot.y);
	m_matWorldRot *= XMMatrixRotationZ(m_vRelativeRot.z);

	m_matWorldPos = XMMatrixTranslation(m_vRelativePos.x, m_vRelativePos.y, m_vRelativePos.z);

	m_matWorld = m_matWorldScale * m_matWorldRot * m_matWorldPos;


	Vec3 vDefaultDir[3] = {
		  Vec3(1.f, 0.f, 0.f)
		, Vec3(0.f, 1.f, 0.f)
		, Vec3(0.f, 0.f, 1.f)
	};

	//방향벡터에 회전행렬을 곱하면 현재 보고 있는 방향이 나온다.
	for (int i = 0; i < 3; ++i)
	{
		m_vRelativeDir[i] = XMVector3TransformNormal(vDefaultDir[i], m_matWorldRot);
	}

	// 부모 오브젝트 확인
	CGameObject* pParent = GetOwner()->GetParent();
	if (pParent)
	{
		if (m_bAbsolute)
		{
			Matrix matParentWorld = pParent->Transform()->m_matWorld;
			Matrix matParentScale = pParent->Transform()->m_matWorldScale;
			Matrix matParentScaleInv = XMMatrixInverse(nullptr, matParentScale); //크기 역함수,nullptr:판별식

			// 월드 = 로컬월드 * 부모크기 역 * 부모 월드(크기/회전/이동)
			m_matWorld = m_matWorld * matParentScaleInv * matParentWorld;
		}
		else
		{
			m_matWorldScale *= pParent->Transform()->m_matWorldScale;
			m_matWorldRot *= pParent->Transform()->m_matWorldRot;
			m_matWorldPos *= pParent->Transform()->m_matWorldPos;
			m_matWorld *= pParent->Transform()->m_matWorld;
		}
	}

	for (int i = 0; i < 3; ++i)
	{
		m_vWorldDir[i] = XMVector3TransformNormal(vDefaultDir[i], m_matWorld); //transformNormal로 m_matWorld의 이동량 삭제
		m_vWorldDir[i].Normalize(); //Scale이 곱해져있으므로 정규화 진행
	}

	m_matWorldInv = XMMatrixInverse(nullptr, m_matWorld);
}

void CTransform::UpdateData()
{
	// 위치값을 상수버퍼에 전달 및 바인딩		
	CConstBuffer* pTransformBuffer = CDevice::GetInst()->GetConstBuffer(CB_TYPE::TRANSFORM);

	g_transform.matWorld = m_matWorld;
	g_transform.matWorldInv = m_matWorldInv;
	g_transform.matWV = g_transform.matWorld * g_transform.matView;
	g_transform.matWVP = g_transform.matWV * g_transform.matProj;


	pTransformBuffer->SetData(&g_transform);
	pTransformBuffer->UpdateData();
}


void CTransform::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_vRelativePos	, sizeof(Vec3), 1, _File);
	fwrite(&m_vRelativeScale, sizeof(Vec3), 1, _File);
	fwrite(&m_vRelativeRot	, sizeof(Vec3), 1, _File);
	fwrite(&m_bAbsolute, sizeof(bool), 1, _File);

	fwrite(&m_bGizmoObjExcept, sizeof(bool), 1, _File);
	fwrite(&m_fGizmoBounding_Radius, sizeof(float), 1, _File);
}

void CTransform::LoadFromLevelFile(FILE* _FILE)
{	
	fread(&m_vRelativePos, sizeof(Vec3), 1, _FILE);
	fread(&m_vRelativeScale, sizeof(Vec3), 1, _FILE);
	fread(&m_vRelativeRot, sizeof(Vec3), 1, _FILE);
	fread(&m_bAbsolute, sizeof(bool), 1, _FILE);
	
	fread(&m_bGizmoObjExcept, sizeof(bool), 1, _FILE);
	fread(&m_fGizmoBounding_Radius, sizeof(float), 1, _FILE);
}

void CTransform::SaveToLevelJsonFile(Value& _objValue, Document::AllocatorType& allocator)
{
	_objValue.AddMember("vRelativePos", SaveVec3Json(m_vRelativePos, allocator), allocator);
	_objValue.AddMember("vRelativeScale", SaveVec3Json(m_vRelativeScale, allocator), allocator);
	_objValue.AddMember("vRelativeRot", SaveVec3Json(m_vRelativeRot, allocator), allocator);
	_objValue.AddMember("bAbsolute", m_bAbsolute, allocator);

	_objValue.AddMember("IsNoGizmoObj", m_bGizmoObjExcept, allocator);
	_objValue.AddMember("GizmoBounding_Radius", m_fGizmoBounding_Radius, allocator);
}

void CTransform::LoadFromLevelJsonFile(const Value& _componentValue)
{
	m_vRelativePos = LoadVec3Json(_componentValue["vRelativePos"]);
	m_vRelativeScale = LoadVec3Json(_componentValue["vRelativeScale"]);
	m_vRelativeRot = LoadVec3Json(_componentValue["vRelativeRot"]);
	m_bAbsolute = _componentValue["bAbsolute"].GetBool();

	m_bGizmoObjExcept = _componentValue["IsNoGizmoObj"].GetBool();
	m_fGizmoBounding_Radius = _componentValue["GizmoBounding_Radius"].GetFloat();
}
