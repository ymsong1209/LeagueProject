#include "pch.h"
#include "Animator3DUI.h"
#include <Engine\CGameObject.h>
#include <Engine/CAnimator3D.h>
#include <Engine\CAnim3D.h>
#include "TreeUI.h"
#include <Engine/CResMgr.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include "ListUI.h"
#include "ContentUI.h"
#include <Engine/CPathMgr.h>

Animator3DUI::Animator3DUI()
	: ComponentUI("##Animator3D", COMPONENT_TYPE::ANIMATOR3D)
{
	SetName("Animator3D");
}

Animator3DUI::~Animator3DUI()
{
}

int Animator3DUI::render_update()
{
	if (FALSE == ComponentUI::render_update())
		return FALSE;
	LoadAnim3D();
	ImGui::SameLine();
	LoadAnimFromFolder();
	DeleteEveryAnim();
	
	SelectPlayMode();
	SetRepeat();
	SetBlendTime();
	
	SelectAnimation(); ImGui::SameLine();
	DeleteCurrentAnimation();


	MainAnimationInfo();

	return 0;
}

void Animator3DUI::LoadAnim3D()
{
	if (ImGui::Button("LoadAnim3D##Animator3DfUI")) {
		// open a file name
		OPENFILENAME ofn = {};

		wstring strAnimationFolderPath = CPathMgr::GetInst()->GetContentPath();
		strAnimationFolderPath += L"animation\\";

		wchar_t szFilePath[256] = {};

		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = NULL;
		ofn.lpstrFile = szFilePath;
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = 256;
		ofn.lpstrFilter = L"anim3d\0*.anim3d\0ALL\0*.*";
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = strAnimationFolderPath.c_str();
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		if (false == GetOpenFileName(&ofn))
			return;

		szFilePath;
		wstring filePath = wstring(szFilePath);
		wstring targetPath = filePath.substr(filePath.find(L"animation\\"));
		GetTarget()->Animator3D()->LoadAnim(targetPath);

		//애니메이션이 로딩되면서 meshdata가 로딩되었을 수도 있다.
		ContentUI* content = (ContentUI*)ImGuiMgr::GetInst()->FindUI("##Content");
		content->ResetContent();

	}
}

wstring Animator3DUI::BrowseFolder()
{
	wchar_t path[MAX_PATH];
	BROWSEINFO bi = { 0 };
	bi.lpszTitle = L"폴더를 선택하세요.";
	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);

	if (pidl != 0)
	{
		// 사용자가 폴더를 선택하면 절대 경로를 얻음
		SHGetPathFromIDList(pidl, path);

		// 메모리 해제
		IMalloc* imalloc = 0;
		if (SUCCEEDED(SHGetMalloc(&imalloc)))
		{
			imalloc->Free(pidl);
			imalloc->Release();
		}

		return path;
	}

	return L"";
}

void Animator3DUI::LoadAnimFromFolder()
{
	if (ImGui::Button("LoadAnimFromFolder##Animator3DUI")) {

		//폴더의 상대경로를 알아냄
		wstring folderPath = BrowseFolder();
		if (folderPath.empty()) {
			MessageBox(nullptr, L"폴더를 선택하지 않았습니다.", L"폴더 선택 오류", MB_OK);
			return;
		}
		wstring ContentPath = CPathMgr::GetInst()->GetContentPath();
		size_t pos = folderPath.find(ContentPath);
		wstring relativePath;
		if (pos != std::wstring::npos)
		{
			relativePath = folderPath.substr(pos + ContentPath.length());
			GetTarget()->Animator3D()->LoadEveryAnimFromFolder(relativePath);
		}
		else
		{
			// 오류 처리. folderPath가 ContentPath로 시작하지 않는 경우입니다.
		}

	}
}

void Animator3DUI::SelectPlayMode()
{
	CAnim3D* CurAnim = GetTarget()->Animator3D()->GetCurAnim();
	if (CurAnim) {
		int pause = CurAnim->IsPause();
		ImGui::RadioButton("Play##Animator3DUI", &pause, 0);
		ImGui::SameLine();
		ImGui::RadioButton("Pause##Animator3DUI", &pause, 1);
		if (pause == 0)
			CurAnim->Play();
		else
			CurAnim->Pause();
		ImGui::SameLine();
		if (ImGui::Button("Stop##Animator3DUI")) {
			GetTarget()->Animator3D()->Stop();
		}

	}
}

void Animator3DUI::SetRepeat()
{
	bool repeat = GetTarget()->Animator3D()->IsRepeat();
	if (ImGui::Checkbox("Repeat##Anim3DEditor", &repeat)) {
		GetTarget()->Animator3D()->SetRepeat(repeat);
	}
}

void Animator3DUI::SetBlendTime()
{
	ImGui::Text("Blend time : ");
	ImGui::SameLine();
	float blendtime = GetTarget()->Animator3D()->GetBlendTime();
	ImGui::SetNextItemWidth(100.f);
	if (ImGui::InputFloat("##BlendTimeAnim3DUI", &blendtime)) {
		GetTarget()->Animator3D()->SetBlendTime(blendtime);
	}
}

void Animator3DUI::SelectAnimation()
{
	vector<string> AnimList;
	map<wstring, CAnim3D*> MapAnim = GetTarget()->Animator3D()->GetMapAnim();
	CAnim3D* CurAnim = GetTarget()->Animator3D()->GetCurAnim();
	if (MapAnim.empty()) AnimList.push_back("No Animation inside this object");
	else if (CurAnim == nullptr) AnimList.push_back("No Current Animation");

	for (const auto& pair : MapAnim)
	{
		AnimList.push_back(string(pair.first.begin(), pair.first.end()));
	}

	//std::back_inserter하기 위해선 global.h에 #include <iterator>해야함
	vector<const char*> v2;
	v2.reserve(AnimList.size());
	std::transform(AnimList.begin(), AnimList.end(), std::back_inserter(v2), [](const std::string& str) { return str.c_str(); });

	wstring targetkey;
	static int item_current_idx = 0;
	int animnum = 0;

	if (CurAnim) {
		targetkey = GetTarget()->Animator3D()->GetCurAnim()->GetName();

		auto it = MapAnim.find(targetkey);
		if (it != MapAnim.end()) {
			animnum = std::distance(MapAnim.begin(), it);
			item_current_idx = animnum;
		}
	}



	static ImGuiComboFlags flags = 0;

	ImGui::SetNextItemWidth(200.f);
	if (ImGui::BeginCombo("##CurAnimAnimator3D", AnimList[animnum].c_str(), ImGuiComboFlags_PopupAlignLeft)) {
		for (int i = 0; i < v2.size(); ++i) {
			const bool is_selected = (item_current_idx == i);
			if (ImGui::Selectable(v2[i], is_selected))
				item_current_idx = i;

			if (ImGui::IsItemHovered() &&
				ImGui::IsMouseClicked(ImGuiMouseButton_::ImGuiMouseButton_Left))

			{
				if (!MapAnim.empty()) {
					string strKey = (char*)AnimList[i].c_str();
					wstring wstrKey = wstring(strKey.begin(), strKey.end());
					bool repeatblend = GetTarget()->Animator3D()->IsRepeatBlend();
					bool isrepeat	= GetTarget()->Animator3D()->IsRepeat();
					float blendtime = GetTarget()->Animator3D()->GetBlendTime();
					GetTarget()->Animator3D()->Play(wstrKey, isrepeat, repeatblend, false, blendtime);
				}
			}

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
}

void Animator3DUI::DeleteCurrentAnimation()
{
	CAnim3D* Curanim = GetTarget()->Animator3D()->GetCurAnim();
	if (ImGui::Button("DeleteCurrentAnim##Animator3DUI")) {
		if (Curanim == nullptr) {
			wchar_t szStr[256] = {};
			wsprintf(szStr, L"Animator3DUI / Object에 Current Anim이 설정되지 않습니다.");
			MessageBox(nullptr, szStr, L"Animation Delete 실패.", MB_OK);
			return;
		}
		GetTarget()->Animator3D()->DeleteCurrentAnim();
	}
}

void Animator3DUI::DeleteEveryAnim()
{
	if (ImGui::Button("DeleteEveryAnim##Animator3DUI")) {
		map<wstring, CAnim3D*> MapAnim = GetTarget()->Animator3D()->GetMapAnim();
		if (MapAnim.size() == 0) {
			wchar_t szStr[256] = {};
			wsprintf(szStr, L"Anim3DEditorUI / Animation이 존재하지 않습니다.");
			MessageBox(nullptr, szStr, L"Animation Delete 실패.", MB_OK);
			return;
		}
		GetTarget()->Animator3D()->DeleteEveryAnim();
	}
}

void Animator3DUI::MainAnimationInfo()
{
	CAnim3D* CurAnim = GetTarget()->Animator3D()->GetCurAnim();
	if (CurAnim) {
		tMTAnimClip clip = CurAnim->GetClipList();


		ImGui::Text("StartFrame		: %d", clip.iStartFrame);
		ImGui::Text("EndFrame		: %d", clip.iEndFrame);
		ImGui::Text("StartTime		: %f", clip.dStartTime);
		ImGui::Text("EndTime		: %f", clip.dEndTime);

		ImGui::Text("CurrentFrame : "); ImGui::SameLine();
		int CurFrame = CurAnim->GetCurFrameIdx();

		ImGui::SetNextItemWidth(150.f);
		if (ImGui::InputInt("##CurFrameAnimator3DUI", &CurFrame)) {
			GetTarget()->Animator3D()->Pause();

			FbxTime::EMode timeMode = clip.eMode;				// 시간 모드
			int frameRate = FbxTime::GetFrameRate(timeMode);	// 프레임 레이트

			float TimePerFrm = 1.f / frameRate;

			CurAnim->SetUpdateTime(TimePerFrm * (CurFrame - CurAnim->GetClipList().iStartFrame));
			CurAnim->SetFrameIdx(CurFrame);
		}
	}
}
