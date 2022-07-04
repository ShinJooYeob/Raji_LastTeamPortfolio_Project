#include "stdafx.h"
#include "..\public\Rangda_MagicCircle.h"




CRangda_MagicCircle::CRangda_MagicCircle(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CRangda_MagicCircle::CRangda_MagicCircle(const CRangda_MagicCircle & rhs)
	: CGameObject(rhs)
{
}

HRESULT CRangda_MagicCircle::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));


	return S_OK;
}

HRESULT CRangda_MagicCircle::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components());

	if (pArg != nullptr)
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, *((_float3*)pArg));

	m_pTransformCom->Rotation_CW(XMVectorSet(1, 0, 0, 0), XMConvertToRadians(90));

	//Set_LimLight_N_Emissive(_float3(0.2f, 0.5f, 1.f ));


	m_pTransformCom->Scaled_All(_float3(10.f, 10.f, 10.f));

	m_fAlpha = 0.00001f;

	return S_OK;
}

_int CRangda_MagicCircle::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)
		return -1;

	m_fAlpha += (_float)fDeltaTime * 0.5f;

	if (m_fAlpha >= 1.f)
	{
		Set_IsDead();
	}



	return _int();
}

_int CRangda_MagicCircle::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)
		return -1;

	if (g_pGameInstance->IsNeedToRender(m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS), m_fFrustumRadius))
	{
		FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_BLEND, this));
	}


	return _int();
}

_int CRangda_MagicCircle::Render()
{
	if (__super::Render() < 0)
		return -1;

	NULL_CHECK_RETURN(m_pVIBufferCom, E_FAIL);


	FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));

	CGameInstance* pInstance = GetSingle(CGameInstance);
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));

	if (FAILED(m_pTextureCom->Bind_OnShader(m_pShaderCom, "g_DiffuseTexture",0)))
		return E_FAIL;

	m_pShaderCom->Set_RawValue("g_Alpha", &m_fAlpha, sizeof(_float));

	m_pVIBufferCom->Render(m_pShaderCom, 2);

	return 0;
}

_int CRangda_MagicCircle::LateRender()
{
	return _int();
}

void CRangda_MagicCircle::ChangeScaledAndTexture()
{

	m_pTextureCom->Change_TextureLayer(L"TestEffect2");
	m_pTransformCom->Scaled_All(_float3(15.f, 15.f, 15.f));
}

HRESULT CRangda_MagicCircle::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VT), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Texture_TestEffect), TAG_COM(Com_Texture), (CComponent**)&m_pTextureCom));

	m_pTextureCom->Change_TextureLayer(L"TestEffect");

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_VIBuffer_Rect), TAG_COM(Com_VIBuffer), (CComponent**)&m_pVIBufferCom));
	

	return S_OK;
}

CRangda_MagicCircle * CRangda_MagicCircle::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CRangda_MagicCircle*	pInstance = new CRangda_MagicCircle(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CRangda_MagicCircle");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CRangda_MagicCircle::Clone(void * pArg)
{
	CRangda_MagicCircle*	pInstance = new CRangda_MagicCircle(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created Clone CRangda_MagicCircle");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CRangda_MagicCircle::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}
