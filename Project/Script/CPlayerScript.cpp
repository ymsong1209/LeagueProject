#include "pch.h"
#include "CPlayerScript.h"

#include <Engine\CMeshRender.h>
#include <Engine\CMaterial.h>

#include "CMissileScript.h"
#include <Engine\Detour\DetourNavMesh.h>
#include <Engine\Detour\DetourNavMeshBuilder.h>
#include <Engine\Detour\\DetourNavMeshQuery.h>
#include <fstream>



struct NavMeshSetHeader
{
	int magic;
	int version;
	int numTiles;
	dtNavMeshParams params;
};

struct NavMeshTileHeader
{
	dtTileRef tileRef;
	int dataSize;
};

static const int NAVMESHSET_MAGIC = 'M' << 24 | 'S' << 16 | 'E' << 8 | 'T'; //'MSET';
static const int NAVMESHSET_VERSION = 1;

dtNavMesh* CPlayerScript::loadNavMeshFromBinFile(const char* path)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
	std::wstring wpath = converter.from_bytes(path);
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += wpath;

	FILE* fp = nullptr;
	errno_t err = _wfopen_s(&fp, strFilePath.c_str(), L"rb");
	if (err != 0 || !fp)
	{
		return 0;
	}
	// Read header.
	NavMeshSetHeader header;
	fread(&header, sizeof(NavMeshSetHeader), 1, fp);
	if (header.magic != NAVMESHSET_MAGIC)
	{
		fclose(fp);
		return 0;
	}
	if (header.version != NAVMESHSET_VERSION)
	{
		fclose(fp);
		return 0;
	}

	dtNavMesh* navMesh = dtAllocNavMesh();
	if (!navMesh)
	{
		fclose(fp);
		return 0;
	}

	dtStatus status = navMesh->init(&header.params);
	if (dtStatusFailed(status))
	{
		fclose(fp);
		return 0;
	}

	// Read tiles.
	for (int i = 0; i < header.numTiles; ++i)
	{
		NavMeshTileHeader tileHeader;
		fread(&tileHeader, sizeof(tileHeader), 1, fp);
		if (!tileHeader.tileRef || !tileHeader.dataSize)
			break;

		unsigned char* data = (unsigned char*)dtAlloc(tileHeader.dataSize, DT_ALLOC_PERM);
		if (!data) break;
		memset(data, 0, tileHeader.dataSize);
		fread(data, tileHeader.dataSize, 1, fp);

		navMesh->addTile(data, tileHeader.dataSize, DT_TILE_FREE_DATA, tileHeader.tileRef, 0);
	}

	fclose(fp);

	return navMesh;

}




CPlayerScript::CPlayerScript()
	: CScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT)
	, m_fSpeed(0.f)
{
}

CPlayerScript::~CPlayerScript()
{

}

void CPlayerScript::begin()
{
}

void CPlayerScript::tick()
{
	if (KEY_TAP(KEY::ENTER))
	{
		// 미리 생성된 NavMesh 파일을 로드합니다.

		dtNavMesh* navMesh = loadNavMeshFromBinFile("Navi\\solo_navmesh.bin");
		if (!navMesh)
		{
			printf("Error loading navmesh\n");
			return;
		}

		// Query 객체를 생성합니다. 이것은 NavMesh와 함께 경로 계획을 수행합니다.
		dtNavMeshQuery* navQuery = dtAllocNavMeshQuery();
		navQuery->init(navMesh, 4866);
		Vec3 Pos = Transform()->GetRelativePos();
		// 시작 위치와 끝 위치를 설정합니다.
		float startpos[3] = { Pos.x, Pos.y, Pos.z }; // 시작 위치`
		float endpos[3] = { 100.f, 0, 0.f }; // 끝 위치

		dtPolyRef startRef, endRef;
		float polyPickExt[3] = { 6000,6000,6000 }; // 범위를 제한하기 위한 벡터

		dtQueryFilter filter;
		filter.setIncludeFlags(0xFFFF); // Include all polygons in pathfinding.
		filter.setExcludeFlags(0); // Exclude none of the polygons.

		//// Set cost for area. Area values are usually defined in the Recast/Detour code.
		//// Let's assume that 1 is ground, 2 is water, and 3 is a wall.
		//filter.setAreaCost(1, 1.0f); // Set cost for ground area.
		//filter.setAreaCost(2, 10.0f); // Set high cost for water area.
		//filter.setAreaCost(3, FLT_MAX); // Set infinite cost for wall area.
		//이거 땅,벽, 물 이런 메쉬 종류에따라 넘을수 있는 정도? 를 설정해주는 필터값인데
		//우리 메쉬에는 아마 그정보가 없는 것 같음.

		navQuery->findNearestPoly(startpos, polyPickExt, &filter, &startRef, 0);
		navQuery->findNearestPoly(endpos, polyPickExt, &filter, &endRef, 0);

		// 시작과 끝 위치를 찾습니다.
		float nearestStartPos[3], nearestEndPos[3];
		navQuery->closestPointOnPoly(startRef, startpos, nearestStartPos, 0);
		navQuery->closestPointOnPoly(endRef, endpos, nearestEndPos, 0);

		// 경로를 계획합니다.
		dtPolyRef path[256];
		int pathCount;
		navQuery->findPath(startRef, endRef, nearestStartPos, nearestEndPos, &filter, path, &pathCount, 256);

		// 경로를 따라 실제 이동 경로를 생성합니다.
		float actualPath[256 * 3];
		int actualPathCount;
		navQuery->findStraightPath(nearestStartPos, nearestEndPos, path, pathCount, actualPath, 0, 0, &actualPathCount, 256);


		// actualPathCount로 받은 값만큼 루프를 돌면서 actualPath 배열을 사용
		for (int i = 0; i < actualPathCount; ++i)
		{
			// actualPath에서 위치 가져오기 (x, y, z)
			float x = actualPath[i * 3 + 0];
			float y = actualPath[i * 3 + 1];
			float z = actualPath[i * 3 + 2];

			//// 물체의 월드 행렬 설정
			//Matrix matWorld;
			//matWorld = XMMatrixTranslation( x, y, z);

			//XMVECTOR scale, rotQuat, trans;
			//XMMatrixDecompose(&scale, &rotQuat, &trans, matWorld);

			//// 별도의 변수에 저장
			//XMFLOAT3 pos, scale3D;
			//XMFLOAT4 rot;
			//XMStoreFloat3(&pos, trans);
			//XMStoreFloat3(&scale3D, scale);
			//XMStoreFloat4(&rot, rotQuat);

			// 오브젝트에 위치, 크기, 회전 설정
			Transform()->SetRelativePos(x, y, z);
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
