#include "stdafx.h"
#include "..\public\Monster_Mahinasura_Minion.h"

#include "HpUI.h"

CMonster_Mahinasura_Minion::CMonster_Mahinasura_Minion(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CMonster(pDevice, pDeviceContext)
{
}

CMonster_Mahinasura_Minion::CMonster_Mahinasura_Minion(const CMonster_Mahinasura_Minion & rhs)
	: CMonster(rhs)
{
}

HRESULT CMonster_Mahinasura_Minion::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));


	return S_OK;
}

HRESULT CMonster_Mahinasura_Minion::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components());

	if (pArg != nullptr)
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, *((_float3*)pArg));

	//m_pTransformCom->Scaled_All(_float3(100, 100, 100));
	m_pTransformCom->Rotation_CW(XMVectorSet(0, 1, 0, 0), XMConvertToRadians(170));


	SetUp_Info();







	/////////////////test
	
	m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, _float3(216.357f, 29.2f, 185.583f));

	m_pNavigationCom->FindCellIndex(m_pTransformCom->Get_MatrixState(CTransform::STATE_POS));
	/////////////////

	m_pTransformCom->Scaled_All(_float3(1.5f,1.5f,1.5f));
	return S_OK;
}

_int CMonster_Mahinasura_Minion::Update(_double dDeltaTime)
{

	if (__super::Update(dDeltaTime) < 0)return -1;

	// #TEXT ANIFUNC
	//마지막 인자의 bBlockAnimUntilReturnChange에는 true로 시작해서 정상작동이 된다면 false가 된다.
	//m_pModel->Change_AnimIndex();
	//m_pModel->Change_AnimIndex_ReturnTo(); //어떤 애니메이션을 돌리고 특정 애니메이션으로 보냄
	//m_pModel->Change_AnimIndex_ReturnTo_Must(); //중간에 애니메이션을 캔슬하고 다른 애니메이션을 동작시킴
	//m_pModel->Change_AnimIndex_UntilTo(); //1~5까지 돌린다고 명령어를 입력하면 1~5까지 돌아감
	//m_pModel->Change_AnimIndex_UntilNReturn();//1~5까지 돌리고 난 이후 특정 애니메이션으로 Return 시킬 경우
	//m_pModel->Change_AnimIndex_UntilNReturn_Must(); //1~5까지 돌리고 난 이후 특정 애니메이션으로 Return 시킬 수 있지만 다른 애니메이션을 동작시킬 수 있음



	PlayAnim(dDeltaTime);


	m_bIsOnScreen = g_pGameInstance->IsNeedToRender(m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS), m_fFrustumRadius);
	FAILED_CHECK(m_pModel->Update_AnimationClip(dDeltaTime * m_dAcceleration, m_bIsOnScreen));
	FAILED_CHECK(Adjust_AnimMovedTransform(dDeltaTime));


	if (m_pHPUI != nullptr)
		m_pHPUI->Update(dDeltaTime);

	Update_Collider(dDeltaTime);

	return _int();
}

_int CMonster_Mahinasura_Minion::LateUpdate(_double dDeltaTime)
{
	if (__super::LateUpdate(dDeltaTime) < 0)return -1;

	//////////
	if (m_bIsOnScreen)
	{
		FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
	}
	//////////

	//FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
	FAILED_CHECK(m_pRendererCom->Add_ShadowGroup(CRenderer::SHADOW_ANIMMODEL, this, m_pTransformCom, m_pShaderCom, m_pModel));
	m_vOldPos = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS);

#ifdef _DEBUG
	FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pColliderCom));
	FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pHandAttackColliderCom));
	FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pTailAttackColliderCom));
#endif

	m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, m_pNavigationCom->Get_NaviPosition(m_pTransformCom->Get_MatrixState(CTransform::STATE_POS)));


	if (m_pHPUI != nullptr)
		m_pHPUI->LateUpdate(dDeltaTime);

	return _int();
}

_int CMonster_Mahinasura_Minion::Render()
{
	if (__super::Render() < 0)
		return -1;

	NULL_CHECK_RETURN(m_pModel, E_FAIL);

	CGameInstance* pInstance = GetSingle(CGameInstance);
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));


	FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));

	_uint NumMaterial = m_pModel->Get_NumMaterial();

	for (_uint i = 0; i < NumMaterial; i++)
	{
		for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; j++)
			FAILED_CHECK(m_pModel->Bind_OnShader(m_pShaderCom, i, j, MODLETEXTYPE(j)));
		FAILED_CHECK(m_pModel->Render(m_pShaderCom, 3, i, "g_BoneMatrices"));
	}



	return _int();
}

_int CMonster_Mahinasura_Minion::LateRender()
{
	if (__super::LateRender() < 0)
		return -1;


	return _int();
}

void CMonster_Mahinasura_Minion::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
	if (CollisionTypeID::CollisionType_Player == eConflictedObjCollisionType)
	{
		_Vector vDamageDir = XMVector3Normalize(pConflictedCollider->Get_ColliderPosition(iConflictedObjColliderIndex).XMVector() - m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS));
		pConflictedObj->Take_Damage(this, 1.f, vDamageDir, m_bOnKnockbackCol, m_fKnockbackColPower);
		pConflictedCollider->Set_Conflicted(1.f);
	}
}

_float CMonster_Mahinasura_Minion::Take_Damage(CGameObject * pTargetObject, _float fDamageAmount, _fVector vDamageDir, _bool bKnockback, _float fKnockbackPower)
{
	m_pHPUI->Set_ADD_HitCount((_int)fDamageAmount);
	m_fHP += -fDamageAmount;

	m_dSpecial_CoolTime = 0;
	m_dOnceCoolTime = 0;
	m_dInfinity_CoolTime = 0;

	m_bIOnceAnimSwitch = true;
	if (bKnockback == false)
	{
		m_iOncePattern = 40;
	}
	else {
		m_iOncePattern = 41;
		
		XMStoreFloat3(&m_fKnockbackDir, vDamageDir);
	}

	if (m_fHP < 5 && m_iBoolOnce == 0)
	{
		m_iOncePattern = 42;
		m_dSpecial_CoolTime = 0;
		m_dOnceCoolTime = 0;
		m_dInfinity_CoolTime = 0;

		m_iBoolOnce += 1;
	}

	if (m_fHP <= 0)
	{
		Set_IsDead();
	}
	return _float();
}

HRESULT CMonster_Mahinasura_Minion::Ready_ParticleDesc()
{

	m_pTextureParticleTransform = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
	NULL_CHECK_RETURN(m_pTextureParticleTransform, E_FAIL);

//	m_pTextureParticleTransform_One = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
//	m_pTextureParticleTransform_Tow = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
	//	m_pTextureParticleTransform_BowFront= (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));




	//// 0
	//INSTPARTICLEDESC instanceDesc = GETPARTICLE->Get_TypeDesc_TextureInstance(CPartilceCreateMgr::TEXTURE_EFFECTJ_Bow_Default);
	//instanceDesc.TotalParticleTime = 99999.f;
	//instanceDesc.FollowingTarget = m_pTextureParticleTransform_BowUp;
	////	GETPARTICLE->Create_Texture_Effect_Desc(instanceDesc, m_eNowSceneNum);
	//m_vecTextureParticleDesc.push_back(instanceDesc);


	//// 1
	//instanceDesc.FollowingTarget = m_pTextureParticleTransform_BowBack;
	////	GETPARTICLE->Create_Texture_Effect_Desc(instanceDesc, m_eNowSceneNum);
	//m_vecTextureParticleDesc.push_back(instanceDesc);

	//// 9999여도 죽는다. 
	//m_pTextureParticleTransform_BowUp->Set_IsOwnerDead(true);
	//m_pTextureParticleTransform_BowBack->Set_IsOwnerDead(true);



	return S_OK;
}

HRESULT CMonster_Mahinasura_Minion::Update_Particle(_double timer)
{

	_Matrix mat = m_pTransformCom->Get_WorldMatrix();

	//	EX)
	//	mat.r[3] = vPos - (mat.r[2] * 0.2f + mat.r[0] * 0.03f + mat.r[1] * 0.03f);
	//	m_vecTextureParticleDesc[0].vFixedPosition = mat.r[3];

	mat.r[0] = XMVector3Normalize(mat.r[0]);
	mat.r[1] = XMVector3Normalize(mat.r[1]);
	mat.r[2] = XMVector3Normalize(mat.r[2]);
	_Vector vPos = mat.r[3];

	m_pTextureParticleTransform->Set_Matrix(mat);

	//_Vector vPos2 = vPos + (mat.r[2] * 0.5f + mat.r[0] * 0.1f);
	//mat.r[3] = vPos2;
	//m_pTextureParticleTransform_BowUp->Set_MatrixState(CTransform::STATE_POS, vPos2);

	//_Vector vPos3 = vPos - (mat.r[2] * 0.4f - mat.r[0] * 0.1f);
	//mat.r[3] = vPos3;

	// m_pTextureParticleTransform_BowBack->Set_Matrix(mat);
	return S_OK;
}

HRESULT CMonster_Mahinasura_Minion::SetUp_Info()
{

	m_pTransformCom->Set_MatrixState(CTransform::STATE_POS,_float3(2.f,0.f,2.f));


	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	m_pPlayerTransform = static_cast<CTransform*>(pGameInstance->Get_Commponent_By_LayerIndex(m_eNowSceneNum, TAG_LAY(Layer_Player),TAG_COM(Com_Transform)));

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CMonster_Mahinasura_Minion::SetUp_Collider()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_Collider), (CComponent**)&m_pColliderCom));
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_ColliderSub), (CComponent**)&m_pHandAttackColliderCom));
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_ColliderSubSub), (CComponent**)&m_pTailAttackColliderCom));

	/////////////////m_pColliderCom!@!@#$@!#$@#$@$!@%#$%@#$%%^^W@!
	COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(3.f, 3.f, 3.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	ATTACHEDESC tAttachedDesc;
	tAttachedDesc.Initialize_AttachedDesc(this, "skd_spine_01", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(0.f, 0.f, -0.5f));
	m_vecAttachedDesc.push_back(tAttachedDesc);


	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "skd_head", _float3(1.f,1.f,1.f), _float3(0.f,0.f,0.f), _float3(0.f, 0.f, -0.8f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pColliderCom->Set_ParantBuffer();


	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.8f, 0.8f, 0.8f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "skd_spine_02", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(0.f, -0.1f, -0.45f)); //마지막 인자에 블렌더 뼈 위치 그대로 넣어줄 것 다만 z엔 -로 해줄것
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pColliderCom->Set_ParantBuffer();


	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "skd_cloth_02", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(0.f, -0.15f, -0.2f)); //마지막 인자에 블렌더 뼈 위치 그대로 넣어줄 것 다만 z엔 -로 해줄것
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pColliderCom->Set_ParantBuffer();
	//////////////////////////////////////////////////////////////////////




	/////////////////m_pAttackColliderCom!@!@#$@!#$@#$@$!@%#$%@#$%%^^W@!

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(5.f, 5.f, 5.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pHandAttackColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "skd_spine_01", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(0.f, 0.f, -0.5f));
	m_vecHandAttackAttachedDesc.push_back(tAttachedDesc);

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.7f, 0.7f, 0.7f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pHandAttackColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "skd_r_index_01", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(-0.34938f,-0.11046f,-0.32727f)); //마지막 인자에 블렌더 뼈 위치 그대로 넣어줄 것 다만 z엔 -로 해줄것
	m_vecHandAttackAttachedDesc.push_back(tAttachedDesc);
	m_pHandAttackColliderCom->Set_ParantBuffer();

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.7f, 0.7f, 0.7f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pHandAttackColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "skd_l_index_01", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(0.34938f, -0.11046f, -0.32727f)); //마지막 인자에 블렌더 뼈 위치 그대로 넣어줄 것 다만 z엔 -로 해줄것
	m_vecHandAttackAttachedDesc.push_back(tAttachedDesc);
	m_pHandAttackColliderCom->Set_ParantBuffer();
	//////////////////////////////////////////////////////////////////////



	////////////////////////////////////m_pTailAttackColliderCom!@#!@##!#@
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(7.f, 7.f, 7.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pTailAttackColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "skd_spine_01", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(0.f, 0.f, -0.5f));
	m_vecTailAttackAttachedDesc.push_back(tAttachedDesc);


	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.5f, 0.5f, 0.5f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pTailAttackColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "skd_tail_end", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(0.f, 1.3931f, -0.3834f)); //마지막 인자에 블렌더 뼈 위치 그대로 넣어줄 것 다만 z엔 -로 해줄것
	m_vecTailAttackAttachedDesc.push_back(tAttachedDesc);
	m_pTailAttackColliderCom->Set_ParantBuffer();


	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.5f, 0.5f, 0.5f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pTailAttackColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "skd_tail_10", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(0.f, 1.18f, -0.42061f)); //마지막 인자에 블렌더 뼈 위치 그대로 넣어줄 것 다만 z엔 -로 해줄것
	m_vecTailAttackAttachedDesc.push_back(tAttachedDesc);
	m_pTailAttackColliderCom->Set_ParantBuffer();


	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.3f, 0.3f, 0.3f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pTailAttackColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "skd_tail_09", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(0.f, 1.0177f, -0.41792f)); //마지막 인자에 블렌더 뼈 위치 그대로 넣어줄 것 다만 z엔 -로 해줄것
	m_vecTailAttackAttachedDesc.push_back(tAttachedDesc);
	m_pTailAttackColliderCom->Set_ParantBuffer();


	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.3f, 0.3f, 0.3f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pTailAttackColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "skd_tail_08", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(0.f, 0.9001f, -0.41846f)); //마지막 인자에 블렌더 뼈 위치 그대로 넣어줄 것 다만 z엔 -로 해줄것
	m_vecTailAttackAttachedDesc.push_back(tAttachedDesc);
	m_pTailAttackColliderCom->Set_ParantBuffer();


	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.3f, 0.3f, 0.3f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pTailAttackColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "skd_tail_07", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(0.f, 0.7588, -0.41846f)); //마지막 인자에 블렌더 뼈 위치 그대로 넣어줄 것 다만 z엔 -로 해줄것
	m_vecTailAttackAttachedDesc.push_back(tAttachedDesc);
	m_pTailAttackColliderCom->Set_ParantBuffer();


	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.3f, 0.3f, 0.3f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pTailAttackColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "skd_tail_06", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(0.f, 0.62504f, -0.41846f)); //마지막 인자에 블렌더 뼈 위치 그대로 넣어줄 것 다만 z엔 -로 해줄것
	m_vecTailAttackAttachedDesc.push_back(tAttachedDesc);
	m_pTailAttackColliderCom->Set_ParantBuffer();


	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.3f, 0.3f, 0.3f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pTailAttackColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "skd_tail_05", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(0.f, 0.5134f, -0.419f)); //마지막 인자에 블렌더 뼈 위치 그대로 넣어줄 것 다만 z엔 -로 해줄것
	m_vecTailAttackAttachedDesc.push_back(tAttachedDesc);
	m_pTailAttackColliderCom->Set_ParantBuffer();


	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.3f, 0.3f, 0.3f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pTailAttackColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "skd_tail_04", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(0.f, 0.39583f, -0.419)); //마지막 인자에 블렌더 뼈 위치 그대로 넣어줄 것 다만 z엔 -로 해줄것
	m_vecTailAttackAttachedDesc.push_back(tAttachedDesc);
	m_pTailAttackColliderCom->Set_ParantBuffer();


	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.3f, 0.3f, 0.3f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pTailAttackColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "skd_tail_03", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(0.f, 0.27448f, -0.41954f)); //마지막 인자에 블렌더 뼈 위치 그대로 넣어줄 것 다만 z엔 -로 해줄것
	m_vecTailAttackAttachedDesc.push_back(tAttachedDesc);
	m_pTailAttackColliderCom->Set_ParantBuffer();


	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.3f, 0.3f, 0.3f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pTailAttackColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "skd_tail_02", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(0.f, 0.15097f, -0.41792f)); //마지막 인자에 블렌더 뼈 위치 그대로 넣어줄 것 다만 z엔 -로 해줄것
	m_vecTailAttackAttachedDesc.push_back(tAttachedDesc);
	m_pTailAttackColliderCom->Set_ParantBuffer();


	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.3f, 0.3f, 0.3f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pTailAttackColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "skd_tail_01", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(0.f, 0.040943f, -0.419f)); //마지막 인자에 블렌더 뼈 위치 그대로 넣어줄 것 다만 z엔 -로 해줄것
	m_vecTailAttackAttachedDesc.push_back(tAttachedDesc);
	m_pTailAttackColliderCom->Set_ParantBuffer();

	//////////////////////////////////////////////////////////////////////


	return S_OK;
}

HRESULT CMonster_Mahinasura_Minion::SetUp_Fight(_double dDeltaTime)
{
	m_fDistance = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS).Get_Distance(m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS));


	if (m_fDistance < 1)
	{
		switch (m_iInfinityAnimNumber)
		{
		case 1:
			m_pTransformCom->Move_Backward(dDeltaTime * 0.6,m_pNavigationCom);
			break;
		case 21:
			m_pTransformCom->Move_Backward(dDeltaTime * 1.2, m_pNavigationCom);
			break;
		default:
			m_pTransformCom->Move_Backward(dDeltaTime, m_pNavigationCom);
			break;

		}
	}

	if (m_bLookAtOn)
	{
		//m_pTransformCom->LookAt(m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS));


		_Vector vTarget = XMVector3Normalize(m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS) - m_pTransformCom->Get_MatrixState(CTransform::STATE_POS));
	
		//m_pTransformCom->Turn_Dir(XMVector3Normalize(m_pTransformCom->Get_MatrixScale(CTransform::STATE_LOOK)*0.9 + vTarget* 0.1));
		m_pTransformCom->Turn_Dir(vTarget,0.9f);
	}

	//CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	////평범하게 움직이기
	//if (pGameInstance->Get_DIKeyState(DIK_Y) & DIS_Press)
	//{
	//	m_pTransformCom->Move_Up(dDeltaTime);
	//}
	//if (pGameInstance->Get_DIKeyState(DIK_H) & DIS_Press)
	//{
	//	m_pTransformCom->Move_Down(dDeltaTime);
	//}

	//RELEASE_INSTANCE(CGameInstance);


	return S_OK;
}

HRESULT CMonster_Mahinasura_Minion::Update_Collider(_double dDeltaTime)
{
	m_pColliderCom->Update_ConflictPassedTime(dDeltaTime);
	m_pHandAttackColliderCom->Update_ConflictPassedTime(dDeltaTime);
	m_pTailAttackColliderCom->Update_ConflictPassedTime(dDeltaTime);

	//Collider
	_uint	iNumCollider = m_pColliderCom->Get_NumColliderBuffer();
	for (_uint i = 0; i < iNumCollider; i++)
		m_pColliderCom->Update_Transform(i, m_vecAttachedDesc[i].Caculate_AttachedBoneMatrix_BlenderFixed());

	if (m_bColliderAttackOn == true)
	{
		switch (m_eColliderType)
		{
		case Client::CMonster_Mahinasura_Minion::HANDATTACK:
			//HandAttackCollider
			iNumCollider = m_pHandAttackColliderCom->Get_NumColliderBuffer();
			for (_uint i = 0; i < iNumCollider; i++)
				m_pHandAttackColliderCom->Update_Transform(i, m_vecHandAttackAttachedDesc[i].Caculate_AttachedBoneMatrix_BlenderFixed());
			break;
		case Client::CMonster_Mahinasura_Minion::TAILATTACK:
			//TailAttackCollider
			iNumCollider = m_pTailAttackColliderCom->Get_NumColliderBuffer();
			for (_uint i = 0; i < iNumCollider; i++)
				m_pTailAttackColliderCom->Update_Transform(i, m_vecTailAttackAttachedDesc[i].Caculate_AttachedBoneMatrix_BlenderFixed());
			break;
		default:
			break;
		}
	}

	FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_Monster, this, m_pColliderCom));




	if (m_bColliderAttackOn == true)
	{
		switch (m_eColliderType)
		{
		case Client::CMonster_Mahinasura_Minion::HANDATTACK:
			FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_MonsterWeapon, this, m_pHandAttackColliderCom));
			break;
		case Client::CMonster_Mahinasura_Minion::TAILATTACK:
			FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_MonsterWeapon, this, m_pTailAttackColliderCom));
			break;
		default:
			break;
		}
	}
	FAILED_CHECK(g_pGameInstance->Add_RepelGroup(m_pTransformCom, 1.f, m_pNavigationCom));

	return S_OK;
}

HRESULT CMonster_Mahinasura_Minion::PlayAnim(_double dDeltaTime)
{
	SetUp_Fight(dDeltaTime);

	CoolTime_Manager(dDeltaTime);

	if (true == m_bIOnceAnimSwitch)
	{
		Once_AnimMotion(dDeltaTime);
		m_pModel->Change_AnimIndex(m_iOnceAnimNumber);
	}
	else
	{
		Infinity_AnimMotion(dDeltaTime);
		m_pModel->Change_AnimIndex(m_iInfinityAnimNumber);
	}

	return S_OK;
}

HRESULT CMonster_Mahinasura_Minion::CoolTime_Manager(_double dDeltaTime)
{
	//한번만 동작하는 애니메이션

	m_dOnceCoolTime += dDeltaTime;

	if (m_dOnceCoolTime > 2 && m_fDistance < 3)
	{
		m_dOnceCoolTime = 0;
		m_dInfinity_CoolTime = 0;

		m_bIOnceAnimSwitch = true;

		m_bFastRunOn = false;

		Pattern_Change();
	}

	//반복적으로 동작하는 애니메이션
	m_dInfinity_CoolTime += dDeltaTime;
	if (m_dInfinity_CoolTime >= 1.5)
	{
		m_iInfinityPattern = rand() % 7;


		m_dInfinity_CoolTime = 0;

		m_bFastRunOn = false;
	}

	Special_Trigger(dDeltaTime);

	return S_OK;
}

HRESULT CMonster_Mahinasura_Minion::Once_AnimMotion(_double dDeltaTime)
{
	switch (m_iOncePattern)
	{
	case 0:
		m_iOnceAnimNumber = 3; //_Dodge_Right
		break;
	//case 1:
	//	m_iOnceAnimNumber = 18;
	//	break;
	case 1:
		m_iOnceAnimNumber = 2; //Dodge_Back
		break;
	case 2:
		m_iOnceAnimNumber = 19; //QuickAttack
		break;
	case 3:
		m_iOnceAnimNumber = 4; //_Dodge_Left
		break;
	case 4:
		m_iOnceAnimNumber = 20; //TailWhip
		break;
	case 30:
		m_iOnceAnimNumber = 6; //Taunt
		break;
	case 40:
		m_iOnceAnimNumber = 15; //LightHit
		break;
	case 41:
		m_iOnceAnimNumber = 16; //HeavyHit
		break;
	case 42:
		m_iOnceAnimNumber = 8; //groggy
		break;
	}

	return S_OK;
}

HRESULT CMonster_Mahinasura_Minion::Pattern_Change()
{

	m_iOncePattern += 1;

	if (m_iOncePattern > 5)
	{
		m_iOncePattern = rand() % 5; //OncePattern Random
	}


	return S_OK;
}

HRESULT CMonster_Mahinasura_Minion::Infinity_AnimMotion(_double dDeltaTime)
{
	switch (m_iInfinityPattern)
	{
	case 0:
		m_iInfinityAnimNumber = 0;
		break;
	case 1:
		m_pTransformCom->Move_Forward(dDeltaTime * 0.3, m_pNavigationCom);
		m_iInfinityAnimNumber = 1;
		break;
	case 2:
		m_iInfinityAnimNumber = 21;
		break;
	case 3:
		m_iInfinityAnimNumber = 21;
		break;
	case 4:
		m_iInfinityAnimNumber = 21;
		break;
	case 5:
		m_iInfinityAnimNumber = 21;
		break;
	case 6:
		m_iInfinityAnimNumber = 21;
		break;
	}

	return S_OK;
}

HRESULT CMonster_Mahinasura_Minion::Special_Trigger(_double dDeltaTime)
{
	m_dSpecial_CoolTime += dDeltaTime;


	if (m_fDistance > 8 && m_dSpecial_CoolTime > 10)
	{
		m_dSpecial_CoolTime = 0;
		m_dOnceCoolTime = 0;
		m_dInfinity_CoolTime = 0;

		m_bIOnceAnimSwitch = true;
		m_iOncePattern = 30;
	}


	return S_OK;
}

HRESULT CMonster_Mahinasura_Minion::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_Monster_Mahinasura_Minion), TAG_COM(Com_Model), (CComponent**)&m_pModel));
	FAILED_CHECK(m_pModel->Change_AnimIndex(0));

	
	CTransform::TRANSFORMDESC tDesc = {};

	tDesc.fMovePerSec = 5;
	tDesc.fRotationPerSec = XMConvertToRadians(60);
	tDesc.vPivot = _float3(0, 0, 0);
	tDesc.fScalingPerSec = 1;

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));


	CHpUI::HPDesc HpDesc;
	HpDesc.m_HPType = CHpUI::HP_MONSTER;
	HpDesc.m_pObjcect = this;
	HpDesc.m_vPos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
	HpDesc.m_Dimensions = 1.5f;
	m_fMaxHP = 15.f;
	m_fHP = m_fMaxHP;
	g_pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&m_pHPUI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_HpUI), &HpDesc);

	SetUp_Collider();
	

	return S_OK;
}

HRESULT CMonster_Mahinasura_Minion::Adjust_AnimMovedTransform(_double dDeltaTime)
{
	_uint iNowAnimIndex = m_pModel->Get_NowAnimIndex();
	_double PlayRate = m_pModel->Get_PlayRate();

	if (iNowAnimIndex != m_iOldAnimIndex || PlayRate > 0.98)
	{
		m_iAdjMovedIndex = 0;

		m_bLookAtOn = true;

		m_dAcceleration = 1;

		m_bColliderAttackOn = false;

		if (PlayRate > 0.98 && m_bIOnceAnimSwitch == true)
		{
			m_bIOnceAnimSwitch = false;
			m_dOnceCoolTime = 0;
			m_dInfinity_CoolTime = 0;
		}
	}
	
	if (PlayRate <= 0.98) //애니메이션의 비율 즉, 0.98은 거의 끝나가는 시점
	{


		//switch (iNowAnimIndex)
		//{
		//case 1://애니메이션 인덱스마다 잡아주면 됨
		//	if (m_iAdjMovedIndex == 0 && PlayRate > 0.0) // 이렇게 되면 이전 애니메이션에서 보간되는 시간 끝나자 마자 바로 들어옴 즉, PlayRate의 0은 >= 하지말고 >로 하셈
		//	{

		//		m_iAdjMovedIndex++; //애니메이션이 동작할 때 한번만 발동시키기 위해 ++시킨다.
		//	}
		//	else if (m_iAdjMovedIndex == 1 && PlayRate > 0.7666666666666666) //특정 프레임 플레이 레이트이후에 들어오면실행
		//	{


		//		m_iAdjMovedIndex++;
		//	}

		//	break;
		//case 2:

		//	break;
		//}
		
		

		switch (iNowAnimIndex)
		{
		//case 1:
		//{
		//	if (PlayRate > 0)
		//	{
		//		m_pTransformCom->Move_Forward(dDeltaTime * 0.2);
		//	}
		//	break;
		//}
		case 2:
		{
			if (PlayRate >= 0.175 && PlayRate <= 0.5)
			{
				m_bLookAtOn = false;

				_float fSpeed = g_pGameInstance->Easing(TYPE_QuinticOut, 4.5f, 1.05f, (_float)PlayRate - 0.175f, 0.325f);
				m_pTransformCom->Move_Backward(dDeltaTime * fSpeed, m_pNavigationCom);
			}
			break;
		}
		case 3:
		{
			if (PlayRate >= 0.1 && PlayRate <= 0.5)
			{
				if (m_iAdjMovedIndex == 0)
				{
					m_TempLook = m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK);

					m_iAdjMovedIndex += 1;
				}
				//_Vector vRight = XMVector3Cross(XMLoadFloat3(&_float3(0.f, 1.f, 0.f)), XMLoadFloat3(&m_TempLook)); //Left
				//_float fSpeed = g_pGameInstance->Easing(TYPE_QuinticOut, 10.f, 3.f, (_float)PlayRate - 0.1f, 0.38f);
				//_float3 vDir;
				//XMStoreFloat3(&vDir, XMVector3Normalize(vRight) * fSpeed * (_float)dDeltaTime);
				//m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, m_pTransformCom->Get_MatrixState(CTransform::STATE_POS) + XMLoadFloat3(&vDir));

				_Vector vRight = XMVector3Cross(XMLoadFloat3(&_float3(0.f, 1.f, 0.f)), XMLoadFloat3(&m_TempLook)); //Left
				_float fSpeed = g_pGameInstance->Easing(TYPE_QuinticOut, 4.5f, 0.75f, (_float)PlayRate - 0.1f, 0.38f);
				m_pTransformCom->MovetoDir(vRight, dDeltaTime * fSpeed, m_pNavigationCom);
			}
			break;
		}
		case 4:
		{
			if (PlayRate>=0.1 && PlayRate <= 0.5)
			{
				if (m_iAdjMovedIndex == 0)
				{
					m_TempLook = m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK);

					m_iAdjMovedIndex += 1;
				}
				//_Vector vRight = XMVector3Cross(XMLoadFloat3(&_float3(0.f, 1.f, 0.f)), XMLoadFloat3(&m_TempLook)); //Left
				//_float fSpeed = g_pGameInstance->Easing(TYPE_QuinticOut, 10.f, 3.f, (_float)PlayRate - 0.1f, 0.38f);
				//_float3 vDir;
				//XMStoreFloat3(&vDir, XMVector3Normalize(-vRight) * fSpeed * (_float)dDeltaTime);
				//m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, m_pTransformCom->Get_MatrixState(CTransform::STATE_POS) + XMLoadFloat3(&vDir));

				_Vector vRight = XMVector3Cross(XMLoadFloat3(&_float3(0.f, 1.f, 0.f)), XMLoadFloat3(&m_TempLook)); //Left;
				_float fSpeed = g_pGameInstance->Easing(TYPE_QuinticOut, 4.5f, 0.75f, (_float)PlayRate - 0.1f, 0.38f);
				m_pTransformCom->MovetoDir(-vRight, dDeltaTime * fSpeed, m_pNavigationCom);
			}
			break;
		}
		case 8:
		{
			if (PlayRate > 0 && PlayRate <= 0.98)
			{
				m_bLookAtOn = false;

				m_dAcceleration = 0.3;
			}
			break;
		}
		case 15:
		{
			if (m_iAdjMovedIndex == 0 && PlayRate > 0)
			{
				m_dAcceleration = 0.5;
				m_iAdjMovedIndex++;
			}
			break;
		}
		case 16:
		{
			if (m_iAdjMovedIndex == 0 && PlayRate > 0)
			{
				m_bLookAtOn = false;
				m_dAcceleration = 1;
				m_iAdjMovedIndex++;
			}
			else if (0.f < PlayRate && PlayRate <= 0.478f)
			{
				m_pTransformCom->Move_Backward(dDeltaTime, m_pNavigationCom);

				m_fKnockbackDir.y = 0;

				m_pTransformCom->Turn_Dir(m_fKnockbackDir.XMVector(), 0.9f);
			}
			break;
		}
		case 18:
		{
			if (m_iAdjMovedIndex == 0)
			{
				m_bLookAtOn = false;
				m_bColliderAttackOn = true;
				m_eColliderType = CMonster_Mahinasura_Minion::HANDATTACK;
				m_iAdjMovedIndex++;
			}
			if (PlayRate >= 0.27272 && PlayRate <= 0.444)
			{

				_float EasingSpeed;
				EasingSpeed = GetSingle(CGameInstance)->Easing(TYPE_QuadIn, 2.25 , 3.3f, (_float)PlayRate -0.27272f, 0.17128f);

				m_pTransformCom->Move_Forward(dDeltaTime * EasingSpeed, m_pNavigationCom);
			}
			else if (PlayRate >= 0.444 && PlayRate <= 0.6)
			{
				_float EasingSpeed;
				EasingSpeed = GetSingle(CGameInstance)->Easing(TYPE_QuadOut, 3.3f , 1.5f,static_cast<_float>(PlayRate) -0.444f, 0.156f);

				m_pTransformCom->Move_Forward(dDeltaTime * EasingSpeed, m_pNavigationCom);
			}
			break;
		}
		case 19: {
			if (m_iAdjMovedIndex == 0 && PlayRate >= 0.24)
			{
				m_bLookAtOn = false;
				m_bColliderAttackOn = true;
				m_eColliderType = CMonster_Mahinasura_Minion::HANDATTACK;

				m_iAdjMovedIndex++;
			}
			if (PlayRate >= 0.24 && PlayRate <= 0.48)
			{
				m_pTransformCom->Move_Forward(dDeltaTime * 0.9, m_pNavigationCom);
			}
			else if (PlayRate >= 0.49 && PlayRate <= 0.84)
			{
				m_pTransformCom->Move_Forward(dDeltaTime * 1.8, m_pNavigationCom);
			}
			break;
		}
		case 20: {
			if (m_iAdjMovedIndex == 0)
			{
				m_bLookAtOn = false;
				m_bColliderAttackOn = true;
				m_eColliderType = CMonster_Mahinasura_Minion::TAILATTACK;

				m_iAdjMovedIndex++;
			}
			if (PlayRate >= 0.24 && PlayRate <= 0.6)
			{

				//_float EasingSpeed;
				//EasingSpeed = GetSingle(CGameInstance)->Easing(TYPE_CircularOut, 1.7f , 1.f,(_float)PlayRate-0.24f, 0.36f);
				//m_pTransformCom->Move_Forward(dDeltaTime * EasingSpeed);

				_float fSpeed = g_pGameInstance->Easing_Return(TYPE_SinInOut, TYPE_SinInOut, 1.5f, 3.f, (_float)PlayRate-0.24f, 0.36f); // PlayRate - 0.266666 and 0.5 - 0.266666
				m_pTransformCom->Move_Forward(dDeltaTime * fSpeed, m_pNavigationCom);

				//m_pTransformCom->Move_Forward(dDeltaTime * 1.2);
			}
			break;
		}
		case 21: {
			if (PlayRate > 0 && PlayRate >= 0.125 && m_bFastRunOn == false)
			{
				m_pTransformCom->Move_Forward(dDeltaTime * 1.8, m_pNavigationCom);
				m_bFastRunOn = true;
			}
			else if (m_bFastRunOn = true)
			{
				m_pTransformCom->Move_Forward(dDeltaTime * 1.8, m_pNavigationCom);
			}
			break;
		}
		default:
			break;
		}
	}

	m_iOldAnimIndex = iNowAnimIndex;
	return S_OK;
}

CMonster_Mahinasura_Minion * CMonster_Mahinasura_Minion::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CMonster_Mahinasura_Minion*	pInstance = NEW CMonster_Mahinasura_Minion(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CMonster_Mahinasura_Minion");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMonster_Mahinasura_Minion::Clone(void * pArg)
{
	CMonster_Mahinasura_Minion*	pInstance = NEW CMonster_Mahinasura_Minion(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CMonster_Mahinasura_Minion");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMonster_Mahinasura_Minion::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModel);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTailAttackColliderCom);
	Safe_Release(m_pHPUI);
}
