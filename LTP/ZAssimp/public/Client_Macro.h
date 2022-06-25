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
#define KEYUP(x)		GetSingle(CGameInstance)->Get_DIKeyState(x)& DIS_DoubleDown


