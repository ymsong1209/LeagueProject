#pragma once
#include "CComponent.h"

class CState;

class CFsm :
    public CComponent
{
public:
    CFsm();
    CFsm(const CFsm& _other);
    ~CFsm();

public:
    CLONE(CFsm);

private:
    map<wstring, CState*>   m_mapState;     // 보유중인 State 목록
    CState*                 m_pCurState;    // 현재 State    

public:
    virtual void tick() override;
    virtual void finaltick() override {};

public:
    void    AddState(const wstring& _strName, CState* _pState);
    CState* FindState(const wstring& _strName);
    void    ChangeState(const wstring& _strStateName);

    CState* GetCurState() { return m_pCurState; }

public:
    virtual void SaveToLevelFile(FILE* _File) override {};
    virtual void LoadFromLevelFile(FILE* _File) override {};

};

