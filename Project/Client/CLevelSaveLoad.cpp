#include "pch.h"
#include "CLevelSaveLoad.h"

#include <Engine\CPathMgr.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CLayer.h>
#include <Engine\CGameObject.h>
#include <Engine\components.h>
#include <Engine\CScript.h>

#include <Script\CScriptMgr.h>
#include <Engine/CCollisionMgr.h>
#include <Engine/CRenderMgr.h>


int CLevelSaveLoad::SaveLevel(const wstring& _LevelPath, CLevel* _Level)
{
	if (_Level->GetState() != LEVEL_STATE::STOP) {
		wchar_t szStr[256] = {};
		wsprintf(szStr, L"현재 Level이 Stop이 아닙니다. Stop상태로 만드십시요.");
		MessageBox(nullptr, szStr, L"Level Save 실패.", MB_OK);
		return E_FAIL;
	}

	wstring strPath = CPathMgr::GetInst()->GetContentPath();
	strPath += _LevelPath;

	FILE* pFile = nullptr;

	_wfopen_s(&pFile, strPath.c_str(), L"wb");

	if (nullptr == pFile)	
		return E_FAIL;

	// 레벨 이름 저장
	SaveWString(_LevelPath, pFile);


	// 레벨의 레이어들을 저장
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		CLayer* pLayer = _Level->GetLayer(i);

		// 레이어 이름 저장
		SaveWString(pLayer->GetName(), pFile);

		// 레이어의 게임오브젝트들 저장
		const vector<CGameObject*>& vecParent = pLayer->GetParentObject();

		// 오브젝트 개수 저장
		size_t objCount = vecParent.size();
		fwrite(&objCount, sizeof(size_t), 1, pFile);
		
		// 각 게임오브젝트
		for (size_t i = 0; i < objCount; ++i)
		{
			SaveGameObject(vecParent[i], pFile);
		}
	}

	//오브젝트 충돌 정보 저장
	UINT CollisionMatrix[MAX_LAYER];
	for (UINT i = 0; i < MAX_LAYER; ++i) {
		CollisionMatrix[i] = CCollisionMgr::GetInst()->GetMatrix()[i];
	}
	fwrite(CollisionMatrix, sizeof(UINT), 32, pFile);

	fclose(pFile);


	return S_OK;
}

int CLevelSaveLoad::SaveGameObject(CGameObject* _Object, FILE* _File)
{
	// 이름
	SaveWString(_Object->GetName(), _File);
	
	// 컴포넌트
	for (UINT i = 0; i <= (UINT)COMPONENT_TYPE::END; ++i)
	{		
		if (i == (UINT)COMPONENT_TYPE::END)
		{
			// 컴포넌트 타입 저장
			fwrite(&i, sizeof(UINT), 1, _File);
			break;
		}

		CComponent* Com = _Object->GetComponent((COMPONENT_TYPE)i);
		if (nullptr == Com)
			continue;

		// 컴포넌트 타입 저장
		fwrite(&i, sizeof(UINT), 1, _File);

		// 컴포넌트 정보 저장
		Com->SaveToLevelFile(_File);
	}

	// 스크립트	
	const vector<CScript*>& vecScript = _Object->GetScripts();
	size_t ScriptCount = vecScript.size();
	fwrite(&ScriptCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < vecScript.size(); ++i)
	{
		wstring ScriptName = CScriptMgr::GetScriptName(vecScript[i]);
		SaveWString(ScriptName, _File);
		vecScript[i]->SaveToLevelFile(_File);
	}


	// 자식 오브젝트
	const vector<CGameObject*>& vecChild = _Object->GetChild();
	size_t ChildCount = vecChild.size();
	fwrite(&ChildCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < ChildCount; ++i)
	{
		SaveGameObject(vecChild[i], _File);		
	}

	return 0;
}

CLevel* CLevelSaveLoad::LoadLevel(const wstring& _LevelPath)
{
	wstring strPath = CPathMgr::GetInst()->GetContentPath();
	strPath += _LevelPath;

	FILE* pFile = nullptr;

	_wfopen_s(&pFile, strPath.c_str(), L"rb");

	if (nullptr == pFile)
		return nullptr;
	
	//Camera의 생성자에서 RenderMgr에 등록할라면 먼저 기존에 RenderMgr가 초기화되어야함
	CRenderMgr::GetInst()->ClearCamera();
	CLevel* NewLevel = new CLevel;

	// 레벨 이름
	wstring strLevelName;
	LoadWString(strLevelName, pFile);
	NewLevel->SetName(strLevelName);


	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		CLayer* pLayer = NewLevel->GetLayer(i);

		// 레이어 이름
		wstring LayerName;
		LoadWString(LayerName, pFile);
		pLayer->SetName(LayerName);

		// 게임 오브젝트 개수
		size_t objCount = 0;
		fread(&objCount, sizeof(size_t), 1, pFile);

		// 각 게임오브젝트
		for (size_t j = 0; j < objCount; ++j)
		{
			CGameObject* pNewObj = LoadGameObject(pFile);
			NewLevel->AddGameObject(pNewObj, i, false);
		}
	}

	
	UINT matrix[MAX_LAYER];
	fread(&matrix, sizeof(UINT), 32, pFile);
	CCollisionMgr::GetInst()->SetMatrix(matrix);

	fclose(pFile);

	return NewLevel;
}

CGameObject* CLevelSaveLoad::LoadGameObject(FILE* _File)
{
	CGameObject* pObject = new CGameObject;

	// 이름
	wstring Name;
	LoadWString(Name, _File);
	pObject->SetName(Name);

	// 컴포넌트
	while (true)
	{
		UINT ComponentType = 0;
		fread(&ComponentType, sizeof(UINT), 1, _File);

		// 컴포넌트 정보의 끝을 확인
		if ((UINT)COMPONENT_TYPE::END == ComponentType)
			break;

		CComponent* Component = nullptr;

		switch ((COMPONENT_TYPE)ComponentType)
		{
		case COMPONENT_TYPE::TRANSFORM:
			Component = new CTransform;
			break;
		case COMPONENT_TYPE::COLLIDER2D:
			Component = new CCollider2D;
			break;
		case COMPONENT_TYPE::COLLIDER3D:
			Component = new CCollider3D;
			break;
		case COMPONENT_TYPE::ANIMATOR2D:
			Component = new CAnimator2D;
			break;
		case COMPONENT_TYPE::ANIMATOR3D:
			Component = new CAnimator3D;
			break;
		case COMPONENT_TYPE::LIGHT2D:
			Component = new CLight2D;
			break;
		case COMPONENT_TYPE::LIGHT3D:
			Component = new CLight3D;
			break;
		case COMPONENT_TYPE::CAMERA:
			Component = new CCamera;
			break;
		case COMPONENT_TYPE::MESHRENDER:
			Component = new CMeshRender;
			break;
		case COMPONENT_TYPE::PARTICLESYSTEM:
			Component = new CParticleSystem;
			break;
		case COMPONENT_TYPE::TILEMAP:
			Component = new CTileMap;
			break;
		case COMPONENT_TYPE::LANDSCAPE:			
			Component = new CLandScape;
			break;
		case COMPONENT_TYPE::DECAL:
			Component = new CDecal;
			break;
		case COMPONENT_TYPE::SKYBOX:
			Component = new CSkyBox;
			break;
		case COMPONENT_TYPE::FSM:
			Component = new CFsm;
			break;
		}

		Component->LoadFromLevelFile(_File);
		pObject->AddComponent(Component);
	}


	// 스크립트	
	size_t ScriptCount = 0;
	fread(&ScriptCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < ScriptCount; ++i)
	{
		wstring ScriptName;
		LoadWString(ScriptName, _File);
		CScript* pScript = CScriptMgr::GetScript(ScriptName);
		pObject->AddComponent(pScript);
		pScript->LoadFromLevelFile(_File);
	}

	// 자식 오브젝트		
	size_t ChildCount = 0;
	fread(&ChildCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < ChildCount; ++i)
	{
		CGameObject* ChildObject = LoadGameObject(_File);
		pObject->AddChild(ChildObject);
	}

	return pObject;
}

int CLevelSaveLoad::SaveLevelToJson(const wstring& levelPath, CLevel* level) 
{
	if (level->GetState() != LEVEL_STATE::STOP) {
		wchar_t szStr[256] = {};
		wsprintf(szStr, L"현재 Level이 Stop이 아닙니다. Stop상태로 만드십시요.");
		MessageBox(nullptr, szStr, L"Level Save 실패.", MB_OK);
		return E_FAIL;
	}

	Document document;
	document.SetObject();

	Document::AllocatorType& allocator = document.GetAllocator();

	// 레벨 이름 저장
	Value levelValue(kStringType);
	levelValue.SetString(wStrToStr(levelPath).c_str(), wStrToStr(levelPath).length(), allocator);
	
	document.AddMember("levelName", SaveWStringJson(levelPath,allocator), allocator);

	// 레벨의 레이어들을 저장
	Value layers(kArrayType);
	for (UINT i = 0; i < MAX_LAYER; ++i) {
		CLayer* layer = level->GetLayer(i);

		Value layerValue(kObjectType);  // 레이어 정보

		// 1. 레이어 이름
		layerValue.AddMember("layerName", SaveWStringJson(layer->GetName(), allocator), allocator);

		// 2. 레이어의 게임 오브젝트들 저장
		const vector<CGameObject*>& gameObjects = layer->GetParentObject();
		Value gameObjectsArray(kArrayType);
		for (CGameObject* gameObject : gameObjects) {
			SaveGameObjectToJson(gameObject, allocator, gameObjectsArray);
		}
		layerValue.AddMember("gameObjects", gameObjectsArray, allocator);

		layers.PushBack(layerValue, allocator);
	}
	document.AddMember("layers", layers, allocator);


	//오브젝트 충돌 정보 저장
	UINT CollisionMatrix[MAX_LAYER];
	for (UINT i = 0; i < MAX_LAYER; ++i) {
		CollisionMatrix[i] = CCollisionMgr::GetInst()->GetMatrix()[i];
	}

	// CollisionMatrixArray 배열을 저장
	Value CollisionMatrixArray(kArrayType);
	for (UINT i = 0; i < MAX_LAYER; ++i) {
		CollisionMatrixArray.PushBack(CollisionMatrix[i], allocator);
	}
	document.AddMember("CollisionMatrix", CollisionMatrixArray, allocator);



	// JSON 데이터를 문자열로 변환
	rapidjson::StringBuffer buffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer); // 가독성
	//rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	document.Accept(writer);
	std::string jsonData = buffer.GetString();

	// JSON 데이터 출력
	std::cout << "JSON data: " << jsonData << std::endl;

	// JSON 데이터를 level 폴더 경로에  json파일에 저장
	wstring strPath = CPathMgr::GetInst()->GetContentPath();
	strPath += levelPath;

	std::ofstream file(strPath);
	if (file.is_open()) {
		file << jsonData << std::endl;
		file.close();
	}
	else {
		wchar_t szStr[256] = {};
		wsprintf(szStr, L"Failed to open JSON file.");
		MessageBox(nullptr, szStr, L"Level Save 실패. // LevelSaveLoad", MB_OK);
		return E_FAIL;

	}

	return 0;
}

int CLevelSaveLoad::SaveGameObjectToJson(CGameObject* gameObject, Document::AllocatorType& allocator, rapidjson::Value& value)
{
	// 이름 저장
	Value gameObjectValue(kObjectType);
	gameObjectValue.AddMember("ObjName", SaveWStringJson(gameObject->GetName(), allocator), allocator);

	// 컴포넌트 저장
	Value components(kArrayType);
	for (UINT i = 0; i <= (UINT)COMPONENT_TYPE::END; ++i) {
		if (i == (UINT)COMPONENT_TYPE::END) {
			// 마지막 저장
			components.PushBack(i, allocator);
			break;
		}

		CComponent* Com = gameObject->GetComponent((COMPONENT_TYPE)i);
		if (nullptr == Com)
			continue;

		// 컴포넌트 value
		Value componentValue(kObjectType);

		// 컴포넌트 이름 저장
		string componentName = ToString((COMPONENT_TYPE)i);
		Value componentNameValue(componentName.c_str(), allocator);
		componentValue.AddMember(componentNameValue, i, allocator);

		// 컴포넌트 정보 저장
		Com->SaveToLevelJsonFile(componentValue, allocator);

		components.PushBack(componentValue, allocator);
	}
	gameObjectValue.AddMember("components", components, allocator);

	// 스크립트 저장
	const vector<CScript*>& scripts = gameObject->GetScripts();
	Value scriptArray(kArrayType);
	for (CScript* script : scripts) {
		Value scriptValue(kObjectType);
		wstring scriptName = CScriptMgr::GetScriptName(script);

		scriptValue.AddMember("scriptName", SaveWStringJson(scriptName,allocator), allocator);
		script->SaveToLevelJsonFile(scriptValue, allocator);
		scriptArray.PushBack(scriptValue, allocator);
	}
	gameObjectValue.AddMember("scripts", scriptArray, allocator);


	// 자식 오브젝트 저장
	const vector<CGameObject*>& vecChild = gameObject->GetChild();
	size_t ChildCount = vecChild.size();

	Value childObjectsArray(kArrayType);
	for (size_t i = 0; i < ChildCount; ++i)
	{
		SaveGameObjectToJson(vecChild[i], allocator, childObjectsArray);
	}
	gameObjectValue.AddMember("children", childObjectsArray, allocator);

	value.PushBack(gameObjectValue, allocator);

	return 0;
}

CLevel* CLevelSaveLoad::LoadLevelFromJson(const wstring& _LevelPath)
{
	// JSON 파일 경로 설정
	wstring jsonPath = CPathMgr::GetInst()->GetContentPath() + _LevelPath;

	// JSON 파일 열기
	ifstream file(jsonPath);
	if (!file.is_open())
	{
		wchar_t szStr[256] = {};
		wsprintf(szStr, L"Failed to open JSON file.");
		MessageBox(nullptr, szStr, L"Level Load 실패.", MB_OK);
		// 파일 열기 실패
		return nullptr;
	}
	// JSON 데이터 읽기
	string jsonData((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());

	// JSON 데이터 파싱
	Document document;
	document.Parse(jsonData.c_str());
	if (document.HasParseError())
	{
		wchar_t szStr[256] = {};
		wsprintf(szStr, L"JSON 파일 파싱 실패.");
		MessageBox(nullptr, szStr, L"Level Load 실패.", MB_OK);
		// 파싱 실패
		return nullptr;
	}

	//Camera의 생성자에서 RenderMgr에 등록할라면 먼저 기존에 RenderMgr가 초기화되어야함
	CRenderMgr::GetInst()->ClearCamera();

	// CLevel 객체 생성
	CLevel* NewLevel = new CLevel();

	// 레벨 이름 로드
	if (document.HasMember("levelName"))
	{
		const Value& levelNameValue = document["levelName"];
		if (levelNameValue.IsString())
		{
			wstring levelName = StrToWStr(levelNameValue.GetString());
			NewLevel->SetName(levelName);
		}
	}

	// 레이어들 로드
	if (document.HasMember("layers"))
	{
		const Value& layersValue = document["layers"];
		if (layersValue.IsArray())
		{
			for (SizeType i = 0; i < layersValue.Size(); ++i)
			{
				const Value& layerValue = layersValue[i];
				if (layerValue.IsObject())
				{
					// 레이어 생성
					CLayer* pLayer = NewLevel->GetLayer(i);

					// 레이어 이름 로드
					if (layerValue.HasMember("layerName"))
					{
						const Value& layerNameValue = layerValue["layerName"];
						if (layerNameValue.IsString())
						{
							wstring layerName = StrToWStr(layerNameValue.GetString());
							pLayer->SetName(layerName);
						}
					}

					// 게임 오브젝트들 로드
					if (layerValue.HasMember("gameObjects"))
					{
						const Value& gameObjectsValue = layerValue["gameObjects"];
						if (gameObjectsValue.IsArray())
						{
							for (SizeType j = 0; j < gameObjectsValue.Size(); ++j)
							{
								const Value& gameObjectValue = gameObjectsValue[j];
								if (gameObjectValue.IsObject())
								{
									// 게임 오브젝트 생성
									CGameObject* gameObject = new CGameObject();

									// 게임 오브젝트 정보 로드
									LoadGameObjectFromJson(gameObjectValue, gameObject);

									// 레이어에 게임 오브젝트 추가
									NewLevel->AddGameObject(gameObject, i, false);
								}
							}
						}
					}

				}
			}
		}
	}

	// 오브젝트 충돌 정보 로드
	UINT matrix[MAX_LAYER];
	const Value& CollisionMatrixArray = document["CollisionMatrix"];
	size_t CollisionMatrixArraySize = CollisionMatrixArray.Size();
	for (size_t i = 0; i < CollisionMatrixArraySize; ++i) {
		matrix[i] = CollisionMatrixArray[i].GetUint();
	}
	CCollisionMgr::GetInst()->SetMatrix(matrix);

	// 파일 닫기
	file.close();

	return NewLevel;
}

CGameObject* CLevelSaveLoad::LoadGameObjectFromJson(const Value& _gameObjectValue, CGameObject* _gameObject)
{
	// 게임 오브젝트 이름 로드
	if (_gameObjectValue.HasMember("ObjName"))
	{
		const Value& objNameValue = _gameObjectValue["ObjName"];
		if (objNameValue.IsString())
		{
			std::wstring objName = StrToWStr(objNameValue.GetString());
			_gameObject->SetName(objName);
		}
	}

	// 컴포넌트 로드
	if (_gameObjectValue.HasMember("components"))
	{
		const Value& componentsValue = _gameObjectValue["components"];
		if (componentsValue.IsArray())
		{
			for (SizeType i = 0; i < componentsValue.Size(); ++i)
			{
				const Value& componentValue = componentsValue[i];
				if (componentValue.IsObject())
				{
					Value::ConstMemberIterator itr = componentValue.MemberBegin();
					const Value& key = itr->value;
					UINT ComponentType = key.GetInt();

					// 컴포넌트 로드 및 추가
					// 컴포넌트 정보의 끝을 확인
					if ((UINT)COMPONENT_TYPE::END == ComponentType)
						break;

					CComponent* Component = nullptr;
					
					switch ((COMPONENT_TYPE)ComponentType)
					{
					case COMPONENT_TYPE::TRANSFORM:
						Component = new CTransform;
						break;
					case COMPONENT_TYPE::COLLIDER2D:
						Component = new CCollider2D;
						break;
					case COMPONENT_TYPE::COLLIDER3D:
						Component = new CCollider3D;
						break;
					case COMPONENT_TYPE::ANIMATOR2D:
						Component = new CAnimator2D;
						break;
					case COMPONENT_TYPE::ANIMATOR3D:
						Component = new CAnimator3D;
						break;
					case COMPONENT_TYPE::LIGHT2D:
						Component = new CLight2D;
						break;
					case COMPONENT_TYPE::LIGHT3D:
						Component = new CLight3D;
						break;
					case COMPONENT_TYPE::CAMERA:
						Component = new CCamera;
						break;
					case COMPONENT_TYPE::MESHRENDER:
						Component = new CMeshRender;
						break;
					case COMPONENT_TYPE::PARTICLESYSTEM:
						Component = new CParticleSystem;
						break;
					case COMPONENT_TYPE::TILEMAP:
						Component = new CTileMap;
						break;
					case COMPONENT_TYPE::SKYBOX:
						Component = new CSkyBox;
						break;
					case COMPONENT_TYPE::LANDSCAPE:
						Component = new CLandScape;
						break;
					case COMPONENT_TYPE::DECAL:
						Component = new CDecal;
						break;
					case COMPONENT_TYPE::FSM:
						Component = new CFsm;
						break;
					}

					Component->LoadFromLevelJsonFile(componentValue);
					_gameObject->AddComponent(Component);
				}
			}
		}
	}

	// 스크립트 load
	const Value& scriptsValue = _gameObjectValue["scripts"];
	for (SizeType i = 0; i < scriptsValue.Size(); ++i)
	{
		const Value& scriptValue = scriptsValue[i];
		if (scriptValue.IsObject())
		{
			wstring ScriptName = StrToWStr(scriptValue["scriptName"].GetString());
			CScript* pScript = CScriptMgr::GetScript(ScriptName);

			pScript->LoadFromLevelJsonFile(scriptValue);
			_gameObject->AddComponent(pScript);
		}
	}

	// 자식 오브젝트 load
	const Value& childrenValue = _gameObjectValue["children"];
	for (SizeType i = 0; i < childrenValue.Size(); ++i)
	{
		const Value& childValue = childrenValue[i];
		if (childValue.IsObject())
		{
			// 게임 오브젝트 생성
			CGameObject* childGameObject = new CGameObject();
			LoadGameObjectFromJson(childValue, childGameObject);
			_gameObject->AddChild(childGameObject);
		}
	}

	return _gameObject;
}

