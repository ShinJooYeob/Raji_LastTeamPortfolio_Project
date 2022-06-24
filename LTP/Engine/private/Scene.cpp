#include "Scene.h"




CScene::CScene(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: m_pDevice(pDevice)
	, m_pDeviceContext(pDeviceContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);
}

HRESULT CScene::Initialize()
{
	return S_OK;
}

_int CScene::Update(_double fDeltaTime)
{
	return _int();
}

_int CScene::LateUpdate(_double fDeltaTime)
{
	return _int();
}

_int CScene::Render()
{
	return _int();
}

_int CScene::LateRender()
{
	return _int();
}

HRESULT CScene::Set_SceneChanging(_uint _eNextScene)
{
	m_bIsNeedToSceneChange = true;
	m_eNextScene = _eNextScene;

	return S_OK;
}

void CScene::Free()
{
	Safe_Release(m_pDeviceContext);
	Safe_Release(m_pDevice);
}
