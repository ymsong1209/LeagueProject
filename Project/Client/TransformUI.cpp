#include "pch.h"
#include "TransformUI.h"

#include <Engine\CGameObject.h>
#include <Engine\CTransform.h>

#include <Engine\CRenderMgr.h>
#include <Engine\CCamera.h>
#include <Engine\CGameObject.h>
#include <ImGuizmo-master/ImGuizmo.h>

// DirectXMath ����� �����մϴ�.
#include <DirectXMath.h>
static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);

TransformUI::TransformUI()
	: ComponentUI("##Transform", COMPONENT_TYPE::TRANSFORM)
{
	SetName("Transform");
}

TransformUI::~TransformUI()
{
}

int TransformUI::render_update()
{
	if (FALSE == ComponentUI::render_update())
		return FALSE;


	//ImGui �������� ����� ������ �ؾ��� ! (Ÿ�ٿ�����Ʈ�� �������)
	if (CRenderMgr::GetInst()->GetGizMoTargetObj())
	{
		CGameObject* TargetObj = CRenderMgr::GetInst()->GetGizMoTargetObj();
		if (TargetObj->Transform())// Ʈ�������� �������ִٸ�
		{
			if (!TargetObj->Transform()->GetNogizmoObj()) //����� ��ġ�Ҽ� �ִ� ������Ʈ���
				RenderGizmo();  //����� ���� ó��
		}
	}

	bool b_IsWindowMode = ImGuiMgr::GetInst()->GetGizmoEditor_WindowMode();
	if (!b_IsWindowMode)
	{
		ImGui::Checkbox("Window Mode", &b_IsWindowMode);
		if (b_IsWindowMode)
			ImGuiMgr::GetInst()->SetGizmoEditor_WindowMode(true);
		else
			ImGuiMgr::GetInst()->SetGizmoEditor_WindowMode(false);
	}

	return TRUE;
}



void TransformUI::RenderGizmo()
{
	//imgui �������� ����� ���� ȣ��Ǿ����
	ImGuizmo::BeginFrame();
	int lastUsing = 0;

	Vec2 Resolution = CEngine::GetInst()->GetWindowResolution();
	ImGuiIO& io = ImGui::GetIO();

	//ImGuizmo::SetRect(0, 0, io.DisplaySize.x + 35.f, io.DisplaySize.y + 80.f);

	CGameObject* GizTarget = CRenderMgr::GetInst()->GetGizMoTargetObj();


	if (GizTarget)
	{
		Vec3 Rot = GizTarget->Transform()->GetRelativeRot();
		Vec3 Pos = GizTarget->Transform()->GetRelativePos();
		Vec3 Scale = GizTarget->Transform()->GetRelativeScale();

		////ImGuizmo::SetRect(100.f, 100.f, 100.f);
		//ImGuizmo::Manipulate(ViewMatArray, ProjMatArray, ImGuizmo::TRANSLATE, ImGuizmo::WORLD, WorldMatArray);
		Matrix WorldMat = GizTarget->Transform()->GetWorldMat();
		Matrix ViewMat = CRenderMgr::GetInst()->GetMainCam()->GetViewMat();
		Matrix ProjMat = CRenderMgr::GetInst()->GetMainCam()->GetProjMat();

		float WorldMatArray[16];
		float ViewMatArray[16];
		float ProjMatArray[16];

		XMStoreFloat4x4(reinterpret_cast<DirectX::XMFLOAT4X4*>(WorldMatArray), WorldMat);
		XMStoreFloat4x4(reinterpret_cast<DirectX::XMFLOAT4X4*>(ViewMatArray), ViewMat);
		XMStoreFloat4x4(reinterpret_cast<DirectX::XMFLOAT4X4*>(ProjMatArray), ProjMat);

		//-----------------------------------------------------


		// create a window and insert the inspector
		ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Appearing);
		ImGui::SetNextWindowSize(ImVec2(320, 340), ImGuiCond_Appearing);

		if (ImGuiMgr::GetInst()->GetGizmoEditor_WindowMode())
			ImGui::Begin("Editor");


		ImGui::Text("X: %f Y: %f", io.MousePos.x, io.MousePos.y);
		if (ImGuizmo::IsUsing())
		{
			ImGui::Text("Using gizmo");
		}
		else
		{
			ImGui::Text(ImGuizmo::IsOver() ? "Over gizmo" : "");
			ImGui::SameLine();
			ImGui::Text(ImGuizmo::IsOver(ImGuizmo::TRANSLATE) ? "Over translate gizmo" : "");
			ImGui::SameLine();
			ImGui::Text(ImGuizmo::IsOver(ImGuizmo::ROTATE) ? "Over rotate gizmo" : "");
			ImGui::SameLine();
			ImGui::Text(ImGuizmo::IsOver(ImGuizmo::SCALE) ? "Over scale gizmo" : "");
		}
		ImGui::Separator();


		for (int matId = 0; matId < 1; matId++)
		{
			ImGuizmo::SetID(matId);

			EditTransform(ViewMatArray, ProjMatArray, WorldMatArray, lastUsing == matId);
			if (ImGuizmo::IsUsing())
			{
				lastUsing = matId;
			}
		}

		bool b_ViewBound = CRenderMgr::GetInst()->GetMainCam()->GetViewGizmoBounding();

		ImGui::Checkbox("View Gizmo Click Bounding", &b_ViewBound);
		if (b_ViewBound)
			CRenderMgr::GetInst()->GetMainCam()->SetViewGizmoBounding(true);
		else
			CRenderMgr::GetInst()->GetMainCam()->SetViewGizmoBounding(false);

		float f_GizBound = GetTarget()->Transform()->GetGizmoBounding();
		ImGui::Text("Gizmo Click Bound Size : ");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(100.f);
		ImGui::DragFloat("##ClickBound", &f_GizBound);
		GetTarget()->Transform()->SetGizmoBounding(f_GizBound);

		if (ImGuiMgr::GetInst()->GetGizmoEditor_WindowMode())
		{
			bool b_IsWindowMode = ImGuiMgr::GetInst()->GetGizmoEditor_WindowMode();
			ImGui::Checkbox("Window Mode", &b_IsWindowMode);
			if (b_IsWindowMode)
				ImGuiMgr::GetInst()->SetGizmoEditor_WindowMode(true);
			else
				ImGuiMgr::GetInst()->SetGizmoEditor_WindowMode(false);

			ImGui::End();
		}

	}

}

void TransformUI::EditTransform(float* cameraView, float* cameraProjection, float* matrix, bool editTransformDecomposition) //3°���ڿ��� �������
{
	static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::LOCAL);
	static bool useSnap = false;
	static float snap[3] = { 1.f, 1.f, 1.f };
	static float bounds[] = { -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f };
	static float boundsSnap[] = { 0.1f, 0.1f, 0.1f };
	static bool boundSizing = false;
	static bool boundSizingSnap = false;


	if (editTransformDecomposition)
	{
		if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
			mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
		ImGui::SameLine();
		if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE))
			mCurrentGizmoOperation = ImGuizmo::ROTATE;
		ImGui::SameLine();
		if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE))
			mCurrentGizmoOperation = ImGuizmo::SCALE;
		if (ImGui::RadioButton("Universal", mCurrentGizmoOperation == ImGuizmo::UNIVERSAL))
			mCurrentGizmoOperation = ImGuizmo::UNIVERSAL;
		float matrixTranslation[3], matrixRotation[3], matrixScale[3];

		ImGuizmo::DecomposeMatrixToComponents(matrix, matrixTranslation, matrixRotation, matrixScale);
		//������ ����϶��� �ߵ��� ��
		ImGui::Text("Position");
		ImGui::SameLine();
		ImGui::DragFloat3("##Tr", matrixTranslation);
		ImGui::Text("Rotation");
		ImGui::SameLine();
		ImGui::DragFloat3("##Rt", matrixRotation);
		ImGui::Text("Scale   ");
		ImGui::SameLine();
		ImGui::DragFloat3("##Sc", matrixScale);
		ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, matrix);

		if (mCurrentGizmoOperation != ImGuizmo::SCALE)
		{
			if (ImGui::RadioButton("Local", mCurrentGizmoMode == ImGuizmo::LOCAL))
				mCurrentGizmoMode = ImGuizmo::LOCAL;
			ImGui::SameLine();
			if (ImGui::RadioButton("World", mCurrentGizmoMode == ImGuizmo::WORLD))
				mCurrentGizmoMode = ImGuizmo::WORLD;
		}

		ImGui::Checkbox("##UseSnap", &useSnap);
		ImGui::SameLine();

		switch (mCurrentGizmoOperation)
		{
		case ImGuizmo::TRANSLATE:
			ImGui::InputFloat3("Snap", &snap[0]);
			break;
		case ImGuizmo::ROTATE:
			ImGui::InputFloat("Angle Snap", &snap[0]);
			break;
		case ImGuizmo::SCALE:
			ImGui::InputFloat("Scale Snap", &snap[0]);
			break;
		}
		ImGui::Checkbox("Bound Sizing", &boundSizing);
		if (boundSizing)
		{
			ImGui::PushID(3);
			ImGui::Checkbox("##BoundSizing", &boundSizingSnap);
			ImGui::SameLine();
			ImGui::InputFloat3("Snap", boundsSnap);
			ImGui::PopID();
		}
	}

	ImGuiIO& io = ImGui::GetIO();
	float viewManipulateRight = io.DisplaySize.x;
	float viewManipulateTop = 0;
	static ImGuiWindowFlags gizmoWindowFlags = 0;

	HWND hWnd = CEngine::GetInst()->GetMainWnd();
	RECT clientRect; // Ŭ���̾�Ʈ ������ �簢��
	GetClientRect(hWnd, &clientRect); // Ŭ���̾�Ʈ ������ �簢���� ����

	int clientX = clientRect.left; // Ŭ���̾�Ʈ ������ �»�� x ��ǥ
	int clientY = clientRect.top; // Ŭ���̾�Ʈ ������ �»�� y ��ǥ

	// ������ ��ǥ��� ��ȯ
	POINT clientTopLeft = { clientX, clientY };
	ClientToScreen(hWnd, &clientTopLeft);

	int windowX = clientTopLeft.x; // �������� �»�� x ��ǥ
	int windowY = clientTopLeft.y; // �������� �»�� y ��ǥ


	ImGuizmo::SetRect(windowX, windowY, io.DisplaySize.x, io.DisplaySize.y);

	//����� ��� ����
	ImGuizmo::Manipulate(cameraView, cameraProjection, mCurrentGizmoOperation, mCurrentGizmoMode, matrix, NULL, useSnap ? &snap[0] : NULL, boundSizing ? bounds : NULL, boundSizingSnap ? boundsSnap : NULL);

	//-------����� ����� ����----------------------
	CGameObject* GizTarget = CRenderMgr::GetInst()->GetGizMoTargetObj();
	Matrix WorldMat = XMLoadFloat4x4(reinterpret_cast<const XMFLOAT4X4*>(matrix));
	float matrixTranslation[3], matrixRotation[3], matrixScale[3];

	//������� ����
	ImGuizmo::DecomposeMatrixToComponents(matrix, matrixTranslation, matrixRotation, matrixScale);

	GizTarget->Transform()->SetRelativeScale(matrixScale[0], matrixScale[1], matrixScale[2]);
	GizTarget->Transform()->SetRelativePos(matrixTranslation[0], matrixTranslation[1], matrixTranslation[2]);
	GizTarget->Transform()->SetRelativeRot(XMConvertToRadians(matrixRotation[0]), XMConvertToRadians(matrixRotation[1]), XMConvertToRadians(matrixRotation[2])); 

	//������� ����
	ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, matrix);

}