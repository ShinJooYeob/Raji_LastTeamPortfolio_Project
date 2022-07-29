#include "stdafx.h"
#include "..\public\PlayerWeapon_Shield.h"
#include "Camera_Main.h"

CPlayerWeapon_Shield::CPlayerWeapon_Shield(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CPlayerWeapon(pDevice, pDeviceContext)
{
}

CPlayerWeapon_Shield::CPlayerWeapon_Shield(const CPlayerWeapon_Shield & rhs)
	: CPlayerWeapon(rhs)
{
}

HRESULT CPlayerWeapon_Shield::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CPlayerWeapon_Shield::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components());
	FAILED_CHECK(SetUp_Collider());
	FAILED_CHECK(SetUp_EtcInfo());


	return S_OK;
}

_int CPlayerWeapon_Shield::Update(_double fDeltaTime)
{
	if (false == m_pDissolveCom->Get_IsFadeIn() && 1.f <= m_pDissolveCom->Get_DissolvingRate())
	{
		return 0;
	}

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

	if (true == m_bUltimateMode)
	{
		Activate_Ultimate(fDeltaTime);
	}
	else if (false == m_bSmashMode && false == m_bUltimateMode && false == m_bThrowMode)
	{
		m_tPlayerWeaponDesc.eAttachedDesc.Set_DefaultBonePivot(_float3(1.1f, 1.1f, 1.1f), _float3(100, 20, -15), _float3(0.0, 0.0, 0.0));
		m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, _float3(0.76f, -0.846f, -0.030f));
	}

	m_pModel->Change_AnimIndex(m_iCurAnim, 0.f);
	m_pMotionTrail->Update_MotionTrail(fDeltaTime);
	FAILED_CHECK(m_pModel->Update_AnimationClip(fDeltaTime, true));

	if (true == m_bActiveCollision)
	{
		Update_Colliders();
		FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_PlayerWeapon, this, m_pCollider));
	}

	if (true == m_bActiveCollision_1)
	{
		Update_Collider_Smash();
		FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_PlayerWeapon, this, m_pCollider_Smash));
		m_bActiveCollision_1 = false;
	}

	FAILED_CHECK(m_pDissolveCom->Update_Dissolving(fDeltaTime));

	return _int();
}

_int CPlayerWeapon_Shield::LateUpdate(_double fDeltaTimer)
{
	if (false == m_pDissolveCom->Get_IsFadeIn() && 1.f <= m_pDissolveCom->Get_DissolvingRate())
	{
		return 0;
	}

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
	FAILED_CHECK(m_pRendererCom->Add_TrailGroup(CRenderer::TRAIL_MOTION, m_pMotionTrail));
	FAILED_CHECK(m_pRendererCom->Add_ShadowGroup(CRenderer::SHADOW_ANIMMODEL_ATTACHED, this, m_pTransformCom, m_pShaderCom, m_pModel, &m_fAttachedMatrix));
	FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pCollider));
	FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pCollider_Smash));

	m_fAttachedMatrix = m_fAttachedMatrix.TransposeXMatrix();

	return _int();
}

_int CPlayerWeapon_Shield::Render()
{
	if (__super::Render() < 0)		return -1;

	NULL_CHECK_RETURN(m_pModel, E_FAIL);

	CGameInstance* pInstance = GetSingle(CGameInstance);

	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_AttechMatrix", &m_fAttachedMatrix, sizeof(_float4x4)));
	FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));

	FAILED_CHECK(m_pDissolveCom->Render(m_iPassNum));

	return _int();
}

_int CPlayerWeapon_Shield::LateRender()
{
	return _int();
}

void CPlayerWeapon_Shield::Active_Trail(_bool bActivate)
{
	__super::Active_Trail(bActivate);

}

void CPlayerWeapon_Shield::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
	if (CollisionTypeID::CollisionType_Monster == eConflictedObjCollisionType)
	{
		_Vector vDamageDir = XMVector3Normalize(pConflictedCollider->Get_ColliderPosition(iConflictedObjColliderIndex).XMVector() - m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS));
		if (0.f > pConflictedObj->Take_Damage(this, 1.f, vDamageDir, m_bOnKnockbackCol, m_fKnockbackColPower))
		{
			GetSingle(CUtilityMgr)->SlowMotionStart(2.f, 0.02f);
		}
		pConflictedCollider->Set_Conflicted(0.5f);

		g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Shield_Impact.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 0.7f);
		GetSingle(CUtilityMgr)->Get_MainCamera()->Start_CameraShaking_Fov(55.f, 3.f, 0.2f, true);
	}
}

_bool CPlayerWeapon_Shield::AbleToChangeWeapon()
{
	return (false == m_pDissolveCom->Get_IsDissolving());
}

void CPlayerWeapon_Shield::Dissolve_In(_double fTargetTime)
{
	m_pDissolveCom->Set_DissolveOn(true, fTargetTime);
}

void CPlayerWeapon_Shield::Dissolve_Out(_double fTargetTime)
{
	m_pDissolveCom->Set_DissolveOn(false, fTargetTime);
}

void CPlayerWeapon_Shield::Start_UltimateMode(_fVector fStartPos, _float fUltimateTargetHeight)
{
	m_fUltimateTargetHeight = fUltimateTargetHeight;
	
	_fVector vBonePos = m_tPlayerWeaponDesc.eAttachedDesc.Get_AttachedBoneWorldPosition();
	_fVector vStartPos = XMVectorSet(XMVectorGetX(fStartPos), XMVectorGetY(vBonePos), XMVectorGetZ(fStartPos), 0.f);
	m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, vStartPos);
	m_pTransformCom->Rotation_CCW(XMVectorSet(0.f, 1.f, 0.f, 0.f), 0.f);
	m_pTransformCom->Scaled_All(_float3(1.2f, 1.2f, 1.2f));
	
	m_iPassNum = 13;
	m_iCurAnim = 1;
	m_bUltimateMode = true;

	m_eUltimateState = SHIELDULTIMATE_MOV;
}

void CPlayerWeapon_Shield::End_UltimateMode()
{
	m_pTransformCom->Set_Matrix(XMMatrixIdentity());
	m_iPassNum = 9;
	m_iCurAnim = 0;
	m_bUltimateMode = false;
}

void CPlayerWeapon_Shield::Start_SmashMode(_fVector vPutOnPos)
{
	m_pTransformCom->Set_Matrix(XMMatrixIdentity());
	m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, vPutOnPos);
	m_pTransformCom->Scaled_All(_float3(1.2f, 1.2f, 1.2f));
	m_iPassNum = 13;
	m_iCurAnim = 1;
	m_bSmashMode = true;
}

void CPlayerWeapon_Shield::End_SmashMode()
{
	m_pTransformCom->Set_Matrix(XMMatrixIdentity());
	m_iPassNum = 9;
	m_iCurAnim = 0;
	m_bSmashMode = false;
	Change_Pivot(EShieldPivot::SHIELD_PIVOT_NORMAL);
}

void CPlayerWeapon_Shield::Start_ThrowMode(_fVector vStartPos, _float fThrowDist)
{
	m_fTargetDist = fThrowDist;

	m_pTransformCom->Set_Matrix(XMMatrixIdentity());
	m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, vStartPos);
	m_pTransformCom->Scaled_All(_float3(1.2f, 1.2f, 1.2f));
	 
	m_iPassNum = 13;
	m_iCurAnim = 1;
	m_bThrowMode = true;

	m_bActiveCollision = true;

	GetSingle(CUtilityMgr)->Get_MainCamera()->Start_CameraShaking_Fov(56.f, 1.f, 0.2f, true);
}

void CPlayerWeapon_Shield::End_ThrowMode()
{
	m_pTransformCom->Set_Matrix(XMMatrixIdentity());

	m_iPassNum = 9;
	m_iCurAnim = 0;
	m_bThrowMode = false;

	m_bActiveCollision = false;
}

void CPlayerWeapon_Shield::Activate_Ultimate(_double fDeltaTime)
{

	switch (m_eUltimateState)
	{
	case EShieldUltimateState::SHIELDULTIMATE_MOV:
		m_pTransformCom->MovetoDir(XMVectorSet(0.f, 1.f, 0.f, 0.f), fDeltaTime);
		_fVector vMyPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
		if (XMVectorGetY(vMyPos) >= m_fUltimateTargetHeight)
		{
			m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, XMVectorSetY(vMyPos, m_fUltimateTargetHeight));
			m_eUltimateState = EShieldUltimateState::SHIELDULTIMATE_ATTACK;
		}
		break;
	case EShieldUltimateState::SHIELDULTIMATE_ATTACK:
		m_pTransformCom->Turn_CCW(XMVectorSet(0.f, 1.f, 0.f, 0.f), fDeltaTime);
		break;
	case EShieldUltimateState::SHIELDULTIMATE_END:
		break;
	}
	
}

void CPlayerWeapon_Shield::Update_ThrowPos(_fVector vStartPos, _fVector vThrowDir, _float fAnimRate)
{
	if (fAnimRate >= 0.7f)
	{
		End_ThrowMode();
		m_pMotionTrail->Update_MotionTrail(100.f);
	}
	else
	{
		static double Timer = 0;
		Timer -= g_fDeltaTime;
		if (Timer < 0)
		{
			Timer = 0.005f;
			m_pMotionTrail->Add_MotionBuffer(m_pTransformCom->Get_WorldFloat4x4(), _float4(1.f, 1.f, 1.f, 1),g_fDeltaTime * 10.f);
		}

		_float vUpdatedDist = g_pGameInstance->Easing_Return(TYPE_ExpoOut, TYPE_ExpoOut, 0, m_fTargetDist, fAnimRate, 1.f);
		m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, (vStartPos + (vThrowDir * vUpdatedDist)));
		m_pTransformCom->LookDir(vThrowDir);
		m_pTransformCom->Rotation_CCW(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK), XMConvertToRadians(90.f));
	}
}

_fVector CPlayerWeapon_Shield::Get_BonePos(const char * pBoneName)
{
	_Matrix BoneMatrix = m_pModel->Get_BoneMatrix(pBoneName);
	_Matrix TransformMatrix = BoneMatrix * m_pTransformCom->Get_WorldMatrix();
	_Vector vPos, vRot, vScale;
	XMMatrixDecompose(&vScale, &vRot, &vPos, TransformMatrix);

	return vPos;
}

void CPlayerWeapon_Shield::Update_AttachCamPos()
{
}

_int CPlayerWeapon_Shield::Update_Structure(_double fDeltaTime)
{
	m_pTransformCom->Turn_CCW(XMVectorSet(0.f, 1.f, 0.f, 0.f), fDeltaTime);

	return _int();
}

_int CPlayerWeapon_Shield::Update_Equip(_double fDeltaTime)
{
	return _int();
}

_int CPlayerWeapon_Shield::Update_NoEquip(_double fDeltaTime)
{
	return _int();
}

_int CPlayerWeapon_Shield::LateUpdate_Structure(_double fDeltaTime)
{
	return _int();
}

_int CPlayerWeapon_Shield::LateUpdate_Equip(_double fDeltaTime)
{
	Update_AttachMatrix();
	return _int();
}

_int CPlayerWeapon_Shield::LateUpdate_NoEquip(_double fDeltaTime)
{

	return _int();
}

void CPlayerWeapon_Shield::Update_AttachMatrix()
{
	m_fAttachedMatrix = m_pTransformCom->Get_WorldMatrix()  * m_tPlayerWeaponDesc.eAttachedDesc.Caculate_AttachedBoneMatrix();
}

void CPlayerWeapon_Shield::Update_Colliders()
{
	m_pCollider->Update_Transform(0, m_pTransformCom->Get_WorldMatrix());
	m_pCollider->Update_Transform(1, m_pTransformCom->Get_WorldMatrix());
}

void CPlayerWeapon_Shield::Update_Collider_Smash()
{
	m_pCollider_Smash->Update_Transform(0, m_pTransformCom->Get_WorldMatrix());
	m_pCollider_Smash->Update_Transform(1, m_pTransformCom->Get_WorldMatrix());

}

void CPlayerWeapon_Shield::Change_Pivot(EShieldPivot ePitvot)
{
	switch (ePitvot)
	{
	case EShieldPivot::SHIELD_PIVOT_NORMAL:
		m_tPlayerWeaponDesc.eAttachedDesc.Set_DefaultBonePivot(_float3(1, 1, 1), _float3(-97, -120, -60), _float3(-0.661f, -0.04f, -1.133f));
		break;
	case EShieldPivot::SHIELD_PIVOT_THROW:
		m_tPlayerWeaponDesc.eAttachedDesc.Set_DefaultBonePivot(_float3(1.f, 1.f, 1.f), _float3(90, 0, 0), _float3(-0.661f, -0.04f, -1.133f));
		break;
	case EShieldPivot::SHIELD_PIVOT_TAKEDOWN:
		m_tPlayerWeaponDesc.eAttachedDesc.Set_DefaultBonePivot(_float3(1.f, 1.f, 1.f), _float3(80, 130, 0), _float3(-0.661f, -0.04f, -1.133f));
		break;
	}
}

HRESULT CPlayerWeapon_Shield::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_PlayerWeapon_Shield), TAG_COM(Com_Model), (CComponent**)&m_pModel));

	CTransform::TRANSFORMDESC tDesc = {};

	tDesc.fMovePerSec = 5;
	tDesc.fRotationPerSec = XMConvertToRadians(60);
	tDesc.fScalingPerSec = 1;
	tDesc.vPivot = _float3(0, 0, 0);

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));

	m_iPassNum = 9;
	m_iCurAnim = 0;


	CMotionTrail::MOTIONTRAILDESC tMotionDesc;
	tMotionDesc.iNumTrailCount = 6;
	tMotionDesc.pModel = m_pModel;
	tMotionDesc.pShader = m_pShaderCom;
	tMotionDesc.iPassIndex = 5;
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_MotionTrail), TAG_COM(Com_MotionTrail), (CComponent**)&m_pMotionTrail, &tMotionDesc));


	CDissolve::DISSOLVEDESC	tDissolveDesc;
	tDissolveDesc.eDissolveModelType = CDissolve::DISSOLVE_ANIM_ATTACHED;
	tDissolveDesc.pModel = m_pModel;
	tDissolveDesc.pShader = m_pShaderCom;
	tDissolveDesc.RampTextureIndex = 1;
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Dissolve), TAG_COM(Com_Dissolve), (CComponent**)&m_pDissolveCom, &tDissolveDesc));

	return S_OK;
}

HRESULT CPlayerWeapon_Shield::SetUp_EtcInfo()
{
	return S_OK;
}

HRESULT CPlayerWeapon_Shield::SetUp_Collider()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_Collider), (CComponent**)&m_pCollider));
	COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(2.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.8f, 0.8f, 0.8f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	m_pCollider->Set_ParantBuffer();



	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_Collider_1), (CComponent**)&m_pCollider_Smash));
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(6.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pCollider_Smash->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(5.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pCollider_Smash->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	m_pCollider_Smash->Set_ParantBuffer();
	return S_OK;
}

CPlayerWeapon_Shield * CPlayerWeapon_Shield::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CPlayerWeapon_Shield*	pInstance = NEW CPlayerWeapon_Shield(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CPlayerWeapon_Shield");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CPlayerWeapon_Shield::Clone(void * pArg)
{
	CPlayerWeapon_Shield*	pInstance = NEW CPlayerWeapon_Shield(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CPlayerWeapon_Shield");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPlayerWeapon_Shield::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModel);
	Safe_Release(m_pMotionTrail);
	Safe_Release(m_pCollider);
	Safe_Release(m_pCollider_Smash);
	Safe_Release(m_pDissolveCom);
}
