#include "stdafx.h"
#include "..\public\LobbyUI.h"
#include "UI.h"


CLobbyUI::CLobbyUI(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CLobbyUI::CLobbyUI(const CLobbyUI & rhs)
	: CGameObject(rhs)
{
}

HRESULT CLobbyUI::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));


	return S_OK;
}

HRESULT CLobbyUI::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	Ready_Layer_UI();

	return S_OK;
}

_int CLobbyUI::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)return -1;

	//CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	//_float2				MousePos;
	//POINT				ptMouse;
	//GetCursorPos(&ptMouse);
	//ScreenToClient(g_hWnd, &ptMouse);

	//MousePos.x = (_float)ptMouse.x;
	//MousePos.y = (_float)ptMouse.y;



	//RELEASE_INSTANCE(CGameInstance);


	FAILED_CHECK(m_pBackGround->Update(fDeltaTime));
	FAILED_CHECK(m_RajiText->Update(fDeltaTime));
	
	return _int();
}

_int CLobbyUI::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)return -1;

	GetSingle(CUtilityMgr)->Get_Renderer()->Add_RenderGroup(CRenderer::RENDER_NONBLEND_NOLIGHT, this);



	return _int();
}

_int CLobbyUI::Render()
{
	//if (__super::Render() < 0)		return -1;

	FAILED_CHECK(m_pBackGround->Render());
	FAILED_CHECK(m_RajiText->Render());
	
	return _int();
}

_int CLobbyUI::LateRender()
{
	if (__super::LateRender() < 0)		return -1;

	return _int();
}



HRESULT CLobbyUI::Ready_Layer_UI()
{

	CGameInstance*		pGameInstance = g_pGameInstance;

	{

		CUI* UI = nullptr;
		CUI::SETTING_UI SettingUI;
		ZeroMemory(&SettingUI, sizeof(SettingUI));

		//Skill_BackGround
		SettingUI.bClick = false;
		SettingUI.bMove = false;
		SettingUI.bDraw = true;
		SettingUI.bColl = false;
		SettingUI.iLevelIndex = m_eNowSceneNum;
		SettingUI.pUI_Name = TEXT("Lobby_BG");
		SettingUI.m_fSizeX = 1280.f;
		SettingUI.m_fSizeY = 720.f;
		SettingUI.m_fX = 640.f;
		SettingUI.m_fY = 360.f;
		SettingUI.fAngle = 0.f;
		SettingUI.iTextureIndex = 0;
		SettingUI.eUIKindsID = CUI::UIID_JY_Lobby;

		pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&m_pBackGround), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
		NULL_CHECK_RETURN(m_pBackGround, E_FAIL);
	}
	{
		 
		CUI* UI = nullptr;
		CUI::SETTING_UI SettingUI;
		ZeroMemory(&SettingUI, sizeof(SettingUI));

		//Skill_BackGround
		SettingUI.bClick = false;
		SettingUI.bMove = false;
		SettingUI.bDraw = true;
		SettingUI.bColl = false;
		SettingUI.iLevelIndex = m_eNowSceneNum;
		SettingUI.pUI_Name = TEXT("Lobby_BG");
		SettingUI.m_fSizeX = 600.f;
		SettingUI.m_fSizeY = 360.f;
		SettingUI.m_fX = 640.f;
		SettingUI.m_fY = 230.f;
		SettingUI.fAngle = 0.f;
		SettingUI.iTextureIndex = 135;

		pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&m_RajiText), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
		NULL_CHECK_RETURN(m_RajiText, E_FAIL);
		m_RajiText->Set_PassIndex(15);
	}
	//m_pBackGround = nullptr;

	//m_RajiText = nullptr;
	m_pPressEnter = nullptr;

	return S_OK;
}

HRESULT CLobbyUI::SetUp_Components()
{
	return S_OK;
}

CLobbyUI * CLobbyUI::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CLobbyUI*	pInstance = NEW CLobbyUI(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CLobbyUI");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CLobbyUI::Clone(void * pArg)
{
	CLobbyUI*	pInstance = NEW CLobbyUI(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created Clone CLobbyUI");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLobbyUI::Free()
{
	__super::Free();

	Safe_Release(m_pBackGround);
	Safe_Release(m_RajiText);
	Safe_Release(m_pPressEnter);

}
