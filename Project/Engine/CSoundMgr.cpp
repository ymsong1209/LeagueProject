#include "pch.h"
#include "CSoundMgr.h"


CSoundMgr::CSoundMgr()
{
}

CSoundMgr::~CSoundMgr()
{
	Safe_Del_Vec(m_vSounds);
}