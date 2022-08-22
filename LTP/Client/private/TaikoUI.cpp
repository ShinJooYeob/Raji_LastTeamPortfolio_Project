#include "stdafx.h"
#include "..\public\TaikoUI.h"
#include "UI.h"

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


	for (auto& UI : m_vecBasicUI)
	{
		if (UI->Get_UIName() == TEXT("Taiko_OutSideR"))
		{
			UI->Set_Size(50.f, 70.f);
			UI->Set_UIPosition(205.f, 338.f);
		}
	}

	if (g_pGameInstance->Get_DIKeyState(DIK_F)&DIS_Down)
	{
		for (auto& UI : m_vecButtonUI)
		{
			if (UI->Get_UIName() == TEXT("Taiko_MidleL"))
			{
				UI->TimeDrawStart(0.1f);
			}
		}
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
	}

	FAILED_CHECK(m_pBackGround->Update(fDeltaTime));
	FAILED_CHECK(m_pNoteUI->Update(fDeltaTime));

	for (auto& UI : m_vecBasicUI)
		FAILED_CHECK(UI->Update(fDeltaTime));

	for (auto& UI : m_vecButtonUI)
		FAILED_CHECK(UI->Update(fDeltaTime));

	return _int();
}

_int CTaikoUI::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)return -1;


	GetSingle(CUtilityMgr)->Get_Renderer()->Add_RenderGroup(CRenderer::RENDER_UI, this);

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


	return _int();
}

_int CTaikoUI::LateRender()
{
	if (__super::LateRender() < 0)		return -1;

	return _int();
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

	for (auto& UI : m_vecBasicUI)
		Safe_Release(UI);

	for (auto& UI : m_vecButtonUI)
		Safe_Release(UI);

	m_vecBasicUI.clear();
	m_vecButtonUI.clear();
}
