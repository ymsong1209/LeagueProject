#pragma once



// 오브젝트 생성
class CGameObject;
void SpawnGameObject(CGameObject* _NewObject, Vec3 _vWorldPos, int _LayerIdx);
void SpawnGameObject(CGameObject* _NewObject, Vec3 _vWorldPos, const wstring& _LayerName);

// 오브젝트 삭제
void DestroyObject(CGameObject* _DeletObject);


// DrawDebugShape
void DrawDebugRect(Vec3 _vWorldPos, Vec2 _vWorldScale, Vec4 _vColor, Vec3 _vRotation, float _fTime = 0.f);
void DrawDebugRect(const Matrix& _matWorld, Vec4 _vColor, float _fTime = 0.f);

void DrawDebugCircle(Vec3 _vWorldPos, float _fRadius, Vec4 _vColor, Vec3 _vRotation, float _fTime = 0.f);
void DrawDebugCircle(const Matrix& _matWorld, Vec4 _vColor, float _fTime = 0.f);

void DrawDebugCube(Vec3 _vWorldPos, Vec2 _vWorldScale, Vec4 _vColor, Vec3 _vRotation, float _fTime = 0.f);
void DrawDebugCube(const Matrix& _matWorld, Vec4 _vColor, float _fTime = 0.f);

void DrawDebugSphere(Vec3 _vWorldPos, float _fRadius, Vec4 _vColor, Vec3 _vRotation, float _fTime = 0.f);
void DrawDebugSphere(const Matrix& _matWorld, Vec4 _vColor, float _fTime = 0.f);

void DrawDebugIceCream(Vec3 _vWorldPos, Vec3 _WorldScale, Vec4 _vColor, Vec3 _vRotation, float _fTime = 0.f);
void DrawDebugIceCream(const Matrix& _matWorld, Vec4 _vColor, float _fTime = 0.f);

void DrawDebugFrustum(Vec3 _vWorldPos, Vec2 _vWorldScale, Vec4 _vColor, Vec3 _vRotation, float _fTime);


Vec3 DecomposeRotMat(const Matrix& _matRot);


const char* ToString(RES_TYPE);
const wchar_t* ToWSTring(RES_TYPE);

const char* ToString(COMPONENT_TYPE);
const wchar_t* ToWSTring(COMPONENT_TYPE);

 
// Relative Path 가져오기
wstring GetRelativePath(const wstring& _strBase, const wstring& _strPath);

// FbxMatrix -> Matrix
Matrix GetMatrixFromFbxMatrix(FbxAMatrix& _mat);




// Save / Load
void SaveWString(const wstring& _str, FILE* _File);
void LoadWString(wstring& _str, FILE* _File);

class CRes;
template<typename T>
class Ptr;

void SaveResRef(Ptr<CRes> _Res, FILE* _File);

class CResMgr;
template<typename T>
void LoadResRef(Ptr<T>& _Res, FILE* _File)
{
	int i = 0;	
	fread(&i, sizeof(i), 1, _File);
	
	if (i)
	{
		wstring strKey, strRelativePath;
		LoadWString(strKey, _File);
		LoadWString(strRelativePath, _File);

		_Res = CResMgr::GetInst()->Load<T>(strKey, strRelativePath);
	}
}

// Json Save / Load
string wStrToStr(wstring _before);
wstring StrToWStr(string _before);
Value SaveWStringJson(wstring _before, Document::AllocatorType& allocator);

Value SaveVec2Json(Vec2 _input, Document::AllocatorType& allocator);
Value SaveVec3Json(Vec3 _input, Document::AllocatorType& allocator);
Value SaveVec4Json(Vec4 _input, Document::AllocatorType& allocator);

Vec2 LoadVec2Json(const Value& _vec2Value);
Vec3 LoadVec3Json(const Value& _vec3Value);
Vec4 LoadVec4Json(const Value& _vec4Value);

Value& SaveResRefJson(Ptr<CRes> _Res, Document::AllocatorType& allocator);

template<typename T>
void LoadResRefJson(Ptr<T>& _Res, const Value& _value)
{
	if (!_value["IsNull"].GetBool())
	{
		wstring strKey, strRelativePath;
		strKey = StrToWStr(_value["Key"].GetString());
		strRelativePath = StrToWStr(_value["RelativePath"].GetString());

		_Res = CResMgr::GetInst()->Load<T>(strKey, strRelativePath);
	}
}






template<typename T, UINT Size>
void Safe_Del_Array(T* (&arr)[Size])
{
	for (UINT i = 0; i < Size; ++i)
	{
		if (nullptr != arr[i])
			delete arr[i];
	}	
}

template<typename T>
void Safe_Del_Vec(vector<T*>& _vec)
{
	for (size_t i = 0; i < _vec.size(); ++i)
	{
		if (nullptr != _vec[i])
		{
			delete _vec[i];
		}
	}
	_vec.clear();
}

template<typename T1, typename T2>
void Safe_Del_Map(map<T1, T2>& _map)
{
	for (const auto& pair : _map)
	{
		if (nullptr != pair.second)
			delete pair.second;
	}

	_map.clear();
}

//문자열내에 |문자를 -로 바꾸는 함수
//파일저장시에 |를 저장 못함
wstring ReplacePipeCharacter(const std::wstring& _mywstring);

