#include "stdafx.h"
#include "..\public\PlayerWeapon_Arrow.h"
#include "Timer.h"
#include "ShellingArrow.h"
#include "PartilceCreateMgr.h"
#include "Camera_Main.h"

CPlayerWeapon_Arrow::CPlayerWeapon_Arrow(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CPlayerWeapon(pDevice, pDeviceContext)
{
}

CPlayerWeapon_Arrow::CPlayerWeapon_Arrow(const CPlayerWeapon_Arrow & rhs)
	: CPlayerWeapon(rhs)
{
}

HRESULT CPlayerWeapon_Arrow::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CPlayerWeapon_Arrow::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components());
	FAILED_CHECK(SetUp_Collider());
	FAILED_CHECK(SetUp_EtcInfo());

	FAILED_CHECK(Ready_ParticleDesc());


	m_bEffect_Head = false;



	return S_OK;
}

_int CPlayerWeapon_Arrow::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0) return -1;

	switch (m_eCurState)
	{
	case Arrow_State_NormalReady:
		UpdateState_NormalReady(fDeltaTime);
		break;
	case Arrow_State_NormalShot:
		UpdateState_NormalShot(fDeltaTime);
		break;
	case Arrow_State_UtilityReady:
		UpdateState_UtilityReady(fDeltaTime);
		break;
	case Arrow_State_UtilityShot:
		UpdateState_UtilityShot(fDeltaTime);
		break;
	case Arrow_State_PowerShot_Combo_0:
		UpdateState_PowerShot_Conbo_0(fDeltaTime);
		break;
	case Arrow_State_PowerShot_Combo_1:
		UpdateState_PowerShot_Conbo_1(fDeltaTime);
		break;
	case Arrow_State_PowerShot_Combo_2:
		UpdateState_PowerShot_Conbo_2(fDeltaTime);
		break;
	case Arrow_State_UltimateReady_Pre_Ready:
		UpdateState_Ultimate_Pre_Ready(fDeltaTime);
		break;
	case Arrow_State_UltimateShot_Pre_Shot:
		UpdateState_Ultimate_Pre_Shot(fDeltaTime);
		break;
	case Arrow_State_UltimateReady_Post_Ready:
		UpdateState_Ultimate_Post_Ready(fDeltaTime);
		break;
	case Arrow_State_UltimateShot_Post_Shot:
		UpdateState_Ultimate_Post_Shot(fDeltaTime);
		break;
	}

	if (
		(m_eCurState != Arrow_State_NormalReady) ||
		(m_eCurState != Arrow_State_UtilityReady) ||
		(m_eCurState != Arrow_State_UltimateReady_Pre_Ready) ||
		(m_eCurState != Arrow_State_UltimateReady_Post_Ready)
		)
	{
		if (!m_bEffect_Head)
		{
			Set_Play_MeshParticle(CPartilceCreateMgr::MESHEFFECT_ARROW_HEAD, m_pTransformCom, &m_bEffect_Head);
		}


	}

	if (true == m_bFired)
	{
		m_fCurTime_Destroy += (_float)g_fDeltaTime;
		if (m_fCurTime_Destroy >= m_fMaxTime_Destroy)
		{
			Set_IsDead();
		}
	}

	Update_Colliders();
	FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_PlayerWeapon, this, m_pCollider));
	Update_Particle(fDeltaTime);

	return _int();
}

_int CPlayerWeapon_Arrow::LateUpdate(_double fDeltaTimer)
{
	if (__super::LateUpdate(fDeltaTimer) < 0) return -1;

	m_fAttachedMatrix = m_pTransformCom->Get_WorldMatrix()  * m_tPlayerWeaponDesc.eAttachedDesc.Caculate_AttachedBoneMatrix();

	_Matrix mat = m_fAttachedMatrix.XMatrix();
	mat.r[0] = XMVector3Normalize(mat.r[0]);
	mat.r[1] = XMVector3Normalize(mat.r[1]);
	mat.r[2] = XMVector3Normalize(mat.r[2]);

	if (true == m_bActiveTrail)
	{
		Update_Trail(&mat, fDeltaTimer);
	}

	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
	FAILED_CHECK(m_pRendererCom->Add_TrailGroup(CRenderer::TRAIL_SWORD, m_pSwordTrail));
	FAILED_CHECK(m_pRendererCom->Add_TrailGroup(CRenderer::TRAIL_SWORD, m_pSwordTrail2));
	FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pCollider));

	m_fAttachedMatrix = m_fAttachedMatrix.TransposeXMatrix();
	return _int();
}

_int CPlayerWeapon_Arrow::Render()
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
			FAILED_CHECK(m_pModel->Bind_OnShader(m_pShaderCom, i, j, MODLETEXTYPE(j)));

		FAILED_CHECK(m_pModel->Render(m_pShaderCom, m_iPassNum, i));
	}

	return _int();
}

_int CPlayerWeapon_Arrow::LateRender()
{
	return _int();
}

void CPlayerWeapon_Arrow::Set_State(EArrowState eNewState, _float fSpeed)
{
	m_eCurState = eNewState;

	switch (m_eCurState)
	{
	case EArrowState::Arrow_State_NormalReady:
		m_iPassNum = 8;
		m_tPlayerWeaponDesc.eAttachedDesc.Set_DefaultBonePivot(_float3(1, 1, 1), _float3(-98, -80, 115), _float3(0.0, 0.0, 0.0));
		m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, _float3(-1.087f, 0.36f, 0.667f));
		m_pSwordTrail->Set_Color(_float4(0.896f, 0.96f, 0.96f, 1.f));
		m_pSwordTrail2->Set_Color(_float4(0.896f, 0.96f, 0.96f, 1.f));
		Active_Trail(true);
		break;
	case EArrowState::Arrow_State_NormalShot:
		Active_Trail(false);
		m_iPassNum = 2;
		m_fAttachedMatrix = m_pTransformCom->Get_WorldMatrix() * m_tPlayerWeaponDesc.eAttachedDesc.Caculate_AttachedBoneMatrix();
		m_pTransformCom->Set_Matrix(m_fAttachedMatrix);
		m_pSwordTrail->Set_Color(_float4(0.896f, 0.96f, 0.96f, 1.f));
		m_pSwordTrail2->Set_Color(_float4(0.896f, 0.96f, 0.96f, 1.f));
		m_fStartPos_y = XMVectorGetY(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS));
		m_pTransformCom->Set_MoveSpeed(fSpeed);

		m_bFired = true;
		break;
	case EArrowState::Arrow_State_UtilityReady:
		m_iPassNum = 8;
		m_tPlayerWeaponDesc.eAttachedDesc.Set_DefaultBonePivot(_float3(1.1f, 1.1f, 1.1f), _float3(48, 150.f, -95), _float3(0.0, 0.0, 0.0));
		m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, _float3(0.473f, 0.3f, 1.057f));
		break;
	case EArrowState::Arrow_State_UtilityShot:
		m_iPassNum = 2;
		m_fAttachedMatrix = m_pTransformCom->Get_WorldMatrix() * m_tPlayerWeaponDesc.eAttachedDesc.Caculate_AttachedBoneMatrix();
		m_pTransformCom->Set_Matrix(m_fAttachedMatrix);
		m_pTransformCom->LookDir(XMVectorSet(0.f, 1.f, 0.f, 0.f));
		Active_Trail(true);
		m_pTransformCom->Set_MoveSpeed(fSpeed);

		m_bFired = true;
		break;
	}

}

void CPlayerWeapon_Arrow::Set_State_PowerShot_Combo_0(_fVector vShotDir, _uint iArrowIndex)
{
	m_eCurState = Arrow_State_PowerShot_Combo_0;

	m_tPlayerWeaponDesc.eAttachedDesc.Set_DefaultBonePivot(_float3(1, 1, 1), _float3(-100, -180, 70), _float3(0.0, 0.0, 0.0));
	m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, _float3(0.283f, -0.85f, 0.497f));

	m_fAttachedMatrix = m_pTransformCom->Get_WorldMatrix() * m_tPlayerWeaponDesc.eAttachedDesc.Caculate_AttachedBoneMatrix();
	m_pTransformCom->Set_Matrix(m_fAttachedMatrix);

	m_iPassNum = 2;

	_Vector vFinalShotDir = XMVectorSetY(vShotDir, 0);
	vFinalShotDir = XMVectorSetY(vFinalShotDir, 0);
	m_pTransformCom->LookDir(vFinalShotDir);
	if (1 == iArrowIndex)
	{
		m_pTransformCom->Turn_Direct(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_UP), XMConvertToRadians(10.f));
	}
	else if (2 == iArrowIndex)
	{
		m_pTransformCom->Turn_Direct(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_UP), XMConvertToRadians(-10.f));
	}

	m_bFired = true;

	// Setting Trail Color
	m_pSwordTrail->Set_Color(_float4(0.896f, 0.96f, 0.96f, 1.f));
	m_pSwordTrail2->Set_Color(_float4(0.896f, 0.96f, 0.96f, 1.f));
	Active_Trail(true);
	//
}

void CPlayerWeapon_Arrow::Set_State_PowerShot_Combo_1(_fVector vShotDir)
{
	m_tPlayerWeaponDesc.eAttachedDesc.Set_DefaultBonePivot(_float3(1, 1, 1), _float3(-98, -80, 115), _float3(0.0, 0.0, 0.0));
	m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, _float3(-1.087f, 0.36f, 0.667f));

	m_fAttachedMatrix = m_pTransformCom->Get_WorldMatrix() * m_tPlayerWeaponDesc.eAttachedDesc.Caculate_AttachedBoneMatrix();
	m_pTransformCom->Set_Matrix(m_fAttachedMatrix);

	m_eCurState = Arrow_State_PowerShot_Combo_1;
	m_iPassNum = 2;

	_Vector vFinalShotDir = XMVectorSetY(vShotDir, 0);
	m_pTransformCom->LookDir(vFinalShotDir);
	m_bFired = true;

	// Setting Trail Color
	m_pSwordTrail->Set_Color(_float4(0.896f, 0.96f, 0.96f, 1.f));
	m_pSwordTrail2->Set_Color(_float4(0.896f, 0.96f, 0.96f, 1.f));
	Active_Trail(true);
	//
}

void CPlayerWeapon_Arrow::Set_State_PowerShot_Combo_2(_fVector vShotDir, _float fSpeed)
{
	m_tPlayerWeaponDesc.eAttachedDesc.Set_DefaultBonePivot(_float3(1.1f, 1.1f, 1.1f), _float3(-110, -80, 110), _float3(0, 0, 0));
	m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, _float3(-0.957f, 0.11f, 0.667f));

	m_fAttachedMatrix = m_pTransformCom->Get_WorldMatrix() * m_tPlayerWeaponDesc.eAttachedDesc.Caculate_AttachedBoneMatrix();
	m_pTransformCom->Set_Matrix(m_fAttachedMatrix);

	m_eCurState = Arrow_State_PowerShot_Combo_2;
	m_iPassNum = 2;

	m_fStartPos_y = XMVectorGetY(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS));

	_Vector vFinalShotDir = XMVectorSetY(vShotDir, 0);
	m_pTransformCom->LookDir(vFinalShotDir);

	m_pTransformCom->Set_MoveSpeed(fSpeed);
	m_bFired = true;

	// Setting Trail Color
	m_pSwordTrail->Set_Color(_float4(0.896f, 0.96f, 0.96f, 1.f));
	m_pSwordTrail2->Set_Color(_float4(0.896f, 0.96f, 0.96f, 1.f));
	Active_Trail(true);
	//
}

void CPlayerWeapon_Arrow::Set_State_Ultimate_Pre_Ready()
{
	m_iPassNum = 8;
	m_tPlayerWeaponDesc.eAttachedDesc.Set_DefaultBonePivot(_float3(1.1f, 1.1f, 1.1f), _float3(48.f, 150.f, -95.f), _float3(0.0, 0.0, 0.0));
	m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, _float3(0.473f, 0.3f, 1.057f));

	m_eCurState = Arrow_State_UltimateReady_Pre_Ready;
}

void CPlayerWeapon_Arrow::Set_State_Ultimate_Pre_Shot()
{
	m_iPassNum = 2;
	m_fAttachedMatrix = m_pTransformCom->Get_WorldMatrix() * m_tPlayerWeaponDesc.eAttachedDesc.Caculate_AttachedBoneMatrix();
	m_pTransformCom->Set_Matrix(m_fAttachedMatrix);

	m_pTransformCom->LookDir(XMVectorSet(0.f, 1.f, 0.f, 0.f));
	m_eCurState = Arrow_State_UltimateShot_Pre_Shot;
	Active_Trail(true);
}

void CPlayerWeapon_Arrow::Set_State_Ultimate_Post_Ready()
{
	m_iPassNum = 8;
	m_tPlayerWeaponDesc.eAttachedDesc.Set_DefaultBonePivot(_float3(1.1f, 1.1f, 1.1f), _float3(38, -50, -32), _float3(0, 0, 0));
	m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, _float3(-1.11f, -0.43f, -0.02f));

	m_eCurState = Arrow_State_UltimateReady_Post_Ready;
}

void CPlayerWeapon_Arrow::Set_State_Ultimate_Post_Shot()
{
	m_iPassNum = 2;
	m_fAttachedMatrix = m_pTransformCom->Get_WorldMatrix() * m_tPlayerWeaponDesc.eAttachedDesc.Caculate_AttachedBoneMatrix();
	m_pTransformCom->Set_Matrix(m_fAttachedMatrix);

	m_pTransformCom->LookDir(XMVectorSet(0.f, 1.f, 0.f, 0.f));
	m_eCurState = Arrow_State_UltimateShot_Post_Shot;
	Active_Trail(true);
}

void CPlayerWeapon_Arrow::Set_TargetPos(_float3 fTargetPos)
{
	m_fTargetPos = fTargetPos;
}

_int CPlayerWeapon_Arrow::UpdateState_NormalReady(_double fDeltaTime)
{

	return _int();
}

_int CPlayerWeapon_Arrow::UpdateState_NormalShot(_double fDeltaTime)
{
	if (30.f <= m_pTransformCom->Get_MoveSpeed())
	{
		m_pTransformCom->Move_Forward(fDeltaTime);
		m_pTimer_Destroy->Get_DeltaTime();
		m_bFired = true;
		FAILED_CHECK_NONERETURN(Set_Play_Particle(1));

	}
	else
	{
		_float fPos_y = m_fStartPos_y + (0.f * m_fTimeAcc - 9.8f * m_fTimeAcc * m_fTimeAcc * 0.5f);
		m_fTimeAcc += 0.015f;

		_Vector vMyPos_Before = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
		m_pTransformCom->Move_Forward(fDeltaTime);
		_Vector vMyPos_After = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);

		vMyPos_After = XMVectorSetY(vMyPos_After, fPos_y);
		m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, vMyPos_After);

		m_pTransformCom->LookDir(XMVector3Normalize(vMyPos_After - vMyPos_Before));
	}


	return _int();
}

_int CPlayerWeapon_Arrow::UpdateState_UtilityReady(_double fDeltaTime)
{

	return _int();
}

_int CPlayerWeapon_Arrow::UpdateState_UtilityShot(_double fDeltaTime)
{
	//m_pTransformCom->Move_Forward(fDeltaTime);
	m_pTransformCom->LookDir(XMVectorSet(0.f, 1.f, 0.f, 0.f));
	m_pTransformCom->MovetoDir(XMVectorSet(0.f, 1.f, 0.f, 0.f), fDeltaTime);

	m_fCurTime_Destroy += (_float)fDeltaTime;
	if (1.f <= m_fCurTime_Destroy)
	{
		CShellingArrow::SHELLINGARROWDESC tShellingArrowDesc;
		tShellingArrowDesc.fStartPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
		tShellingArrowDesc.fTargetPos = m_fTargetPos;
		FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_PlayerSkill), TAG_OP(Prototype_PlayerSkill_ShellingArrow), &tShellingArrowDesc));

		Set_IsDead();
	}

	m_bFired = true;

	return _int();
}

_int CPlayerWeapon_Arrow::UpdateState_UltimateReady(_double fDeltaTime)
{

	return _int();
}

_int CPlayerWeapon_Arrow::UpdateState_UltimateShot(_double fDeltaTime)
{
	m_pTimer_Destroy->Get_DeltaTime();
	m_bFired = true;
	return _int();
}

_int CPlayerWeapon_Arrow::UpdateState_PowerShot_Conbo_0(_double fDeltaTime)
{
	m_pTransformCom->Move_Forward(fDeltaTime);

	return _int();
}

_int CPlayerWeapon_Arrow::UpdateState_PowerShot_Conbo_1(_double fDeltaTime)
{
	m_pTransformCom->Move_Forward(fDeltaTime);

	return _int();
}

_int CPlayerWeapon_Arrow::UpdateState_PowerShot_Conbo_2(_double fDeltaTime)
{
	_float fPos_y = m_fStartPos_y + (0.f * m_fTimeAcc - 9.8f * m_fTimeAcc * m_fTimeAcc * 0.5f);
	m_fTimeAcc += 0.02f;

	_Vector vMyPos_Before = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
	m_pTransformCom->Move_Forward(fDeltaTime);
	_Vector vMyPos_After = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);

	vMyPos_After = XMVectorSetY(vMyPos_After, fPos_y);
	m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, vMyPos_After);

	m_pTransformCom->LookDir(XMVector3Normalize(vMyPos_After - vMyPos_Before));

	return _int();
}

_int CPlayerWeapon_Arrow::UpdateState_Ultimate_Pre_Ready(_double fDeltaTime)
{

	return _int();
}

_int CPlayerWeapon_Arrow::UpdateState_Ultimate_Pre_Shot(_double fDeltaTime)
{
	m_fTimeAcc += (_float)g_fDeltaTime;
	m_pTransformCom->Move_Forward(fDeltaTime);

	if (m_fTimeAcc > 1.f)
	{
		// Spawn Core Sphere
		Set_IsDead();
	}

	return _int();
}

_int CPlayerWeapon_Arrow::UpdateState_Ultimate_Post_Ready(_double fDeltaTime)
{

	return _int();
}

_int CPlayerWeapon_Arrow::UpdateState_Ultimate_Post_Shot(_double fDeltaTime)
{
	m_pTransformCom->Move_Forward(fDeltaTime);



	/** Check Collision to CoreSphere
	if ()
	{

	}
	*/
	return _int();
}



HRESULT CPlayerWeapon_Arrow::Ready_ParticleDesc()
{

	FAILED_CHECK(__super::Ready_ParticleDesc());

	// Transform 
	m_pTextureParticleTransform = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
	NULL_CHECK_BREAK(m_pTextureParticleTransform);

	m_pTextureParticleTransform_Hand = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
	NULL_CHECK_BREAK(m_pTextureParticleTransform_Hand);

	// Particle

	// 0
	auto instanceDesc = GETPARTICLE->Get_TypeDesc_TextureInstance(CPartilceCreateMgr::TEXTURE_EFFECTJ_Bow_Charze_ArrowHead);
	instanceDesc.FollowingTarget = nullptr;
	GETPARTICLE->Create_Texture_Effect_Desc(instanceDesc, m_eNowSceneNum);
	m_vecTextureParticleDesc.push_back(instanceDesc);

	// 1
	instanceDesc = GETPARTICLE->Get_TypeDesc_TextureInstance(CPartilceCreateMgr::TEXTURE_EFFECTJ_Bow_Bow_ArrowTrail);
	instanceDesc.FollowingTarget = m_pTextureParticleTransform;
	m_vecTextureParticleDesc.push_back(instanceDesc);

	// 2
	instanceDesc = GETPARTICLE->Get_TypeDesc_TextureInstance(CPartilceCreateMgr::TEXTURE_EFFECTJ_Bow_ArrowHit);
	instanceDesc.FollowingTarget = m_pTextureParticleTransform;
	m_vecTextureParticleDesc.push_back(instanceDesc);

	// 3
	instanceDesc = GETPARTICLE->Get_TypeDesc_TextureInstance(CPartilceCreateMgr::TEXTURE_EFFECTJ_Bow_Charze_Long);
	instanceDesc.FollowingTarget = m_pTextureParticleTransform_Hand;
	m_vecTextureParticleDesc.push_back(instanceDesc);




	return S_OK;
}

HRESULT CPlayerWeapon_Arrow::Update_Particle(_double fDeltaTime)
{
	FAILED_CHECK(__super::Update_Particle(fDeltaTime));

	//	_Matrix mat = m_pTransformCom->Get_WorldMatrix()  * m_tPlayerWeaponDesc.eAttachedDesc.Caculate_AttachedBoneMatrix();

		//	EX)
		//	mat.r[3] = vPos - (mat.r[2] * 0.2f + mat.r[0] * 0.03f + mat.r[1] * 0.03f);
		//	m_vecTextureParticleDesc[0].vFixedPosition = mat.r[3];

	_Matrix mat = m_pTransformCom->Get_WorldMatrix();
	m_pTextureParticleTransform->Set_Matrix(mat);

	_Matrix mat2 = m_pTransformCom->Get_WorldMatrix()  * m_tPlayerWeaponDesc.eAttachedDesc.Caculate_AttachedBoneMatrix();
	mat2.r[0] = XMVector3Normalize(mat2.r[0]);
	mat2.r[1] = XMVector3Normalize(mat2.r[1]);
	mat2.r[2] = XMVector3Normalize(mat2.r[2]);

	mat2.r[3] += mat2.r[0] * 0 + mat2.r[1] * 0 + mat2.r[2] * 0.8f;

	m_pTextureParticleTransform_Hand->Set_Matrix(mat2);


	return S_OK;
}

_fVector CPlayerWeapon_Arrow::Get_BonePos(const char * pBoneName)
{
	return _fVector();
}

void CPlayerWeapon_Arrow::Update_AttachCamPos()
{
}

void CPlayerWeapon_Arrow::Update_AttachMatrix()
{
}

void CPlayerWeapon_Arrow::Update_Trail(_fMatrix * pMat, _double fDeltaTime)
{
	_Matrix mat = m_pTransformCom->Get_WorldMatrix() * m_tPlayerWeaponDesc.eAttachedDesc.Caculate_AttachedBoneMatrix();
	switch (m_eCurState)
	{
	case EArrowState::Arrow_State_NormalReady:
		m_pSwordTrail->Update_SwordTrail((*pMat).r[3] + (*pMat).r[2] * 0.62f, (*pMat).r[3] + (*pMat).r[2] * 0.75f, fDeltaTime);
		FAILED_CHECK_NONERETURN(Set_Play_Particle(0, (*pMat).r[3] + (*pMat).r[2] * 0.75f));
		break;
	case EArrowState::Arrow_State_NormalShot:
	case EArrowState::Arrow_State_PowerShot_Combo_0:
	case EArrowState::Arrow_State_PowerShot_Combo_1:
	case EArrowState::Arrow_State_PowerShot_Combo_2:
		m_pSwordTrail->Set_TextureIndex(3);
		m_pSwordTrail->Update_SwordTrail(
			m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS) + (m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK) * 0.5f) + (XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_UP)) * 0.015f),
			m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS) + (m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK) * 0.2f) - (XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_UP)) * 0.03f),
			fDeltaTime
		);

		m_pSwordTrail2->Set_TextureIndex(3);
		m_pSwordTrail2->Update_SwordTrail(
			m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS) + (m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK) * 0.5f) + (XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_RIGHT)) * 0.015f),
			m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS) + (m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK) * 0.2f) - (XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_RIGHT)) * 0.03f),
			fDeltaTime
		);
		break;

	case EArrowState::Arrow_State_UtilityShot:
	case EArrowState::Arrow_State_UltimateShot_Pre_Shot:
	case EArrowState::Arrow_State_UltimateShot_Post_Shot:
		m_pSwordTrail->Set_TextureIndex(3);
		m_pSwordTrail->Update_SwordTrail(
			m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS) + (m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK) * 0.3f) + (m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_UP) * 0.015f),
			m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS) + (m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK) * 0.1f) - (m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_UP) * 0.015f),
			fDeltaTime
		);

		m_pSwordTrail2->Set_TextureIndex(3);
		m_pSwordTrail2->Update_SwordTrail(
			m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS) + (m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK) * 0.3f) + (m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_UP) * 0.015f),
			m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS) + (m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK) * 0.1f) - (m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_UP) * 0.015f),
			fDeltaTime
		);
		break;
	}

}

void CPlayerWeapon_Arrow::Update_Colliders()
{
	_Matrix Matrix = m_pTransformCom->Get_WorldMatrix();

	Matrix.r[3] += (m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK) * 0.7f);
	m_pCollider->Update_Transform(0, Matrix);
	m_pCollider->Update_Transform(1, Matrix);
}

void CPlayerWeapon_Arrow::Active_Trail(_bool bActivate)
{
	__super::Active_Trail(bActivate);

	if (true == m_bActiveTrail)
	{
		_Matrix mat = m_pTransformCom->Get_WorldMatrix() * m_tPlayerWeaponDesc.eAttachedDesc.Caculate_AttachedBoneMatrix();
		mat.r[0] = XMVector3Normalize(mat.r[0]);
		mat.r[1] = XMVector3Normalize(mat.r[1]);
		mat.r[2] = XMVector3Normalize(mat.r[2]);




		switch (m_eCurState)
		{
		case EArrowState::Arrow_State_NormalReady:
			m_pSwordTrail->Set_TrailTurnOn(true, mat.r[3] + mat.r[2] * 0.62f, mat.r[3] + mat.r[2] * 0.75f);

			break;
		case EArrowState::Arrow_State_NormalShot:
			m_pSwordTrail->Set_TrailTurnOn(
				true,
				m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS) + (m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK) * 0.5f) + (m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_UP) * 0.015f),
				m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS) + (m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK) * 0.2f) - (m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_UP) * 0.015f)
			);

			m_pSwordTrail2->Set_TrailTurnOn(
				true,
				m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS) + (m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK) * 0.5f) + (m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_RIGHT) * 0.015f),
				m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS) + (m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK) * 0.2f) - (m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_RIGHT) * 0.015f)
			);

			break;
		case EArrowState::Arrow_State_PowerShot_Combo_0:
		case EArrowState::Arrow_State_PowerShot_Combo_1:
		case EArrowState::Arrow_State_PowerShot_Combo_2:
			m_pSwordTrail->Set_TrailTurnOn(
				true,
				m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS) + (m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK) * 1.f) + (m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_RIGHT) * 0.015f),
				m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS) + (m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK) * 0.8f) - (m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_RIGHT) * 0.015f)
			);

			m_pSwordTrail2->Set_TrailTurnOn(
				true,
				m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS) + (m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK) * 1.f) + (m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_UP) * 0.015f),
				m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS) + (m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK) * 0.8f) - (m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_UP) * 0.015f)
			);

			break;
		case EArrowState::Arrow_State_UtilityShot:
		case EArrowState::Arrow_State_UltimateShot_Pre_Shot:
		case EArrowState::Arrow_State_UltimateShot_Post_Shot:
			m_pSwordTrail->Set_TrailTurnOn(
				true,
				m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS) + (m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK) * 0.6f) + (m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_UP) * 0.015f),
				m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS) + (m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK) * 0.1f) - (m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_UP) * 0.015f)
			);

			m_pSwordTrail->Set_TrailTurnOn(
				true,
				m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS) + (m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK) * 0.6f) + (m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_RIGHT) * 0.015f),
				m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS) + (m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK) * 0.1f) - (m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_RIGHT) * 0.015f)
			);

			break;
		}
	}
	else
	{
		m_pSwordTrail->Set_TrailTurnOn(false, _float3(0.f, 0.f, 0.f), _float3(0.f, 0.f, 0.f));
	}
}

void CPlayerWeapon_Arrow::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
	if (CollisionTypeID::CollisionType_Monster == eConflictedObjCollisionType)
	{
		if (true == m_bFired)
		{
			GetSingle(CUtilityMgr)->PlusBowSkillPersent(1.f);

			_Vector vDamageDir = XMVector3Normalize(pConflictedCollider->Get_ColliderPosition(iConflictedObjColliderIndex).XMVector() - m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS));
			if (0.f > pConflictedObj->Take_Damage(this, 1.f, vDamageDir, m_bOnKnockbackCol, m_fKnockbackColPower))
			{
				GetSingle(CUtilityMgr)->SlowMotionStart(2.f, 0.02f);
			}
			pConflictedCollider->Set_Conflicted(0.2f);

			_int iSelectSoundFileIndex = rand() % 3;
			_tchar pSoundFile[MAXLEN] = TEXT("");
			swprintf_s(pSoundFile, TEXT("Jino_Raji_Arrow_Impact_%d.wav"), iSelectSoundFileIndex);
			g_pGameInstance->Play3D_Sound(pSoundFile, m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 1.f);

			FAILED_CHECK_NONERETURN(Set_Play_Particle(2));

			GetSingle(CUtilityMgr)->Get_MainCamera()->Start_CameraShaking_Fov(55.f, 3.f, 0.2f, true);
		}
	}
}

void CPlayerWeapon_Arrow::LookAtDir(_fVector fDir)
{
	m_pTransformCom->LookDir(fDir);
}

HRESULT CPlayerWeapon_Arrow::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VNAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_PlayerWeapon_Arrow), TAG_COM(Com_Model), (CComponent**)&m_pModel));

	CTransform::TRANSFORMDESC tDesc = {};
	tDesc.fMovePerSec = 30;
	tDesc.fRotationPerSec = XMConvertToRadians(60);
	tDesc.fScalingPerSec = 1;
	tDesc.vPivot = _float3(0, 0, 0);
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));

	CSwordTrail::TRAILDESC tSwordDesc;
	tSwordDesc.iPassIndex = 0;
	tSwordDesc.vColor = _float4(0.896f, 0.96f, 0.96f, 1.f);
	tSwordDesc.iTextureIndex = 1;
	tSwordDesc.NoiseSpeed = 0;
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_SwordTrail), TAG_COM(Com_SwordTrail), (CComponent**)&m_pSwordTrail, &tSwordDesc));

	tSwordDesc.iPassIndex = 0;
	tSwordDesc.vColor = _float4(0.896f, 0.96f, 0.96f, 1.f);
	tSwordDesc.iTextureIndex = 1;
	tSwordDesc.NoiseSpeed = 0;
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_SwordTrail), TAG_COM(Com_SubSwordTrail), (CComponent**)&m_pSwordTrail2, &tSwordDesc));
	return S_OK;
}

HRESULT CPlayerWeapon_Arrow::SetUp_EtcInfo()
{
	m_pTimer_Destroy = CTimer::Create();
	m_fMaxTime_Destroy = 2.f;
	m_fCurTime_Destroy = 0.f;

	return S_OK;
}

HRESULT CPlayerWeapon_Arrow::SetUp_Collider()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_Collider), (CComponent**)&m_pCollider));

	COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.2f, 0.2f, 0.2f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	m_pCollider->Set_ParantBuffer();

	return S_OK;
}

CPlayerWeapon_Arrow * CPlayerWeapon_Arrow::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CPlayerWeapon_Arrow*	pInstance = NEW CPlayerWeapon_Arrow(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CPlayerWeapon_Arrow");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CPlayerWeapon_Arrow::Clone(void * pArg)
{
	CPlayerWeapon_Arrow*	pInstance = NEW CPlayerWeapon_Arrow(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CPlayerWeapon_Arrow");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPlayerWeapon_Arrow::Free()
{
	__super::Free();

	Safe_Release(m_pTimer_Destroy);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModel);
	Safe_Release(m_pSwordTrail);
	Safe_Release(m_pSwordTrail2);
	Safe_Release(m_pCollider);

	TRANSDEAD(m_pTextureParticleTransform);
	TRANSDEAD(m_pTextureParticleTransform_Hand);

	Safe_Release(m_pTextureParticleTransform);
	Safe_Release(m_pTextureParticleTransform_Hand);

}
