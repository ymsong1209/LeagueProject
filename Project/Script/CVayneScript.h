#pragma once
#pragma once
#include "CChampionScript.h"

struct tSkinningInfo
{
    Vec3 vPos;
    Vec3 vTangent;
    Vec3 vBinormal;
    Vec3 vNormal;
};


class CVayneScript :
    public CChampionScript
{
public:
    CVayneScript();
    ~CVayneScript();

private:

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    void Skinning(Vec3& _vPos, Vec3& _vTangent, Vec3& _vBinormal, Vec3& _vNormal,
        Vec4& _vWeight, Vec4& _vIndices, int _iRowIdx, CStructuredBuffer* _arrBoneMat, int _iBoneCount);

    Matrix GetBoneMat(int _iBoneIdx, int _iRowIdx, CStructuredBuffer* _arrBoneMat, int _BoneCount);
     
public:
};