#include "pch.h"
#include "TransformUI.h"

#include <Engine\CGameObject.h>
#include <Engine\CTransform.h>

#include <Engine\CRenderMgr.h>
#include <Engine\CCamera.h>
#include <Engine\CGameObject.h>
#include <ImGuizmo-master/ImGuizmo.h>
#include <Engine\CKeyMgr.h>

// DirectXMath ����� �����մϴ�.
#include <DirectXMath.h>
static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);

TransformUI::TransformUI()
	: ComponentUI("##Transform", COMPONENT_TYPE::TRANSFORM)
	, b_IsWindowMode(false)
	, m_fDragSpeed(1.f)
	, m_bTestText(false)
	, m_TestTextPos(Vec3(100.f,100.f,0.f))
	, m_fFontSize(16.f)
	, m_wTextInput()
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

	if (b_IsWindowMode)
		SetSize(0.f, 50.f);
	else
		SetSize(0.f, 350.f);

	if (GetTarget()->GetParent()) //����� ������� ���ʿ� �ϼ��� ������ķ� �Ѱ��ִ� �������� �θ��ڽ� �������� ���װ� �ֽ��ϴ�. �ٵ� ���� ������Ʈ�� ���ؼ� �ڽ��� �׳� ���� �������� �����μ̴� Ʈ������ ui�����ֵ��� �߾��(�������x)
	{
		bool bAbsolute = GetTarget()->Transform()->GetAbsolute();
		ImGui::Checkbox("IsAbsolute", &bAbsolute);
		if (bAbsolute)
			GetTarget()->Transform()->SetAbsolute(true);
		else
			GetTarget()->Transform()->SetAbsolute(false);

		Vec3 vPos = GetTarget()->Transform()->GetRelativePos();
		Vec3 vScale = GetTarget()->Transform()->GetRelativeScale();
		Vec3 vRotation = GetTarget()->Transform()->GetRelativeRot();
		vRotation = (vRotation / XM_PI) * 180.f;


		ImGui::Text("Move Speed : ");
		ImGui::SameLine();
		ImGui::DragFloat("##MoveSpeed", &m_fDragSpeed, 0.04f); 


		ImGui::Text("Position");
		ImGui::SameLine();
		ImGui::DragFloat3("##Relative Position", vPos, m_fDragSpeed);

		ImGui::Text("Scale   ");
		ImGui::SameLine();
		ImGui::DragFloat3("##Relative Scale", vScale, m_fDragSpeed);

		ImGui::Text("Rotation");
		ImGui::SameLine();
		ImGui::DragFloat3("##Relative Rotation", vRotation, m_fDragSpeed);

		GetTarget()->Transform()->SetRelativePos(vPos);
		GetTarget()->Transform()->SetRelativeScale(vScale);

		vRotation = (vRotation / 180.f) * XM_PI;
		GetTarget()->Transform()->SetRelativeRot(vRotation);

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
	}

	else
	{
		//ImGui �������� ����� ������ �ؾ��� ! (Ÿ�ٿ�����Ʈ�� �������)
		if (CRenderMgr::GetInst()->GetGizMoTargetObj())
		{
			CGameObject* TargetObj = CRenderMgr::GetInst()->GetGizMoTargetObj();
			if (TargetObj->Transform())// Ʈ�������� �������ִٸ�
			{
				if (!TargetObj->Transform()->GetGizmoObjExcept()) //����� ��ġ�Ҽ� �ִ� ������Ʈ���
					RenderGizmo();  //����� ���� ó��
			}
		}

		b_IsWindowMode = ImGuiMgr::GetInst()->GetGizmoEditor_WindowMode();
		if (!b_IsWindowMode)
		{
			ImGui::Checkbox("Window Mode", &b_IsWindowMode);
			if (b_IsWindowMode)
				ImGuiMgr::GetInst()->SetGizmoEditor_WindowMode(true);
			else
				ImGuiMgr::GetInst()->SetGizmoEditor_WindowMode(false);

			int i_Mode = CRenderMgr::GetInst()->GetMainCam()->GetIsGizmoEditMode();
			//----------------------------------
			if (ImGui::RadioButton("Default Mode", i_Mode == 0))
				CRenderMgr::GetInst()->GetMainCam()->SetIsGizmoEditMode(0);

			ImGui::SameLine();
			if (ImGui::RadioButton("Gizmo Edit Mode", i_Mode == 1))
				CRenderMgr::GetInst()->GetMainCam()->SetIsGizmoEditMode(1);


			ImGui::Text("ChangeMode To Key (F1)");
			ImGui::Text("");

		}

		if (KEY_TAP(KEY::F1))
		{
			if (CRenderMgr::GetInst()->GetMainCam()->GetIsGizmoEditMode() == 0)
				CRenderMgr::GetInst()->GetMainCam()->SetIsGizmoEditMode(1);
			else
				CRenderMgr::GetInst()->GetMainCam()->SetIsGizmoEditMode(0);
		}
	}


	//================��Ʈ ������ ��ġ�غ����� ���ϰ� �������================
	ImGui::Checkbox("Test Text", &m_bTestText);
	if (m_bTestText)
	{
		CCamera* UICam = CRenderMgr::GetInst()->GetCamerafromIdx(1);
		if (UICam)
		{
			Vec3 Pos = m_TestTextPos;
			float Size = m_fFontSize;
			ImGui::DragFloat3("Text Position", Pos, m_fDragSpeed);
			ImGui::DragFloat("##Text Size", &Size, m_fDragSpeed);
			m_TestTextPos = Pos;
			m_fFontSize = Size;

			char szBuff[50] = {};
			ImGui::Text("Font Text :");
			ImGui::SameLine();

			ImGui::InputText("##Level Name", m_wTextInput, 50);

			// UTF-8 to UTF-16 conversion
			int size_needed = MultiByteToWideChar(CP_UTF8, 0, &m_wTextInput[0], (int)strlen(m_wTextInput), NULL, 0);
			std::wstring BurffChar(size_needed, 0);
			MultiByteToWideChar(CP_UTF8, 0, &m_wTextInput[0], (int)strlen(m_wTextInput), &BurffChar[0], size_needed);

			tFont Font3 = {};
			Font3.wInputText = BurffChar;
			Font3.fontType = FONT_TYPE::RIX_KOR_L;
			Font3.fFontSize = m_fFontSize;
			Font3.vDisplayPos = Vec2(Pos.x, Pos.y);
			Font3.iFontColor = FONT_RGBA(252, 252, 250, 255);
			UICam->AddText(FONT_DOMAIN::TRANS, Font3);
		}
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
		ImGui::SetNextWindowSize(ImVec2(370, 360), ImGuiCond_Appearing);

		if (ImGuiMgr::GetInst()->GetGizmoEditor_WindowMode())
			ImGui::Begin("Editor");

		ImGui::Text("X: %f Y: %f", io.MousePos.x, io.MousePos.y);

		//Ray�� ��� Object���� üũ
		bool IsShootingRay = GetTarget()->Transform()->GetIsShootingRay();
		ImGui::Text("Ray Object"); ImGui::SameLine();
		if (ImGui::Checkbox("##RayObjectTransformUI", &IsShootingRay)) {
			GetTarget()->Transform()->SetIsShootingRay(IsShootingRay);
		}

		if (IsShootingRay) {
			ImGui::Text("Ray Range"); ImGui::SameLine();
			float RayRange = GetTarget()->Transform()->GetRayRange();
			ImGui::SetNextItemWidth(100.f);
			if (ImGui::InputFloat("##RayRangeTransformUI", &RayRange)) {
				GetTarget()->Transform()->SetRayRange(RayRange);
			}
		}
		if (ImGuizmo::IsUsing())
			ImGui::Text("Using gizmo");
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

		bool bUseMouseOutLine = GetTarget()->Transform()->GetUseMouseOutline();
		float OutlineThickness = GetTarget()->Transform()->GetOutlineThickness();
		if (bUseMouseOutLine)
		{
			ImGui::Text("Outline Thickness :");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(100.f);
			ImGui::DragFloat("##OutlineThick", &OutlineThickness,0.001f,0.0f,0.2f);
			GetTarget()->Transform()->SetOutlineThickness(OutlineThickness);
		}

		if (ImGuiMgr::GetInst()->GetGizmoEditor_WindowMode())
		{
			b_IsWindowMode = ImGuiMgr::GetInst()->GetGizmoEditor_WindowMode();
			ImGui::Checkbox("Window Mode", &b_IsWindowMode);
			if (b_IsWindowMode)
				ImGuiMgr::GetInst()->SetGizmoEditor_WindowMode(true);
			else
				ImGuiMgr::GetInst()->SetGizmoEditor_WindowMode(false);

			int i_Mode = CRenderMgr::GetInst()->GetMainCam()->GetIsGizmoEditMode();
			//----------------------------------
			if (ImGui::RadioButton("Default Mode", i_Mode == 0))
				CRenderMgr::GetInst()->GetMainCam()->SetIsGizmoEditMode(0);

			ImGui::SameLine();
			if (ImGui::RadioButton("Gizmo Edit Mode", i_Mode == 1))
				CRenderMgr::GetInst()->GetMainCam()->SetIsGizmoEditMode(1);

			ImGui::Text("ChangeMode To Key (F1)");
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

	int i_isEditMode = CRenderMgr::GetInst()->GetMainCam()->GetIsGizmoEditMode();
	//����Ʈ ����϶��� ����� ��� ���� 
	if (i_isEditMode == 1)
	{
		ImGuizmo::Manipulate(cameraView, cameraProjection, mCurrentGizmoOperation, mCurrentGizmoMode, matrix, NULL, useSnap ? &snap[0] : NULL, boundSizing ? bounds : NULL, boundSizingSnap ? boundsSnap : NULL);
	}
	
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