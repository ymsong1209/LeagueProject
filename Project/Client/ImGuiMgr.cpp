#include "pch.h"
#include "ImGuiMgr.h"

#include <Engine\CDevice.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CKeyMgr.h>
#include <Engine\CPathMgr.h>

#include <Engine\CGameObject.h>
#include <Engine\CRenderMgr.h>

#include "UI.h"
#include "ParamUI.h"


ImGuiMgr::ImGuiMgr()
    : m_hMainHwnd(nullptr)   
    , m_hObserver(nullptr)
{

}

ImGuiMgr::~ImGuiMgr()
{
    // ImGui Release
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    // UI 삭제
    Safe_Del_Map(m_mapUI);
}


void ImGuiMgr::init(HWND _hWnd)
{
    m_hMainHwnd = _hWnd;

    // ImGui 초기화
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;
    //io.ConfigViewportsNoDefaultParent = true;
    //io.ConfigDockingAlwaysTabBar = true;
    //io.ConfigDockingTransparentPayload = true;
    //io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;     // FIXME-DPI: Experimental. THIS CURRENTLY DOESN'T WORK AS EXPECTED. DON'T USE IN USER APP!
    //io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports; // FIXME-DPI: Experimental.

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(m_hMainHwnd);
    ImGui_ImplDX11_Init(DEVICE, CONTEXT);

    // Tool 용 UI 생성
    CreateUI();

    // Content 폴더 감시
    wstring strContentPath = CPathMgr::GetInst()->GetContentPath();
    m_hObserver = FindFirstChangeNotification(strContentPath.c_str(), true
        , FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME
        | FILE_ACTION_REMOVED | FILE_ACTION_ADDED);    
}

void ImGuiMgr::progress()
{
    begin();

    tick();

    finaltick();

    render();

    // Content 폴더 변경 감시
    ObserveContent();
}



void ImGuiMgr::begin()
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ParamUI::g_NextId = 0;
}

void ImGuiMgr::tick()
{
    for (const auto& pair : m_mapUI)
    {
        pair.second->tick();
    }    
}

void ImGuiMgr::finaltick()
{
    // Demo UI
    ImGui::ShowDemoWindow();

    //LayoutDesign();

    // InspectorUI
    for (const auto& pair : m_mapUI)
    {
        if (pair.second->IsActive())
        {
            pair.second->finaltick();
        }        
    }

    if (KEY_TAP(KEY::ENTER))
        ImGui::SetWindowFocus(nullptr);

    if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow))
        CRenderMgr::GetInst()->SetIsImGuiHovered(true);
    else
        CRenderMgr::GetInst()->SetIsImGuiHovered(false);
}

void ImGuiMgr::render()
{
    // ImGui Rendering
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
}


#include "InspectorUI.h"
#include "ContentUI.h"
#include "OutlinerUI.h"
#include "ListUI.h"
#include "MenuUI.h"
#include "LevelUI.h"
#include "Anim3DEditorUI.h"

void ImGuiMgr::CreateUI()
{
    UI* pUI = nullptr;

    // InspectorUI
    pUI = new InspectorUI;
    pUI->SetActive(true);
    m_mapUI.insert(make_pair(pUI->GetID(), pUI));

    // ContentUI
    pUI = new ContentUI;
    pUI->SetActive(true);
    m_mapUI.insert(make_pair(pUI->GetID(), pUI));

    // Outliner
    pUI = new OutlinerUI;
    pUI->SetActive(true);
    m_mapUI.insert(make_pair(pUI->GetID(), pUI));

    // Menu
    pUI = new MenuUI;
    pUI->SetActive(true);
    m_mapUI.insert(make_pair(pUI->GetID(), pUI));

    //LevelUI
    pUI = new LevelUI;
    pUI->SetActive(true);
    m_mapUI.insert(make_pair(pUI->GetID(), pUI));

    // Anim3DEditor
    pUI = new Anim3DEditorUI;
    pUI->SetActive(false);
    m_mapUI.insert(make_pair(pUI->GetID(), pUI));

    // ListUI
    pUI = new ListUI;
    pUI->SetModal(true);
    pUI->SetActive(false);
    m_mapUI.insert(make_pair(pUI->GetID(), pUI));




    for (const auto& pair : m_mapUI )
    {
        pair.second->init();
    }
}

void ImGuiMgr::ObserveContent()
{
    DWORD dwWaitStatus = WaitForSingleObject(m_hObserver, 0);

    if (dwWaitStatus == WAIT_OBJECT_0)
    {
        // content 폴더에 변경점이 생겼다.
        ContentUI* UI = (ContentUI*)FindUI("##Content");
        UI->Reload();

        FindNextChangeNotification(m_hObserver);        
    }
}

void ImGuiMgr::LayoutDesign()
{
   ImGuiStyle& style = ImGui::GetStyle();
style.FrameRounding = 10.f;
style.GrabRounding = style.FrameRounding;
style.ScrollbarSize = 20;
style.GrabMinSize = 20;
style.WindowPadding = ImVec2(6, 5);

style.Colors[ImGuiCol_TitleBg] = ImVec4(0.647, 0.741, 0.659, 0.39);  // Soft Olive
style.Colors[ImGuiCol_WindowBg] = ImVec4(0.4, 0.45, 0.39, 1);  // Olive
style.Colors[ImGuiCol_Border] = ImVec4(0.35, 0.35, 0.35, 0.5);  // Gray
style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.827, 0.925, 0.831, 1);  // Light Olive
style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.827, 0.925, 0.831, 1);  // Light Olive

style.Colors[ImGuiCol_CheckMark] = ImVec4(0.933, 0.976, 0.941, 1);  // Pale Ivory
style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.533, 0.631, 0.541, 1);  // Olive Green
style.Colors[ImGuiCol_FrameBg] = ImVec4(0.3, 0.3, 0.3, 0.36);  // Dark Gray
style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.3, 0.3, 0.3, 0.78);  // Dark Gray
style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.3, 0.3, 0.3, 1);  // Dark Gray

style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.6, 0.6, 0.6, 1);  // Light Gray
style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.6, 0.6, 0.6, 1);  // Light Gray
style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(1, 1, 1, 0.09);
style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.933, 0.976, 0.941, 1);  // Pale Ivory
style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.733, 0.831, 0.741, 1);  // Soft Green

style.Colors[ImGuiCol_Button] = ImVec4(0.8, 0.8, 0.8, 0.71);  // Light Gray
style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.8, 0.8, 0.8, 0.78);  // Light Gray
style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.8, 0.8, 0.8, 1);  // Light Gray
style.Colors[ImGuiCol_Header] = ImVec4(0.647, 0.741, 0.659, 1);  // Pale Olive
style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.647, 0.741, 0.659, 1);  // Pale Olive

style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.678, 0.776, 0.686, 1);  // Pale Olive
style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.678, 0.776, 0.686, 1);  // Pale Olive
style.Colors[ImGuiCol_Tab] = ImVec4(0.678, 0.776, 0.686, 0.71);  // Pale Olive
style.Colors[ImGuiCol_TabHovered] = ImVec4(0.678, 0.776, 0.686, 1);  // Pale Olive
style.Colors[ImGuiCol_TabActive] = ImVec4(0.678, 0.776, 0.686, 1);  // Pale Olive
style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.647, 0.741, 0.659, 1);  // Light Olive


}


UI* ImGuiMgr::FindUI(const string& _UIName)
{
    map<string, UI*>::iterator iter = m_mapUI.find(_UIName);

    if(iter == m_mapUI.end())
        return nullptr;

    return iter->second;
}