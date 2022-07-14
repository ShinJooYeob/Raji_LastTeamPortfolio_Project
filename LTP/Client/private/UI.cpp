#include "stdafx.h"
#include "..\public\UI.h"

CUI::CUI(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CUI::CUI(const CUI & rhs)
	: CGameObject(rhs)
{
}

HRESULT CUI::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CUI::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	if (nullptr != pArg)
	{
		SETTING_UI UI_Setting;
		ZeroMemory(&UI_Setting, sizeof(SETTING_UI));

		memcpy(&UI_Setting, pArg, sizeof(SETTING_UI));

		SetUp_UIInfo(UI_Setting);
	}

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH((_float)g_iWinCX, (_float)g_iWinCY, 0.f, 1.f)));

	m_pTransformCom->Scaled_All(XMVectorSet(m_fSizeX, m_fSizeY, 1.f, 0.0f));
	m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, XMVectorSet(m_fX - ((_float)g_iWinCX * 0.5f), -m_fY + ((_float)g_iWinCY * 0.5f), 0.f, 1.f));

	m_fAliveTime = 1.5f;

	return S_OK;
}

_int CUI::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)return -1;



	Update_Rect();

	return _int();
}

_int CUI::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)return -1;

	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this));

	return _int();
}

_int CUI::Render()
{
	if (!m_bDraw)
		return S_OK;

	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	//if (m_pUIName == TEXT("UI_Damage_Num"))
	//	_int a = 0;

	m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix");
	m_pShaderCom->Set_RawValue("g_ViewMatrix", &XMMatrixIdentity(), sizeof(_float4x4));
	m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));

	if (FAILED(m_pTextureCom->Bind_OnShader(m_pShaderCom, "g_DiffuseTexture", m_iTextureIndex)))
		return E_FAIL;

	//if (m_pUIName == TEXT("UI_Fade"))
	//	m_pShaderCom->Set_RawValue("g_Alpha", &m_fFadeTime, sizeof(_float));

	//if (m_pUIName == TEXT("UI_Fade"))
	//	m_pVIBufferCom->Render(m_pShaderCom, 5);
	//else
	//	m_pVIBufferCom->Render(m_pShaderCom, 0);

	m_pVIBufferCom->Render(m_pShaderCom, 0);


	return S_OK;
}

_int CUI::LateRender()
{

	return _int();
}

HRESULT CUI::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VT), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_VIBuffer_Rect), TAG_COM(Com_VIBuffer), (CComponent**)&m_pVIBufferCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom));

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(m_iLevelIndex, m_pPrototypeTag, m_pComponentTag, (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

void CUI::SetUp_UIInfo(SETTING_UI & pStruct)
{
	m_fX = pStruct.m_fX;
	m_fY = pStruct.m_fY;
	m_fSizeX = pStruct.m_fSizeX;
	m_fSizeY = pStruct.m_fSizeY;

	m_pUIName = pStruct.pUI_Name;

	m_bColl = pStruct.bColl;
	m_bMove = pStruct.bMove;
	m_bDraw = pStruct.bDraw;
	m_bClick = pStruct.bClick;
	m_iLevelIndex = pStruct.iLevelIndex;
	m_pPrototypeTag = pStruct.pPrototypeTag;
	m_pComponentTag = pStruct.pComponentTag;
}

void CUI::Update_Rect()
{
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH((_float)g_iWinCX, (_float)g_iWinCY, 0.f, 1.f)));

	m_pTransformCom->Scaled_All(XMVectorSet(m_fSizeX, m_fSizeY, 1.f, 0.0f));
	m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, XMVectorSet(m_fX - (g_iWinCX * 0.5f), -m_fY + (g_iWinCY * 0.5f), 0.f, 1.f));
}

CUI * CUI::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void* pArg)
{
	CUI*	pInstance = new CUI(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CUI");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CUI::Clone(void * pArg)
{
	CUI*	pInstance = new CUI(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created  Clone CUI");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}
