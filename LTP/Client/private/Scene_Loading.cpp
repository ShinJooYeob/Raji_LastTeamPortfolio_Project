#include "stdafx.h"
#include "..\Public\Scene_Loading.h"

#include "Loader.h"
#include "Scene_Loby.h"
#include "Scene_StageSelect.h"
#include "Scene_Stage1.h"
#include "Scene_Stage2.h"
#include "Scene_Stage3.h"
#include "Scene_Stage4.h"
#include "Scene_Stage5.h"
#include "Scene_Stage6.h"
#include "Scene_Stage7.h"
#include "Scene_Boss.h"
#include "Scene_Laboratory_Jino.h"
#include "Scene_Minigame1.h"
#include "Scene_Minigame_PM.h"
#include "Scene_MiniGame_Jino.h"
#include "Scene_MiniGame_DonkeyKong.h"
#include "Scene_MiniGame_JJB.h"
//#include "Scene_Ending.h"

#include "MiniGameBuilding.h"

#ifdef _DEBUG
 #include "Scene_Edit.h"
#endif // USE_IMGUI



_uint CScene_Loading::m_iLoadingKinds = LOADINGKINDS_NORMAL;

CScene_Loading::CScene_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CScene(pDevice, pDeviceContext)
{
}

HRESULT CScene_Loading::Initialize(SCENEID eSceneID)
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	m_eOldSceneIndex =(SCENEID)g_pGameInstance->Get_NowSceneNum();

	_bool bOldIsMiniGame = (m_eOldSceneIndex < SCENE_LABORATORY_JINO || m_eOldSceneIndex >= SCENE_EDIT) ? false : true;
	_bool bNextIsMiniGame = (eSceneID < SCENE_LABORATORY_JINO || eSceneID >= SCENE_EDIT) ? false : true;

	FAILED_CHECK(GetSingle(CUtilityMgr)->Get_Renderer()->Clear_RenderGroup_forSceneChaging());


	//LOADINGKINDS_NORMAL, LOADINGKINDS_NORMAL_TO_MINI, LOADINGKINDS_MINI_TO_NORMAL, LOADINGKINDS_END

	m_iLoadingKinds = (bOldIsMiniGame) ? ((bNextIsMiniGame)? LOADINGKINDS_NORMAL : LOADINGKINDS_MINI_TO_NORMAL) 
		: ((bNextIsMiniGame) ? LOADINGKINDS_NORMAL_TO_MINI : LOADINGKINDS_NORMAL);

	switch (m_iLoadingKinds)
	{
	case Client::CScene_Loading::LOADINGKINDS_NORMAL:
	{
		FAILED_CHECK(GetSingle(CUtilityMgr)->Copy_LastDeferredToToonShadingTexture(_float(0)));
	}
		break;
	case Client::CScene_Loading::LOADINGKINDS_NORMAL_TO_MINI:
	{

		GetSingle(CUtilityMgr)->Get_Renderer()->Copy_CompletelyLastDeferredTexture();
		switch (eSceneID)
		{
		case SCENE_LABORATORY_JINO:
			m_iMiniGameIndex = CMiniGameBuilding::MINIGAME_FALLOFF;
			break;
		case SCENE_MINIGAME1:
			m_iMiniGameIndex = CMiniGameBuilding::MINIGAME_VAMPIRESURVIAL;
			break;
		case SCENE_MINIGAME_PM:
			m_iMiniGameIndex = CMiniGameBuilding::MINIGAME_PACKMAN;
			break;
		case SCENE_MINIGAME_Jino:
			m_iMiniGameIndex = CMiniGameBuilding::MINIGAME_CIRCUS;
			break;
		default:
			break;
		}
	}
	case Client::CScene_Loading::LOADINGKINDS_MINI_TO_NORMAL:		
	{

		GetSingle(CUtilityMgr)->Get_Renderer()->Copy_CompletelyLastDeferredTexture();
		switch (m_eOldSceneIndex)
		{
		case SCENE_LABORATORY_JINO:
			m_iMiniGameIndex = CMiniGameBuilding::MINIGAME_FALLOFF;
			break;
		case SCENE_MINIGAME1:
			m_iMiniGameIndex = CMiniGameBuilding::MINIGAME_VAMPIRESURVIAL;
			break;
		case SCENE_MINIGAME_PM:
			m_iMiniGameIndex = CMiniGameBuilding::MINIGAME_PACKMAN;
			break;
		case SCENE_MINIGAME_Jino:
			m_iMiniGameIndex = CMiniGameBuilding::MINIGAME_CIRCUS;
			break;
		default:
			break;
		}
	}
		break;
	default:
		__debugbreak();
		MSGBOX("Worng LoadingKinds");
		break;
	}





	m_eNextSceneIndex = eSceneID;
	m_pLoader = CLoader::Create(m_pDevice,m_pDeviceContext,eSceneID);

	UIDESC tUIDesc;
	tUIDesc.fCX = g_iWinCX;
	tUIDesc.fCY = (g_iWinCY);
	tUIDesc.fX = g_iWinCX * 0.5f;
	tUIDesc.fY = g_iWinCY * 0.5f;


	if (FAILED(Ready_Layer_LoadingUI(TEXT("Layer_Loading"))))
		return E_FAIL;


	g_pGameInstance->Stop_AllChannel();


	return S_OK;
}

_int CScene_Loading::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)
		return -1;


	switch (m_iLoadingKinds)
	{
	case Client::CScene_Loading::LOADINGKINDS_NORMAL:
		if (m_fSceneStartTimer < 2.f)
		{
			GetSingle(CUtilityMgr)->Copy_LastDeferredToToonShadingTexture(_float(m_fSceneStartTimer / 2.f));
		}
		break;
	case Client::CScene_Loading::LOADINGKINDS_NORMAL_TO_MINI:
		break;
	case Client::CScene_Loading::LOADINGKINDS_MINI_TO_NORMAL:
		break;
	default:
		break;
	}

	return 0;
}

_int CScene_Loading::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)
		return -1;


	//로딩이 끝낫을 경우
	if (m_fSceneStartTimer > 2.5f && m_pLoader->IsLoadFinished())
	{

		g_pGameInstance->Set_TargetSceneNum(m_eNextSceneIndex);
		FAILED_CHECK(GetSingle(CUtilityMgr)->Clear_RenderGroup_forSceneChange());

		switch (m_eNextSceneIndex)
		{
		case SCENEID::SCENE_LOBY:
			FAILED_CHECK(g_pGameInstance->Scene_Change(CScene_Loby::Create(m_pDevice, m_pDeviceContext), m_eNextSceneIndex));
			break;

		case SCENEID::SCENE_STAGESELECT:
			FAILED_CHECK(g_pGameInstance->Scene_Change(CScene_StageSelect::Create(m_pDevice, m_pDeviceContext), m_eNextSceneIndex));
			break;

		case SCENEID::SCENE_STAGE1:
			FAILED_CHECK(g_pGameInstance->Scene_Change(CScene_Stage1::Create(m_pDevice, m_pDeviceContext), m_eNextSceneIndex));
			break;

		case SCENEID::SCENE_STAGE2:
			FAILED_CHECK(g_pGameInstance->Scene_Change(CScene_Stage2::Create(m_pDevice, m_pDeviceContext), m_eNextSceneIndex));
			break;
		case SCENEID::SCENE_STAGE3:
			FAILED_CHECK(g_pGameInstance->Scene_Change(CScene_Stage3::Create(m_pDevice, m_pDeviceContext), m_eNextSceneIndex));
			break;
		case SCENEID::SCENE_STAGE4:
			FAILED_CHECK(g_pGameInstance->Scene_Change(CScene_Stage4::Create(m_pDevice, m_pDeviceContext), m_eNextSceneIndex));
			break;
		case SCENEID::SCENE_STAGE5:
			FAILED_CHECK(g_pGameInstance->Scene_Change(CScene_Stage5::Create(m_pDevice, m_pDeviceContext), m_eNextSceneIndex));
			break;
		case SCENEID::SCENE_STAGE6:
			FAILED_CHECK(g_pGameInstance->Scene_Change(CScene_Stage6::Create(m_pDevice, m_pDeviceContext), m_eNextSceneIndex));
			break;
		case SCENEID::SCENE_STAGE7:
			FAILED_CHECK(g_pGameInstance->Scene_Change(CScene_Stage7::Create(m_pDevice, m_pDeviceContext), m_eNextSceneIndex));
			break;
		case SCENEID::SCENE_BOSS:
			FAILED_CHECK(g_pGameInstance->Scene_Change(CScene_Boss::Create(m_pDevice, m_pDeviceContext), m_eNextSceneIndex));
			break;

		case SCENEID::SCENE_LABORATORY_JINO:
			FAILED_CHECK(g_pGameInstance->Scene_Change(CScene_Laboratory_Jino::Create(m_pDevice, m_pDeviceContext), m_eNextSceneIndex));
			break;

		case SCENEID::SCENE_MINIGAME1:
			FAILED_CHECK(g_pGameInstance->Scene_Change(CScene_Minigame1::Create(m_pDevice, m_pDeviceContext), m_eNextSceneIndex));
			break;

		case SCENEID::SCENE_MINIGAME_PM:
			FAILED_CHECK(g_pGameInstance->Scene_Change(CScene_Minigame_PM::Create(m_pDevice, m_pDeviceContext), m_eNextSceneIndex));
			break;
			
		case SCENEID::SCENE_MINIGAME_Jino:
			FAILED_CHECK(g_pGameInstance->Scene_Change(CScene_MiniGame_Jino::Create(m_pDevice, m_pDeviceContext), m_eNextSceneIndex));
			break;

		case SCENEID::SCENE_MINIGAME_DONKEYKONG:
			FAILED_CHECK(g_pGameInstance->Scene_Change(Scene_MiniGame_DonkeyKong::Create(m_pDevice, m_pDeviceContext), m_eNextSceneIndex));
			break;
		case SCENEID::SCENE_MINIGAME_JJB:
			FAILED_CHECK(g_pGameInstance->Scene_Change(CScene_MiniGame_JJB::Create(m_pDevice, m_pDeviceContext), m_eNextSceneIndex));
			break;
//		case SCENEID::SCENE_ENDING:
//			FAILED_CHECK(g_pGameInstance->Scene_Change(CScene_Ending::Create(m_pDevice, m_pDeviceContext), m_eNextSceneIndex));
//			break;
//
//
#ifdef _DEBUG
		case SCENEID::SCENE_EDIT:
			FAILED_CHECK(g_pGameInstance->Scene_Change(CScene_Edit::Create(m_pDevice, m_pDeviceContext), m_eNextSceneIndex));
			break;
#endif // USE_IMGUI

		default:
			MSGBOX("Failed to SceneChange");
			break;


		}

	}

	return 0;
}

_int CScene_Loading::Render()
{
	if (__super::Render() < 0)
		return -1;


	switch (m_iLoadingKinds)
	{
	case Client::CScene_Loading::LOADINGKINDS_NORMAL:
		GetSingle(CUtilityMgr)->SCD_Rendering();
		break;
	case Client::CScene_Loading::LOADINGKINDS_NORMAL_TO_MINI:
	{
		_float fValue = min(m_fSceneStartTimer / 2.f, 1.f);
		GetSingle(CUtilityMgr)->Get_Renderer()->Copy_CompletelyLastDeferredTexToNowTex(fValue, CMiniGameBuilding::m_vecMGBTargets[m_iMiniGameIndex].pSRV);
	}
		break;
	case Client::CScene_Loading::LOADINGKINDS_MINI_TO_NORMAL:
	{
		_float fValue = min(m_fSceneStartTimer / 2.f, 1.f);
		GetSingle(CUtilityMgr)->Get_Renderer()->Copy_MiniGameDeferredTexToNowTex(fValue, CMiniGameBuilding::m_vecMGBTargets[m_iMiniGameIndex].pSRV,
			GetSingle(CUtilityMgr)->Get_UtilTex_SRV(CUtilityMgr::UTILTEX_MINIGAMETEX, 1));
	}
		break;
	default:
		break;
	}




	ID3D11ShaderResourceView* pSRV[8] = { nullptr };
	m_pDeviceContext->PSSetShaderResources(0, 8, pSRV);
	return 0;
}

_int CScene_Loading::LateRender()
{
	if (__super::LateRender() < 0)
		return -1;

	return 0;
}

HRESULT CScene_Loading::Ready_Layer_LoadingUI(const _tchar * pLayerTag)
{

	//FAILED_CHECK(GetSingle(CGameInstance)->Add_GameObject_To_Layer(SCENEID::SCENE_LOADING, pLayerTag, TAG_OP(Prototype_UILoading)));
	
	return S_OK;
}

CScene_Loading * CScene_Loading::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, SCENEID eSceneID)
{
	CScene_Loading* pTempLoadingScene = NEW CScene_Loading(pDevice, pDeviceContext);

	if (FAILED(pTempLoadingScene->Initialize((eSceneID))))
	{
		Safe_Release(pTempLoadingScene);
		MSGBOX("Failed to Creating CScene_Loading");
		return nullptr;
	}


	return pTempLoadingScene;
}

void CScene_Loading::Free()
{
	__super::Free();
	//m_pLoadingSCD.Free();
	
	Safe_Release(m_pLoader);
}
