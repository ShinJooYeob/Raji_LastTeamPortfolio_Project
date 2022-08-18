#include "stdafx.h"
#include "..\public\CircusBackground.h"
#include "Player.h"

CCircusBackground::CCircusBackground(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CCircusBackground::CCircusBackground(const CCircusBackground & rhs)
	: CGameObject(rhs)
{
}

HRESULT CCircusBackground::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CCircusBackground::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components());

	_float3 vPos = *(static_cast<_float3*>(pArg));
	m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, vPos);
	
	FAILED_CHECK(SetUp_Etc());

	return S_OK;
}

_int CCircusBackground::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0) return -1;

	return _int();
}

_int CCircusBackground::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)return -1;

	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_BLEND, this));

	return _int();
}

_int CCircusBackground::Render()
{
	if (__super::Render() < 0)
		return -1;

	NULL_CHECK_RETURN(m_pVIBufferCom, E_FAIL);


	FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));

	CGameInstance* pInstance = GetSingle(CGameInstance);
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));

	if (FAILED(m_pTextureCom->Bind_OnShader(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;

	m_pVIBufferCom->Render(m_pShaderCom, 3);

	return _int();
}

_int CCircusBackground::LateRender()
{
	return _int();
}

HRESULT CCircusBackground::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VT), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_VIBuffer_Rect), TAG_COM(Com_VIBuffer), (CComponent**)&m_pVIBufferCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Texture_MiniGameJino_Background), TAG_COM(Com_Texture), (CComponent**)&m_pTextureCom));

	CTransform::TRANSFORMDESC tDesc = {};
	tDesc.fMovePerSec = 5;
	tDesc.fRotationPerSec = XMConvertToRadians(60);
	tDesc.fScalingPerSec = 1;
	tDesc.vPivot = _float3(0, 0, 0);
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));

	return S_OK;
}

HRESULT CCircusBackground::SetUp_Etc()
{
	m_pTransformCom->Scaled_All(_float3(20.f, 5.f, 1.f));
	return S_OK;
}

CCircusBackground * CCircusBackground::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CCircusBackground*	pInstance = NEW CCircusBackground(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CCircusBackground");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CCircusBackground::Clone(void * pArg)
{
	CCircusBackground*	pInstance = NEW CCircusBackground(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CCircusBackground");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCircusBackground::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);

}

