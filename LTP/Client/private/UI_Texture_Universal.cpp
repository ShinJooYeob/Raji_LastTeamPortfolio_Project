#include "stdafx.h"
#include "..\public\UI_Texture_Universal.h"

const _tchar* m_pUI_Texture_Universal_Tag[CUI_Texture_Universal::UI_END]
{
	L"UI_Skill_FireBall"
};

CUI_Texture_Universal::CUI_Texture_Universal(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{

}

CUI_Texture_Universal::CUI_Texture_Universal(const CUI_Texture_Universal & rhs)
	: CGameObject(rhs)
{
}

HRESULT CUI_Texture_Universal::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CUI_Texture_Universal::Initialize_Clone(void * pArg)
{
	if (nullptr != pArg)
	{
		memcpy(&m_UI_UniversalDesc, pArg, sizeof(UI_TEXTURE_UNIVERSALDESC));
	}
	else {
		__debugbreak();
		MSGBOX("CUI_Texture_Universal의 pArg가 Nullptr입니다.");
	}

	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components());

	//m_fX와 m_fY로 위치 조정하기
	m_fSizeX = m_UI_UniversalDesc.fSizeX;
	m_fSizeY = m_UI_UniversalDesc.fSizeY;
	m_fX = m_UI_UniversalDesc.fX;	//(g_iWinCX * 0.5f + 525) = 1165;
	m_fY = m_UI_UniversalDesc.fY;	//(g_iWinCY * 0.5f + 250) = 610;


	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH(g_iWinCX, g_iWinCY, 0.f, 1.f)));


	m_fRenderSortValue = m_UI_UniversalDesc.fDepth;

	return S_OK;
}

_int CUI_Texture_Universal::Update(_double dDeltaTime)
{
	if (__super::Update(dDeltaTime) < 0)return -1;

	Play_UI(dDeltaTime);


	return _int();
}

_int CUI_Texture_Universal::LateUpdate(_double dDeltaTime)
{
	if (__super::LateUpdate(dDeltaTime) < 0)return -1;

	Set_UI_Transform();


	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this));

	return _int();
}

_int CUI_Texture_Universal::Render()
{
	if (__super::Render() < 0)
		return -1;

	NULL_CHECK_RETURN(m_pVIBufferCom, E_FAIL);

	m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix");
	m_pShaderCom->Set_RawValue("g_ViewMatrix", &XMMatrixIdentity(), sizeof(_float4x4));
	m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));

	if (FAILED(m_pTextureCom->Bind_OnShader(m_pShaderCom, "g_DiffuseTexture", m_UI_UniversalDesc.iTextureIndex)))
		return E_FAIL;

	m_pVIBufferCom->Render(m_pShaderCom, 0);

	return _int();
}

_int CUI_Texture_Universal::LateRender()
{
	return _int();
}

HRESULT CUI_Texture_Universal::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VT), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_VIBuffer_Rect), TAG_COM(Com_VIBuffer), (CComponent**)&m_pVIBufferCom));

	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_UI_Texture_Universal), TAG_COM(Com_Texture), (CComponent**)&m_pTextureCom));

	m_pTextureCom->Change_TextureLayer(m_pUI_Texture_Universal_Tag[m_UI_UniversalDesc.iUI_TextureType]);

	CTransform::TRANSFORMDESC tDesc = {};

	tDesc.fMovePerSec = 5;
	tDesc.fRotationPerSec = XMConvertToRadians(60);
	tDesc.vPivot = _float3(0, 0, 0);
	tDesc.fScalingPerSec = 1;

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));


	return S_OK;
}

HRESULT CUI_Texture_Universal::Set_UI_Transform()
{
	m_pTransformCom->Scaled(CTransform::STATE_RIGHT, m_fSizeX);
	m_pTransformCom->Scaled(CTransform::STATE_UP, m_fSizeY);
	m_pTransformCom->Scaled(CTransform::STATE_LOOK, 1.f);
	m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, XMVectorSet(m_fX - (g_iWinCX * 0.5f), -m_fY + (g_iWinCY * 0.5f), 0.f, 1.f));
	return S_OK;
}

HRESULT CUI_Texture_Universal::Play_UI(_double dDeltaTime)
{
	return S_OK;
}

HRESULT CUI_Texture_Universal::Frame_Speed(_double dDeltaTime)
{
	return S_OK;
}

CUI_Texture_Universal * CUI_Texture_Universal::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void* pArg)
{
	CUI_Texture_Universal*	pInstance = NEW CUI_Texture_Universal(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CUI_Texture_Universal");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CUI_Texture_Universal::Clone(void * pArg)
{
	CUI_Texture_Universal*	pInstance = NEW CUI_Texture_Universal(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CUI_Texture_Universal");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_Texture_Universal::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}
