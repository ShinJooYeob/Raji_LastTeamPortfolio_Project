#pragma once



#define FPS		1.f/60.f



#define PlayerMaxDashPower	30.f 
#define PlayerMaxJumpPower	10.f 
#define PlayerSmallingSize	0.3f 
#define PlayerGiantSize		20.f 
#define PlayerMoveSpeed		5.f 
#define PlayerMaxHP			32 
#define PlayerGrinderCoolTime 2.5f

#define MonsterDmgTime		1.5

#define SceneChangeCopyFrame 2

#define KEYUP(x)		GetSingle(CGameInstance)->Get_DIKeyState(x)& DIS_Up
#define KEYDOWN(x)		GetSingle(CGameInstance)->Get_DIKeyState(x)& DIS_Down
#define KEYPRESS(x)		GetSingle(CGameInstance)->Get_DIKeyState(x)& DIS_Press
#define KEYDOUBLE(x)	GetSingle(CGameInstance)->Get_DIKeyState(x)& DIS_DoubleDown

#define DefalutSunPosition _float4(320.f,64.f,-64.f,0)
#define PXVEC3TOFLOAT3(x) *((_float3*)&x)
#define FLOAT3TOPXVEC3(x) *((PxVec3*)&x)

#define MAT4X4TOPXMAT(x) *((PxMat44*)&x)
#define PXMATTOMAT4x4(x) *((_float4x4*)&x)

#define PXVEC4TOFLOAT4(x) *((_float4*)&x)
#define FLOAT4TOPXVEC4(x) *((PxVec4*)&x)

#define DEBUGVALUE1	GetSingle(CPhysXMgr)->gDebugValue1
#define DEBUGVALUE2	GetSingle(CPhysXMgr)->gDebugValue2
#define DEBUGVALUE3	GetSingle(CPhysXMgr)->gDebugValue3
#define DEBUGVALUE4	GetSingle(CPhysXMgr)->gDebugValue4

#define PARTILCECOUNT		10
#define TRANSDEAD(x) if(x){x->Set_IsOwnerDead(true);}

#define STATIC_EFFECTLOAD(x) GetSingle(CAssimpCreateMgr)->Load_Model_One_ByFBXName(TAG_CP(x), TransformMatrix)


#define  STR_FILEPATH_RESOURCE_L					L"..\\Bin\\Resources"
#define  STR_FILEPATH_FBX_STATIC_L					L"..\\Bin\\Resources\\Mesh\\StaticMesh"
#define  STR_FILEPATH_FBX_DYNAMICSTATIC_L			L"..\\Bin\\Resources\\Mesh\\DynamicMesh"

#define  STR_FILEPATH_PARTICLE_FOLDER_L				L"..\\Bin\\Resources\\Data\\ParicleData"
#define  STR_FILEPATH_PARTICLE_SAVE_L				L"..\\Bin\\Resources\\Data\\ParicleData\\ParticlePathData"


#define  STR_FILEPATH_RESOURCE_DAT_L				L"..\\Bin\\Resources\\Data\\Mesh"
#define  STR_FILEPATH_RESOURCE_PATH_L				L"..\\Bin\\Resources\\PathTxT"

#define  STR_FILEPATH_RESOURCE_STATICDAT_L				L"..\\Bin\\Resources\\PathTxT\\3DPath_Static.txt"
#define  STR_FILEPATH_RESOURCE_DYNAMICDAT_L				L"..\\Bin\\Resources\\PathTxT\\3DPath_Dynamic.txt"
#define  STR_FILEPATH_RESOURCE_FBXDAT_L					L"..\\Bin\\Resources\\PathTxT\\FBX_Dat.txt"

#define STR_FILENAME_3DSTATIC_TXT					L"3DPath_Static.txt"
#define STR_FILENAME_3DDYNAMIC_TXT					L"3DPath_Dynamic.txt"
#define STR_FILENAME_FBXDAT_TXT						L"FBX_Dat.txt"

