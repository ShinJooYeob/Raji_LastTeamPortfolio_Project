#include "stdafx.h"
#include "..\public\TaikoUI.h"
#include "UI.h"
#include "Scene_MiniGame_JJB.h"

CTaikoUI::CTaikoUI(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CTaikoUI::CTaikoUI(const CTaikoUI & rhs)
	: CGameObject(rhs)
{
}

HRESULT CTaikoUI::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CTaikoUI::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	Ready_Layer_UI();

	return S_OK;
}

_int CTaikoUI::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)return -1;

	if(m_bISStartTime)
		m_TestTime += (_float)fDeltaTime;

	if (m_bIsShowGoodEffectTime)
	{
		m_fShowGoodEffectUITime += (_float)fDeltaTime;
		
		if (m_fShowGoodEffectUITime > 0.15f)
		{
			m_bIsShowGoodEffectTime = false;
			m_fShowGoodEffectUITime = 0.f;

			for (auto& UI : m_vecGoodEffectUI)
				UI->Set_IsDraw(false);
		}
	}

	if (m_bIsShowResultt)
	{
		m_fShowResultTime += (_float)fDeltaTime;

		if (m_fShowResultTime > 2.f)
		{
			m_bIsShowResultt = false;
			m_ResultUI->Set_UIPosition(640.f, 360.f);
			m_fShowResultTime = 0.f;
		}
		else
		{
			_float PosY = g_pGameInstance->Easing(TYPE_BounceOut, -200.f, 360.f, m_fShowResultTime, 2.f);
			m_ResultUI->Set_UIPosition(640.f, PosY);

		}
	}

	if (m_bIsQuestUI)
	{
		m_fQuestTime += (_float)fDeltaTime;

		if (m_fQuestTime > 1.f)
		{
			m_bIsQuestUI = false;
			m_QuestUI->Set_UIPosition(1100.f, 210.f);
			m_fQuestTime = 0.f;
		}
		else
		{
			_float PosX = g_pGameInstance->Easing(TYPE_Linear, 1380.f, 1100.f, m_fQuestTime, 1.f);
			m_QuestUI->Set_UIPosition(PosX, 210.f);
		}

	}

	if (m_bIsShowComboUIOn)
	{
		UseComboUI(fDeltaTime);
	}
	//if (g_pGameInstance->Get_DIKeyState(DIK_T)&DIS_Down)
	//{
	//	m_bISStartTime = true;
	//}
	if (g_pGameInstance->Get_DIKeyState(DIK_F)&DIS_Down)
	{
		for (auto& UI : m_vecButtonUI)
		{
			if (UI->Get_UIName() == TEXT("Taiko_MidleL"))
			{
				UI->TimeDrawStart(0.1f);
			}
		}
		//string ttszLog = "Sec :" + to_string(m_TestTime) + "f,   TYPE_SMALLRED" + "\n";
		//wstring ttDebugLog;
		//ttDebugLog.assign(ttszLog.begin(), ttszLog.end());
		//OutputDebugStringW(ttDebugLog.c_str());
	}

	if (g_pGameInstance->Get_DIKeyState(DIK_J)&DIS_Down)
	{
		for (auto& UI : m_vecButtonUI)
		{
			if (UI->Get_UIName() == TEXT("Taiko_MidleR"))
			{
				UI->TimeDrawStart(0.1f);
			}
		}
		//string ttszLog = "Sec :" + to_string(m_TestTime) + "f,   TYPE_SMALLRED" + "\n";
		//wstring ttDebugLog;
		//ttDebugLog.assign(ttszLog.begin(), ttszLog.end());
		//OutputDebugStringW(ttDebugLog.c_str());
	}

	if (g_pGameInstance->Get_DIKeyState(DIK_D)&DIS_Down)
	{
		for (auto& UI : m_vecButtonUI)
		{
			if (UI->Get_UIName() == TEXT("Taiko_OutSideL"))
			{
				UI->TimeDrawStart(0.1f);
			}
		}
		//string ttszLog = "Sec :" + to_string(m_TestTime) + "f,   TYPE_SMALLBLUE" + "\n";
		//wstring ttDebugLog;
		//ttDebugLog.assign(ttszLog.begin(), ttszLog.end());
		//OutputDebugStringW(ttDebugLog.c_str());
	}

	if (g_pGameInstance->Get_DIKeyState(DIK_K)&DIS_Down)
	{
		for (auto& UI : m_vecButtonUI)
		{
			if (UI->Get_UIName() == TEXT("Taiko_OutSideR"))
			{
				UI->TimeDrawStart(0.1f);
			}
		}
		//string ttszLog = "Sec :" + to_string(m_TestTime) + "f,   TYPE_SMALLBLUE" + "\n";
		//wstring ttDebugLog;
		//ttDebugLog.assign(ttszLog.begin(), ttszLog.end());
		//OutputDebugStringW(ttDebugLog.c_str());
	}

	if (g_pGameInstance->Get_DIKeyState(DIK_SPACE)&DIS_Down)
	{
		for (auto& UI : m_vecButtonUI)
		{
			if (UI->Get_UIName() == TEXT("Taiko_MidleL") || UI->Get_UIName() == TEXT("Taiko_MidleR"))
			{
				UI->TimeDrawStart(0.1f);
			}
		}
		//string ttszLog = "Sec :" + to_string(m_TestTime) + "f,   TYPE_BIGRED" + "\n";
		//wstring ttDebugLog;
		//ttDebugLog.assign(ttszLog.begin(), ttszLog.end());
		//OutputDebugStringW(ttDebugLog.c_str());
	}

	FAILED_CHECK(m_pBackGround->Update(fDeltaTime));
	FAILED_CHECK(m_pNoteUI->Update(fDeltaTime));
	FAILED_CHECK(m_StartTimeUI->Update(fDeltaTime));
	FAILED_CHECK(m_QuestUI->Update(fDeltaTime));
	FAILED_CHECK(m_ResultUI->Update(fDeltaTime));

	for (auto& UI : m_vecBasicUI)
		FAILED_CHECK(UI->Update(fDeltaTime));

	for (auto& UI : m_vecButtonUI)
		FAILED_CHECK(UI->Update(fDeltaTime));

	for (auto& UI : m_vecComboUI)
		FAILED_CHECK(UI->Update(fDeltaTime));

	for (auto& UI : m_vecGoodEffectUI)
		FAILED_CHECK(UI->Update(fDeltaTime));


	return _int();
}

_int CTaikoUI::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)return -1;


	GetSingle(CUtilityMgr)->Get_Renderer()->Add_RenderGroup(CRenderer::RENDER_JJBMINIGAME, this);

	return _int();
}

_int CTaikoUI::Render()
{
	if (__super::Render() < 0)		return -1;

	FAILED_CHECK(m_pBackGround->Render());
	FAILED_CHECK(m_pNoteUI->Render());

	for (auto& UI : m_vecBasicUI)
		FAILED_CHECK(UI->Render());

	for (auto& UI : m_vecButtonUI)
		FAILED_CHECK(UI->Render());

	FAILED_CHECK(m_StartTimeUI->Render());
	FAILED_CHECK(m_QuestUI->Render());

	for (auto& UI : m_vecComboUI)
		FAILED_CHECK(UI->Render());

	for (auto& UI : m_vecGoodEffectUI)
		FAILED_CHECK(UI->Render());

	FAILED_CHECK(m_ResultUI->Render());

	return _int();
}

_int CTaikoUI::LateRender()
{
	if (__super::LateRender() < 0)		return -1;

	return _int();
}

void CTaikoUI::Set_IsQuestUIStart()
{
	m_bIsQuestUI = true;
}

void CTaikoUI::Set_IsShowGoodEffectUI()
{
	m_bIsShowGoodEffectTime = true;
	m_fShowGoodEffectUITime = 0.f;

	for (auto& UI : m_vecGoodEffectUI)
		UI->Set_IsDraw(true);
}

void CTaikoUI::Set_ShowResultUI(_int TextureIndex)
{
	m_bIsShowResultt = true;
	m_fShowResultTime = 0.f;

	m_ResultUI->Set_TextureIndex(TextureIndex);
	m_ResultUI->Set_IsDraw(true);
}

void CTaikoUI::UseComboUI(_double fDeltaTime)
{
	m_fShowComboUITime += (_float)fDeltaTime;
	_int iCombo = static_cast<CScene_MiniGame_JJB*>(g_pGameInstance->Get_NowScene())->Get_ComboCount();

	_int iHundred = iCombo / 100;
	iCombo = iCombo % 100;
	_int iTen = iCombo / 10;
	iCombo = iCombo % 10;
	_int iOne = iCombo;

	if (m_fShowComboUITime > 1.f)
	{
		for (auto& UI : m_vecComboUI)
			UI->Set_IsDraw(false);

		m_fShowComboUITime = 0.f;
		m_bIsShowComboUIOn = false;
	}
	else
	{
		for (auto& UI : m_vecComboUI)
		{
			UI->Set_IsDraw(true);

			if (UI->Get_UIName() == TEXT("Taiko_ComboUI"))
			{
				_float SizeX = g_pGameInstance->Easing(TYPE_Linear, 125.f, 120.f, m_fShowComboUITime, 1.f);
				_float SizeY = g_pGameInstance->Easing(TYPE_Linear, 50.f, 45.f, m_fShowComboUITime, 1.f);
				UI->Set_Size(SizeX, SizeY);
			}
			else
			{
				_float SizeX = g_pGameInstance->Easing(TYPE_Linear, 35.f, 30.f, m_fShowComboUITime, 1.f);
				_float SizeY = g_pGameInstance->Easing(TYPE_Linear, 45.f, 40.f, m_fShowComboUITime, 1.f);
				UI->Set_Size(SizeX, SizeY);
			}

			if (UI->Get_UIName() == TEXT("Taiko_Combo100"))
				UI->Set_TextureIndex(13 + iHundred);
			else if (UI->Get_UIName() == TEXT("Taiko_Combo10"))
				UI->Set_TextureIndex(13 + iTen);
			else if(UI->Get_UIName() == TEXT("Taiko_Combo1"))
				UI->Set_TextureIndex(13 + iOne);
		}
	}
}

HRESULT CTaikoUI::Ready_Layer_UI()
{
	CGameInstance*		pGameInstance = g_pGameInstance;

	{
		CUI* UI = nullptr;
		CUI::SETTING_UI SettingUI;
		ZeroMemory(&SettingUI, sizeof(SettingUI));

		//Taiko_BackGround
		SettingUI.bClick = false;
		SettingUI.bMove = false;
		SettingUI.bDraw = true;
		SettingUI.bColl = false;
		SettingUI.iLevelIndex = m_eNowSceneNum;
		SettingUI.pUI_Name = TEXT("Taiko_BackGround");
		SettingUI.m_fSizeX = 1280.f;
		SettingUI.m_fSizeY = 720.f;
		SettingUI.m_fX = 640.f;
		SettingUI.m_fY = 360.f;
		SettingUI.fAngle = 0.f;
		SettingUI.iTextureIndex = 11;
		SettingUI.eUIKindsID = CUI::UIID_MINIGAME_TAIKO;

		pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&m_pBackGround), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
		NULL_CHECK_RETURN(m_pBackGround, E_FAIL);
	}

	{
		CUI* UI = nullptr;
		CUI::SETTING_UI SettingUI;
		ZeroMemory(&SettingUI, sizeof(SettingUI));

		//Taiko_NoteUI
		SettingUI.bClick = false;
		SettingUI.bMove = false;
		SettingUI.bDraw = true;
		SettingUI.bColl = false;
		SettingUI.iLevelIndex = m_eNowSceneNum;
		SettingUI.pUI_Name = TEXT("Taiko_NoteUI");
		SettingUI.m_fSizeX = 1100.f;
		SettingUI.m_fSizeY = 150.f;
		SettingUI.m_fX = 781.f;
		SettingUI.m_fY = 360.f;
		SettingUI.fAngle = 0.f;
		SettingUI.iTextureIndex = 0;
		SettingUI.eUIKindsID = CUI::UIID_MINIGAME_TAIKO;

		pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&m_pNoteUI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
		NULL_CHECK_RETURN(m_pNoteUI, E_FAIL);
	}

	{
		CUI* UI = nullptr;
		CUI::SETTING_UI SettingUI;
		ZeroMemory(&SettingUI, sizeof(SettingUI));

		//Taiko_NoteBackUI
		SettingUI.bClick = false;
		SettingUI.bMove = false;
		SettingUI.bDraw = true;
		SettingUI.bColl = false;
		SettingUI.iLevelIndex = m_eNowSceneNum;
		SettingUI.pUI_Name = TEXT("Taiko_NoteBackUI");
		SettingUI.m_fSizeX = 240.f;
		SettingUI.m_fSizeY = 150.f;
		SettingUI.m_fX = 115.f;
		SettingUI.m_fY = 360.f;
		SettingUI.fAngle = 0.f;
		SettingUI.iTextureIndex = 10;
		SettingUI.eUIKindsID = CUI::UIID_MINIGAME_TAIKO;

		pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
		NULL_CHECK_RETURN(UI, E_FAIL);
		m_vecBasicUI.push_back(UI);
	}

	{
		CUI* UI = nullptr;
		CUI::SETTING_UI SettingUI;
		ZeroMemory(&SettingUI, sizeof(SettingUI));

		//Taiko_DrumCharacter
		SettingUI.bClick = false;
		SettingUI.bMove = false;
		SettingUI.bDraw = true;
		SettingUI.bColl = false;
		SettingUI.iLevelIndex = m_eNowSceneNum;
		SettingUI.pUI_Name = TEXT("Taiko_DrumCharacter");
		SettingUI.m_fSizeX = 300.f;
		SettingUI.m_fSizeY = 220.f;
		SettingUI.m_fX = 125.f;
		SettingUI.m_fY = 220.f;
		SettingUI.fAngle = 0.f;
		SettingUI.iTextureIndex = 0;
		SettingUI.eUIKindsID = CUI::UIID_MINIGAME_TAIKODRUM;

		pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
		NULL_CHECK_RETURN(UI, E_FAIL);
		m_vecBasicUI.push_back(UI);
	}

	{
		CUI* UI = nullptr;
		CUI::SETTING_UI SettingUI;
		ZeroMemory(&SettingUI, sizeof(SettingUI));

		//Taiko_Drum
		SettingUI.bClick = false;
		SettingUI.bMove = false;
		SettingUI.bDraw = true;
		SettingUI.bColl = false;
		SettingUI.iLevelIndex = m_eNowSceneNum;
		SettingUI.pUI_Name = TEXT("Taiko_Drum");
		SettingUI.m_fSizeX = 100.f;
		SettingUI.m_fSizeY = 100.f;
		SettingUI.m_fX = 180.f;
		SettingUI.m_fY = 350.f;
		SettingUI.fAngle = 0.f;
		SettingUI.iTextureIndex = 1;
		SettingUI.eUIKindsID = CUI::UIID_MINIGAME_TAIKO;

		pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
		NULL_CHECK_RETURN(UI, E_FAIL);
		m_vecBasicUI.push_back(UI);
	}

	{
		CUI* UI = nullptr;
		CUI::SETTING_UI SettingUI;
		ZeroMemory(&SettingUI, sizeof(SettingUI));

		//Taiko_MidleL
		SettingUI.bClick = false;
		SettingUI.bMove = false;
		SettingUI.bDraw = false;
		SettingUI.bColl = false;
		SettingUI.iLevelIndex = m_eNowSceneNum;
		SettingUI.pUI_Name = TEXT("Taiko_MidleL");
		SettingUI.m_fSizeX = 47.f;
		SettingUI.m_fSizeY = 56.f;
		SettingUI.m_fX = 164.5f;
		SettingUI.m_fY = 335.5f;
		SettingUI.fAngle = 0.f;
		SettingUI.iTextureIndex = 8;
		SettingUI.eUIKindsID = CUI::UIID_MINIGAME_TAIKO;

		pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
		NULL_CHECK_RETURN(UI, E_FAIL);
		m_vecButtonUI.push_back(UI);
	}

	{
		CUI* UI = nullptr;
		CUI::SETTING_UI SettingUI;
		ZeroMemory(&SettingUI, sizeof(SettingUI));

		//Taiko_MidleR
		SettingUI.bClick = false;
		SettingUI.bMove = false;
		SettingUI.bDraw = false;
		SettingUI.bColl = false;
		SettingUI.iLevelIndex = m_eNowSceneNum;
		SettingUI.pUI_Name = TEXT("Taiko_MidleR");
		SettingUI.m_fSizeX = 47.f;
		SettingUI.m_fSizeY = 56.f;
		SettingUI.m_fX = 198.0f;
		SettingUI.m_fY = 335.5f;
		SettingUI.fAngle = 0.f;
		SettingUI.iTextureIndex = 6;
		SettingUI.eUIKindsID = CUI::UIID_MINIGAME_TAIKO;

		pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
		NULL_CHECK_RETURN(UI, E_FAIL);
		m_vecButtonUI.push_back(UI);
	}

	{
		CUI* UI = nullptr;
		CUI::SETTING_UI SettingUI;
		ZeroMemory(&SettingUI, sizeof(SettingUI));

		//Taiko_OutSideL
		SettingUI.bClick = false;
		SettingUI.bMove = false;
		SettingUI.bDraw = false;
		SettingUI.bColl = false;
		SettingUI.iLevelIndex = m_eNowSceneNum;
		SettingUI.pUI_Name = TEXT("Taiko_OutSideL");
		SettingUI.m_fSizeX = 50.f;
		SettingUI.m_fSizeY = 70.f;
		SettingUI.m_fX = 157.f;
		SettingUI.m_fY = 338.f;
		SettingUI.fAngle = 0.f;
		SettingUI.iTextureIndex = 9;
		SettingUI.eUIKindsID = CUI::UIID_MINIGAME_TAIKO;

		pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
		NULL_CHECK_RETURN(UI, E_FAIL);
		m_vecButtonUI.push_back(UI);
	}

	{
		CUI* UI = nullptr;
		CUI::SETTING_UI SettingUI;
		ZeroMemory(&SettingUI, sizeof(SettingUI));

		//Taiko_OutSideR
		SettingUI.bClick = false;
		SettingUI.bMove = false;
		SettingUI.bDraw = false;
		SettingUI.bColl = false;
		SettingUI.iLevelIndex = m_eNowSceneNum;
		SettingUI.pUI_Name = TEXT("Taiko_OutSideR");
		SettingUI.m_fSizeX = 50.f;
		SettingUI.m_fSizeY = 70.f;
		SettingUI.m_fX = 205.f;
		SettingUI.m_fY = 338.f;
		SettingUI.fAngle = 0.f;
		SettingUI.iTextureIndex = 7;
		SettingUI.eUIKindsID = CUI::UIID_MINIGAME_TAIKO;

		pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
		NULL_CHECK_RETURN(UI, E_FAIL);
		m_vecButtonUI.push_back(UI);
	}

	{
		CUI* UI = nullptr;
		CUI::SETTING_UI SettingUI;
		ZeroMemory(&SettingUI, sizeof(SettingUI));

		//Taiko_StartTimeUI
		SettingUI.bClick = false;
		SettingUI.bMove = false;
		SettingUI.bDraw = true;
		SettingUI.bColl = false;
		SettingUI.iLevelIndex = m_eNowSceneNum;
		SettingUI.pUI_Name = TEXT("Taiko_StartTimeUI");
		SettingUI.m_fSizeX = 100.f;
		SettingUI.m_fSizeY = 100.f;
		SettingUI.m_fX = 640.f;
		SettingUI.m_fY = 360.f;
		SettingUI.fAngle = 0.f;
		SettingUI.iTextureIndex = 16;
		SettingUI.eUIKindsID = CUI::UIID_MINIGAME_TAIKO;

		pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&m_StartTimeUI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
		NULL_CHECK_RETURN(m_StartTimeUI, E_FAIL);
	}

	{
		CUI* UI = nullptr;
		CUI::SETTING_UI SettingUI;
		ZeroMemory(&SettingUI, sizeof(SettingUI));

		//Taiko_QuestUI
		SettingUI.bClick = false;
		SettingUI.bMove = false;
		SettingUI.bDraw = true;
		SettingUI.bColl = false;
		SettingUI.iLevelIndex = m_eNowSceneNum;
		SettingUI.pUI_Name = TEXT("Taiko_QuestUI");
		SettingUI.m_fSizeX = 300.f;
		SettingUI.m_fSizeY = 80.f;
		SettingUI.m_fX = 1380.f;
		SettingUI.m_fY = 210.f;
		SettingUI.fAngle = 0.f;
		SettingUI.iTextureIndex = 25;
		SettingUI.eUIKindsID = CUI::UIID_MINIGAME_TAIKO;

		pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&m_QuestUI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
		NULL_CHECK_RETURN(m_QuestUI, E_FAIL);
	}

	{
		CUI* UI = nullptr;
		CUI::SETTING_UI SettingUI;
		ZeroMemory(&SettingUI, sizeof(SettingUI));

		//Taiko_ComboUI
		SettingUI.bClick = false;
		SettingUI.bMove = false;
		SettingUI.bDraw = false;
		SettingUI.bColl = false;
		SettingUI.iLevelIndex = m_eNowSceneNum;
		SettingUI.pUI_Name = TEXT("Taiko_ComboUI");
		SettingUI.m_fSizeX = 120.f;
		SettingUI.m_fSizeY = 45.f;
		SettingUI.m_fX = 310.f;
		SettingUI.m_fY = 250.f;
		SettingUI.fAngle = 0.f;
		SettingUI.iTextureIndex = 12;
		SettingUI.eUIKindsID = CUI::UIID_MINIGAME_TAIKO;

		pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
		NULL_CHECK_RETURN(UI, E_FAIL);
		m_vecComboUI.push_back(UI);
	}

	{
		CUI* UI = nullptr;
		CUI::SETTING_UI SettingUI;
		ZeroMemory(&SettingUI, sizeof(SettingUI));

		//Taiko_Combo100
		SettingUI.bClick = false;
		SettingUI.bMove = false;
		SettingUI.bDraw = false;
		SettingUI.bColl = false;
		SettingUI.iLevelIndex = m_eNowSceneNum;
		SettingUI.pUI_Name = TEXT("Taiko_Combo100");
		SettingUI.m_fSizeX = 30.f;
		SettingUI.m_fSizeY = 40.f;
		SettingUI.m_fX = 385.f;
		SettingUI.m_fY = 250.f;
		SettingUI.fAngle = 0.f;
		SettingUI.iTextureIndex = 13;
		SettingUI.eUIKindsID = CUI::UIID_MINIGAME_TAIKO;

		pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
		NULL_CHECK_RETURN(UI, E_FAIL);
		m_vecComboUI.push_back(UI);
	}

	{
		CUI* UI = nullptr;
		CUI::SETTING_UI SettingUI;
		ZeroMemory(&SettingUI, sizeof(SettingUI));

		//Taiko_Combo10
		SettingUI.bClick = false;
		SettingUI.bMove = false;
		SettingUI.bDraw = false;
		SettingUI.bColl = false;
		SettingUI.iLevelIndex = m_eNowSceneNum;
		SettingUI.pUI_Name = TEXT("Taiko_Combo10");
		SettingUI.m_fSizeX = 30.f;
		SettingUI.m_fSizeY = 40.f;
		SettingUI.m_fX = 410.f;
		SettingUI.m_fY = 250.f;
		SettingUI.fAngle = 0.f;
		SettingUI.iTextureIndex = 13;
		SettingUI.eUIKindsID = CUI::UIID_MINIGAME_TAIKO;

		pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
		NULL_CHECK_RETURN(UI, E_FAIL);
		m_vecComboUI.push_back(UI);
	}

	{
		CUI* UI = nullptr;
		CUI::SETTING_UI SettingUI;
		ZeroMemory(&SettingUI, sizeof(SettingUI));

		//Taiko_Combo1
		SettingUI.bClick = false;
		SettingUI.bMove = false;
		SettingUI.bDraw = false;
		SettingUI.bColl = false;
		SettingUI.iLevelIndex = m_eNowSceneNum;
		SettingUI.pUI_Name = TEXT("Taiko_Combo1");
		SettingUI.m_fSizeX = 30.f;
		SettingUI.m_fSizeY = 40.f;
		SettingUI.m_fX = 435.f;
		SettingUI.m_fY = 250.f;
		SettingUI.fAngle = 0.f;
		SettingUI.iTextureIndex = 13;
		SettingUI.eUIKindsID = CUI::UIID_MINIGAME_TAIKO;

		pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
		NULL_CHECK_RETURN(UI, E_FAIL);
		m_vecComboUI.push_back(UI);
	}

	{
		CUI* UI = nullptr;
		CUI::SETTING_UI SettingUI;
		ZeroMemory(&SettingUI, sizeof(SettingUI));

		//Taiko_GoodEffect1
		SettingUI.bClick = false;
		SettingUI.bMove = false;
		SettingUI.bDraw = false;
		SettingUI.bColl = false;
		SettingUI.iLevelIndex = m_eNowSceneNum;
		SettingUI.pUI_Name = TEXT("Taiko_GoodEffect1");
		SettingUI.m_fSizeX = 80.f;
		SettingUI.m_fSizeY = 80.f;
		SettingUI.m_fX = 323.5f;
		SettingUI.m_fY = 347.f;
		SettingUI.fAngle = 0.f;
		SettingUI.iTextureIndex = 26;
		SettingUI.eUIKindsID = CUI::UIID_MINIGAME_TAIKO;

		pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
		NULL_CHECK_RETURN(UI, E_FAIL);
		m_vecGoodEffectUI.push_back(UI);
	}

	{
		CUI* UI = nullptr;
		CUI::SETTING_UI SettingUI;
		ZeroMemory(&SettingUI, sizeof(SettingUI));

		//Taiko_GoodEffect2
		SettingUI.bClick = false;
		SettingUI.bMove = false;
		SettingUI.bDraw = false;
		SettingUI.bColl = false;
		SettingUI.iLevelIndex = m_eNowSceneNum;
		SettingUI.pUI_Name = TEXT("Taiko_GoodEffect2");
		SettingUI.m_fSizeX = 140.f;
		SettingUI.m_fSizeY = 140.f;
		SettingUI.m_fX = 323.5f;
		SettingUI.m_fY = 347.f;
		SettingUI.fAngle = 0.f;
		SettingUI.iTextureIndex = 27;
		SettingUI.eUIKindsID = CUI::UIID_MINIGAME_TAIKO;

		pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
		NULL_CHECK_RETURN(UI, E_FAIL);
		m_vecGoodEffectUI.push_back(UI);
	}

	{
		CUI* UI = nullptr;
		CUI::SETTING_UI SettingUI;
		ZeroMemory(&SettingUI, sizeof(SettingUI));

		//Taiko_ResultUI
		SettingUI.bClick = false;
		SettingUI.bMove = false;
		SettingUI.bDraw = false;
		SettingUI.bColl = false;
		SettingUI.iLevelIndex = m_eNowSceneNum;
		SettingUI.pUI_Name = TEXT("Taiko_ResultUI");
		SettingUI.m_fSizeX = 500.f;
		SettingUI.m_fSizeY = 180.f;
		SettingUI.m_fX = 640.f;
		SettingUI.m_fY = -200.f;
		SettingUI.fAngle = 0.f;
		SettingUI.iTextureIndex = 28;
		SettingUI.eUIKindsID = CUI::UIID_MINIGAME_TAIKO;

		pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&m_ResultUI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
		NULL_CHECK_RETURN(m_ResultUI, E_FAIL);
	}

	return S_OK;
}

HRESULT CTaikoUI::SetUp_Components()
{
	return S_OK;
}

CTaikoUI * CTaikoUI::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CTaikoUI*	pInstance = NEW CTaikoUI(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CTaikoUI");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CTaikoUI::Clone(void * pArg)
{
	CTaikoUI*	pInstance = NEW CTaikoUI(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created Clone CTaikoUI");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTaikoUI::Free()
{
	__super::Free();

	Safe_Release(m_pBackGround);
	Safe_Release(m_pNoteUI);
	Safe_Release(m_StartTimeUI);
	Safe_Release(m_QuestUI);
	Safe_Release(m_ResultUI);

	for (auto& UI : m_vecBasicUI)
		Safe_Release(UI);

	for (auto& UI : m_vecButtonUI)
		Safe_Release(UI);

	for (auto& UI : m_vecComboUI)
		Safe_Release(UI);
	for (auto& UI : m_vecGoodEffectUI)
		Safe_Release(UI);

	m_vecBasicUI.clear();
	m_vecButtonUI.clear();
	m_vecComboUI.clear();
}
