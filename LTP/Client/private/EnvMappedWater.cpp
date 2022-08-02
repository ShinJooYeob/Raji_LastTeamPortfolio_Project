#include "stdafx.h"
#include "..\public\EnvMappedWater.h"


CEnvMappedWater::CEnvMappedWater(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CEnvMappedWater::CEnvMappedWater(const CEnvMappedWater & rhs)
	: CGameObject(rhs)
{
}

HRESULT CEnvMappedWater::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CEnvMappedWater::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));
	NULL_CHECK_RETURN(pArg, E_FAIL);

	memcpy(&m_tEmwDesc, pArg, sizeof(EMWDESC));
	m_tEmwDesc.vNoisePushDir = m_tEmwDesc.vNoisePushDir.Get_Nomalize();

	FAILED_CHECK(SetUp_Components());

	

	Set_LimLight_N_Emissive(_float4(0), m_tEmwDesc.vEmissive);
	Set_IsOcllusion(false);
	m_fPassedTime = 0;
	return S_OK;
}

_int CEnvMappedWater::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0) return -1;

	m_fPassedTime += (_float)fDeltaTime * m_tEmwDesc.fFlowRate;

	return _int();
}

_int CEnvMappedWater::LateUpdate(_double fDeltaTimer)
{
	if (__super::LateUpdate(fDeltaTimer) < 0) return -1;

	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ENVMAPPED, this));

	return _int();
}

_int CEnvMappedWater::Render()
{
	if (__super::Render() < 0)		return -1;


	FAILED_CHECK(m_pRendererCom->End_RenderTarget(TEXT("MRT_Material")));
	FAILED_CHECK(m_pRendererCom->Copy_BluredMtrlDiffuse(3.f));
	FAILED_CHECK(m_pRendererCom->Begin_RenderTarget(TEXT("MRT_Material")));



	NULL_CHECK_RETURN(m_pVIBufferCom, E_FAIL);

	CGameInstance* pInstance = GetSingle(CGameInstance);

	
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_fTimer", &m_fPassedTime, sizeof(_float)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_vColor", &m_tEmwDesc.vMixColor,  sizeof(_float4)));
	
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));

	FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));


	CUtilityMgr* pUtil = GetSingle(CUtilityMgr);

	FAILED_CHECK(m_pTextureCom->Bind_OnShader(m_pShaderCom, "g_NoiseTexture", 0));
	FAILED_CHECK(m_pTextureCom->Bind_OnShader(m_pShaderCom, "g_SourTexture", 0));
	FAILED_CHECK(m_pShaderCom->Set_Texture("g_BackBufferTexture", g_pGameInstance->Get_SRV(L"Target_ReferenceMtrlDiffuse")));

	FAILED_CHECK(m_pShaderCom->Set_RawValue("noisingdir", &m_tEmwDesc.vNoisePushDir, sizeof(_float2)));


	const LIGHTDESC* pLightDesc = g_pGameInstance->Get_LightDesc(tagLightDesc::TYPE_DIRECTIONAL, 0);
	_float4		vLightPosition = pLightDesc->vVector;

	//_float4		vLightPosition = pInstance->Get_TargetPostion_float4(PLV_CAMERA);

	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_vLightPosition", &vLightPosition, sizeof(_float4)));

	FAILED_CHECK(m_pTextureCom->Bind_OnShader(m_pShaderCom, "g_DiffuseTexture", 2));



	FAILED_CHECK(m_pVIBufferCom->Render(m_pShaderCom,9));


	return _int();
}

_int CEnvMappedWater::LateRender()
{
	return _int();
}

HRESULT CEnvMappedWater::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VT), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom));

	m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, m_tEmwDesc.vPosition);

	m_pTransformCom->Rotation_CW(XMVectorSet(m_tEmwDesc.vRotAxis_N_Angle.x, m_tEmwDesc.vRotAxis_N_Angle.y, m_tEmwDesc.vRotAxis_N_Angle.z, 0),
		XMConvertToRadians(m_tEmwDesc.vRotAxis_N_Angle.w));

	m_pTransformCom->Scaled_All(m_tEmwDesc.vScale);


	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_VIBuffer_Rect), TAG_COM(Com_VIBuffer), (CComponent**)&m_pVIBufferCom));

	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Texture_EnvMappedWater), TAG_COM(Com_Texture), (CComponent**)&m_pTextureCom));



	return S_OK;
}




CEnvMappedWater * CEnvMappedWater::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CEnvMappedWater*	pInstance = NEW CEnvMappedWater(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CEnvMappedWater");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CEnvMappedWater::Clone(void * pArg)
{
	CEnvMappedWater*	pInstance = NEW CEnvMappedWater(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CEnvMappedWater");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEnvMappedWater::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	
}
