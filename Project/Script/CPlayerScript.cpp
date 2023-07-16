#include "pch.h"
#include "CPlayerScript.h"

#include <Engine\CMeshRender.h>
#include <Engine\CMaterial.h>
#include <Engine\CPathFinder.h>

#include <fstream>

CPlayerScript::CPlayerScript()
	: CScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT)
	, m_fSpeed(200.f)
	, m_vTargetPosTest(Vec3(0.f, 0.f, 0.f))
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fSpeed, "Speed");
	AddScriptParam(SCRIPT_PARAM::VEC4, &m_vTargetPosTest, "TargetPos");
}

CPlayerScript::~CPlayerScript()
{

}

void CPlayerScript::begin()
{
}

void CPlayerScript::tick()
{
	if (KEY_TAP(KEY::LBTN))
	{
		GetOwner()->PathFinder()->FindPathMousePicking();
		//���콺 ��ŷ������ ��ã�� ��� ������ �˷���! �̵��� ���� �ڵ忡�� ����!
	}

	if (GetOwner()->PathFinder() != nullptr)
	{
		Vec3 NextPos = GetOwner()->PathFinder()->GetNextPos();

		// NextPos�� ��ȿ�� ���̶��
		if (!isnan(NextPos.x))
		{
			// ���� ��ġ
			Vec3 CurPos = GetOwner()->Transform()->GetRelativePos();

			// ������ ���� ���ϱ�
			Vec3 Dir = (NextPos - CurPos).Normalize();

			Vec3 NewPos = CurPos + (Dir * m_fSpeed * EditorDT);

			// Pos �ݿ�
			GetOwner()->Transform()->SetRelativePos(NewPos);

			// ��ǥ������ �����ߴٸ�
			if ((NewPos - NextPos).Length() < m_fSpeed * EditorDT)
			{
				// ���� ��ġ �����϶�� ��û
				GetOwner()->PathFinder()->FindNextPath();
			}
		}
	}
}

void CPlayerScript::Shoot()
{

}



void CPlayerScript::BeginOverlap(CCollider2D* _Other)
{

}





void CPlayerScript::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_fSpeed, sizeof(float), 1, _File);
}

void CPlayerScript::LoadFromLevelFile(FILE* _File)
{
	fread(&m_fSpeed, sizeof(float), 1, _File);
}

void CPlayerScript::SaveToLevelJsonFile(Value& _objValue, Document::AllocatorType& allocator)
{
	_objValue.AddMember("fSpeed", m_fSpeed, allocator);
}

void CPlayerScript::LoadFromLevelJsonFile(const Value& _componentValue)
{
	m_fSpeed = _componentValue["fSpeed"].GetFloat();
}


//
//dtNavMesh* loadNavMeshFromBinFile(const char* path)
//{
//    const char* path = "your/path/here";
//    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
//    std::wstring wpath = converter.from_bytes(path);
//    wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
//    strFilePath += wpath;
//
//    FILE* fp = nullptr;
//    errno_t err = _wfopen_s(&fp, strFilePath.c_str(), L"rb");
//    if (err != 0 || !fp)
//    {
//        return 0;
//    }
//
//    // Read header.
//    NavMeshSetHeader header;
//    fread(&header, sizeof(NavMeshSetHeader), 1, fp);
//    if (header.magic != NAVMESHSET_MAGIC)
//    {
//        fclose(fp);
//        return 0;
//    }
//    if (header.version != NAVMESHSET_VERSION)
//    {
//        fclose(fp);
//        return 0;
//    }
//
//    dtNavMesh* navMesh = dtAllocNavMesh();
//    if (!navMesh)
//    {
//        fclose(fp);
//        return 0;
//    }
//
//    dtStatus status = navMesh->init(&header.params);
//    if (dtStatusFailed(status))
//    {
//        fclose(fp);
//        return 0;
//    }
//
//    // Read tiles.
//    for (int i = 0; i < header.numTiles; ++i)
//    {
//        NavMeshTileHeader tileHeader;
//        fread(&tileHeader, sizeof(tileHeader), 1, fp);
//        if (!tileHeader.tileRef || !tileHeader.dataSize)
//            break;
//
//        unsigned char* data = (unsigned char*)dtAlloc(tileHeader.dataSize, DT_ALLOC_PERM);
//        if (!data) break;
//        memset(data, 0, tileHeader.dataSize);
//        fread(data, tileHeader.dataSize, 1, fp);
//
//        navMesh->addTile(data, tileHeader.dataSize, DT_TILE_FREE_DATA, tileHeader.tileRef, 0);
//    }
//
//    fclose(fp);
//
//    return navMesh;
//}
//
