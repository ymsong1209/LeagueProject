#pragma once



struct tVertex
{
	Vec3 vPos;
	Vec4 vColor;
	Vec2 vUV;

	// 방향 벡터
	Vec3 vTangent;	// 접선 벡터
	Vec3 vNormal;	// 법선 벡터
	Vec3 vBinormal; // 종법선 벡터

	// Animation 가중치 및 인덱스
	Vec4 vWeights;
	Vec4 vIndices;
};

typedef tVertex Vtx;


// Event
struct tEvent
{
	EVENT_TYPE	Type;
	DWORD_PTR	wParam;
	DWORD_PTR	lParam;
};


struct tDebugShapeInfo
{
	SHAPE_TYPE	eShape;
	Matrix		matWorld;
	Vec3		vWorldPos;
	Vec3		vWorldScale;
	Vec3		vWorldRotation;
	Vec4		vColor;
	float		fMaxTime;
	float		fCurTime;
};


struct tDebugBoundingInfo
{
	Vec3		vWorldPos;
	float		fBounding;
	float		fMaxTime;
	float		fCurTime;
};

struct tLightColor
{
	Vec4 vDiffuse;	// 빛의 색상
	Vec4 vAmbient;	// 주변 광(환경 광)
	Vec4 vSpecular; // 반사광 세기
};

// LightInfo
struct tLightInfo
{
	tLightColor Color;		 // 빛의 색상

	Vec4		vWorldPos;   // 광원의 월드 스페이스 위치
	Vec4		vWorldDir;	 // 빛을 보내는 방향

	UINT		LightType;   // 빛의 타입(방향성, 점, 스포트)
	float		Radius;		 // 빛의 반경(사거리)
	float		Angle;		 // 빛의 각도
	float		InnerAngle;
};


// TileMap
struct tTile
{
	Vec2 vLeftTop;
	Vec2 vSlice;
};

// Animator2D
struct tAnim2DFrm
{
	Vec2	LeftTopUV;
	Vec2	SliceUV;
	Vec2	OffsetUV;
	float	fDuration;

	// DynamicTransform
	Vec3	DynamicPos;
	Vec3	DynamicScale;
	Vec3	DynamicRot;
};


// Particle
struct tParticle
{
	Vec4	vLocalPos;		// 오브젝트로부터 떨어진 거리
	Vec4	vWorldPos;		// 파티클 최종 월드위치
	Vec4	vWorldScale;	// 파티클 크기	
	Vec4	vColor;			// 파티클 색상
	Vec4	vVelocity;		// 파티클 현재 속도
	Vec4	vForce;			// 파티클에 주어진 힘
	Vec4	vRandomForce;	// 파티클에 적용되는 랜덤방향 힘

	float   Age;			// 생존 시간
	float   PrevAge;		// 이전 프레임 생존 시간
	float   NormalizedAge;	// 수명대비 생존시간을 0~1로 정규화 한 값
	float	LifeTime;		// 수명
	float	Mass;			// 질량
	float   ScaleFactor;	// 추가 크기 배율

	int     Active;			// 파티클 활성화 여부
	int     pad;
};

struct tRWParticleBuffer
{	
	int		SpawnCount;			// 스폰 시킬 파티클 개수
	int		padding[3];
};


struct tParticleModule
{
	// 스폰 모듈
	Vec4    vSpawnColor;
	Vec4	vSpawnScaleMin;
	Vec4	vSpawnScaleMax;
	Vec3	vBoxShapeScale;	
	float	fSphereShapeRadius;	
	int		SpawnShapeType;		// 0 : BOX, 1 : Sphere
	int		SpawnRate;			// 초당 생성 개수
	int		Space;				// 파티클 업데이트 좌표계 ( 0 : World,  1 : Local)
	float   MinLifeTime;		// 최소 수명
	float   MaxLifeTime;		// 최대 수명
	int     spawnpad[3];

	// Color Change 모듈
	Vec4	vStartColor;		// 초기 색상
	Vec4	vEndColor;			// 최종 색상

	// Scale Change 모듈
	float	StartScale;			// 초기 배율
	float	EndScale;			// 최종 배율	
	

	// 버퍼 최대크기
	int		iMaxParticleCount;
	int		ipad;

	// Add Velocity 모듈
	Vec4	vVelocityDir;
	int     AddVelocityType;	// 0 : From Center, 1: To Center, 2 : Fixed Direction	
	float	OffsetAngle;		
	float	Speed;
	int     addvpad;

	// Drag 모듈 - 속도 제한
	float	StartDrag;
	float	EndDrag;

	// NoiseForce 모듈 - 랜덤 힘 적용	
	float	fNoiseTerm;		// 랜덤 힘 변경 간격
	float	fNoiseForce;	// 랜덤 힘 크기

	// Gravity 모듈 - 중력 추가
	float	fGravityForce;
	float	GravityPad[3];

	// Render 모듈
	int		VelocityAlignment;	// 1 : 속도정렬 사용(이동 방향으로 회전) 0 : 사용 안함
	int		VelocityScale;		// 1 : 속도에 따른 크기 변화 사용, 0 : 사용 안함	
	int		AnimationUse;		// 1 : 애니메이션 사용, 0 : 사용 안함
	int		AnimationLoop;		// 1 : 애니메이션 루프, 0 : 수명에 맞게 한번만
	int     bRotate;			// 1 : 회전함, 0 : 회전안함

	float   vMaxSpeed;			// 최대 크기에 도달하는 속력
	float   fRotationAngle;
	float   fRotateSpeed;
	Vec4	vMaxVelocityScale;	// 속력에 따른 크기 변화량 최대치

	int		iAnimXCount;
	int		iAnimYCount;
	float	fAnimFrmTime;

	int		renderpad;

	// Module Check
	int		ModuleCheck[(UINT)PARTICLE_MODULE::END];
};


// 광선 구조체
struct tRay
{
	Vec3 vStart;
	Vec3 vDir;
};

// Raycast 결과를 받을 구조체
struct tRaycastOut
{
	Vec2 vUV;
	int  iDist;
	int  bSuccess;
};


// Intersect 결과 받을 구조체 (Custom)
struct IntersectResult
{
	Vec3  vCrossPoint;
	float fResult;
	bool  bResult;
};


struct tWeight_4
{
	float arrWeight[4];
};


// Material 계수
struct tMtrlData
{
	Vec4 vDiff;
	Vec4 vSpec;
	Vec4 vAmb;
	Vec4 vEmv;
};

// ============
// Animation 3D
// ============
struct tFrameTrans
{
	Vec4	vTranslate;
	Vec4	vScale;
	Vec4	qRot;
};

struct tMTKeyFrame
{
	double	dTime;
	int		iFrame;
	Vec3	vTranslate;
	Vec3	vScale;
	Vec4	qRot;
};


struct tMTBone
{
	wstring				strBoneName;
	int					iDepth;
	int					iParentIndx;
	Matrix				matOffset;	// Offset 행렬(뼈 -> 루트 까지의 행렬)
	Matrix				matBone;   // 이거 안씀
	vector<tMTKeyFrame>	vecKeyFrame;
};

struct tMTAnimClip
{
	wstring			strAnimName;
	int				iStartFrame;
	int				iEndFrame;
	int				iFrameLength;

	double			dStartTime;
	double			dEndTime;
	double			dTimeLength;
	float			fUpdateTime; // 이거 안씀

	FbxTime::EMode	eMode;
};


// ===================
// 상수버퍼 대응 구조체
// ===================
struct tTransform
{
	Matrix matWorld;
	Matrix matWorldInv;
	Matrix matView;
	Matrix matViewInv;
	Matrix matProj;

	Matrix matWV;
	Matrix matWVP;
};

extern tTransform g_transform;


struct tMtrlConst
{
	tMtrlData mtrl;

	int arrInt[4];
	float arrFloat[4];
	Vec2 arrV2[4];
	Vec4 arrV4[4];
	Matrix arrMat[4];

	int arrTex[(UINT)TEX_PARAM::TEX_END];

	int	arrAnimData[4];	// 3D Animation 정보
};


struct tGlobal
{
	Vec2  Resolution;
	float tDT;				//play도중일때 한 프레임당 흐르는 시간
	float tAccTime;			//프로그램이 실행되고 나서 쌓인 총 시간

	UINT  Light2DCount;
	UINT  Light3DCount;
	float tEditDT;			//Play,Pause상관없이 흐르는 DT
	int	  globalpadding;
};

extern tGlobal GlobalData;



