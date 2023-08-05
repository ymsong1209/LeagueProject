#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#include "pch.h"

#include "CEventMgr.h"
#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"
#include "CTransform.h"
#include "CRenderMgr.h"
#include "ptr.h"
#include "CResMgr.h"
#include "func.h"

void SpawnGameObject(CGameObject* _NewObject, Vec3 _vWorldPos, int _LayerIdx)
{
	_NewObject->Transform()->SetRelativePos(_vWorldPos);

	tEvent evn = {};

	evn.Type = EVENT_TYPE::CREATE_OBJECT;
	evn.wParam = (DWORD_PTR)_NewObject;
	evn.lParam = _LayerIdx;

	CEventMgr::GetInst()->AddEvent(evn);
}

void SpawnGameObject(CGameObject* _NewObject, Vec3 _vWorldPos, const wstring& _LayerName)
{
	_NewObject->Transform()->SetRelativePos(_vWorldPos);

	tEvent evn = {};

	evn.Type = EVENT_TYPE::CREATE_OBJECT;
	evn.wParam = (DWORD_PTR)_NewObject;
	evn.lParam = CLevelMgr::GetInst()->GetCurLevel()->FindLayerByName(_LayerName)->GetLayerIndex();

	CEventMgr::GetInst()->AddEvent(evn);
}

void DestroyObject(CGameObject* _DeletObject)
{
	if (_DeletObject->IsDead())
		return;

	tEvent evn = {};

	evn.Type = EVENT_TYPE::DELETE_OBJECT;
	evn.wParam = (DWORD_PTR)_DeletObject;
	
	CEventMgr::GetInst()->AddEvent(evn);
}

void ScriptInspectorReload()
{
	tEvent evn = {};
	evn.Type = EVENT_TYPE::INSPECTOR_RELOAD;
	CEventMgr::GetInst()->AddEvent(evn);
}



void DrawDebugRect(Vec3 _vWorldPos, Vec2 _vWorldScale, Vec4 _vColor, Vec3 _vRotation, float _fTime)
{
	tDebugShapeInfo info = {};

	info.matWorld = XMMatrixIdentity();
	info.eShape = SHAPE_TYPE::RECT;
	info.fMaxTime = _fTime;
	info.vWorldPos = _vWorldPos;
	info.vWorldScale = Vec3(_vWorldScale.x, _vWorldScale.y, 1.f);
	info.vWorldRotation = _vRotation;
	info.vColor = _vColor;

	CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void DrawDebugRect(const Matrix& _matWorld, Vec4 _vColor, float _fTime)
{
	tDebugShapeInfo info = {};

	info.matWorld = _matWorld;
	info.eShape = SHAPE_TYPE::RECT;
	info.fMaxTime = _fTime;	
	info.vColor = _vColor;

	CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void DrawDebugCircle(Vec3 _vWorldPos, float _fRadius, Vec4 _vColor, Vec3 _vRotation, float _fTime)
{
	tDebugShapeInfo info = {};

	info.matWorld = XMMatrixIdentity();
	info.eShape = SHAPE_TYPE::CIRCLE;
	info.fMaxTime = _fTime;
	info.vWorldPos = _vWorldPos;
	info.vWorldScale = Vec3(_fRadius, _fRadius, 1.f);
	info.vWorldRotation = _vRotation;
	info.vColor = _vColor;

	CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void DrawDebugCircle(const Matrix& _matWorld, Vec4 _vColor, float _fTime)
{
	tDebugShapeInfo info = {};

	info.matWorld = _matWorld;
	info.eShape = SHAPE_TYPE::CIRCLE;
	info.fMaxTime = _fTime;	
	info.vColor = _vColor;

	CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void DrawDebugCube(Vec3 _vWorldPos, Vec2 _vWorldScale, Vec4 _vColor, Vec3 _vRotation, float _fTime)
{
	tDebugShapeInfo info = {};

	info.matWorld = XMMatrixIdentity();
	info.eShape = SHAPE_TYPE::CUBE;
	info.fMaxTime = _fTime;
	info.vWorldPos = _vWorldPos;
	info.vWorldScale = Vec3(_vWorldScale.x, _vWorldScale.y, 1.f);
	info.vWorldRotation = _vRotation;
	info.vColor = _vColor;

	CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void DrawDebugCube(const Matrix& _matWorld, Vec4 _vColor, float _fTime)
{
	tDebugShapeInfo info = {};

	info.matWorld = _matWorld;
	info.eShape = SHAPE_TYPE::CUBE;
	info.fMaxTime = _fTime;
	info.vColor = _vColor;

	CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void DrawDebugSphere(Vec3 _vWorldPos, float _fRadius, Vec4 _vColor, Vec3 _vRotation, float _fTime)
{
	tDebugShapeInfo info = {};

	info.matWorld = XMMatrixIdentity();
	info.eShape = SHAPE_TYPE::SPHERE;
	info.fMaxTime = _fTime;
	info.vWorldPos = _vWorldPos;
	info.vWorldScale = Vec3(_fRadius, _fRadius, _fRadius);
	info.vWorldRotation = _vRotation;
	info.vColor = _vColor;

	CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void DrawDebugSphere(const Matrix& _matWorld, Vec4 _vColor, float _fTime)
{
	tDebugShapeInfo info = {};

	info.matWorld = _matWorld;
	info.eShape = SHAPE_TYPE::SPHERE;
	info.fMaxTime = _fTime;
	info.vColor = _vColor;

	CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void DrawDebugIceCream(Vec3 _vWorldPos, Vec3 _WorldScale, Vec4 _vColor, Vec3 _vRotation, float _fTime)
{
	tDebugShapeInfo info = {};


	info.matWorld = XMMatrixIdentity();
	info.eShape = SHAPE_TYPE::ICECREAM;
	info.fMaxTime = _fTime;
	info.vWorldPos = _vWorldPos;
	info.vWorldScale = _WorldScale;
	info.vWorldRotation = _vRotation;
	info.vColor = _vColor;


	CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void DrawDebugIceCream(const Matrix& _matWorld, Vec4 _vColor, float _fTime)
{
	tDebugShapeInfo info = {};

	info.matWorld = _matWorld;
	info.eShape = SHAPE_TYPE::ICECREAM;
	info.fMaxTime = _fTime;
	info.vColor = _vColor;

	CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}
void DrawDebugFrustum(Vec3 _vWorldPos, Vec2 _vWorldScale, Vec4 _vColor, Vec3 _vRotation, float _fTime)
{
	tDebugShapeInfo info = {};

	info.matWorld = XMMatrixIdentity();
	info.eShape = SHAPE_TYPE::FRUSTUM;
	info.fMaxTime = _fTime;
	info.vWorldPos = _vWorldPos;
	info.vWorldScale = Vec3(_vWorldScale.x, _vWorldScale.y, 1.f);
	info.vWorldRotation = _vRotation;
	info.vColor = _vColor;

	CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}


const char* ToString(RES_TYPE type)
{
	return RES_TYPE_STR[(UINT)type];
}

const wchar_t* ToWString(RES_TYPE type)
{
	return RES_TYPE_WSTR[(UINT)type];
}


const char* ToString(COMPONENT_TYPE type)
{
	return COMPONENT_TYPE_STR[(UINT)type];
}

wstring GetRelativePath(const wstring& _strBase, const wstring& _strPath)
{
	wstring strRelativePath;
	if (-1 == _strPath.find(_strBase))
	{
		return strRelativePath;
	}

	strRelativePath = _strPath.substr(_strBase.length(), _strPath.length());
	return strRelativePath;
}

Matrix GetMatrixFromFbxMatrix(FbxAMatrix& _mat)
{
	Matrix mat;
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			mat.m[i][j] = (float)_mat.Get(i, j);
		}
	}
	return mat;
}

void SaveWString(const wstring& _str, FILE* _File)
{	
	UINT iLen = (UINT)_str.length();
	fwrite(&iLen, sizeof(UINT), 1, _File);
	fwrite(_str.c_str(), sizeof(wchar_t), _str.length(), _File);
}

void LoadWString(wstring& _str, FILE* _File)
{
	wchar_t szBuffer[256] = {};

	UINT iLen = 0;
	fread(&iLen, sizeof(UINT), 1, _File);
	fread(szBuffer, sizeof(wchar_t), iLen, _File);	

	_str = szBuffer;
}

void SaveResRef(Ptr<CRes> _Res, FILE* _File)
{
	int i = 0;
	if (nullptr == _Res)
	{
		fwrite(&i, sizeof(i), 1, _File);
	}
	else
	{
		i = 1;
		fwrite(&i, sizeof(i), 1, _File);
		SaveWString(_Res->GetKey(), _File);
		SaveWString(_Res->GetRelativePath(), _File);
	}
}

const wchar_t* ToWString(COMPONENT_TYPE type)
{
	return COMPONENT_TYPE_WSTR[(UINT)type];
}




bool closeEnough(const float& a, const float& b
	, const float& epsilon = std::numeric_limits<float>::epsilon())
{
	return (epsilon > std::abs(a - b));
}

Vec3 DecomposeRotMat(const Matrix& _matRot)
{
	// _mat 을 분해 후 다시 행렬 만들기	
	Vec4 vMat[4];

	vMat[0] = Vec4(_matRot._11, _matRot._12, _matRot._13, _matRot._14);
	vMat[1] = Vec4(_matRot._21, _matRot._22, _matRot._23, _matRot._24);
	vMat[2] = Vec4(_matRot._31, _matRot._32, _matRot._33, _matRot._34);
	vMat[3] = Vec4(_matRot._41, _matRot._42, _matRot._43, _matRot._44);

	/*XMStoreFloat4(&vMat[0], _matRot._11);
	XMStoreFloat4(&vMat[1], _matRot.r[1]);
	XMStoreFloat4(&vMat[2], _matRot.r[2]);
	XMStoreFloat4(&vMat[3], _matRot.r[3]);*/

	Vec3 vNewRot;
	if (closeEnough(vMat[0].z, -1.0f)) {
		float x = 0; //gimbal lock, value of x doesn't matter
		float y = XM_PI / 2;
		float z = x + atan2f(vMat[1].x, vMat[2].x);
		vNewRot = Vec3{ x, y, z };
	}
	else if (closeEnough(vMat[0].z, 1.0f)) {
		float x = 0;
		float y = -XM_PI / 2;
		float z = -x + atan2f(-vMat[1].x, -vMat[2].x);
		vNewRot = Vec3{ x, y, z };
	}
	else { //two solutions exist
		float y1 = -asinf(vMat[0].z);
		float y2 = XM_PI - y1;

		float x1 = atan2f(vMat[1].z / cosf(y1), vMat[2].z / cosf(y1));
		float x2 = atan2f(vMat[1].z / cosf(y2), vMat[2].z / cosf(y2));

		float z1 = atan2f(vMat[0].y / cosf(y1), vMat[0].x / cosf(y1));
		float z2 = atan2f(vMat[0].y / cosf(y2), vMat[0].x / cosf(y2));

		//choose one solution to return
		//for example the "shortest" rotation
		if ((std::abs(x1) + std::abs(y1) + std::abs(z1)) <= (std::abs(x2) + std::abs(y2) + std::abs(z2)))
		{
			vNewRot = Vec3{ x1, y1, z1 };
		}
		else {
			vNewRot = Vec3{ x2, y2, z2 };
		}
	}
	return vNewRot;
}

wstring ReplacePipeCharacter(const std::wstring& _mywstring)
{
	std::wstring replacedFileName = _mywstring;
	size_t found = replacedFileName.find(L"|");
	while (found != std::wstring::npos) {
		replacedFileName.replace(found, 1, L"-");
		found = replacedFileName.find(L"|", found + 1);
	}
	return replacedFileName;
}

string wStrToStr(wstring _before)
{
	return string(_before.begin(), _before.end());
}

wstring StrToWStr(string _before)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	return converter.from_bytes(_before);
}

Value SaveWStringJson(wstring _before, Document::AllocatorType& allocator)
{
	Value vTemp(kStringType);
	vTemp.SetString(wStrToStr(_before).c_str(), allocator);
	return vTemp;
}


Value SaveVec2Json(Vec2 _input, Document::AllocatorType& allocator)
{
	Value vTemp(kObjectType);

	vTemp.AddMember("x", _input.x, allocator);
	vTemp.AddMember("y", _input.y, allocator);

	return vTemp;
}

Value SaveVec3Json(Vec3 _input, Document::AllocatorType& allocator)
{
	Value vTemp(kObjectType);

	vTemp.AddMember("x", _input.x, allocator);
	vTemp.AddMember("y", _input.y, allocator);
	vTemp.AddMember("z", _input.z, allocator);

	return vTemp;
}

Value SaveVec4Json(Vec4 _input, Document::AllocatorType& allocator)
{
	Value vTemp(kObjectType);

	vTemp.AddMember("x", _input.x, allocator);
	vTemp.AddMember("y", _input.y, allocator);
	vTemp.AddMember("z", _input.z, allocator);
	vTemp.AddMember("w", _input.w, allocator);

	return vTemp;
}

Value SaveMatrixJson(Matrix& _input, Document::AllocatorType& allocator)
{
	Value matrixObject(kObjectType);

	// 각 요소를 저장
	matrixObject.AddMember("m00", _input._11, allocator);
	matrixObject.AddMember("m01", _input._12, allocator);
	matrixObject.AddMember("m02", _input._13, allocator);
	matrixObject.AddMember("m03", _input._14, allocator);
	matrixObject.AddMember("m10", _input._21, allocator);
	matrixObject.AddMember("m11", _input._22, allocator);
	matrixObject.AddMember("m12", _input._23, allocator);
	matrixObject.AddMember("m13", _input._24, allocator);
	matrixObject.AddMember("m20", _input._31, allocator);
	matrixObject.AddMember("m21", _input._32, allocator);
	matrixObject.AddMember("m22", _input._33, allocator);
	matrixObject.AddMember("m23", _input._34, allocator);
	matrixObject.AddMember("m30", _input._41, allocator);
	matrixObject.AddMember("m31", _input._42, allocator);
	matrixObject.AddMember("m32", _input._43, allocator);
	matrixObject.AddMember("m33", _input._44, allocator);

	return matrixObject;
}

Vec2 LoadVec2Json(const Value& _vec2Value)
{
	Vec2 vTemp;

	if (_vec2Value.IsObject())
	{
		vTemp.x = _vec2Value["x"].GetFloat();
		vTemp.y = _vec2Value["y"].GetFloat();
	}
	return vTemp;
}

Vec3 LoadVec3Json(const Value& _vec3Value)
{
	Vec3 vTemp;

	if (_vec3Value.IsObject())
	{
		vTemp.x = _vec3Value["x"].GetFloat();
		vTemp.y = _vec3Value["y"].GetFloat();
		vTemp.z = _vec3Value["z"].GetFloat();
	}
	return vTemp;
}

Vec4 LoadVec4Json(const Value& _vec4Value)
{
	Vec4 vTemp;

	if (_vec4Value.IsObject())
	{
		vTemp.x = _vec4Value["x"].GetFloat();
		vTemp.y = _vec4Value["y"].GetFloat();
		vTemp.z = _vec4Value["z"].GetFloat();
		vTemp.w = _vec4Value["w"].GetFloat();
	}
	return vTemp;
}

Matrix LoadMatrixJson(const Value& _MatrixValue)
{
	Matrix matrix;

	matrix._11 = _MatrixValue["m00"].GetFloat();
	matrix._12 = _MatrixValue["m01"].GetFloat();
	matrix._13 = _MatrixValue["m02"].GetFloat();
	matrix._14 = _MatrixValue["m03"].GetFloat();
	matrix._21 = _MatrixValue["m10"].GetFloat();
	matrix._22 = _MatrixValue["m11"].GetFloat();
	matrix._23 = _MatrixValue["m12"].GetFloat();
	matrix._24 = _MatrixValue["m13"].GetFloat();
	matrix._31 = _MatrixValue["m20"].GetFloat();
	matrix._32 = _MatrixValue["m21"].GetFloat();
	matrix._33 = _MatrixValue["m22"].GetFloat();
	matrix._34 = _MatrixValue["m23"].GetFloat();
	matrix._41 = _MatrixValue["m30"].GetFloat();
	matrix._42 = _MatrixValue["m31"].GetFloat();
	matrix._43 = _MatrixValue["m32"].GetFloat();
	matrix._44 = _MatrixValue["m33"].GetFloat();

	return matrix;
}

Value SavetMtrlConst(tMtrlConst _constData, Document::AllocatorType& allocator)
{
	Value vTemp(kObjectType);

	// tMtrlData mtrl
	Value tMtrlDataValue(kObjectType);
	tMtrlDataValue.AddMember("vDiff", SaveVec4Json(_constData.mtrl.vDiff, allocator), allocator);
	tMtrlDataValue.AddMember("vSpec", SaveVec4Json(_constData.mtrl.vSpec, allocator), allocator);
	tMtrlDataValue.AddMember("vAmb", SaveVec4Json(_constData.mtrl.vAmb, allocator), allocator);
	tMtrlDataValue.AddMember("vEmv", SaveVec4Json(_constData.mtrl.vEmv, allocator), allocator);
	vTemp.AddMember("mtrl", tMtrlDataValue, allocator);

	// int arrInt[12];
	Value arrIntArray(kArrayType);
	for (int i = 0; i < 12; ++i)
	{
		Value ArrIntValue(kObjectType);
		string key = "arrInt[" + std::to_string(i) + "]";
		Value keyName(kStringType);
		keyName.SetString(key.c_str(), key.length(), allocator);
		ArrIntValue.AddMember(keyName, _constData.arrInt[i], allocator);

		arrIntArray.PushBack(ArrIntValue, allocator);
	}
	vTemp.AddMember("arrInt[12]s", arrIntArray, allocator);

	// float arrFloat[12];
	Value arrFloatArray(kArrayType);
	for (int i = 0; i < 12; ++i)
	{
		Value ArrFloatValue(kObjectType);
		string key = "arrFloat[" + std::to_string(i) + "]";
		Value keyName(kStringType);
		keyName.SetString(key.c_str(), key.length(), allocator);
		ArrFloatValue.AddMember(keyName, _constData.arrFloat[i], allocator);

		arrFloatArray.PushBack(ArrFloatValue, allocator);
	}
	vTemp.AddMember("arrFloat[12]s", arrFloatArray, allocator);

	// Vec2 arrV2[12];
	Value arrV2Array(kArrayType);
	for (int i = 0; i < 12; ++i)
	{
		Value ArrV2Value(kObjectType);
		string key = "arrV2[" + std::to_string(i) + "]";
		Value keyName(kStringType);
		keyName.SetString(key.c_str(), key.length(), allocator);
		ArrV2Value.AddMember(keyName, SaveVec2Json(_constData.arrV2[i],allocator), allocator);

		arrV2Array.PushBack(ArrV2Value, allocator);
	}
	vTemp.AddMember("arrV2[12]s", arrV2Array, allocator);

	// Vec4 arrV4[12];
	Value arrV4Array(kArrayType);
	for (int i = 0; i < 12; ++i)
	{
		Value ArrV4Value(kObjectType);
		string key = "arrV4[" + std::to_string(i) + "]";
		Value keyName(kStringType);
		keyName.SetString(key.c_str(), key.length(), allocator);
		ArrV4Value.AddMember(keyName, SaveVec4Json(_constData.arrV4[i], allocator), allocator);

		arrV4Array.PushBack(ArrV4Value, allocator);
	}
	vTemp.AddMember("arrV4[12]s", arrV4Array, allocator);

	// Matrix arrMat[4];
	Value arrMatArray(kArrayType);
	for (int i = 0; i <4; ++i)
	{
		Value ArrMatValue(kObjectType);
		string key = "arrMat[" + std::to_string(i) + "]";
		Value keyName(kStringType);
		keyName.SetString(key.c_str(), key.length(), allocator);
		ArrMatValue.AddMember(keyName, SaveMatrixJson(_constData.arrMat[i], allocator), allocator);

		arrMatArray.PushBack(ArrMatValue, allocator);
	}
	vTemp.AddMember("arrMat[4]s", arrMatArray, allocator);

	// int arrTex[(UINT)TEX_PARAM::TEX_END];
	Value arrTexArray(kArrayType);
	for (int i = 0; i < (UINT)TEX_PARAM::TEX_END; ++i)
	{
		Value ArrTexValue(kObjectType);
		string key = "arrTex[" + std::to_string(i) + "]";
		Value keyName(kStringType);
		keyName.SetString(key.c_str(), key.length(), allocator);
		ArrTexValue.AddMember(keyName, _constData.arrTex[i], allocator);

		arrTexArray.PushBack(ArrTexValue, allocator);
	}
	vTemp.AddMember("arrTex[(UINT)TEX_PARAM::TEX_END]s", arrTexArray, allocator);
	
	// int	arrAnimData[4];
	Value arrAnimDataArray(kArrayType);
	for (int i = 0; i < 4; ++i)
	{
		Value ArrAnimDataValue(kObjectType);
		string key = "arrAnimData[" + std::to_string(i) + "]";
		Value keyName(kStringType);
		keyName.SetString(key.c_str(), key.length(), allocator);
		ArrAnimDataValue.AddMember(keyName, _constData.arrAnimData[i], allocator);

		arrAnimDataArray.PushBack(ArrAnimDataValue, allocator);
	}
	vTemp.AddMember("arrAnimData[4]s", arrAnimDataArray, allocator);

	return vTemp;
}

tMtrlConst LoadtMtrlConst(const Value& _tMtrlConstValue)
{
	tMtrlConst mtrlConst;

	// tMtrlData mtrl
	const Value& tMtrlDataValue = _tMtrlConstValue["mtrl"];
	mtrlConst.mtrl.vDiff = LoadVec4Json(tMtrlDataValue["vDiff"]);
	mtrlConst.mtrl.vSpec = LoadVec4Json(tMtrlDataValue["vSpec"]);
	mtrlConst.mtrl.vAmb = LoadVec4Json(tMtrlDataValue["vAmb"]);
	mtrlConst.mtrl.vEmv = LoadVec4Json(tMtrlDataValue["vEmv"]);

	// int arrInt[12];
	const Value& arrIntArray = _tMtrlConstValue["arrInt[12]s"];
	for (int i = 0; i < arrIntArray.Size(); ++i)
	{
		string key = "arrInt[" + std::to_string(i) + "]";
		Value keyName(kStringType);
		keyName.SetString(key.c_str(), key.length());

		mtrlConst.arrInt[i] = arrIntArray[i][keyName].GetInt();
	}

	// float arrFloat[12];
	const Value& arrFloatArray = _tMtrlConstValue["arrFloat[12]s"];
	for (int i = 0; i < arrFloatArray.Size(); ++i)
	{
		string key = "arrFloat[" + std::to_string(i) + "]";
		Value keyName(kStringType);
		keyName.SetString(key.c_str(), key.length());

		mtrlConst.arrFloat[i] = arrFloatArray[i][keyName].GetFloat();
	}

	// Vec2 arrV2[12];
	const Value& arrV2Array = _tMtrlConstValue["arrV2[12]s"];
	for (int i = 0; i < arrV2Array.Size(); ++i)
	{
		string key = "arrV2[" + std::to_string(i) + "]";
		Value keyName(kStringType);
		keyName.SetString(key.c_str(), key.length());

		mtrlConst.arrV2[i] = LoadVec2Json(arrV2Array[i][keyName]);
	}

	// Vec4 arrV4[12];
	const Value& arrV4Array = _tMtrlConstValue["arrV4[12]s"];
	for (int i = 0; i < arrV4Array.Size(); ++i)
	{
		string key = "arrV4[" + std::to_string(i) + "]";
		Value keyName(kStringType);
		keyName.SetString(key.c_str(), key.length());

		mtrlConst.arrV4[i] = LoadVec4Json(arrV4Array[i][keyName]);
	}

	// Matrix arrMat[4];
	const Value& arrMatArray = _tMtrlConstValue["arrMat[4]s"];
	for (int i = 0; i < arrMatArray.Size(); ++i)
	{
		string key = "arrMat[" + std::to_string(i) + "]";
		Value keyName(kStringType);
		keyName.SetString(key.c_str(), key.length());

		mtrlConst.arrMat[i] = LoadMatrixJson(arrMatArray[i][keyName]);
	}

	// int arrTex[(UINT)TEX_PARAM::TEX_END];
	const Value& arrTexArray = _tMtrlConstValue["arrTex[(UINT)TEX_PARAM::TEX_END]s"];
	for (int i = 0; i < arrTexArray.Size(); ++i)
	{
		string key = "arrTex[" + std::to_string(i) + "]";
		Value keyName(kStringType);
		keyName.SetString(key.c_str(), key.length());

		mtrlConst.arrTex[i] = arrTexArray[i][keyName].GetInt();
	}

	// int arrAnimData[4];
	const Value& arrAnimDataArray = _tMtrlConstValue["arrAnimData[4]s"];
	for (int i = 0; i < arrAnimDataArray.Size(); ++i)
	{
		string key = "arrAnimData[" + std::to_string(i) + "]";
		Value keyName(kStringType);
		keyName.SetString(key.c_str(), key.length());

		mtrlConst.arrAnimData[i] = arrAnimDataArray[i][keyName].GetInt();
	}

	return mtrlConst;
}

Value& SaveResRefJson(Ptr<CRes> _Res, Document::AllocatorType& allocator)
{

	Value vTemp(kObjectType);

	if (nullptr == _Res)
	{
		vTemp.AddMember("IsNull", true, allocator);
	}
	else
	{
		vTemp.AddMember("IsNull", false, allocator);
		vTemp.AddMember("Key", Value(wStrToStr(_Res->GetKey()).c_str(), allocator).Move(), allocator);
		vTemp.AddMember("RelativePath", Value(wStrToStr(_Res->GetRelativePath()).c_str(), allocator).Move(), allocator);
	}
	return vTemp;
}

int GetSizeofFormat(DXGI_FORMAT _eFormat)
{
	int iRetByte = 0;
	switch (_eFormat)
	{
	case DXGI_FORMAT_R32G32B32A32_TYPELESS:
	case DXGI_FORMAT_R32G32B32A32_FLOAT:
	case DXGI_FORMAT_R32G32B32A32_UINT:
	case DXGI_FORMAT_R32G32B32A32_SINT:
		iRetByte = 128;
		break;

	case DXGI_FORMAT_R32G32B32_TYPELESS:
	case DXGI_FORMAT_R32G32B32_FLOAT:
	case DXGI_FORMAT_R32G32B32_UINT:
	case DXGI_FORMAT_R32G32B32_SINT:
		iRetByte = 96;
		break;
	case DXGI_FORMAT_R16G16B16A16_TYPELESS:
	case DXGI_FORMAT_R16G16B16A16_FLOAT:
	case DXGI_FORMAT_R16G16B16A16_UNORM:
	case DXGI_FORMAT_R16G16B16A16_UINT:
	case DXGI_FORMAT_R16G16B16A16_SNORM:
	case DXGI_FORMAT_R16G16B16A16_SINT:
	case DXGI_FORMAT_R32G32_TYPELESS:
	case DXGI_FORMAT_R32G32_FLOAT:
	case DXGI_FORMAT_R32G32_UINT:
	case DXGI_FORMAT_R32G32_SINT:
	case DXGI_FORMAT_R32G8X24_TYPELESS:
	case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
	case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
	case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
		iRetByte = 64;
		break;
	case DXGI_FORMAT_R10G10B10A2_TYPELESS:
	case DXGI_FORMAT_R10G10B10A2_UNORM:
	case DXGI_FORMAT_R10G10B10A2_UINT:
	case DXGI_FORMAT_R11G11B10_FLOAT:
	case DXGI_FORMAT_R8G8B8A8_TYPELESS:
	case DXGI_FORMAT_R8G8B8A8_UNORM:
	case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
	case DXGI_FORMAT_R8G8B8A8_UINT:
	case DXGI_FORMAT_R8G8B8A8_SNORM:
	case DXGI_FORMAT_R8G8B8A8_SINT:
	case DXGI_FORMAT_R16G16_TYPELESS:
	case DXGI_FORMAT_R16G16_FLOAT:
	case DXGI_FORMAT_R16G16_UNORM:
	case DXGI_FORMAT_R16G16_UINT:
	case DXGI_FORMAT_R16G16_SNORM:
	case DXGI_FORMAT_R16G16_SINT:
	case DXGI_FORMAT_R32_TYPELESS:
	case DXGI_FORMAT_D32_FLOAT:
	case DXGI_FORMAT_R32_FLOAT:
	case DXGI_FORMAT_R32_UINT:
	case DXGI_FORMAT_R32_SINT:
	case DXGI_FORMAT_R24G8_TYPELESS:
	case DXGI_FORMAT_D24_UNORM_S8_UINT:
	case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
	case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
	case DXGI_FORMAT_B8G8R8A8_UNORM:
	case DXGI_FORMAT_B8G8R8X8_UNORM:
		iRetByte = 32;
		break;
	case DXGI_FORMAT_R8G8_TYPELESS:
	case DXGI_FORMAT_R8G8_UNORM:
	case DXGI_FORMAT_R8G8_UINT:
	case DXGI_FORMAT_R8G8_SNORM:
	case DXGI_FORMAT_R8G8_SINT:
	case DXGI_FORMAT_R16_TYPELESS:
	case DXGI_FORMAT_R16_FLOAT:
	case DXGI_FORMAT_D16_UNORM:
	case DXGI_FORMAT_R16_UNORM:
	case DXGI_FORMAT_R16_UINT:
	case DXGI_FORMAT_R16_SNORM:
	case DXGI_FORMAT_R16_SINT:
	case DXGI_FORMAT_B5G6R5_UNORM:
	case DXGI_FORMAT_B5G5R5A1_UNORM:
		iRetByte = 16;
		break;
	case DXGI_FORMAT_R8_TYPELESS:
	case DXGI_FORMAT_R8_UNORM:
	case DXGI_FORMAT_R8_UINT:
	case DXGI_FORMAT_R8_SNORM:
	case DXGI_FORMAT_R8_SINT:
	case DXGI_FORMAT_A8_UNORM:
		iRetByte = 8;
		break;
		// Compressed format; http://msdn2.microsoft.com/en-us/library/bb694531(VS.85).aspx
	case DXGI_FORMAT_BC2_TYPELESS:
	case DXGI_FORMAT_BC2_UNORM:
	case DXGI_FORMAT_BC2_UNORM_SRGB:
	case DXGI_FORMAT_BC3_TYPELESS:
	case DXGI_FORMAT_BC3_UNORM:
	case DXGI_FORMAT_BC3_UNORM_SRGB:
	case DXGI_FORMAT_BC5_TYPELESS:
	case DXGI_FORMAT_BC5_UNORM:
	case DXGI_FORMAT_BC5_SNORM:
		iRetByte = 128;
		break;
		// Compressed format; http://msdn2.microsoft.com/en-us/library/bb694531(VS.85).aspx
	case DXGI_FORMAT_R1_UNORM:
	case DXGI_FORMAT_BC1_TYPELESS:
	case DXGI_FORMAT_BC1_UNORM:
	case DXGI_FORMAT_BC1_UNORM_SRGB:
	case DXGI_FORMAT_BC4_TYPELESS:
	case DXGI_FORMAT_BC4_UNORM:
	case DXGI_FORMAT_BC4_SNORM:
		iRetByte = 64;
		break;
		// Compressed format; http://msdn2.microsoft.com/en-us/library/bb694531(VS.85).aspx
	case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:
		iRetByte = 32;
		break;
		// These are compressed, but bit-size information is unclear.
	case DXGI_FORMAT_R8G8_B8G8_UNORM:
	case DXGI_FORMAT_G8R8_G8B8_UNORM:
		iRetByte = 32;
		break;
	case DXGI_FORMAT_UNKNOWN:
	default:
		return -1;
	}

	return iRetByte / 8;
}
