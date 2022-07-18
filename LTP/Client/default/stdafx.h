#pragma once



#include "GameInstance.h"
#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>



#define NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#else
#define NEW new 
//#define NEW new


#endif



#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>




// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
#include "Client_Define.h"

