#pragma once


class CTimeMgr
	: public CSingleton<CTimeMgr>
{	
	SINGLE(CTimeMgr);
private:
	LARGE_INTEGER	m_llPrevCount;
	LARGE_INTEGER	m_llCurCount;
	LARGE_INTEGER	m_llFrequency;

	UINT			m_iCallCount;
	float			m_fDeltaTime;
	float			m_fEditorDeltaTime;
	float			m_fTime;
	float			m_fAccTime;

	float			m_fTimeScale;

	

public:
	void init();
	void tick();
	void render();
	void SetTimeScale(float _Scale) { m_fTimeScale = _Scale; }

public:
	float GetDeltaTime() { return m_fDeltaTime; }
	float GetEditorDeltaTime() { return m_fEditorDeltaTime; }
	float GetAccTime() { return m_fAccTime; }


};

