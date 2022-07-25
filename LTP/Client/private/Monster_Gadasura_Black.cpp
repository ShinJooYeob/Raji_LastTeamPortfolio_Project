#include "stdafx.h"
#include "..\public\Monster_Gadasura_Black.h"
#include "Monster_Bullet_Universal.h"
#include "Monster_Weapon_Universal.h"
#include "Monster_Texture_Bullet.h"
#include "HpUI.h"
#include "Monster_Weapon_Universal.h"

CMonster_Gadasura_Black::CMonster_Gadasura_Black(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CMonster(pDevice, pDeviceContext)
{
}

CMonster_Gadasura_Black::CMonster_Gadasura_Black(const CMonster_Gadasura_Black & rhs)
	: CMonster(rhs)
{
}

HRESULT CMonster_Gadasura_Black::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));


	return S_OK;
}

HRESULT CMonster_Gadasura_Black::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components());

	if (pArg != nullptr)
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, *((_float3*)pArg));


	SetUp_Info();

	SetUp_Weapon();



	//////////////////testPosition
	m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, _float3(216.357f, 29.2f, 188.583f));

	m_pNavigationCom->FindCellIndex(m_pTransformCom->Get_MatrixState(CTransform::STATE_POS));

	m_pTransformCom->LookAtExceptY(m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS), 1);
	//////////////////////////////
	return S_OK;
}

_int CMonster_Gadasura_Black::Update(_double dDeltaTime)
{
	if (__super::Update(dDeltaTime) < 0)return -1;

	PlayAnim(dDeltaTime);

	m_bIsOnScreen = g_pGameInstance->IsNeedToRender(m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS), m_fFrustumRadius);
	FAILED_CHECK(m_pModel->Update_AnimationClip(dDeltaTime * m_dAcceleration, m_bIsOnScreen));
	FAILED_CHECK(Adjust_AnimMovedTransform(dDeltaTime));

	if (m_pHPUI != nullptr)
		m_pHPUI->Update(dDeltaTime);

	if (m_pWeapon != nullptr)
		m_pWeapon->Update(dDeltaTime);

	Update_Collider(dDeltaTime);

	return _int();
}

_int CMonster_Gadasura_Black::LateUpdate(_double dDeltaTime)
{
	if (__super::LateUpdate(dDeltaTime) < 0)return -1;

	//////////
	if (m_bIsOnScreen)
	{
		FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
	}

	FAILED_CHECK(m_pRendererCom->Add_ShadowGroup(CRenderer::SHADOW_ANIMMODEL, this, m_pTransformCom, m_pShaderCom, m_pModel));
	m_vOldPos = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS);

#ifdef _DEBUG
	FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pColliderCom));
#endif

	m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, m_pNavigationCom->Get_NaviPosition(m_pTransformCom->Get_MatrixState(CTransform::STATE_POS)));


	if (m_pHPUI != nullptr)
		m_pHPUI->LateUpdate(dDeltaTime);

	if (m_pWeapon != nullptr)
		m_pWeapon->LateUpdate(dDeltaTime);
	return _int();
}

_int CMonster_Gadasura_Black::Render()
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

_int CMonster_Gadasura_Black::LateRender()
{
	if (__super::LateRender() < 0)
		return -1;

	return _int();
}

void CMonster_Gadasura_Black::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
	if (CollisionTypeID::CollisionType_Player == eConflictedObjCollisionType)
	{
		_Vector vDamageDir = XMVector3Normalize(pConflictedCollider->Get_ColliderPosition(iConflictedObjColliderIndex).XMVector() - m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS));
		pConflictedObj->Take_Damage(this, 1.f, vDamageDir, m_bOnKnockbackCol, m_fKnockbackColPower);
		pConflictedCollider->Set_Conflicted(1.f);
	}
}

_float CMonster_Gadasura_Black::Take_Damage(CGameObject * pTargetObject, _float fDamageAmount, _fVector vDamageDir, _bool bKnockback, _float fKnockbackPower)
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

HRESULT CMonster_Gadasura_Black::SetUp_Info()
{

	m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, _float3(2.f, 0.f, 2.f));


	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	m_pPlayerTransform = static_cast<CTransform*>(pGameInstance->Get_Commponent_By_LayerIndex(m_eNowSceneNum, TAG_LAY(Layer_Player), TAG_COM(Com_Transform)));

	RELEASE_INSTANCE(CGameInstance);


	return S_OK;
}

HRESULT CMonster_Gadasura_Black::SetUp_Weapon()
{
	CMonster_Weapon_Universal::Monster_Weapon_UniversalDesc MonsterWeaponDesc;
	MonsterWeaponDesc.iMonsterWeaponMeshNumber = CMonster_Weapon_Universal::GADASURA_BLACK_WEAPON;
	MonsterWeaponDesc.Object = this;
	MonsterWeaponDesc.eAttachedDesc.Initialize_AttachedDesc(this, "skd_r_wrist", _float3(1.f, 1.f, 1.f), _float3(0, 0, 0), _float3(-2.535f, -0.075f, -3.03f));

	g_pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&m_pWeapon), m_eNowSceneNum, TAG_OP(Prototype_Object_Monster_Weapon_Universal), &MonsterWeaponDesc);
	return S_OK;
}

HRESULT CMonster_Gadasura_Black::SetUp_Collider()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_Collider), (CComponent**)&m_pColliderCom));

	/////////////////m_pColliderCom!@!@#$@!#$@#$@$!@%#$%@#$%%^^W@!
	COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(6.f, 6.f, 6.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	ATTACHEDESC tAttachedDesc;
	tAttachedDesc.Initialize_AttachedDesc(this, "skd_spine_01", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(-0.00047f, -0.027856f, -1.8269f));
	m_vecAttachedDesc.push_back(tAttachedDesc);


	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "skd_head", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(0.f, -0.13306f, -2.6351f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pColliderCom->Set_ParantBuffer();


	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(2.f, 2.f, 2.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "skd_spine_03", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(0.f, -0.030646f, -2.3865f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pColliderCom->Set_ParantBuffer();


	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(2.f, 2.f, 2.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "skd_pelvis", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(0.f, -0.004822f, -1.4863f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pColliderCom->Set_ParantBuffer();


	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "skd_r_knee", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(-0.30817f, -0.017663f, -0.88963f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pColliderCom->Set_ParantBuffer();


	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "skd_r_ankle", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(-0.30817f, 0.16343f, -0.15622f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pColliderCom->Set_ParantBuffer();


	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "skd_l_knee", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(0.30817f, -0.017663f, -0.88963f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pColliderCom->Set_ParantBuffer();


	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "skd_l_ankle", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(0.30817f, 0.16343f, -0.15622f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pColliderCom->Set_ParantBuffer();



	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "skd_r_arm", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(-0.47793f, 0.16684f, -2.5459f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pColliderCom->Set_ParantBuffer();



	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.5f, 1.5f, 1.5f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "skd_r_elbow", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(-0.96232f, 0.20043f, -2.3494f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pColliderCom->Set_ParantBuffer();



	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "skd_l_arm", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(0.47793f, 0.16684f, -2.5459f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pColliderCom->Set_ParantBuffer();


	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.5f, 1.5f, 1.5f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "skd_l_elbow", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(0.96232f, 0.20043f, -2.3494f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pColliderCom->Set_ParantBuffer();


	return S_OK;
}

HRESULT CMonster_Gadasura_Black::SetUp_Fight(_double dDeltaTime)
{
	m_fDistance = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS).Get_Distance(m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS));

	if (m_bLookAtOn)
	{
		//m_pTransformCom->LookAt(m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS));


		_Vector vTarget = XMVector3Normalize(m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS) - m_pTransformCom->Get_MatrixState(CTransform::STATE_POS));

		//m_pTransformCom->Turn_Dir(XMVector3Normalize(m_pTransformCom->Get_MatrixScale(CTransform::STATE_LOOK)*0.9 + vTarget* 0.1));
		m_pTransformCom->Turn_Dir(vTarget, 0.9f);
	}



	return S_OK;
}

HRESULT CMonster_Gadasura_Black::Update_Collider(_double dDeltaTime)
{
	m_pColliderCom->Update_ConflictPassedTime(dDeltaTime);

	//Collider
	_uint	iNumCollider = m_pColliderCom->Get_NumColliderBuffer();
	for (_uint i = 0; i < iNumCollider; i++)
		m_pColliderCom->Update_Transform(i, m_vecAttachedDesc[i].Caculate_AttachedBoneMatrix_BlenderFixed());

	FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_Monster, this, m_pColliderCom));
	FAILED_CHECK(g_pGameInstance->Add_RepelGroup(m_pTransformCom, 1.5f, m_pNavigationCom));

	return S_OK;
}

HRESULT CMonster_Gadasura_Black::PlayAnim(_double dDeltaTime)
{
	SetUp_Fight(dDeltaTime);

	CoolTime_Manager(dDeltaTime);

	if (true == m_bIOnceAnimSwitch)
	{
		Once_AnimMotion(dDeltaTime);
		_uint i = m_pModel->Get_NowAnimIndex();
		switch (i)
		{
		case 6:
			m_pModel->Change_AnimIndex(m_iOnceAnimNumber, 0.2f);
			break;
		case 14:
			m_pModel->Change_AnimIndex(m_iOnceAnimNumber, 0.f);
			break;
		case 15:
			m_pModel->Change_AnimIndex(m_iOnceAnimNumber, 0.f);
			break;
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

HRESULT CMonster_Gadasura_Black::CoolTime_Manager(_double dDeltaTime)
{
	//한번만 동작하는 애니메이션

	m_dOnceCoolTime += dDeltaTime;
	m_dSpecial_CoolTime += dDeltaTime;

	if (m_dOnceCoolTime > 2 && m_fDistance < 4 || m_bComboAnimSwitch == true)
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
	m_dInfinity_CoolTime += dDeltaTime;
	if (m_dInfinity_CoolTime >= 1.5)
	{
		m_iInfinityPattern = rand() % 6;


		m_dInfinity_CoolTime = 0;
	}
	if (m_bComboAnimSwitch == false && m_bIOnceAnimSwitch == false)
	{
		Special_Trigger(dDeltaTime);
	}
	return S_OK;
}

HRESULT CMonster_Gadasura_Black::Once_AnimMotion(_double dDeltaTime)
{
	// #DEBUG PatternSET
	 m_iOncePattern = 0;

	switch (m_iOncePattern)
	{
	case 0:
		m_iOnceAnimNumber = 17; //Attack1 
		m_bComboAnimSwitch = true;
		break;
	case 1:
		m_iOnceAnimNumber = 3; //Right Move
		m_bComboAnimSwitch = false;
		break;
	case 2:
		m_iOnceAnimNumber = 19; //smash Attack
		m_bComboAnimSwitch = false;
		break;
	case 3:
		m_iOnceAnimNumber = 25; //Rage Run
		m_bComboAnimSwitch = true;
		break;
	case 4:
		m_iOnceAnimNumber = 14; //Rage Run
		m_bComboAnimSwitch = true;
		break;
	case 5:
		m_iOnceAnimNumber = 21; //Rage Run Attack
		m_bComboAnimSwitch = false;
		break;
	case 6:
		m_iOnceAnimNumber = 19; //smash Attack
		m_bComboAnimSwitch = false;
		break;
	case 7:
		m_iOnceAnimNumber = 18; //Attack2
		m_bComboAnimSwitch = true;
		break;
	case 8:
		m_iOnceAnimNumber = 4; //Left Move
		m_bComboAnimSwitch = false;
		break;
	case 9:
		m_iOnceAnimNumber = 25; //Rage Run
		m_bComboAnimSwitch = true;
		break;
	case 10:
		m_iOnceAnimNumber = 15; //Rage Run
		m_bComboAnimSwitch = true;
		break;
	case 11:
		m_iOnceAnimNumber = 21; //Rage Run Attack
		m_bComboAnimSwitch = false;
		break;
	case 12:
		m_iOnceAnimNumber = 22; //Taunt
		m_bComboAnimSwitch = false;
		break;
	case 13:
		m_iOnceAnimNumber = 18; //Attack2
		m_bComboAnimSwitch = false;
		break;
	case 14:
		m_iOnceAnimNumber = 19; //smash Attack
		m_bComboAnimSwitch = true;
		break;
	case 15:
		m_iOnceAnimNumber = 5; //Back
		m_bComboAnimSwitch = false;
		break;
	case 40:
		m_iOnceAnimNumber = 11; //right hit
		break;
	case 41:
		m_iOnceAnimNumber = 12; //HeavyHit
		break;
	case 42:
		m_iOnceAnimNumber = 6; //groggy
		break;
	case 51:
		m_iOnceAnimNumber = 20; //stomp Attack
		m_bComboAnimSwitch = false;
		break;

	}

	return S_OK;
}

HRESULT CMonster_Gadasura_Black::Pattern_Change()
{

	m_iOncePattern += 1;

	if (m_iOncePattern >= 16)
	{
		m_iOncePattern = 0; //OncePattern Random
	}


	return S_OK;
}

HRESULT CMonster_Gadasura_Black::Infinity_AnimMotion(_double dDeltaTime)
{
	switch (m_iInfinityPattern)
	{
	case 0:
		m_iInfinityAnimNumber = 24;
		break;
	case 1:
		m_iInfinityAnimNumber = 24;
		break;
	case 2:
		m_iInfinityAnimNumber = 24;
		break;
	case 3:
		m_iInfinityAnimNumber = 24;
		break;
	case 4:
		m_iInfinityAnimNumber = 24;
		break;
	case 5:
		m_iInfinityAnimNumber = 24;
		break;
	case 6:
		m_iInfinityAnimNumber = 24;
		break;
	default:
		m_iInfinityAnimNumber = 0;
		break;
	}

	return S_OK;
}

HRESULT CMonster_Gadasura_Black::Special_Trigger(_double dDeltaTime)
{

	if (m_fDistance < 2 && m_dSpecial_CoolTime > 15)
	{
		m_dSpecial_CoolTime = 0;
		m_dOnceCoolTime = 0;
		m_dInfinity_CoolTime = 0;

		m_bIOnceAnimSwitch = true;
		m_iOncePattern = 51;
	}


	if (m_fDistance > 8 && m_dSpecial_CoolTime > 10)
	{
		m_dSpecial_CoolTime = 0;
		m_dOnceCoolTime = 0;
		m_dInfinity_CoolTime = 0;

		m_bIOnceAnimSwitch = true;
		m_iOncePattern = 3;
	}




	return S_OK;
}

HRESULT CMonster_Gadasura_Black::Ready_ParticleDesc()
{
	// HandPos

	m_pTextureParticleTransform_Hand = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
	NULL_CHECK_RETURN(m_pTextureParticleTransform_Hand, E_FAIL);

	m_pTextureParticleTransform_Demo1 = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
	NULL_CHECK_RETURN(m_pTextureParticleTransform_Demo1, E_FAIL);

	m_pTextureParticleTransform_Demo2 = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
	NULL_CHECK_RETURN(m_pTextureParticleTransform_Demo2, E_FAIL);

	m_pTextureParticleTransform_Demo3 = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
	NULL_CHECK_RETURN(m_pTextureParticleTransform_Demo3, E_FAIL);
	



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

HRESULT CMonster_Gadasura_Black::Update_Particle(_double timer)
{

	_Matrix mat_World = m_pTransformCom->Get_WorldMatrix();

	ATTACHEDESC boneDesc = m_pWeapon->Get_WeaponDesc().eAttachedDesc;

	_Matrix mat_Weapon = boneDesc.Caculate_AttachedBoneMatrix_BlenderFixed();
	_Vector Vec_WeaponPos = boneDesc.Get_AttachedBoneWorldPosition_BlenderFixed();
	mat_Weapon.r[3] = Vec_WeaponPos;

	m_pTextureParticleTransform_Hand->Set_Matrix(mat_Weapon);

//	mat_Weapon.r[3] = Pos + m_pColliderCom->Get_ColliderPosition(9).XMVector();
	m_pTextureParticleTransform_Demo1->Set_Matrix(mat_Weapon);



	return S_OK;
}

HRESULT CMonster_Gadasura_Black::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_Monster_Gadasura_Black), TAG_COM(Com_Model), (CComponent**)&m_pModel));
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

	m_fFrustumRadius = 2.5;
	return S_OK;
}

HRESULT CMonster_Gadasura_Black::Adjust_AnimMovedTransform(_double dDeltaTime)
{
	_uint iNowAnimIndex = m_pModel->Get_NowAnimIndex();
	_double PlayRate = m_pModel->Get_PlayRate();

	if (iNowAnimIndex != m_iOldAnimIndex || PlayRate > 0.98)
	{
		m_iAdjMovedIndex = 0;
		m_EffectAdjust = 0;
		m_dAcceleration = 1;

		m_bLookAtOn = true;
		m_bWeaponAttackSwitch = false;

		if (PlayRate > 0.98 && m_bIOnceAnimSwitch == true)
		{
			m_bIOnceAnimSwitch = false;
			m_dOnceCoolTime = 0;
			m_dInfinity_CoolTime = 0;
		}
	}

	if (PlayRate <= 0.98) //애니메이션의 비율 즉, 0.98은 거의 끝나가는 시점
	{
		switch (iNowAnimIndex)
		{
		case 3:
		{
			if (PlayRate >= 0.28571 && PlayRate <= 0.571428)
				m_pTransformCom->Move_Right(dDeltaTime * 1.5, m_pNavigationCom);
			break;
		}
		case 4:
		{
			if (PlayRate >= 0.22222 && PlayRate <= 0.44444)
				m_pTransformCom->Move_Left(dDeltaTime * 1.5 , m_pNavigationCom);
			break;
		}
		case 5:
		{
			if (PlayRate >= 0.266666 && PlayRate <= 0.5)
			{
				_float fSpeed = g_pGameInstance->Easing(TYPE_QuinticOut, 2.7f, 1.2f, (_float)PlayRate - 0.266666f, 0.2334f); // PlayRate - 0.266666 and 0.5 - 0.266666
				m_pTransformCom->Move_Backward(dDeltaTime * fSpeed, m_pNavigationCom);
			}
			break;
		}
		case 6:
		{
			if (m_iAdjMovedIndex == 0 && PlayRate > 0)
			{
				m_iAdjMovedIndex++;
				m_bLookAtOn = false;
			}
			break;
		}
		case 11:
		{
			if (m_iAdjMovedIndex == 0 && PlayRate > 0)
			{
				m_dAcceleration = 0.7;
				m_iAdjMovedIndex++;
			}
			break;
		}
		case 12:
		{
			if (m_iAdjMovedIndex == 0 && PlayRate > 0)
			{
				m_bLookAtOn = false;
				m_dAcceleration = 0.7;
				m_iAdjMovedIndex++;
			}
			else if (0.f < PlayRate && PlayRate <= 0.8636)
			{
				m_pTransformCom->Move_Backward(dDeltaTime, m_pNavigationCom);

				m_fKnockbackDir.y = 0;

				m_pTransformCom->Turn_Dir(m_fKnockbackDir.XMVector(), 0.9f);
			}
			break;
		}
		case 14:
		{
			if (m_iAdjMovedIndex == 0)
			{
				m_bLookAtOn = true;
				m_iAdjMovedIndex++;
			}
			m_pTransformCom->Move_Forward(dDeltaTime * 2.25, m_pNavigationCom);
			break;
		}
		case 15:
		{
			if (m_iAdjMovedIndex == 0)
			{
				m_bLookAtOn = true;
				m_iAdjMovedIndex++;
			}
			m_pTransformCom->Move_Forward(dDeltaTime * 2.25, m_pNavigationCom);
			break;
		}
		case 17:
		{
			_float Value = g_pGameInstance->Easing_Return(TYPE_Linear, TYPE_Linear, 0, 1, (_float)PlayRate, 0.9f);
			Value = max(min(Value, 1.f), 0.f);
			Set_LimLight_N_Emissive(_float4(0.27f, 0.94f, 0.38f, Value), _float4(Value, Value*0.7f, Value, 0.9f));


			if (m_iAdjMovedIndex == 0 && PlayRate > 0)
			{
				m_bLookAtOn = false;
				m_iAdjMovedIndex++;
			}	


			if (m_EffectAdjust == 0 && PlayRate >= 0.3f)
			{
				// #TIME Attack1
				Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_MONSTER_GM_ATT0, m_pTextureParticleTransform_Demo1);
				Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_MONSTER_GM_ATT1, m_pTransformCom);

				m_EffectAdjust++;

			}
			
			
			else if (PlayRate >= 0.2155 &&PlayRate <= 0.3879)
			{
				m_bWeaponAttackSwitch = true;
			}
			else if (m_iAdjMovedIndex == 1 && PlayRate >= 0.3879)
			{
				m_bWeaponAttackSwitch = false;
				m_iAdjMovedIndex++;
			}
			
			
			
			break;
		}
		case 18:
		{
			if (m_iAdjMovedIndex == 0 && PlayRate > 0)
			{
				m_bLookAtOn = false;
				m_iAdjMovedIndex++;
			}
			else if (PlayRate >= 0.1754 && PlayRate <= 0.3879)
			{

				m_bWeaponAttackSwitch = true;
			}
			else if (m_iAdjMovedIndex == 1 && PlayRate >= 0.3879)
			{
				m_bWeaponAttackSwitch = false;
			}

			break;
		}
		case 19:
		{
			_float Value = g_pGameInstance->Easing_Return(TYPE_Linear, TYPE_Linear, 0, 1, (_float)PlayRate, 0.9f);
			Value = max(min(Value, 1.f), 0.f);
			Set_LimLight_N_Emissive(_float4(0.27f, 0.94f, 0.38f, Value), _float4(Value, Value*0.7f, Value, 0.9f));


			if (PlayRate > 0 && PlayRate <= 0.539215)
			{
				m_bLookAtOn = false; 

				m_dAcceleration = 2.5;

				//if (PlayRate >= 0.3465)
				//	m_bWeaponAttackSwitch = true;
			}
			else {
				m_dAcceleration = 1;
				//m_bWeaponAttackSwitch = false;
			}

			if (m_iAdjMovedIndex == 0 && PlayRate >= 0.539215)
			{
				CMonster_Texture_Bullet::MONSTER_TEXTURE_BULLETDESC Monster_Texture_BulletDesc;

				Monster_Texture_BulletDesc.iBulletTextureNumber = CMonster_Texture_Bullet::GADASURA_TERRAIN_BULLET;
				Monster_Texture_BulletDesc.fSpeedPerSec = 15;
				Monster_Texture_BulletDesc.fScale = _float3(1.f, 1.f, 1.f);

				Monster_Texture_BulletDesc.Object_Transform = m_pTransformCom;
				Monster_Texture_BulletDesc.fPositioning = _float3(0.f, 0.45f, 3.f);


				Monster_Texture_BulletDesc.Object = this;

				Monster_Texture_BulletDesc.dDuration = 0.7;

				FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_MonsterBullet), TAG_OP(Prototype_Object_Monster_Texture_Bullet), &Monster_Texture_BulletDesc));

				
				m_iAdjMovedIndex++;
			}

			if (m_EffectAdjust == 0 && PlayRate >= 0.25f)
			{
				// #TIME SmashAttack
				Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_MONSTER_GM_SKILLSMASH1, m_pTransformCom);
				m_EffectAdjust++;
			}

			if (m_EffectAdjust == 1 && PlayRate >= 0.3f)
			{
				// #TIME SmashAttack
				Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_MONSTER_GM_SKILLSMASH2, m_pTextureParticleTransform_Hand);
				m_EffectAdjust++;
			}
			
			if (m_EffectAdjust ==2 && PlayRate>=0.539215f)
			{
				// #TIME SmashAttack
				Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_MONSTER_GM_SKILLSMASH0, m_pTransformCom);
				m_EffectAdjust++;

			}

			break;
		}
		case 20:
		{

			_float Value = g_pGameInstance->Easing_Return(TYPE_Linear, TYPE_Linear, 0, 1, (_float)PlayRate, 0.9f);
			Value = max(min(Value, 1.f), 0.f);
			Set_LimLight_N_Emissive(_float4(0.27f, 0.94f, 0.38f, Value), _float4(Value, Value*0.7f, Value, 0.9f));


			if (m_iAdjMovedIndex == 0 && PlayRate > 0)
			{
				m_bLookAtOn = false;
				m_iAdjMovedIndex++;
			}
			if (m_iAdjMovedIndex == 1 && PlayRate >= 0.4205607)
			{
				CMonster_Bullet_Universal::MONSTER_BULLET_UNIVERSALDESC Monster_BulletDesc;

				Monster_BulletDesc.iBulletMeshNumber = CMonster_Bullet_Universal::GADASURA_SINKHOLE;
				Monster_BulletDesc.fSpeedPerSec = 15.f;
				Monster_BulletDesc.fScale = _float3(7.5f, 7.5f, 7.5f);

				Monster_BulletDesc.Object_Transform = m_pTransformCom;
				Monster_BulletDesc.fPositioning = _float3(0.f, 0.f, 1.8f);


				Monster_BulletDesc.Object = this;

				Monster_BulletDesc.dDuration = 5;

				Monster_BulletDesc.bBornAttachOn = true;
				Monster_BulletDesc.pBoneName = "skd_l_ball";

				FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_MonsterBullet), TAG_OP(Prototype_Object_Monster_Bullet_Universal), &Monster_BulletDesc));

				m_iAdjMovedIndex++;
			}

			if (m_EffectAdjust == 0 && PlayRate >= 0.1f)
			{
				// #TIME StopBoundAttack
				Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_MONSTER_GM_SKILLBOUND0, m_pTransformCom);
				m_EffectAdjust++;
			}
			if (m_EffectAdjust == 1 && PlayRate >= 0.3f)
			{
				// #TIME StopBoundAttack
				Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_MONSTER_GM_SKILLBOUND1, m_pTransformCom);
				m_EffectAdjust++;
			}



			break;
		}
		case 21:
		{
			_float Value = g_pGameInstance->Easing_Return(TYPE_Linear, TYPE_Linear, 0, 1, (_float)PlayRate, 0.9f);
			Value = max(min(Value, 1.f), 0.f);
			Set_LimLight_N_Emissive(_float4(0.27f, 0.94f, 0.38f, Value), _float4(Value, Value*0.7f, Value, 0.9f));

			if (m_iAdjMovedIndex==0 && PlayRate >= 0.29411)
			{
				m_bWeaponAttackSwitch = true;
				m_iAdjMovedIndex++;
			}

			if (PlayRate <= 0.588235)
			{
				m_pTransformCom->Move_Forward(dDeltaTime * 1.5);
				
			}
			else
			{
				m_bWeaponAttackSwitch = false;
			}

			if (m_EffectAdjust == 0 && PlayRate >= 0.05f)
			{
				// #TIME Run Attack
				Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_MONSTER_GM_SKILLRUN0, m_pTransformCom);
				m_EffectAdjust++;
			}

			if (m_EffectAdjust == 1 && PlayRate >= 0.29411)
			{
				// #TIME Run Attack
				Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_MONSTER_GM_SKILLRUN1, m_pTransformCom);
				m_EffectAdjust++;
			}
			break;
		}
		case 22:
		{
			if (m_iAdjMovedIndex == 0 && PlayRate > 0)
			{
				m_bLookAtOn = false;
				m_iAdjMovedIndex++;
			}
			break;
		}

		case 24:
		{
			m_pTransformCom->Move_Forward(dDeltaTime * 0.6, m_pNavigationCom);

			break;
		}
		case 25:
		{
			_float Value = g_pGameInstance->Easing_Return(TYPE_Linear, TYPE_Linear, 0, 1, (_float)PlayRate, 0.9f);
			Value = max(min(Value, 1.f), 0.f);
			Set_LimLight_N_Emissive(_float4(0.27f, 0.94f, 0.38f, Value), _float4(Value, Value*0.7f, Value, 0.9f));


			if (m_iAdjMovedIndex == 0 && PlayRate >= 0.9574)
			{
				m_bLookAtOn = false;
				m_iAdjMovedIndex++;
			}
			else if (PlayRate >= 0.9574 && PlayRate <= 0.98) {
				m_bLookAtOn = true;
				m_pTransformCom->Move_Forward(dDeltaTime * 2.25, m_pNavigationCom);
			}

			if (m_EffectAdjust == 0 && PlayRate >= 0.1)
			{
				Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_MONSTER_GM_Cash2, m_pTransformCom);
				m_EffectAdjust++;
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

CMonster_Gadasura_Black * CMonster_Gadasura_Black::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CMonster_Gadasura_Black*	pInstance = NEW CMonster_Gadasura_Black(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CMonster_Gadasura_Black");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMonster_Gadasura_Black::Clone(void * pArg)
{
	CMonster_Gadasura_Black*	pInstance = NEW CMonster_Gadasura_Black(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CMonster_Gadasura_Black");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMonster_Gadasura_Black::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModel);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pHPUI);
	Safe_Release(m_pWeapon);

	Safe_Release(m_pTextureParticleTransform_Hand);
	Safe_Release(m_pTextureParticleTransform_Demo1);
	Safe_Release(m_pTextureParticleTransform_Demo2);
	Safe_Release(m_pTextureParticleTransform_Demo3);
	

}