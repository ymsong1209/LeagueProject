#include "pch.h"
#include "CRenderMgr.h"

#include "CResMgr.h"
#include "CDevice.h"
#include "CStructuredBuffer.h"

#include "CMRT.h"



void CRenderMgr::init()
{
    // Light2DBuffer 구조화 버퍼 생성
    m_Light2DBuffer = new CStructuredBuffer;
    m_Light2DBuffer->Create(sizeof(tLightInfo), 10, SB_TYPE::READ_ONLY, true);

    // Light3DBuffer 구조화 버퍼 생성
    m_Light3DBuffer = new CStructuredBuffer;
    m_Light3DBuffer->Create(sizeof(tLightInfo), 10, SB_TYPE::READ_ONLY, true);

    // CamFrustumBuffer 구조화 버퍼 생성
    m_CamFrustumBuffer = new CStructuredBuffer;
    m_CamFrustumBuffer->Create(sizeof(Vec4), 10, SB_TYPE::READ_ONLY, true);

    // ==================
    // SwapChain MRT 생성
    // ==================
    {
        Ptr<CTexture> arrRTTex[8] = { CResMgr::GetInst()->FindRes<CTexture>(L"RenderTargetTex"), };
        Vec4          arrClear[8] = { Vec4(0.2f, 0.2f, 0.2f, 1.f) , };
        Ptr<CTexture> DSTex = CResMgr::GetInst()->FindRes<CTexture>(L"DepthStencilTex");

        m_MRT[(UINT)MRT_TYPE::SWAPCHAIN] = new CMRT;
        m_MRT[(UINT)MRT_TYPE::SWAPCHAIN]->Create(arrRTTex, arrClear, DSTex);
    }

    // ============
    // Deferred MRT
    // ============
    {
        Vec2 vRenderResolotion = CDevice::GetInst()->GetRenderResolution();

        Ptr<CTexture> arrRTTex[8] = {
              CResMgr::GetInst()->CreateTexture(L"ColorTargetTex"
                , (UINT)vRenderResolotion.x, (UINT)vRenderResolotion.y
                , DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
                , D3D11_USAGE_DEFAULT),

              CResMgr::GetInst()->CreateTexture(L"NormalTargetTex"
                , (UINT)vRenderResolotion.x, (UINT)vRenderResolotion.y
                , DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
                , D3D11_USAGE_DEFAULT),

             CResMgr::GetInst()->CreateTexture(L"PositionTargetTex"
                , (UINT)vRenderResolotion.x, (UINT)vRenderResolotion.y
                , DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
                , D3D11_USAGE_DEFAULT),

             CResMgr::GetInst()->CreateTexture(L"DataTargetTex"
                , (UINT)vRenderResolotion.x, (UINT)vRenderResolotion.y
                , DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
                , D3D11_USAGE_DEFAULT),

            CResMgr::GetInst()->CreateTexture(L"EmissiveTargetTex"
                , (UINT)vRenderResolotion.x, (UINT)vRenderResolotion.y
                , DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
                , D3D11_USAGE_DEFAULT),
        };

        Vec4          arrClear[8] = { Vec4(0.f, 0.f, 0.f, 0.f) , };

        Ptr<CTexture> DSTex = CResMgr::GetInst()->FindRes<CTexture>(L"DepthStencilTex");

        m_MRT[(UINT)MRT_TYPE::DEFERRED] = new CMRT;
        m_MRT[(UINT)MRT_TYPE::DEFERRED]->Create(arrRTTex, arrClear, DSTex);
    }

    // =========
    // DECAL MRT
    // =========
    {
        Vec2 vRenderResolotion = CDevice::GetInst()->GetRenderResolution();

        Ptr<CTexture> arrRTTex[8] = {
            CResMgr::GetInst()->FindRes<CTexture>(L"ColorTargetTex"),
            CResMgr::GetInst()->FindRes<CTexture>(L"EmissiveTargetTex"),
        };
        Vec4          arrClear[8] = { Vec4(0.f, 0.f, 0.f, 0.f) , };

        Ptr<CTexture> DSTex = nullptr;

        m_MRT[(UINT)MRT_TYPE::DECAL] = new CMRT;
        m_MRT[(UINT)MRT_TYPE::DECAL]->Create(arrRTTex, arrClear, DSTex);
    }

    // =========
    // Light MRT
    // =========
    {
        Vec2 vRenderResolotion = CDevice::GetInst()->GetRenderResolution();

        Ptr<CTexture> arrRTTex[8] = {
              CResMgr::GetInst()->CreateTexture(L"DiffuseTargetTex"
                , (UINT)vRenderResolotion.x, (UINT)vRenderResolotion.y
                , DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
                , D3D11_USAGE_DEFAULT),

              CResMgr::GetInst()->CreateTexture(L"SpecularTargetTex"
                , (UINT)vRenderResolotion.x, (UINT)vRenderResolotion.y
                , DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
                , D3D11_USAGE_DEFAULT),
        };

        Vec4          arrClear[8] = { Vec4(0.f, 0.f, 0.f, 0.f) , };

        m_MRT[(UINT)MRT_TYPE::LIGHT] = new CMRT;
        m_MRT[(UINT)MRT_TYPE::LIGHT]->Create(arrRTTex, arrClear, nullptr);
    }

    // =============
    // ShadowMap MRT
    // =============
    {
        Ptr<CTexture> arrRTTex[8] = {
            CResMgr::GetInst()->CreateTexture(L"DepthMapTex"
              , 4096, 4096
              , DXGI_FORMAT_R32_FLOAT, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
              , D3D11_USAGE_DEFAULT),
        };

        Vec4 arrClear[8] = { Vec4(0.f, 0.f, 0.f, 0.f) , };

        // Depth Stencil Texture 만들기
        Ptr<CTexture> DSTex = CResMgr::GetInst()->CreateTexture(L"DepthMapDSTex", 4096, 4096
            , DXGI_FORMAT_D32_FLOAT, D3D11_BIND_DEPTH_STENCIL, D3D11_USAGE_DEFAULT);

        m_MRT[(UINT)MRT_TYPE::SHADOW] = new CMRT;
        m_MRT[(UINT)MRT_TYPE::SHADOW]->Create(arrRTTex, arrClear, DSTex);
    }

}