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
	// �浹 ȸ���� ������ ���
	assert(0 <= m_iCollisionCount);

	// ũ��, ȸ��, �̵� ��� ����
	m_matColliderScale = XMMatrixScaling(m_vOffsetScale.x, m_vOffsetScale.y, m_vOffsetScale.z);

	m_matColliderRot = XMMatrixRotationX(m_vOffsetRot.x) * XMMatrixRotationY(m_vOffsetRot.y) * XMMatrixRotationZ(m_vOffsetRot.z);

	m_matColliderPos = XMMatrixTranslation(m_vOffsetPos.x, m_vOffsetPos.y, m_vOffsetPos.z);

	// ũ�� * ȸ�� * �̵�
	m_matCollider2D = m_matColliderScale * m_matColliderRot * m_matColliderPos;

	// �θ� (����) ��� ����
	const Matrix& matWorld = Transform()->GetWorldMat();

	if (m_bFixed)
	{
		// ��ġ�� ��ü�� ��ġ�� ������ �մϴ�.
		Matrix matTranslateToObjPosition = XMMatrixTranslationFromVector(Transform()->GetWorldPos());
		m_matCollider2D = m_matColliderScale * m_matColliderRot * matTranslateToObjPosition;
	}
	else if (m_bAbsolute)
	{
		// �θ��� Scale�� ���ֱ�
		Matrix matParentScaleInv = XMMatrixInverse(nullptr, Transform()->GetWorldScaleMat());
		m_matCollider2D = m_matCollider2D * matParentScaleInv * matWorld;
	}
	else
	{
		m_matCollider2D *= matWorld;
	}

	// DebugShape ��û
	Vec4 vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	if (0 < m_iCollisionCount)
		vColor = Vec4(1.f, 0.f, 0.f, 1.f);

	if (m_bDrawCollision)
	{
		// �������� 0.f�� ������ GameObject�� ������� Collider�� �������.
		if (COLLIDER2D_TYPE::CIRCLE == m_Shape)
			DrawDebugCircle(m_matCollider2D, vColor, 0.f);
		else
			DrawDebugRect(m_matCollider2D, vColor, 0.f);
	}
}


void CCollider2D::BeginOverlap(CCollider2D* _Other)
{
	m_iCollisionCount += 1;

	// Script ȣ��
	const vector<CScript*>& vecScript = GetOwner()->GetScripts();
	for (size_t i = 0; i < vecScript.size(); ++i)
	{
		vecScript[i]->BeginOverlap(_Other);
	}
}

void CCollider2D::OnOverlap(CCollider2D* _Other)
{
	// Script ȣ��
	const vector<CScript*>& vecScript = GetOwner()->GetScripts();
	for (size_t i = 0; i < vecScript.size(); ++i)
	{
		vecScript[i]->OnOverlap(_Other);
	}
}

void CCollider2D::EndOverlap(CCollider2D* _Other)
{
	m_iCollisionCount -= 1;

	// Script ȣ��
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
