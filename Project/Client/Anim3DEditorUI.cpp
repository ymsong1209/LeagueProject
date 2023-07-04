#include "pch.h"
#include "Anim3DEditorUI.h"
#include <Engine/CAnimator3D.h>
#include <Engine/CAnim3D.h>
#include <Engine/CMesh.h>
#include <Engine/CMeshData.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CGameObject.h>
#include <Engine/components.h>
#include <Engine/CPathMgr.h>
#include <Engine/CResMgr.h>
#include "ContentUI.h"



Anim3DEditorUI::Anim3DEditorUI()
	: UI("##Anim3DEditorUI")
	, m_iEditMode(0)
	, m_pTestObject(nullptr)
	, m_bRepeat(false)
	, m_fBlendTime(0.f)
{
	SetName("Anim3DEditorUI");
}

Anim3DEditorUI::~Anim3DEditorUI()
{
}

void Anim3DEditorUI::tick()
{
}

int Anim3DEditorUI::render_update()
{
	SpawnTestObject(); ImGui::SameLine();
	LoadFBX(); ImGui::SameLine();
	LoadMeshdata(); ImGui::SameLine();
	LoadAnim3D();
	SelectEditMode();

	if (!FindTestObject()) return TRUE;

	if (m_iEditMode == 0) {
		SelectPlayMode(); ImGui::SameLine();
		SetRepeat();
		SetRepeatBlendTime();
		SelectMainAnimation(); ImGui::SameLine();
		DeleteCurrentAnimation(); ImGui::SameLine();
		DeleteEveryAnim();
		MainAnimationInfo();
		SetCustomAnim();
		SaveCustomAnim();
	}
	else if (m_iEditMode == 1) {
		SelectBlendTestPlayMode();
		SetBlendTime();
		SelectBaseAnimation();
		SelectBlendAnimation();
		BlendAnim();
	}
	return TRUE;
}

bool Anim3DEditorUI::FindTestObject()
{
	CGameObject* test = CLevelMgr::GetInst()->FindObjectByName(L"Anim3DTestObject");
	if (test) return true;
	else {
		return false;
		m_pTestObject = nullptr;
	}
}
void Anim3DEditorUI::SpawnTestObject()
{
	if (ImGui::Button("SpawnTestObject##Anim3DEditor")) {

		CLevel* CurLevel = CLevelMgr::GetInst()->GetCurLevel();
		if (CurLevel->GetName() == L"")
		{
			wchar_t szStr[256] = {};
			wsprintf(szStr, L"레벨 이름 없음 / 레벨을 먼저 저장하십시요");
			MessageBox(nullptr, szStr, L"레이어 이름 변경후 저장실패.", MB_OK);
			return;
		}

		CGameObject* TestObject = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Anim3DTestObject");
		if (TestObject) {
			wchar_t szStr[256] = {};
			wsprintf(szStr, L"Test Object가 이미 Level에 존재합니다/ Anim3DTestObject 오브젝트를 찾으십시오.");
			MessageBox(nullptr, szStr, L"Test Object 소환 실패.", MB_OK);
			return;
		}
		// Spawn Test Object
		TestObject = new CGameObject;
		TestObject->SetName(L"Anim3DTestObject");

		TestObject->AddComponent(new CTransform);
		TestObject->AddComponent(new CMeshRender);
		TestObject->AddComponent(new CAnimator3D);
		TestObject->Animator3D()->SetDebugAnimator(true);

		SpawnGameObject(TestObject, Vec3(0.f, 0.f, 0.f), 0);

		m_pTestObject = TestObject;
	}
}

void Anim3DEditorUI::LoadFBX()
{
	if (ImGui::Button("LoadFBX##Anim3DEditorUI")) {
		if (!FindTestObject()) {
			wchar_t szStr[256] = {};
			wsprintf(szStr, L"Anim3DEditorUI / TestObject가 없습니다. Spawn버튼을 누르십시요.");
			MessageBox(nullptr, szStr, L"FBX 로드 실패.", MB_OK);
			return;
		}

		// open a file name
		OPENFILENAME ofn = {};

		wstring strAnimationFolderPath = CPathMgr::GetInst()->GetContentPath();
		strAnimationFolderPath += L"fbx\\";

		wchar_t szFilePath[256] = {};

		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = NULL;
		ofn.lpstrFile = szFilePath;
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = 256;
		ofn.lpstrFilter = L"fbx\0*.fbx\0ALL\0*.*";
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = strAnimationFolderPath.c_str();
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		if (false == GetOpenFileName(&ofn))
			return;

		szFilePath;
		wstring filePath = wstring(szFilePath);
		wstring targetPath = filePath.substr(filePath.find(L"fbx\\"));

		Ptr<CMeshData> pMeshData = nullptr;
		pMeshData = CResMgr::GetInst()->LoadFBX(targetPath);

		if (false == pMeshData->GetMesh()->IsAnimMesh()) {
			wchar_t szStr[256] = {};
			wsprintf(szStr, L"Anim3DEditorUI / fbx가 애니메이션을 들고 있지 않습니다.");
			MessageBox(nullptr, szStr, L"FBX 로드 실패.", MB_OK);
			//애니메이션이 로딩되면서 meshdata가 로딩되었을 수도 있다.
			ContentUI* content = (ContentUI*)ImGuiMgr::GetInst()->FindUI("##Content");
			content->ResetContent();
			return;
		}

		m_pTestObject->MeshRender()->SetMesh(pMeshData->GetMesh());
		for (UINT i = 0; i < pMeshData->GetMaterial().size(); ++i) {
			m_pTestObject->MeshRender()->SetMaterial(pMeshData->GetMaterial()[i], i);
		}

		m_pTestObject->Animator3D()->SetBones(pMeshData->GetMesh()->GetBones());
		m_pTestObject->Animator3D()->SetMeshDataRelativePath(pMeshData->GetRelativePath());

		const vector<tMTAnimClip>* animClipPtr = pMeshData->GetMesh()->GetAnimClip();  // GetAnimClip()로부터 포인터를 가져옴

		// 벡터의 요소들에 접근
		for (const tMTAnimClip& animClip : *animClipPtr)
		{
			// 각 tMTAnimClip에 대한 작업 수행
			// animClip을 읽기 전용으로 사용
			CAnim3D* Anim = m_pTestObject->Animator3D()->CreateAnimation(animClip);
			Anim->Save();
		}

		//애니메이션이 로딩되면서 meshdata가 로딩되었을 수도 있다.
		ContentUI* content = (ContentUI*)ImGuiMgr::GetInst()->FindUI("##Content");
		content->ResetContent();

	}
}

void Anim3DEditorUI::LoadMeshdata()
{
	if (ImGui::Button("LoadMeshData##Anim3DEditorUI")) {
		if (!FindTestObject()) {
			wchar_t szStr[256] = {};
			wsprintf(szStr, L"Anim3DEditorUI / TestObject가 없습니다. Spawn버튼을 누르십시요.");
			MessageBox(nullptr, szStr, L"MeshData 로드 실패.", MB_OK);
			return;
		}

		// open a file name
		OPENFILENAME ofn = {};

		wstring strAnimationFolderPath = CPathMgr::GetInst()->GetContentPath();
		strAnimationFolderPath += L"meshdata\\";

		wchar_t szFilePath[256] = {};

		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = NULL;
		ofn.lpstrFile = szFilePath;
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = 256;
		ofn.lpstrFilter = L"mdat\0*.mdat\0ALL\0*.*";
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = strAnimationFolderPath.c_str();
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		if (false == GetOpenFileName(&ofn))
			return;

		szFilePath;
		wstring filePath = wstring(szFilePath);
		wstring targetPath = filePath.substr(filePath.find(L"meshdata\\"));

		Ptr<CMeshData> pMeshData = nullptr;
		pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(targetPath);

		if (false == pMeshData->GetMesh()->IsAnimMesh()) {
			wchar_t szStr[256] = {};
			wsprintf(szStr, L"Anim3DEditorUI / MeshData가 애니메이션을 들고 있지 않습니다.");
			MessageBox(nullptr, szStr, L"MeshData 로드 실패.", MB_OK);
			//애니메이션이 로딩되면서 meshdata가 로딩되었을 수도 있다.
			ContentUI* content = (ContentUI*)ImGuiMgr::GetInst()->FindUI("##Content");
			content->ResetContent();
			return;
		}

		m_pTestObject->MeshRender()->SetMesh(pMeshData->GetMesh());
		for (UINT i = 0; i < pMeshData->GetMaterial().size(); ++i) {
			m_pTestObject->MeshRender()->SetMaterial(pMeshData->GetMaterial()[i], i);
		}

		m_pTestObject->Animator3D()->SetBones(pMeshData->GetMesh()->GetBones());
		m_pTestObject->Animator3D()->SetMeshDataRelativePath(pMeshData->GetRelativePath());

		const vector<tMTAnimClip>* animClipPtr = pMeshData->GetMesh()->GetAnimClip();  // GetAnimClip()로부터 포인터를 가져옴

		// 벡터의 요소들에 접근
		for (const tMTAnimClip& animClip : *animClipPtr)
		{
			// 각 tMTAnimClip에 대한 작업 수행
			// animClip을 읽기 전용으로 사용
			CAnim3D* Anim = m_pTestObject->Animator3D()->CreateAnimation(animClip);
			Anim->Save();
		}

		//애니메이션이 로딩되면서 meshdata가 로딩되었을 수도 있다.
		ContentUI* content = (ContentUI*)ImGuiMgr::GetInst()->FindUI("##Content");
		content->ResetContent();

	}
}

void Anim3DEditorUI::LoadAnim3D()
{
	if (ImGui::Button("LoadAnim3D##Anim3DEditorUI")) {
		if (!FindTestObject()) {
			wchar_t szStr[256] = {};
			wsprintf(szStr, L"Anim3DEditorUI / TestObject가 없습니다. Spawn버튼을 누르십시요.");
			MessageBox(nullptr, szStr, L"Anim3D 로드 실패.", MB_OK);
			return;
		}

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
		m_pTestObject->Animator3D()->LoadAnim(targetPath);

		//애니메이션이 로딩되면서 meshdata가 로딩되었을 수도 있다.
		ContentUI* content = (ContentUI*)ImGuiMgr::GetInst()->FindUI("##Content");
		content->ResetContent();

	}
}

void Anim3DEditorUI::SelectEditMode()
{
	ImGui::RadioButton("Create Animation##Anim3DEditor", &m_iEditMode, 0); ImGui::SameLine();
	ImGui::RadioButton("Blend Test##Anim3DEditor", &m_iEditMode, 1);
}

void Anim3DEditorUI::SelectPlayMode()
{
	CAnim3D* CurAnim = m_pTestObject->Animator3D()->GetCurAnim();
	if (CurAnim) {
		int pause = CurAnim->IsPause();
		ImGui::RadioButton("Play##Anim3DEditor", &pause, 0);
		ImGui::SameLine();
		ImGui::RadioButton("Pause##Anim3DEditor", &pause, 1);
		if (pause == 0)
			CurAnim->Play();
		else
			CurAnim->Pause();
		ImGui::SameLine();
		if (ImGui::Button("Stop##Anim3dEditor")) {
			m_pTestObject->Animator3D()->Stop();
		}

	}
}

void Anim3DEditorUI::SetRepeat()
{
	m_bRepeat = m_pTestObject->Animator3D()->IsRepeat();
	if (ImGui::Checkbox("Repeat##Anim3DEditor", &m_bRepeat)) {
		m_pTestObject->Animator3D()->SetRepeat(m_bRepeat);
	}
}

void Anim3DEditorUI::SetRepeatBlendTime()
{
	m_bRepeat = m_pTestObject->Animator3D()->IsRepeat();
	m_fBlendTime = m_pTestObject->Animator3D()->GetBlendTime();
	bool repeatblend = m_pTestObject->Animator3D()->IsRepeatBlend();
	if (m_bRepeat) {
		ImGui::SameLine();
		if (ImGui::Checkbox("RepeatBlend##Anim3DEditor", &repeatblend)) {
			m_pTestObject->Animator3D()->SetRepeatBlend(repeatblend, m_fBlendTime);
		}
		if (repeatblend) {
			ImGui::SetNextItemWidth(100.f);
			if (ImGui::InputFloat("##RepeatTimeAnimation3DUI", &m_fBlendTime)) {
				m_pTestObject->Animator3D()->SetRepeatBlend(repeatblend, m_fBlendTime);
			}
		}
	}
}

void Anim3DEditorUI::SetBlendTime()
{

	ImGui::Text("Blend time : ");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(100.f);
	if (ImGui::InputFloat("##BlendTimeAnim3DEditor", &m_fBlendTime)) {

	}
}

void Anim3DEditorUI::SelectMainAnimation()
{
	vector<string> AnimList;
	map<wstring, CAnim3D*> MapAnim = m_pTestObject->Animator3D()->GetMapAnim();
	CAnim3D* CurAnim = m_pTestObject->Animator3D()->GetCurAnim();
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
		targetkey = m_pTestObject->Animator3D()->GetCurAnim()->GetName();

		auto it = MapAnim.find(targetkey);
		if (it != MapAnim.end()) {
			animnum = std::distance(MapAnim.begin(), it);
			item_current_idx = animnum;
		}
	}



	static ImGuiComboFlags flags = 0;

	ImGui::SetNextItemWidth(400.f);
	if (ImGui::BeginCombo("##CurAnimAnimator3DEditor", AnimList[animnum].c_str(), ImGuiComboFlags_PopupAlignLeft)) {
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
					m_pTestObject->Animator3D()->Play(wstrKey, m_bRepeat, m_fBlendTime);
				}
			}

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
}

void Anim3DEditorUI::DeleteCurrentAnimation()
{
	CAnim3D* Curanim = m_pTestObject->Animator3D()->GetCurAnim();
	if (ImGui::Button("DeleteCurrentAnim##Anim3DEditorUI")) {
		if (Curanim == nullptr) {
			wchar_t szStr[256] = {};
			wsprintf(szStr, L"Anim3DEditorUI / TestObject에 Current Anim이 설정되지 않습니다.");
			MessageBox(nullptr, szStr, L"Animation Delete 실패.", MB_OK);
			return;
		}
		m_pTestObject->Animator3D()->DeleteCurrentAnim();
	}
}

void Anim3DEditorUI::DeleteEveryAnim()
{
	if (ImGui::Button("DeleteEveryAnim##Anim3DEditorUI")) {
		map<wstring, CAnim3D*> MapAnim = m_pTestObject->Animator3D()->GetMapAnim();
		if (MapAnim.size() == 0) {
			wchar_t szStr[256] = {};
			wsprintf(szStr, L"Anim3DEditorUI / Animation이 존재하지 않습니다.");
			MessageBox(nullptr, szStr, L"Animation Delete 실패.", MB_OK);
			return;
		}
		m_pTestObject->Animator3D()->DeleteEveryAnim();
	}
}

void Anim3DEditorUI::SaveEveryAnim()
{
	vector<string> AnimList;
	map<wstring, CAnim3D*> MapAnim = m_pTestObject->Animator3D()->GetMapAnim();
	if (MapAnim.size() == 0) {
		wchar_t szStr[256] = {};
		wsprintf(szStr, L"Anim3DEditorUI / TestObject에 Anim이 없습니다.");
		MessageBox(nullptr, szStr, L"Animation Save 실패.", MB_OK);
		return;
	}
	else if (MapAnim.size() == 1) {
		auto iter = MapAnim.find(L"TestAnimation");
		if (iter == MapAnim.end()) {
			wchar_t szStr[256] = {};
			wsprintf(szStr, L"Anim3DEditorUI / TestObject에 TestAnimation만 존재.");
			MessageBox(nullptr, szStr, L"Animation Save 실패.", MB_OK);
			return;
		}
	}


	for (const auto& pair : MapAnim)
	{
		if (pair.first != L"TestAnimation") {
			wstring targetpath;
			targetpath += L"animation\\";
			targetpath += pair.first;
			targetpath += L".anim3d";
			pair.second->Save(targetpath);
		}
	}

}

void Anim3DEditorUI::MainAnimationInfo()
{
	CAnim3D* CurAnim = m_pTestObject->Animator3D()->GetCurAnim();
	if (CurAnim) {
		tMTAnimClip clip = CurAnim->GetClipList();


		ImGui::Text("StartFrame		: %d", clip.iStartFrame);
		ImGui::Text("EndFrame		: %d", clip.iEndFrame);
		ImGui::Text("StartTime		: %f", clip.dStartTime);
		ImGui::Text("EndTime		: %f", clip.dEndTime);

		ImGui::Text("CurrentFrame : "); ImGui::SameLine();
		int CurFrame = CurAnim->GetCurFrameIdx();

		ImGui::SetNextItemWidth(150.f);
		if (ImGui::InputInt("##CurFrameAnim3DEditor", &CurFrame)) {
			m_pTestObject->Animator3D()->Pause();

			FbxTime::EMode timeMode = clip.eMode;				// 시간 모드
			int frameRate = FbxTime::GetFrameRate(timeMode);	// 프레임 레이트

			float TimePerFrm = 1.f / frameRate;

			CurAnim->SetUpdateTime(TimePerFrm * (CurFrame - CurAnim->GetClipList().iStartFrame));
			CurAnim->SetFrameIdx(CurFrame);
		}
		ImGui::SameLine();
		if (ImGui::Button("Set As Start##Anim3DEditor")) {
			m_iStartFrm = CurFrame;
		}
		ImGui::SameLine();
		if (ImGui::Button("Set As End##Anim3DEditor")) {
			m_iEndFrm = CurFrame;
		}
	}
}

void Anim3DEditorUI::SetCustomAnim()
{
	ImGui::Text("StartFrame		: %d", m_iStartFrm);
	ImGui::Text("EndFrame		: %d", m_iEndFrm); ImGui::SameLine();
	if (ImGui::Button("Bake Animation##Anim3DEditor")) {
		CAnim3D* Anim = m_pTestObject->Animator3D()->FindAnim(L"TestAnimation");
		tMTAnimClip clip;
		clip.strAnimName = L"TestAnimation";
		clip.iStartFrame = m_iStartFrm;
		clip.iEndFrame = m_iEndFrm;
		clip.iFrameLength = m_iEndFrm - m_iStartFrm;
		clip.eMode = m_pTestObject->Animator3D()->GetCurAnim()->GetClipList().eMode;
		int frameRate = FbxTime::GetFrameRate(clip.eMode);	// 프레임 레이트
		float TimePerFrm = 1.f / frameRate;
		clip.dStartTime = TimePerFrm * m_iStartFrm;
		clip.dEndTime = TimePerFrm * m_iEndFrm;
		clip.dTimeLength = clip.dEndTime - clip.dStartTime;

		if (Anim == nullptr) {

			m_pTestObject->Animator3D()->CreateAnimation(clip, L"TestAnimation");
		}
		else {
			Anim->SetClipList(clip);
		}
	}

}

void Anim3DEditorUI::SaveCustomAnim()
{
	if (ImGui::Button("SaveCustomAnim##Anim3DEditorUI")) {
		CAnim3D* anim = m_pTestObject->Animator3D()->FindAnim(L"TestAnimation");
		if (!anim) {
			wchar_t szStr[256] = {};
			wsprintf(szStr, L"Anim3DEditorUI / TestObject에 TestAnimation이 존재하지 않습니다.");
			MessageBox(nullptr, szStr, L"Animation Save 실패.", MB_OK);
			return;
		}

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

		if (false == GetSaveFileName(&ofn))
			return;

		szFilePath;
		wstring filePath = wstring(szFilePath);
		int length = filePath.length();
		if (length < 7 || filePath.substr(length - 7) != L".anim3d") {
			filePath.append(L".anim3d");
		}
		wstring targetPath = filePath.substr(filePath.find(L"animation\\"));
		wstring fileName = targetPath.substr(targetPath.find(L"\\") + 1, targetPath.find(L".") - targetPath.find(L"\\") - 1);

		tMTAnimClip clip = anim->GetClipList();
		clip.strAnimName = fileName;
		anim->SetClipList(clip);
		anim->SetName(fileName);
		anim->Save(targetPath);

		clip.strAnimName = L"TestAnimation";
		anim->SetName(L"TestAnimation");
		anim->SetClipList(clip);
	}



}

void Anim3DEditorUI::SelectBlendTestPlayMode()
{
	if (!m_pBaseAnim) return;

	if (nullptr == m_pTestObject->Animator3D()->FindAnim(m_pBaseAnim->GetName())) {
		m_pBaseAnim = nullptr;
		return;
	};

	CAnim3D* CurAnim = m_pTestObject->Animator3D()->GetCurAnim();

	int pause;
	if (CurAnim == nullptr) pause = 1;
	else if (CurAnim->IsFinish()) pause = 1;
	else {
		pause = CurAnim->IsPause();
	}
	if (ImGui::RadioButton("Play##Anim3DEditor", &pause, 0)) {
		if (m_pBlendAnim == nullptr) {
			wchar_t szStr[256] = {};
			wsprintf(szStr, L"Anim3DEditorUI / Blend Animation이 설정되지 않습니다.");
			MessageBox(nullptr, szStr, L"Animation Play 실패.", MB_OK);
			return;
		}
		else if (m_pTestObject->Animator3D()->FindAnim(m_pBlendAnim->GetName()) == nullptr) {
			wchar_t szStr[256] = {};
			wsprintf(szStr, L"Anim3DEditorUI / Blend Animation이 nullptr로 설정되었습니다.");
			MessageBox(nullptr, szStr, L"Animation Play 실패.", MB_OK);
			m_pBlendAnim = nullptr;
			return;
		}

		m_pTestObject->Animator3D()->Play(m_pBaseAnim->GetName(), false, m_fBlendTime);
	}
	ImGui::SameLine();

	if (ImGui::RadioButton("Pause##Anim3DEditor", &pause, 1)) {
		if (CurAnim) {
			m_pTestObject->Animator3D()->Pause();
		}
	}

	ImGui::SameLine();
	if (ImGui::Button("Stop##Anim3dEditor")) {
		m_pTestObject->Animator3D()->Stop();
	}
}

void Anim3DEditorUI::SelectBaseAnimation()
{
	vector<string> AnimList;
	map<wstring, CAnim3D*> MapAnim = m_pTestObject->Animator3D()->GetMapAnim();
	CAnim3D* Curanim = m_pTestObject->Animator3D()->GetCurAnim();

	if (MapAnim.empty()) AnimList.push_back("No Animation inside this object");
	else if (m_pBaseAnim == nullptr) AnimList.push_back("No Base Animation Selected");

	for (const auto& pair : MapAnim)
	{
		AnimList.push_back(string(pair.first.begin(), pair.first.end()));
	}

	//std::back_inserter하기 위해선 global.h에 #include <iterator>해야함
	vector<const char*> v2;
	v2.reserve(AnimList.size());
	std::transform(AnimList.begin(), AnimList.end(), std::back_inserter(v2), [](const std::string& str) { return str.c_str(); });

	wstring targetkey;
	static int BaseAnim_current_idx = 0;
	int animnum = 0;

	if (m_pBaseAnim) {
		targetkey = m_pBaseAnim->GetName();

		auto it = MapAnim.find(targetkey);
		if (it != MapAnim.end()) {
			animnum = std::distance(MapAnim.begin(), it);
			BaseAnim_current_idx = animnum;
		}
	}



	static ImGuiComboFlags flags = 0;

	ImGui::SetNextItemWidth(400.f);
	if (ImGui::BeginCombo("##CurBaseAnimAnimator3DEditor", AnimList[animnum].c_str(), ImGuiComboFlags_PopupAlignLeft)) {
		for (int i = 0; i < v2.size(); ++i) {
			const bool is_selected = (BaseAnim_current_idx == i);
			if (ImGui::Selectable(v2[i], is_selected))
				BaseAnim_current_idx = i;

			if (ImGui::IsItemHovered() &&
				ImGui::IsMouseClicked(ImGuiMouseButton_::ImGuiMouseButton_Left))

			{
				if (!MapAnim.empty()) {
					string strKey = (char*)AnimList[i].c_str();
					wstring wstrKey = wstring(strKey.begin(), strKey.end());
					m_pBaseAnim = m_pTestObject->Animator3D()->FindAnim(wstrKey);
				}
			}

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
}

void Anim3DEditorUI::SelectBlendAnimation()
{
	vector<string> AnimList;
	map<wstring, CAnim3D*> MapAnim = m_pTestObject->Animator3D()->GetMapAnim();

	if (MapAnim.empty()) AnimList.push_back("No Animation inside this object");
	else if (m_pBlendAnim == nullptr) AnimList.push_back("No Blend Animation Selected");

	for (const auto& pair : MapAnim)
	{
		AnimList.push_back(string(pair.first.begin(), pair.first.end()));
	}

	//std::back_inserter하기 위해선 global.h에 #include <iterator>해야함
	vector<const char*> v2;
	v2.reserve(AnimList.size());
	std::transform(AnimList.begin(), AnimList.end(), std::back_inserter(v2), [](const std::string& str) { return str.c_str(); });

	wstring targetkey;
	static int Blenditem_current_idx = 0;
	int animnum = 0;

	if (m_pBlendAnim) {
		targetkey = m_pBlendAnim->GetName();

		auto it = MapAnim.find(targetkey);
		if (it != MapAnim.end()) {
			animnum = std::distance(MapAnim.begin(), it);
			Blenditem_current_idx = animnum;
		}
	}



	static ImGuiComboFlags flags = 0;

	ImGui::SetNextItemWidth(400.f);
	if (ImGui::BeginCombo("##CurBlendAnimAnimator3DEditor", AnimList[animnum].c_str(), ImGuiComboFlags_PopupAlignLeft)) {
		for (int i = 0; i < v2.size(); ++i) {
			const bool is_selected = (Blenditem_current_idx == i);
			if (ImGui::Selectable(v2[i], is_selected))
				Blenditem_current_idx = i;

			if (ImGui::IsItemHovered() &&
				ImGui::IsMouseClicked(ImGuiMouseButton_::ImGuiMouseButton_Left))

			{
				if (!MapAnim.empty()) {
					string strKey = (char*)AnimList[i].c_str();
					wstring wstrKey = wstring(strKey.begin(), strKey.end());
					m_pBlendAnim = m_pTestObject->Animator3D()->FindAnim(wstrKey);
				}
			}

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
}

void Anim3DEditorUI::BlendAnim()
{
	if (m_pBaseAnim && m_pBlendAnim) {
		CAnim3D* curanim = m_pTestObject->Animator3D()->GetCurAnim();
		if (!curanim) return;

		//baseanim이 끝나면 blendanim blend하면서 재생
		if (curanim == m_pBaseAnim && curanim->IsFinish() && !m_pTestObject->Animator3D()->IsBlend()) {
			m_pTestObject->Animator3D()->Play(m_pBlendAnim->GetName(), true, m_fBlendTime);
		}
	}
}
