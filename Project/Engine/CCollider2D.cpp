#include "pch.h"
#include "CCollider2D.h"

#include "CScript.h"
#include "components.h"


CCollider2D::CCollider2D()
	: CComponent(COMPONENT_TYPE::COLLIDER2D)
	, m_Shape(COLLIDER2D_TYPE::RECT)
	, m_bAbsolute(false)
	, m_iCollisionCount(0)
	, m_bIsCollidedFromRay(false)
	, m_bDrawCollision(true)
{
	SetName(L"Collider2D");
}

CCollider2D::CCollider2D(const CCollider2D& _other)
	: CComponent(COMPONENT_TYPE::COLLIDER2D)
	, m_vOffsetPos(_other.m_vOffsetPos)
	, m_vOffsetScale(_other.m_vOffsetScale)
	, m_vOffsetRot(_other.m_vOffsetRot)
	, m_bAbsolute(_other.m_bAbsolute)
	, m_Shape(_other.m_Shape)
	, m_bDrawCollision(_other.m_bDrawCollision)

{

}

CCollider2D::~CCollider2D()
{
}


void CCollider2D::finaltick()
{
	assert(0 <= m_iCollisionCount);

	// 물체의 월드 행렬
	const Matrix& matWorld = Transform()->GetWorldMat();
	Vec3 objWorldPos = Transform()->GetRelativePos();  // 물체의 월드 위치
	Vec3 objWorldScale = Transform()->GetRelativeScale();  // 물체의 월드 스케일

	if (m_bFixed) {
		// 위치만 물체의 위치를 따르게 합니다.
		m_matColliderPos = XMMatrixTranslation(objWorldPos.x + m_vOffsetPos.x, objWorldPos.y + m_vOffsetPos.y, objWorldPos.z + m_vOffsetPos.z);

		// 충돌체의 최종 스케일을 물체의 스케일과 offset 스케일을 곱하여 계산합니다.
		m_matColliderScale = XMMatrixScaling(objWorldScale.x * m_vOffsetScale.x, objWorldScale.y * m_vOffsetScale.y, m_vOffsetScale.z);

		// 충돌체의 최종 행렬
		m_matCollider2D = m_matColliderScale * m_matColliderPos;
	}
	else {
		// 크기 * 회전 * 이동 순서로 행렬을 곱합니다.
		m_matCollider2D = m_matColliderScale * m_matColliderRot * m_matColliderPos * matWorld;
	}

	if (m_bAbsolute) {
		// 부모의 Scale을 없애기 위한 로직
		Matrix matParentScaleInv = XMMatrixInverse(nullptr, Transform()->GetWorldScaleMat());
		m_matCollider2D = m_matCollider2D * matParentScaleInv * matWorld;
	}

	// DebugShape 요청
	Vec4 vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	if (0 < m_iCollisionCount)
		vColor = Vec4(1.f, 0.f, 0.f, 1.f);

	if (m_bDrawCollision) {
		if (COLLIDER2D_TYPE::CIRCLE == m_Shape)
			DrawDebugCircle(m_matCollider2D, vColor, 0.f);
		else
			DrawDebugRect(m_matCollider2D, vColor, 0.f);
	}
}



void CCollider2D::BeginOverlap(CCollider2D* _Other)
{
	m_iCollisionCount += 1;

	// Script 호출
	const vector<CScript*>& vecScript = GetOwner()->GetScripts();
	for (size_t i = 0; i < vecScript.size(); ++i)
	{
		vecScript[i]->BeginOverlap(_Other);
	}
}

void CCollider2D::OnOverlap(CCollider2D* _Other)
{
	// Script 호출
	const vector<CScript*>& vecScript = GetOwner()->GetScripts();
	for (size_t i = 0; i < vecScript.size(); ++i)
	{
		vecScript[i]->OnOverlap(_Other);
	}
}

void CCollider2D::EndOverlap(CCollider2D* _Other)
{
	m_iCollisionCount -= 1;

	// Script 호출
	const vector<CScript*>& vecScript = GetOwner()->GetScripts();
	for (size_t i = 0; i < vecScript.size(); ++i)
	{
		vecScript[i]->EndOverlap(_Other);
	}
}

void CCollider2D::BeginRayOverlap()
{
	int a = 30;
	//GetOwner()->GetRenderComponent()->GetMaterial(0)->SetScalarParam(INT_2, &a);
}

void CCollider2D::OnRayOverlap()
{
	int a = 20;
	//GetOwner()->GetRenderComponent()->GetMaterial(0)->SetScalarParam(INT_2, &a);
}

void CCollider2D::EndRayOverlap()
{
	int b = 0;
	//GetOwner()->GetRenderComponent()->GetMaterial(0)->SetScalarParam(INT_2, &b);
}

void CCollider2D::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_vOffsetPos, sizeof(Vec3), 1, _File);
	fwrite(&m_vOffsetScale, sizeof(Vec3), 1, _File);
	fwrite(&m_vOffsetRot, sizeof(Vec3), 1, _File);
	fwrite(&m_bAbsolute, sizeof(bool), 1, _File);
	fwrite(&m_Shape, sizeof(UINT), 1, _File);
	fwrite(&m_bDrawCollision, sizeof(bool), 1, _File);

}

void CCollider2D::LoadFromLevelFile(FILE* _File)
{
	fread(&m_vOffsetPos, sizeof(Vec3), 1, _File);
	fread(&m_vOffsetScale, sizeof(Vec3), 1, _File);
	fread(&m_vOffsetRot, sizeof(Vec3), 1, _File);
	fread(&m_bAbsolute, sizeof(bool), 1, _File);
	fread(&m_Shape, sizeof(UINT), 1, _File);
	fread(&m_bDrawCollision, sizeof(bool), 1, _File);
}

void CCollider2D::SaveToLevelJsonFile(Value& _objValue, Document::AllocatorType& allocator)
{
	_objValue.AddMember("vOffsetPos", SaveVec3Json(m_vOffsetPos, allocator), allocator);
	_objValue.AddMember("vOffsetScale", SaveVec3Json(m_vOffsetScale, allocator), allocator);
	_objValue.AddMember("vOffsetRot", SaveVec3Json(m_vOffsetRot, allocator), allocator);
	_objValue.AddMember("bAbsolute", m_bAbsolute, allocator);
	_objValue.AddMember("Shape", (UINT)m_Shape, allocator);
	_objValue.AddMember("DrawCollision", m_bDrawCollision, allocator);
}

void CCollider2D::LoadFromLevelJsonFile(const Value& _componentValue)
{
	m_vOffsetPos = LoadVec3Json(_componentValue["vOffsetPos"]);
	m_vOffsetScale = LoadVec3Json(_componentValue["vOffsetScale"]);
	m_vOffsetRot = LoadVec3Json(_componentValue["vOffsetRot"]);
	m_bAbsolute = _componentValue["bAbsolute"].GetBool();
	m_Shape = (COLLIDER2D_TYPE)_componentValue["Shape"].GetUint();
	m_bDrawCollision = _componentValue["DrawCollision"].GetBool();
}
