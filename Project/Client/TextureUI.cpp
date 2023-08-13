#include "pch.h"
#include "TextureUI.h"

#include <Engine/ptr.h>
#include <Engine/CTexture.h>

TextureUI::TextureUI()
    : ResUI(RES_TYPE::TEXTURE)
{
    SetName("Texture");
}

TextureUI::~TextureUI()
{
}

int TextureUI::render_update()
{
    ResUI::render_update();
    
    //Ptr<CTexture> TargetTex = (CTexture*)GetTargetRes().Get();

    //ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
    //ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
    //ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
    //ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white

    //if (TargetTex == nullptr)
    //{
    //    ImGui::Image((ImTextureID)0, ImVec2(150, 150), uv_min, uv_max, tint_col, border_col);
    //}
    //else
    //{
    //    ImGui::Image((ImTextureID)TargetTex->GetSRV().Get(), ImVec2(150, 150), uv_min, uv_max, tint_col, border_col);
    //}

     // Texture ¿Ã∏ß
    ImGui::Text("Texture");
    ImGui::SameLine();

    Ptr<CTexture> pTex = (CTexture*)GetTargetRes().Get();
    string strKey = string(pTex->GetKey().begin(), pTex->GetKey().end());
    ImGui::InputText("##TEXUIName", (char*)strKey.c_str(), ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);

    ID3D11ShaderResourceView* Atlas = (pTex->GetSRV()).Get();
    float AtlasWidth = pTex->Width();
    float AtlasHeight = pTex->Height();
    static float MaterialUIzoom = 0.5f;

    if (ImGui::BeginChild("TextureUIimage", ImVec2(200.f, 200.f), false, ImGuiWindowFlags_HorizontalScrollbar))
    {
        ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
        ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
        ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
        ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white
        ImGui::Image((void*)Atlas, ImVec2(150 * MaterialUIzoom, 150 * MaterialUIzoom), uv_min, uv_max, tint_col, border_col);
        ImGui::EndChild();
    }

    if (ImGui::InputFloat("zoom", &MaterialUIzoom, 0.5f, 1.0f))
    {
        if (MaterialUIzoom < 0.1f)
            MaterialUIzoom = 0.1f;

    }

    if (ImGui::SliderFloat("zoom2", &MaterialUIzoom, 0.0f, 20.f)) {

    };

    return 0;
}
