#pragma once

class CEngine
	: public CSingleton<CEngine>
{
private:
	HWND	m_hWnd;

	// 윈도우 해상도
	Vec2	m_vResolution;

	bool	m_bShowImgui;
	

public:
	Vec2 GetWindowResolution() { return m_vResolution; }
	HWND GetMainWnd() { return m_hWnd ; }
	bool GetImguiActive() { return m_bShowImgui; }

public:
	int init(HWND _hWnd, UINT _iWidth, UINT _iHeight);
	void progress();

private:
	void tick();
	void render();


public:
	CEngine();
	~CEngine();
};
