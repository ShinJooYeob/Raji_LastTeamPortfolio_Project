#include "stdafx.h"
#include "..\public\PlayerWeapon_Bow.h"
#include "..\public\PartilceCreateMgr.h"



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

	FAILED_CHECK(Ready_ParticleDesc());

	_bool							m_bEffect_Bow = false;


	return S_OK;
}

_int CPlayerWeapon_Bow::Update(_double fDeltaTime)
{
	if (false == m_pDissolveCom->Get_IsFadeIn() && 1.f <= m_pDissolveCom->Get_DissolvingRate())
		return 0;

	if (__super::Update(fDeltaTime) < 0) return -1;

	//if (g_pGameInstance->Get_DIKeyState(DIK_Z) & DIS_Down)
	//{
	//	m_pDissolveCom->Set_DissolveOn(false, 5.5f);

	//}
	//if (g_pGameInstance->Get_DIKeyState(DIK_X) & DIS_Down)
	//{
	//	m_pDissolveCom->Set_DissolveOn(true, 1.5f);
	//}
	



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

	FAILED_CHECK(m_pModel->Update_AnimationClip(fDeltaTime * m_fAnimSpeed, true));
	FAILED_CHECK(m_pDissolveCom->Update_Dissolving(fDeltaTime));

	FAILED_CHECK(Update_Particle(fDeltaTime));

	return _int();
}

_int CPlayerWeapon_Bow::LateUpdate(_double fDeltaTimer)
{
	if (false == m_pDissolveCom->Get_IsFadeIn() && 1.f <= m_pDissolveCom->Get_DissolvingRate())
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




	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
	FAILED_CHECK(m_pRendererCom->Add_ShadowGroup(CRenderer::SHADOW_ANIMMODEL_ATTACHED, this, m_pTransformCom, m_pShaderCom, m_pModel, &m_fAttachedMatrix, m_pDissolveCom));
	m_fAttachedMatrix = m_fAttachedMatrix.TransposeXMatrix();

	
	

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

	FAILED_CHECK(m_pDissolveCom->Render(9));
	return _int();
}

_int CPlayerWeapon_Bow::LateRender()
{
	return _int();
}

_bool CPlayerWeapon_Bow::AbleToChangeWeapon()
{
	return (false == m_pDissolveCom->Get_IsDissolving());
}

void CPlayerWeapon_Bow::Dissolve_In(_double fTargetTime)
{
	m_pDissolveCom->Set_DissolveOn(true, fTargetTime);
}

void CPlayerWeapon_Bow::Dissolve_Out(_double fTargetTime)
{
	m_pDissolveCom->Set_DissolveOn(false, fTargetTime);
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

HRESULT CPlayerWeapon_Bow::Update_Particle(_double fDeltaTime)
{

	FAILED_CHECK(__super::Update_Particle(fDeltaTime));

	_Matrix mat = m_pTransformCom->Get_WorldMatrix()  * m_tPlayerWeaponDesc.eAttachedDesc.Caculate_AttachedBoneMatrix();

//	EX)
//	mat.r[3] = vPos - (mat.r[2] * 0.2f + mat.r[0] * 0.03f + mat.r[1] * 0.03f);
//	m_vecTextureParticleDesc[0].vFixedPosition = mat.r[3];
	
	mat.r[0] = XMVector3Normalize(mat.r[0]);
	mat.r[1] = XMVector3Normalize(mat.r[1]);
	mat.r[2] = XMVector3Normalize(mat.r[2]);
	_Vector vPos = mat.r[3];

	m_pTextureParticleTransform->Set_Matrix(mat);

	_Vector vPos2 = vPos + (mat.r[2] * 0.5f + mat.r[0] * 0.1f);
	mat.r[3] = vPos2;
	m_pTextureParticleTransform_BowUp->Set_MatrixState(CTransform::STATE_POS, vPos2);
	
	_Vector vPos3 = vPos - (mat.r[2] * 0.4f - mat.r[0] * 0.1f);
	mat.r[3] = vPos3;
	m_pTextureParticleTransform_BowBack->Set_Matrix(mat);

	

	return S_OK;

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


	CDissolve::DISSOLVEDESC	tDissolveDesc;
	tDissolveDesc.eDissolveModelType = CDissolve::DISSOLVE_ANIM_ATTACHED;
	tDissolveDesc.pModel = m_pModel;
	tDissolveDesc.pShader = m_pShaderCom;
	tDissolveDesc.RampTextureIndex = 1;
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Dissolve), TAG_COM(Com_Dissolve), (CComponent**)&m_pDissolveCom, &tDissolveDesc));

	return S_OK;
}

HRESULT CPlayerWeapon_Bow::SetUp_EtcInfo()
{
	m_pModel->Change_AnimIndex(0);
	return S_OK;
}

HRESULT CPlayerWeapon_Bow::Ready_ParticleDesc()
{
	FAILED_CHECK(__super::Ready_ParticleDesc());

	m_pTextureParticleTransform = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
	m_pTextureParticleTransform_BowUp = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
	m_pTextureParticleTransform_BowBack = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
//	m_pTextureParticleTransform_BowFront= (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
	NULL_CHECK_RETURN(m_pTextureParticleTransform, E_FAIL);
	NULL_CHECK_RETURN(m_pTextureParticleTransform_BowUp, E_FAIL);
	NULL_CHECK_RETURN(m_pTextureParticleTransform_BowBack, E_FAIL);



	// 0
	auto instanceDesc = GETPARTICLE->Get_TypeDesc_TextureInstance(CPartilceCreateMgr::TEXTURE_EFFECTJ_Bow_Default);
	instanceDesc.TotalParticleTime = 99999.f;
	instanceDesc.FollowingTarget = m_pTextureParticleTransform_BowUp;
//	GETPARTICLE->Create_Texture_Effect_Desc(instanceDesc, m_eNowSceneNum);
	m_vecTextureParticleDesc.push_back(instanceDesc);


	// 1
	instanceDesc.FollowingTarget = m_pTextureParticleTransform_BowBack;
//	GETPARTICLE->Create_Texture_Effect_Desc(instanceDesc, m_eNowSceneNum);
	m_vecTextureParticleDesc.push_back(instanceDesc);

	// 9999여도 죽는다. 
	m_pTextureParticleTransform_BowUp->Set_IsOwnerDead(true);
	m_pTextureParticleTransform_BowBack->Set_IsOwnerDead(true);

	// 2
	instanceDesc = GETPARTICLE->Get_TypeDesc_TextureInstance(CPartilceCreateMgr::TEXTURE_EFFECTJ_Bow_Charze_Circle);
	instanceDesc.FollowingTarget = m_pTextureParticleTransform;
	m_vecTextureParticleDesc.push_back(instanceDesc);



	// 3
	instanceDesc = GETPARTICLE->Get_TypeDesc_TextureInstance(CPartilceCreateMgr::TEXTURE_EFFECTJ_Bow_Charze_Suck);
	instanceDesc.FollowingTarget = m_pTextureParticleTransform;
	m_vecTextureParticleDesc.push_back(instanceDesc);

	// 4
	instanceDesc = GETPARTICLE->Get_TypeDesc_TextureInstance(CPartilceCreateMgr::TEXTURE_EFFECTJ_Bow_Charze_Dash);
	instanceDesc.FollowingTarget = nullptr;
	m_vecTextureParticleDesc.push_back(instanceDesc);

	// 5
	//instanceDesc = GETPARTICLE->Get_TypeDesc_TextureInstance(CPartilceCreateMgr::TEXTURE_EFFECTJ_Bow_Charze_Long);
	//instanceDesc.FollowingTarget = m_pTextureParticleTransform_BowFront;
	//m_vecTextureParticleDesc.push_back(instanceDesc);




	return S_OK;
}

CPlayerWeapon_Bow * CPlayerWeapon_Bow::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CPlayerWeapon_Bow*	pInstance = NEW CPlayerWeapon_Bow(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CPlayerWeapon_Bow");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CPlayerWeapon_Bow::Clone(void * pArg)
{
	CPlayerWeapon_Bow*	pInstance = NEW CPlayerWeapon_Bow(*this);

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
	Safe_Release(m_pDissolveCom);

	TRANSDEAD(m_pTextureParticleTransform);
	TRANSDEAD(m_pTextureParticleTransform_BowUp);
	TRANSDEAD(m_pTextureParticleTransform_BowBack);


	Safe_Release(m_pTextureParticleTransform);
	Safe_Release(m_pTextureParticleTransform_BowUp);
	Safe_Release(m_pTextureParticleTransform_BowBack);
	
}
