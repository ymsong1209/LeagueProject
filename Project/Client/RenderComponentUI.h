#pragma once
#include "UI.h"

#include <Engine\ptr.h>
#include <Engine\CRes.h>

class CGameObject;


class RenderComponentUI :
    public UI
{
private:
    CGameObject* m_pTarget;

public:
    void SetTarget(CGameObject* _target);

public:
    virtual int render_update() override;
public:
    RenderComponentUI();
    ~RenderComponentUI();
};
