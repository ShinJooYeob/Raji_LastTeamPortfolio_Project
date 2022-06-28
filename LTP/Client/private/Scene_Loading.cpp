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
//#include "Scene_Ending.h"


#ifdef USE_IMGUI
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


	m_eNextSceneIndex = eSceneID;
	m_pLoader = CLoader::Create(m_pDevice,m_pDeviceContext,eSceneID);

	if (FAILED(Ready_Layer_LoadingUI(TEXT("Layer_Loading"))))
		return E_FAIL;

	m_FadePassedTime = 0;

	g_pGameInstance->Stop_AllChannel();

	return S_OK;
}

_int CScene_Loading::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)
		return -1;


	return 0;
}

_int CScene_Loading::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)
		return -1;


	//로딩이 끝낫을 경우
	if (m_pLoader->IsLoadFinished())
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



//		case SCENEID::SCENE_ENDING:
//			FAILED_CHECK(g_pGameInstance->Scene_Change(CScene_Ending::Create(m_pDevice, m_pDeviceContext), m_eNextSceneIndex));
//			break;
//
//
#ifdef USE_IMGUI
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
	CScene_Loading* pTempLoadingScene = new CScene_Loading(pDevice, pDeviceContext);

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
	
	Safe_Release(m_pLoader);
}
