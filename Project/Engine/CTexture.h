#pragma once
#include "CRes.h"

#include <DirectXTex\DirectXTex.h>

#ifdef _DEBUG
#pragma comment(lib, "DirectXTex//DirectXTex_debug")
#else 
#pragma comment(lib, "DirectXTex//DirectXTex")
#endif

class CTexture :
    public CRes
{
private:
    ComPtr<ID3D11Texture2D>             m_Tex2D;
    ComPtr<ID3D11ShaderResourceView>    m_SRV;
    ComPtr<ID3D11RenderTargetView>	    m_RTV;
    ComPtr<ID3D11DepthStencilView>	    m_DSV;
    ComPtr<ID3D11UnorderedAccessView>   m_UAV;

    D3D11_TEXTURE2D_DESC                m_Desc;
    ScratchImage                        m_Image;

    UINT                                m_iRecentNum;

public:
    int Create(UINT _Width, UINT _Height, DXGI_FORMAT _pixelformat
        , UINT _BindFlag, D3D11_USAGE _Usage);

    int  Create(ComPtr<ID3D11Texture2D> _tex2D);
    int  CreateArrayTexture(const vector<Ptr<CTexture>>& _vecTex, int _iMapLevel);
    void GenerateMip(UINT _iMipLevel);

    void SaveTextureAsDDS(const wstring& _LevelPath = L"");


public:
    float                               Width() { return (float)m_Desc.Width; }
	float                               Height() { return (float)m_Desc.Height; }
    UINT                                GetArraySize() { return m_Desc.ArraySize; }
    UINT                                GetRowPitch() const { return (UINT)m_Image.GetImages()->rowPitch; }
    UINT                                GetSlicePitch()const { return (UINT)m_Image.GetImages()->slicePitch; }
    void* GetSysMem()                   { return m_Image.GetPixels(); }

    const D3D11_TEXTURE2D_DESC&         GetDesc() { return m_Desc; }
	ComPtr<ID3D11Texture2D>             GetTex2D() { return m_Tex2D; }
	ComPtr<ID3D11ShaderResourceView>    GetSRV() { return m_SRV; }
	ComPtr<ID3D11RenderTargetView>	    GetRTV() { return m_RTV; }
	ComPtr<ID3D11DepthStencilView>	    GetDSV() { return m_DSV; }
	ComPtr<ID3D11UnorderedAccessView>   GetUAV() { return m_UAV; }

    // MipMap을 포함한 Texture 전용 Load 함수
    int Load(const wstring& _strFilePath, int _iMipLevel);

private:
    virtual int Load(const wstring& _strFilePath) override;
public:
    virtual int Save(const wstring& _strRelativePath) override;

public:
    // _PipelineStage : PIPELINE_STAGE
    void UpdateData(int _iRegisterNum, int _PipelineStage);
    void UpdateData_CS(int _iRegisterNum, bool _bShaderRes);
    void Clear();
    static void Clear(int _iRegisterNum);
    void Clear_CS(bool _bShaderRes);


private:
    void UpdateData();

public:
    CTexture(bool _bEngine = false);
    ~CTexture();
};

