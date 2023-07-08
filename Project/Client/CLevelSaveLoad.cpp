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
		wsprintf(szStr, L"���� Level�� Stop�� �ƴմϴ�. Stop���·� ����ʽÿ�.");
		MessageBox(nullptr, szStr, L"Level Save ����.", MB_OK);
		return E_FAIL;
	}

	wstring strPath = CPathMgr::GetInst()->GetContentPath();
	strPath += _LevelPath;

	FILE* pFile = nullptr;

	_wfopen_s(&pFile, strPath.c_str(), L"wb");

	if (nullptr == pFile)	
		return E_FAIL;

	// ���� �̸� ����
	SaveWString(_LevelPath, pFile);


	// ������ ���̾���� ����
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		CLayer* pLayer = _Level->GetLayer(i);

		// ���̾� �̸� ����
		SaveWString(pLayer->GetName(), pFile);

		// ���̾��� ���ӿ�����Ʈ�� ����
		const vector<CGameObject*>& vecParent = pLayer->GetParentObject();

		// ������Ʈ ���� ����
		size_t objCount = vecParent.size();
		fwrite(&objCount, sizeof(size_t), 1, pFile);
		
		// �� ���ӿ�����Ʈ
		for (size_t i = 0; i < objCount; ++i)
		{
			SaveGameObject(vecParent[i], pFile);
		}
	}

	//������Ʈ �浹 ���� ����
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
	// �̸�
	SaveWString(_Object->GetName(), _File);
	
	// ������Ʈ
	for (UINT i = 0; i <= (UINT)COMPONENT_TYPE::END; ++i)
	{		
		if (i == (UINT)COMPONENT_TYPE::END)
		{
			// ������Ʈ Ÿ�� ����
			fwrite(&i, sizeof(UINT), 1, _File);
			break;
		}

		CComponent* Com = _Object->GetComponent((COMPONENT_TYPE)i);
		if (nullptr == Com)
			continue;

		// ������Ʈ Ÿ�� ����
		fwrite(&i, sizeof(UINT), 1, _File);

		// ������Ʈ ���� ����
		Com->SaveToLevelFile(_File);
	}

	// ��ũ��Ʈ	
	const vector<CScript*>& vecScript = _Object->GetScripts();
	size_t ScriptCount = vecScript.size();
	fwrite(&ScriptCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < vecScript.size(); ++i)
	{
		wstring ScriptName = CScriptMgr::GetScriptName(vecScript[i]);
		SaveWString(ScriptName, _File);
		vecScript[i]->SaveToLevelFile(_File);
	}


	// �ڽ� ������Ʈ
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
	
	//Camera�� �����ڿ��� RenderMgr�� ����Ҷ�� ���� ������ RenderMgr�� �ʱ�ȭ�Ǿ����
	CRenderMgr::GetInst()->ClearCamera();
	CLevel* NewLevel = new CLevel;

	// ���� �̸�
	wstring strLevelName;
	LoadWString(strLevelName, pFile);
	NewLevel->SetName(strLevelName);


	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		CLayer* pLayer = NewLevel->GetLayer(i);

		// ���̾� �̸�
		wstring LayerName;
		LoadWString(LayerName, pFile);
		pLayer->SetName(LayerName);

		// ���� ������Ʈ ����
		size_t objCount = 0;
		fread(&objCount, sizeof(size_t), 1, pFile);

		// �� ���ӿ�����Ʈ
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

	// �̸�
	wstring Name;
	LoadWString(Name, _File);
	pObject->SetName(Name);

	// ������Ʈ
	while (true)
	{
		UINT ComponentType = 0;
		fread(&ComponentType, sizeof(UINT), 1, _File);

		// ������Ʈ ������ ���� Ȯ��
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


	// ��ũ��Ʈ	
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

	// �ڽ� ������Ʈ		
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
		wsprintf(szStr, L"���� Level�� Stop�� �ƴմϴ�. Stop���·� ����ʽÿ�.");
		MessageBox(nullptr, szStr, L"Level Save ����.", MB_OK);
		return E_FAIL;
	}

	Document document;
	document.SetObject();

	Document::AllocatorType& allocator = document.GetAllocator();

	// ���� �̸� ����
	Value levelValue(kStringType);
	levelValue.SetString(wStrToStr(levelPath).c_str(), wStrToStr(levelPath).length(), allocator);
	
	document.AddMember("levelName", SaveWStringJson(levelPath,allocator), allocator);

	// ������ ���̾���� ����
	Value layers(kArrayType);
	for (UINT i = 0; i < MAX_LAYER; ++i) {
		CLayer* layer = level->GetLayer(i);

		Value layerValue(kObjectType);  // ���̾� ����

		// 1. ���̾� �̸�
		layerValue.AddMember("layerName", SaveWStringJson(layer->GetName(), allocator), allocator);

		// 2. ���̾��� ���� ������Ʈ�� ����
		const vector<CGameObject*>& gameObjects = layer->GetParentObject();
		Value gameObjectsArray(kArrayType);
		for (CGameObject* gameObject : gameObjects) {
			SaveGameObjectToJson(gameObject, allocator, gameObjectsArray);
		}
		layerValue.AddMember("gameObjects", gameObjectsArray, allocator);

		layers.PushBack(layerValue, allocator);
	}
	document.AddMember("layers", layers, allocator);


	//������Ʈ �浹 ���� ����
	UINT CollisionMatrix[MAX_LAYER];
	for (UINT i = 0; i < MAX_LAYER; ++i) {
		CollisionMatrix[i] = CCollisionMgr::GetInst()->GetMatrix()[i];
	}

	// CollisionMatrixArray �迭�� ����
	Value CollisionMatrixArray(kArrayType);
	for (UINT i = 0; i < MAX_LAYER; ++i) {
		CollisionMatrixArray.PushBack(CollisionMatrix[i], allocator);
	}
	document.AddMember("CollisionMatrix", CollisionMatrixArray, allocator);



	// JSON �����͸� ���ڿ��� ��ȯ
	rapidjson::StringBuffer buffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer); // ������
	//rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	document.Accept(writer);
	std::string jsonData = buffer.GetString();

	// JSON ������ ���
	std::cout << "JSON data: " << jsonData << std::endl;

	// JSON �����͸� level ���� ��ο�  json���Ͽ� ����
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
		MessageBox(nullptr, szStr, L"Level Save ����. // LevelSaveLoad", MB_OK);
		return E_FAIL;

	}

	return 0;
}

int CLevelSaveLoad::SaveGameObjectToJson(CGameObject* gameObject, Document::AllocatorType& allocator, rapidjson::Value& value)
{
	// �̸� ����
	Value gameObjectValue(kObjectType);
	gameObjectValue.AddMember("ObjName", SaveWStringJson(gameObject->GetName(), allocator), allocator);

	// ������Ʈ ����
	Value components(kArrayType);
	for (UINT i = 0; i <= (UINT)COMPONENT_TYPE::END; ++i) {
		if (i == (UINT)COMPONENT_TYPE::END) {
			// ������ ����
			components.PushBack(i, allocator);
			break;
		}

		CComponent* Com = gameObject->GetComponent((COMPONENT_TYPE)i);
		if (nullptr == Com)
			continue;

		// ������Ʈ value
		Value componentValue(kObjectType);

		// ������Ʈ �̸� ����
		string componentName = ToString((COMPONENT_TYPE)i);
		Value componentNameValue(componentName.c_str(), allocator);
		componentValue.AddMember(componentNameValue, i, allocator);

		// ������Ʈ ���� ����
		Com->SaveToLevelJsonFile(componentValue, allocator);

		components.PushBack(componentValue, allocator);
	}
	gameObjectValue.AddMember("components", components, allocator);

	// ��ũ��Ʈ ����
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


	// �ڽ� ������Ʈ ����
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
	// JSON ���� ��� ����
	wstring jsonPath = CPathMgr::GetInst()->GetContentPath() + _LevelPath;

	// JSON ���� ����
	ifstream file(jsonPath);
	if (!file.is_open())
	{
		wchar_t szStr[256] = {};
		wsprintf(szStr, L"Failed to open JSON file.");
		MessageBox(nullptr, szStr, L"Level Load ����.", MB_OK);
		// ���� ���� ����
		return nullptr;
	}
	// JSON ������ �б�
	string jsonData((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());

	// JSON ������ �Ľ�
	Document document;
	document.Parse(jsonData.c_str());
	if (document.HasParseError())
	{
		wchar_t szStr[256] = {};
		wsprintf(szStr, L"JSON ���� �Ľ� ����.");
		MessageBox(nullptr, szStr, L"Level Load ����.", MB_OK);
		// �Ľ� ����
		return nullptr;
	}

	//Camera�� �����ڿ��� RenderMgr�� ����Ҷ�� ���� ������ RenderMgr�� �ʱ�ȭ�Ǿ����
	CRenderMgr::GetInst()->ClearCamera();

	// CLevel ��ü ����
	CLevel* NewLevel = new CLevel();

	// ���� �̸� �ε�
	if (document.HasMember("levelName"))
	{
		const Value& levelNameValue = document["levelName"];
		if (levelNameValue.IsString())
		{
			wstring levelName = StrToWStr(levelNameValue.GetString());
			NewLevel->SetName(levelName);
		}
	}

	// ���̾�� �ε�
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
					// ���̾� ����
					CLayer* pLayer = NewLevel->GetLayer(i);

					// ���̾� �̸� �ε�
					if (layerValue.HasMember("layerName"))
					{
						const Value& layerNameValue = layerValue["layerName"];
						if (layerNameValue.IsString())
						{
							wstring layerName = StrToWStr(layerNameValue.GetString());
							pLayer->SetName(layerName);
						}
					}

					// ���� ������Ʈ�� �ε�
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
									// ���� ������Ʈ ����
									CGameObject* gameObject = new CGameObject();

									// ���� ������Ʈ ���� �ε�
									LoadGameObjectFromJson(gameObjectValue, gameObject);

									// ���̾ ���� ������Ʈ �߰�
									NewLevel->AddGameObject(gameObject, i, false);
								}
							}
						}
					}

				}
			}
		}
	}

	// ������Ʈ �浹 ���� �ε�
	UINT matrix[MAX_LAYER];
	const Value& CollisionMatrixArray = document["CollisionMatrix"];
	size_t CollisionMatrixArraySize = CollisionMatrixArray.Size();
	for (size_t i = 0; i < CollisionMatrixArraySize; ++i) {
		matrix[i] = CollisionMatrixArray[i].GetUint();
	}
	CCollisionMgr::GetInst()->SetMatrix(matrix);

	// ���� �ݱ�
	file.close();

	return NewLevel;
}

CGameObject* CLevelSaveLoad::LoadGameObjectFromJson(const Value& _gameObjectValue, CGameObject* _gameObject)
{
	// ���� ������Ʈ �̸� �ε�
	if (_gameObjectValue.HasMember("ObjName"))
	{
		const Value& objNameValue = _gameObjectValue["ObjName"];
		if (objNameValue.IsString())
		{
			std::wstring objName = StrToWStr(objNameValue.GetString());
			_gameObject->SetName(objName);
		}
	}

	// ������Ʈ �ε�
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

					// ������Ʈ �ε� �� �߰�
					// ������Ʈ ������ ���� Ȯ��
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

	// ��ũ��Ʈ load
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

	// �ڽ� ������Ʈ load
	const Value& childrenValue = _gameObjectValue["children"];
	for (SizeType i = 0; i < childrenValue.Size(); ++i)
	{
		const Value& childValue = childrenValue[i];
		if (childValue.IsObject())
		{
			// ���� ������Ʈ ����
			CGameObject* childGameObject = new CGameObject();
			LoadGameObjectFromJson(childValue, childGameObject);
			_gameObject->AddChild(childGameObject);
		}
	}

	return _gameObject;
}

