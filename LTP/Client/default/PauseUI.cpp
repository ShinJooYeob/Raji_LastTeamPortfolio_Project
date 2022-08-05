#include "stdafx.h"
#include "PauseUI.h"
#include "UI.h"

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

	return S_OK;
}

_int CPauseUI::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)return -1;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CUtilityMgr* pUyilityMgr = GetSingle(CUtilityMgr);

	_float2				MousePos;
	POINT				ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	MousePos.x = (_float)ptMouse.x;
	MousePos.y = (_float)ptMouse.y;

	_float UIPosX;
	_float UIPosY;
	_float2 UISize;


	return _int();
}

_int CPauseUI::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)return -1;


	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

	return _int();
}

_int CPauseUI::Render()
{
	if (__super::Render() < 0)		return -1;


	return _int();
}

_int CPauseUI::LateRender()
{
	if (__super::LateRender() < 0)		return -1;

	return _int();
}

HRESULT CPauseUI::Ready_Layer_UI()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CUI* UI = nullptr;
	CUI::SETTING_UI SettingUI;
	ZeroMemory(&SettingUI, sizeof(SettingUI));

	//Skill_BackGround
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Skill_BackGround");
	SettingUI.m_fSizeX = 1280.f;
	SettingUI.m_fSizeY = 720.f;
	SettingUI.m_fX = 640.f;
	SettingUI.m_fY = 360.f;
	SettingUI.fAngle = 0.f;
	SettingUI.iTextureIndex = 131;


	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&m_pBackGround), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);

	RELEASE_INSTANCE(CGameInstance);

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

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pBackGround);

}
