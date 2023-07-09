#pragma once

class CLevel;
class CLayer;
class CGameObject;


class CLevelSaveLoad
{
public:
	static int SaveLevel(const wstring& _LevelPath, CLevel* _Level);
	static int SaveGameObject(CGameObject* _Object, FILE* _File);

	static CLevel* LoadLevel(const wstring& _LevelPath);
	static CGameObject* LoadGameObject(FILE* _File);

	static int SaveLevelToJson(const wstring& levelPath, CLevel* level);
	static int SaveGameObjectToJson(CGameObject* gameObject, Document::AllocatorType& allocator, Value& value);

	static CLevel* LoadLevelFromJson(const wstring& _LevelPath);
	static CGameObject* LoadGameObjectFromJson(const Value& _gameObjectValue, CGameObject* _gameObject);
};

