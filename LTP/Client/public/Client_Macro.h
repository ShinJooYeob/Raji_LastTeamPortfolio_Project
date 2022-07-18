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

#define KEYUP(x)		GetSingle(CGameInstance)->Get_DIKeyState(x)& DIS_Up
#define KEYDOWN(x)		GetSingle(CGameInstance)->Get_DIKeyState(x)& DIS_Down
#define KEYPRESS(x)		GetSingle(CGameInstance)->Get_DIKeyState(x)& DIS_Press
#define KEYDOUBLE(x)	GetSingle(CGameInstance)->Get_DIKeyState(x)& DIS_DoubleDown

#define DefalutSunPosition _float4(-64.f,64.f,-64.f,0)
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



