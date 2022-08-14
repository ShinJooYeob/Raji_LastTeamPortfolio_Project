#include "stdafx.h"
#include "..\public\Rangda_Finger.h"

CRangda_Finger::CRangda_Finger(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CRangda_Finger::CRangda_Finger(const CRangda_Finger & rhs)
	: CGameObject(rhs)
{
}

HRESULT CRangda_Finger::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));
	return S_OK;
}
HRESULT CRangda_Finger::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));
	FAILED_CHECK(SetUp_Components());


	if (pArg != nullptr)
	{
		_float3 Pos = *(_float3*)pArg;
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, Pos);

	}
	m_iPassIndex = 3;

	m_pTransformCom->Scaled_All(_float3(5.f));
	m_pTransformCom->Rotation_CCW(XMVectorSet(0,1.f,0,0),XMConvertToRadians(90.f));

	return S_OK;
}

_int CRangda_Finger::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)
		return -1;

	m_pDissolveCom->Update_Dissolving(fDeltaTime);

	m_fAliveTime -= (_float)fDeltaTime;

	if (m_fAliveTime <= 0 && m_bIsDissolveStart == false)
	{
		m_bIsDissolveStart = true;
		//Set_IsDead();
		m_fAliveTime = 4.f;
	}

	if (m_pDissolveCom->Get_IsFadeIn() == false && m_pDissolveCom->Get_DissolvingRate() >= 1.0)
	{
		Set_IsDead();
	}

	if (m_bIsDissolveStart)
		m_pDissolveCom->Set_DissolveOn(false, 5.f);

	return _int();
}

_int CRangda_Finger::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)
		return -1;

	FAILED_CHECK(m_pRendererCom->Add_ShadowGroup(CRenderer::SHADOW_NONANIMMODEL, this, m_pTransformCom, m_pShaderCom, m_pModel));

	if (g_pGameInstance->IsNeedToRender(m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS), m_fFrustumRadius))
	{
		FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
	}

	return _int();
}

_int CRangda_Finger::Render()
{
	if (__super::Render() < 0)
		return -1;

	NULL_CHECK_RETURN(m_pModel, E_FAIL);


	FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));

	CGameInstance* pInstance = GetSingle(CGameInstance);
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));

	FAILED_CHECK(m_pDissolveCom->Render(3));

	//_uint NumMaterial = m_pModel->Get_NumMaterial();

	//for (_uint i = 0; i < NumMaterial; i++)
	//{

	//	for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; j++)
	//		FAILED_CHECK(m_pModel->Bind_OnShader(m_pShaderCom, i, j, MODLETEXTYPE(j)));

	//	FAILED_CHECK(m_pModel->Render(m_pShaderCom, m_iPassIndex, i));
	//}

	return 0;
}

_int CRangda_Finger::LateRender()
{
	if (__super::LateRender() < 0)
		return -1;

	return _int();
}

HRESULT CRangda_Finger::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VNAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_Boss_Rangda_Finger), TAG_COM(Com_Model), (CComponent**)&m_pModel));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom));

	CDissolve::DISSOLVEDESC DissolveDesc;
	DissolveDesc.pModel = m_pModel;
	DissolveDesc.eDissolveModelType = CDissolve::DISSOLVE_NONANIM;
	DissolveDesc.pShader = m_pShaderCom;
	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Dissolve), TAG_COM(Com_Dissolve), (CComponent**)&m_pDissolveCom, &DissolveDesc));
	return S_OK;
}

CRangda_Finger * CRangda_Finger::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CRangda_Finger*	pInstance = NEW CRangda_Finger(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CRangda_Finger");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CRangda_Finger::Clone(void * pArg)
{
	CRangda_Finger*	pInstance = NEW CRangda_Finger(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created Clone CRangda_Finger");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CRangda_Finger::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModel);
	Safe_Release(m_pDissolveCom);
}
