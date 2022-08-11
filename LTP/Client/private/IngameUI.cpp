#include "stdafx.h"
#include "..\public\IngameUI.h"
#include "UI.h"

CIngameUI::CIngameUI(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CIngameUI::CIngameUI(const CIngameUI & rhs)
	: CGameObject(rhs)
{
}

HRESULT CIngameUI::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));


	return S_OK;
}

HRESULT CIngameUI::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	SetUp_Components();

	Ready_Layer_UI();

	m_Skillpercent.fAngle = 0.f;
	m_Skillpercent.szString = L"100%";
	m_Skillpercent.vPosition = _float2(1122.f, 604.f);
	m_Skillpercent.vColor = _float4(0.f, 0.f, 0.f, 1.f);
	m_Skillpercent.vFontScale = _float2(0.7f);


	return S_OK;
}

_int CIngameUI::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)return -1;

	if (m_bIsChangeWeapon)
		TurnWeaponUIAngle(fDeltaTime);


	m_pWeaponUI->Update(fDeltaTime);
	m_pWeaponMidleUI->Update(fDeltaTime);

	return _int();
}

_int CIngameUI::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)return -1;

	m_pWeaponUI->LateUpdate(fDeltaTime);
	m_pWeaponMidleUI->LateUpdate(fDeltaTime);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

	return _int();
}

_int CIngameUI::Render()
{
	if (__super::Render() < 0)		return -1;

	//Render_Fonts();

	return _int();
}

_int CIngameUI::LateRender()
{
	if (__super::LateRender() < 0)		return -1;

	return _int();
}

void CIngameUI::Set_WeaponUIAngle(_int iWeapon)
{
	m_bIsChangeWeapon = true;
	m_iNextWeapon = iWeapon;

	switch (iWeapon)
	{
	case 0:
		m_fTurningAngle = 240.f;
		break;
	case 1:
		m_fTurningAngle = 120.f;
		break;
	case 2:
		m_fTurningAngle = 0.f;
		break;
	}
}

void CIngameUI::TurnWeaponUIAngle(_double fDeltaTime)
{
	m_fDeltaTime += (_float)fDeltaTime;

	CGameInstance*		pGameInstance = GetSingle(CGameInstance);

	_float fAngle = pGameInstance->Easing(TYPE_SinInOut, m_fWeaponNowUIAngle, 720 + m_fTurningAngle, m_fDeltaTime, 0.5f);

	m_pWeaponUI->Set_Angle(fAngle);

	if (m_fDeltaTime >= .5f)
	{
		m_pWeaponUI->Set_Angle(m_fTurningAngle);
		m_bIsChangeWeapon = false;
		m_iBeforeWeapon = m_iNextWeapon;
		m_fDeltaTime = 0.f;
		m_fWeaponNowUIAngle = m_fTurningAngle;
	}

}

HRESULT CIngameUI::Ready_Layer_UI()
{
	CGameInstance*		pGameInstance = GetSingle(CGameInstance);

	CUI* UI = nullptr;
	CUI::SETTING_UI SettingUI;
	ZeroMemory(&SettingUI, sizeof(SettingUI));

	//Sound_BackGround
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("IngameWeaponUI");
	SettingUI.m_fSizeX = 200.f;
	SettingUI.m_fSizeY = 200.f;
	SettingUI.m_fX = 1145.f;
	SettingUI.m_fY = 617.f;
	SettingUI.fAngle = 0.f;
	SettingUI.iTextureIndex = 138;


	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&m_pWeaponUI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_pWeaponUI->Set_RenderSortValue(2.f);

	ZeroMemory(&SettingUI, sizeof(SettingUI));

	//Sound_BackGround_Image
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("IngameWeaponMidleUI");
	SettingUI.m_fSizeX = 65.f;
	SettingUI.m_fSizeY = 65.f;
	SettingUI.m_fX = 1145.f;
	SettingUI.m_fY = 617.f;
	SettingUI.fAngle = 0.f;
	SettingUI.iTextureIndex = 139;
	SettingUI.bSettingOtherTexture = true;
	SettingUI.iNoiseTextureIndex = 319;


	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&m_pWeaponMidleUI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_pWeaponMidleUI->Set_RenderSortValue(2.f);
	m_pWeaponMidleUI->Set_PassIndex(12);

	return S_OK;
}

HRESULT CIngameUI::Render_Fonts()
{
	CGameInstance* pInstance = g_pGameInstance;

	pInstance->Render_Font(L"VinerFonts", m_Skillpercent.szString.c_str(), m_Skillpercent.vPosition,
		m_Skillpercent.vColor, m_Skillpercent.fAngle, m_Skillpercent.vFontScale);

	return S_OK;
}

HRESULT CIngameUI::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	return S_OK;
}

CIngameUI * CIngameUI::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CIngameUI*	pInstance = NEW CIngameUI(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CSoundUI");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CIngameUI::Clone(void * pArg)
{
	CIngameUI*	pInstance = NEW CIngameUI(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created Clone CSoundUI");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CIngameUI::Free()
{
	__super::Free();
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pWeaponUI);
	Safe_Release(m_pWeaponMidleUI);
}
