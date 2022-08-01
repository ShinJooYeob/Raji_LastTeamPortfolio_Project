#pragma once
#pragma warning(disable : 4099)

static const int g_iWinCX = 1280;
static const int g_iWinCY = 720;
extern HINSTANCE g_hInst;
extern HWND g_hWnd;
extern _double g_fDeltaTime;
extern _bool	g_bIsShowFPS;
extern CGameInstance* g_pGameInstance;
extern _bool g_bShowMouse;


#ifdef _DEBUG
#define GETIMGUI GetSingle(CImguiMgr)
#endif // USE_IMGUI

#define GETPARTICLE GetSingle(CPartilceCreateMgr)

#define MYCOLORA(r,g,b)			_float4(0.rf,0.gf,0.bf,1.f)
#define MYCOLOR(r,g,b,a)		_float4(r,g,b,a)


namespace Client {};
using namespace Client;

#include "Client_Enum.h"
#include "Client_Struct.h"
#include "Client_Macro.h"

#include <PxPhysics.h>
#include <PxPhysicsAPI.h>
using namespace physx;

#include "UtilityMgr.h"
#include "physX\PhysXMgr.h"

