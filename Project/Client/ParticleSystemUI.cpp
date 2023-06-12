#include "pch.h"
#include "ParticleSystemUI.h"

#include <Engine/CResMgr.h>
#include <Engine\CParticleSystem.h>
#include "TreeUI.h"


ParticleSystemUI::ParticleSystemUI()
	: ComponentUI("##ParticleSystem", COMPONENT_TYPE::PARTICLESYSTEM)

{
	SetName("ParticleSystem");


}

ParticleSystemUI::~ParticleSystemUI()
{
}




int ParticleSystemUI::render_update()
{
	if (FALSE == ComponentUI::render_update())
		return FALSE;




	m_ModuleData = GetTarget()->ParticleSystem()->GetModuleData();
	for (int i = 0; i < (int)PARTICLE_MODULE::END; ++i) {
		m_bModuleCheck[i] = m_ModuleData.ModuleCheck[i];
	}


	UINT flag = ImGuiTreeNodeFlags_Selected;

	ImGui::Text("MaxParticleSpawn");
	ImGui::SameLine();
	ImGui::InputInt("##MaxParticleSpawn", &m_ModuleData.iMaxParticleCount);

	ParticleTex = GetTarget()->ParticleSystem()->GetParticleTexture();
	if (ImGui::Button("LoadParticleTex", ImVec2(120.f, 20.f))) {
		ParticleImageLoad();
	}
	ImGui::SameLine();
	if (ImGui::Button("RemoveTexture", ImVec2(120.f, 20.f))) {
		ParticleTex = nullptr;
	}

	if (ParticleTex.Get() != nullptr) {
		ImGui::Text("ParticleName : ");
		ImGui::SameLine();
		string name(ParticleTex->GetKey().begin(), ParticleTex->GetKey().end());

		ImGui::Text(name.c_str());

		ID3D11ShaderResourceView* ImageView = (ParticleTex->GetSRV()).Get();
		ImGui::Image(ImageView, ImVec2(100.f, 100.f));
	}




	if (ImGui::Checkbox(("##SpawnCheck"), &m_bModuleCheck[(UINT)PARTICLE_MODULE::PARTICLE_SPAWN])) {
		m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::PARTICLE_SPAWN] = (int)m_bModuleCheck[(UINT)PARTICLE_MODULE::PARTICLE_SPAWN];
	}
	ImGui::SameLine();
	if (ImGui::TreeNodeEx("SpawnCheck", flag))
	{
		ImGui::Text("SpawnRate");
		ImGui::SameLine();
		ImGui::InputInt("##SpawnRate", &m_ModuleData.SpawnRate);

		static bool alpha_preview = false;
		static bool alpha_half_preview = true;
		static bool options_menu = true;

		ImGuiColorEditFlags misc_flags = (alpha_half_preview ? ImGuiColorEditFlags_AlphaPreviewHalf : (alpha_preview ? ImGuiColorEditFlags_AlphaPreview : 0)) | (options_menu ? 0 : ImGuiColorEditFlags_NoOptions);

		ImGui::Text("SpawnColor");
		ImGui::SameLine();
		ImGui::ColorEdit4("SpawnColor##Particle", (float*)&m_ModuleData.vSpawnColor, ImGuiColorEditFlags_Float | misc_flags);


		ImGui::Text("SpawnScaleMin");
		ImGui::InputFloat3("##ParticleSpawnScaleMin", m_ModuleData.vSpawnScaleMin);
		ImGui::Text("SpawnScaleMax");
		ImGui::InputFloat3("##ParticleSpawnScaleMax", m_ModuleData.vSpawnScaleMax);

		const char* SpawnShapeType[2] = { "Box", "Circle" };
		//ImGui::ListBox("listbox", &m_ModuleData.SpawnShapeType, SpawnShapeType, IM_ARRAYSIZE(SpawnShapeType));

		static ImGuiComboFlags flags = 0;
		if (ImGui::BeginCombo("##SpawnShapeType", SpawnShapeType[m_ModuleData.SpawnShapeType], flags)) {
			for (int i = 0; i < 2; ++i) {
				const bool is_selected = (m_ModuleData.SpawnShapeType == i);
				if (ImGui::Selectable(SpawnShapeType[i], is_selected))
					m_ModuleData.SpawnShapeType = i;

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		if (m_ModuleData.SpawnShapeType == 0) {
			ImGui::Text("BoxShapeScale");
			ImGui::InputFloat3("##BoxShapeScale", m_ModuleData.vBoxShapeScale);
		}
		else if (m_ModuleData.SpawnShapeType == 1)
		{
			ImGui::Text("SphereShapeScale");
			ImGui::InputFloat("##SphereShapeScale", &m_ModuleData.fSphereShapeRadius);
		}

		ImGui::Text("ModuleSpace");
		const char* ModuleSpace[2] = { "World", "Local" };
		if (ImGui::BeginCombo("##ModuleSpace", ModuleSpace[m_ModuleData.Space], flags)) {
			for (int i = 0; i < 2; ++i) {
				const bool is_selected = (m_ModuleData.Space == i);
				if (ImGui::Selectable(ModuleSpace[i], is_selected))
					m_ModuleData.Space = i;

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		ImGui::Text("MinLifeTime");
		ImGui::InputFloat("##ParticleMinLifeTime", &m_ModuleData.MinLifeTime);
		ImGui::Text("MaxLifeTime");
		ImGui::InputFloat("##ParticleMaxLifeTime", &m_ModuleData.MaxLifeTime);


		ImGui::TreePop();
	}

	if (ImGui::Checkbox(("##ColorChange"), &m_bModuleCheck[(UINT)PARTICLE_MODULE::COLOR_CHANGE])) {
		m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::COLOR_CHANGE] = m_bModuleCheck[(UINT)PARTICLE_MODULE::COLOR_CHANGE];
	}
	ImGui::SameLine();
	if (ImGui::TreeNodeEx("ColorChange", flag))
	{

		static bool alpha_preview = false;
		static bool alpha_half_preview = true;
		static bool options_menu = true;

		ImGuiColorEditFlags misc_flags = (alpha_half_preview ? ImGuiColorEditFlags_AlphaPreviewHalf : (alpha_preview ? ImGuiColorEditFlags_AlphaPreview : 0)) | (options_menu ? 0 : ImGuiColorEditFlags_NoOptions);

		ImGui::Text("StartColor");
		ImGui::SameLine();
		ImGui::ColorEdit4("StartColor##Particle", (float*)&m_ModuleData.vStartColor, ImGuiColorEditFlags_Float | misc_flags);
		ImGui::Text("EndColor");
		ImGui::SameLine();
		ImGui::ColorEdit4("EndColor##Particle", (float*)&m_ModuleData.vEndColor, ImGuiColorEditFlags_Float | misc_flags);

		ImGui::TreePop();
	}


	if (ImGui::Checkbox(("##ScaleChange"), &m_bModuleCheck[(UINT)PARTICLE_MODULE::SCALE_CHANGE])) {
		m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::SCALE_CHANGE] = m_bModuleCheck[(UINT)PARTICLE_MODULE::SCALE_CHANGE];
	}
	ImGui::SameLine();
	if (ImGui::TreeNodeEx("ScaleChange", flag)) {

		ImGui::Text("StartScale");
		ImGui::InputFloat("##ParticleStartScale", &m_ModuleData.StartScale);
		ImGui::Text("EndScale");
		ImGui::InputFloat("##ParticleEndScale", &m_ModuleData.EndScale);


		ImGui::TreePop();
	}


	if (ImGui::Checkbox(("##AddVelocity"), &m_bModuleCheck[(UINT)PARTICLE_MODULE::ADD_VELOCITY])) {
		m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::ADD_VELOCITY] = m_bModuleCheck[(UINT)PARTICLE_MODULE::ADD_VELOCITY];
	}
	ImGui::SameLine();
	if (ImGui::TreeNodeEx("AddVelocity", flag)) {

		ImGui::Text("AddVelocityType");
		ImGui::SameLine();
		static ImGuiComboFlags flags = 0;
		const char* VelocityType[3] = { "From Center", "To Center", "Directional" };
		if (ImGui::BeginCombo("##VelocityType", VelocityType[m_ModuleData.AddVelocityType], flags)) {
			for (int i = 0; i < 3; ++i) {
				const bool is_selected = (m_ModuleData.AddVelocityType == i);
				if (ImGui::Selectable(VelocityType[i], is_selected))
					m_ModuleData.AddVelocityType = i;

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		ImGui::Text("Speed");
		ImGui::SameLine();
		if (ImGui::InputFloat("##ParticleSpeed", &m_ModuleData.Speed)) {
			if (m_ModuleData.Speed <= 0.f) {
				m_ModuleData.Speed = 0.001f;
			}
		};

		if (m_ModuleData.AddVelocityType == 2) {
			ImGui::Text("VelocityDir");
			if (ImGui::DragFloat3("##VelocityDir", m_ModuleData.vVelocityDir), 0.0001f, -1.f, 1.f) {
				float x = m_ModuleData.vVelocityDir.x;
				float y = m_ModuleData.vVelocityDir.y;
				float z = m_ModuleData.vVelocityDir.z;

				float size = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
				Vec3 normalized = Vec3(x / size, y / size, z / size);
				m_ModuleData.vVelocityDir = normalized;
			}

			ImGui::Text("Angle");
			if (ImGui::DragFloat("##ParticleAngle", &m_ModuleData.OffsetAngle)) {
				if (m_ModuleData.OffsetAngle > 360) {
					m_ModuleData.OffsetAngle -= 360;
				}
				if (m_ModuleData.OffsetAngle < 0) {
					m_ModuleData.OffsetAngle += 360;
				}
			}
		}



		ImGui::TreePop();
	}
	if (ImGui::Checkbox(("##Drag"), &m_bModuleCheck[(UINT)PARTICLE_MODULE::DRAG])) {
		m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::DRAG] = m_bModuleCheck[(UINT)PARTICLE_MODULE::DRAG];
	}
	ImGui::SameLine();
	if (ImGui::TreeNodeEx("Drag", flag)) {

		ImGui::Text("StartDrag");
		ImGui::InputFloat("##ParticleStartDrag", &m_ModuleData.StartDrag);
		ImGui::Text("EndDrag");
		ImGui::InputFloat("##ParticleEndDrag", &m_ModuleData.EndDrag);


		ImGui::TreePop();
	}

	if (ImGui::Checkbox(("##NoiseForce"), &m_bModuleCheck[(UINT)PARTICLE_MODULE::NOISE_FORCE])) {
		m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::NOISE_FORCE] = m_bModuleCheck[(UINT)PARTICLE_MODULE::NOISE_FORCE];
	}
	ImGui::SameLine();
	if (ImGui::TreeNodeEx("NoiseForce", flag)) {

		ImGui::Text("NoiseTerm");
		ImGui::InputFloat("##ParticleNoiseTerm", &m_ModuleData.fNoiseTerm);
		ImGui::Text("NoiseForce");
		ImGui::InputFloat("##ParticleNoiseForce", &m_ModuleData.fNoiseForce);


		ImGui::TreePop();
	}

	if (ImGui::Checkbox(("##Gravity"), &m_bModuleCheck[(UINT)PARTICLE_MODULE::GRAVITY])) {
		m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::GRAVITY] = m_bModuleCheck[(UINT)PARTICLE_MODULE::GRAVITY];
	}
	ImGui::SameLine();
	if (ImGui::TreeNodeEx("Gravity", flag)) {

		ImGui::Text("GravityForce");
		ImGui::DragFloat("##ParticleGravity", &m_ModuleData.fGravityForce);

		ImGui::TreePop();
	}

	if (ImGui::Checkbox(("##Render"), &m_bModuleCheck[(UINT)PARTICLE_MODULE::RENDER])) {
		m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::RENDER] = m_bModuleCheck[(UINT)PARTICLE_MODULE::RENDER];
	}
	ImGui::SameLine();
	if (ImGui::TreeNodeEx("Render", flag)) {
		int a = m_ModuleData.VelocityAlignment;
		renderbool[0] = m_ModuleData.VelocityAlignment;
		renderbool[1] = m_ModuleData.VelocityScale;
		renderbool[2] = m_ModuleData.bRotate;
		renderbool[3] = m_ModuleData.AnimationUse;
		renderbool[4] = m_ModuleData.AnimationLoop;

		if (ImGui::Checkbox("##VelocityAlighment", &renderbool[0])) {
			m_ModuleData.VelocityAlignment = renderbool[0];
		}
		ImGui::SameLine();
		ImGui::Text("VelocityAlignment");
		if (m_ModuleData.VelocityAlignment) {
			ImGui::Text("MaxSpeed");
			ImGui::InputFloat("##ParticleMaxSpeed", &m_ModuleData.vMaxSpeed);
		}

		if (ImGui::Checkbox("##VelocityScale", &renderbool[1])) {
			m_ModuleData.VelocityAlignment = renderbool[1];
		}
		ImGui::SameLine();
		ImGui::Text("VelocityScale");
		if (m_ModuleData.VelocityScale) {
			ImGui::Text("MaxVelocityScale");
			ImGui::InputFloat3("##ParticleMaxVelocityScale", m_ModuleData.vMaxVelocityScale);
		}


		if (ImGui::Checkbox("##RotateParticle", &renderbool[2])) {
			m_ModuleData.VelocityAlignment = renderbool[2];
		}
		ImGui::SameLine();
		ImGui::Text("RotateParticle");
		if (m_ModuleData.bRotate) {
			ImGui::Text("RotationSpeed");
			ImGui::SameLine();
			ImGui::InputFloat("##ParticleRotationSpeed", &m_ModuleData.fRotateSpeed);

			ImGui::Text("RotationAngle");
			ImGui::SameLine();
			ImGui::InputFloat("##ParticleRotationAngle", &m_ModuleData.fRotationAngle);
		}

		if (ImGui::Checkbox("##AnimationUse", &renderbool[3])) {
			m_ModuleData.AnimationUse = renderbool[3];
		}
		ImGui::SameLine();
		ImGui::Text("AnimationUse");
		if (m_ModuleData.AnimationUse) {
			if (ImGui::Checkbox("##AnimationLoop", &renderbool[4])) {
				m_ModuleData.AnimationLoop = renderbool[4];
			}
			ImGui::SameLine();
			ImGui::Text("AnimationLoop");
			if (m_ModuleData.AnimationLoop) {
				ImGui::Text("AnimationTime");
				ImGui::SameLine();
				ImGui::InputFloat("##ParticleAnimationTime", &m_ModuleData.fAnimFrmTime);
			}
			ImGui::Text("AnimXCount");
			ImGui::SameLine();
			ImGui::InputInt("##ParticleAnimXCount", &m_ModuleData.iAnimXCount);

			ImGui::Text("AnimYCount");
			ImGui::SameLine();
			ImGui::InputInt("##ParticleAnimYCount", &m_ModuleData.iAnimYCount);
		}



		m_ModuleData.VelocityAlignment = renderbool[0];
		m_ModuleData.VelocityScale = renderbool[1];
		m_ModuleData.bRotate = renderbool[2];
		m_ModuleData.AnimationUse = renderbool[3];
		m_ModuleData.AnimationLoop = renderbool[4];
		ImGui::TreePop();
	}



	GetTarget()->ParticleSystem()->SetParticleTexture(ParticleTex);
	GetTarget()->ParticleSystem()->GetModuleData() = m_ModuleData;
	GetTarget()->ParticleSystem()->SetParticleBufferSize();


	return TRUE;
}

void ParticleSystemUI::ParticleImageLoad()
{

	// open a file name
	OPENFILENAME ofn = {};

	wstring strAnimationFolderPath = CPathMgr::GetInst()->GetContentPath();
	strAnimationFolderPath += L"texture\\";

	wchar_t szFilePath[256] = {};

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFilePath;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = 256;
	ofn.lpstrFilter = L"ALL\0*.*";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = strAnimationFolderPath.c_str();
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (false == GetOpenFileName(&ofn))
		return;

	szFilePath;
	wstring filePath = wstring(szFilePath);
	wstring targetPath = filePath.substr(filePath.find(L"texture\\"));

	wstring filename;

	size_t pos1 = filePath.rfind(L"\\");
	size_t pos2 = filePath.find(L".");
	if (pos1 != std::wstring::npos && pos2 != std::wstring::npos) {
		filename = filePath.substr(pos1 + 1, pos2 - (pos1 + 1));
	}

	ParticleTex = CResMgr::GetInst()->Load<CTexture>(filename, targetPath);


}