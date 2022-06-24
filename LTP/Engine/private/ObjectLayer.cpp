#include "..\Public\ObjectLayer.h"

#include "GameObject.h"


CObjectLayer::CObjectLayer()
{
}

HRESULT CObjectLayer::Add_GameObject(CGameObject * pGameObject)
{
	m_ObjectList.push_back(pGameObject);
	return S_OK;
}

_int CObjectLayer::Update(_double fDeltaTime)
{

	auto ObjectIter = m_ObjectList.begin();

	for (; ObjectIter != m_ObjectList.end();)
	{
		if ((*ObjectIter)->Get_IsDead())
		{
			Safe_Release(*ObjectIter);
			ObjectIter = m_ObjectList.erase(ObjectIter);
		}
		else
		{
			if ((*ObjectIter)->Update(fDeltaTime) < 0)
				return -1;

			ObjectIter++;
		}
	}

	return _int();
}

_int CObjectLayer::LateUpdate(_double fDeltaTime)
{

	auto ObjectIter = m_ObjectList.begin();

	for (; ObjectIter != m_ObjectList.end();)
	{
		if ((*ObjectIter)->Get_IsDead())
		{
			Safe_Release(*ObjectIter);
			ObjectIter = m_ObjectList.erase(ObjectIter);
		}
		else
		{
			if ((*ObjectIter)->LateUpdate(fDeltaTime) < 0)
				return -1;

			ObjectIter++;
		}
	}




	return _int();
}

CComponent * CObjectLayer::Get_Commponent_By_LayerIndex(const _tchar* tagComponet, _uint iLayerIndex)
{
	if (iLayerIndex >= m_ObjectList.size())
		return nullptr;

	auto iter = m_ObjectList.begin();

	for (_uint i = 0; i<iLayerIndex;i++)
		iter++;

	
	return (*iter)->Find_Components(tagComponet);
}

CGameObject * CObjectLayer::Get_GameObject_By_LayerIndex(_uint iLayerIndex)
{
	if (iLayerIndex >= m_ObjectList.size())
		return nullptr;

	auto iter = m_ObjectList.begin();

	for (_uint i = 0; i < iLayerIndex; i++)
		iter++;

	return (*iter);
}

CGameObject * CObjectLayer::Get_GameObject_By_LayerLastIndex()
{

	if (m_ObjectList.size() == 0)
		return nullptr;

	auto iter =(m_ObjectList.back());

	return (iter);

}

HRESULT CObjectLayer::Delete_GameObject_By_LayerIndex(_uint iIndex)
{
	// 특정 오브젝트 삭제 / 인덱스번호
	if (iIndex >= m_ObjectList.size())
	{
		__debugbreak();
		return E_FAIL;
	}

	int index = 0;
	for (auto iter = m_ObjectList.begin(); iter!=m_ObjectList.end();iter++)
	{
		if (index == iIndex)
		{
			(*iter)->Set_IsDead();
			return S_OK;
		}
		index++;
	}

	__debugbreak();
	return E_FAIL;
}

HRESULT CObjectLayer::Delete_GameObject_By_LayerObject(CGameObject * obj)
{
	// 특정 오브젝트 삭제 / obj 포인터
	for (auto iter = m_ObjectList.begin(); iter != m_ObjectList.end(); iter++)
	{
		if (*iter == obj)
		{
			(*iter)->Set_IsDead();
			return S_OK;
		}
	}
	return S_OK;
}

HRESULT CObjectLayer::Set_NowSceneNum(_uint eScene)
{
	for (auto& pGameObeject : m_ObjectList)
		pGameObeject->Set_NowSceneNum(eScene);
	
	return S_OK;
}




CObjectLayer * CObjectLayer::Create()
{
	CObjectLayer* pLayer = new CObjectLayer();
	return pLayer;
}

void CObjectLayer::Free()
{
	for (auto& pGameObject : m_ObjectList)
	{
		//pGameObject->Release();
		Safe_Release(pGameObject);
	}
	m_ObjectList.clear();

}
