#include "stdafx.h"
#include "..\public\LobbyUI.h"
#include "UI.h"
#include "Scene_Loading.h"


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

	_float2				MousePos;
	POINT				ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);
	MousePos.x = (_float)ptMouse.x;
	MousePos.y = (_float)ptMouse.y;


	if (!m_bIsStageOn)
	{
		for (auto& UI : m_vecLobbyBtns)
		{
			UI->Set_3DScaled(_float3(2.f, 0.5f, 1.f));

			if (MousePos.x > 38.f && MousePos.x < 314.f && MousePos.y > 441.f && MousePos.y < 503.f && m_bIsStageOn == false)
			{
				if (UI->Get_UIName() == TEXT("Lobby_First_Btn"))
				{
					UI->Set_3DScaled(_float3(2.1f, 0.6f, 1.f));

					//UI->Set_IsRevolutionCCW(true);
					//UI->Set_RevolutionGoalAngle(360.f);
				}
			}

			if (MousePos.x > 35.f && MousePos.x < 316.f && MousePos.y > 510.f && MousePos.y < 592.f && m_bIsStageOn == false)
			{
				if (UI->Get_UIName() == TEXT("Lobby_Stage_Btn"))
				{
					UI->Set_3DScaled(_float3(2.1f, 0.6f, 1.f));
					//UI->Set_IsRevolutionCCW(true);
					//UI->Set_RevolutionGoalAngle(360.f);
				}
			}

			if (MousePos.x > 35.f && MousePos.x < 316.f && MousePos.y > 610.f && MousePos.y < 717.f && m_bIsStageOn == false)
			{
				if (UI->Get_UIName() == TEXT("Lobby_Exit_Btn"))
				{
					UI->Set_3DScaled(_float3(2.1f, 0.6f, 1.f));
					//UI->Set_IsRevolutionCCW(true);
					//UI->Set_RevolutionGoalAngle(360.f);
				}
			}
		}
	}



	if (g_pGameInstance->Get_DIMouseButtonState(CInput_Device::MBS_LBUTTON) & DIS_Down)
	{
		//string ttszLog = "MousePosX " + to_string(MousePos.x) +" " + "MousePosY" + to_string(MousePos.y) + "\n";
		//wstring ttDebugLog;
		//ttDebugLog.assign(ttszLog.begin(), ttszLog.end());
		//OutputDebugStringW(ttDebugLog.c_str());

		for (auto& UI : m_vecLobbyBtns)
		{
			if (MousePos.x > 38.f && MousePos.x < 314.f && MousePos.y > 441.f && MousePos.y < 503.f && m_bIsStageOn == false)
			{
				if (UI->Get_UIName() == TEXT("Lobby_First_Btn"))
				{
					g_pGameInstance->Get_NowScene()->Set_SceneChanging(SCENE_STAGE1);
				}
			}

			if (MousePos.x > 35.f && MousePos.x < 316.f && MousePos.y > 510.f && MousePos.y < 592.f && m_bIsStageOn == false)
			{
				if (UI->Get_UIName() == TEXT("Lobby_Stage_Btn"))
				{
					m_bIsStageOn = true;
					for (auto& UI : m_vec3DStageUI)
					{
						UI->UI_RevolutionCWInitialization(180.f);
					}
				}
			}

			if (MousePos.x > 35.f && MousePos.x < 316.f && MousePos.y > 610.f && MousePos.y < 717.f && m_bIsStageOn == false)
			{
				if (UI->Get_UIName() == TEXT("Lobby_Exit_Btn"))
				{
					DestroyWindow(g_hWnd);
				}
			}
		}


	}


	FAILED_CHECK(m_pBackGround->Update(fDeltaTime));
	FAILED_CHECK(m_pBackGround2->Update(fDeltaTime));
	FAILED_CHECK(m_RajiText->Update(fDeltaTime));
	m_RajiText->Set_UV_Y(0);



	for (auto& UI : m_vecLobbyBtns)
		UI->Update(fDeltaTime);

	if (m_bIsStageOn)
	{
		for (auto& UI : m_vec3DStageUI)
			UI->Update(fDeltaTime);
	}
	
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
	FAILED_CHECK(m_pBackGround2->Render());

	if (!m_bIsStageOn)
	{
		FAILED_CHECK(m_RajiText->Render());
	}


	for (auto& UI : m_vecLobbyBtns)
		UI->Render();

	if (m_bIsStageOn)
	{
		for (auto& UI : m_vec3DStageUI)
			UI->Render();
	}
	
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
		SettingUI.pUI_Name = TEXT("Lobby_BG_2");
		SettingUI.m_fSizeX = 1280.f;
		SettingUI.m_fSizeY = 720.f;
		SettingUI.m_fX = 640.f;
		SettingUI.m_fY = 360.f;
		SettingUI.fAngle = 0.f;
		SettingUI.iTextureIndex = 136;

		pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&m_pBackGround2), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
		NULL_CHECK_RETURN(m_pBackGround2, E_FAIL);
		//m_pBackGround2->Set_RenderSortValue(2.f);
	}

	{
		 
		CUI* UI = nullptr;
		CUI::SETTING_UI SettingUI;
		ZeroMemory(&SettingUI, sizeof(SettingUI));

		//Lobby_RajiText
		SettingUI.bClick = false;
		SettingUI.bMove = false;
		SettingUI.bDraw = true;
		SettingUI.bColl = false;
		SettingUI.iLevelIndex = m_eNowSceneNum;
		SettingUI.pUI_Name = TEXT("Lobby_RajiText");
		SettingUI.m_fSizeX = 600.f;
		SettingUI.m_fSizeY = 360.f;
		SettingUI.m_fX = 640.f;
		SettingUI.m_fY = 230.f;
		SettingUI.fAngle = 0.f;
		SettingUI.iTextureIndex = 135;
		SettingUI.bSettingOtherTexture = true;
		SettingUI.iNoiseTextureIndex = 85;
		SettingUI.iMaskTextureIndex = 41;

		pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&m_RajiText), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
		NULL_CHECK_RETURN(m_RajiText, E_FAIL);
		m_RajiText->Set_PassIndex(16);
		//m_RajiText->Set_PassIndex(15);
	}

	{
		CUI* UI = nullptr;
		CUI::SETTING_UI SettingUI;
		ZeroMemory(&SettingUI, sizeof(SettingUI));

		//Lobby_First_Btn
		SettingUI.bClick = false;
		SettingUI.bMove = false;
		SettingUI.bDraw = true;
		SettingUI.bColl = false;
		SettingUI.iLevelIndex = m_eNowSceneNum;
		SettingUI.pUI_Name = TEXT("Lobby_First_Btn");
		SettingUI.v3DUIPosition = _float3(-3.f, 0.2f, 5.f);
		SettingUI.v3DUIScaled = _float3(2.f, 0.5f, 1.f);
		SettingUI.fAngle = 30.f;
		SettingUI.iTextureIndex = 141;
		SettingUI.eUIType = CUI::UI_3D;

		pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
		NULL_CHECK_RETURN(UI, E_FAIL);

		m_vecLobbyBtns.push_back(UI);
	}

	{
		CUI* UI = nullptr;
		CUI::SETTING_UI SettingUI;
		ZeroMemory(&SettingUI, sizeof(SettingUI));

		//Lobby_Stage_Btn
		SettingUI.bClick = false;
		SettingUI.bMove = false;
		SettingUI.bDraw = true;
		SettingUI.bColl = false;
		SettingUI.iLevelIndex = m_eNowSceneNum;
		SettingUI.pUI_Name = TEXT("Lobby_Stage_Btn");
		SettingUI.v3DUIPosition = _float3(-3.f, -0.5f, 5.f);
		SettingUI.v3DUIScaled = _float3(2.f, 0.5f, 1.f);
		SettingUI.fAngle = 30.f;
		SettingUI.iTextureIndex = 142;
		SettingUI.eUIType = CUI::UI_3D;

		pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
		NULL_CHECK_RETURN(UI, E_FAIL);

		m_vecLobbyBtns.push_back(UI);
	}

	{
		CUI* UI = nullptr;
		CUI::SETTING_UI SettingUI;
		ZeroMemory(&SettingUI, sizeof(SettingUI));

		//Lobby_Exit_Btn
		SettingUI.bClick = false;
		SettingUI.bMove = false;
		SettingUI.bDraw = true;
		SettingUI.bColl = false;
		SettingUI.iLevelIndex = m_eNowSceneNum;
		SettingUI.pUI_Name = TEXT("Lobby_Exit_Btn");
		SettingUI.v3DUIPosition = _float3(-3.f, -1.2f, 5.f);
		SettingUI.v3DUIScaled = _float3(2.f, 0.5f, 1.f);
		SettingUI.fAngle = 30.f;
		SettingUI.iTextureIndex = 143;
		SettingUI.eUIType = CUI::UI_3D;

		pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
		NULL_CHECK_RETURN(UI, E_FAIL);

		m_vecLobbyBtns.push_back(UI);
	}

	{
		CUI* UI = nullptr;
		CUI::SETTING_UI SettingUI;
		ZeroMemory(&SettingUI, sizeof(SettingUI));

		//Stage_GetIn_Btn
		SettingUI.bClick = false;
		SettingUI.bMove = false;
		SettingUI.bDraw = true;
		SettingUI.bColl = false;
		SettingUI.iLevelIndex = m_eNowSceneNum;
		SettingUI.pUI_Name = TEXT("Stage_GetIn_Btn");
		SettingUI.v3DUIPosition = _float3(-3.f, 0.2f, 5.f);
		SettingUI.v3DUIScaled = _float3(2.f, 0.5f, 1.f);
		SettingUI.fAngle = 40.f;
		SettingUI.iTextureIndex = 144;
		SettingUI.eUIType = CUI::UI_3D;

		pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
		NULL_CHECK_RETURN(UI, E_FAIL);

		m_vec3DStageUI.push_back(UI);
	}

	{
		CUI* UI = nullptr;
		CUI::SETTING_UI SettingUI;
		ZeroMemory(&SettingUI, sizeof(SettingUI));

		//Stage_Return_Btn
		SettingUI.bClick = false;
		SettingUI.bMove = false;
		SettingUI.bDraw = true;
		SettingUI.bColl = false;
		SettingUI.iLevelIndex = m_eNowSceneNum;
		SettingUI.pUI_Name = TEXT("Stage_Return_Btn");
		SettingUI.v3DUIPosition = _float3(-3.f, -0.5f, 5.f);
		SettingUI.v3DUIScaled = _float3(2.f, 0.5f, 1.f);
		SettingUI.fAngle = 40.f;
		SettingUI.iTextureIndex = 145;
		SettingUI.eUIType = CUI::UI_3D;

		pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
		NULL_CHECK_RETURN(UI, E_FAIL);

		m_vec3DStageUI.push_back(UI);
	}

	{
		CUI* UI = nullptr;
		CUI::SETTING_UI SettingUI;
		ZeroMemory(&SettingUI, sizeof(SettingUI));

		//Stage_Number
		SettingUI.bClick = false;
		SettingUI.bMove = false;
		SettingUI.bDraw = true;
		SettingUI.bColl = false;
		SettingUI.iLevelIndex = m_eNowSceneNum;
		SettingUI.pUI_Name = TEXT("Stage_Number");
		SettingUI.v3DUIPosition = _float3(-3.f, 1.3f, 5.f);
		SettingUI.v3DUIScaled = _float3(1.7f, 0.3f, 1.f);
		SettingUI.fAngle = 40.f;
		SettingUI.iTextureIndex = 146;
		SettingUI.eUIType = CUI::UI_3D;

		pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
		NULL_CHECK_RETURN(UI, E_FAIL);

		m_vec3DStageUI.push_back(UI);
	}

	{
		CUI* UI = nullptr;
		CUI::SETTING_UI SettingUI;
		ZeroMemory(&SettingUI, sizeof(SettingUI));

		//Stage_BackGround
		SettingUI.bClick = false;
		SettingUI.bMove = false;
		SettingUI.bDraw = true;
		SettingUI.bColl = false;
		SettingUI.iLevelIndex = m_eNowSceneNum;
		SettingUI.pUI_Name = TEXT("Stage_BackGround");
		SettingUI.v3DUIPosition = _float3(1.0f, 1.5f, 5.f);
		SettingUI.v3DUIScaled = _float3(10.f, 7.f, 7.f);
		SettingUI.fAngle = -35.f;
		SettingUI.iTextureIndex = 47;
		SettingUI.eUIType = CUI::UI_3D;

		pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
		NULL_CHECK_RETURN(UI, E_FAIL);

		m_vec3DStageUI.push_back(UI);
	}

	{
		CUI* UI = nullptr;
		CUI::SETTING_UI SettingUI;
		ZeroMemory(&SettingUI, sizeof(SettingUI));

		//Stage_Btn1
		SettingUI.bClick = false;
		SettingUI.bMove = false;
		SettingUI.bDraw = true;
		SettingUI.bColl = false;
		SettingUI.iLevelIndex = m_eNowSceneNum;
		SettingUI.pUI_Name = TEXT("Stage_Btn1");
		SettingUI.v3DUIPosition = _float3(-0.4f, 2.3f, 5.f);
		SettingUI.v3DUIScaled = _float3(1.7f, 0.3f, 1.f);
		SettingUI.fAngle = -35.f;
		SettingUI.iTextureIndex = 146;
		SettingUI.eUIType = CUI::UI_3D;

		pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
		NULL_CHECK_RETURN(UI, E_FAIL);

		m_vec3DStageUI.push_back(UI);
	}

	{
		CUI* UI = nullptr;
		CUI::SETTING_UI SettingUI;
		ZeroMemory(&SettingUI, sizeof(SettingUI));

		//Stage_Btn2
		SettingUI.bClick = false;
		SettingUI.bMove = false;
		SettingUI.bDraw = true;
		SettingUI.bColl = false;
		SettingUI.iLevelIndex = m_eNowSceneNum;
		SettingUI.pUI_Name = TEXT("Stage_Btn2");
		SettingUI.v3DUIPosition = _float3(-0.4f, 2.f, 5.f);
		SettingUI.v3DUIScaled = _float3(1.7f, 0.3f, 1.f);
		SettingUI.fAngle = -35.f;
		SettingUI.iTextureIndex = 147;
		SettingUI.eUIType = CUI::UI_3D;

		pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
		NULL_CHECK_RETURN(UI, E_FAIL);

		m_vec3DStageUI.push_back(UI);
	}

	{
		CUI* UI = nullptr;
		CUI::SETTING_UI SettingUI;
		ZeroMemory(&SettingUI, sizeof(SettingUI));

		//Stage_Btn3
		SettingUI.bClick = false;
		SettingUI.bMove = false;
		SettingUI.bDraw = true;
		SettingUI.bColl = false;
		SettingUI.iLevelIndex = m_eNowSceneNum;
		SettingUI.pUI_Name = TEXT("Stage_Btn3");
		SettingUI.v3DUIPosition = _float3(-0.4f, 1.7f, 5.f);
		SettingUI.v3DUIScaled = _float3(1.7f, 0.3f, 1.f);
		SettingUI.fAngle = -35.f;
		SettingUI.iTextureIndex = 148;
		SettingUI.eUIType = CUI::UI_3D;

		pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
		NULL_CHECK_RETURN(UI, E_FAIL);

		m_vec3DStageUI.push_back(UI);
	}

	{
		CUI* UI = nullptr;
		CUI::SETTING_UI SettingUI;
		ZeroMemory(&SettingUI, sizeof(SettingUI));

		//Stage_Btn4
		SettingUI.bClick = false;
		SettingUI.bMove = false;
		SettingUI.bDraw = true;
		SettingUI.bColl = false;
		SettingUI.iLevelIndex = m_eNowSceneNum;
		SettingUI.pUI_Name = TEXT("Stage_Btn4");
		SettingUI.v3DUIPosition = _float3(-0.4f, 1.4f, 5.f);
		SettingUI.v3DUIScaled = _float3(1.7f, 0.3f, 1.f);
		SettingUI.fAngle = -35.f;
		SettingUI.iTextureIndex = 149;
		SettingUI.eUIType = CUI::UI_3D;

		pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
		NULL_CHECK_RETURN(UI, E_FAIL);

		m_vec3DStageUI.push_back(UI);
	}

	{
		CUI* UI = nullptr;
		CUI::SETTING_UI SettingUI;
		ZeroMemory(&SettingUI, sizeof(SettingUI));

		//Stage_Btn5
		SettingUI.bClick = false;
		SettingUI.bMove = false;
		SettingUI.bDraw = true;
		SettingUI.bColl = false;
		SettingUI.iLevelIndex = m_eNowSceneNum;
		SettingUI.pUI_Name = TEXT("Stage_Btn5");
		SettingUI.v3DUIPosition = _float3(-0.4f, 1.1f, 5.f);
		SettingUI.v3DUIScaled = _float3(1.7f, 0.3f, 1.f);
		SettingUI.fAngle = -35.f;
		SettingUI.iTextureIndex = 150;
		SettingUI.eUIType = CUI::UI_3D;

		pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
		NULL_CHECK_RETURN(UI, E_FAIL);

		m_vec3DStageUI.push_back(UI);
	}

	{
		CUI* UI = nullptr;
		CUI::SETTING_UI SettingUI;
		ZeroMemory(&SettingUI, sizeof(SettingUI));

		//Stage_Btn6
		SettingUI.bClick = false;
		SettingUI.bMove = false;
		SettingUI.bDraw = true;
		SettingUI.bColl = false;
		SettingUI.iLevelIndex = m_eNowSceneNum;
		SettingUI.pUI_Name = TEXT("Stage_Btn6");
		SettingUI.v3DUIPosition = _float3(-0.4f, 0.8f, 5.f);
		SettingUI.v3DUIScaled = _float3(1.7f, 0.3f, 1.f);
		SettingUI.fAngle = -35.f;
		SettingUI.iTextureIndex = 151;
		SettingUI.eUIType = CUI::UI_3D;

		pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
		NULL_CHECK_RETURN(UI, E_FAIL);

		m_vec3DStageUI.push_back(UI);
	}

	{
		CUI* UI = nullptr;
		CUI::SETTING_UI SettingUI;
		ZeroMemory(&SettingUI, sizeof(SettingUI));

		//Stage_Btn7
		SettingUI.bClick = false;
		SettingUI.bMove = false;
		SettingUI.bDraw = true;
		SettingUI.bColl = false;
		SettingUI.iLevelIndex = m_eNowSceneNum;
		SettingUI.pUI_Name = TEXT("Stage_Btn7");
		SettingUI.v3DUIPosition = _float3(-0.4f, 0.5f, 5.f);
		SettingUI.v3DUIScaled = _float3(1.7f, 0.3f, 1.f);
		SettingUI.fAngle = -35.f;
		SettingUI.iTextureIndex = 152;
		SettingUI.eUIType = CUI::UI_3D;

		pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
		NULL_CHECK_RETURN(UI, E_FAIL);

		m_vec3DStageUI.push_back(UI);
	}

	{
		CUI* UI = nullptr;
		CUI::SETTING_UI SettingUI;
		ZeroMemory(&SettingUI, sizeof(SettingUI));

		//Stage_Image
		SettingUI.bClick = false;
		SettingUI.bMove = false;
		SettingUI.bDraw = true;
		SettingUI.bColl = false;
		SettingUI.iLevelIndex = m_eNowSceneNum;
		SettingUI.pUI_Name = TEXT("Stage_Image");
		SettingUI.v3DUIPosition = _float3(2.f, 1.5f, 5.f);
		SettingUI.v3DUIScaled = _float3(2.75f, 3.3f, 1.f);
		SettingUI.fAngle = -35.f;
		SettingUI.iTextureIndex = 159;
		SettingUI.eUIType = CUI::UI_3D;

		pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
		NULL_CHECK_RETURN(UI, E_FAIL);

		m_vec3DStageUI.push_back(UI);
	}

	{
		CUI* UI = nullptr;
		CUI::SETTING_UI SettingUI;
		ZeroMemory(&SettingUI, sizeof(SettingUI));

		//Stage_ImageBack
		SettingUI.bClick = false;
		SettingUI.bMove = false;
		SettingUI.bDraw = true;
		SettingUI.bColl = false;
		SettingUI.iLevelIndex = m_eNowSceneNum;
		SettingUI.pUI_Name = TEXT("Stage_ImageBack");
		SettingUI.v3DUIPosition = _float3(2.f, 1.5f, 5.f);
		SettingUI.v3DUIScaled = _float3(3.5f, 3.5f, 1.f);
		SettingUI.fAngle = -35.f;
		SettingUI.iTextureIndex = 31;
		SettingUI.eUIType = CUI::UI_3D;

		pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
		NULL_CHECK_RETURN(UI, E_FAIL);

		m_vec3DStageUI.push_back(UI);
	}



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
	Safe_Release(m_pBackGround2);

	for (auto& UI : m_vecLobbyBtns)
		Safe_Release(UI);

	for (auto& UI : m_vec3DStageUI)
		Safe_Release(UI);

	m_vecLobbyBtns.clear();
	m_vec3DStageUI.clear();
}
