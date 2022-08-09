#include "stdafx.h"
#include "..\public\GraphicUI.h"
#include "UI.h"
#include "PauseUI.h"

CGraphicUI::CGraphicUI(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CGraphicUI::CGraphicUI(const CGraphicUI & rhs)
	: CGameObject(rhs)
{
}

HRESULT CGraphicUI::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));


	return S_OK;
}

HRESULT CGraphicUI::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	SetUp_Components();

	Ready_Layer_UI();

	m_OptionName_Bloom.fAngle = 0.f;
	m_OptionName_Bloom.szString = L"Bloom";
	m_OptionName_Bloom.vPosition = _float2(425.f, 240.f);
	m_OptionName_Bloom.vColor = _float4(1.f, 1.f, 1.f, 1.f);
	m_OptionName_Bloom.vFontScale = _float2(0.5f);

	m_OptionName_DOF.fAngle = 0.f;
	m_OptionName_DOF.szString = L"Depth Of Field";
	m_OptionName_DOF.vPosition = _float2(425.f, 290.f);
	m_OptionName_DOF.vColor = _float4(1.f, 1.f, 1.f, 1.f);
	m_OptionName_DOF.vFontScale = _float2(0.5f);

	m_OptionName_DDFOG.fAngle = 0.f;
	m_OptionName_DDFOG.szString = L"Fog";
	m_OptionName_DDFOG.vPosition = _float2(425.f, 340.f);
	m_OptionName_DDFOG.vColor = _float4(1.f, 1.f, 1.f, 1.f);
	m_OptionName_DDFOG.vFontScale = _float2(0.5f);

	m_OptionName_Shadow.fAngle = 0.f;
	m_OptionName_Shadow.szString = L"Shadow";
	m_OptionName_Shadow.vPosition = _float2(425.f, 390.f);
	m_OptionName_Shadow.vColor = _float4(1.f, 1.f, 1.f, 1.f);
	m_OptionName_Shadow.vFontScale = _float2(0.5f);

	m_OptionName_GODRAY.fAngle = 0.f;
	m_OptionName_GODRAY.szString = L"God Ray";
	m_OptionName_GODRAY.vPosition = _float2(425.f, 440.f);
	m_OptionName_GODRAY.vColor = _float4(1.f, 1.f, 1.f, 1.f);
	m_OptionName_GODRAY.vFontScale = _float2(0.5f);

	m_OptionName_Lenseflare.fAngle = 0.f;
	m_OptionName_Lenseflare.szString = L"Lense Flare";
	m_OptionName_Lenseflare.vPosition = _float2(415.f, 490.f);
	m_OptionName_Lenseflare.vColor = _float4(1.f, 1.f, 1.f, 1.f);
	m_OptionName_Lenseflare.vFontScale = _float2(0.5f);

	CRenderer* Renderer = GetSingle(CUtilityMgr)->Get_Renderer();

	_bool bOnOff = Renderer->Get_IsOnPostPorcessing(POSTPROCESSING_BLOOM);

	m_BloomOnOFf.fAngle = 0.f;
	m_BloomOnOFf.vPosition = _float2(770.f, 230.f);
	m_BloomOnOFf.vColor = _float4(1.f, 1.f, 1.f, 1.f);
	m_BloomOnOFf.vFontScale = _float2(0.8f);
	if(bOnOff)
		m_BloomOnOFf.szString = L"ON";
	else
		m_BloomOnOFf.szString = L"OFF";

	m_DOFOnOFf.fAngle = 0.f;
	m_DOFOnOFf.vPosition = _float2(770.f, 280.f);
	m_DOFOnOFf.vColor = _float4(1.f, 1.f, 1.f, 1.f);
	m_DOFOnOFf.vFontScale = _float2(0.8f);
	bOnOff = Renderer->Get_IsOnPostPorcessing(POSTPROCESSING_DOF);
	if (bOnOff)
		m_DOFOnOFf.szString = L"ON";
	else
		m_DOFOnOFf.szString = L"OFF";



	m_DDFOGOnOFf.fAngle = 0.f;
	m_DDFOGOnOFf.vPosition = _float2(770.f, 330.f);
	m_DDFOGOnOFf.vColor = _float4(1.f, 1.f, 1.f, 1.f);
	m_DDFOGOnOFf.vFontScale = _float2(0.8f);
	bOnOff = Renderer->Get_IsOnPostPorcessing(POSTPROCESSING_DDFOG);
	if (bOnOff)
		m_DDFOGOnOFf.szString = L"ON";
	else
		m_DDFOGOnOFf.szString = L"OFF";



	m_ShadowOnOFf.fAngle = 0.f;
	m_ShadowOnOFf.vPosition = _float2(770.f, 380.f);
	m_ShadowOnOFf.vColor = _float4(1.f, 1.f, 1.f, 1.f);
	m_ShadowOnOFf.vFontScale = _float2(0.8f);
	bOnOff = Renderer->Get_IsOnPostPorcessing(POSTPROCESSING_SHADOW);
	if (bOnOff)
		m_ShadowOnOFf.szString = L"ON";
	else
		m_ShadowOnOFf.szString = L"OFF";

	m_GODRAYOnOFf.fAngle = 0.f;
	m_GODRAYOnOFf.vPosition = _float2(770.f, 430.f);
	m_GODRAYOnOFf.vColor = _float4(1.f, 1.f, 1.f, 1.f);
	m_GODRAYOnOFf.vFontScale = _float2(0.8f);
	bOnOff = Renderer->Get_IsOnPostPorcessing(POSTPROCESSING_GODRAY);
	if (bOnOff)
		m_GODRAYOnOFf.szString = L"ON";
	else
		m_GODRAYOnOFf.szString = L"OFF";



	m_LenseflareOnOFf.fAngle = 0.f;
	m_LenseflareOnOFf.vPosition = _float2(770.f, 480.f);
	m_LenseflareOnOFf.vColor = _float4(1.f, 1.f, 1.f, 1.f);
	m_LenseflareOnOFf.vFontScale = _float2(0.8f);
	bOnOff = Renderer->Get_IsOnPostPorcessing(POSTPROCESSING_LENSEFLARE);
	if (bOnOff)
		m_LenseflareOnOFf.szString = L"ON";
	else
		m_LenseflareOnOFf.szString = L"OFF";



	m_BtnBack.fAngle = 0.f;
	m_BtnBack.szString = L"BACK";
	m_BtnBack.vPosition = _float2(590.f, 550.f);
	m_BtnBack.vColor = _float4(1.f, 1.f, 1.f, 1.f);
	m_BtnBack.vFontScale = _float2(0.8f);



	return S_OK;
}

_int CGraphicUI::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)return -1;

	if(!m_bISUIOn)
		return _int();

	CheekBTN();



	//UI
	m_pBackGround->Update(fDeltaTime);
	m_pBackGroundImage->Update(fDeltaTime);
	m_pBackBtn->Update(fDeltaTime);

	for (auto& OptionName : m_pGraphicOptionUIs)
	{
		OptionName->Update(fDeltaTime);
	}

	for (auto& Btn : m_pBtns)
	{
		Btn->Update(fDeltaTime);
	}

	return _int();
}

_int CGraphicUI::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)return -1;

	if (!m_bISUIOn)
		return _int();

	m_pBackGround->LateUpdate(fDeltaTime);
	m_pBackGroundImage->LateUpdate(fDeltaTime);
	m_pBackBtn->LateUpdate(fDeltaTime);

	for (auto& OptionName : m_pGraphicOptionUIs)
	{
		OptionName->LateUpdate(fDeltaTime);
	}

	for (auto& Btn : m_pBtns)
	{
		Btn->LateUpdate(fDeltaTime);
	}

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

	return _int();
}

_int CGraphicUI::Render()
{
	if (__super::Render() < 0)		return -1;

	Render_Fonts();

	return _int();
}

_int CGraphicUI::LateRender()
{
	if (__super::LateRender() < 0)		return -1;

	return _int();
}

void CGraphicUI::Set_PauseUIRef(void * UI)
{
	 m_pPauseUI = (CPauseUI*)UI; 
}

void CGraphicUI::CheekBTN()
{
	CGameInstance* pGameInstance = GetSingle(CGameInstance);

	_float2				MousePos;
	POINT				ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	MousePos.x = (_float)ptMouse.x;
	MousePos.y = (_float)ptMouse.y;

	_float UIPosX = 0.f;
	_float UIPosY = 0.f;
	_float2 UISize;

	UIPosX = m_pBackBtn->Get_PosX();
	UIPosY = m_pBackBtn->Get_PosY();
	UISize = m_pBackBtn->Get_Size();

	if (MousePos.x > UIPosX - (UISize.x*0.5f) &&
		MousePos.x < UIPosX + (UISize.x*0.5f) &&
		MousePos.y > UIPosY - (UISize.y*0.5f) &&
		MousePos.y < UIPosY + (UISize.y*0.5f))
	{
		//185, 50
		m_pBackBtn->Set_Size(220.f, 53.f);
		m_BtnBack.vFontScale = _float2(0.85f);
	}
	else
	{
		m_pBackBtn->Set_Size(185.f, 50.f);
		m_BtnBack.vFontScale = _float2(0.8f);
	}


	if (pGameInstance->Get_DIMouseButtonState(CInput_Device::MBS_LBUTTON) & DIS_Down)
	{
		CRenderer* Renderer = GetSingle(CUtilityMgr)->Get_Renderer();

		UIPosX = m_pBackBtn->Get_PosX();
		UIPosY = m_pBackBtn->Get_PosY();
		UISize = m_pBackBtn->Get_Size();

		if (MousePos.x > UIPosX - (UISize.x*0.5f) &&
			MousePos.x < UIPosX + (UISize.x*0.5f) &&
			MousePos.y > UIPosY - (UISize.y*0.5f) &&
			MousePos.y < UIPosY + (UISize.y*0.5f))
		{
			m_bISUIOn = false;
			m_pPauseUI->Set_GraphicUIOn(false);
		}

		for (auto& Btn : m_pBtns)
		{
			UIPosX = Btn->Get_PosX();
			UIPosY = Btn->Get_PosY();
			UISize = Btn->Get_Size();

			if (MousePos.x > UIPosX - (UISize.x*0.5f) &&
				MousePos.x < UIPosX + (UISize.x*0.5f) &&
				MousePos.y > UIPosY - (UISize.y*0.5f) &&
				MousePos.y < UIPosY + (UISize.y*0.5f))
			{
				if (Btn->Get_UIName() == TEXT("Graphic_Btn1_1"))
				{
					Renderer->OnOff_PostPorcessing_byParameter(POSTPROCESSING_BLOOM, false);
					m_BloomOnOFf.szString = L"OFF";
				}
				else if (Btn->Get_UIName() == TEXT("Graphic_Btn1_2"))
				{
					Renderer->OnOff_PostPorcessing_byParameter(POSTPROCESSING_BLOOM, true);
					m_BloomOnOFf.szString = L"ON";
				}
				if (Btn->Get_UIName() == TEXT("Graphic_Btn2_1"))
				{
					Renderer->OnOff_PostPorcessing_byParameter(POSTPROCESSING_DOF, false);
					m_DOFOnOFf.szString = L"OFF";
				}
				else if (Btn->Get_UIName() == TEXT("Graphic_Btn2_2"))
				{
					Renderer->OnOff_PostPorcessing_byParameter(POSTPROCESSING_DOF, true);
					m_DOFOnOFf.szString = L"ON";
				}
				if (Btn->Get_UIName() == TEXT("Graphic_Btn3_1"))
				{
					Renderer->OnOff_PostPorcessing_byParameter(POSTPROCESSING_DDFOG, false);
					m_DDFOGOnOFf.szString = L"OFF";
				}
				else if (Btn->Get_UIName() == TEXT("Graphic_Btn3_2"))
				{
					Renderer->OnOff_PostPorcessing_byParameter(POSTPROCESSING_DDFOG, true);
					m_DDFOGOnOFf.szString = L"ON";
				}
				if (Btn->Get_UIName() == TEXT("Graphic_Btn4_1"))
				{
					Renderer->OnOff_PostPorcessing_byParameter(POSTPROCESSING_SHADOW, false);
					m_ShadowOnOFf.szString = L"OFF";
				}
				else if (Btn->Get_UIName() == TEXT("Graphic_Btn4_2"))
				{
					Renderer->OnOff_PostPorcessing_byParameter(POSTPROCESSING_SHADOW, true);
					m_ShadowOnOFf.szString = L"ON";
				}
				if (Btn->Get_UIName() == TEXT("Graphic_Btn5_1"))
				{
					Renderer->OnOff_PostPorcessing_byParameter(POSTPROCESSING_GODRAY, false);
					m_GODRAYOnOFf.szString = L"OFF";
				}
				else if (Btn->Get_UIName() == TEXT("Graphic_Btn5_2"))
				{
					Renderer->OnOff_PostPorcessing_byParameter(POSTPROCESSING_GODRAY, true);
					m_GODRAYOnOFf.szString = L"ON";
				}
				if (Btn->Get_UIName() == TEXT("Graphic_Btn6_1"))
				{
					Renderer->OnOff_PostPorcessing_byParameter(POSTPROCESSING_LENSEFLARE, false);
					m_LenseflareOnOFf.szString = L"OFF";
				}
				else if (Btn->Get_UIName() == TEXT("Graphic_Btn6_2"))
				{
					Renderer->OnOff_PostPorcessing_byParameter(POSTPROCESSING_LENSEFLARE, true);
					m_LenseflareOnOFf.szString = L"ON";
				}
			}
		}
	}
}

HRESULT CGraphicUI::Ready_Layer_UI()
{
	CGameInstance*		pGameInstance = GetSingle(CGameInstance);

	CUI* UI = nullptr;
	CUI::SETTING_UI SettingUI;
	ZeroMemory(&SettingUI, sizeof(SettingUI));

	//Graphic_BackGround
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Graphic_BackGround");
	SettingUI.m_fSizeX = 1280.f;
	SettingUI.m_fSizeY = 720.f;
	SettingUI.m_fX = 640.f;
	SettingUI.m_fY = 360.f;
	SettingUI.fAngle = 0.f;
	SettingUI.iTextureIndex = 136;


	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&m_pBackGround), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);


	ZeroMemory(&SettingUI, sizeof(SettingUI));

	//Graphic_BackGround_Image
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Graphic_BackGround_Image");
	SettingUI.m_fSizeX = 1280.f;
	SettingUI.m_fSizeY = 720.f;
	SettingUI.m_fX = 640.f;
	SettingUI.m_fY = 360.f;
	SettingUI.fAngle = 0.f;
	SettingUI.iTextureIndex = 47;


	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&m_pBackGroundImage), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);


	ZeroMemory(&SettingUI, sizeof(SettingUI));

	//Graphic_BloomUI
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Graphic_BloomUI");
	SettingUI.m_fSizeX = 300.f;
	SettingUI.m_fSizeY = 30.f;
	SettingUI.m_fX = 450.f;
	SettingUI.m_fY = 250.f;
	SettingUI.fAngle = 0.f;
	SettingUI.iTextureIndex = 31;


	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_pGraphicOptionUIs.push_back(UI);

	ZeroMemory(&SettingUI, sizeof(SettingUI));

	//Graphic_DOFUI
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Graphic_DOFUI");
	SettingUI.m_fSizeX = 300.f;
	SettingUI.m_fSizeY = 30.f;
	SettingUI.m_fX = 450.f;
	SettingUI.m_fY = 300.f;
	SettingUI.fAngle = 0.f;
	SettingUI.iTextureIndex = 31;


	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_pGraphicOptionUIs.push_back(UI);

	ZeroMemory(&SettingUI, sizeof(SettingUI));

	//Graphic_DDFOGUI
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Graphic_DDFOGUI");
	SettingUI.m_fSizeX = 300.f;
	SettingUI.m_fSizeY = 30.f;
	SettingUI.m_fX = 450.f;
	SettingUI.m_fY = 350.f;
	SettingUI.fAngle = 0.f;
	SettingUI.iTextureIndex = 31;


	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_pGraphicOptionUIs.push_back(UI);

	ZeroMemory(&SettingUI, sizeof(SettingUI));

	//Graphic_ShadowUI
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Graphic_ShadowUI");
	SettingUI.m_fSizeX = 300.f;
	SettingUI.m_fSizeY = 30.f;
	SettingUI.m_fX = 450.f;
	SettingUI.m_fY = 400.f;
	SettingUI.fAngle = 0.f;
	SettingUI.iTextureIndex = 31;


	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_pGraphicOptionUIs.push_back(UI);

	ZeroMemory(&SettingUI, sizeof(SettingUI));

	//Graphic_GODRAYUI
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Graphic_GODRAYUI");
	SettingUI.m_fSizeX = 300.f;
	SettingUI.m_fSizeY = 30.f;
	SettingUI.m_fX = 450.f;
	SettingUI.m_fY = 450.f;
	SettingUI.fAngle = 0.f;
	SettingUI.iTextureIndex = 31;


	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_pGraphicOptionUIs.push_back(UI);

	ZeroMemory(&SettingUI, sizeof(SettingUI));

	//Graphic_Lenseflare
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Graphic_Lenseflare");
	SettingUI.m_fSizeX = 300.f;
	SettingUI.m_fSizeY = 30.f;
	SettingUI.m_fX = 450.f;
	SettingUI.m_fY = 500.f;
	SettingUI.fAngle = 0.f;
	SettingUI.iTextureIndex = 31;


	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_pGraphicOptionUIs.push_back(UI);

	ZeroMemory(&SettingUI, sizeof(SettingUI));

	//Graphic_Btn1_1
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Graphic_Btn1_1");
	SettingUI.m_fSizeX = 25.f;
	SettingUI.m_fSizeY = 25.f;
	SettingUI.m_fX = 680.f;
	SettingUI.m_fY = 250.f;
	SettingUI.fAngle = 0.f;
	SettingUI.iTextureIndex = 21;


	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_pBtns.push_back(UI);

	ZeroMemory(&SettingUI, sizeof(SettingUI));

	//Graphic_Btn1_2
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Graphic_Btn1_2");
	SettingUI.m_fSizeX = 25.f;
	SettingUI.m_fSizeY = 25.f;
	SettingUI.m_fX = 925.f;
	SettingUI.m_fY = 250.f;
	SettingUI.fAngle = 180.f;
	SettingUI.iTextureIndex = 21;


	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_pBtns.push_back(UI);

	ZeroMemory(&SettingUI, sizeof(SettingUI));

	//Graphic_Btn2_1
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Graphic_Btn2_1");
	SettingUI.m_fSizeX = 25.f;
	SettingUI.m_fSizeY = 25.f;
	SettingUI.m_fX = 680.f;
	SettingUI.m_fY = 300.f;
	SettingUI.fAngle = 0.f;
	SettingUI.iTextureIndex = 21;


	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_pBtns.push_back(UI);

	ZeroMemory(&SettingUI, sizeof(SettingUI));

	//Graphic_Btn2_2
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Graphic_Btn2_2");
	SettingUI.m_fSizeX = 25.f;
	SettingUI.m_fSizeY = 25.f;
	SettingUI.m_fX = 925.f;
	SettingUI.m_fY = 300.f;
	SettingUI.fAngle = 180.f;
	SettingUI.iTextureIndex = 21;


	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_pBtns.push_back(UI);

	ZeroMemory(&SettingUI, sizeof(SettingUI));

	//Graphic_Btn3_1
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Graphic_Btn3_1");
	SettingUI.m_fSizeX = 25.f;
	SettingUI.m_fSizeY = 25.f;
	SettingUI.m_fX = 680.f;
	SettingUI.m_fY = 350.f;
	SettingUI.fAngle = 0.f;
	SettingUI.iTextureIndex = 21;


	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_pBtns.push_back(UI);

	ZeroMemory(&SettingUI, sizeof(SettingUI));

	//Graphic_Btn3_2
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Graphic_Btn3_2");
	SettingUI.m_fSizeX = 25.f;
	SettingUI.m_fSizeY = 25.f;
	SettingUI.m_fX = 925.f;
	SettingUI.m_fY = 350.f;
	SettingUI.fAngle = 180.f;
	SettingUI.iTextureIndex = 21;


	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_pBtns.push_back(UI);

	ZeroMemory(&SettingUI, sizeof(SettingUI));

	//Graphic_Btn4_1
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Graphic_Btn4_1");
	SettingUI.m_fSizeX = 25.f;
	SettingUI.m_fSizeY = 25.f;
	SettingUI.m_fX = 680.f;
	SettingUI.m_fY = 400.f;
	SettingUI.fAngle = 0.f;
	SettingUI.iTextureIndex = 21;


	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_pBtns.push_back(UI);

	ZeroMemory(&SettingUI, sizeof(SettingUI));

	//Graphic_Btn4_2
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Graphic_Btn4_2");
	SettingUI.m_fSizeX = 25.f;
	SettingUI.m_fSizeY = 25.f;
	SettingUI.m_fX = 925.f;
	SettingUI.m_fY = 400.f;
	SettingUI.fAngle = 180.f;
	SettingUI.iTextureIndex = 21;


	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_pBtns.push_back(UI);

	ZeroMemory(&SettingUI, sizeof(SettingUI));

	//Graphic_Btn5_1
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Graphic_Btn5_1");
	SettingUI.m_fSizeX = 25.f;
	SettingUI.m_fSizeY = 25.f;
	SettingUI.m_fX = 680.f;
	SettingUI.m_fY = 450.f;
	SettingUI.fAngle = 0.f;
	SettingUI.iTextureIndex = 21;


	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_pBtns.push_back(UI);

	ZeroMemory(&SettingUI, sizeof(SettingUI));

	//Graphic_Btn5_2
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Graphic_Btn5_2");
	SettingUI.m_fSizeX = 25.f;
	SettingUI.m_fSizeY = 25.f;
	SettingUI.m_fX = 925.f;
	SettingUI.m_fY = 450.f;
	SettingUI.fAngle = 180.f;
	SettingUI.iTextureIndex = 21;


	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_pBtns.push_back(UI);

	ZeroMemory(&SettingUI, sizeof(SettingUI));

	//Graphic_Btn6_1
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Graphic_Btn6_1");
	SettingUI.m_fSizeX = 25.f;
	SettingUI.m_fSizeY = 25.f;
	SettingUI.m_fX = 680.f;
	SettingUI.m_fY = 500.f;
	SettingUI.fAngle = 0.f;
	SettingUI.iTextureIndex = 21;


	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_pBtns.push_back(UI);

	ZeroMemory(&SettingUI, sizeof(SettingUI));

	//Graphic_Btn6_2
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Graphic_Btn6_2");
	SettingUI.m_fSizeX = 25.f;
	SettingUI.m_fSizeY = 25.f;
	SettingUI.m_fX = 925.f;
	SettingUI.m_fY = 500.f;
	SettingUI.fAngle = 180.f;
	SettingUI.iTextureIndex = 21;


	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_pBtns.push_back(UI);

	ZeroMemory(&SettingUI, sizeof(SettingUI));

	//Graphic_BackBtn
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Graphic_BackBtn");
	SettingUI.m_fSizeX = 185.f;
	SettingUI.m_fSizeY = 50.f;
	SettingUI.m_fX = 640.f;
	SettingUI.m_fY = 570.f;
	SettingUI.fAngle = 180.f;
	SettingUI.iTextureIndex = 26;


	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&m_pBackBtn), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);

	return S_OK;
}

HRESULT CGraphicUI::Render_Fonts()
{
	CGameInstance* pInstance = g_pGameInstance;

	pInstance->Render_Font(L"VinerFonts", m_OptionName_Bloom.szString.c_str(), m_OptionName_Bloom.vPosition,
		m_OptionName_Bloom.vColor, m_OptionName_Bloom.fAngle, m_OptionName_Bloom.vFontScale);

	pInstance->Render_Font(L"VinerFonts", m_OptionName_DOF.szString.c_str(), m_OptionName_DOF.vPosition,
		m_OptionName_DOF.vColor, m_OptionName_DOF.fAngle, m_OptionName_DOF.vFontScale);

	pInstance->Render_Font(L"VinerFonts", m_OptionName_DDFOG.szString.c_str(), m_OptionName_DDFOG.vPosition,
		m_OptionName_DDFOG.vColor, m_OptionName_DDFOG.fAngle, m_OptionName_DDFOG.vFontScale);

	pInstance->Render_Font(L"VinerFonts", m_OptionName_Shadow.szString.c_str(), m_OptionName_Shadow.vPosition,
		m_OptionName_Shadow.vColor, m_OptionName_Shadow.fAngle, m_OptionName_Shadow.vFontScale);

	pInstance->Render_Font(L"VinerFonts", m_OptionName_GODRAY.szString.c_str(), m_OptionName_GODRAY.vPosition,
		m_OptionName_GODRAY.vColor, m_OptionName_GODRAY.fAngle, m_OptionName_GODRAY.vFontScale);

	pInstance->Render_Font(L"VinerFonts", m_OptionName_Lenseflare.szString.c_str(), m_OptionName_Lenseflare.vPosition,
		m_OptionName_Lenseflare.vColor, m_OptionName_Lenseflare.fAngle, m_OptionName_Lenseflare.vFontScale);

	pInstance->Render_Font(L"VinerFonts", m_BloomOnOFf.szString.c_str(), m_BloomOnOFf.vPosition,
		m_BloomOnOFf.vColor, m_BloomOnOFf.fAngle, m_BloomOnOFf.vFontScale);

	pInstance->Render_Font(L"VinerFonts", m_DOFOnOFf.szString.c_str(), m_DOFOnOFf.vPosition,
		m_DOFOnOFf.vColor, m_DOFOnOFf.fAngle, m_DOFOnOFf.vFontScale);

	pInstance->Render_Font(L"VinerFonts", m_DDFOGOnOFf.szString.c_str(), m_DDFOGOnOFf.vPosition,
		m_DDFOGOnOFf.vColor, m_DDFOGOnOFf.fAngle, m_DDFOGOnOFf.vFontScale);

	pInstance->Render_Font(L"VinerFonts", m_ShadowOnOFf.szString.c_str(), m_ShadowOnOFf.vPosition,
		m_ShadowOnOFf.vColor, m_ShadowOnOFf.fAngle, m_ShadowOnOFf.vFontScale);

	pInstance->Render_Font(L"VinerFonts", m_GODRAYOnOFf.szString.c_str(), m_GODRAYOnOFf.vPosition,
		m_GODRAYOnOFf.vColor, m_GODRAYOnOFf.fAngle, m_GODRAYOnOFf.vFontScale);

	pInstance->Render_Font(L"VinerFonts", m_LenseflareOnOFf.szString.c_str(), m_LenseflareOnOFf.vPosition,
		m_LenseflareOnOFf.vColor, m_LenseflareOnOFf.fAngle, m_LenseflareOnOFf.vFontScale);

	pInstance->Render_Font(L"VinerFonts", m_BtnBack.szString.c_str(), m_BtnBack.vPosition,
		m_BtnBack.vColor, m_BtnBack.fAngle, m_BtnBack.vFontScale);

	return S_OK;
}

HRESULT CGraphicUI::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	return S_OK;
}

CGraphicUI * CGraphicUI::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CGraphicUI*	pInstance = NEW CGraphicUI(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CGraphicUI");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CGraphicUI::Clone(void * pArg)
{
	CGraphicUI*	pInstance = NEW CGraphicUI(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created Clone CGraphicUI");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CGraphicUI::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pBackGround);
	Safe_Release(m_pBackGroundImage);
	Safe_Release(m_pBackBtn);

	for (auto& OptionName : m_pGraphicOptionUIs)
		Safe_Release(OptionName);

	for (auto& Btn : m_pBtns)
		Safe_Release(Btn);

}
