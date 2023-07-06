#include "pch.h"
#include "CCollisionMgr.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"
#include "CCollider3D.h"
#include "CTransform.h"


void CCollisionMgr::CollisionBtw3DObject(CGameObject* _LeftObject, CGameObject* _RightObject)
{
	// �浹ü�� �������� ���� ���
	if (!(_LeftObject->Collider3D() && _RightObject->Collider3D()))
		return;

	// �浹ü ID ����
	CollisionID id = {};
	if (_LeftObject->Collider3D()->GetID() < _RightObject->Collider3D()->GetID()) {
		id.LeftID = _LeftObject->Collider3D()->GetID();
		id.RightID = _RightObject->Collider3D()->GetID();
	}
	else {
		id.LeftID = _RightObject->Collider3D()->GetID();
		id.RightID = _LeftObject->Collider3D()->GetID();
	}

	// ID �˻�
	map<UINT_PTR, bool>::iterator iter = m_mapColID.find(id.id);
	if (iter == m_mapColID.end())
	{
		m_mapColID.insert(make_pair(id.id, false));
		iter = m_mapColID.find(id.id);
	}

	// �� �� �ϳ��� ���� ���� ���¶��(Dead ����)
	bool bDead = false;
	if (_LeftObject->IsDead() || _RightObject->IsDead())
	{
		bDead = true;
	}

	// �� �浹ü�� ���� �浹 ������ Ȯ��
	if (CollisionBtw3DCollider(_LeftObject->Collider3D(), _RightObject->Collider3D()))
	{
		// ������ �浹�� ���� �ְ�, ���� �ϳ� �̻��� ���� �����̶��
		if (bDead && iter->second)
		{
			_LeftObject->Collider3D()->EndOverlap(_RightObject->Collider3D());
			_RightObject->Collider3D()->EndOverlap(_LeftObject->Collider3D());
		}
		else if (iter->second)
		{
			// �������� �浹������ �־���, �̹� �����ӿ����� �浹���̴�.
			_LeftObject->Collider3D()->OnOverlap(_RightObject->Collider3D());
			_RightObject->Collider3D()->OnOverlap(_LeftObject->Collider3D());
		}
		else
		{
			// �̹� �����ӿ� ó�� �浹�ߴ�.
			if (!bDead) // ���� �ϳ��� Dead ���¸� �浹�� �����Ѵ�.
			{
				_LeftObject->Collider3D()->BeginOverlap(_RightObject->Collider3D());
				_RightObject->Collider3D()->BeginOverlap(_LeftObject->Collider3D());
				iter->second = true;
			}
		}
	}

	else
	{
		// ���� �����ӿ��� �浹�߾��µ� �̹� �����ӿ��� �浹�� ���ߴ�.
		if (iter->second)
		{
			_LeftObject->Collider3D()->EndOverlap(_RightObject->Collider3D());
			_RightObject->Collider3D()->EndOverlap(_LeftObject->Collider3D());
			iter->second = false;
		}
	}
}

bool CCollisionMgr::CollisionBtw3DCollider(CCollider3D* _pLeft, CCollider3D* _pRight)
{
	if (_pLeft->GetCollider3DType() == COLLIDER3D_TYPE::CUBE && _pRight->GetCollider3DType() == COLLIDER3D_TYPE::CUBE)
	{
		// �簢���� ������(local space������ ��ǥ)�� ��Ÿ���� �迭
		Vec3 arrLocal[8] =
		{
			Vec3(-0.5f, 0.5f, 0.5f),
			Vec3(0.5f, 0.5f, 0.5f),
			Vec3(0.5f, -0.5f, 0.5f),
			Vec3(-0.5f, -0.5f, 0.5f),
			Vec3(-0.5f, 0.5f, -0.5f),
			Vec3(0.5f, 0.5f, -0.5f),
			Vec3(0.5f, -0.5f, -0.5f),
			Vec3(-0.5f, -0.5f, -0.5f)
		};

		// �� �浹ü�� �� ǥ�� ���� 3���� ����
		// ������ 6���� ���Ѵ�.
		Vec3 arrProj[6] = {};

		// ���� �浹ü 4->0�� ���� ����
		arrProj[0] = XMVector3TransformCoord(arrLocal[0], _pLeft->GetColliderWorldMat()) - XMVector3TransformCoord(arrLocal[4], _pLeft->GetColliderWorldMat());
		// ���� �浹ü 4->5
		arrProj[1] = XMVector3TransformCoord(arrLocal[5], _pLeft->GetColliderWorldMat()) - XMVector3TransformCoord(arrLocal[4], _pLeft->GetColliderWorldMat());
		// ���� �浹ü 4->7
		arrProj[2] = XMVector3TransformCoord(arrLocal[7], _pLeft->GetColliderWorldMat()) - XMVector3TransformCoord(arrLocal[4], _pLeft->GetColliderWorldMat());
		// ������ �浹ü 4->0�� ���� ����
		arrProj[3] = XMVector3TransformCoord(arrLocal[0], _pRight->GetColliderWorldMat()) - XMVector3TransformCoord(arrLocal[4], _pRight->GetColliderWorldMat());
		// ������ �浹ü 4->5
		arrProj[4] = XMVector3TransformCoord(arrLocal[5], _pRight->GetColliderWorldMat()) - XMVector3TransformCoord(arrLocal[4], _pRight->GetColliderWorldMat());
		// ������ �浹ü 4->7
		arrProj[5] = XMVector3TransformCoord(arrLocal[7], _pRight->GetColliderWorldMat()) - XMVector3TransformCoord(arrLocal[4], _pRight->GetColliderWorldMat());

		// �� �浹ü�� �߽����� ����
		Vec3 vCenter = XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), _pRight->GetColliderWorldMat()) - XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), _pLeft->GetColliderWorldMat());

		// �и��� �׽�Ʈ
		for (int i = 0; i < 6; ++i)
		{
			Vec3 vProj = arrProj[i];
			vProj.Normalize(); //�������ͷ� �����.


			// 6���� ǥ����͸� ������ ���������� ������Ų �Ÿ��� ���� / 2
			float fProjDist = 0.f;
			for (int j = 0; j < 6; ++j)
			{
				//Dot : ����
				//Cos�׷����� -1~1 �����̹Ƿ� ���밪 ���ؾ���.
				fProjDist += fabsf(arrProj[j].Dot(vProj));
			}
			fProjDist /= 2.f;

			float fCenter = fabsf(vCenter.Dot(vProj));

			if (fProjDist < fCenter)
				return false;
		}

		return true;
	}

	//��, ��
	else if (_pLeft->GetCollider3DType() == COLLIDER3D_TYPE::SPHERE && _pRight->GetCollider3DType() == COLLIDER3D_TYPE::SPHERE) {
		Vec3 RightCenter = XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), _pRight->GetColliderWorldMat());
		Vec3 LeftCenter = XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), _pLeft->GetColliderWorldMat());

		float CenterLength = pow(pow((RightCenter.x - LeftCenter.x), 2) + pow((RightCenter.y - LeftCenter.y), 2) + pow((RightCenter.z - LeftCenter.z), 2), 1.0 / 2.0);

		Vec3 LeftCircleRadius = Vec3(1.f, 1.f, 1.f);
		LeftCircleRadius = XMVector3TransformCoord(LeftCircleRadius, _pLeft->GetColliderScaleMat());
		float LeftRadius = LeftCircleRadius.x / 2.f;

		Vec3 RightCircleRadius = Vec3(1.f, 1.f, 1.f);
		RightCircleRadius = XMVector3TransformCoord(RightCircleRadius, _pRight->GetColliderScaleMat());
		float RightRadius = RightCircleRadius.x / 2.f;

		if (LeftRadius + RightRadius >= CenterLength) return true;
	}

	//��, ť�� 
	else if ((_pLeft->GetCollider3DType() == COLLIDER3D_TYPE::SPHERE && _pRight->GetCollider3DType() == COLLIDER3D_TYPE::CUBE) ||
		(_pLeft->GetCollider3DType() == COLLIDER3D_TYPE::CUBE && _pRight->GetCollider3DType() == COLLIDER3D_TYPE::SPHERE)) {
		CCollider3D* CubeCollider;
		CCollider3D* SphereCollider;

		if (_pLeft->GetCollider3DType() == COLLIDER3D_TYPE::CUBE) {
			CubeCollider = _pLeft;
			SphereCollider = _pRight;
		}
		else {
			CubeCollider = _pRight;
			SphereCollider = _pLeft;
		}


		// �簢���� ������(local space������ ��ǥ)�� ��Ÿ���� �迭
		Vec3 arrLocal[8] =
		{
			Vec3(-0.5f, 0.5f, 0.5f),
			Vec3(0.5f, 0.5f, 0.5f),
			Vec3(0.5f, -0.5f, 0.5f),
			Vec3(-0.5f, -0.5f, 0.5f),
			Vec3(-0.5f, 0.5f, -0.5f),
			Vec3(0.5f, 0.5f, -0.5f),
			Vec3(0.5f, -0.5f, -0.5f),
			Vec3(-0.5f, -0.5f, -0.5f)
		};
		Vec3 sphereCenter = XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), SphereCollider->GetColliderWorldMat());
		Vec3 sphereRadius = Vec3(1.f, 1.f, 1.f);
		sphereRadius = XMVector3TransformCoord(sphereRadius, SphereCollider->GetColliderScaleMat());
		float radius = sphereRadius.x / 2.f;

		// Cube�� AABB ���ϱ�
		Vec3 cubeMin = XMVector3TransformCoord(arrLocal[7], CubeCollider->GetColliderWorldMat()); // �⺻������ (0.5, 0.5, 0.5)�� �ּҷ� ����
		Vec3 cubeMax = cubeMin;

		for (int i = 0; i < 8; ++i) {
			Vec3 cubeCorner = XMVector3TransformCoord(arrLocal[i], CubeCollider->GetColliderWorldMat());
			cubeMin = Vec3::Min(cubeMin, cubeCorner);
			cubeMax = Vec3::Max(cubeMax, cubeCorner);
		}

		// Sphere�� �߽��� AABB ���� �ִ��� Ȯ��
		if (sphereCenter.x >= cubeMin.x && sphereCenter.y >= cubeMin.y && sphereCenter.z >= cubeMin.z &&
			sphereCenter.x <= cubeMax.x && sphereCenter.y <= cubeMax.y && sphereCenter.z <= cubeMax.z) {
			return true;
		}

		// �׷��� �ʴٸ�, AABB�� ���� ����� ���������� �Ÿ��� Sphere�� ������ ��
		float x = max(cubeMin.x - sphereCenter.x, max(0.f, sphereCenter.x - cubeMax.x));
		float y = max(cubeMin.y - sphereCenter.y, max(0.f, sphereCenter.y - cubeMax.y));
		float z = max(cubeMin.z - sphereCenter.z, max(0.f, sphereCenter.z - cubeMax.z));

		return (x * x + y * y + z * z) <= (radius * radius);
	}


	return false;
}