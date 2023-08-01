#include "pch.h"
#include "CCollisionMgr.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"
#include "CCollider2D.h"
#include "CCollider3D.h"
#include "CTransform.h"

CCollisionMgr::CCollisionMgr()
    : m_matrix{}
{

}


CCollisionMgr::~CCollisionMgr()
{

}



void CCollisionMgr::SetMatrix(UINT _matrix[MAX_LAYER])
{
	Clear();
	for (UINT i = 0; i < MAX_LAYER; ++i) {
		m_matrix[i] = _matrix[i];
	}
}

void CCollisionMgr::tick()
{
	CLevel* pLevel = CLevelMgr::GetInst()->GetCurLevel();

	for (UINT iRow = 0; iRow < MAX_LAYER; ++iRow)
	{
		for (UINT iCol = iRow; iCol < MAX_LAYER; ++iCol)
		{
			if (!(m_matrix[iRow] & (1 << iCol)))
				continue;

			// iRow ���̾�� iCol ���̾�� ���� �浹�˻縦 �����Ѵ�.
			CollisionBtwLayer(pLevel->GetLayer(iRow), pLevel->GetLayer(iCol));
		}
	}
}

void CCollisionMgr::CollisionBtwLayer(CLayer* _Left, CLayer* _Right)
{
	const vector<CGameObject*>& vecLeft = _Left->GetObjects();
	const vector<CGameObject*>& vecRight = _Right->GetObjects();

	//���� Layer���� �˻��ϴ� ���, �ߺ��˻縦 ��������Ѵ�.
	if (_Left == _Right)
	{
		for (size_t i = 0; i < vecLeft.size(); ++i)
		{
			for (size_t j = i + 1; j < vecRight.size(); ++j)
			{
				CollisionBtw2DObject(vecLeft[i], vecRight[j]);
				CollisionBtw3DObject(vecLeft[i], vecRight[j]);
			}
		}
	}
	//���� �ٸ� Layer���� �˻��ϴ°��
	else
	{
		for (size_t i = 0; i < vecLeft.size(); ++i)
		{
			for (size_t j = 0; j < vecRight.size(); ++j)
			{
				CollisionBtw2DObject(vecLeft[i], vecRight[j]);
				CollisionBtw3DObject(vecLeft[i], vecRight[j]);
			}
		}
	}
}

void CCollisionMgr::CollisionBtw2DObject(CGameObject* _LeftObject, CGameObject* _RightObject)
{
	// �浹ü�� �������� ���� ���
	if (!(_LeftObject->Collider2D() && _RightObject->Collider2D()))
		return;

	// �浹ü ID ����
	CollisionID id = {};

	if (_LeftObject->Collider2D()->GetID() < _RightObject->Collider2D()->GetID()) {
		id.LeftID = _LeftObject->Collider2D()->GetID();
		id.RightID = _RightObject->Collider2D()->GetID();
	}
	else {
		id.LeftID = _RightObject->Collider2D()->GetID();
		id.RightID = _LeftObject->Collider2D()->GetID();
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
	if (CollisionBtw2DCollider(_LeftObject->Collider2D(), _RightObject->Collider2D()))
	{
		// ������ �浹�� ���� �ְ�, ���� �ϳ� �̻��� ���� �����̶��
		if (bDead && iter->second)
		{
			_LeftObject->Collider2D()->EndOverlap(_RightObject->Collider2D());
			_RightObject->Collider2D()->EndOverlap(_LeftObject->Collider2D());
		}
		else if (iter->second)
		{
			// �������� �浹������ �־���, �̹� �����ӿ����� �浹���̴�.
			_LeftObject->Collider2D()->OnOverlap(_RightObject->Collider2D());
			_RightObject->Collider2D()->OnOverlap(_LeftObject->Collider2D());
		}
		else
		{
			// �̹� �����ӿ� ó�� �浹�ߴ�.
			if (!bDead) // ���� �ϳ��� Dead ���¸� �浹�� �����Ѵ�.
			{
				_LeftObject->Collider2D()->BeginOverlap(_RightObject->Collider2D());
				_RightObject->Collider2D()->BeginOverlap(_LeftObject->Collider2D());
				iter->second = true;
			}
		}
	}

	else
	{
		// ���� �����ӿ��� �浹�߾��µ� �̹� �����ӿ��� �浹�� ���ߴ�.
		if (iter->second)
		{
			_LeftObject->Collider2D()->EndOverlap(_RightObject->Collider2D());
			_RightObject->Collider2D()->EndOverlap(_LeftObject->Collider2D());
			iter->second = false;
		}
	}


}



// �� �浹ü�� �浹 �˻� ����
bool CCollisionMgr::CollisionBtw2DCollider(CCollider2D* _pLeft, CCollider2D* _pRight)
{

	//XZ ��鳢�� �˻�
	if (_pLeft->GetOffsetRot() == Vec3(XMConvertToRadians(90.f), 0.f, 0.f) &&
		_pRight->GetOffsetRot() == Vec3(XMConvertToRadians(90.f), 0.f, 0.f)) {
		//�簢��, �簢��
		if (_pLeft->GetCollider2DType() == COLLIDER2D_TYPE::RECT && _pRight->GetCollider2DType() == COLLIDER2D_TYPE::RECT)
		{

			// 0 -- 1
			// |    |
			// 3 -- 2
			// ���� ���ʹ� ���̰� 1�̴�.
			Vec3 arrLocal[4] =
			{
				Vec3(-0.5f, 0.5f, 0.f),
				Vec3(0.5f, 0.5f, 0.f),
				Vec3(0.5f, -0.5f, 0.f),
				Vec3(-0.5f, -0.5f, 0.f),
			};

			// �� �浹ü�� �� ǥ�� ���� 2���� ����
			// ������ 4���� ���Ѵ�.
			Vec3 arrProj[4] = {};

			// ���� �浹ü 0->1�� ���� ����
			arrProj[0] = XMVector3TransformCoord(arrLocal[1], _pLeft->GetColliderWorldMat()) - XMVector3TransformCoord(arrLocal[0], _pLeft->GetColliderWorldMat());
			// ���� �浹ü 0->3
			arrProj[1] = XMVector3TransformCoord(arrLocal[3], _pLeft->GetColliderWorldMat()) - XMVector3TransformCoord(arrLocal[0], _pLeft->GetColliderWorldMat());
			// ������ �浹ü 0->1
			arrProj[2] = XMVector3TransformCoord(arrLocal[1], _pRight->GetColliderWorldMat()) - XMVector3TransformCoord(arrLocal[0], _pRight->GetColliderWorldMat());
			// ������ �浹ü 0->3
			arrProj[3] = XMVector3TransformCoord(arrLocal[3], _pRight->GetColliderWorldMat()) - XMVector3TransformCoord(arrLocal[0], _pRight->GetColliderWorldMat());

			// �� �浹ü�� �߽����� ����
			Vec3 vCenter = XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), _pRight->GetColliderWorldMat()) - XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), _pLeft->GetColliderWorldMat());


			// �и��� �׽�Ʈ
			for (int i = 0; i < 4; ++i)
			{
				Vec3 vProj = arrProj[i];
				vProj.Normalize(); //�������ͷ� �����.


				// 4���� ǥ����͸� ������ ���������� ������Ų �Ÿ��� ���� / 2
				float fProjDist = 0.f;
				for (int j = 0; j < 4; ++j)
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
		else if (_pLeft->GetCollider2DType() == COLLIDER2D_TYPE::CIRCLE && _pRight->GetCollider2DType() == COLLIDER2D_TYPE::CIRCLE) {
			Vec3 RightCenter = XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), _pRight->GetColliderWorldMat());
			Vec3 LeftCenter = XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), _pLeft->GetColliderWorldMat());

			float CenterLength = pow(pow((RightCenter.x - LeftCenter.x), 2) + pow((RightCenter.z - LeftCenter.z), 2), 1.0 / 2.0);

			Vec3 LeftCircleRadius = Vec3(1.f, 0.f, 1.f);
			LeftCircleRadius = XMVector3TransformCoord(LeftCircleRadius, _pLeft->GetColliderScaleMat());
			float LeftRadius = LeftCircleRadius.x / 2.f;

			Vec3 RightCircleRadius = Vec3(1.f, 0.f, 1.f);
			RightCircleRadius = XMVector3TransformCoord(RightCircleRadius, _pRight->GetColliderScaleMat());
			float RightRadius = RightCircleRadius.x / 2.f;

			if (LeftRadius + RightRadius >= CenterLength) return true;

			return false;
		}

		//��, �簢��
		else if ((_pLeft->GetCollider2DType() == COLLIDER2D_TYPE::RECT && _pRight->GetCollider2DType() == COLLIDER2D_TYPE::CIRCLE) ||
			(_pLeft->GetCollider2DType() == COLLIDER2D_TYPE::CIRCLE && _pRight->GetCollider2DType() == COLLIDER2D_TYPE::RECT)) {
			CCollider2D* SquareCollider;
			CCollider2D* CircleCollider;

			if (_pLeft->GetCollider2DType() == COLLIDER2D_TYPE::RECT) {
				SquareCollider = _pLeft;
				CircleCollider = _pRight;
			}
			else {
				SquareCollider = _pRight;
				CircleCollider = _pLeft;
			}

			//0---1
			//|   |
			//3---2

			Vec3 BoxCorner[4] =
			{
				Vec3(-0.5f, 0.5f, 0.f),
				Vec3(0.5f, 0.5f, 0.5f),
				Vec3(0.5f, -0.5f, 0.f),
				Vec3(-0.5f, -0.5f, 0.f),
			};
			Vec3 OuterBoxCorner[4] =
			{
				Vec3(-0.5f, 0.5f, 0.f),
				Vec3(0.5f, 0.5f, 0.f),
				Vec3(0.5f, -0.5f, 0.f),
				Vec3(-0.5f, -0.5f, 0.f)
			};
			Matrix SquareWorldMatRotated = XMMatrixIdentity();
			SquareWorldMatRotated = SquareCollider->GetColliderScaleMat();
			Vec3 CircleRadius = Vec3(1.f, 1.f, 0.f);
			CircleRadius = XMVector3TransformCoord(CircleRadius, CircleCollider->GetColliderScaleMat());
			float Radius = CircleRadius.x / 2.f;

			for (int i = 0; i < 4; ++i) {
				BoxCorner[i] = XMVector3TransformCoord(BoxCorner[i], SquareWorldMatRotated);
				OuterBoxCorner[i] = XMVector3TransformCoord(OuterBoxCorner[i], SquareWorldMatRotated);
			}
			//OuterBoxCorner Ȯ��
			OuterBoxCorner[0].x -= Radius;
			OuterBoxCorner[0].y += Radius;
			OuterBoxCorner[1].x += Radius;
			OuterBoxCorner[1].y += Radius;
			OuterBoxCorner[2].x += Radius;
			OuterBoxCorner[2].y -= Radius;
			OuterBoxCorner[3].x -= Radius;
			OuterBoxCorner[3].y -= Radius;

			Vec3 CircleCenter = Vec3(0.f, 0.f, 0.f);
			Matrix CircleColliderRotated = XMMatrixIdentity();

			Matrix SquareColliderScale = SquareCollider->GetColliderScaleMat();
			Matrix SquareColliderScaleInv = XMMatrixInverse(nullptr, SquareColliderScale);
			Matrix temp = SquareColliderScaleInv * SquareCollider->GetColliderWorldMat();
			Matrix SquareColliderRotated = XMMatrixInverse(nullptr, temp);



			CircleColliderRotated = CircleCollider->GetColliderWorldMat();


			CircleCenter = XMVector3TransformCoord(CircleCenter, CircleColliderRotated);
			CircleCenter = XMVector3TransformCoord(CircleCenter, SquareColliderRotated);


			if (OuterBoxCorner[0].x < CircleCenter.x && CircleCenter.x < OuterBoxCorner[1].x
				&& OuterBoxCorner[3].y < CircleCenter.y && CircleCenter.y < OuterBoxCorner[0].y)
			{
				if (CircleCenter.x < BoxCorner[0].x && BoxCorner[0].y < CircleCenter.y)
				{
					Vec2 BoxVertex2Circle(CircleCenter.x - BoxCorner[0].x, CircleCenter.y - BoxCorner[0].y);
					float Length = BoxVertex2Circle.Length();
					if (Length <= Radius) return true;
					else return false;
				}
				else if (CircleCenter.x > BoxCorner[1].x && BoxCorner[1].y < CircleCenter.y)
				{
					Vec2 BoxVertex2Circle(CircleCenter.x - BoxCorner[1].x, CircleCenter.y - BoxCorner[1].y);
					float Length = BoxVertex2Circle.Length();
					if (Length <= Radius) return true;
					else return false;
				}
				else if (CircleCenter.x > BoxCorner[2].x && BoxCorner[2].y > CircleCenter.y)
				{
					Vec2 BoxVertex2Circle(CircleCenter.x - BoxCorner[2].x, CircleCenter.y - BoxCorner[2].y);
					float Length = BoxVertex2Circle.Length();
					if (Length <= Radius) return true;
					else return false;
				}
				else if (CircleCenter.x < BoxCorner[3].x && BoxCorner[3].y > CircleCenter.y)
				{
					Vec2 BoxVertex2Circle(CircleCenter.x - BoxCorner[3].x, CircleCenter.y - BoxCorner[3].y);
					float Length = BoxVertex2Circle.Length();
					if (Length <= Radius) return true;
					else return false;
				}

				return true;
			}
			else {
				return false;
			}

		}
	}
	//XY ��鳢�� �˻�
	else if (_pLeft->GetOffsetRot() == Vec3(0.f, 0.f, 0.f) &&
		_pRight->GetOffsetRot() == Vec3(0.f, 0.f, 0.f)) {
		//�簢��, �簢��
		if (_pLeft->GetCollider2DType() == COLLIDER2D_TYPE::RECT && _pRight->GetCollider2DType() == COLLIDER2D_TYPE::RECT)
		{

			// 0 -- 1
			// |    |
			// 3 -- 2
			// ���� ���ʹ� ���̰� 1�̴�.
			Vec3 arrLocal[4] =
			{
				Vec3(-0.5f, 0.5f, 0.f),
				Vec3(0.5f, 0.5f, 0.f),
				Vec3(0.5f, -0.5f, 0.f),
				Vec3(-0.5f, -0.5f, 0.f),
			};

			// �� �浹ü�� �� ǥ�� ���� 2���� ����
			// ������ 4���� ���Ѵ�.
			Vec3 arrProj[4] = {};

			// ���� �浹ü 0->1�� ���� ����
			arrProj[0] = XMVector3TransformCoord(arrLocal[1], _pLeft->GetColliderWorldMat()) - XMVector3TransformCoord(arrLocal[0], _pLeft->GetColliderWorldMat());
			// ���� �浹ü 0->3
			arrProj[1] = XMVector3TransformCoord(arrLocal[3], _pLeft->GetColliderWorldMat()) - XMVector3TransformCoord(arrLocal[0], _pLeft->GetColliderWorldMat());
			// ������ �浹ü 0->1
			arrProj[2] = XMVector3TransformCoord(arrLocal[1], _pRight->GetColliderWorldMat()) - XMVector3TransformCoord(arrLocal[0], _pRight->GetColliderWorldMat());
			// ������ �浹ü 0->3
			arrProj[3] = XMVector3TransformCoord(arrLocal[3], _pRight->GetColliderWorldMat()) - XMVector3TransformCoord(arrLocal[0], _pRight->GetColliderWorldMat());

			// �� �浹ü�� �߽����� ����
			Vec3 vCenter = XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), _pRight->GetColliderWorldMat()) - XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), _pLeft->GetColliderWorldMat());


			// �и��� �׽�Ʈ
			for (int i = 0; i < 4; ++i)
			{
				Vec3 vProj = arrProj[i];
				vProj.Normalize(); //�������ͷ� �����.


				// 4���� ǥ����͸� ������ ���������� ������Ų �Ÿ��� ���� / 2
				float fProjDist = 0.f;
				for (int j = 0; j < 4; ++j)
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
		else if (_pLeft->GetCollider2DType() == COLLIDER2D_TYPE::CIRCLE && _pRight->GetCollider2DType() == COLLIDER2D_TYPE::CIRCLE) {
			Vec3 RightCenter = XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), _pRight->GetColliderWorldMat());
			Vec3 LeftCenter = XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), _pLeft->GetColliderWorldMat());

			float CenterLength = pow(pow((RightCenter.x - LeftCenter.x), 2) + pow((RightCenter.y - LeftCenter.y), 2), 1.0 / 2.0);

			Vec3 LeftCircleRadius = Vec3(1.f, 1.f, 0.f);
			LeftCircleRadius = XMVector3TransformCoord(LeftCircleRadius, _pLeft->GetColliderScaleMat());
			float LeftRadius = LeftCircleRadius.x / 2.f;

			Vec3 RightCircleRadius = Vec3(1.f, 1.f, 0.f);
			RightCircleRadius = XMVector3TransformCoord(RightCircleRadius, _pRight->GetColliderScaleMat());
			float RightRadius = RightCircleRadius.x / 2.f;

			if (LeftRadius + RightRadius >= CenterLength) return true;

			return false;
		}

		//��, �簢��
		else if ((_pLeft->GetCollider2DType() == COLLIDER2D_TYPE::RECT && _pRight->GetCollider2DType() == COLLIDER2D_TYPE::CIRCLE) ||
			(_pLeft->GetCollider2DType() == COLLIDER2D_TYPE::CIRCLE && _pRight->GetCollider2DType() == COLLIDER2D_TYPE::RECT)) {
			CCollider2D* SquareCollider;
			CCollider2D* CircleCollider;

			if (_pLeft->GetCollider2DType() == COLLIDER2D_TYPE::RECT) {
				SquareCollider = _pLeft;
				CircleCollider = _pRight;
			}
			else {
				SquareCollider = _pRight;
				CircleCollider = _pLeft;
			}

			//0---1
			//|   |
			//3---2

			Vec3 BoxCorner[4] =
			{
				Vec3(-0.5f, 0.5f, 0.f),
				Vec3(0.5f, 0.5f, 0.f),
				Vec3(0.5f, -0.5f, 0.f),
				Vec3(-0.5f, -0.5f, 0.f),
			};
			Vec3 OuterBoxCorner[4] =
			{
				Vec3(-0.5f, 0.5f, 0.f),
				Vec3(0.5f, 0.5f, 0.f),
				Vec3(0.5f, -0.5f, 0.f),
				Vec3(-0.5f, -0.5f, 0.f)
			};
			Matrix SquareWorldMatRotated = XMMatrixIdentity();
			SquareWorldMatRotated = SquareCollider->GetColliderScaleMat();
			Vec3 CircleRadius = Vec3(1.f, 1.f, 0.f);
			CircleRadius = XMVector3TransformCoord(CircleRadius, CircleCollider->GetColliderScaleMat());
			float Radius = CircleRadius.x / 2.f;

			for (int i = 0; i < 4; ++i) {
				BoxCorner[i] = XMVector3TransformCoord(BoxCorner[i], SquareWorldMatRotated);
				OuterBoxCorner[i] = XMVector3TransformCoord(OuterBoxCorner[i], SquareWorldMatRotated);
			}
			//OuterBoxCorner Ȯ��
			OuterBoxCorner[0].x -= Radius;
			OuterBoxCorner[0].y += Radius;
			OuterBoxCorner[1].x += Radius;
			OuterBoxCorner[1].y += Radius;
			OuterBoxCorner[2].x += Radius;
			OuterBoxCorner[2].y -= Radius;
			OuterBoxCorner[3].x -= Radius;
			OuterBoxCorner[3].y -= Radius;

			Vec3 CircleCenter = Vec3(0.f, 0.f, 0.f);
			Matrix CircleColliderRotated = XMMatrixIdentity();

			Matrix SquareColliderScale = SquareCollider->GetColliderScaleMat();
			Matrix SquareColliderScaleInv = XMMatrixInverse(nullptr, SquareColliderScale);
			Matrix temp = SquareColliderScaleInv * SquareCollider->GetColliderWorldMat();
			Matrix SquareColliderRotated = XMMatrixInverse(nullptr, temp);



			CircleColliderRotated = CircleCollider->GetColliderWorldMat();


			CircleCenter = XMVector3TransformCoord(CircleCenter, CircleColliderRotated);
			CircleCenter = XMVector3TransformCoord(CircleCenter, SquareColliderRotated);


			if (OuterBoxCorner[0].x < CircleCenter.x && CircleCenter.x < OuterBoxCorner[1].x
				&& OuterBoxCorner[3].y < CircleCenter.y && CircleCenter.y < OuterBoxCorner[0].y)
			{
				if (CircleCenter.x < BoxCorner[0].x && BoxCorner[0].y < CircleCenter.y)
				{
					Vec2 BoxVertex2Circle(CircleCenter.x - BoxCorner[0].x, CircleCenter.y - BoxCorner[0].y);
					float Length = BoxVertex2Circle.Length();
					if (Length <= Radius) return true;
					else return false;
				}
				else if (CircleCenter.x > BoxCorner[1].x && BoxCorner[1].y < CircleCenter.y)
				{
					Vec2 BoxVertex2Circle(CircleCenter.x - BoxCorner[1].x, CircleCenter.y - BoxCorner[1].y);
					float Length = BoxVertex2Circle.Length();
					if (Length <= Radius) return true;
					else return false;
				}
				else if (CircleCenter.x > BoxCorner[2].x && BoxCorner[2].y > CircleCenter.y)
				{
					Vec2 BoxVertex2Circle(CircleCenter.x - BoxCorner[2].x, CircleCenter.y - BoxCorner[2].y);
					float Length = BoxVertex2Circle.Length();
					if (Length <= Radius) return true;
					else return false;
				}
				else if (CircleCenter.x < BoxCorner[3].x && BoxCorner[3].y > CircleCenter.y)
				{
					Vec2 BoxVertex2Circle(CircleCenter.x - BoxCorner[3].x, CircleCenter.y - BoxCorner[3].y);
					float Length = BoxVertex2Circle.Length();
					if (Length <= Radius) return true;
					else return false;
				}

				return true;
			}
			else {
				return false;
			}

		}
	}


	

	return false;
}





void CCollisionMgr::LayerCheck(UINT _left, UINT _right)
{
	UINT iRow = (UINT)_left;
	UINT iCol = (UINT)_right;

	if (iRow > iCol)
	{
		UINT iTemp = iCol;
		iCol = iRow;
		iRow = iTemp;
	}

	m_matrix[iRow] |= (1 << iCol);
}

void CCollisionMgr::LayerCheck(const wstring& _strLeftLayer, const wstring& _strRightLayer)
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();

	CLayer* pLeft = pCurLevel->FindLayerByName(_strLeftLayer);
	CLayer* pRight = pCurLevel->FindLayerByName(_strRightLayer);

	LayerCheck(pLeft->GetLayerIndex(), pRight->GetLayerIndex());
}