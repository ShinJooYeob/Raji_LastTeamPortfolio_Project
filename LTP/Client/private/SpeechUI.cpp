#include "stdafx.h"
#include "..\public\SpeechUI.h"


CSpeechUI::CSpeechUI(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CSpeechUI::CSpeechUI(const CSpeechUI & rhs)
	: CGameObject(rhs)
{
}

HRESULT CSpeechUI::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));


	return S_OK;
}

HRESULT CSpeechUI::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	if (pArg != nullptr)
	{
		memcpy(&m_FonstDesc, pArg, sizeof(SPEECHFONTDESC));
	}

	return S_OK;
}

_int CSpeechUI::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)return -1;

	return _int();
}

_int CSpeechUI::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)return -1;

	return _int();
}

_int CSpeechUI::Render()
{
	if (__super::Render() < 0)return -1;

	return _int();
}

_int CSpeechUI::LateRender()
{
	if (__super::LateRender() < 0)return -1;

	return _int();
}

HRESULT CSpeechUI::Render_Fonts()
{
	return S_OK;
}

HRESULT CSpeechUI::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	return S_OK;
}

CSpeechUI * CSpeechUI::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CSpeechUI*	pInstance = NEW CSpeechUI(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CSpeechUI");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CSpeechUI::Clone(void * pArg)
{
	CSpeechUI*	pInstance = NEW CSpeechUI(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created Clone CSpeechUI");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSpeechUI::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
}
