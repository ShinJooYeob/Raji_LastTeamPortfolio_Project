#include "stdafx.h"
#include "..\public\PlayerWeapon_Spear.h"

CPlayerWeapon_Spear::CPlayerWeapon_Spear(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CPlayerWeapon(pDevice, pDeviceContext)
{
}

CPlayerWeapon_Spear::CPlayerWeapon_Spear(const CPlayerWeapon_Spear & rhs)
	: CPlayerWeapon(rhs)
{
}

HRESULT CPlayerWeapon_Spear::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CPlayerWeapon_Spear::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components());

	//m_pTransformCom->Rotation_CW(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(170.f));

	FAILED_CHECK(SetUp_EtcInfo());
	Set_IsOcllusion(true);

	return S_OK;
}

_int CPlayerWeapon_Spear::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0) return -1;

	switch (m_tPlayerWeaponDesc.eWeaponState)
	{
	case EWeaponState::STATE_STRUCTURE:
		Update_Structure(fDeltaTime);
		break;
	case EWeaponState::STATE_EQUIP:
		Update_Equip(fDeltaTime);
		break;
	case EWeaponState::STATE_NON_EQUIP:
		Update_NoEquip(fDeltaTime);
		break;
	}

	return _int();
}

_int CPlayerWeapon_Spear::LateUpdate(_double fDeltaTimer)
{
	if (__super::LateUpdate(fDeltaTimer) < 0) return -1;

	switch (m_tPlayerWeaponDesc.eWeaponState)
	{
	case EWeaponState::STATE_STRUCTURE:
		LateUpdate_Structure(fDeltaTimer);
		break;
	case EWeaponState::STATE_EQUIP:
		LateUpdate_Equip(fDeltaTimer);
		break;
	case EWeaponState::STATE_NON_EQUIP:
		LateUpdate_NoEquip(fDeltaTimer);
		break;
	}

	m_fAttachedMatrix = m_fAttachedMatrix.TransposeXMatrix();

	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
	FAILED_CHECK(m_pRendererCom->Add_ShadowGroup(CRenderer::SHADOW_ANIMMODEL_ATTACHED, this, m_pTransformCom, m_pShaderCom, m_pModel, &m_fAttachedMatrix));
	return _int();
}

_int CPlayerWeapon_Spear::Render()
{
	if (__super::Render() < 0)		return -1;

	NULL_CHECK_RETURN(m_pModel, E_FAIL);

	CGameInstance* pInstance = GetSingle(CGameInstance);

	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_AttechMatrix",  &m_fAttachedMatrix, sizeof(_float4x4)));
	
	FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));


	////
	//if (nullptr != m_tPlayerWeaponDesc.pAttachTarget && STATE_EQUIP == m_tPlayerWeaponDesc.eWeaponState)
	//{
	//	_Matrix AttachTargetBoneMatrix = m_tPlayerWeaponDesc.pAttachTarget->Get_BoneMatrix(m_tPlayerWeaponDesc.pAttachBoneName);
	//}
	////


	_uint NumMaterial = m_pModel->Get_NumMaterial();

	for (_uint i = 0; i < NumMaterial; i++)
	{
		for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; j++)
		{
			FAILED_CHECK(m_pModel->Bind_OnShader(m_pShaderCom, i, j, MODLETEXTYPE(j)));
		}
		FAILED_CHECK(m_pModel->Render(m_pShaderCom, 4, i, "g_BoneMatrices"));
	}

	return _int();
}

_int CPlayerWeapon_Spear::LateRender()
{
	return _int();
}

_fVector CPlayerWeapon_Spear::Get_BonePos(const char * pBoneName)
{
	_Matrix BoneMatrix = m_pModel->Get_BoneMatrix(pBoneName);
	_Matrix TransformMatrix = BoneMatrix * m_pTransformCom->Get_WorldMatrix();
	_Vector vPos, vRot, vScale;
	XMMatrixDecompose(&vScale, &vRot, &vPos, TransformMatrix);

	return vPos;
}

void CPlayerWeapon_Spear::Update_AttachCamPos()
{
}

_int CPlayerWeapon_Spear::Update_Structure(_double fDeltaTime)
{
	m_pTransformCom->Turn_CCW(XMVectorSet(0.f, 1.f, 0.f, 0.f), fDeltaTime);

	return _int();
}

_int CPlayerWeapon_Spear::Update_Equip(_double fDeltaTime)
{

	return _int();
}

_int CPlayerWeapon_Spear::Update_NoEquip(_double fDeltaTime)
{
	return _int();
}

_int CPlayerWeapon_Spear::LateUpdate_Structure(_double fDeltaTime)
{
	return _int();
}

_int CPlayerWeapon_Spear::LateUpdate_Equip(_double fDeltaTime)
{

	Update_AttachMatrix();

	return _int();
}

_int CPlayerWeapon_Spear::LateUpdate_NoEquip(_double fDeltaTime)
{
	return _int();
}

void CPlayerWeapon_Spear::Update_AttachMatrix()
{
	m_fAttachedMatrix  =  m_pTransformCom->Get_WorldMatrix()  * m_tPlayerWeaponDesc.eAttachedDesc.Caculate_AttachedBoneMatrix();
}

HRESULT CPlayerWeapon_Spear::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_PlayerWeapon_Spear), TAG_COM(Com_Model), (CComponent**)&m_pModel));
	
	CTransform::TRANSFORMDESC tDesc = {};

	tDesc.fMovePerSec = 5;
	tDesc.fRotationPerSec = XMConvertToRadians(60);
	tDesc.fScalingPerSec = 1;
	tDesc.vPivot = _float3(0, 0, 0);

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));


	return S_OK;
}

HRESULT CPlayerWeapon_Spear::SetUp_EtcInfo()
{
	return S_OK;
}

CPlayerWeapon_Spear * CPlayerWeapon_Spear::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CPlayerWeapon_Spear*	pInstance = new CPlayerWeapon_Spear(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CPlayerWeapon_Spear");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CPlayerWeapon_Spear::Clone(void * pArg)
{
	CPlayerWeapon_Spear*	pInstance = new CPlayerWeapon_Spear(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CPlayerWeapon_Spear");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPlayerWeapon_Spear::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModel);
}