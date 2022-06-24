#include "..\Public\ObjectMgr.h"
#include "GameObject.h"
#include "ObjectLayer.h"



IMPLEMENT_SINGLETON(CObjectMgr)

CObjectMgr::CObjectMgr()
{
}

HRESULT CObjectMgr::Reserve_Container(_uint _eSceneNum)
{
	m_iMaxSceneNum = _eSceneNum;

	m_mapLayer = new LAYERS[m_iMaxSceneNum];

	return S_OK;
}

HRESULT CObjectMgr::Add_GameObject_Prototype(const _tchar * tagPrototype, CGameObject * pPrototype)
{
	NULL_CHECK_BREAK(pPrototype);


	if (Find_Prototype(tagPrototype) != nullptr)
	{
		OutputDebugString(TEXT("GameObject Prototypes already exist,So won't add more And did Release Parameter GameObject\n"));
		Safe_Release(pPrototype);
		return S_FALSE;
	}
	else {

		m_mapPrototypes.emplace(tagPrototype, pPrototype);
	}


	return S_OK;
}

HRESULT CObjectMgr::Add_GameObject_Out_of_Manager(CGameObject ** ppOutGameObj, _uint eSceneNum, const _tchar * tagPrototype, void * pArg)
{
	if (eSceneNum >= m_iMaxSceneNum)
	{
		__debugbreak();
		return E_FAIL;
	}
	CGameObject* pPrototype = Find_Prototype(tagPrototype);
	NULL_CHECK_BREAK(pPrototype);

	CGameObject* pInstance = pPrototype->Clone(pArg);
	NULL_CHECK_BREAK(pInstance);

	pInstance->Set_NowSceneNum(eSceneNum);

	//if (pInstance->Get_NameTag() == nullptr)
	//	pInstance->Set_NameTag(tagLayer);

	*ppOutGameObj = pInstance;


	return S_OK;
}

HRESULT CObjectMgr::Add_GameObject_To_Layer(_uint eSceneNum, const _tchar * tagLayer, const _tchar * tagPrototype, void * pArg)
{
	if (eSceneNum >= m_iMaxSceneNum)
	{
		__debugbreak();
		return E_FAIL;
	}

	CGameObject* pPrototype = Find_Prototype(tagPrototype);

	NULL_CHECK_BREAK(pPrototype);


	CGameObject* pInstance =  pPrototype->Clone(pArg);

	NULL_CHECK_BREAK(pInstance);

	pInstance->Set_NowSceneNum(eSceneNum);

	if (pInstance->Get_NameTag() == nullptr)
		pInstance->Set_NameTag(tagLayer);

	CObjectLayer* pLayer = Find_Layer(eSceneNum,tagLayer);

	if (pLayer == nullptr)
	{
		pLayer = CObjectLayer::Create();
		NULL_CHECK_BREAK(pLayer);


		FAILED_CHECK(pLayer->Add_GameObject(pInstance));

		m_mapLayer[eSceneNum].emplace(tagLayer, pLayer);
	}
	else 
	{
		FAILED_CHECK(pLayer->Add_GameObject(pInstance));

		m_mapLayer[eSceneNum].emplace(tagLayer, pLayer);
	}



	return S_OK;
}

HRESULT CObjectMgr::Add_GameObject_To_Layer_By_Parameter(_uint eSceneNum, const _tchar * tagLayer, CGameObject * pGameObject)
{


	CGameObject* pInstance = pGameObject;

	NULL_CHECK_BREAK(pInstance);

	pInstance->Set_NowSceneNum(eSceneNum);

	if (pInstance->Get_NameTag() == nullptr)
		pInstance->Set_NameTag(tagLayer);

	CObjectLayer* pLayer = Find_Layer(eSceneNum, tagLayer);

	if (pLayer == nullptr)
	{
		pLayer = CObjectLayer::Create();
		NULL_CHECK_BREAK(pLayer);


		FAILED_CHECK(pLayer->Add_GameObject(pInstance));

		m_mapLayer[eSceneNum].emplace(tagLayer, pLayer);
	}
	else
	{
		FAILED_CHECK(pLayer->Add_GameObject(pInstance));

		m_mapLayer[eSceneNum].emplace(tagLayer, pLayer);
	}

	return S_OK;
}

HRESULT CObjectMgr::Delete_GameObject_To_Layer_Index(_uint eSceneNum, const _tchar * tagLayer, int index)
{
	if (eSceneNum >= m_iMaxSceneNum || m_mapLayer == nullptr)
	{
		__debugbreak();
		return E_FAIL;
	}

	CObjectLayer* pLayer = Find_Layer(eSceneNum, tagLayer);

	if (pLayer == nullptr)
		return E_FAIL;
	
	FAILED_CHECK(pLayer->Delete_GameObject_By_LayerIndex(index));

	return S_OK;
}


HRESULT CObjectMgr::Delete_GameObject_To_Layer_Object(_uint eSceneNum, const _tchar * tagLayer, CGameObject* obj)
{
	if (eSceneNum >= m_iMaxSceneNum || m_mapLayer == nullptr)
		return E_FAIL;

	CObjectLayer* pLayer = Find_Layer(eSceneNum, tagLayer);

	NULL_CHECK_BREAK(pLayer);


	FAILED_CHECK(pLayer->Delete_GameObject_By_LayerObject(obj));


	return S_OK;
}

HRESULT CObjectMgr::Clear_Scene_GameObjects(_uint eSceneNum)
{

	if (eSceneNum >= m_iMaxSceneNum)
	{
		__debugbreak();
		return E_FAIL;
	}

	for (auto& pair : m_mapLayer[eSceneNum])
		Safe_Release(pair.second);


	m_mapLayer[eSceneNum].clear();

	
	return S_OK;
}

HRESULT CObjectMgr::Reset_StaticObject_NowSceneNum(_uint eSceneNum)
{
	for (auto& pair : m_mapLayer[0])
		pair.second->Set_NowSceneNum(eSceneNum);

	return S_OK;
}

CComponent * CObjectMgr::Get_Commponent_By_LayerIndex(_uint eSceneNum, const _tchar * tagLayer, const _tchar* tagComponet,  _uint iLayerIndex)
{
	if (eSceneNum >= m_iMaxSceneNum || m_mapLayer == nullptr)
		return nullptr;

	CObjectLayer* pLayer = Find_Layer(eSceneNum, tagLayer);

	if (pLayer == nullptr)
	{
		pLayer = Find_Layer(0, tagLayer);

		if (pLayer == nullptr)
			return nullptr;
	}

	return pLayer->Get_Commponent_By_LayerIndex(tagComponet, iLayerIndex);
}

CGameObject * CObjectMgr::Get_GameObject_By_LayerIndex(_uint eSceneNum, const _tchar * tagLayer, _uint iLayerIndex)
{
	if (eSceneNum >= m_iMaxSceneNum || m_mapLayer == nullptr)
		return nullptr;

	CObjectLayer* pLayer = Find_Layer(eSceneNum, tagLayer);

	if (pLayer == nullptr)
	{
		pLayer = Find_Layer(0, tagLayer);

		if (pLayer == nullptr)
			return nullptr;
	}

	return pLayer->Get_GameObject_By_LayerIndex(iLayerIndex);
}

CGameObject * CObjectMgr::Get_GameObject_By_LayerLastIndex(_uint eSceneNum, const _tchar * tagLayer)
{
	if (eSceneNum >= m_iMaxSceneNum || m_mapLayer == nullptr)
		return nullptr;

	CObjectLayer* pLayer = Find_Layer(eSceneNum, tagLayer);

	if (pLayer == nullptr)
	{
		pLayer = Find_Layer(0, tagLayer);

		if (pLayer == nullptr)
			return nullptr;
	}

	return pLayer->Get_GameObject_By_LayerLastIndex();
}




list<CGameObject*>* CObjectMgr::Get_ObjectList_from_Layer(_uint iSceneNum, const _tchar * tagLayer)
{
	if (iSceneNum >= m_iMaxSceneNum || m_mapLayer == nullptr)
		return nullptr;

	CObjectLayer* pLayer = Find_Layer(iSceneNum, tagLayer);
	
	if (pLayer == nullptr)
	{
		pLayer = Find_Layer(0, tagLayer);

		if (pLayer == nullptr)
			return nullptr;
	}

	return pLayer->Get_ObjectList();
}

_int CObjectMgr::Update(_double fDeltaTime)
{
	for (_uint eSceneNym = 0 ; eSceneNym < m_iMaxSceneNum; eSceneNym++)
	{
		for (auto& pair : m_mapLayer[eSceneNym]) 
		{
			if (pair.second->Update(fDeltaTime) < 0)
			{
				__debugbreak();
				return -1;
			}
		}
	}


	return 0;
}

_int CObjectMgr::LateUpdate(_double fDeltaTime)
{
	for (_uint eSceneNym = 0; eSceneNym < m_iMaxSceneNum; eSceneNym++)
	{
		for (auto& pair : m_mapLayer[eSceneNym])
		{
			if (pair.second->LateUpdate(fDeltaTime) < 0)
			{
				__debugbreak();
				return -1;
			}

		}
	}
	return 0;
}

CGameObject * CObjectMgr::Find_Prototype(const _tchar * tagPrototype)
{
	auto iter = find_if(m_mapPrototypes.begin(), m_mapPrototypes.end(), CTagFinder(tagPrototype));

	if (iter == m_mapPrototypes.end())
		return nullptr;

	return iter->second;
}

CObjectLayer * CObjectMgr::Find_Layer(_uint iSceneNum, const _tchar * tagLayer)
{

	auto iter = find_if(m_mapLayer[iSceneNum].begin(), m_mapLayer[iSceneNum].end(), CTagFinder(tagLayer));

	if (iter == m_mapLayer[iSceneNum].end())
		return nullptr;

	return iter->second;
}



void CObjectMgr::Free()
{

	for (_uint iSceneNum = 0; iSceneNum < m_iMaxSceneNum; ++iSceneNum)
	{
		for (auto& pair : m_mapLayer[iSceneNum])
			Safe_Release(pair.second);


		m_mapLayer[iSceneNum].clear();
	}

	Safe_Delete_Array(m_mapLayer);

	for (auto& pair : m_mapPrototypes)
	{
		Safe_Release(pair.second);
	}
	m_mapPrototypes.clear();


}
