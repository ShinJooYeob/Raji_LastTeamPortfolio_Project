#include "stdafx.h"
#include "..\public\PauseUI.h"
#include "UI.h"
#include "GraphicUI.h"
#include "SoundUI.h"

CPauseUI::CPauseUI(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CPauseUI::CPauseUI(const CPauseUI & rhs)
	: CGameObject(rhs)
{
}

HRESULT CPauseUI::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));


	return S_OK;
}

HRESULT CPauseUI::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	SetUp_Components();

	Ready_Layer_UI();

	m_BtnContinueFont.fAngle = 0.f;
	m_BtnContinueFont.szString = L"Continue";
	m_BtnContinueFont.vPosition = _float2(590.f, 270.f);
	m_BtnContinueFont.vColor = _float4(0.50f, 0.50f, 0.50f, 1.f);
	m_BtnContinueFont.vFontScale = _float2(0.5f);

	m_BtnGraphicFont.fAngle = 0.f;
	m_BtnGraphicFont.szString = L"Game Graphic";
	m_BtnGraphicFont.vPosition = _float2(590.f, 315.f);
	m_BtnGraphicFont.vColor = _float4(0.50f, 0.50f, 0.50f, 1.f);
	m_BtnGraphicFont.vFontScale = _float2(0.5f);

	m_BtnSoundFont.fAngle = 0.f;
	m_BtnSoundFont.szString = L"Game Sound";
	m_BtnSoundFont.vPosition = _float2(590.f, 360.f);
	m_BtnSoundFont.vColor = _float4(0.50f, 0.50f, 0.50f, 1.f);
	m_BtnSoundFont.vFontScale = _float2(0.5f);

	m_BtnExitFont.fAngle = 0.f;
	m_BtnExitFont.szString = L"Exit";
	m_BtnExitFont.vPosition = _float2(605.f, 405.f);
	m_BtnExitFont.vColor = _float4(0.50f, 0.50f, 0.50f, 1.f);
	m_BtnExitFont.vFontScale = _float2(0.5f);

	g_pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)&m_pGraphicUI, m_eNowSceneNum, TAG_OP(Prototype_Object_GraphicUI));
	m_pGraphicUI->Set_PauseUIRef(this);

	g_pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)&m_pSoundUI, m_eNowSceneNum, TAG_OP(Prototype_Object_SoundUI));
	m_pSoundUI->Set_PauseUIRef(this);

	return S_OK;
}

_int CPauseUI::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)return -1;

	CGameInstance* pGameInstance = GetSingle(CGameInstance);
	
	if (g_pGameInstance->Get_DIKeyState(DIK_ESCAPE)& DIS_Down)
	{
		m_bIsPauseOn = !m_bIsPauseOn;
	}

	if (!m_bIsPauseOn)
	{
		m_bIsGraphicUIOn = false;
		m_pGraphicUI->UI_OnOff(false);

		m_bIsSoundUIOn = false;
		m_pSoundUI->UI_OnOff(false);

		return _int();
	}

	CUtilityMgr* pUyilityMgr = GetSingle(CUtilityMgr);

	//if (m_pGraphicUI->Get_OnOffState() == true)
	//{
	//	m_bIsGraphicUIOn = true;
	//}
	//else
	//{
	//	m_bIsGraphicUIOn = false;
	//}

	_float2				MousePos;
	POINT				ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	MousePos.x = (_float)ptMouse.x;
	MousePos.y = (_float)ptMouse.y;

	_float UIPosX = 0.f;
	_float UIPosY = 0.f;
	_float2 UISize;

	m_bIsBtnClick = false;

	if (pGameInstance->Get_DIMouseButtonState(CInput_Device::MBS_LBUTTON) & DIS_Down)
	{
		m_bIsBtnClick = true;
		g_pGameInstance->PlaySoundW(L"JJB_Button_1.mp3", CHANNEL_EFFECT);
	}

	if (!m_bIsGraphicUIOn && !m_bIsSoundUIOn)
	{
		for (auto& BtnUI : m_vecBtns)
		{
			UIPosX = BtnUI->Get_PosX();
			UIPosY = BtnUI->Get_PosY();
			UISize = BtnUI->Get_Size();

			if (MousePos.x > UIPosX - (UISize.x*0.5f) &&
				MousePos.x < UIPosX + (UISize.x*0.5f) &&
				MousePos.y > UIPosY - (UISize.y*0.5f) &&
				MousePos.y < UIPosY + (UISize.y*0.5f))
			{

				BtnUI->Set_Size(340.f, 33.f);
				BtnUI->Set_TextureIndex(30);

				if (BtnUI->Get_UIName() == TEXT("Pause_Btn_Continue"))
				{
					m_BtnContinueFont.vFontScale = _float2(0.52f);
					m_BtnContinueFont.vColor = _float4(1.f, 1.f, 1.f, 1.f);

					if (m_bIsBtnClick)
					{
						m_bIsPauseOn = false;
					}
				}
				else if (BtnUI->Get_UIName() == TEXT("Pause_Btn_graphic"))
				{
					m_BtnGraphicFont.vFontScale = _float2(0.52f);
					m_BtnGraphicFont.vColor = _float4(1.f, 1.f, 1.f, 1.f);
					if (m_bIsBtnClick)
					{
						m_bIsGraphicUIOn = true;
						m_pGraphicUI->UI_OnOff(true);
					}
				}
				else if (BtnUI->Get_UIName() == TEXT("Pause_Btn_Sound"))
				{
					m_BtnSoundFont.vFontScale = _float2(0.52f);
					m_BtnSoundFont.vColor = _float4(1.f, 1.f, 1.f, 1.f);
					if (m_bIsBtnClick)
					{
						m_bIsSoundUIOn = true;
						m_pSoundUI->UI_OnOff(true);
					}
				}
				else if (BtnUI->Get_UIName() == TEXT("Pause_Btn_exit"))
				{
					m_BtnExitFont.vFontScale = _float2(0.52f);
					m_BtnExitFont.vColor = _float4(1.f, 1.f, 1.f, 1.f);
					if (m_bIsBtnClick)
					{
						DestroyWindow(g_hWnd);
					}
				}

			}
			else
			{
				BtnUI->Set_Size(300.f, 30.f);
				BtnUI->Set_TextureIndex(29);

				if (BtnUI->Get_UIName() == TEXT("Pause_Btn_Continue"))
				{
					m_BtnContinueFont.vFontScale = _float2(0.5f);
					m_BtnContinueFont.vColor = _float4(0.50f, 0.50f, 0.50f, 1.f);
				}
				else if (BtnUI->Get_UIName() == TEXT("Pause_Btn_graphic"))
				{
					m_BtnGraphicFont.vFontScale = _float2(0.5f);
					m_BtnGraphicFont.vColor = _float4(0.50f, 0.50f, 0.50f, 1.f);
				}
				else if (BtnUI->Get_UIName() == TEXT("Pause_Btn_Sound"))
				{
					m_BtnSoundFont.vFontScale = _float2(0.5f);
					m_BtnSoundFont.vColor = _float4(0.50f, 0.50f, 0.50f, 1.f);
				}
				else if (BtnUI->Get_UIName() == TEXT("Pause_Btn_exit"))
				{
					m_BtnExitFont.vFontScale = _float2(0.5f);
					m_BtnExitFont.vColor = _float4(0.50f, 0.50f, 0.50f, 1.f);
				}
			}
		}


		//UI
		m_pBackGround->Update(fDeltaTime);
		m_pBackGroundImage->Update(fDeltaTime);

		for (auto& BtnUI : m_vecBtns)
			BtnUI->Update(fDeltaTime);
	}


	if (m_bIsGraphicUIOn)
	{
		m_pGraphicUI->Update(fDeltaTime);
	}

	if (m_bIsSoundUIOn)
		m_pSoundUI->Update(fDeltaTime);

	return _int();
}

_int CPauseUI::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)return -1;

	if (!m_bIsPauseOn)
		return _int();

	if (!m_bIsGraphicUIOn && !m_bIsSoundUIOn)
	{
		//UI
		m_pBackGround->LateUpdate(fDeltaTime);
		m_pBackGroundImage->LateUpdate(fDeltaTime);

		for (auto& BtnUI : m_vecBtns)
			BtnUI->LateUpdate(fDeltaTime);
	}

	if (m_bIsGraphicUIOn)
	{
		m_pGraphicUI->LateUpdate(fDeltaTime);
	}

	if (m_bIsSoundUIOn)
		m_pSoundUI->LateUpdate(fDeltaTime);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

	return _int();
}

_int CPauseUI::Render()
{
	if (__super::Render() < 0)		return -1;
	
	if(!m_bIsGraphicUIOn && !m_bIsSoundUIOn)
		Render_Fonts();

	return _int();
}

_int CPauseUI::LateRender()
{
	if (__super::LateRender() < 0)		return -1;

	return _int();
}

void CPauseUI::DrawOnOff(_bool State)
{
	m_pBackGround->Set_IsDraw(State);
	m_pBackGroundImage->Set_IsDraw(State);

	for (auto& Btn : m_vecBtns)
	{
		Btn->Set_IsDraw(State);
	}
}

HRESULT CPauseUI::Ready_Layer_UI()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CUI* UI = nullptr;
	CUI::SETTING_UI SettingUI;
	ZeroMemory(&SettingUI, sizeof(SettingUI));

	//Pause_BackGround
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Pause_BackGround");
	SettingUI.m_fSizeX = 1280.f;
	SettingUI.m_fSizeY = 720.f;
	SettingUI.m_fX = 640.f;
	SettingUI.m_fY = 360.f;
	SettingUI.fAngle = 0.f;
	SettingUI.iTextureIndex = 136;


	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&m_pBackGround), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);


	ZeroMemory(&SettingUI, sizeof(SettingUI));

	//Pause_BackGround_Image
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Pause_BackGround_Image");
	SettingUI.m_fSizeX = 1280.f;
	SettingUI.m_fSizeY = 720.f;
	SettingUI.m_fX = 640.f;
	SettingUI.m_fY = 360.f;
	SettingUI.fAngle = 0.f;
	SettingUI.iTextureIndex = 47;


	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&m_pBackGroundImage), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);


	ZeroMemory(&SettingUI, sizeof(SettingUI));

	//Pause_Btn_Continue
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Pause_Btn_Continue");
	SettingUI.m_fSizeX = 300.f;
	SettingUI.m_fSizeY = 30.f;
	SettingUI.m_fX = 640.f;
	SettingUI.m_fY = 280.f;
	SettingUI.fAngle = 0.f;
	SettingUI.iTextureIndex = 29;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_vecBtns.push_back(UI);


	ZeroMemory(&SettingUI, sizeof(SettingUI));

	//Pause_Btn_graphic
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Pause_Btn_graphic");
	SettingUI.m_fSizeX = 300.f;
	SettingUI.m_fSizeY = 30.f;
	SettingUI.m_fX = 640.f;
	SettingUI.m_fY = 325.f;
	SettingUI.fAngle = 0.f;
	SettingUI.iTextureIndex = 29;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_vecBtns.push_back(UI);


	ZeroMemory(&SettingUI, sizeof(SettingUI));

	//Pause_Btn_Sound
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Pause_Btn_Sound");
	SettingUI.m_fSizeX = 300.f;
	SettingUI.m_fSizeY = 30.f;
	SettingUI.m_fX = 640.f;
	SettingUI.m_fY = 370.f;
	SettingUI.fAngle = 0.f;
	SettingUI.iTextureIndex = 29;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_vecBtns.push_back(UI);


	ZeroMemory(&SettingUI, sizeof(SettingUI));

	//Pause_Btn_exit
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Pause_Btn_exit");
	SettingUI.m_fSizeX = 300.f;
	SettingUI.m_fSizeY = 30.f;
	SettingUI.m_fX = 640.f;
	SettingUI.m_fY = 415.f;
	SettingUI.fAngle = 0.f;
	SettingUI.iTextureIndex = 29;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_vecBtns.push_back(UI);



	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CPauseUI::Render_Fonts()
{
	CGameInstance* pInstance = g_pGameInstance;

	pInstance->Render_Font(L"VinerFonts", m_BtnContinueFont.szString.c_str(), m_BtnContinueFont.vPosition,
		m_BtnContinueFont.vColor, m_BtnContinueFont.fAngle, m_BtnContinueFont.vFontScale);

	pInstance->Render_Font(L"VinerFonts", m_BtnGraphicFont.szString.c_str(), m_BtnGraphicFont.vPosition,
		m_BtnGraphicFont.vColor, m_BtnGraphicFont.fAngle, m_BtnGraphicFont.vFontScale);

	pInstance->Render_Font(L"VinerFonts", m_BtnSoundFont.szString.c_str(), m_BtnSoundFont.vPosition,
		m_BtnSoundFont.vColor, m_BtnSoundFont.fAngle, m_BtnSoundFont.vFontScale);

	pInstance->Render_Font(L"VinerFonts", m_BtnExitFont.szString.c_str(), m_BtnExitFont.vPosition,
		m_BtnExitFont.vColor, m_BtnExitFont.fAngle, m_BtnExitFont.vFontScale);
	return S_OK;
}

HRESULT CPauseUI::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	return S_OK;
}

CPauseUI * CPauseUI::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CPauseUI*	pInstance = NEW CPauseUI(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CPauseUI");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CPauseUI::Clone(void * pArg)
{
	CPauseUI*	pInstance = NEW CPauseUI(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created Clone CPauseUI");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPauseUI::Free()
{
	__super::Free();

	Safe_Release(m_pGraphicUI);
	Safe_Release(m_pSoundUI);

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pBackGround);
	Safe_Release(m_pBackGroundImage);

	for (auto& Btn : m_vecBtns)
	{
		Safe_Release(Btn);
	}

	m_vecBtns.clear();

}