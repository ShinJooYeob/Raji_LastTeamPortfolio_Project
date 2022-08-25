#include "stdafx.h"
#include "..\public\Monster_Ninjasura_Minion.h"
#include "Monster_Bullet_Universal.h"
#include "HpUI.h"

CMonster_Ninjasura_Minion::CMonster_Ninjasura_Minion(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CMonster(pDevice, pDeviceContext)
{
}

CMonster_Ninjasura_Minion::CMonster_Ninjasura_Minion(const CMonster_Ninjasura_Minion & rhs)
	: CMonster(rhs)
{
}

HRESULT CMonster_Ninjasura_Minion::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));


	return S_OK;
}

HRESULT CMonster_Ninjasura_Minion::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components());

	if (pArg != nullptr)
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, *((_float3*)pArg));

	m_pTransformCom->Rotation_CW(XMVectorSet(0, 1, 0, 0), XMConvertToRadians(170));


	SetUp_Info();


#ifdef _DEBUG
	//////////////////testPosition
	// m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, _float3(493.f, 7.100010f, 103.571f));// Stage2
	// m_pNavigationCom->FindCellIndex(m_pTransformCom->Get_MatrixState(CTransform::STATE_POS));
	//////////////////////////////
#endif

// Particle
	 Set_DealyDIssolveTime(1.0f,0.3f);
	return S_OK;
}

_int CMonster_Ninjasura_Minion::Update(_double dDeltaTime)
{

	if (__super::Update(dDeltaTime) < 0)return -1;
	if (__super::Update(dDeltaTime) == UPDATE_SKIP)
		return UPDATE_SKIP;
	if (m_fHP <= 0)
	{
		m_bRepelOff = true;
		m_bLookAtOn = false;
		m_pDissolve->Update_Dissolving(dDeltaTime);
		m_pDissolve->Set_DissolveOn(false, 2.f);

		m_dDissolveTime += dDeltaTime;

		m_bMotionTrailOn = false;
		if (m_bDieSound == false && m_dDissolveTime >= 1.)
		{
			g_pGameInstance->Play3D_Sound(TEXT("EH_Ninja_Hit_Vox_04.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_MONSTER, 0.5f);
			m_bDieSound = true;
		}
		if (m_dDissolveTime >= 2)
		{
			Set_IsDead();
		}
	}

	//마지막 인자의 bBlockAnimUntilReturnChange에는 true로 시작해서 정상작동이 된다면 false가 된다.
	//m_pModel->Change_AnimIndex();
	//m_pModel->Change_AnimIndex_ReturnTo(); //어떤 애니메이션을 돌리고 특정 애니메이션으로 보냄
	//m_pModel->Change_AnimIndex_ReturnTo_Must(); //중간에 애니메이션을 캔슬하고 다른 애니메이션을 동작시킴
	//m_pModel->Change_AnimIndex_UntilTo(); //1~5까지 돌린다고 명령어를 입력하면 1~5까지 돌아감
	//m_pModel->Change_AnimIndex_UntilNReturn();//1~5까지 돌리고 난 이후 특정 애니메이션으로 Return 시킬 경우
	//m_pModel->Change_AnimIndex_UntilNReturn_Must(); //1~5까지 돌리고 난 이후 특정 애니메이션으로 Return 시킬 수 있지만 다른 애니메이션을 동작시킬 수 있음



	PlayAnim(dDeltaTime);

	m_bIsOnScreen = g_pGameInstance->IsNeedToRender(m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS), m_fFrustumRadius);
	if (m_fHP > 0)
	{
		FAILED_CHECK(m_pModel->Update_AnimationClip(dDeltaTime * m_dAcceleration, m_bIsOnScreen));
	}
	FAILED_CHECK(Adjust_AnimMovedTransform(dDeltaTime));

	//////////////Motion Test
	m_pMotionTrail->Update_MotionTrail(dDeltaTime);
	/////////////////////////

	if (m_pHPUI != nullptr && m_bMotionTrailOn == false)
		m_pHPUI->Update(dDeltaTime);

	Update_Collider(dDeltaTime);

	return _int();
}

_int CMonster_Ninjasura_Minion::LateUpdate(_double dDeltaTime)
{
	if (__super::LateUpdate(dDeltaTime) < 0)return -1;
	if (__super::LateUpdate(dDeltaTime) == UPDATE_SKIP)
		return UPDATE_SKIP;
	//////////
	if (m_bIsOnScreen && false == m_bMotionTrailOn)
	{
		FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
	}
	//////////////Motion Test
	FAILED_CHECK(m_pRendererCom->Add_TrailGroup(CRenderer::TRAIL_MOTION, m_pMotionTrail));
	////////////////

	if (m_bColliderAttackOn == true)
	{
		FAILED_CHECK(m_pRendererCom->Add_ShadowGroup(CRenderer::SHADOW_ANIMMODEL, this, m_pTransformCom, m_pShaderCom, m_pModel, nullptr, m_pDissolve));
	}
	m_vOldPos = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS);

#ifdef _DEBUG
	FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pColliderCom));
	FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pAttackColliderCom));
#endif

	m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, m_pNavigationCom->Get_NaviPosition(m_pTransformCom->Get_MatrixState(CTransform::STATE_POS)));

	if (m_SpawnDealytime <= 0 && m_bIsSpawnDissolove == false)
	{
		m_pDissolve->Set_DissolveOn(true, m_SpawnDissolveTime);
		m_pDissolve->Update_Dissolving(dDeltaTime);

		if (m_pDissolve->Get_IsDissolving() == false)
			m_bIsSpawnDissolove = true;
	}

	if (m_pHPUI != nullptr && m_bMotionTrailOn == false)
	{
		m_pHPUI->LateUpdate(dDeltaTime);
	}

	return _int();
}

_int CMonster_Ninjasura_Minion::Render()
{
	if (__super::Render() < 0)
		return -1;
	if (__super::Render() == UPDATE_SKIP)
		return UPDATE_SKIP;
	NULL_CHECK_RETURN(m_pModel, E_FAIL);

	CGameInstance* pInstance = GetSingle(CGameInstance);
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));


	FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));

	FAILED_CHECK(m_pDissolve->Render(3)); //디졸브 내부에서 밑의 머테리얼을 찾아주고 있음

	//_uint NumMaterial = m_pModel->Get_NumMaterial();

	//for (_uint i = 0; i < NumMaterial; i++)
	//{
	//	for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; j++)
	//		FAILED_CHECK(m_pModel->Bind_OnShader(m_pShaderCom, i, j, MODLETEXTYPE(j)));
	//	FAILED_CHECK(m_pModel->Render(m_pShaderCom, 3, i, "g_BoneMatrices"));
	//}



	return _int();
}

_int CMonster_Ninjasura_Minion::LateRender()
{
	if (__super::LateRender() < 0)
		return -1;
	if (__super::LateRender() == UPDATE_SKIP)
		return UPDATE_SKIP;
	return _int();
}

void CMonster_Ninjasura_Minion::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
	if (CollisionTypeID::CollisionType_Player == eConflictedObjCollisionType)
	{
		_Vector vDamageDir = XMVector3Normalize(pConflictedCollider->Get_ColliderPosition(iConflictedObjColliderIndex).XMVector() - m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS));
		pConflictedObj->Take_Damage(this, 1.f, vDamageDir, m_bOnKnockbackCol, m_fKnockbackColPower);
		pConflictedCollider->Set_Conflicted(1.f);
	}
}

_float CMonster_Ninjasura_Minion::Take_Damage(CGameObject * pTargetObject, _float fDamageAmount, _fVector vDamageDir, _bool bKnockback, _float fKnockbackPower)
{
	m_pHPUI->Set_ADD_HitCount((_int)fDamageAmount);
	m_fHP += -fDamageAmount;

	m_bStopCoolTimeOn = true;
	m_bMotionTrailOn = false;

	m_bIOnceAnimSwitch = true;
	if (m_eMonster_State != Anim_State::MONSTER_ATTACK)
	{
		if (bKnockback == false)
		{
			m_bKnockbackOn = false;

			_int iRand_Num = rand() % 3;
			if (2 == iRand_Num)
			{
				m_iOncePattern = 40;
			}
		}
		else {
			m_bKnockbackOn = true;

			_int iRand_Num = rand() % 3;
			if (2 == iRand_Num)
			{
				m_iOncePattern = 40;
				m_iOncePattern = 40;
			}

			XMStoreFloat3(&m_fKnockbackDir, vDamageDir);
		}


		if (m_fHP < 5 && m_iBoolOnce == 0)
		{
			m_iOncePattern = 42;
			m_dOnceCoolTime = 0;
			m_dInfinity_CoolTime = 0;

			m_iBoolOnce += 1;
		}
	}

	if (0 >= m_fHP)
	{
		return -1.f;
	}

	return _float();
}

HRESULT CMonster_Ninjasura_Minion::SetUp_Info()
{

	m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, _float3(2.f, 0.f, 2.f));


	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	m_pPlayerTransform = static_cast<CTransform*>(pGameInstance->Get_Commponent_By_LayerIndex(m_eNowSceneNum, TAG_LAY(Layer_Player), TAG_COM(Com_Transform)));
	m_pPlayerNavigation = static_cast<CNavigation*>(pGameInstance->Get_Commponent_By_LayerIndex(m_eNowSceneNum, TAG_LAY(Layer_Player), TAG_COM(Com_Navaigation)));
	RELEASE_INSTANCE(CGameInstance);


	return S_OK;
}

HRESULT CMonster_Ninjasura_Minion::SetUp_Collider()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_Collider), (CComponent**)&m_pColliderCom));
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_ColliderSub), (CComponent**)&m_pAttackColliderCom));

	/////////////////////////m_pColliderCom
	COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(3.f, 3.f, 3.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	ATTACHEDESC tAttachedDesc;
	tAttachedDesc.Initialize_AttachedDesc(this, "spine_01", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(0.f, -0.023774f, -1.1029f));
	m_vecAttachedDesc.push_back(tAttachedDesc);


	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "head", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(0.f, -0.01186f, -1.5361f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pColliderCom->Set_ParantBuffer();


	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "pelvis", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(0.f, -0.008054f, -0.4974f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pColliderCom->Set_ParantBuffer();


	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "thigh_r", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(0.f, -0.006605f, -1.0128f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pColliderCom->Set_ParantBuffer();
	//////////////////////////////////////////////


	/////////////////////////////////////////////m_pAttackColliderCom
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(5.f, 5.f, 5.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pAttackColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "spine_01", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(0.f, 0.f, -0.5f));
	m_vecAttackAttachedDesc.push_back(tAttachedDesc);


	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.7f, 0.7f, 0.7f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pAttackColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "pelvis", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(-0.f, -0.008054f, -0.9948f)); //마지막 인자에 블렌더 뼈 위치 그대로 넣어줄 것 다만 z엔 -로 해줄것
	m_vecAttackAttachedDesc.push_back(tAttachedDesc);
	m_pAttackColliderCom->Set_ParantBuffer();


	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.7f, 0.7f, 0.7f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pAttackColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "lowerarm_twist_01_r", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(-0.50823f, -0.094688f, -1.0442f)); //마지막 인자에 블렌더 뼈 위치 그대로 넣어줄 것 다만 z엔 -로 해줄것
	m_vecAttackAttachedDesc.push_back(tAttachedDesc);
	m_pAttackColliderCom->Set_ParantBuffer();


	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.7f, 0.7f, 0.7f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pAttackColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "sword_r", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(-0.82335f, -0.24675f, -0.77251f)); //마지막 인자에 블렌더 뼈 위치 그대로 넣어줄 것 다만 z엔 -로 해줄것
	m_vecAttackAttachedDesc.push_back(tAttachedDesc);
	m_pAttackColliderCom->Set_ParantBuffer();


	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.7f, 0.7f, 0.7f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pAttackColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "lowerarm_twist_01_l", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(0.50823f, -0.094688f, -1.0442f)); //마지막 인자에 블렌더 뼈 위치 그대로 넣어줄 것 다만 z엔 -로 해줄것
	m_vecAttackAttachedDesc.push_back(tAttachedDesc);
	m_pAttackColliderCom->Set_ParantBuffer();


	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.7f, 0.7f, 0.7f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pAttackColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "sword_l", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(0.82335f, -0.24675f, -0.77251f)); //마지막 인자에 블렌더 뼈 위치 그대로 넣어줄 것 다만 z엔 -로 해줄것
	m_vecAttackAttachedDesc.push_back(tAttachedDesc);
	m_pAttackColliderCom->Set_ParantBuffer();
	/////////////////////////////////////////////
	return S_OK;
}

HRESULT CMonster_Ninjasura_Minion::SetUp_Fight(_double dDeltaTime)
{
	m_fDistance = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS).Get_Distance(m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS));

	if (m_fDistance >= 0.3)
	{
		if (m_bLookAtOn)
		{
			//m_pTransformCom->LookAt(m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS));


			_Vector vTarget = XMVector3Normalize(m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS) - m_pTransformCom->Get_MatrixState(CTransform::STATE_POS));

			//m_pTransformCom->Turn_Dir(XMVector3Normalize(m_pTransformCom->Get_MatrixScale(CTransform::STATE_LOOK)*0.9 + vTarget* 0.1));
			m_pTransformCom->Turn_Dir(vTarget, 0.9f);
		}
	}


	return S_OK;
}

HRESULT CMonster_Ninjasura_Minion::Update_Collider(_double dDeltaTime)
{
	m_pColliderCom->Update_ConflictPassedTime(dDeltaTime);
	m_pAttackColliderCom->Update_ConflictPassedTime(dDeltaTime);

	//m_pColliderCom
	_uint	iNumCollider = m_pColliderCom->Get_NumColliderBuffer();
	for (_uint i = 0; i < iNumCollider; i++)
		m_pColliderCom->Update_Transform(i, m_vecAttachedDesc[i].Caculate_AttachedBoneMatrix_BlenderFixed());

	FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_Monster, this, m_pColliderCom));

	//m_pAttackColliderCom
	if (m_bColliderAttackOn == true)
	{
		iNumCollider = m_pAttackColliderCom->Get_NumColliderBuffer();
		for (_uint i = 0; i < iNumCollider; i++)
			m_pAttackColliderCom->Update_Transform(i, m_vecAttackAttachedDesc[i].Caculate_AttachedBoneMatrix_BlenderFixed());

		FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_MonsterWeapon, this, m_pAttackColliderCom));
	}

	if(m_bMotionTrailOn == false && m_bRepelOff != true)
		FAILED_CHECK(g_pGameInstance->Add_RepelGroup(m_pTransformCom, 1.2f, m_pNavigationCom));


	return S_OK;
}

HRESULT CMonster_Ninjasura_Minion::Ready_ParticleDesc()
{
	m_pTextureParticleTransform_Demo1 = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
	NULL_CHECK_BREAK(m_pTextureParticleTransform_Demo1);

	m_pTextureParticleTransform_Demo2 = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
	NULL_CHECK_BREAK(m_pTextureParticleTransform_Demo2);

	m_pTextureParticleTransform_Demo3 = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
	NULL_CHECK_BREAK(m_pTextureParticleTransform_Demo3);

	m_pTextureParticleTransform_Demo4 = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
	NULL_CHECK_BREAK(m_pTextureParticleTransform_Demo4);
	return S_OK;

}

HRESULT CMonster_Ninjasura_Minion::Update_Particle(_double timer)
{
//	_Matrix mat_World = m_vecAttachedDesc[0].Caculate_AttachedBoneMatrix_BlenderFixed();
	_Matrix mat_World = m_pTransformCom->Get_WorldMatrix();
	mat_World.r[0] = XMVector3Normalize(mat_World.r[0]);
	mat_World.r[1] = XMVector3Normalize(mat_World.r[1]);
	mat_World.r[2] = XMVector3Normalize(mat_World.r[2]);

//mat_World.r[3] = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS).XMVector();
	m_pTextureParticleTransform_Demo1->Set_Matrix(mat_World); // Head
	
	_Matrix mat_spine_01 = m_vecAttachedDesc[1].Caculate_AttachedBoneMatrix_BlenderFixed();
	m_pTextureParticleTransform_Demo2->Set_Matrix(mat_spine_01); // 

	if (KEYDOWN(DIK_V))
	{
	//	Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_MONSTER_NM_Test, m_pTextureParticleTransform_Demo1);
	// 	Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_MONSTER_NM_Cash4, m_pTextureParticleTransform_Demo1);


		{
			//
			INSTMESHDESC testMesh = GETPARTICLE->Get_EffectSetting_Mesh(CPartilceCreateMgr::E_MESHINST_EFFECTJ::Um_MeshBase,
				//	Prototype_Mesh_SM_DivineLaserBeam_02,
				Prototype_Mesh_SM_DS_Basic_03_3_X_L,
				1.0f,
				0.2f,
				_float4(0.15f, 0.38f, 0.92f, 1),
				_float4(0.15f, 0.38f, 0.92f, 0.0f),
				1,
				_float3(0.1f, 0.5f, 0.1f).XMVector()*0.6f,
				_float3(0.1f, 0.5f, 0.1f).XMVector()*0.6f,
				1);

			// testMesh.fDistortionNoisingPushPower = 0;
			// testMesh.Particle_Power = ;
			testMesh.eParticleTypeID = InstanceEffect_Ball;
			testMesh.eInstanceCount = Prototype_ModelInstance_8;
			testMesh.ePassID = MeshPass_MaskingNoising_Appear_Bright;
			// testMesh.Particle_Power = randpower;
			testMesh.iMaskingTextureIndex = 64;
			testMesh.iNoiseTextureIndex = 102;
			testMesh.vEmissive_SBB = _float3(1);



			testMesh.ParticleStartRandomPosMin = _float3(-2, -1.0f, -2);
			testMesh.ParticleStartRandomPosMax = _float3(2, 1.0f, 2);
			//	_Matrix mat = m_pTextureParticleTransform_Demo1->Get_WorldMatrix();
			//	_Vector pos = mat.r[3];
			//	testMesh.vFixedPosition = pos;
			testMesh.FollowingTarget = m_pTextureParticleTransform_Demo1;
			testMesh.vNoisePushingDir = _float2(-1, 0);
		//	GETPARTICLE->Create_MeshInst_DESC(testMesh, m_eNowSceneNum);

		}
		{
			INSTMESHDESC testMesh = GETPARTICLE->Get_EffectSetting_Mesh(CPartilceCreateMgr::E_MESHINST_EFFECTJ::Um_MeshBase,
				Prototype_Mesh_SM_DS_Basic_03_3_X_L,
				1.0f,
				0.5f,
				_float4(0.15f, 0.38f, 0.92f, 1),
				_float4(0.15f, 0.38f, 0.92f, 0.0f),
				1,
				_float3(0.1f, 0.5f, 0.1f).XMVector()*0.3f,
				_float3(0.1f, 0.5f, 0.1f).XMVector()*3.0f,
				1);

			// testMesh.fDistortionNoisingPushPower = 0;
			// testMesh.Particle_Power = ;
			testMesh.eParticleTypeID = InstanceEffect_Ball;
			testMesh.eInstanceCount = Prototype_ModelInstance_16;
			testMesh.ePassID = MeshPass_MaskingNoising_Appear_Bright;
			// testMesh.Particle_Power = randpower;
			// testMesh.iMaskingTextureIndex = 64;
			// testMesh.iNoiseTextureIndex = 102;
			// testMesh.vEmissive_SBB = _float3(1,0,0);



			testMesh.ParticleStartRandomPosMin = _float3(-3, 1.0f, -3);
			testMesh.ParticleStartRandomPosMax = _float3(3, 3.0f, 3);
			_Matrix mat = m_pTextureParticleTransform_Demo1->Get_WorldMatrix();
			_Vector pos = mat.r[3];
			testMesh.vFixedPosition = pos;
			// testMesh.FollowingTarget = m_pTextureParticleTransform_Demo1;
			testMesh.vNoisePushingDir = _float2(-1, 0);
			GETPARTICLE->Create_MeshInst_DESC(testMesh, m_eNowSceneNum);


		}

		{
		INSTMESHDESC testMesh = GETPARTICLE->Get_EffectSetting_Mesh(CPartilceCreateMgr::E_MESHINST_EFFECTJ::Um_Mesh_Sword1,
			COMPONENTPROTOTYPEID(0),
			2.0f,
			0.5f,
			_float4(0.15f, 0.38f, 0.92f, 1),
			_float4(0.15f, 0.38f, 0.92f, 0.0f),
			0,
			_float3(0.1f, 0.5f, 0.1f).XMVector()*0.6f,
			_float3(0.1f, 0.5f, 0.1f).XMVector()*0.6f,
			0);
		testMesh.fAppearTimer = 0.3f;

		testMesh.FollowingTarget = m_pTextureParticleTransform_Demo1;
	//	GETPARTICLE->Create_MeshInst_DESC(testMesh, m_eNowSceneNum);

		}
		{
			INSTMESHDESC testMesh = GETPARTICLE->Get_EffectSetting_Mesh(CPartilceCreateMgr::E_MESHINST_EFFECTJ::Um_Mesh_Sword2,
				COMPONENTPROTOTYPEID(0),
				0,
				0,
				_float4(0.15f, 0.38f, 0.92f, 1),
				_float4(0.15f, 0.38f, 0.92f, 0.0f),
				0,
				_float3(0.1f, 0.5f, 0.1f).XMVector()*0.6f,
				_float3(0.1f, 0.5f, 0.1f).XMVector()*0.6f,
				0);

			testMesh.FollowingTarget = m_pTextureParticleTransform_Demo1;
		//	GETPARTICLE->Create_MeshInst_DESC(testMesh, m_eNowSceneNum);
		}
	}

	if (KEYDOWN(DIK_C))
	{

		{
			INSTPARTICLEDESC testTex = GETPARTICLE->Get_EffectSetting_Tex(CPartilceCreateMgr::E_TEXTURE_EFFECTJ::Um_Hit_2_DisDiffuse,
				0,
				0.5f,
				_float4(1),
				_float4(1, 1, 1, 0.5f),
				1,
				_float3(0.05f, 3, 0.05f),
				_float3(0.05f, 4, 0.05f),
				1);
			//	testTex.eParticleTypeID = InstanceEffect_Ball;
			//	testTex.ePassID = InstancePass_OriginColor;
			//	testTex.ePassID = InstancePass_BrightColor;
			//	testTex.vEmissive_SBB = _float3(1,0,0);

			testTex.Particle_Power = 5.0f;


			testTex.ParticleStartRandomPosMin = _float3(-1.5f, 1, -1.5f);
			testTex.ParticleStartRandomPosMax = _float3(1.5f, 3, 1.5f);

			testTex.vPowerDirection = _float3(1, 1, 1);
			testTex.SubPowerRandomRange_RUL = _float3(1, 1, 1);

			testTex.iTextureLayerIndex = 10;
			//	testTex.iMaskingTextureIndex = 74;
			//	testTex.iNoiseTextureIndex = 160;
				testTex.TempBuffer_1.y = 0.0f;

			////	testTex.FollowingTarget = m_pTextureParticleTransform_Demo1;
			_Matrix mat = m_pTextureParticleTransform_Demo1->Get_WorldMatrix();
			_Vector pos = mat.r[3] - mat.r[2] * 3.0f;
			testTex.vFixedPosition = pos;


		//	GETPARTICLE->Create_Texture_Effect_Desc(testTex, m_eNowSceneNum);
		}

		{
			INSTPARTICLEDESC testTex = GETPARTICLE->Get_EffectSetting_Tex(CPartilceCreateMgr::E_TEXTURE_EFFECTJ::Um_Hit_2_DisDiffuse,
				0.5f,
				0.8f,
				_float4(1),
				_float4(1, 1, 1, 0.2f),
				1,
				_float3(0.05f, 2, 0.05f),
				_float3(0.05f, 4, 0.05f),
				1);
			//	testTex.eParticleTypeID = InstanceEffect_Ball;
			//	testTex.ePassID = InstancePass_OriginColor;
			//	testTex.ePassID = InstancePass_BrightColor;
			testTex.eInstanceCount = Prototype_VIBuffer_Point_Instance_32;

			testTex.vEmissive_SBB = _float3(1,1,1);

			testTex.Particle_Power = 5.0f;

			_float val = 0.5f;
			testTex.ParticleStartRandomPosMin = _float3(-val, 1, -val);
			testTex.ParticleStartRandomPosMax = _float3(val, 3, val);

			testTex.vPowerDirection = _float3(1, 1, 1);
			testTex.SubPowerRandomRange_RUL = _float3(1, 1, 1);

			testTex.iTextureLayerIndex = 10;
			testTex.TempBuffer_1.y = 0.0f;

			////	testTex.FollowingTarget = m_pTextureParticleTransform_Demo1;
			_Matrix mat = m_pTextureParticleTransform_Demo1->Get_WorldMatrix();
			_Vector pos = mat.r[3];
			testTex.vFixedPosition = pos;


			//GETPARTICLE->Create_Texture_Effect_Desc(testTex, m_eNowSceneNum);
		}

		{
			INSTPARTICLEDESC testTex = GETPARTICLE->Get_EffectSetting_Tex(
				CPartilceCreateMgr::Um_Spawn4_smoke,
				0.05f,
				0.3f,
				_float4(0.8f),
				_float4(0.0f),
				1,
				_float3(1.5f),
				_float3(0.0f),
				1);
			testTex.iTextureLayerIndex = 10;
			testTex.eParticleTypeID = InstanceEffect_Fountain;
			testTex.eInstanceCount = Prototype_VIBuffer_Point_Instance_16;
			testTex.Particle_Power = 5.0f;


			_float val = 1;
			testTex.ParticleStartRandomPosMin = _float3(-val, 0,  -val);
			testTex.ParticleStartRandomPosMax = _float3(val, 0.5f,val);
			testTex.FollowingTarget = m_pPlayerTransform;
			testTex.iFollowingDir = FollowingDir_Up;
			testTex.vEmissive_SBB = _float3(1, 0.8f, 0.1f);
			testTex.m_fAlphaTestValue = 0.2f;
			testTex.TempBuffer_1.y = 1;

			GETPARTICLE->Create_Texture_Effect_Desc(testTex, m_eNowSceneNum);
		}

	}


	return S_OK;
}

HRESULT CMonster_Ninjasura_Minion::Play_SpawnEffect()
{
	if (m_SpawnEffectAdjust == 0)
	{
		m_SpawnEffectAdjust++;
		{
			INSTPARTICLEDESC testTex = GETPARTICLE->Get_EffectSetting_Tex(CPartilceCreateMgr::E_TEXTURE_EFFECTJ::Um_Hit_2_DisDiffuse,
				1.0f,
				0.6f,
				_float4(1),
				_float4(1, 1, 1, 0.0f),
				1,
				_float3(0.05f, 1, 0.05f),
				_float3(0.05f, 2, 0.05f),
				1);
			//	testTex.eParticleTypeID = InstanceEffect_Ball;
			//	testTex.ePassID = InstancePass_OriginColor;
			//	testTex.ePassID = InstancePass_BrightColor;
			testTex.eInstanceCount = Prototype_VIBuffer_Point_Instance_32;

			testTex.vEmissive_SBB = _float3(1, 1, 1);

			testTex.Particle_Power = 2.f;

			_float val = 0.6f;
			testTex.ParticleStartRandomPosMin = _float3(-val, 0.5f, -val);
			testTex.ParticleStartRandomPosMax = _float3(val, 2, val);

			testTex.vPowerDirection = _float3(1, 1, 1);
			testTex.SubPowerRandomRange_RUL = _float3(1, 1, 1);

			testTex.iTextureLayerIndex = 10;
			testTex.TempBuffer_1.y = 0.0f;

			testTex.FollowingTarget = m_pTransformCom;


		//	GETPARTICLE->Create_Texture_Effect_Desc(testTex, m_eNowSceneNum);
		}
	}


	if (m_SpawnEffectAdjust == 1 && m_SpawnDealytime <= 0.3f)
	{
		// smoke
		m_SpawnEffectAdjust++;
		{
			INSTPARTICLEDESC testTex = GETPARTICLE->Get_EffectSetting_Tex(
				CPartilceCreateMgr::Um_Spawn4_smoke,
				0.05f,
				0.3f,
				_float4(0.8f),
				_float4(0.0f),
				1,
				_float3(1.5f),
				_float3(0.0f),
				1);
			testTex.iTextureLayerIndex = 10;
			testTex.eParticleTypeID = InstanceEffect_Fountain;
			testTex.eInstanceCount = Prototype_VIBuffer_Point_Instance_16;
			testTex.Particle_Power = 5.0f;


			_float val = 1;
			testTex.ParticleStartRandomPosMin = _float3(-val, 0, -val);
			testTex.ParticleStartRandomPosMax = _float3(val, 0.5f, val);
			testTex.FollowingTarget = m_pTransformCom;
			testTex.iFollowingDir = FollowingDir_Up;
			testTex.vEmissive_SBB = _float3(1, 0.8f, 0.1f);
			testTex.m_fAlphaTestValue = 0.2f;
			testTex.TempBuffer_1.y = 1;

			GETPARTICLE->Create_Texture_Effect_Desc(testTex, m_eNowSceneNum);
		}
	}

	return S_OK;
}

HRESULT CMonster_Ninjasura_Minion::PlayAnim(_double dDeltaTime)
{
	SetUp_Fight(dDeltaTime);

	CoolTime_Manager(dDeltaTime);

	if (true == m_bIOnceAnimSwitch)
	{
		Once_AnimMotion(dDeltaTime);
		_uint i = m_pModel->Get_NowAnimIndex();
		switch (i)
		{
			//case 1:
			//	m_pModel->Change_AnimIndex(m_iOnceAnimNumber, 0.f);
			//	break;
			//case 2:
			//	m_pModel->Change_AnimIndex(m_iOnceAnimNumber, 0.f);
			//	break;
		case 11:
			m_pModel->Change_AnimIndex(m_iOnceAnimNumber, 0.f);
			break;
		case 16:
			m_pModel->Change_AnimIndex(m_iOnceAnimNumber, 0.f);
			break;
		case 17:
			m_pModel->Change_AnimIndex(m_iOnceAnimNumber, 0.f);
			break;
		case 21:
			m_pModel->Change_AnimIndex(m_iOnceAnimNumber, 0.f);
			break;
		default:
			m_pModel->Change_AnimIndex(m_iOnceAnimNumber, 0.15f);
			break;
		}
	}
	else
	{
		Infinity_AnimMotion(dDeltaTime);
		m_pModel->Change_AnimIndex(m_iInfinityAnimNumber);
	}

	return S_OK;
}

HRESULT CMonster_Ninjasura_Minion::CoolTime_Manager(_double dDeltaTime)
{
	//한번만 동작하는 애니메이션
	if (m_bStopCoolTimeOn == false)
	{
		m_dOnceCoolTime += dDeltaTime;
		m_dInfinity_CoolTime += dDeltaTime;
	}

	if (m_dOnceCoolTime > 2 || m_bComboAnimSwitch == true)
	{
		m_dOnceCoolTime = 0;
		m_dInfinity_CoolTime = 0;

		if (m_bIOnceAnimSwitch == false)
		{
			Pattern_Change();


			m_bIOnceAnimSwitch = true;
		}
	}

	//반복적으로 동작하는 애니메이션
	if (m_dInfinity_CoolTime >= 1.5)
	{
		m_iInfinityPattern = rand() % 7;


		m_dInfinity_CoolTime = 0;

		m_pTransformCom->LookAtExceptY(m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS));
		m_bMotionTrailOn = false; //!@#!@#@!#!@#!@#!@#!@#@$@!Test 맞을떄 모션 트레일온 false 시켜야함
	}

	return S_OK;
}

HRESULT CMonster_Ninjasura_Minion::Once_AnimMotion(_double dDeltaTime)
{
#ifdef _DEBUG
	// #DEBUG PatternSET
	// m_iOncePattern = 2;

	if (KEYPRESS(DIK_B))
		m_iOncePattern = 0;
#endif // _DEBUG

	switch (m_iOncePattern)
	{
	case 0:
		m_iOnceAnimNumber = 2; //Rush Ready
		m_bComboAnimSwitch = true;
		m_iAfterPattern = m_iOncePattern + 1;
		m_eMonster_State = Anim_State::MONSTER_ATTACK;
		break;
	case 1:
		m_iOnceAnimNumber = 3; //Rush start
		m_bComboAnimSwitch = true;
		m_iAfterPattern = m_iOncePattern + 1;
		m_eMonster_State = Anim_State::MONSTER_ATTACK;
		break;
	case 2:
		m_iOnceAnimNumber = 11; //Attack1 Ready
		m_bComboAnimSwitch = true;
		m_iAfterPattern = m_iOncePattern + 1;
		m_eMonster_State = Anim_State::MONSTER_ATTACK;
		break;
	case 3:
		m_iOnceAnimNumber = 12; //Attack1 Fire
		m_bComboAnimSwitch = false;
		m_iAfterPattern = m_iOncePattern + 1;
		m_eMonster_State = Anim_State::MONSTER_ATTACK;
		break;
	case 4:
		m_iOnceAnimNumber = 2; //Rush Ready
		m_bComboAnimSwitch = true;
		m_iAfterPattern = m_iOncePattern + 1;
		m_eMonster_State = Anim_State::MONSTER_ATTACK;
		break;
	case 5:
		m_iOnceAnimNumber = 3; //Rush start
		m_bComboAnimSwitch = true;
		m_iAfterPattern = m_iOncePattern + 1;
		m_eMonster_State = Anim_State::MONSTER_ATTACK;
		break;
	case 6:
		m_iOnceAnimNumber = 15; //Spinning start
		m_bComboAnimSwitch = true;
		m_iAfterPattern = m_iOncePattern + 1;
		m_eMonster_State = Anim_State::MONSTER_ATTACK;
		break;
	case 7:
		m_iOnceAnimNumber = 16; //Spinning Attack
		m_bComboAnimSwitch = true;
		m_iAfterPattern = m_iOncePattern + 1;
		m_eMonster_State = Anim_State::MONSTER_ATTACK;
		break;
	case 8:
		m_iOnceAnimNumber = 17; //Spinning Attack
		m_bComboAnimSwitch = true;
		m_iAfterPattern = m_iOncePattern + 1;
		m_eMonster_State = Anim_State::MONSTER_ATTACK;
		break;
	case 9:
		m_iOnceAnimNumber = 16; //Spinning Attack
		m_bComboAnimSwitch = true;
		m_iAfterPattern = m_iOncePattern + 1;
		m_eMonster_State = Anim_State::MONSTER_ATTACK;
		break;
	case 10:
		m_iOnceAnimNumber = 18; //Spinning End
		m_bComboAnimSwitch = false;
		m_iAfterPattern = m_iOncePattern + 1;
		m_eMonster_State = Anim_State::MONSTER_ATTACK;
		break;
	case 11:
		m_iOnceAnimNumber = 2; //Rush Ready
		m_bComboAnimSwitch = true;
		m_iAfterPattern = m_iOncePattern + 1;
		m_eMonster_State = Anim_State::MONSTER_ATTACK;
		break;
	case 12:
		m_iOnceAnimNumber = 3; //Rush start
		m_bComboAnimSwitch = true;
		m_iAfterPattern = m_iOncePattern + 1;
		m_eMonster_State = Anim_State::MONSTER_ATTACK;
		break;
	case 15:
		m_iOnceAnimNumber = 13; //Attack2 Ready
		m_bComboAnimSwitch = true;
		m_iAfterPattern = m_iOncePattern + 1;
		m_eMonster_State = Anim_State::MONSTER_ATTACK;
		break;
	case 16:
		m_iOnceAnimNumber = 14; //Attack2 Fire
		m_bComboAnimSwitch = false;
		m_iAfterPattern = m_iOncePattern + 1;
		m_eMonster_State = Anim_State::MONSTER_ATTACK;
		break;
	case 40:
		m_iOnceAnimNumber = 5;
		m_eMonster_State = Anim_State::MONSTER_HIT;
		break;
	case 41:
		m_iOnceAnimNumber = 5;
		m_eMonster_State = Anim_State::MONSTER_HIT;
		break;
	case 42:
		m_iOnceAnimNumber = 7;
		break;
	}

	return S_OK;
}

HRESULT CMonster_Ninjasura_Minion::Pattern_Change()
{

	m_iOncePattern += 1;

	if (m_iOncePattern >= 17)
	{
		if (m_iAfterPattern < 17)
		{
			m_iOncePattern = m_iAfterPattern;
		}
		else {
			m_iOncePattern = 0; //OncePattern Random
			m_iAfterPattern = m_iOncePattern + 1;
		}
	}


	return S_OK;
}

HRESULT CMonster_Ninjasura_Minion::Infinity_AnimMotion(_double dDeltaTime)
{
	switch (m_iInfinityPattern)
	{
	case 0:
		m_iInfinityAnimNumber = 1;
		break;
	case 1:
		m_iInfinityAnimNumber = 1;
		break;
	case 2:
		m_iInfinityAnimNumber = 1;
		break;
	case 3:
		m_iInfinityAnimNumber = 1;
		break;
	case 4:
		m_iInfinityAnimNumber = 1;
		break;
	case 5:
		m_iInfinityAnimNumber = 1;
		break;
	case 6:
		m_iInfinityAnimNumber = 1;
		break;
	case 20:
		m_iInfinityAnimNumber = 0;
		break;
	default:
		m_iInfinityAnimNumber = 1;
		break;
	}

	return S_OK;
}

HRESULT CMonster_Ninjasura_Minion::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_Monster_Ninjasura_Minion), TAG_COM(Com_Model), (CComponent**)&m_pModel));
	FAILED_CHECK(m_pModel->Change_AnimIndex(0));


	CTransform::TRANSFORMDESC tDesc = {};

	tDesc.fMovePerSec = 5;
	tDesc.fRotationPerSec = XMConvertToRadians(60);
	tDesc.vPivot = _float3(0, 0, 0);
	tDesc.fScalingPerSec = 1;

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));

	////////////Motion Test
	CMotionTrail::MOTIONTRAILDESC tMotionDesc;

	tMotionDesc.iNumTrailCount = 1;
	tMotionDesc.pModel = m_pModel;
	tMotionDesc.pShader = m_pShaderCom;
	tMotionDesc.iPassIndex = 5;

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_MotionTrail), TAG_COM(Com_MotionTrail), (CComponent**)&m_pMotionTrail, &tMotionDesc));
	///////////////////////////


	CHpUI::HPDesc HpDesc;
	HpDesc.m_HPType = CHpUI::HP_MONSTER;
	HpDesc.m_pObjcect = this;
	HpDesc.m_vPos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
	HpDesc.m_Dimensions = 1.f;
	m_fMaxHP = 15.f;
	m_fHP = m_fMaxHP;
	g_pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&m_pHPUI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_HpUI), &HpDesc);


	CDissolve::DISSOLVEDESC DissolveDesc;
	DissolveDesc.pModel = m_pModel;
	DissolveDesc.eDissolveModelType = CDissolve::DISSOLVE_ANIM;
	DissolveDesc.pShader = m_pShaderCom;
	DissolveDesc.RampTextureIndex = 5;
	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Dissolve), TAG_COM(Com_Dissolve), (CComponent**)&m_pDissolve, &DissolveDesc));


	SetUp_Collider();

	return S_OK;
}

HRESULT CMonster_Ninjasura_Minion::Adjust_AnimMovedTransform(_double dDeltaTime)
{
	_uint iNowAnimIndex = m_pModel->Get_NowAnimIndex();
	_double PlayRate = m_pModel->Get_PlayRate();

	if (iNowAnimIndex != m_iOldAnimIndex || PlayRate > 0.95)
	{
		m_iAdjMovedIndex = 0;
		m_bLookAtOn = false;
		m_dAcceleration = 1;

		m_bColliderAttackOn = false;

		m_iSoundIndex = 0;
		m_bStopCoolTimeOn = false;
		m_EffectAdjust = 0;

		if (PlayRate > 0.95 && m_bIOnceAnimSwitch == true)
		{
			m_bIOnceAnimSwitch = false;
			if (m_eMonster_State != Anim_State::MONSTER_HIT)
				m_dOnceCoolTime = 0;
			m_dInfinity_CoolTime = 0;
		}
		m_eMonster_State = Anim_State::MONSTER_IDLE;
	}

	if (PlayRate <= 0.95) //애니메이션의 비율 즉, 0.98은 거의 끝나가는 시점
	{
		switch (iNowAnimIndex)
		{
		case 0:
		{
			if (m_iAdjMovedIndex == 0 && m_bMotionTrailOn == false)
			{
			//	m_pMotionTrail->Add_MotionBuffer(m_pTransformCom->Get_WorldFloat4x4(), _float4(1.f, 0.f, 0.f, 1.f), 1.f);
				g_pGameInstance->Play3D_Sound(TEXT("EH_M1_1005.mp3"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_SUBEFFECT, 1.f);
				m_iAdjMovedIndex++;
			}
			break;
		}
		case 1:
		{
			if (m_iAdjMovedIndex == 0 && m_bMotionTrailOn == false)
			{
				m_pMotionTrail->Add_MotionBuffer(m_pTransformCom->Get_WorldFloat4x4(), _float4(1.f, 0.f, 0.f, 1.f), 1.f);
				g_pGameInstance->Play3D_Sound(TEXT("EH_M1_1005.mp3"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_SUBEFFECT, 1.f);
				m_iAdjMovedIndex++;
			}
		}
		case 2:
		{
			_float Value = g_pGameInstance->Easing_Return(TYPE_Linear, TYPE_Linear, 0, 1, (_float)PlayRate, 0.9f);
			Value = max(min(Value, 1.f), 0.f);
			Set_LimLight_N_Emissive(_float4(1.0f, 0.0f, 0.0f, Value), _float4(Value, Value*0.7f, Value, 0.9f));


			if (PlayRate >= 0.9)
			{
				m_bIOnceAnimSwitch = false;
			}
			//if (m_iSoundIndex == 0)
			//{
			//	g_pGameInstance->Play3D_Sound(TEXT("EH_M1_1005.mp3"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_SUBEFFECT, 1.f);

			//	m_iSoundIndex++;
			//}
			break;
		}
		case 3:
		{
			_float Value = g_pGameInstance->Easing_Return(TYPE_Linear, TYPE_Linear, 0, 1, (_float)PlayRate, 0.9f);
			Value = max(min(Value, 1.f), 0.f);
			Set_LimLight_N_Emissive(_float4(0.0f, 0.0f, 1.0f, Value), _float4(Value, Value*0.7f, Value, 0.9f));


			m_bLookAtOn = false;
			if (m_iAdjMovedIndex == 0)
			{
				m_pTransformCom->LookAtExceptY(m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS));

				m_bColliderAttackOn = true;


				g_pGameInstance->Play3D_Sound(TEXT("EH_Ninjasura_DashSwoosh_01.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_SUBEFFECT, 0.5f);
				g_pGameInstance->Play3D_Sound(TEXT("EH_Ninja_Blade.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_SUBEFFECT, 1.f);
				g_pGameInstance->Play3D_Sound(TEXT("EH_M1_1005.mp3"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_SUBEFFECT, 1.f);

				m_iAdjMovedIndex++;
			}

			if (PlayRate > 0 && PlayRate <= 0.1)
			{
				m_bMotionTrailOn = true;
				m_pMotionTrail->Add_MotionBuffer(m_pTransformCom->Get_WorldFloat4x4(), _float4(1.f, 0.f, 0.f, 1.f), 1.f);
			}
			else if (PlayRate >= 0.1 && PlayRate <= 0.8)
			{
				m_bMotionTrailOn = false;
				m_pTransformCom->Move_Forward(dDeltaTime * 5,m_pNavigationCom);
			}
			else if (PlayRate >= 0.8)
			{
				m_bMotionTrailOn = true;
				m_pMotionTrail->Add_MotionBuffer(m_pTransformCom->Get_WorldFloat4x4(), _float4(1.f, 0.f, 0.f, 1.f), 1.f);

				if (m_iSoundIndex == 0)
				{
					g_pGameInstance->Play3D_Sound(TEXT("EH_M1_1005.mp3"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_SUBEFFECT, 1.f);
					m_iSoundIndex++;
				}
			}

			if (m_EffectAdjust == 0 && PlayRate >= 0.9)
			// if (m_EffectAdjust == 0 && PlayRate >= 0.6)
			{
				Effect_Knife();

				m_EffectAdjust++;
			}
			break;
		}
		case 5:
		{
			if (m_iAdjMovedIndex == 0 && PlayRate > 0 && m_bKnockbackOn == false)
			{
				m_dAcceleration = 0.7;
				m_iAdjMovedIndex++;

				if (m_iSoundIndex == 0)
				{
					g_pGameInstance->Play3D_Sound(TEXT("EH_Ninja_Hit_Vox_03.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_MONSTER, 0.5f);
					m_iSoundIndex++;
				}
			}
			if (m_bKnockbackOn == true)
			{
				if (m_iAdjMovedIndex == 0 && PlayRate > 0)
				{
					m_bLookAtOn = false;
					m_dAcceleration = 0.7;
					m_iAdjMovedIndex++;


					if (m_iSoundIndex == 0)
					{
						g_pGameInstance->Play3D_Sound(TEXT("EH_Ninja_Grunt_Vox_03.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_MONSTER, 0.5f);
						m_iSoundIndex++;
					}
				}
				else if (0.f < PlayRate && PlayRate <= 0.6428)
				{
					if (PlayRate >= 0 && PlayRate <= 0.6428)
						m_pTransformCom->Move_Backward(dDeltaTime* 0.5, m_pNavigationCom);

					m_fKnockbackDir.y = 0;

					m_pTransformCom->Turn_Dir(m_fKnockbackDir.XMVector(), 0.9f);
				}
			}
			break;
		}
		case 7:
		{
			m_bLookAtOn = false;
			break;
		}
		case 11:
		{
			if (m_iAdjMovedIndex == 0 && PlayRate >0)
			{
				m_bMotionTrailOn = true;
				m_bColliderAttackOn = true;

				m_iAdjMovedIndex++;
			}

			if (PlayRate >= 0.85)
			{
				m_pTransformCom->LookAtExceptY(m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS));
				m_pMotionTrail->Add_MotionBuffer(m_pTransformCom->Get_WorldFloat4x4(), _float4(1.f, 0.f, 0.f, 1.f), 1.f);
			
			}
			break;
		}
		case 12:
		{
			m_bLookAtOn = false;
			
	
			if (PlayRate>0 && PlayRate <= 0.4)
			{
				if (m_iAdjMovedIndex == 0)
				{
					m_bMotionTrailOn = false;
					m_pMotionTrail->Add_MotionBuffer(m_pTransformCom->Get_WorldFloat4x4(), _float4(1.f, 0.f, 0.f, 1.f), 1.f);

					m_bColliderAttackOn = true;
					m_iAdjMovedIndex++;
				}
				m_pTransformCom->Move_Forward(dDeltaTime * 5, m_pNavigationCom);

			}
			else if (m_iAdjMovedIndex == 1 && PlayRate >= 0.4)
			{
				m_bMotionTrailOn = true;
				m_pMotionTrail->Add_MotionBuffer(m_pTransformCom->Get_WorldFloat4x4(), _float4(1.f, 0.f, 0.f, 1.f), 1.f);
				g_pGameInstance->Play3D_Sound(TEXT("EH_M1_1005.mp3"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_SUBEFFECT, 1.f);
				m_iAdjMovedIndex++;
			}
			else if (PlayRate >= 0.8)
			{
				if (m_iAdjMovedIndex == 2)
				{
					_Vector PlayerPos = m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS);
					_Vector PlayerNaviPos = m_pPlayerNavigation->Get_IndexPosition(m_pPlayerNavigation->Get_CurNavCellIndex());

					CUtilityMgr* pUtil = GetSingle(CUtilityMgr);

					//랜덤방향
					_Vector vDir = XMVector3Normalize((m_pPlayerTransform->Get_MatrixState(CTransform::STATE_LOOK) * pUtil->RandomFloat(-1, 1) + m_pPlayerTransform->Get_MatrixState(CTransform::STATE_RIGHT) * pUtil->RandomFloat(-1, 1)));

					XMStoreFloat3(&m_fDirection, vDir);

					//플레이어와 몬스터가 겹친 상태에서 LookAt을 때리면 스케일이 0이 되버리기 때문에 예외처리함
					m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, PlayerPos -(XMVector3Normalize(PlayerNaviPos - PlayerPos) *0.1f));
					m_iAdjMovedIndex++;
				}
				m_pTransformCom->MovetoDir_bySpeed(XMLoadFloat3(&m_fDirection), 50, dDeltaTime, m_pNavigationCom);

			}
			//else if (m_iAdjMovedIndex == 2 && PlayRate >= 0.8)
			//{
			//	//플레이어 주변 랜덤 생성
			//	_Vector PlayerPos = m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS);

			//	CUtilityMgr* pUtil = GetSingle(CUtilityMgr);

			//	//랜덤방향
			//	_Vector vDir = XMVector3Normalize((m_pPlayerTransform->Get_MatrixState(CTransform::STATE_LOOK) * pUtil->RandomFloat(-1, 1) + m_pPlayerTransform->Get_MatrixState(CTransform::STATE_RIGHT) * pUtil->RandomFloat(-1, 1)));
			//
			//	PlayerPos = PlayerPos + (vDir * 4);

			//	m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, PlayerPos);
			//	m_iAdjMovedIndex++;
			//
			//}
			if (m_iSoundIndex == 0)
			{
				g_pGameInstance->Play3D_Sound(TEXT("EH_Ninjasura_DashSwoosh_01.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_SUBEFFECT, 0.5f);
				g_pGameInstance->Play3D_Sound(TEXT("EH_Ninjasura_Sword_Sheath_01.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_SUBEFFECT, 1.f);
				m_iSoundIndex++;
			}

			if (m_EffectAdjust == 0 && PlayRate >= 0.5)
			//if (m_EffectAdjust == 0 && PlayRate >= 0.2)
			{

				Effect_Knife();
				m_EffectAdjust++;
			}
			break;
		}
		case 13:
		{
			if (m_iAdjMovedIndex == 0 && PlayRate > 0)
			{
				m_bMotionTrailOn = true;
				m_bColliderAttackOn = true;
				m_iAdjMovedIndex++;
			}

			if (PlayRate >= 0.66)
			{
				m_pTransformCom->LookAtExceptY(m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS));
				m_pMotionTrail->Add_MotionBuffer(m_pTransformCom->Get_WorldFloat4x4(), _float4(1.f, 0.f, 0.f, 1.f), 1.f);
			}
			break;
		}
		case 14:
		{
			m_bLookAtOn = false;


			if (PlayRate > 0 && PlayRate <= 0.4)
			{
				if (m_iAdjMovedIndex == 0)
				{
					m_bMotionTrailOn = false;
					m_pMotionTrail->Add_MotionBuffer(m_pTransformCom->Get_WorldFloat4x4(), _float4(1.f, 0.f, 0.f, 1.f), 1.f);

					m_bColliderAttackOn = true;
					m_iAdjMovedIndex++;
				}
				m_pTransformCom->Move_Forward(dDeltaTime * 5, m_pNavigationCom);
			}
			else if (m_iAdjMovedIndex == 1 && PlayRate >= 0.4)
			{
				m_bMotionTrailOn = true;
				m_pMotionTrail->Add_MotionBuffer(m_pTransformCom->Get_WorldFloat4x4(), _float4(1.f, 0.f, 0.f, 1.f), 1.f);
				m_iAdjMovedIndex++;
			}
			else if (PlayRate >= 0.8)
			{
				if (m_iAdjMovedIndex == 2)
				{
					_Vector PlayerPos = m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS);

					CUtilityMgr* pUtil = GetSingle(CUtilityMgr);

					//랜덤방향
					_Vector vDir = XMVector3Normalize((m_pPlayerTransform->Get_MatrixState(CTransform::STATE_LOOK) * pUtil->RandomFloat(-1, 1) + m_pPlayerTransform->Get_MatrixState(CTransform::STATE_RIGHT) * pUtil->RandomFloat(-1, 1)));

					XMStoreFloat3(&m_fDirection, vDir);

					//플레이어와 몬스터가 겹친 상태에서 LookAt을 때리면 스케일이 0이 되버리기 때문에 예외처리함
					_Vector PlayerNaviPos = m_pPlayerNavigation->Get_IndexPosition(m_pPlayerNavigation->Get_CurNavCellIndex());
					m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, PlayerPos - (XMVector3Normalize(PlayerNaviPos - PlayerPos) *0.1f));
					//m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, PlayerPos );
					m_iAdjMovedIndex++;
				}
				m_pTransformCom->MovetoDir_bySpeed(XMLoadFloat3(&m_fDirection), 50, dDeltaTime, m_pNavigationCom);

			}
			if (m_iSoundIndex == 0)
			{
				g_pGameInstance->Play3D_Sound(TEXT("EH_Ninjasura_DashSwoosh_01.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_SUBEFFECT, 0.5f);
				g_pGameInstance->Play3D_Sound(TEXT("EH_Ninjasura_Sword_Sheath_01.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_SUBEFFECT, 1.f);
				g_pGameInstance->Play3D_Sound(TEXT("EH_M1_1005.mp3"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_SUBEFFECT, 1.f);
				m_iSoundIndex++;
			}
			else if (m_iSoundIndex == 1 && PlayRate >= 0.4)
			{
				g_pGameInstance->Play3D_Sound(TEXT("EH_M1_1005.mp3"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_SUBEFFECT, 1.f);
				m_iSoundIndex++;
			}

			if (m_EffectAdjust == 0 && PlayRate >= 0.5)
			// if (m_EffectAdjust == 0 && PlayRate >= 0.2)
			{
				Effect_Knife();

				m_EffectAdjust++;
			}

			break;
		}
		case 15:
		{
			if (m_iAdjMovedIndex == 0 && PlayRate > 0)
			{
				m_bMotionTrailOn = true;
				m_bColliderAttackOn = true;

				m_pTransformCom->LookAtExceptY(m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS));

				_Vector PlayerPos = m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS);

				CUtilityMgr* pUtil = GetSingle(CUtilityMgr);

				//랜덤방향
				_Vector vDir = XMVector3Normalize((m_pPlayerTransform->Get_MatrixState(CTransform::STATE_LOOK) * pUtil->RandomFloat(-1, 1) + m_pPlayerTransform->Get_MatrixState(CTransform::STATE_RIGHT) * pUtil->RandomFloat(-1, 1)));

				XMStoreFloat3(&m_fDirection, vDir);

				//플레이어와 몬스터가 겹친 상태에서 LookAt을 때리면 스케일이 0이 되버리기 때문에 예외처리함
				_Vector PlayerNaviPos = m_pPlayerNavigation->Get_IndexPosition(m_pPlayerNavigation->Get_CurNavCellIndex());
				m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, PlayerPos - (XMVector3Normalize(PlayerNaviPos - PlayerPos) *0.1f));
				//m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, PlayerPos);

				m_pTransformCom->MovetoDir_bySpeed(XMLoadFloat3(&m_fDirection), 250, dDeltaTime, m_pNavigationCom);

				g_pGameInstance->Play3D_Sound(TEXT("EH_M1_1005.mp3"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_SUBEFFECT, 1.f);

				m_iAdjMovedIndex++;
			}
			else if (m_iAdjMovedIndex == 1 && PlayRate > 0)
			{
				m_bMotionTrailOn = true;
				m_pMotionTrail->Add_MotionBuffer(m_pTransformCom->Get_WorldFloat4x4(), _float4(1.f, 0.f, 0.f, 1.f), 1.f);
				m_iAdjMovedIndex++;
			}
			else if (m_iAdjMovedIndex == 2 && PlayRate > 0.1)
			{
				m_bMotionTrailOn = false;

				m_iAdjMovedIndex++;
			}
			break;
		}
		case 16:
		{
			_float Value = g_pGameInstance->Easing_Return(TYPE_Linear, TYPE_Linear, 0, 1, (_float)PlayRate, 0.9f);
			Value = max(min(Value, 1.f), 0.f);
			Set_LimLight_N_Emissive(_float4(1.0f, 0.0f, 0.0f, Value), _float4(Value, Value*0.7f, Value, 0.9f));

			m_bColliderAttackOn = true;

			m_bMotionTrailOn = false;
			_Vector vTarget = XMVector3Normalize(m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS) - m_pTransformCom->Get_MatrixState(CTransform::STATE_POS));
			m_pTransformCom->Turn_Dir(vTarget, 0.7f);
			
			if (PlayRate > 0 && PlayRate <= 0.95)
			{
				_float fSpeed = g_pGameInstance->Easing_Return(TYPE_SinInOut, TYPE_SinInOut, 0, 2.852698f, (_float)PlayRate, 0.95f); // PlayRate - 0.266666 and 0.5 - 0.266666
				m_pTransformCom->Move_Forward(dDeltaTime * fSpeed, m_pNavigationCom);
				m_dAcceleration = 0.8;
			}
			if (m_iSoundIndex == 0)
			{
				g_pGameInstance->Play3D_Sound(TEXT("EH_Ninjasura_Spin_Attack_03.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_SUBEFFECT, 1.f);
				m_iSoundIndex++;
			}
			if (m_EffectAdjust == 0)
			{
				Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_MONSTER_NM_Cash3, m_pTextureParticleTransform_Demo2);
				m_EffectAdjust++;
			}

			break;
		}
		case 17:
		{
			_float Value = g_pGameInstance->Easing_Return(TYPE_Linear, TYPE_Linear, 0, 1, (_float)PlayRate, 0.9f);
			Value = max(min(Value, 1.f), 0.f);
			Set_LimLight_N_Emissive(_float4(0.0f, 0.0f, 1.0f, Value), _float4(Value, Value*0.7f, Value, 0.9f));


			m_bColliderAttackOn = true;

			m_bMotionTrailOn = false;
			_Vector vTarget = XMVector3Normalize(m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS) - m_pTransformCom->Get_MatrixState(CTransform::STATE_POS));
			m_pTransformCom->Turn_Dir(vTarget, 0.7f);
			if (PlayRate > 0 && PlayRate <= 0.95)
			{
				_float fSpeed = g_pGameInstance->Easing_Return(TYPE_SinInOut, TYPE_SinInOut, 0, 2.852698f, (_float)PlayRate, 0.95f); // PlayRate - 0.266666 and 0.5 - 0.266666
				m_pTransformCom->Move_Forward(dDeltaTime * fSpeed, m_pNavigationCom);
				m_dAcceleration = 0.8;
			}
			if (m_iSoundIndex == 0)
			{
				g_pGameInstance->Play3D_Sound(TEXT("EH_Ninjasura_Spin_Attack_03.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_SUBEFFECT, 1.f);
				m_iSoundIndex++;
			}

			if (m_EffectAdjust == 0)
			{
				Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_MONSTER_NM_Cash3, m_pTextureParticleTransform_Demo2);
				m_EffectAdjust++;
			}
			break;
		}
		case 18:
		{
			if (m_iAdjMovedIndex == 0 && PlayRate >= 0.75)
			{

				m_bMotionTrailOn = true;
				m_pMotionTrail->Add_MotionBuffer(m_pTransformCom->Get_WorldFloat4x4(), _float4(1.f, 0.f, 0.f, 1.f), 1.f);

				m_iAdjMovedIndex++;

			}else if (PlayRate >= 0.8)
			{
				if (m_iAdjMovedIndex == 2)
				{
					_Vector PlayerPos = m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS);

					CUtilityMgr* pUtil = GetSingle(CUtilityMgr);

					//랜덤방향
					_Vector vDir = XMVector3Normalize((m_pPlayerTransform->Get_MatrixState(CTransform::STATE_LOOK) * pUtil->RandomFloat(-1, 1) + m_pPlayerTransform->Get_MatrixState(CTransform::STATE_RIGHT) * pUtil->RandomFloat(-1, 1)));

					XMStoreFloat3(&m_fDirection, vDir);

					//플레이어와 몬스터가 겹친 상태에서 LookAt을 때리면 스케일이 0이 되버리기 때문에 예외처리함
					_Vector PlayerNaviPos = m_pPlayerNavigation->Get_IndexPosition(m_pPlayerNavigation->Get_CurNavCellIndex());
					m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, PlayerPos - (XMVector3Normalize(PlayerNaviPos - PlayerPos) *0.1f));
					//m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, PlayerPos);
					m_iAdjMovedIndex++;
				}
				m_pTransformCom->MovetoDir_bySpeed(XMLoadFloat3(&m_fDirection), 25, dDeltaTime, m_pNavigationCom);

				if (m_iSoundIndex == 1)
				{
					g_pGameInstance->Play3D_Sound(TEXT("EH_M1_1005.mp3"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_SUBEFFECT, 1.f);

					m_iSoundIndex++;
				}
			}
			if (m_iSoundIndex == 0)
			{
				g_pGameInstance->Play3D_Sound(TEXT("EH_Ninjasura_Spin_Attack_03.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_SUBEFFECT, 1.f);
				m_iSoundIndex++;
			}
			break;
		}
		}

	}

	m_iOldAnimIndex = iNowAnimIndex;
	return S_OK;
}

void CMonster_Ninjasura_Minion::Effect_Knife()
{
	Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_MONSTER_NM_Cash4, m_pTextureParticleTransform_Demo1);

	{
		INSTMESHDESC testMesh = GETPARTICLE->Get_EffectSetting_Mesh(CPartilceCreateMgr::E_MESHINST_EFFECTJ::Um_MeshBase,
			Prototype_Mesh_SM_DS_Basic_03_3_X_L,
			0.4f,
			0.2f,
			_float4(0.15f, 0.38f, 0.92f, 1),
			_float4(0.15f, 0.38f, 0.92f, 0.0f),
			1,
			_float3(0.1f, 0.5f, 0.1f).XMVector()*0.3f,
			_float3(0.1f, 0.5f, 0.1f).XMVector()*3.0f,
			1);

		// testMesh.fDistortionNoisingPushPower = 0;
		// testMesh.Particle_Power = ;
		testMesh.eParticleTypeID = InstanceEffect_Ball;
		testMesh.eInstanceCount = Prototype_ModelInstance_16;
		testMesh.ePassID = MeshPass_MaskingNoising_Appear_Bright;
		// testMesh.Particle_Power = randpower;
		// testMesh.iMaskingTextureIndex = 64;
		// testMesh.iNoiseTextureIndex = 102;
		// testMesh.vEmissive_SBB = _float3(1,0,0);



		testMesh.ParticleStartRandomPosMin = _float3(-2.5f, 1, -2.5f);
		testMesh.ParticleStartRandomPosMax = _float3(2.5f, 4, 2.5f);

		// TRANS 
	//	testMesh.FollowingTarget = m_pTransformCom;
	//	testMesh.TempBuffer_0.z = 0;

		// FIXPOS
		_Matrix mat = m_pTextureParticleTransform_Demo1->Get_WorldMatrix();
		_Vector pos = mat.r[3] - mat.r[2] * 3.0f;
		testMesh.vFixedPosition = pos;


		// testMesh.FollowingTarget = m_pTextureParticleTransform_Demo1;
		testMesh.vNoisePushingDir = _float2(-1, 0);
		GETPARTICLE->Create_MeshInst_DESC(testMesh, m_eNowSceneNum);
	}
	{
		INSTPARTICLEDESC testTex = GETPARTICLE->Get_EffectSetting_Tex(CPartilceCreateMgr::E_TEXTURE_EFFECTJ::Um_Hit_2_DisDiffuse,
			0,
			0.5f,
			_float4(1),
			_float4(1, 1, 1, 0.5f),
			1,
			_float3(0.05f, 3, 0.05f),
			_float3(0.05f, 4, 0.05f),
			1);
		//	testTex.eParticleTypeID = InstanceEffect_Ball;
		//	testTex.ePassID = InstancePass_OriginColor;
		//	testTex.ePassID = InstancePass_BrightColor;
		//	testTex.vEmissive_SBB = _float3(1,0,0);

		testTex.Particle_Power = 5.0f;

		testTex.eInstanceCount = Prototype_VIBuffer_Point_Instance_16;

		testTex.ParticleStartRandomPosMin = _float3(-1.5f, 1, -1.5f);
		testTex.ParticleStartRandomPosMax = _float3(1.5f, 3, 1.5f);

		testTex.vPowerDirection = _float3(1, 1, 1);
		testTex.SubPowerRandomRange_RUL = _float3(1, 1, 1);

		testTex.iTextureLayerIndex = 10;
		//	testTex.iMaskingTextureIndex = 74;
		//	testTex.iNoiseTextureIndex = 160;

		// TRANS 
	//	testTex.FollowingTarget = m_pTransformCom;
	//	testTex.TempBuffer_1.y = 0.0f;

		_Matrix mat = m_pTextureParticleTransform_Demo1->Get_WorldMatrix();
		_Vector pos = mat.r[3] - mat.r[2] * 3.0f;
		testTex.vFixedPosition = pos;

		GETPARTICLE->Create_Texture_Effect_Desc(testTex, m_eNowSceneNum);

	}

}

CMonster_Ninjasura_Minion * CMonster_Ninjasura_Minion::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CMonster_Ninjasura_Minion*	pInstance = NEW CMonster_Ninjasura_Minion(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CMonster_Ninjasura_Minion");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMonster_Ninjasura_Minion::Clone(void * pArg)
{
	CMonster_Ninjasura_Minion*	pInstance = NEW CMonster_Ninjasura_Minion(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CMonster_Ninjasura_Minion");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMonster_Ninjasura_Minion::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModel);
	////////////Motion Test
	Safe_Release(m_pMotionTrail);
	////////////
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pAttackColliderCom);
	Safe_Release(m_pHPUI);
	Safe_Release(m_pDissolve);

	Safe_Release(m_pTextureParticleTransform_Demo1);
	Safe_Release(m_pTextureParticleTransform_Demo2);
	Safe_Release(m_pTextureParticleTransform_Demo3);
	Safe_Release(m_pTextureParticleTransform_Demo4);
}