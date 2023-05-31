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
    
    Ptr<CTexture> TargetTex = (CTexture*)GetTargetRes().Get();

    ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
    ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
    ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
    ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white

    if (TargetTex == nullptr)
    {
        ImGui::Image((ImTextureID)0, ImVec2(150, 150), uv_min, uv_max, tint_col, border_col);
    }
    else
    {
        ImGui::Image((ImTextureID)TargetTex->GetSRV().Get(), ImVec2(150, 150), uv_min, uv_max, tint_col, border_col);
    }

    return 0;
}
