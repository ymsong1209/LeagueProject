#include "pch.h"
#include "Animator2DUI.h"

#include <Engine\CResMgr.h>
#include <Engine\CRenderMgr.h>
#include <Engine\CMRT.h>
#include <Engine\CGameObject.h>
#include <Engine\CAnim2D.h>
#include <Engine\CAnimator2D.h>
#include <Engine\CDevice.h>
#include <Engine\ptr.h>
#include <Engine\CEventMgr.h>

#include "ImGuiMgr.h"
#include "ListUI.h"


Animator2DUI::Animator2DUI()
	: ComponentUI("##Animator2D", COMPONENT_TYPE::ANIMATOR2D)
	, m_iSelectedAnimIdx(0)
	, m_pCurAnim(nullptr)
	, m_iCol(1)
	, m_iRow(1)
	, m_bRepeat(false)
	, m_bPause(false)
	, m_Changed(false)
	, m_iGridRow(10)
	, m_iGridCol(10)
	, m_bShowOverlay(false)
	, m_fTransparency(0.5f)
{
	SetName("Animator2D");
}

Animator2DUI::~Animator2DUI()
{
	for (size_t i = 0; i < m_vecAnimFrm.size(); i++)
	{
		//wParam : RES_TYPE, lParam : Resource Adress
		tEvent evn = {};
		evn.Type = EVENT_TYPE::DELETE_RESOURCE;
		evn.wParam = (DWORD_PTR)RES_TYPE::TEXTURE;
		evn.lParam = (DWORD_PTR)m_vecAnimFrm[i].Get();
		CEventMgr::GetInst()->AddEvent(evn);
	}
	m_vecAnimFrm.clear();

	delete m_pEditingAnim;
}


void Animator2DUI::init()
{
	m_pCurAnim = GetTarget()->Animator2D()->GetCurAnim();
	if (m_EditDSTex != nullptr)
	{
		m_EditDSTex = nullptr;
	}
	if (m_pCurAnim)
	{
		m_pEditingAnim = m_pCurAnim->Clone();
		CreateFrameImage(m_pEditingAnim);
	}
}

int Animator2DUI::render_update()
{
	if (FALSE == ComponentUI::render_update())
		return FALSE;

	m_mapAnim = GetTarget()->Animator2D()->GetAnimMap();
	m_pCurAnim = GetTarget()->Animator2D()->GetCurAnim();

	if (m_pCurAnim)
	{
		if (m_Changed)
		{
			CreateFrameImage(m_pEditingAnim);
			m_Changed = false;
		}
	}

	PlayAnim();

	// ======= 1. ���� �ִϸ��̼� ����Ʈ========
	char szBuff[50] = {};

	// 1-1. �ִϸ��̼� ���� �ҷ��� �� Ű���� Wstring->string->const char* ������ ��ȯ 
	const int MaxSize = 50;
	static const char* AnimList[MaxSize];
	vector<string> AnimListstr;
	vector<wstring> AnimListWstr;

	map<wstring, CAnim2D*>::iterator iter = m_mapAnim.begin();
	for (; iter != m_mapAnim.end(); iter++)
	{
		AnimListWstr.push_back(iter->first);
		string strAnimName = string(iter->first.begin(), iter->first.end());
		AnimListstr.push_back(strAnimName);
	}

	for (int i = 0; i < AnimListstr.size(); i++)
		AnimList[i] = AnimListstr[i].c_str();

	// 1-2. ���õ� �ִϸ��̼� Key�� ���
	ImGui::Text("Animation List : ");

	ImGui::SetNextItemWidth(200);
	if (ImGui::Combo("##AnimList", &m_iSelectedAnimIdx, AnimList, (int)AnimListstr.size()))
	{
		wstring AnimKey = AnimListWstr[m_iSelectedAnimIdx];
		GetTarget()->Animator2D()->Play(AnimKey, true);

		if (m_pEditingAnim != nullptr)
			delete m_pEditingAnim;

		m_pEditingAnim = GetTarget()->Animator2D()->GetCurAnim()->Clone();
		m_Changed = true;
	}

	ImGui::SameLine();

	// =========== 2. Edit ��ư==============
	static bool show_editor_window = false;

	if (ImGui::Button("Edit"))
	{
		if (m_pCurAnim != nullptr)
			show_editor_window = true;
	}
	if (show_editor_window)
	{
		if (ImGui::Begin("Animation Editor", &show_editor_window, ImGuiWindowFlags_NoBringToFrontOnFocus))
		{
			// 1. �ִϸ��̼� ������ ����
			ImGui::BeginChild("##Animation Frame Image", ImVec2(0, 130.f), true, ImGuiWindowFlags_HorizontalScrollbar);

			GetEditingAnimInfo();

			// 1-2. CurAnim�� ������ ����ŭ �ݺ����� ����, ���̴��� ��ģ �ؽ�ó�� �߶� ����Ѵ�.		
			for (int i = 0; i < m_vecAnimFrm.size(); i++)
			{
				char button_label[32];
				sprintf_s(button_label, "Button %d", i);

				ImVec2 ButtonSize = ImVec2(100, 100);
				ImVec4 transparent = ImVec4(1.0f, 1.0f, 1.0f, 0.0f);

				ImGui::PushStyleColor(ImGuiCol_Button, transparent);
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1, 1, 0, 1));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1, 1, 0, 1));

				ImGui::Text("%d", i);
				ImGui::SameLine();
				if (ImGui::ImageButton(button_label, m_vecAnimFrm[i]->GetSRV().Get(), ButtonSize))
				{
					GetTarget()->Animator2D()->SetPause(true);
					m_pEditingAnim->SetFrameIdx(i);
				}

				ImGui::PopStyleColor(3);

				if (i == 0 || i != m_pCurAnim->GetTotalFrameCount() - 1)
					ImGui::SameLine();
			}
			ImGui::EndChild();

			// 2. �ִϸ��̼� ������ �̸����� �̹���, ����
			ImGui::BeginChild("##FramePreview", ImVec2(ImGui::GetWindowWidth() * 0.32f, 0), true);

			// 2-1. �ִϸ��̼� ������ �̸�����(�ִϸ��̼� ��� + �׵θ�)
			ImVec2 ImageSize = ImVec2(ImGui::GetWindowWidth() * 0.95f, 0);
			float AspectRatio = m_vBackSizePx.x / m_vBackSizePx.y;
			ImageSize.y = ImageSize.x / AspectRatio;
			ImVec2 pos = ImGui::GetCursorScreenPos();
			ImGui::Image(m_vecAnimFrm[m_pEditingAnim->GetCurFrameIdx()]->GetSRV().Get(), ImageSize);

			if (m_bShowOverlay)
			{
				ImVec2 ImagePos = ImGui::GetItemRectMin();
				ImGui::SetCursorScreenPos(ImagePos);
				DrawOverlay(ImageSize, m_fTransparency);
			}


			ImGui::GetWindowDrawList()->AddRect(pos, ImVec2(pos.x + ImageSize.x, pos.y + ImageSize.y), IM_COL32(255, 255, 255, 255), 0.0f, ImDrawCornerFlags_All, 1.0f);

			ImVec2 ImagePos = ImGui::GetItemRectMin();


			// Grid �ɼ�
			if (m_bShowGrid)
			{
				DrawGrid(ImagePos, ImageSize, m_iGridRow, m_iGridCol);
			}


			// 2-2. ������ ���� �� ���� ��ư

			// 2-2-0. ���� �ִϸ��̼� �̸�
			wstring AnimName = m_pEditingAnim->GetName();
			string AnimNamestr = string(AnimName.begin(), AnimName.end());
			ImGui::Text("Animation Name : %s", AnimNamestr.c_str());

			// 2-2-1. ���� ������ ����
			ImGui::Text("Frame          : %d / %d", m_pEditingAnim->GetCurFrameIdx(), m_pEditingAnim->GetTotalFrameCount() - 1);

			// 2-2-2. ����/���� ������ ��ư
			// ���� ������
			if (ImGui::Button("Prev"))
			{
				GetTarget()->Animator2D()->SetPause(true);

				int PrevFrame = m_pEditingAnim->GetCurFrameIdx() - 1;

				if (PrevFrame < 0)
					PrevFrame = m_vecAnimFrm.size() - 1;

				m_pEditingAnim->SetFrameIdx(PrevFrame);
			}
			ImGui::SameLine(50);
			// ���� ������
			if (ImGui::Button("Next"))
			{
				GetTarget()->Animator2D()->SetPause(true);

				int NextFrame = m_pEditingAnim->GetCurFrameIdx() + 1;

				if (NextFrame > m_vecAnimFrm.size() - 1)
					NextFrame = 0;

				m_pEditingAnim->SetFrameIdx(NextFrame);
			}


			// 2-2-3. �������� ���, �׸��� ��� üũ�ڽ�
			ImGui::Checkbox("Show Overlay", &m_bShowOverlay);
			if (m_bShowOverlay)
			{
				ImGui::SliderFloat("Transparency", &m_fTransparency, 0.0f, 1.0f);
			}

			ImGui::Checkbox("Show Grid", &m_bShowGrid);
			if (m_bShowGrid)
			{
				ImGui::DragInt("Row : ", &m_iGridRow, 1.f, 1.f);
				ImGui::DragInt("Col : ", &m_iGridCol, 1.f, 1.f);
			}

			// 2-2-4. Play / Pause ��ư
			if (ImGui::Button("Play/Pause"))
			{
				if (GetTarget()->Animator2D()->IsPaused())
				{
					GetTarget()->Animator2D()->SetPause(false);
				}
				else
				{
					GetTarget()->Animator2D()->SetPause(true);
				}
			}

			if (ImGui::Button("Apply"))
			{
				GetTarget()->Animator2D()->AddAnim(m_pEditingAnim->Clone());

				if (GetTarget()->Animator2D()->FindAnim(L"New Anim") != nullptr)
					GetTarget()->Animator2D()->RemoveAnim(L"New Anim");
			}

			ImGui::EndChild();
			ImGui::SameLine();

			// 3. �ִϸ��̼� ������ ���� ����
			ImGui::BeginChild("##FrameInfo", ImVec2(ImGui::GetWindowWidth() * 0.32f, 0), true);

			ImGui::Text("Current Frame: %d", m_iCurFrmIdx);
			ImGui::SameLine();
			bool IsRepeating = GetTarget()->Animator2D()->IsRepeating();
			if (ImGui::Checkbox("Repeat", &IsRepeating))
			{
				GetTarget()->Animator2D()->SetRepeat(IsRepeating);
			}

			ImGui::Separator();

			EditFrameInfo();

			// ======= 
			if (ImGui::Button("PopBack"))
			{
				if (m_pEditingAnim->GetTotalFrameCount() > 1)
					m_pEditingAnim->PopBackFrame();
				m_Changed = true;
			}
			ImGui::SameLine();
			if (ImGui::Button("Erase Frame"))
			{
				if (GetTarget()->Animator2D()->IsPaused())
				{
					if (!(m_pEditingAnim->GetTotalFrameCount() <= 1))
						m_pEditingAnim->DeleteFrameByIdx(m_iCurFrmIdx);
					m_Changed = true;
				}
			}
			ImGui::SameLine();
			if (ImGui::Button("Save"))
			{
				//m_pEditingAnim->Save();
			}
			ImGui::SameLine();
			if (ImGui::Button("Load"))
			{
				//m_pEditingAnim->Load();
				//m_Changed = true;
			}

			// Dynamic Transfrom Option
			bool bUseDynamicTransform = m_pEditingAnim->IsUsingDynamicTransform();
			if (ImGui::Checkbox("Dynamic Transform", &bUseDynamicTransform))
			{
				m_pCurAnim->SetUseDynamicTransform(bUseDynamicTransform);
				m_pEditingAnim->SetUseDynamicTransform(bUseDynamicTransform);
			}

			if (bUseDynamicTransform)
			{
				ImGui::BeginChild("##OriginalTransform", ImVec2(0.f, 100.f), true);
				ImGui::Text("Original Transform");

				tOriginalTransform tOriPos = GetTarget()->Animator2D()->GetOriginalTransform();

				ImGui::Text("Position  :");
				ImGui::SameLine();
				ImGui::InputFloat3("##OriginalPos", tOriPos.OriginalPos, "%.1f", ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);

				ImGui::Text("Scale     :");
				ImGui::SameLine();
				ImGui::InputFloat3("##OriginalScale", tOriPos.OriginalScale, "%.1f", ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);

				ImGui::Text("Rotation  :");
				ImGui::SameLine();
				ImGui::InputFloat3("##OriginalRot", tOriPos.OriginalRot, "%.1f", ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);

				ImGui::EndChild();
				
				ImGui::BeginChild("##Dynamic Transform", ImVec2(0.f, 100.f), true);
				ImGui::Text("Dynamic Transform");

				ImGui::Text("Position  :");
				ImGui::SameLine();
				Vec3 DynamicPos = m_pEditingAnim->GetCurFrameDynamicPos();
				if (Im Gui::DragFloat3("##DynamicPos", DynamicPos))
				{
					if (GetTarget()->Animator2D()->IsPaused())
					{
						m_pCurAnim->SetCurFrameDynamicPos(DynamicPos);
						m_pEditingAnim->SetCurFrameDynamicPos(DynamicPos);
					}
				}

				ImGui::Text("Scale     :");
				ImGui::SameLine();
				Vec3 DynamicScale = m_pEditingAnim->GetCurFrameDynamicScale();
				if (ImGui::DragFloat3("##DynamicScale", DynamicScale))
				{
					if (GetTarget()->Animator2D()->IsPaused())
					{
						m_pEditingAnim->SetCurFrameDynamicScale(DynamicScale);
						m_pCurAnim->SetCurFrameDynamicScale(DynamicScale);
					}
				}

				ImGui::Text("Rotation   :");
				ImGui::SameLine();
				Vec3 DynamicRot = m_pEditingAnim->GetCurFrameDynamicRot();
				if (ImGui::DragFloat3("##DynamicRot", DynamicRot))
				{
					if (GetTarget()->Animator2D()->IsPaused())
					{
						DynamicRot = (DynamicRot / 180.f) * XM_PI;
						m_pEditingAnim->SetCurFrameDynamicRot(DynamicRot);
						m_pCurAnim->SetCurFrameDynamicRot(DynamicRot);
					}
				}

				ImGui::EndChild();
			}

			ImGui::EndChild();

			ImGui::SameLine();

			// ========= Atlas Info & Setting ===========
			ImGui::BeginChild("##AtlasInfo", ImVec2(ImGui::GetWindowWidth() * 0.32f, 0), true);

			Ptr<CTexture> pAnimAtlas = m_pEditingAnim->GetAtlasTex();
			ImTextureID AtlasID = (void*)(pAnimAtlas->GetSRV().Get());

			ImGui::Text("Atlas Info");
			ImGui::SameLine();
			const char* buttonText = "Open Atlas";
			float buttonWidth = ImGui::CalcTextSize(buttonText).x + ImGui::GetStyle().FramePadding.x * 2.0f;
			ImGui::SetCursorPosX(ImGui::GetWindowWidth() - buttonWidth - 30.f);
			if (ImGui::Button(buttonText))
			{
				ImGui::OpenPopup("Atlas Setting");
			}
			if (ImGui::BeginPopup("Atlas Setting"))
			{
				ImGui::Text("Atlas Resolution: %d", int(m_vAtlasResolution.x));
				ImGui::SameLine();
				ImGui::Text("X %d", int(m_vAtlasResolution.y));

				ImGui::Text("LeftTop :");
				ImGui::SameLine();
				ImGui::Text("%f X %f", m_vRectStartPos.x, m_vRectEndPos.y);

				if (m_iCol < 1)
					m_iCol = 1;
				if (m_iRow < 1)
					m_iRow = 1;

				ImGui::Text("X   :");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(80);
				ImGui::InputInt("##AtlasiCol", &m_iCol);
				ImGui::SameLine();
				ImGui::Text("Y   :");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(80);
				ImGui::InputInt("##AtlasiRow", &m_iRow);
				ImGui::SameLine();
				static float Ratio = 0.5f;
				if (Ratio <= 0.f)
					Ratio = 0.1f;
				else if (Ratio >= 1.f)
					Ratio = 1.f;
				ImGui::Text("   Resolution Ratio   :");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(100);
				ImGui::InputFloat("##Ratio", &Ratio, (float)0.1, (float)0.1, " % .1f");

				Vec2 CurResolution = Vec2(m_vAtlasResolution.x * Ratio, m_vAtlasResolution.y * Ratio);

				// Free Drag Mode (���콺 ������ ��ư���� ������ ���� ����)
				if (m_iCol == 1 && m_iRow == 1)
				{
					ImGui::Image(AtlasID, ImVec2(CurResolution.x, CurResolution.y));

					if (DragRect(1))
					{
						ImVec2 LeftTop = m_vRectStartPos;
						ImVec2 Size = ImVec2(m_vRectEndPos.x - m_vRectStartPos.x, m_vRectEndPos.y - m_vRectStartPos.y);

						if (!(Size.x == 0 || Size.y == 0))
						{
							tAnim2DFrm frm = {};

							frm.LeftTopUV = Vec2(m_vRectStartPos.x / CurResolution.x, m_vRectStartPos.y / CurResolution.y);
							frm.SliceUV = Vec2(Size.x / CurResolution.x, Size.y / CurResolution.y);
							frm.OffsetUV = Vec2(0, 0);
							frm.fDuration = 1.f / m_pEditingAnim->GetFPS();

							// DynamicTransform �ʱⰪ
							frm.DynamicPos = Vec3(0.f, 0.f, 0.f);
							frm.DynamicScale = Vec3(1.f, 1.f, 1.f);
							frm.DynamicRot = Vec3(0.f, 0.f, 0.f);

							if (GetTarget()->Animator2D()->IsPaused())
							{
								m_pEditingAnim->AddFrame(frm);
								m_Changed = true;
							}
						}
					}
				}
				// ��Ʋ�� �̹��� ������ �������� �߶󳻱�
				else
				{
					ImVec2 original_item_spacing = ImGui::GetStyle().ItemSpacing;
					ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

					Vec2 Slice = Vec2(CurResolution / Vec2(m_iCol, m_iRow));
					Vec2 SliceUV = Slice / CurResolution;

					for (int i = 0; i < m_iRow; i++)
					{
						for (int j = 0; j < m_iCol; j++)
						{
							ImVec2 LeftTop = ImVec2(j * Slice.x, i * Slice.y);

							char button_label[32];
							sprintf_s(button_label, "Button %d", i * m_iCol + j);

							ImVec2 LeftTopUV = ImVec2(LeftTop.x / CurResolution.x, LeftTop.y / CurResolution.y);

							if (ImGui::ImageButton(button_label, AtlasID
								, ImVec2(Slice.x, Slice.y)
								, ImVec2(LeftTopUV.x, LeftTopUV.y)
								, ImVec2((LeftTopUV.x + SliceUV.x), (LeftTopUV.y + SliceUV.y))))
							{
								tAnim2DFrm frm = {};

								frm.LeftTopUV = Vec2(LeftTopUV.x, LeftTopUV.y);
								frm.SliceUV = Vec2(SliceUV.x, SliceUV.y);
								frm.OffsetUV = Vec2(0, 0);
								frm.fDuration = 1.f / m_pEditingAnim->GetFPS();

								// DynamicTransform �ʱⰪ
								frm.DynamicPos = Vec3(0.f, 0.f, 0.f);
								frm.DynamicScale = Vec3(1.f, 1.f, 1.f);
								frm.DynamicRot = Vec3(0.f, 0.f, 0.f);

								m_pEditingAnim->AddFrame(frm);
								m_Changed = true;
							}

							if (j != m_iCol - 1)
								ImGui::SameLine();
						}
					}
					ImGui::PopStyleVar();
					ImGui::GetStyle().ItemSpacing = original_item_spacing;
				}

				ImGui::EndPopup();
			}

			ImGui::NewLine();

			wstring WAtlasKey = m_pEditingAnim->GetAtlasTex()->GetKey();
			string AtlasKey = string(WAtlasKey.begin(), WAtlasKey.end());
			ImGui::Text("Key       : %s", AtlasKey.c_str());

			ImGui::Text("Resolution: %d", int(m_vAtlasResolution.x));
			ImGui::SameLine();
			ImGui::Text("X %d", int(m_vAtlasResolution.y));

			ImVec2 AtlasTexSize = ImVec2(ImGui::GetWindowWidth() * 0.95f, 0);
			float AtlasTexAspectRatio = m_vAtlasResolution.x / m_vAtlasResolution.y;
			AtlasTexSize.y = AtlasTexSize.x / AtlasTexAspectRatio;
			//AtlasTexSize = ImVec2(AtlasTexSize.x * 0.95f, AtlasTexSize.y * 0.95f);
			ImVec2 Atlaspos = ImGui::GetCursorScreenPos();
			ImGui::Image(m_pEditingAnim->GetAtlasTex()->GetSRV().Get(), AtlasTexSize);
			ImGui::GetWindowDrawList()->AddRect(Atlaspos, ImVec2(Atlaspos.x + AtlasTexSize.x, Atlaspos.y + AtlasTexSize.y), IM_COL32(255, 255, 255, 255), 0.0f, ImDrawCornerFlags_All, 1.0f);

			ImGui::EndChild();

			ImGui::End();
		}
	}

	// =========3. Create ��ư =============
	if (ImGui::Button("Create"))
	{
		const map<wstring, Ptr<CRes>>& mapTex = CResMgr::GetInst()->GetResources(RES_TYPE::TEXTURE);

		ListUI* pListUI = (ListUI*)ImGuiMgr::GetInst()->FindUI("##List");

		pListUI->Reset("Texture List", ImVec2(300.f, 500.f));
		for (const auto& pair : mapTex)
		{
			//MapMesh ���� Ű������ pListUI�� ��Ʈ�� ���� ���� ����ִ´�.
			pListUI->AddItem(string(pair.first.begin(), pair.first.end()));
		}

		// �׸� ���ý� ȣ����� ��������Ʈ ���
		pListUI->AddDynamic_Select(this, (UI_DELEGATE_1)&Animator2DUI::CreateNewAnim);
	}

	if (ImGui::Button("Load"))
	{
		//CAnim2D* newAnim = new CAnim2D;
		//newAnim->Load();
		//GetTarget()->Animator2D()->AddAnim(newAnim->Clone());
	}

	if(ImGui::Button("Set to Original Transform"))
	{
		GetTarget()->Animator2D()->SetOriginalTransform();
	}

	return TRUE;
}

void Animator2DUI::PlayAnim()
{
	if (nullptr != m_pEditingAnim)
	{
		if (GetTarget()->Animator2D()->IsPaused())
			return;

		// �ݺ���� ���� && ���� �ִϸ��̼� ���Ῡ��
		if (GetTarget()->Animator2D()->IsRepeating() && m_pEditingAnim->IsFinish())
		{
			m_pEditingAnim->Reset();
		}

		m_pEditingAnim->finaltick();
	}
}

void Animator2DUI::GetEditingAnimInfo()
{
	if (m_pEditingAnim)
	{// ���� �ִϸ��̼� ������ ���� �޾ƿ��� (LeftTop, Slice, BackSize, Offset, Duration)
		m_vAtlasResolution = m_pEditingAnim->GetAtlasResolution();

		m_iCurFrmIdx = m_pEditingAnim->GetCurFrameIdx();

		Vec2 vLeftTopUV = m_pEditingAnim->GetCurFrame().LeftTopUV;
		m_vLeftTopPx = vLeftTopUV * m_vAtlasResolution;

		Vec2 vSliceUV = m_pEditingAnim->GetCurFrame().SliceUV;
		m_vSlicePx = vSliceUV * m_vAtlasResolution;

		Vec2 vOffsetUV = m_pEditingAnim->GetCurFrame().OffsetUV;
		m_vOffsetPx = vOffsetUV * m_vAtlasResolution;

		Vec2 vBackSizeUV = m_pEditingAnim->GetBackSizeUV();
		m_vBackSizePx = vBackSizeUV * m_vAtlasResolution;

		float fDuration = m_pEditingAnim->GetCurFrame().fDuration;
		m_iFPS = m_pEditingAnim->GetFPS();
	}
}

void Animator2DUI::EditFrameInfo()
{
	ImGui::Text("LeftTop    :");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(200);
	if (ImGui::DragFloat2("##LeftTop", (float*)&m_vLeftTopPx, 1.f, 0.f))
	{
		m_pEditingAnim->SetCurFrameLeftTopUV(m_vLeftTopPx / m_vAtlasResolution);
		m_Changed = true;
	}

	Vec2 vLeftTopUV = m_vLeftTopPx / m_vAtlasResolution;
	ImGui::Text("LeftTopUV  :");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(200);
	ImGui::InputFloat2("##LeftTopUV", (float*)&vLeftTopUV, "%.3f", ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);


	ImGui::Text("Slice      :");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(200);
	if (ImGui::DragFloat2("##Slice", (float*)&m_vSlicePx, 1.f, 0.f))
	{
		m_pEditingAnim->SetCurFrameSliceUV(m_vSlicePx / m_vAtlasResolution);
		m_Changed = true;
	}

	Vec2 vSliceUV = m_vSlicePx / m_vAtlasResolution;
	ImGui::Text("SliceUV    :");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(200);
	ImGui::InputFloat2("##SliceUV", (float*)&vSliceUV, "%.3f", ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);


	ImGui::Text("Offset     :");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(200);
	if (ImGui::DragFloat2("##Offset", (float*)&m_vOffsetPx, 1.f, 0.f))
	{
		m_pEditingAnim->SetCurFrameOffsetUV(m_vOffsetPx / m_vAtlasResolution);
		m_Changed = true;
	}

	Vec2 vOffsetUV = m_vSlicePx / m_vAtlasResolution;
	ImGui::Text("OffsetUV   :");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(200);
	ImGui::InputFloat2("##OffsetUV", (float*)&vOffsetUV, "%.3f", ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);

	ImGui::Text("BackSize   :");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(200);
	if (ImGui::DragFloat2("##BackSize", (float*)&m_vBackSizePx, 1.f, 0.f))
	{
		m_pEditingAnim->SetBackSizeUV(m_vBackSizePx / m_vAtlasResolution);
		m_Changed = true;
	}

	Vec2 vBackSizeUV = m_vBackSizePx / m_vAtlasResolution;
	ImGui::Text("BackSizeUV :");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(200);
	ImGui::InputFloat2("##BackSizeUV", (float*)&vBackSizeUV, "%.3f", ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);

	ImGui::Text("FPS        :");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(200);
	if (ImGui::DragInt("##vFPS", &m_iFPS, 1.0f, 1))
	{
		m_pEditingAnim->SetFPS(m_iFPS);
		m_pEditingAnim->SetTotalDuration(1.0f / (float)m_iFPS);
	}

	float fDuration = 1.0f / (float)m_iFPS;
	ImGui::Text("Duration   :");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(200);
	ImGui::InputFloat("##Druation", (float*)&fDuration, 0.0f, 0.0f, "%.2f", ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);

}

void Animator2DUI::CreateNewAnim(DWORD_PTR _AtlasKey)
{
	// ���ڷ� ���� Ű������ ���ҽ��Ŵ����� �ʿ��� �ش� Texture�� ã�ƿ´�
	string strKey = (char*)_AtlasKey;
	Ptr<CTexture> pAtlasTex = CResMgr::GetInst()->FindRes<CTexture>(wstring(strKey.begin(), strKey.end()));

	CAnim2D* NewAnim = new CAnim2D;

	// Atlas �� �� ¥���� �ִϸ��̼��� �����.
	NewAnim->Create(L"New Anim", pAtlasTex, Vec2(0, 0), Vec2(1, 1), Vec2(0.1f, 0.1f), 1, 16);

	//Target�� Anim Map�� �ش� �ִϸ��̼��� ����Ѵ�.
	GetTarget()->Animator2D()->AddAnim(NewAnim);

	m_pEditingAnim = NewAnim->Clone();
	m_Changed = true;
}

void Animator2DUI::CreateFrameImage(CAnim2D* _curAnim)
{
	// 0. ������ ������ �̹����� �ִٸ� �����Ѵ�
	m_EditDSTex = CResMgr::GetInst()->FindRes<CTexture>(L"EditDSTex");

	if (m_EditDSTex != nullptr)
	{
		CResMgr::GetInst()->DeleteTexture(m_EditDSTex);
		m_EditDSTex = nullptr;
	}

	for (size_t i = 0; i < m_vecAnimFrm.size(); i++)
	{
		CResMgr::GetInst()->DeleteTexture(m_vecAnimFrm[i]);
	}
	m_vecAnimFrm.clear();


	Ptr<CTexture> pAnimAtlas = _curAnim->GetAtlasTex();
	m_vAtlasResolution = Vec2(pAnimAtlas->Width(), pAnimAtlas->Height());

	Vec2 vBackSizeUV = _curAnim->GetBackSizeUV();
	m_vBackSizePx = vBackSizeUV * m_vAtlasResolution;

	// 1. DSTex ����
	m_EditDSTex = CResMgr::GetInst()->CreateTexture(L"EditDSTex"
		, (UINT)(m_vBackSizePx.x), (UINT)(m_vBackSizePx.y)
		, DXGI_FORMAT_D24_UNORM_S8_UINT, D3D11_BIND_DEPTH_STENCIL, D3D11_USAGE_DEFAULT);


	for (int i = 0; i < _curAnim->GetTotalFrameCount(); i++)
	{
		// 1-1. DSTex ���� �ʱ�ȭ
		CONTEXT->ClearDepthStencilView(m_EditDSTex->GetDSV().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

		// 2. ������ �ؽ�ó�� Ű��(�ִϸ��̼� �̸�+�ε���) �����
		wstring AnimKey = _curAnim->GetName();
		wstring IdxNum = std::to_wstring(i);

		AnimKey = AnimKey + IdxNum;

		// 2. ������ �̹��������� ����� RTTex ���� �� RenderTarget���� ���� (�ػ󵵴� BackSize��ŭ)
		Ptr<CTexture> pAnimFrameTex = CResMgr::GetInst()->CreateTexture(AnimKey
			, (UINT)(m_vBackSizePx.x), (UINT)(m_vBackSizePx.y)
			, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
			, D3D11_USAGE_DEFAULT);

		CONTEXT->OMSetRenderTargets(1, (pAnimFrameTex->GetRTV()).GetAddressOf(), (m_EditDSTex->GetDSV()).Get());
		float arrColor[4] = { 0,0,0,0 };
		CONTEXT->ClearRenderTargetView(pAnimFrameTex->GetRTV().Get(), arrColor);

		// 3. ViewPort �ػ� �缳��
		D3D11_VIEWPORT EditView = {};
		EditView.TopLeftX = 0.f;
		EditView.TopLeftY = 0.f;

		EditView.Width = m_vBackSizePx.x;
		EditView.Height = m_vBackSizePx.y;

		EditView.MinDepth = 0.f;
		EditView.MaxDepth = 1.f;
		CONTEXT->RSSetViewports(1, &EditView);

		// 4. Mtrl�� i��° ������ ����, ���� �ؽ�ó ���ε�
		Ptr<CMaterial> pAnimFrmMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DAnimEditMtrl");

		const tAnim2DFrm& frm = _curAnim->GetFrameByIdx(i);

		int iAnimUse = 1;
		pAnimFrmMtrl->SetScalarParam(INT_0, &iAnimUse);
		pAnimFrmMtrl->SetScalarParam(VEC2_0, &frm.LeftTopUV);
		pAnimFrmMtrl->SetScalarParam(VEC2_1, &frm.SliceUV);
		pAnimFrmMtrl->SetScalarParam(VEC2_2, &frm.OffsetUV);
		pAnimFrmMtrl->SetScalarParam(VEC2_3, &vBackSizeUV);

		pAnimFrmMtrl->SetTexParam(TEX_0, pAnimAtlas);

		pAnimFrmMtrl->UpdateData();

		// 5. RectMesh->render()�� �� �ؽ�ó�� �ش� ������ �߶� ���
		Ptr<CMesh> pRectMesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh");
		pRectMesh->render();

		// 6. ��� �Ϸ�� �ؽ�ó ���Ϳ� ����
		m_vecAnimFrm.push_back(pAnimFrameTex);

		// 7. ����Ʈ �ٽ� �������
		D3D11_VIEWPORT OriginalView = {};
		OriginalView.TopLeftX = 0.f;
		OriginalView.TopLeftY = 0.f;

		Vec2 vWindowResolution = CEngine::GetInst()->GetWindowResolution();
		OriginalView.Width = vWindowResolution.x;
		OriginalView.Height = vWindowResolution.y;

		OriginalView.MinDepth = 0.f;
		OriginalView.MaxDepth = 1.f;
		CONTEXT->RSSetViewports(1, &OriginalView);
	}

	// 8. ������ ���� �Ϸ� �� ����Ÿ�� �ʱ�ȭ
	CRenderMgr::GetInst()->GetMRT(MRT_TYPE::SWAPCHAIN)->OMSet();

}

bool Animator2DUI::DragRect(int _MouseBtn)
{
	ImVec2 AtlasStartPos = ImGui::GetItemRectMin();

	if (ImGui::IsMouseClicked(_MouseBtn))
		m_vRectStartPos = ImVec2(ImGui::GetMousePos().x - AtlasStartPos.x, ImGui::GetMousePos().y - AtlasStartPos.y);

	if (ImGui::IsMouseDown(_MouseBtn))
	{
		m_vRectEndPos = ImVec2(ImGui::GetMousePos().x - AtlasStartPos.x, ImGui::GetMousePos().y - AtlasStartPos.y);

		ImVec2 RectStart = ImVec2(m_vRectStartPos.x + AtlasStartPos.x, m_vRectStartPos.y + AtlasStartPos.y);
		ImVec2 RectEnd = ImVec2(m_vRectEndPos.x + AtlasStartPos.x, m_vRectEndPos.y + AtlasStartPos.y);

		// Draw the rectangle on top of the image
		ImDrawList* draw_list = ImGui::GetForegroundDrawList();
		draw_list->AddRect(RectStart, RectEnd, IM_COL32(255, 0, 0, 255));
	}

	return ImGui::IsMouseReleased(_MouseBtn);
}

void Animator2DUI::DrawGrid(ImVec2 imagePos, ImVec2 imageSize, int _iRow, int _iCol)
{
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	float cellWidth = imageSize.x / _iCol;
	float cellHeight = imageSize.y / _iRow;

	// ���� �׸���
	for (int i = 0; i <= _iRow; ++i) {
		ImVec2 startPos(imagePos.x, imagePos.y + i * cellHeight);
		ImVec2 endPos(imagePos.x + imageSize.x, imagePos.y + i * cellHeight);
		draw_list->AddLine(startPos, endPos, IM_COL32(255, 255, 255, 128));
	}

	// ���� �׸���
	for (int i = 0; i <= _iCol; ++i) {
		ImVec2 startPos(imagePos.x + i * cellWidth, imagePos.y);
		ImVec2 endPos(imagePos.x + i * cellWidth, imagePos.y + imageSize.y);
		draw_list->AddLine(startPos, endPos, IM_COL32(255, 255, 255, 128));
	}
}

void Animator2DUI::DrawOverlay(ImVec2 _imageSize, float _fTransparency)
{
	ImVec4 ImageColor = ImVec4(1.0f, 1.0f, 1.0f, _fTransparency);

	int PrevFrameTexIdx = m_pEditingAnim->GetCurFrameIdx() - 1;
	if (PrevFrameTexIdx < 0)
		PrevFrameTexIdx = m_vecAnimFrm.size() - 1;

	ImTextureID PrevFrameTexId = m_vecAnimFrm[PrevFrameTexIdx]->GetSRV().Get();

	ImGui::Image(PrevFrameTexId, _imageSize, ImVec2(0, 0), ImVec2(1, 1), ImageColor);
}
