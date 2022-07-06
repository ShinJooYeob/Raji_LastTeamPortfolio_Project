#include "stdafx.h"
#include "ESCursor.h"



CESCursor::CESCursor(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext) 
	: CGameObject(pDevice, pDeviceContext)
{
}

CESCursor::CESCursor(const CESCursor & rhs)
	: CGameObject(rhs)
{
}

HRESULT CESCursor::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));


	return S_OK;
}

HRESULT CESCursor::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components());


	return S_OK;
}

_int CESCursor::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)
		return -1;

	return _int();
}

_int CESCursor::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)
		return -1;

	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_PRIORITY, this));

	return _int();
}

_int CESCursor::Render()
{
	if (__super::Render() < 0)
		return -1;
	NULL_CHECK_RETURN(m_pVIBufferCom, E_FAIL);


	FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));


	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_vColor", &m_vColor, sizeof(_float4)));
	

	CGameInstance* pInstance = GetSingle(CGameInstance);

	_float4x4		ViewFloat4x4 = pInstance->Get_Transform_Float4x4_TP(PLM_VIEW);
	_float4x4		ProjFloat4x4 = pInstance->Get_Transform_Float4x4_TP(PLM_PROJ);

	m_pShaderCom->Set_RawValue("g_ViewMatrix", &ViewFloat4x4, sizeof(_float4x4));
	m_pShaderCom->Set_RawValue("g_ProjMatrix", &ProjFloat4x4, sizeof(_float4x4));

	//FAILED_CHECK(m_pTextureCom->Bind_OnShader(m_pShaderCom, "g_DiffuseTexture"));

	FAILED_CHECK(m_pVIBufferCom->Render(m_pShaderCom, 3));

	return _int();
}

_int CESCursor::LateRender()
{
	if (__super::LateRender() < 0)
		return -1;




	return _int();
}

void CESCursor::Set_Position(_float3 vPosition)
{
	m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, vPosition);
}

HRESULT CESCursor::SetUp_Components()
{


	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VCT), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_VIBuffer_Cube), TAG_COM(Com_VIBuffer), (CComponent**)&m_pVIBufferCom));


	CTransform::TRANSFORMDESC tDesc = {};

	tDesc.fMovePerSec = 5;
	tDesc.fRotationPerSec = XMConvertToRadians(60);
	tDesc.fScalingPerSec = 1;
	tDesc.vPivot = _float3(0, 0, 0);

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));


	

	return S_OK;
}

CESCursor * CESCursor::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CESCursor*	pInstance = new CESCursor(pDevice,pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CESCursor");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CESCursor::Clone(void * pArg)
{
	CESCursor*	pInstance = new CESCursor(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CESCursor");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CESCursor::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	
}
