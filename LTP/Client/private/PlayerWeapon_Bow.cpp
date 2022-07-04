#include "stdafx.h"
#include "..\public\PlayerWeapon_Bow.h"



CPlayerWeapon_Bow::CPlayerWeapon_Bow(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CPlayerWeapon(pDevice, pDeviceContext)
{
}

CPlayerWeapon_Bow::CPlayerWeapon_Bow(const CPlayerWeapon_Bow & rhs)
	: CPlayerWeapon(rhs)
{
}

HRESULT CPlayerWeapon_Bow::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CPlayerWeapon_Bow::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components());

	FAILED_CHECK(SetUp_EtcInfo());

	return S_OK;
}

_int CPlayerWeapon_Bow::Update(_double fDeltaTime)
{
	if (true == m_bBlockUpdate)
		return 0;

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

	Set_Pivot();

	//m_pModel->Change_AnimIndex(0);
	FAILED_CHECK(m_pModel->Update_AnimationClip(fDeltaTime * m_fAnimSpeed, true));
	return _int();
}

_int CPlayerWeapon_Bow::LateUpdate(_double fDeltaTimer)
{
	if (true == m_bBlockUpdate)
		return 0;

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

_int CPlayerWeapon_Bow::Render()
{
	if (__super::Render() < 0)		return -1;

	NULL_CHECK_RETURN(m_pModel, E_FAIL);

	CGameInstance* pInstance = GetSingle(CGameInstance);

	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_AttechMatrix", &m_fAttachedMatrix, sizeof(_float4x4)));

	FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));

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

_int CPlayerWeapon_Bow::LateRender()
{
	return _int();
}

void CPlayerWeapon_Bow::PlayAnim_Idle()
{
	m_pModel->Change_AnimIndex(BOW_ANIM_IDLE);
	m_bOnceAnim = false;
}

void CPlayerWeapon_Bow::PlayAnim_NormalAttack_Ready()
{
	if (false == m_bOnceAnim)
	{
		m_pModel->Change_AnimIndex_ReturnTo(BOW_ANIM_NORMAL_READY, BOW_ANIM_NORMAL_LOOP, 0.1f, false);
		m_bOnceAnim = true;
		m_fAnimSpeed = 2.5f;
	}
}

void CPlayerWeapon_Bow::PlayAnim_NormalAttack_Shot()
{
	if (true == m_bOnceAnim)
	{
		m_pModel->Change_AnimIndex_ReturnTo_Must(BOW_ANIM_NORMAL_SHOT, BOW_ANIM_IDLE, 0.1f, false);
		m_bOnceAnim = false;
	}
}

void CPlayerWeapon_Bow::PlayAnim_UtilityAttack_Ready()
{
	if (false == m_bOnceAnim)
	{
		m_pModel->Change_AnimIndex_ReturnTo_Must(BOW_ANIM_UTILITY_READY, BOW_ANIM_NORMAL_LOOP, 0.1f, false);
		m_bOnceAnim = true;
		m_fAnimSpeed = 2.f;
	}
}

void CPlayerWeapon_Bow::PlayAnim_UtilityAttack_Loop()
{
	m_pModel->Change_AnimIndex(BOW_ANIM_NORMAL_LOOP, 0.1f);
	m_bOnceAnim = true;
}

void CPlayerWeapon_Bow::PlayAnim_UtilityAttack_Shot()
{
	if (true == m_bOnceAnim)
	{
		m_pModel->Change_AnimIndex_ReturnTo_Must(BOW_ANIM_UTILITY_SHOT, BOW_ANIM_NORMAL_LOOP, 0.f, false);
		m_bOnceAnim = false;
	}
}

void CPlayerWeapon_Bow::Set_AnimSpeed(_float fAnimSpeed)
{
	m_fAnimSpeed = fAnimSpeed;
}

_fVector CPlayerWeapon_Bow::Get_BonePos(const char * pBoneName)
{
	_Matrix BoneMatrix = m_pModel->Get_BoneMatrix(pBoneName);
	_Matrix TransformMatrix = BoneMatrix * m_pTransformCom->Get_WorldMatrix();
	_Vector vPos, vRot, vScale;
	XMMatrixDecompose(&vScale, &vRot, &vPos, TransformMatrix);

	return vPos;
}

void CPlayerWeapon_Bow::Update_AttachCamPos()
{
}

_int CPlayerWeapon_Bow::Update_Structure(_double fDeltaTime)
{
	m_pTransformCom->Turn_CCW(XMVectorSet(0.f, 1.f, 0.f, 0.f), fDeltaTime);

	return _int();
}

_int CPlayerWeapon_Bow::Update_Equip(_double fDeltaTime)
{
	return _int();
}

_int CPlayerWeapon_Bow::Update_NoEquip(_double fDeltaTime)
{
	return _int();
}

_int CPlayerWeapon_Bow::LateUpdate_Structure(_double fDeltaTime)
{
	return _int();
}

_int CPlayerWeapon_Bow::LateUpdate_Equip(_double fDeltaTime)
{
	Update_AttachMatrix();

	return _int();
}

_int CPlayerWeapon_Bow::LateUpdate_NoEquip(_double fDeltaTime)
{
	return _int();
}

void CPlayerWeapon_Bow::Update_AttachMatrix()
{
	m_fAttachedMatrix = m_pTransformCom->Get_WorldMatrix()  * m_tPlayerWeaponDesc.eAttachedDesc.Caculate_AttachedBoneMatrix();
}

void CPlayerWeapon_Bow::Set_Pivot()
{
	m_tPlayerWeaponDesc.eAttachedDesc.Set_DefaultBonePivot(_float3(0.012f, 0.012f, 0.012f), _float3(90, -30, -185), _float3(0.0, 0.0, 0.0));
	m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, _float3(-93.33f, -54.7f, -9.27f));
	//m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, _float3(-111.6f, -66.4f, -7.2f));
}

void CPlayerWeapon_Bow::Change_Pivot(EBowPivot ePitvot)
{
	switch (ePitvot)
	{
	case EBowPivot::BOW_PIVOT_NORMAL:
		m_tPlayerWeaponDesc.eAttachedDesc.Set_DefaultBonePivot(_float3(1, 1, 1), _float3(-97, -120, -60), _float3(-0.661f, -0.04f, -1.133f));
		break;
	case EBowPivot::BOW_PIVOT_THROW:
		m_tPlayerWeaponDesc.eAttachedDesc.Set_DefaultBonePivot(_float3(1.f, 1.f, 1.f), _float3(90, 0, 0), _float3(-0.661f, -0.04f, -1.133f));
		break;
	case EBowPivot::BOW_PIVOT_TAKEDOWN:
		m_tPlayerWeaponDesc.eAttachedDesc.Set_DefaultBonePivot(_float3(1.f, 1.f, 1.f), _float3(80, 130, 0), _float3(-0.661f, -0.04f, -1.133f));
		break;
	}
}

HRESULT CPlayerWeapon_Bow::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_PlayerWeapon_Bow), TAG_COM(Com_Model), (CComponent**)&m_pModel));

	CTransform::TRANSFORMDESC tDesc = {};

	tDesc.fMovePerSec = 5;
	tDesc.fRotationPerSec = XMConvertToRadians(60);
	tDesc.fScalingPerSec = 1;
	tDesc.vPivot = _float3(0, 0, 0);

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));


	return S_OK;
}

HRESULT CPlayerWeapon_Bow::SetUp_EtcInfo()
{
	m_pModel->Change_AnimIndex(0);
	return S_OK;
}

CPlayerWeapon_Bow * CPlayerWeapon_Bow::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CPlayerWeapon_Bow*	pInstance = new CPlayerWeapon_Bow(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CPlayerWeapon_Bow");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CPlayerWeapon_Bow::Clone(void * pArg)
{
	CPlayerWeapon_Bow*	pInstance = new CPlayerWeapon_Bow(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CPlayerWeapon_Bow");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPlayerWeapon_Bow::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModel);
}
