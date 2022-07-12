#include "stdafx.h"
#include "..\public\ShellingSkillRange.h"
#include "Player.h"
#include "ShellingSkillAttackPoint.h"

CShellingSkillRange::CShellingSkillRange(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CShellingSkillRange::CShellingSkillRange(const CShellingSkillRange & rhs)
	: CGameObject(rhs)
{
}

HRESULT CShellingSkillRange::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CShellingSkillRange::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components());

	memcpy(&m_ShellingSkillRangeDesc, pArg, sizeof(SHELLINGSKILLRANGEDESC));

	if (pArg != nullptr)
	{
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, m_ShellingSkillRangeDesc.fInitPos);
	}

	m_pTransformCom->Rotation_CW(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(90.f));
	m_pOwnerTransform = static_cast<CTransform*>(m_ShellingSkillRangeDesc.pOwner->Get_Component(TAG_COM(Com_Transform)));

	m_pTransformCom->Set_ScalingSpeed(m_ShellingSkillRangeDesc.fSpeed);

	FAILED_CHECK(SetUp_Etc());

	return S_OK;
}

_int CShellingSkillRange::Update(_double fDeltaTime)
{
	if (false == m_bActive)
	{
		return 0;
	}

	if (__super::Update(fDeltaTime) < 0) return -1;

	_Vector vOwnerPos = m_pOwnerTransform->Get_MatrixState(CTransform::TransformState::STATE_POS);
	vOwnerPos = XMVectorSetY(vOwnerPos, XMVectorGetY(vOwnerPos) + 0.1f);
	m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, vOwnerPos);

	m_pTransformCom->Scaling_All(fDeltaTime);
	if (XMVectorGetX(m_pTransformCom->Get_Scale()) >= m_ShellingSkillRangeDesc.fMaxDist)
	{
		m_pTransformCom->Scaled_All(_float3(m_ShellingSkillRangeDesc.fMaxDist, m_ShellingSkillRangeDesc.fMaxDist, m_ShellingSkillRangeDesc.fMaxDist));
		m_pShellingSkillAttackPoint->Set_Active(true);
	}
	return _int();
}

_int CShellingSkillRange::LateUpdate(_double fDeltaTime)
{
	if (false == m_bActive)
	{
		return 0;
	}

	if (__super::LateUpdate(fDeltaTime) < 0)return -1;


	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_BLEND, this));

	return _int();
}

_int CShellingSkillRange::Render()
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

_int CShellingSkillRange::LateRender()
{
	return _int();
}

void CShellingSkillRange::Set_Active(_bool bActive)
{
	m_bActive = bActive;
	m_pTransformCom->Scaled_All(_float3(m_ShellingSkillRangeDesc.fStartDist, m_ShellingSkillRangeDesc.fStartDist, m_ShellingSkillRangeDesc.fStartDist));

	if (false == bActive)
	{
		m_pShellingSkillAttackPoint->Set_Active(false);
	}
}

_bool CShellingSkillRange::Get_IsActive()
{
	return m_bActive;
}

_float3 CShellingSkillRange::Get_AttackPoint()
{
	return m_pShellingSkillAttackPoint->Get_AttackPointPos();
}

HRESULT CShellingSkillRange::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VT), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_VIBuffer_Rect), TAG_COM(Com_VIBuffer), (CComponent**)&m_pVIBufferCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Texture_ShellingRange), TAG_COM(Com_Texture), (CComponent**)&m_pTextureCom));

	CTransform::TRANSFORMDESC tDesc = {};
	tDesc.fMovePerSec = 5;
	tDesc.fRotationPerSec = XMConvertToRadians(60);
	tDesc.fScalingPerSec = 1;
	tDesc.vPivot = _float3(0, 0, 0);
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));

	return S_OK;
}

HRESULT CShellingSkillRange::SetUp_Etc()
{
	m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
	
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_PlayerEffect), TAG_OP(Prototype_PlayerEffect_ShellingPoint), &(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS))));
	m_pShellingSkillAttackPoint = (CShellingSkillAttackPoint*)(g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_PlayerEffect)));
	m_pShellingSkillAttackPoint->Set_OwnerTransform(m_pTransformCom);
	m_pShellingSkillAttackPoint->Set_Active(false);
	m_pShellingSkillAttackPoint->Set_MaxRange(m_ShellingSkillRangeDesc.fMaxDist);
	return S_OK;
}

CShellingSkillRange * CShellingSkillRange::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CShellingSkillRange*	pInstance = new CShellingSkillRange(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CShellingSkillRange");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CShellingSkillRange::Clone(void * pArg)
{
	CShellingSkillRange*	pInstance = new CShellingSkillRange(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CShellingSkillRange");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CShellingSkillRange::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);

}

