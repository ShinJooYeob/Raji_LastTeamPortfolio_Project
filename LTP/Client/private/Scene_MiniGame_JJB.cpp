#include "stdafx.h"
#include "..\public\Scene_MiniGame_JJB.h"
#include "Scene_Loading.h"
#include "Camera_Main.h"
#include "MiniGameBuilding.h"
#include "GaolCollider.h"
#include "TaikoUI.h"
#include "UI.h"
#include "PartilceCreateMgr.h"

CScene_MiniGame_JJB::CScene_MiniGame_JJB(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CScene(pDevice, pDeviceContext)
{
}

HRESULT CScene_MiniGame_JJB::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;
	m_pUtilMgr = GetSingle(CUtilityMgr);


	FAILED_CHECK(Ready_Layer_GoalCollider(TAG_LAY(Layer_MiniGameGoalCollider)));
	FAILED_CHECK(Ready_Light());
	FAILED_CHECK(Ready_Layer_MainCamera(TAG_LAY(Layer_Camera_Main)));
	FAILED_CHECK(Ready_Layer_UI(TAG_LAY(Layer_UI)));
	FAILED_CHECK(Ready_Layer_Player(TAG_LAY(Layer_MiniGamePlayer_JJB)));
	FAILED_CHECK(Ready_Layer_Monster(TAG_LAY(Layer_MiniGameMonster_JJB)));

	FAILED_CHECK(Ready_PostPorcessing());

	m_pMainCam->Ortho_OnOff(true, 30.f);

	TimingNoteSetting();

	return S_OK;
}

_int CScene_MiniGame_JJB::Update(_double fDeltaTime)
{
	m_pMainCam->Ortho_OnOff(true, 30.f);

	if (__super::Update(fDeltaTime) < 0)
		return -1;

	if (m_fReadyTime > 0 && m_bIsStartBGM == false && m_bIsGameEnd == false)
	{
		m_fReadyTime -= (_float)fDeltaTime;

		if (m_bIsSceneChange)
		{
			m_fChangeWaitingTime += (_float)fDeltaTime;

			if (m_fChangeWaitingTime > 5.f)
			{
				m_bIsSceneChange = false;
				m_fChangeWaitingTime = 0.f;
				//여기서 하심 됨
			}
		}

		if (m_fReadyTime <= 0)
		{
			CUI* StartTimeUI = nullptr;

			StartTimeUI = m_pTaikoUI->Get_StartTimeUI();
			StartTimeUI->Set_IsDraw(false);
			m_pTaikoUI->Set_IsQuestUIStart();

			m_fReadyTime = 3.5f;
			m_bIsStartBGM = true;
			g_pGameInstance->PlayBGM(TEXT("sacuranbo.wav"));
			g_pGameInstance->PlaySoundW(L"Taiko_begin_1P.wav", CHANNEL_EFFECT);
		}

		if (m_fReadyTime > 3.f)
		{
			CUI* StartTimeUI = nullptr;

			StartTimeUI = m_pTaikoUI->Get_StartTimeUI();

			StartTimeUI->Set_TextureIndex(16);
		}
		else if (m_fReadyTime > 2.f && m_fReadyTime < 3.f)
		{
			CUI* StartTimeUI = nullptr;

			StartTimeUI = m_pTaikoUI->Get_StartTimeUI();

			StartTimeUI->Set_TextureIndex(15);
		}
		else if (m_fReadyTime > 1.f && m_fReadyTime < 2.f)
		{
			CUI* StartTimeUI = nullptr;

			StartTimeUI = m_pTaikoUI->Get_StartTimeUI();

			StartTimeUI->Set_TextureIndex(14);

		}
		else
		{
			CUI* StartTimeUI = nullptr;

			StartTimeUI = m_pTaikoUI->Get_StartTimeUI();

			StartTimeUI->Set_TextureIndex(24);
			StartTimeUI->Set_Size(150.f, 150.f);
		}
	}

	if (m_bIsStartBGM)
	{
		m_AccumulateTime += (_float)fDeltaTime;
	}

	if (g_pGameInstance->Get_DIKeyState(DIK_RETURN)&DIS_Down)
	{
		CMiniGameBuilding::Copy_NowScreenToBuliding(CMiniGameBuilding::MINIGAME_RHYTHM);


		FAILED_CHECK(m_pUtilMgr->Clear_RenderGroup_forSceneChange());
		FAILED_CHECK(g_pGameInstance->Scene_Change(CScene_Loading::Create(m_pDevice, m_pDeviceContext, SCENEID::SCENE_STAGE2), SCENEID::SCENE_LOADING));
		return 0;
	}


	if (m_iSceneStartChecker <= SceneChangeCopyFrame && CScene_Loading::m_iLoadingKinds == CScene_Loading::LOADINGKINDS_NORMAL)
	{
		FAILED_CHECK(m_pUtilMgr->Get_Renderer()->Copy_LastDeferredTexture());
		FAILED_CHECK(m_pUtilMgr->Get_Renderer()->Copy_LastDeferredToToonShadingTexture(1.f, true));
	}

	/*if (g_pGameInstance->Get_DIKeyState(DIK_RETURN)&DIS_Down)
	{

	CMiniGameBuilding::Copy_NowScreenToBuliding(CMiniGameBuilding::MINIGAME_RHYTHM);

	FAILED_CHECK(m_pUtilMgr->Clear_RenderGroup_forSceneChange());
	FAILED_CHECK(g_pGameInstance->Scene_Change(CScene_Loading::Create(m_pDevice, m_pDeviceContext, SCENEID::SCENE_LOBY), SCENEID::SCENE_LOADING));
	return 0;
	}*/

	//if (g_pGameInstance->Get_DIKeyState(DIK_T)&DIS_Down)
	//{
	//   m_StartBGM = true;
	//   g_pGameInstance->PlayBGM(TEXT("sacuranbo.wav"));
	//}

	if (m_iVecCount < m_vecNoteTimings.size() && m_bIsStartBGM)
		ProduceTimingNote();

	if (m_iVecCount >= m_vecNoteTimings.size() && m_bIsGameEnd == false && 10 == m_NoteBigRedMonsterList.size())
	{
		g_pGameInstance->Stop_AllChannel();

		m_bIsStartBGM = false;
		m_bIsGameEnd = true;

		g_pGameInstance->PlaySoundW(L"Taiko_end.wav", CHANNEL_EFFECT);
	}

	if (m_bIsGameEnd && m_fEndTime > 0.f)
	{
		m_fEndTime -= (_float)fDeltaTime;

		if (m_fEndTime <= 0.f)
		{
			m_fEndTime = 0.f;
			m_bIsResult = true;
			m_bIsSceneChange = true;
			if (m_iMaxCombo > 70)
				m_pTaikoUI->Set_ShowResultUI(28);
			else
				m_pTaikoUI->Set_ShowResultUI(29);
		}
	}

	if (m_bIsResult)
	{
		m_bIsResult = false;
		if (m_iMaxCombo >= 70)
		{
			g_pGameInstance->PlaySoundW(L"Taiko_clear_pt_1P.wav", CHANNEL_EFFECT);

		}
		else
		{
			g_pGameInstance->PlaySoundW(L"Taiko_fail1_1P.wav", CHANNEL_EFFECT);
		}
	}

	return _int();
}

_int CScene_MiniGame_JJB::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)
		return -1;

	if (m_iMaxCombo < m_iComboCount)
		m_iMaxCombo = m_iComboCount;

	//KeyClick();

	return _int();
}

_int CScene_MiniGame_JJB::Render()
{
	if (__super::Render() < 0)
		return -1;

	if (CScene_Loading::m_iLoadingKinds == CScene_Loading::LOADINGKINDS_NORMAL)
	{
		if (m_fSceneStartTimer < 0.5f)
		{
			FAILED_CHECK(m_pUtilMgr->SCD_Rendering_Rolling(((_float)m_fSceneStartTimer), 0.5f, L"Target_ToonDeferredSceneChaging2"));
		}
		else if (m_fSceneStartTimer < 2.5f)
		{
			FAILED_CHECK(m_pUtilMgr->SCD_Rendering_FadeOut(((_float)m_fSceneStartTimer - 0.5f), 2.f, L"Target_ToonDeferredSceneChaging2"));
		}
	}

	return _int();
}

_int CScene_MiniGame_JJB::LateRender()
{
	if (__super::LateRender() < 0)
		return -1;

	return _int();
}

void CScene_MiniGame_JJB::Push_NoteSmallRedMonsterLsit(CTaiko_Monster * pMonster)
{
	m_NoteSmallRedMonsterList.push_back(pMonster);
}

void CScene_MiniGame_JJB::Push_NoteBigRedMonsterLsit(CTaiko_Monster * pMonster)
{
	m_NoteBigRedMonsterList.push_back(pMonster);
}

void CScene_MiniGame_JJB::Push_NoteSmallBlueMonsterLsit(CTaiko_Monster * pMonster)
{
	m_NoteSmallBlueMonsterList.push_back(pMonster);
}

void CScene_MiniGame_JJB::Push_NoteBigBlueMonsterLsit(CTaiko_Monster * pMonster)
{
	m_NoteBigBlueMonsterList.push_back(pMonster);
}

void CScene_MiniGame_JJB::Pop_NoteSmallRedMonsterLsit()
{
	CTaiko_Monster* pMonster = m_NoteSmallRedMonsterList.front();
	pMonster->UseON();
	m_NoteSmallRedMonsterList.pop_front();
}

void CScene_MiniGame_JJB::Pop_NoteBigRedMonsterLsit()
{
	CTaiko_Monster* pMonster = m_NoteBigRedMonsterList.front();
	pMonster->UseON();
	m_NoteBigRedMonsterList.pop_front();
}

void CScene_MiniGame_JJB::Pop_NoteSmallBlueMonsterLsit()
{
	CTaiko_Monster* pMonster = m_NoteSmallBlueMonsterList.front();
	pMonster->UseON();
	m_NoteSmallBlueMonsterList.pop_front();
}

void CScene_MiniGame_JJB::Pop_NoteBigBlueMonsterLsit()
{
	CTaiko_Monster* pMonster = m_NoteBigBlueMonsterList.front();
	pMonster->UseON();
	m_NoteBigBlueMonsterList.pop_front();
}

void CScene_MiniGame_JJB::TimingNoteSetting()
{
#pragma region NoteVector
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 9.219098f,   TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 9.585941f,   TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 9.752646f,   TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 9.952678f,   TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 10.119604f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 10.319639f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 10.653112f,  TYPE_BIGRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 11.003173f,  TYPE_BIGRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 12.939631f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 13.106457f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 13.306906f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 13.825568f,  TYPE_BIGRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 15.761685f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 15.945096f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 16.130106f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 16.630335f,  TYPE_BIGRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 18.634733f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 18.801529f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 18.984922f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 19.502787f,  TYPE_BIGRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 20.738346f,  TYPE_SMALLBLUE });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 21.088903f,  TYPE_SMALLBLUE });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 21.439417f,  TYPE_SMALLBLUE });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 21.768911f,  TYPE_SMALLBLUE });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 21.968937f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 22.319012f,  TYPE_SMALLBLUE });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 22.669130f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 22.852560f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 23.052589f,  TYPE_SMALLBLUE });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 23.402725f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 23.572281f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 23.739019f,  TYPE_SMALLBLUE });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 23.905725f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 24.289145f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 24.455843f,  TYPE_SMALLBLUE });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 24.805895f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 25.155939f,  TYPE_SMALLBLUE });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 25.522697f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 25.706057f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 25.889427f,  TYPE_SMALLBLUE });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 26.222826f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 26.389509f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 26.572868f,  TYPE_SMALLBLUE });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 26.739563f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 27.106285f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 27.272978f,  TYPE_SMALLBLUE });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 27.606379f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 27.956423f,  TYPE_SMALLBLUE });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 28.339821f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 28.506536f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 28.673370f,  TYPE_SMALLBLUE });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 29.024029f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 29.207430f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 29.374128f,  TYPE_SMALLBLUE });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 29.557899f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 29.924648f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 30.091721f,  TYPE_SMALLBLUE });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 30.441784f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 30.625153f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 30.808563f,  TYPE_SMALLBLUE });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 30.975283f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 31.325785f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 31.509159f,  TYPE_SMALLBLUE });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 31.826700f,  TYPE_BIGRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 32.193996f,  TYPE_BIGRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 32.545094f,  TYPE_BIGRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 33.246216f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 33.613136f,  TYPE_SMALLBLUE });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 33.797626f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 34.181145f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 34.331421f,  TYPE_SMALLBLUE });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 34.531559f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 34.699627f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 35.050575f,  TYPE_SMALLBLUE });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 35.233940f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 35.400616f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 35.734295f,  TYPE_SMALLBLUE });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 36.101658f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 36.451736f,  TYPE_SMALLBLUE });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 36.636375f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 37.020710f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 37.187611f,  TYPE_SMALLBLUE });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 37.354721f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 37.865089f,  TYPE_SMALLBLUE });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 38.048450f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 38.248486f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 38.598541f,  TYPE_SMALLBLUE });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 38.965267f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 39.332031f,  TYPE_SMALLBLUE });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 39.498711f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 39.832096f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 39.999470f,  TYPE_SMALLBLUE });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 40.366180f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 40.532871f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 40.716278f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 40.900120f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 41.217342f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 41.384041f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 41.700939f,  TYPE_SMALLBLUE });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 41.884308f,  TYPE_SMALLBLUE });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 42.084389f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 42.251083f,  TYPE_SMALLBLUE });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 42.601318f,  TYPE_SMALLBLUE });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 42.801338f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 42.968040f,  TYPE_SMALLBLUE });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 43.168503f,  TYPE_BIGRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 43.518593f,  TYPE_BIGRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 44.219276f,  TYPE_BIGRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 43.885815f,  TYPE_SMALLBLUE });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 44.602764f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 44.769455f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 44.969475f,  TYPE_SMALLBLUE });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 45.136475f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 45.336578f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 45.520149f,  TYPE_SMALLBLUE });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 45.904251f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 46.104362f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 45.704174f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 46.555569f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 46.739002f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 46.922440f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 47.272572f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 47.439762f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 47.623535f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 47.807915f,  TYPE_SMALLBLUE });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 47.958603f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 48.275719f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 48.459190f,  TYPE_SMALLBLUE });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 48.625885f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 48.826317f,  TYPE_BIGRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 49.177181f,  TYPE_BIGRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 49.527229f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 49.694546f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 49.928768f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 50.279026f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 50.445721f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 50.629555f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 50.963852f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 51.114292f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 51.298206f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 51.649021f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 53.001610f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 53.185017f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 53.368877f,  TYPE_SMALLBLUE });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 53.702728f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 53.879414f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 54.096115f,  TYPE_SMALLBLUE });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 54.462841f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 54.629581f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 54.812939f,  TYPE_SMALLBLUE });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 54.979630f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 55.162987f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 55.513042f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 55.863163f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 56.046532f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 56.246559f,  TYPE_SMALLBLUE });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 56.596668f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 56.763351f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 56.963539f,  TYPE_SMALLBLUE });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 57.280254f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 57.463627f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 57.630325f,  TYPE_SMALLBLUE });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 57.797020f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 57.997044f,  TYPE_BIGRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 58.347095f,  TYPE_BIGRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 58.713909f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 58.880604f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 59.047310f,  TYPE_SMALLBLUE });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 59.397465f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 59.564240f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 59.748013f,  TYPE_SMALLBLUE });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 60.098587f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 60.265255f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 60.432228f,  TYPE_SMALLBLUE });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 60.615814f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 60.799263f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 61.132904f,  TYPE_SMALLBLUE });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 61.533764f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 61.884964f,  TYPE_SMALLBLUE });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 62.235252f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 62.552181f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 62.737148f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 62.920902f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 63.104908f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 63.288486f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 63.605446f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 63.788815f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 63.972267f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 64.139000f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 64.339790f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 64.673531f,  TYPE_BIGRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 65.007347f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 65.207481f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 65.407860f,  TYPE_BIGRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 65.776382f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 65.943100f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 66.093437f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 66.276947f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 66.443703f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 66.777863f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 67.128334f,  TYPE_BIGRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 67.479836f,  TYPE_SMALLBLUE });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 67.880280f,  TYPE_SMALLBLUE });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 68.230972f,  TYPE_SMALLBLUE });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 68.581192f,  TYPE_BIGRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 68.965096f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 69.299232f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 69.632820f,  TYPE_BIGRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 70.032082f,  TYPE_SMALLBLUE });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 70.348846f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 70.698921f,  TYPE_SMALLBLUE });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 70.865623f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 71.048996f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 71.399178f,  TYPE_SMALLBLUE });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 71.749718f,  TYPE_SMALLBLUE });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 72.083221f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 72.433289f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 72.816727f,  TYPE_SMALLBLUE });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 73.150131f,  TYPE_SMALLBLUE });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 73.316826f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 73.500198f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 73.666901f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 73.883614f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 74.217010f,  TYPE_BIGRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 74.583755f,  TYPE_BIGRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 75.117325f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 75.517403f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 75.851334f,  TYPE_SMALLBLUE });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 76.051376f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 76.218079f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 76.551529f,  TYPE_SMALLRED });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 76.751572f,  TYPE_SMALLBLUE });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 76.918304f,  TYPE_SMALLBLUE });
	m_vecNoteTimings.push_back(NOTETIMINGDESC{ 77.401810f,  TYPE_BIGRED });


#pragma endregion NoteVector
}

void CScene_MiniGame_JJB::ProduceTimingNote()
{
	if (m_AccumulateTime > m_vecNoteTimings[m_iVecCount].fNoteTiming - 1.25f)
	{
		switch (m_vecNoteTimings[m_iVecCount].eType)
		{
		case Client::CScene_MiniGame_JJB::TYPE_SMALLRED:
			Pop_NoteSmallRedMonsterLsit();
			break;
		case Client::CScene_MiniGame_JJB::TYPE_BIGRED:
			Pop_NoteBigRedMonsterLsit();
			break;
		case Client::CScene_MiniGame_JJB::TYPE_SMALLBLUE:
			Pop_NoteSmallBlueMonsterLsit();
			break;
		case Client::CScene_MiniGame_JJB::TYPE_BIGBLUE:
			Pop_NoteBigBlueMonsterLsit();
			break;
		}

		++m_iVecCount;
	}
}

//void CScene_MiniGame_JJB::KeyClick()
//{
//   if (g_pGameInstance->Get_DIKeyState(DIK_F)&DIS_Down)
//   {
//      if (m_pGoodCollider->Get_GoodColliderSmallRedCrashing())
//      {
//         m_pGoodCollider->Get_GoodCollisionMonster()->UseOFF();
//         Set_ComboPlus();
//      }
//   }
//
//   if (g_pGameInstance->Get_DIKeyState(DIK_J)&DIS_Down)
//   {
//      if (m_pGoodCollider->Get_GoodColliderSmallRedCrashing())
//      {
//         m_pGoodCollider->Get_GoodCollisionMonster()->UseOFF();
//         Set_ComboPlus();
//      }
//   }
//
//   if (g_pGameInstance->Get_DIKeyState(DIK_D)&DIS_Down)
//   {
//      if (m_pGoodCollider->Get_GoodColliderSmallBlueCrashing())
//      {
//         m_pGoodCollider->Get_GoodCollisionMonster()->UseOFF();
//         Set_ComboPlus();
//      }
//   }
//
//   if (g_pGameInstance->Get_DIKeyState(DIK_K)&DIS_Down)
//   {
//      if (m_pGoodCollider->Get_GoodColliderSmallBlueCrashing())
//      {
//         m_pGoodCollider->Get_GoodCollisionMonster()->UseOFF();
//         Set_ComboPlus();
//      }
//   }
//
//   if (g_pGameInstance->Get_DIKeyState(DIK_SPACE)&DIS_Down)
//   {
//      if (m_pGoodCollider->Get_GoodColliderBigBlueCrashing() || m_pGoodCollider->Get_GoodColliderBigRedCrashing())
//      {
//         m_pGoodCollider->Get_GoodCollisionMonster()->UseOFF();
//         Set_ComboPlus();
//      }
//   }
//}

void CScene_MiniGame_JJB::ShowComboUI()
{
	m_pTaikoUI->Set_IsShowComboUI();
}

void CScene_MiniGame_JJB::ShowGoodEffectUI()
{
	m_pTaikoUI->Set_IsShowGoodEffectUI();
}

HRESULT CScene_MiniGame_JJB::Ready_Light()
{
	const LIGHTDESC* pLightDesc = g_pGameInstance->Get_LightDesc(tagLightDesc::TYPE_DIRECTIONAL, 0);

	if (pLightDesc == nullptr)
	{

		LIGHTDESC LightDesc;

		LightDesc.eLightType = tagLightDesc::TYPE_DIRECTIONAL;
		LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
		LightDesc.vAmbient = _float4(1.0f);
		LightDesc.vSpecular = _float4(1);
		LightDesc.vVector = DefalutSunPosition;

		g_pGameInstance->Add_Light(LightDesc);
	}
	else
	{
		g_pGameInstance->Relocate_LightDesc(tagLightDesc::TYPE_DIRECTIONAL, 0, DefalutSunPosition.XMVector());

	}

	return S_OK;
}

HRESULT CScene_MiniGame_JJB::Ready_Layer_MainCamera(const _tchar * pLayerTag)
{
	CCamera::CAMERADESC CameraDesc;
	CameraDesc.vWorldRotAxis = _float3(0, 0, 0);
	CameraDesc.vEye = _float3(0, 0.f, -5.f);
	CameraDesc.vAt = _float3(0, 0.f, 0.1f);
	CameraDesc.vAxisY = _float3(0, 1, 0);

	CameraDesc.fFovy = XMConvertToRadians(60.f);
	CameraDesc.fAspect = _float(g_iWinCX) / g_iWinCY;
	CameraDesc.fNear = 0.2f;
	CameraDesc.fFar = CAMERAFAR;

	CameraDesc.iWinCX = g_iWinCX;
	CameraDesc.iWinCY = g_iWinCY;

	CameraDesc.TransformDesc.fMovePerSec = 5.f;
	CameraDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(60.0f);
	CameraDesc.TransformDesc.fScalingPerSec = 1.f;

	m_pMainCam = (CCamera_Main*)(g_pGameInstance->Get_GameObject_By_LayerIndex(SCENE_STATIC, TAG_LAY(Layer_Camera_Main)));

	if (m_pMainCam == nullptr)
	{
		FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STATIC, pLayerTag, TAG_OP(Prototype_Camera_Main), &CameraDesc));

		m_pMainCam = (CCamera_Main*)(g_pGameInstance->Get_GameObject_By_LayerIndex(SCENE_STATIC, TAG_LAY(Layer_Camera_Main)));

		NULL_CHECK_RETURN(m_pMainCam, E_FAIL);
	}
	else
	{

		m_pMainCam->Set_NowSceneNum(SCENE_MINIGAME_JJB);
		m_pMainCam->Set_CameraDesc(CameraDesc);

		m_pMainCam->Get_Camera_Transform()->Set_MatrixState(CTransform::STATE_POS, _float3(0, 0.f, -5.f));
		m_pMainCam->Get_Camera_Transform()->LookAt(_float3(0, 0.f, 0.1f).XMVector());
	}

	m_pMainCam->Set_TargetArmLength(15.f);
	m_pMainCam->Set_MaxTargetArmLength(15.f);
	m_pMainCam->Set_MinTargetArmLength(15.f);
	m_pMainCam->Set_ProjectMatrix(true);

	return S_OK;
}

HRESULT CScene_MiniGame_JJB::Ready_Layer_Objects(const _tchar * pLayerTag)
{
	return S_OK;
}

HRESULT CScene_MiniGame_JJB::Ready_Layer_Player(const _tchar * pLayerTag)
{
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_JJB, pLayerTag, TAG_OP(Prototype_Object_MiniGame_JJB_Player), &_float3(0, 0, 5.f)));

	m_pMainCam->Set_CameraMode(CAM_MODE_FIX);
	m_pMainCam->Lock_CamLook(true, XMVectorSet(0.f, 0.f, 1.f, 0.f));
	return S_OK;
}

HRESULT CScene_MiniGame_JJB::Ready_Layer_Monster(const _tchar * pLayerTag)
{
	CTaiko_Monster::NOTEDESC NoteMonsterDesc;

	for (_int i = 0; i < 30; ++i)
	{
		NoteMonsterDesc.NoteType = CTaiko_Monster::NOTE_SMALL;
		NoteMonsterDesc.NotePosType = CTaiko_Monster::NOTEPOS_IN;
		NoteMonsterDesc.vNotePosition = _float3(0.f, 0.5f, -1.f);
		NoteMonsterDesc.vScaled = _float3(2.0f);
		FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_JJB, pLayerTag, TAG_OP(Prototype_Object_PM_Monster), &NoteMonsterDesc));

		CTaiko_Monster* Monster = (CTaiko_Monster*)g_pGameInstance->Get_GameObject_By_LayerLastIndex(SCENEID::SCENE_MINIGAME_JJB, pLayerTag);
		m_NoteSmallRedMonsterList.push_back(Monster);
	}

	for (_int i = 0; i < 10; ++i)
	{
		NoteMonsterDesc.NoteType = CTaiko_Monster::NOTE_BIG;
		NoteMonsterDesc.NotePosType = CTaiko_Monster::NOTEPOS_IN;
		NoteMonsterDesc.vNotePosition = _float3(0.f, 0.5f, -1.f);
		NoteMonsterDesc.vScaled = _float3(3.f);
		FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_JJB, pLayerTag, TAG_OP(Prototype_Object_PM_Monster), &NoteMonsterDesc));

		CTaiko_Monster* Monster = (CTaiko_Monster*)g_pGameInstance->Get_GameObject_By_LayerLastIndex(SCENEID::SCENE_MINIGAME_JJB, pLayerTag);
		m_NoteBigRedMonsterList.push_back(Monster);
	}

	for (_int i = 0; i < 20; ++i)
	{
		NoteMonsterDesc.NoteType = CTaiko_Monster::NOTE_SMALL;
		NoteMonsterDesc.NotePosType = CTaiko_Monster::NOTEPOS_OUT;
		NoteMonsterDesc.vNotePosition = _float3(0.f, 0.5f, -1.f);
		NoteMonsterDesc.vScaled = _float3(2.f);
		FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_JJB, pLayerTag, TAG_OP(Prototype_Object_PM_Monster), &NoteMonsterDesc));

		CTaiko_Monster* Monster = (CTaiko_Monster*)g_pGameInstance->Get_GameObject_By_LayerLastIndex(SCENEID::SCENE_MINIGAME_JJB, pLayerTag);
		m_NoteSmallBlueMonsterList.push_back(Monster);
	}

	for (_int i = 0; i < 10; ++i)
	{
		NoteMonsterDesc.NoteType = CTaiko_Monster::NOTE_BIG;
		NoteMonsterDesc.NotePosType = CTaiko_Monster::NOTEPOS_OUT;
		NoteMonsterDesc.vNotePosition = _float3(0.f, 0.5f, -1.f);
		NoteMonsterDesc.vScaled = _float3(3.f);
		FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_JJB, pLayerTag, TAG_OP(Prototype_Object_PM_Monster), &NoteMonsterDesc));

		CTaiko_Monster* Monster = (CTaiko_Monster*)g_pGameInstance->Get_GameObject_By_LayerLastIndex(SCENEID::SCENE_MINIGAME_JJB, pLayerTag);
		m_NoteBigBlueMonsterList.push_back(Monster);
	}

	/*NoteMonsterDesc.NoteType = CTaiko_Monster::NOTE_SMALL;
	NoteMonsterDesc.vNotePosition = _float3(0.f, 0.5f, -1.f);
	NoteMonsterDesc.vScaled = _float3(1.2f);
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_JJB, pLayerTag, TAG_OP(Prototype_Object_PM_Monster), &NoteMonsterDesc));
	*/
	return S_OK;
}

HRESULT CScene_MiniGame_JJB::Ready_Layer_GoalCollider(const _tchar * pLayerTag)
{
	CGaolCollider::tagGoalColliderDesc ColliderDesc;

	ColliderDesc.ColliderType = CGaolCollider::COLLIDER_GOOD;
	ColliderDesc.vNotePosition = _float3(-13.2f, 0.5f, -1.f);
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_JJB, pLayerTag, TAG_OP(Prototype_Object_MiniGame_GoalCollider), &ColliderDesc));

	m_pGoodCollider = (CGaolCollider*)g_pGameInstance->Get_GameObject_By_LayerLastIndex(SCENEID::SCENE_MINIGAME_JJB, pLayerTag);

	ColliderDesc.ColliderType = CGaolCollider::COLLIDER_DIE;
	ColliderDesc.vNotePosition = _float3(-19.2f, 0.5f, -1.f);
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_JJB, pLayerTag, TAG_OP(Prototype_Object_MiniGame_GoalCollider), &ColliderDesc));

	return S_OK;

}

HRESULT CScene_MiniGame_JJB::Ready_Layer_UI(const _tchar * pLayerTag)
{
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_MINIGAME_JJB, pLayerTag, TAG_OP(Prototype_Object_TaikoUI)));

	m_pTaikoUI = (CTaikoUI*)g_pGameInstance->Get_GameObject_By_LayerLastIndex(SCENEID::SCENE_MINIGAME_JJB, pLayerTag);

	return S_OK;
}

HRESULT CScene_MiniGame_JJB::Ready_PostPorcessing()
{
#ifndef _DEBUG

	LIGHTDESC* pLightDesc = g_pGameInstance->Get_LightDesc(tagLightDesc::TYPE_DIRECTIONAL, 0);
	m_pUtilMgr->Get_Renderer()->Set_SunAtPoint(_float3(128.f, -64.f, 256.f));
	pLightDesc->vDiffuse = _float4(0.78125f, 0.78125f, 1.f, 1.f);
	pLightDesc->vAmbient = _float4(0.6640625f, 0.65625f, 1.f, 1.f);
	pLightDesc->vSpecular = _float4(0.234375f, 0.234375f, 0.234375f, 1.f);

	CRenderer* pRenderer = m_pUtilMgr->Get_Renderer();


	for (_uint i = 0; i < POSTPROCESSING_END; i++)
		pRenderer->OnOff_PostPorcessing_byParameter(POSTPROCESSINGID(i), false);


	pRenderer->OnOff_PostPorcessing_byParameter(POSTPROCESSING_SHADOW, false);
	pRenderer->Set_ShadowIntensive(0.3f);

	pRenderer->OnOff_PostPorcessing_byParameter(POSTPROCESSING_BLOOM, false);
	pRenderer->Set_BloomOverLuminceValue(1.0f);
	pRenderer->Set_BloomBrightnessMul(1.5f);

	//pRenderer->OnOff_PostPorcessing_byParameter(POSTPROCESSING_DOF, true);
	//pRenderer->Set_DofLength(30.f);
	//pRenderer->Set_DofBlurIntensive(1.f);

	pRenderer->OnOff_PostPorcessing_byParameter(POSTPROCESSING_DDFOG, false);
	pRenderer->Set_FogStartDist(5.f);
	pRenderer->Set_FogGlobalDensity(0.1f);
	pRenderer->Set_FogHeightFalloff(0.1f);

	//POSTPROCESSING_GODRAY
	//POSTPROCESSING_LENSEFLARE
	//POSTPROCESSING_CAMMOTIONBLUR
#else
#ifdef DEBUGONSHADERSETTING

	LIGHTDESC* pLightDesc = g_pGameInstance->Get_LightDesc(tagLightDesc::TYPE_DIRECTIONAL, 0);
	pLightDesc->vVector = _float4(0, 64.f, -64.f, 1);
	m_pUtilMgr->Get_Renderer()->Set_SunAtPoint(_float3(0, -64.f, 64.f));
	pLightDesc->vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	pLightDesc->vAmbient = _float4(1.f, 1.f, 1.f, 1.f);
	pLightDesc->vSpecular = _float4(0.234375f, 0.234375f, 0.234375f, 1.f);

	CRenderer* pRenderer = m_pUtilMgr->Get_Renderer();


	for (_uint i = 0; i < POSTPROCESSING_END; i++)
		pRenderer->OnOff_PostPorcessing_byParameter(POSTPROCESSINGID(i), false);


	//pRenderer->OnOff_PostPorcessing_byParameter(POSTPROCESSING_SHADOW, true);
	//pRenderer->Set_ShadowIntensive(0.3f);

	//pRenderer->OnOff_PostPorcessing_byParameter(POSTPROCESSING_BLOOM, true);
	//pRenderer->Set_BloomOverLuminceValue(1.0f);
	//pRenderer->Set_BloomBrightnessMul(1.5f);

	////pRenderer->OnOff_PostPorcessing_byParameter(POSTPROCESSING_DOF, true);
	////pRenderer->Set_DofLength(30.f);
	////pRenderer->Set_DofBlurIntensive(1.f);

	//pRenderer->OnOff_PostPorcessing_byParameter(POSTPROCESSING_DDFOG, true);
	//pRenderer->Set_FogColor(_float3{ 0.234375f });
	//pRenderer->Set_FogHighlightColor(_float3{ 1.f });
	//pRenderer->Set_FogStartDist(5.f);
	//pRenderer->Set_FogGlobalDensity(0.1f);
	//pRenderer->Set_FogHeightFalloff(0.1f);
#endif

#endif // !_DEBUGee

	return S_OK;
}

CScene_MiniGame_JJB * CScene_MiniGame_JJB::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CScene_MiniGame_JJB* pScene_MinigameTaiko = NEW CScene_MiniGame_JJB(pDevice, pDeviceContext);

	if (FAILED(pScene_MinigameTaiko->Initialize()))
	{
		Safe_Release(pScene_MinigameTaiko);
		MSGBOX("Failed to Creating CScene_MiniGame_JJB");
		return nullptr;
	}

	return pScene_MinigameTaiko;
}

void CScene_MiniGame_JJB::Free()
{
	__super::Free();
}