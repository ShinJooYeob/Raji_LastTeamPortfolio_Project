#include "stdafx.h"
#include "..\public\SoundUI.h"
#include "UI.h"
#include "PauseUI.h"

CSoundUI::CSoundUI(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CSoundUI::CSoundUI(const CSoundUI & rhs)
	: CGameObject(rhs)
{
}

HRESULT CSoundUI::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));


	return S_OK;
}

HRESULT CSoundUI::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	SetUp_Components();

	Ready_Layer_UI();

	m_SoundOptionName_Totla.fAngle = 0.f;
	m_SoundOptionName_Totla.szString = L"All Sound";
	m_SoundOptionName_Totla.vPosition = _float2(407.f, 190.f);
	m_SoundOptionName_Totla.vColor = _float4(1.f, 1.f, 1.f, 1.f);
	m_SoundOptionName_Totla.vFontScale = _float2(0.5f);

	m_SoundOptionName_Player.fAngle = 0.f;
	m_SoundOptionName_Player.szString = L"Player Sound";
	m_SoundOptionName_Player.vPosition = _float2(395.f, 240.f);
	m_SoundOptionName_Player.vColor = _float4(1.f, 1.f, 1.f, 1.f);
	m_SoundOptionName_Player.vFontScale = _float2(0.5f);

	m_SoundOptionName_Monster.fAngle = 0.f;
	m_SoundOptionName_Monster.szString = L"Monster Sound";
	m_SoundOptionName_Monster.vPosition = _float2(390.f, 290.f);
	m_SoundOptionName_Monster.vColor = _float4(1.f, 1.f, 1.f, 1.f);
	m_SoundOptionName_Monster.vFontScale = _float2(0.5f);

	m_SoundOptionName_MapObject.fAngle = 0.f;
	m_SoundOptionName_MapObject.szString = L"MapObject Sound";
	m_SoundOptionName_MapObject.vPosition = _float2(380.f, 340.f);
	m_SoundOptionName_MapObject.vColor = _float4(1.f, 1.f, 1.f, 1.f);
	m_SoundOptionName_MapObject.vFontScale = _float2(0.5f);

	m_SoundOptionName_Effect.fAngle = 0.f;
	m_SoundOptionName_Effect.szString = L"Effect Sound";
	m_SoundOptionName_Effect.vPosition = _float2(402.f, 390.f);
	m_SoundOptionName_Effect.vColor = _float4(1.f, 1.f, 1.f, 1.f);
	m_SoundOptionName_Effect.vFontScale = _float2(0.5f);

	m_SoundOptionName_UI.fAngle = 0.f;
	m_SoundOptionName_UI.szString = L"UI Sound";
	m_SoundOptionName_UI.vPosition = _float2(407.f, 440.f);
	m_SoundOptionName_UI.vColor = _float4(1.f, 1.f, 1.f, 1.f);
	m_SoundOptionName_UI.vFontScale = _float2(0.5f);

	m_SoundOptionName_BGM.fAngle = 0.f;
	m_SoundOptionName_BGM.szString = L"BGM Sound";
	m_SoundOptionName_BGM.vPosition = _float2(404.f, 490.f);
	m_SoundOptionName_BGM.vColor = _float4(1.f, 1.f, 1.f, 1.f);
	m_SoundOptionName_BGM.vFontScale = _float2(0.5f);

	m_BtnBack.fAngle = 0.f;
	m_BtnBack.szString = L"Back";
	m_BtnBack.vPosition = _float2(605.f, 550.f);
	m_BtnBack.vColor = _float4(1.f, 1.f, 1.f, 1.f);
	m_BtnBack.vFontScale = _float2(0.8f);

	return S_OK;

}

_int CSoundUI::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)return -1;

	if (!m_bISUIOn)
		return _int();


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
		g_pGameInstance->PlaySoundW(L"JJB_Button_1.mp3", CHANNEL_EFFECT);

		UIPosX = m_pBackBtn->Get_PosX();
		UIPosY = m_pBackBtn->Get_PosY();
		UISize = m_pBackBtn->Get_Size();

		if (MousePos.x > UIPosX - (UISize.x*0.5f) &&
			MousePos.x < UIPosX + (UISize.x*0.5f) &&
			MousePos.y > UIPosY - (UISize.y*0.5f) &&
			MousePos.y < UIPosY + (UISize.y*0.5f))
		{
			m_bISUIOn = false;
			m_pPauseUI->Set_SoundUIOn(false);
		}
	}
	if (pGameInstance->Get_DIMouseButtonState(CInput_Device::MBS_LBUTTON) & DIS_Press)
	{
		for (auto& Rect : m_pVolumeRects)
		{
			if (Rect->Get_PosX() < 709.f)
				Rect->Set_UIPosition(709.f, Rect->Get_PosY());
			else if (Rect->Get_PosX() > 934.f)
				Rect->Set_UIPosition(934.f, Rect->Get_PosY());

			UIPosX = Rect->Get_PosX();
			UIPosY = Rect->Get_PosY();
			UISize = Rect->Get_Size();

			if (MousePos.x > UIPosX - (UISize.x*0.5f) &&
				MousePos.x < UIPosX + (UISize.x*0.5f) &&
				MousePos.y > UIPosY - (UISize.y*0.5f) &&
				MousePos.y < UIPosY + (UISize.y*0.5f))
			{
				if (Rect->Get_UIName() == TEXT("Sound_TotalVolumeRectUI"))
				{
					Rect->Set_UIPosition(MousePos.x, Rect->Get_PosY());
					_float Volume = pGameInstance->Easing(TYPE_Linear, 0.f, 1.f, Rect->Get_PosX() - 709.f, 225.f);

					for (_int i = 0; i < CHANNEL_MAXCHANNEL; ++i)
					{
						_float fVol = g_pGameInstance->Get_Channel_Volume((CHANNELID)i);
						g_pGameInstance->Set_ChannelVolume((CHANNELID)i, fVol * Volume);
					}

				}

				if (Rect->Get_UIName() == TEXT("Sound_PlayerVolumeRectUI"))
				{
					Rect->Set_UIPosition(MousePos.x, Rect->Get_PosY());
					_float Volume = pGameInstance->Easing(TYPE_Linear, 0.f, 1.f, Rect->Get_PosX() - 709.f, 225.f);

					g_pGameInstance->Set_ChannelVolume(CHANNEL_PLAYER, Volume);

				}

				if (Rect->Get_UIName() == TEXT("Sound_MonsterVolumeRectUI"))
				{
					Rect->Set_UIPosition(MousePos.x, Rect->Get_PosY());
					_float Volume = pGameInstance->Easing(TYPE_Linear, 0.f, 1.f, Rect->Get_PosX() - 709.f, 225.f);

					g_pGameInstance->Set_ChannelVolume(CHANNEL_MONSTER, Volume);

				}

				if (Rect->Get_UIName() == TEXT("Sound_MapObjectVolumeRectUI"))
				{
					Rect->Set_UIPosition(MousePos.x, Rect->Get_PosY());
					_float Volume = pGameInstance->Easing(TYPE_Linear, 0.f, 1.f, Rect->Get_PosX() - 709.f, 225.f);

					g_pGameInstance->Set_ChannelVolume(CHANNEL_MAPOBJECT, Volume);

				}

				if (Rect->Get_UIName() == TEXT("Sound_EffectVolumeRectUI"))
				{
					Rect->Set_UIPosition(MousePos.x, Rect->Get_PosY());
					_float Volume = pGameInstance->Easing(TYPE_Linear, 0.f, 1.f, Rect->Get_PosX() - 709.f, 225.f);

					g_pGameInstance->Set_ChannelVolume(CHANNEL_EFFECT, Volume);

				}

				if (Rect->Get_UIName() == TEXT("Sound_UIVolumeRectUI"))
				{
					Rect->Set_UIPosition(MousePos.x, Rect->Get_PosY());
					_float Volume = pGameInstance->Easing(TYPE_Linear, 0.f, 1.f, Rect->Get_PosX() - 709.f, 225.f);

					g_pGameInstance->Set_ChannelVolume(CHANNEL_UI, Volume);

				}

				if (Rect->Get_UIName() == TEXT("Sound_BGMVolumeRectUI"))
				{
					Rect->Set_UIPosition(MousePos.x, Rect->Get_PosY());
					_float Volume = pGameInstance->Easing(TYPE_Linear, 0.f, 1.f, Rect->Get_PosX() - 709.f, 225.f);

					g_pGameInstance->Set_ChannelVolume(CHANNEL_BGM, Volume);

				}
			}
		}
	}


	//UI
	m_pBackGround->Update(fDeltaTime);
	m_pBackGroundImage->Update(fDeltaTime);
	m_pBackBtn->Update(fDeltaTime);

	for (auto& SoundUI : m_pSoundOptionUIs)
	{
		SoundUI->Update(fDeltaTime);
	}

	for (auto& BarUI : m_pSoundBarUI)
	{
		BarUI->Update(fDeltaTime);
	}

	for (auto& VolumeRectUI : m_pVolumeRects)
	{
		VolumeRectUI->Update(fDeltaTime);
	}
	return _int();
}

_int CSoundUI::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)return -1;

	if (!m_bISUIOn)
		return _int();

	//UI
	m_pBackGround->LateUpdate(fDeltaTime);
	m_pBackGroundImage->LateUpdate(fDeltaTime);
	m_pBackBtn->LateUpdate(fDeltaTime);

	for (auto& SoundUI : m_pSoundOptionUIs)
	{
		SoundUI->LateUpdate(fDeltaTime);
	}

	for (auto& BarUI : m_pSoundBarUI)
	{
		BarUI->LateUpdate(fDeltaTime);
	}

	for (auto& VolumeRectUI : m_pVolumeRects)
	{
		VolumeRectUI->LateUpdate(fDeltaTime);
	}

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

	return _int();
}

_int CSoundUI::Render()
{
	if (__super::Render() < 0)		return -1;

	Render_Fonts();

	return _int();
}

_int CSoundUI::LateRender()
{
	if (__super::LateRender() < 0)		return -1;

	return _int();
}

void CSoundUI::CheekBTN()
{
}

void CSoundUI::Set_PauseUIRef(void * UI)
{
	m_pPauseUI = (CPauseUI*)UI;
}

HRESULT CSoundUI::Ready_Layer_UI()
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
	SettingUI.pUI_Name = TEXT("Graphic_BackGround");
	SettingUI.m_fSizeX = 1280.f;
	SettingUI.m_fSizeY = 720.f;
	SettingUI.m_fX = 640.f;
	SettingUI.m_fY = 360.f;
	SettingUI.fAngle = 0.f;
	SettingUI.iTextureIndex = 136;


	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&m_pBackGround), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);


	ZeroMemory(&SettingUI, sizeof(SettingUI));

	//Sound_BackGround_Image
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

	//Sound_TotalSoundUI
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Sound_TotalSoundUI");
	SettingUI.m_fSizeX = 300.f;
	SettingUI.m_fSizeY = 30.f;
	SettingUI.m_fX = 450.f;
	SettingUI.m_fY = 200.f;
	SettingUI.fAngle = 0.f;
	SettingUI.iTextureIndex = 31;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_pSoundOptionUIs.push_back(UI);

	ZeroMemory(&SettingUI, sizeof(SettingUI));

	//Sound_PlayerSoundUI
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Sound_PlayerSoundUI");
	SettingUI.m_fSizeX = 300.f;
	SettingUI.m_fSizeY = 30.f;
	SettingUI.m_fX = 450.f;
	SettingUI.m_fY = 250.f;
	SettingUI.fAngle = 0.f;
	SettingUI.iTextureIndex = 31;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_pSoundOptionUIs.push_back(UI);

	ZeroMemory(&SettingUI, sizeof(SettingUI));

	//Sound_MonsterSoundUI
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Sound_MonsterSoundUI");
	SettingUI.m_fSizeX = 300.f;
	SettingUI.m_fSizeY = 30.f;
	SettingUI.m_fX = 450.f;
	SettingUI.m_fY = 300.f;
	SettingUI.fAngle = 0.f;
	SettingUI.iTextureIndex = 31;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_pSoundOptionUIs.push_back(UI);


	ZeroMemory(&SettingUI, sizeof(SettingUI));

	//Sound_MapObjectSoundUI
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Sound_MapObjectSoundUI");
	SettingUI.m_fSizeX = 300.f;
	SettingUI.m_fSizeY = 30.f;
	SettingUI.m_fX = 450.f;
	SettingUI.m_fY = 350.f;
	SettingUI.fAngle = 0.f;
	SettingUI.iTextureIndex = 31;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_pSoundOptionUIs.push_back(UI);

	ZeroMemory(&SettingUI, sizeof(SettingUI));

	//Sound_EffectSoundUI
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Sound_EffectSoundUI");
	SettingUI.m_fSizeX = 300.f;
	SettingUI.m_fSizeY = 30.f;
	SettingUI.m_fX = 450.f;
	SettingUI.m_fY = 400.f;
	SettingUI.fAngle = 0.f;
	SettingUI.iTextureIndex = 31;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_pSoundOptionUIs.push_back(UI);

	ZeroMemory(&SettingUI, sizeof(SettingUI));

	//Sound_UISoundUI
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Sound_UISoundUI");
	SettingUI.m_fSizeX = 300.f;
	SettingUI.m_fSizeY = 30.f;
	SettingUI.m_fX = 450.f;
	SettingUI.m_fY = 450.f;
	SettingUI.fAngle = 0.f;
	SettingUI.iTextureIndex = 31;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_pSoundOptionUIs.push_back(UI);

	ZeroMemory(&SettingUI, sizeof(SettingUI));

	//Sound_BGMUI
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Sound_BGMUI");
	SettingUI.m_fSizeX = 300.f;
	SettingUI.m_fSizeY = 30.f;
	SettingUI.m_fX = 450.f;
	SettingUI.m_fY = 500.f;
	SettingUI.fAngle = 0.f;
	SettingUI.iTextureIndex = 31;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_pSoundOptionUIs.push_back(UI);

	ZeroMemory(&SettingUI, sizeof(SettingUI));

	//Sound_TotalBarUI
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Sound_TotalBarUI");
	SettingUI.m_fSizeX = 230.f;
	SettingUI.m_fSizeY = 5.f;
	SettingUI.m_fX = 820.f;
	SettingUI.m_fY = 200.f;
	SettingUI.fAngle = 0.f;
	SettingUI.iTextureIndex = 23;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_pSoundBarUI.push_back(UI);

	ZeroMemory(&SettingUI, sizeof(SettingUI));

	//Sound_PlayerBarUI
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Sound_PlayerBarUI");
	SettingUI.m_fSizeX = 230.f;
	SettingUI.m_fSizeY = 5.f;
	SettingUI.m_fX = 820.f;
	SettingUI.m_fY = 250.f;
	SettingUI.fAngle = 0.f;
	SettingUI.iTextureIndex = 23;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_pSoundBarUI.push_back(UI);

	ZeroMemory(&SettingUI, sizeof(SettingUI));

	//Sound_MonsterBarUI
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Sound_MonsterBarUI");
	SettingUI.m_fSizeX = 230.f;
	SettingUI.m_fSizeY = 5.f;
	SettingUI.m_fX = 820.f;
	SettingUI.m_fY = 300.f;
	SettingUI.fAngle = 0.f;
	SettingUI.iTextureIndex = 23;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_pSoundBarUI.push_back(UI);

	ZeroMemory(&SettingUI, sizeof(SettingUI));

	//Sound_MapObjectBarUI
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Sound_MapObjectBarUI");
	SettingUI.m_fSizeX = 230.f;
	SettingUI.m_fSizeY = 5.f;
	SettingUI.m_fX = 820.f;
	SettingUI.m_fY = 350.f;
	SettingUI.fAngle = 0.f;
	SettingUI.iTextureIndex = 23;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_pSoundBarUI.push_back(UI);

	ZeroMemory(&SettingUI, sizeof(SettingUI));

	//Sound_EffectBarUI
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Sound_EffectBarUI");
	SettingUI.m_fSizeX = 230.f;
	SettingUI.m_fSizeY = 5.f;
	SettingUI.m_fX = 820.f;
	SettingUI.m_fY = 400.f;
	SettingUI.fAngle = 0.f;
	SettingUI.iTextureIndex = 23;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_pSoundBarUI.push_back(UI);

	ZeroMemory(&SettingUI, sizeof(SettingUI));

	//Sound_SoundBarUI
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Sound_UIBarUI");
	SettingUI.m_fSizeX = 230.f;
	SettingUI.m_fSizeY = 5.f;
	SettingUI.m_fX = 820.f;
	SettingUI.m_fY = 450.f;
	SettingUI.fAngle = 0.f;
	SettingUI.iTextureIndex = 23;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_pSoundBarUI.push_back(UI);

	ZeroMemory(&SettingUI, sizeof(SettingUI));

	//Sound_BGMBarUI
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Sound_BGMBarUI");
	SettingUI.m_fSizeX = 230.f;
	SettingUI.m_fSizeY = 5.f;
	SettingUI.m_fX = 820.f;
	SettingUI.m_fY = 500.f;
	SettingUI.fAngle = 0.f;
	SettingUI.iTextureIndex = 23;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_pSoundBarUI.push_back(UI);

	ZeroMemory(&SettingUI, sizeof(SettingUI));

	//Sound_TotalVolumeRectUI
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Sound_TotalVolumeRectUI");
	SettingUI.m_fSizeX = 20;
	SettingUI.m_fSizeY = 30.f;
	SettingUI.m_fX = 820.f;
	SettingUI.m_fY = 200.f;
	SettingUI.fAngle = 0.f;
	SettingUI.iTextureIndex = 23;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_pVolumeRects.push_back(UI);

	ZeroMemory(&SettingUI, sizeof(SettingUI));

	//Sound_PlayerVolumeRectUI
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Sound_PlayerVolumeRectUI");
	SettingUI.m_fSizeX = 20;
	SettingUI.m_fSizeY = 30.f;
	SettingUI.m_fX = 820.f;
	SettingUI.m_fY = 250.f;
	SettingUI.fAngle = 0.f;
	SettingUI.iTextureIndex = 23;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_pVolumeRects.push_back(UI);

	ZeroMemory(&SettingUI, sizeof(SettingUI));

	//Sound_MonsterVolumeRectUI
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Sound_MonsterVolumeRectUI");
	SettingUI.m_fSizeX = 20;
	SettingUI.m_fSizeY = 30.f;
	SettingUI.m_fX = 820.f;
	SettingUI.m_fY = 300.f;
	SettingUI.fAngle = 0.f;
	SettingUI.iTextureIndex = 23;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_pVolumeRects.push_back(UI);

	ZeroMemory(&SettingUI, sizeof(SettingUI));

	//Sound_MapObjectVolumeRectUI
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Sound_MapObjectVolumeRectUI");
	SettingUI.m_fSizeX = 20;
	SettingUI.m_fSizeY = 30.f;
	SettingUI.m_fX = 820.f;
	SettingUI.m_fY = 350.f;
	SettingUI.fAngle = 0.f;
	SettingUI.iTextureIndex = 23;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_pVolumeRects.push_back(UI);

	ZeroMemory(&SettingUI, sizeof(SettingUI));

	//Sound_EffectVolumeRectUI
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Sound_EffectVolumeRectUI");
	SettingUI.m_fSizeX = 20;
	SettingUI.m_fSizeY = 30.f;
	SettingUI.m_fX = 820.f;
	SettingUI.m_fY = 400.f;
	SettingUI.fAngle = 0.f;
	SettingUI.iTextureIndex = 23;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_pVolumeRects.push_back(UI);

	ZeroMemory(&SettingUI, sizeof(SettingUI));

	//Sound_SoundVolumeRectUI
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Sound_UIVolumeRectUI");
	SettingUI.m_fSizeX = 20;
	SettingUI.m_fSizeY = 30.f;
	SettingUI.m_fX = 820.f;
	SettingUI.m_fY = 450.f;
	SettingUI.fAngle = 0.f;
	SettingUI.iTextureIndex = 23;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_pVolumeRects.push_back(UI);

	ZeroMemory(&SettingUI, sizeof(SettingUI));

	//Sound_BGMVolumeRectUI
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Sound_BGMVolumeRectUI");
	SettingUI.m_fSizeX = 20;
	SettingUI.m_fSizeY = 30.f;
	SettingUI.m_fX = 820.f;
	SettingUI.m_fY = 500.f;
	SettingUI.fAngle = 0.f;
	SettingUI.iTextureIndex = 23;

	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&UI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);
	m_pVolumeRects.push_back(UI);


	ZeroMemory(&SettingUI, sizeof(SettingUI));

	//Sound_BackBtn
	SettingUI.bClick = false;
	SettingUI.bMove = false;
	SettingUI.bDraw = true;
	SettingUI.bColl = false;
	SettingUI.iLevelIndex = m_eNowSceneNum;
	SettingUI.pUI_Name = TEXT("Sound_BackBtn");
	SettingUI.m_fSizeX = 185.f;
	SettingUI.m_fSizeY = 50.f;
	SettingUI.m_fX = 640.f;
	SettingUI.m_fY = 570.f;
	SettingUI.fAngle = 180.f;
	SettingUI.iTextureIndex = 26;


	pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&m_pBackBtn), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_UI), &SettingUI);

	return S_OK;
}

HRESULT CSoundUI::Render_Fonts()
{
	CGameInstance* pInstance = g_pGameInstance;

	pInstance->Render_Font(L"VinerFonts", m_SoundOptionName_Totla.szString.c_str(), m_SoundOptionName_Totla.vPosition,
		m_SoundOptionName_Totla.vColor, m_SoundOptionName_Totla.fAngle, m_SoundOptionName_Totla.vFontScale);

	pInstance->Render_Font(L"VinerFonts", m_SoundOptionName_Player.szString.c_str(), m_SoundOptionName_Player.vPosition,
		m_SoundOptionName_Player.vColor, m_SoundOptionName_Player.fAngle, m_SoundOptionName_Player.vFontScale);

	pInstance->Render_Font(L"VinerFonts", m_SoundOptionName_Monster.szString.c_str(), m_SoundOptionName_Monster.vPosition,
		m_SoundOptionName_Monster.vColor, m_SoundOptionName_Monster.fAngle, m_SoundOptionName_Monster.vFontScale);

	pInstance->Render_Font(L"VinerFonts", m_SoundOptionName_MapObject.szString.c_str(), m_SoundOptionName_MapObject.vPosition,
		m_SoundOptionName_MapObject.vColor, m_SoundOptionName_MapObject.fAngle, m_SoundOptionName_MapObject.vFontScale);

	pInstance->Render_Font(L"VinerFonts", m_SoundOptionName_Effect.szString.c_str(), m_SoundOptionName_Effect.vPosition,
		m_SoundOptionName_Effect.vColor, m_SoundOptionName_Effect.fAngle, m_SoundOptionName_Effect.vFontScale);

	pInstance->Render_Font(L"VinerFonts", m_SoundOptionName_UI.szString.c_str(), m_SoundOptionName_UI.vPosition,
		m_SoundOptionName_UI.vColor, m_SoundOptionName_UI.fAngle, m_SoundOptionName_UI.vFontScale);

	pInstance->Render_Font(L"VinerFonts", m_SoundOptionName_BGM.szString.c_str(), m_SoundOptionName_BGM.vPosition,
		m_SoundOptionName_BGM.vColor, m_SoundOptionName_BGM.fAngle, m_SoundOptionName_BGM.vFontScale);

	pInstance->Render_Font(L"VinerFonts", m_BtnBack.szString.c_str(), m_BtnBack.vPosition,
		m_BtnBack.vColor, m_BtnBack.fAngle, m_BtnBack.vFontScale);

	return S_OK;
}

HRESULT CSoundUI::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	return S_OK;
}

CSoundUI * CSoundUI::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CSoundUI*	pInstance = NEW CSoundUI(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CSoundUI");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CSoundUI::Clone(void * pArg)
{
	CSoundUI*	pInstance = NEW CSoundUI(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created Clone CSoundUI");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSoundUI::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pBackGround);
	Safe_Release(m_pBackGroundImage);
	Safe_Release(m_pBackBtn);

	for (auto& SoundUI : m_pSoundOptionUIs)
		Safe_Release(SoundUI);

	for (auto& BarUI : m_pSoundBarUI)
		Safe_Release(BarUI);

	for (auto& VolumeUI : m_pVolumeRects)
		Safe_Release(VolumeUI);

	m_pSoundOptionUIs.clear();
	m_pSoundBarUI.clear();
	m_pVolumeRects.clear();
}
