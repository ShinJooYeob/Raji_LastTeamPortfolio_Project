#include "..\Public\SceneMgr.h"
#include "Scene.h"
#include "GameInstance.h"



IMPLEMENT_SINGLETON(CSceneMgr)

CSceneMgr::CSceneMgr()
	:m_pScene(nullptr), m_iCurrentSceneIdx(0)
{
}

HRESULT CSceneMgr::Scene_Chage(CScene * _pScene , _int iNextSceneIndex)
{
	NULL_CHECK_BREAK(_pScene);

	if (m_iCurrentSceneIdx && FAILED(GetSingle(CGameInstance)->Clear_Scene_Resource(m_iCurrentSceneIdx)))
	{
		__debugbreak();
		return E_FAIL;
	}
	


	Safe_Release(m_pScene);
	m_pScene = _pScene;
	m_iCurrentSceneIdx = iNextSceneIndex;
	m_iTargetSceneIdx = iNextSceneIndex;

	return S_OK;
}

_int CSceneMgr::Update(_double fDeltaTime)
{
	NULL_CHECK_BREAK(m_pScene);


	if (m_pScene->Get_bIsNeedToSceneChange())
		return m_pScene->Change_to_NextScene();

	return m_pScene->Update(fDeltaTime);
}

_int CSceneMgr::LateUpdate(_double fDeltaTime)
{
	NULL_CHECK_BREAK(m_pScene);


	if (m_pScene->Get_bIsNeedToSceneChange())
		return m_pScene->Change_to_NextScene();

	return m_pScene->LateUpdate(fDeltaTime);
	
}

_int CSceneMgr::Render()
{
	NULL_CHECK_BREAK(m_pScene);

	return m_pScene->Render();
}

_int CSceneMgr::LateRender()
{
	NULL_CHECK_BREAK(m_pScene);

	return m_pScene->LateRender();
}

CScene * CSceneMgr::Get_NowScene()
{
	NULL_CHECK_BREAK(m_pScene);
	return m_pScene;
}

HRESULT CSceneMgr::Set_SceneChanging_to_Scene(_uint _INextScene)
{
	NULL_CHECK_BREAK(m_pScene);

	return m_pScene->Set_SceneChanging(_INextScene);
}


void CSceneMgr::Free()
{
	Safe_Release(m_pScene);
}
