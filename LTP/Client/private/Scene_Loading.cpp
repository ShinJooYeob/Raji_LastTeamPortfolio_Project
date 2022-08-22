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
#include "Scene_MiniGame_JJB.h"
//#include "Scene_Ending.h"


#ifdef _DEBUG
 #include "Scene_Edit.h"
#endif // USE_IMGUI








CScene_Loading::CScene_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:CScene(pDevice, pDeviceContext)
{
}

HRESULT CScene_Loading::Initialize(SCENEID eSceneID)
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	FAILED_CHECK(GetSingle(CUtilityMgr)->Get_Renderer()->Clear_RenderGroup_forSceneChaging());

	FAILED_CHECK(GetSingle(CUtilityMgr)->Copy_LastDeferredToToonShadingTexture(_float(0)));

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

	if (m_fSceneStartTimer < 2.f)
	{
		GetSingle(CUtilityMgr)->Copy_LastDeferredToToonShadingTexture(_float(m_fSceneStartTimer / 2.f));

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



	GetSingle(CUtilityMgr)->SCD_Rendering();
	//FAILED_CHECK(m_pLoadingSCD.Render_SCD(1));



#ifdef _DEBUG
	if (!g_bIsShowFPS)

	if (m_pLoader != nullptr) 
	{
		_int iPercent = int((m_pLoader->Get_ProgressCount() / (float)m_pLoader->Get_MaxCount()) * 100);
		_tchar  szTempString[MAX_PATH];
		_itow_s(iPercent, szTempString, MAX_PATH , 10);
		lstrcat(szTempString,L"% 로딩 중 (Loading Scene)");

		SetWindowText(g_hWnd, szTempString);
	}
#endif // _DEBUG


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
