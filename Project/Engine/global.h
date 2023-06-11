#pragma once

#include <vector>
#include <queue>
#include <list>
#include <map>
#include <string>
#include <iterator>
#include <algorithm>
#include <cmath>
#include <unordered_set>
#include <ctime>
#include <functional>

using std::vector;
using std::queue;
using std::priority_queue;
using std::list;
using std::map;
using std::make_pair;
using std::wstring;
using std::string;
using std::unordered_set;
using std::pair;

#include <typeinfo>
#include <assert.h>


// FileSystem
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>
using namespace std::experimental;
using namespace std::experimental::filesystem;



#include <wrl.h>
#include <commdlg.h>
using namespace Microsoft::WRL;

#include <d3d11.h>					// DirectX11
#include <d3dcompiler.h>			// Shader ÄÄÆÄÀÏ
#include <DirectXMath.h>			// DX Math
#include <DirectXPackedVector.h>	

using namespace DirectX;
using namespace DirectX::PackedVector;

#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")


#include "SimpleMath.h"
using namespace DirectX::SimpleMath;

typedef Vector2 Vec2;
typedef Vector3 Vec3;
typedef Vector4 Vec4;


#include "define.h"
#include "struct.h"
#include "func.h"

#include "CSingleton.h"