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
	FAILED_CHECK(SetUp_Collider());
	FAILED_CHECK(SetUp_EtcInfo());
	Set_IsOcllusion(true);

	FAILED_CHECK(Ready_ParticleDesc());
	
	return S_OK;
}

_int CPlayerWeapon_Spear::Update(_double fDeltaTime)
{
	m_vecTextureParticleDesc[0].TotalParticleTime = 2.f;

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

	m_pModel->Change_AnimIndex(m_iCurAnim, 0.f);
	FAILED_CHECK(m_pModel->Update_AnimationClip(fDeltaTime, true));


	if (true == m_bActiveCollision)
	{
		Update_Colliders();
		FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_PlayerWeapon, this, m_pCollider));
	}

	if (true == m_bActiveCollision_1)
	{
		Update_Colliders_1();
		FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_PlayerWeapon, this, m_pCollider_Range));
	}


	FAILED_CHECK(m_pDissolveCom->Update_Dissolving(fDeltaTime));
	Update_ParticleTransform();

	return _int();
}

_int CPlayerWeapon_Spear::LateUpdate(_double fDeltaTimer)
{
	if (false == m_pDissolveCom->Get_IsFadeIn() && 1.f <= m_pDissolveCom->Get_DissolvingRate())
		return 0;
	
	if (__super::LateUpdate(fDeltaTimer) < 0) return -1;  

	if (true == m_bThrowing)
	{
		Throw(fDeltaTimer);
	}


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
	//FAILED_CHECK(m_pRendererCom->Add_ShadowGroup(CRenderer::SHADOW_ANIMMODEL_ATTACHED, this, m_pTransformCom, m_pShaderCom, m_pModel, nullptr, m_pDissolveCom));
	//FAILED_CHECK(m_pRendererCom->Add_ShadowGroup(CRenderer::SHADOW_ANIMMODEL_ATTACHED, this, m_pTransformCom, m_pShaderCom, m_pModel, &_float4x4(mat)));
	//FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pCollider_Range));
	//FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pCollider));

	m_fAttachedMatrix = m_fAttachedMatrix.TransposeXMatrix();
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

	FAILED_CHECK(m_pDissolveCom->Render(m_iPassNum));

	return _int();
}

_int CPlayerWeapon_Spear::LateRender()
{


	return _int();
}

void CPlayerWeapon_Spear::Active_Trail(_bool bActivate)
{
	__super::Active_Trail(bActivate);
	
	static _bool bChecker = false;



	if (true == m_bActiveTrail)
	{
		_Matrix mat = m_pTransformCom->Get_WorldMatrix()  * m_tPlayerWeaponDesc.eAttachedDesc.Caculate_AttachedBoneMatrix();

		mat.r[0] = XMVector3Normalize(mat.r[0]);
		mat.r[1] = XMVector3Normalize(mat.r[1]);
		mat.r[2] = XMVector3Normalize(mat.r[2]);
		m_pSwordTrail->Set_TrailTurnOn(true, mat.r[3] + mat.r[2] * 0.5f, mat.r[3] - (mat.r[2] * 1.f + mat.r[0] * 0.1f + mat.r[1] * 0.1f));

		if (!bChecker)
		{
			GetSingle(CUtilityMgr)->Create_TextureInstance(m_eNowSceneNum, m_vecTextureParticleDesc[0]);
			bChecker = true;
		}
	}
	else
	{
		m_pSwordTrail->Set_TrailTurnOn(false, _float3(0.f, 0.f, 0.f), _float3(0.f, 0.f, 0.f));
		bChecker = false;
	}
}

void CPlayerWeapon_Spear::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
	if (CollisionTypeID::CollisionType_Monster == eConflictedObjCollisionType)
	{
		_Vector vDamageDir = XMVector3Normalize(pConflictedCollider->Get_ColliderPosition(iConflictedObjColliderIndex).XMVector() - m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS));
		pConflictedObj->Take_Damage(this, 1.f, vDamageDir, m_bOnKnockbackCol, m_fKnockbackColPower);
		pConflictedCollider->Set_Conflicted(0.5f);

		_int iSelectSoundFileIndex = rand() % 2;
		_tchar pSoundFile[MAXLEN] = TEXT("");
		swprintf_s(pSoundFile, TEXT("Jino_Raji_Trishul_Impact_%d.wav"), iSelectSoundFileIndex);
		g_pGameInstance->Play3D_Sound(pSoundFile, m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 1.f);
	}
}

_fVector CPlayerWeapon_Spear::Get_BonePos(const char * pBoneName)
{
	_Matrix BoneMatrix = m_pModel->Get_BoneMatrix(pBoneName);
	_Matrix TransformMatrix = BoneMatrix * m_pTransformCom->Get_WorldMatrix();
	_Vector vPos, vRot, vScale;
	XMMatrixDecompose(&vScale, &vRot, &vPos, TransformMatrix);

	return vPos;
}

_fMatrix CPlayerWeapon_Spear::Get_BoneMatrix(const char * pBoneName)
{
	_Matrix BoneMatrix = m_pModel->Get_BoneMatrix(pBoneName);
	_Matrix TransformMatrix = BoneMatrix * m_pTransformCom->Get_WorldMatrix();

	return TransformMatrix;
}

void CPlayerWeapon_Spear::Update_AttachCamPos()
{
}

_bool CPlayerWeapon_Spear::AbleToChangeWeapon()
{
	return (false == m_pDissolveCom->Get_IsDissolving());
}

void CPlayerWeapon_Spear::Dissolve_In(_double fTargetTime)
{
	m_pDissolveCom->Set_DissolveOn(true, fTargetTime);
}

void CPlayerWeapon_Spear::Dissolve_Out(_double fTargetTime)
{
	m_pDissolveCom->Set_DissolveOn(false, fTargetTime);
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

void CPlayerWeapon_Spear::Update_Trail(_fMatrix * pMat, _double fDeltaTime)
{
	m_pSwordTrail->Update_SwordTrail((*pMat).r[3] + (*pMat).r[2] * 0.5f, (*pMat).r[3] - ((*pMat).r[2] * 1.f + (*pMat).r[0] * 0.1f + (*pMat).r[1] * 0.1f), fDeltaTime);
}

void CPlayerWeapon_Spear::Update_Colliders()
{
	if (true == m_bThrowing)
	{
		_Matrix mat = m_pTransformCom->Get_WorldMatrix();
		_Vector vPos = mat.r[3];

		m_pCollider->Update_Transform(0, mat);
		mat.r[3] = vPos + mat.r[2] * 0.9f;
		m_pCollider->Update_Transform(1, mat);
		mat.r[3] = vPos + mat.r[2] * 0.6f;
		m_pCollider->Update_Transform(2, mat);
		mat.r[3] = vPos + mat.r[2] * 0.3f;
		m_pCollider->Update_Transform(3, mat);
	}
	else
	{
		_Matrix mat = m_pTransformCom->Get_WorldMatrix()  * m_tPlayerWeaponDesc.eAttachedDesc.Caculate_AttachedBoneMatrix();

		mat.r[0] = XMVector3Normalize(mat.r[0]);
		mat.r[1] = XMVector3Normalize(mat.r[1]);
		mat.r[2] = XMVector3Normalize(mat.r[2]);
		_Vector vPos = mat.r[3];

		m_pCollider->Update_Transform(0, mat);

		mat.r[3] = vPos - (mat.r[2] * 0.8f + mat.r[0] * 0.1f + mat.r[1] * 0.1f);
		m_pCollider->Update_Transform(1, mat);

		mat.r[3] = vPos - (mat.r[2] * 0.5f + mat.r[0] * 0.05f + mat.r[1] * 0.05f);
		m_pCollider->Update_Transform(2, mat);

		mat.r[3] = vPos - (mat.r[2] * 0.2f + mat.r[0] * 0.f + mat.r[1] * 0.f);
		m_pCollider->Update_Transform(3, mat);
	}
}

void CPlayerWeapon_Spear::Update_Colliders_1()
{
	CTransform* pPlayerTransform = static_cast<CTransform*>(m_tPlayerWeaponDesc.pOwner->Get_Component(TAG_COM(Com_Transform)));
	_Vector vPlayerPos = pPlayerTransform->Get_MatrixState(CTransform::TransformState::STATE_POS);
	_Vector vPlayerLook = XMVector3Normalize(pPlayerTransform->Get_MatrixState(CTransform::TransformState::STATE_LOOK));

	_Matrix mat = XMMatrixIdentity();
	mat.r[3] = vPlayerPos + (vPlayerLook * 0.8f);

	m_pCollider_Range->Update_Transform(0, mat);
	m_pCollider_Range->Update_Transform(1, mat);
}

void CPlayerWeapon_Spear::Update_ParticleTransform()
{
	m_pTextureParticleTransform->Set_MatrixState(CTransform::STATE_POS, m_pCollider->Get_ColliderPosition(2));
	m_pTextureParticleTransform->LookAt(m_pCollider->Get_ColliderPosition(1).XMVector());
}

void CPlayerWeapon_Spear::Change_Pivot(ESpearPivot ePitvot)
{
	switch (ePitvot)
	{
	case ESpearPivot::SPEAR_PIVOT_NORMAL:
		m_tPlayerWeaponDesc.eAttachedDesc.Set_DefaultBonePivot(_float3(1, 1, 1), _float3(-97, -120, -60), _float3(-0.661f, -0.04f, -1.133f));
		m_bActiveCollision = false;
		break;
	case ESpearPivot::SPEAR_PIVOT_THROW:
		m_tPlayerWeaponDesc.eAttachedDesc.Set_DefaultBonePivot(_float3(1.f, 1.f, 1.f), _float3(90, 0, 0), _float3(-0.661f, -0.04f, -1.133f));
		break;
	case ESpearPivot::SPEAR_PIVOT_TAKEDOWN:
		m_tPlayerWeaponDesc.eAttachedDesc.Set_DefaultBonePivot(_float3(1.f, 1.f, 1.f), _float3(80, 130, 0), _float3(-0.661f, -0.04f, -1.133f));
		break;
	}
}

void CPlayerWeapon_Spear::Throw_Start(_fVector vThrowDir)
{
	CTransform* pPlayerTransform = static_cast<CTransform*>(m_tPlayerWeaponDesc.pOwner->Get_Component(TAG_COM(Com_Transform)));
	_Vector vPlayerPos = pPlayerTransform->Get_MatrixState(CTransform::TransformState::STATE_POS);
	g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Trishul_Throw.wav"), vPlayerPos, CHANNELID::CHANNEL_PLAYER, 1.f);
	m_pTransformCom->Set_Matrix(m_tPlayerWeaponDesc.eAttachedDesc.Caculate_AttachedBoneMatrix());
	m_pTransformCom->LookDir(vThrowDir);
	m_bThrowing = true;
	m_bThrowDir = vThrowDir;
	m_iPassNum = 13;
	m_iCurAnim = 8;



	m_pMeshParticleTransform->Set_IsOwnerDead(false);
	GetSingle(CUtilityMgr)->Create_TextureInstance(m_eNowSceneNum, m_vecTextureParticleDesc[3]);
	
}

void CPlayerWeapon_Spear::Throw_End()
{

	m_pMeshParticleTransform->Set_IsOwnerDead(true);

	m_pTransformCom->Set_Matrix(XMMatrixIdentity());
	m_bThrowing = false;
	m_bThrowDir = { 0.f, 0.f, 0.f };
	m_iPassNum = 9;
	m_iCurAnim = 0;
}

void CPlayerWeapon_Spear::Throw(_double fDeltaTimer)
{


	m_pMeshParticleTransform->Set_MatrixState(CTransform::STATE_POS, m_pCollider->Get_ColliderPosition(2));
	m_pMeshParticleTransform->LookAt(m_pCollider->Get_ColliderPosition(1).XMVector());

	m_bActiveCollision = true;
	m_pTransformCom->MovetoDir(m_bThrowDir.XMVector(), fDeltaTimer);
}

HRESULT CPlayerWeapon_Spear::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_PlayerWeapon_Spear), TAG_COM(Com_Model), (CComponent**)&m_pModel));
	
	CTransform::TRANSFORMDESC tDesc = {};

	tDesc.fMovePerSec = 30.f;
	tDesc.fRotationPerSec = XMConvertToRadians(360); 
	tDesc.fScalingPerSec = 1;  
	tDesc.vPivot = _float3(0, 0, 0);

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));

	CSwordTrail::TRAILDESC tSwordDesc;
	tSwordDesc.iPassIndex = 0;
	tSwordDesc.vColor = _float4(1.f, 0.2f, 0.1f, 1.f);
	tSwordDesc.iTextureIndex = 1;
	tSwordDesc.NoiseSpeed = 0;

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_SwordTrail), TAG_COM(Com_SwordTrail), (CComponent**)&m_pSwordTrail, &tSwordDesc));

	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_PlayerWeapon_Spear), TAG_COM(Com_SubModel), (CComponent**)&m_pModel_Skill));
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_SubTransform), (CComponent**)&m_pTransformCom_Skill, &tDesc));

	CDissolve::DISSOLVEDESC	tDissolveDesc;
	tDissolveDesc.eDissolveModelType = CDissolve::DISSOLVE_ANIM_ATTACHED;
	tDissolveDesc.pModel = m_pModel;
	tDissolveDesc.pShader = m_pShaderCom;
	tDissolveDesc.RampTextureIndex = 1;
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Dissolve), TAG_COM(Com_Dissolve), (CComponent**)&m_pDissolveCom, &tDissolveDesc));
	return S_OK;
}

HRESULT CPlayerWeapon_Spear::SetUp_EtcInfo()
{
	m_iPassNum = 9;
	m_iCurAnim = 0;
	return S_OK;
}

HRESULT CPlayerWeapon_Spear::SetUp_Collider()
{
	// Weapon Collider
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_Collider), (CComponent**)&m_pCollider));
	COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(3.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.5f, 0.5f, 0.5f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	m_pCollider->Set_ParantBuffer();

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.5f, 0.5f, 0.5f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	m_pCollider->Set_ParantBuffer();

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.5f, 0.5f, 0.5f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	m_pCollider->Set_ParantBuffer();
	//


	// Attack Range Collider
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_Collider_1), (CComponent**)&m_pCollider_Range));
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(6.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pCollider_Range->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(5.f, 5.f, 5.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pCollider_Range->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	m_pCollider_Range->Set_ParantBuffer();
	//

	return S_OK;
}

HRESULT CPlayerWeapon_Spear::Ready_ParticleDesc()
{
	m_pTextureParticleTransform = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
	NULL_CHECK_RETURN(m_pTextureParticleTransform, E_FAIL);
	m_pMeshParticleTransform = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
	NULL_CHECK_RETURN(m_pMeshParticleTransform, E_FAIL);

	CUtilityMgr* pUtil = GetSingle(CUtilityMgr);

	//	0
	m_vecTextureParticleDesc.push_back(pUtil->Get_TextureParticleDesc(L"SpearNormalAttack"));
	m_vecTextureParticleDesc[0].TotalParticleTime = 2.f;
	m_vecTextureParticleDesc[0].FollowingTarget = m_pTextureParticleTransform;
	m_vecTextureParticleDesc[0].iFollowingDir = FollowingDir_Look;


	//	1
	m_vecTextureParticleDesc.push_back(pUtil->Get_TextureParticleDesc(L"FireSmallParticle"));
	//	2
	m_vecTextureParticleDesc.push_back(pUtil->Get_TextureParticleDesc(L"FireSlamCircle"));
	//	3
	m_vecTextureParticleDesc.push_back(pUtil->Get_TextureParticleDesc(L"Spear_ThrowAttack"));
	m_vecTextureParticleDesc[3].FollowingTarget = m_pMeshParticleTransform;
	m_vecTextureParticleDesc[3].iFollowingDir = FollowingDir_Look;

	return S_OK;
}

CPlayerWeapon_Spear * CPlayerWeapon_Spear::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CPlayerWeapon_Spear*	pInstance = NEW CPlayerWeapon_Spear(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CPlayerWeapon_Spear");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CPlayerWeapon_Spear::Clone(void * pArg)
{
	CPlayerWeapon_Spear*	pInstance = NEW CPlayerWeapon_Spear(*this);

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
	Safe_Release(m_pSwordTrail);


	Safe_Release(m_pTextureParticleTransform);
	Safe_Release(m_pMeshParticleTransform);


	Safe_Release(m_pModel_Skill);
	Safe_Release(m_pTransformCom_Skill);
	Safe_Release(m_pCollider);
	Safe_Release(m_pCollider_Range);
	Safe_Release(m_pCollider_Range);
	Safe_Release(m_pCollider_Range);
	Safe_Release(m_pCollider_Range);
	Safe_Release(m_pDissolveCom);
}