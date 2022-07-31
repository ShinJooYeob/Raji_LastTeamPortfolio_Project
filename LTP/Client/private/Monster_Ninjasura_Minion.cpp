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
	m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, _float3(216.357f, 29.2f, 188.583f));

	m_pNavigationCom->FindCellIndex(m_pTransformCom->Get_MatrixState(CTransform::STATE_POS));
	//////////////////////////////
#endif

	return S_OK;
}

_int CMonster_Ninjasura_Minion::Update(_double dDeltaTime)
{

	if (__super::Update(dDeltaTime) < 0)return -1;

	if (m_fHP <= 0)
	{
		m_bLookAtOn = false;
		m_pDissolve->Update_Dissolving(dDeltaTime);
		m_pDissolve->Set_DissolveOn(false, 2.f);

		m_dDissolveTime += dDeltaTime;

		if (m_bDieSound == false && m_dDissolveTime >= 1.)
		{
			//g_pGameInstance->Play3D_Sound(TEXT("EH_Mahinasura_Die.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_MONSTER, 0.5f);
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

	if (m_pHPUI != nullptr && m_MotionTrailOn == false)
		m_pHPUI->Update(dDeltaTime);

	Update_Collider(dDeltaTime);

	return _int();
}

_int CMonster_Ninjasura_Minion::LateUpdate(_double dDeltaTime)
{
	if (__super::LateUpdate(dDeltaTime) < 0)return -1;

	//////////
	if (m_bIsOnScreen && false == m_MotionTrailOn)
	{
		FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
	}
	//////////////Motion Test
	FAILED_CHECK(m_pRendererCom->Add_TrailGroup(CRenderer::TRAIL_MOTION, m_pMotionTrail));
	////////////////

	FAILED_CHECK(m_pRendererCom->Add_ShadowGroup(CRenderer::SHADOW_ANIMMODEL, this, m_pTransformCom, m_pShaderCom, m_pModel, nullptr, m_pDissolve));
	m_vOldPos = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS);

#ifdef _DEBUG
	FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pColliderCom));
	FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pAttackColliderCom));
#endif

	m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, m_pNavigationCom->Get_NaviPosition(m_pTransformCom->Get_MatrixState(CTransform::STATE_POS)));

	if (m_pHPUI != nullptr && m_MotionTrailOn == false)
	{
		m_pHPUI->LateUpdate(dDeltaTime);
	}

	return _int();
}

_int CMonster_Ninjasura_Minion::Render()
{
	if (__super::Render() < 0)
		return -1;

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
	m_MotionTrailOn = false;

	m_bIOnceAnimSwitch = true;
	if (m_eMonster_State != Anim_State::MONSTER_ATTACK)
	{
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
	tAttachedDesc.Initialize_AttachedDesc(this, "spine_01", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(0.f, 0.f, -0.5f));
	m_vecAttachedDesc.push_back(tAttachedDesc);


	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "head", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(0.f, 0.f, -0.8f));
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
	tAttachedDesc.Initialize_AttachedDesc(this, "head", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(-0.34938f, -0.11046f, -0.32727f)); //마지막 인자에 블렌더 뼈 위치 그대로 넣어줄 것 다만 z엔 -로 해줄것
	m_vecAttackAttachedDesc.push_back(tAttachedDesc);
	m_pAttackColliderCom->Set_ParantBuffer();
	/////////////////////////////////////////////
	return S_OK;
}

HRESULT CMonster_Ninjasura_Minion::SetUp_Fight(_double dDeltaTime)
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

	if(m_MotionTrailOn == false)
		FAILED_CHECK(g_pGameInstance->Add_RepelGroup(m_pTransformCom, 1.5f, m_pNavigationCom));


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

		m_pTransformCom->LookAt(m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS));
		m_MotionTrailOn = false; //!@#!@#@!#!@#!@#!@#!@#@$@!Test 맞을떄 모션 트레일온 false 시켜야함
	}

	return S_OK;
}

HRESULT CMonster_Ninjasura_Minion::Once_AnimMotion(_double dDeltaTime)
{
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
		m_iInfinityAnimNumber = 0;
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
	HpDesc.m_Dimensions = 1.5f;
	m_fMaxHP = 15.f;
	m_fHP = m_fMaxHP;
	g_pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&m_pHPUI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_HpUI), &HpDesc);


	CDissolve::DISSOLVEDESC DissolveDesc;
	DissolveDesc.pModel = m_pModel;
	DissolveDesc.eDissolveModelType = CDissolve::DISSOLVE_ANIM;
	DissolveDesc.pShader = m_pShaderCom;
	DissolveDesc.RampTextureIndex = 11;
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
		case 2:
		{
			if (PlayRate >= 0.9)
			{
				m_bIOnceAnimSwitch = false;
			}
		}
		case 3:
		{
			m_bLookAtOn = false;
			if (m_iAdjMovedIndex == 0)
			{
				m_pTransformCom->LookAt(m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS));

				m_bColliderAttackOn = true;
				m_iAdjMovedIndex++;
			}

			if (PlayRate > 0 && PlayRate <= 0.1)
			{
				m_MotionTrailOn = true;
				m_pMotionTrail->Add_MotionBuffer(m_pTransformCom->Get_WorldFloat4x4(), _float4(1.f, 0.f, 0.f, 1.f), 1.f);
			}
			else if (PlayRate >= 0.1 && PlayRate <= 0.8)
			{
				m_MotionTrailOn = false;
				m_pTransformCom->Move_Forward(dDeltaTime * 5,m_pNavigationCom);
			}
			else if (PlayRate >= 0.8)
			{
				m_MotionTrailOn = true;
				m_pMotionTrail->Add_MotionBuffer(m_pTransformCom->Get_WorldFloat4x4(), _float4(1.f, 0.f, 0.f, 1.f), 1.f);
			}

			break;
		}
		case 11:
		{
			if (m_iAdjMovedIndex == 0 && PlayRate >0)
			{
				m_MotionTrailOn = true;
				m_bColliderAttackOn = true;
				m_iAdjMovedIndex++;
			}

			if (PlayRate >= 0.85)
			{
				m_pTransformCom->LookAt(m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS));
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
					m_MotionTrailOn = false;
					m_pMotionTrail->Add_MotionBuffer(m_pTransformCom->Get_WorldFloat4x4(), _float4(1.f, 0.f, 0.f, 1.f), 1.f);

					m_bColliderAttackOn = true;
					m_iAdjMovedIndex++;
				}
				m_pTransformCom->Move_Forward(dDeltaTime * 5, m_pNavigationCom);
			}
			else if (m_iAdjMovedIndex == 1 && PlayRate >= 0.4)
			{
				m_MotionTrailOn = true;
				m_pMotionTrail->Add_MotionBuffer(m_pTransformCom->Get_WorldFloat4x4(), _float4(1.f, 0.f, 0.f, 1.f), 1.f);
				m_iAdjMovedIndex++;
			}else if (m_iAdjMovedIndex == 2 && PlayRate >= 0.8)
			{
				//플레이어 주변 랜덤 생성
				_Vector PlayerPos = m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS);

				CUtilityMgr* pUtil = GetSingle(CUtilityMgr);

				//랜덤방향
				_Vector vDir = XMVector3Normalize((m_pPlayerTransform->Get_MatrixState(CTransform::STATE_LOOK) * pUtil->RandomFloat(-1, 1) + m_pPlayerTransform->Get_MatrixState(CTransform::STATE_RIGHT) * pUtil->RandomFloat(-1, 1)));
			
				PlayerPos = PlayerPos + (vDir * 4);

				m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, PlayerPos);
				m_iAdjMovedIndex++;
			
			}
			break;
		}
		case 13:
		{
			if (m_iAdjMovedIndex == 0 && PlayRate > 0)
			{
				m_MotionTrailOn = true;
				m_bColliderAttackOn = true;
				m_iAdjMovedIndex++;
			}

			if (PlayRate >= 0.66)
			{
				m_pTransformCom->LookAt(m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS));
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
					m_MotionTrailOn = false;
					m_pMotionTrail->Add_MotionBuffer(m_pTransformCom->Get_WorldFloat4x4(), _float4(1.f, 0.f, 0.f, 1.f), 1.f);

					m_bColliderAttackOn = true;
					m_iAdjMovedIndex++;
				}
				m_pTransformCom->Move_Forward(dDeltaTime * 5, m_pNavigationCom);
			}
			else if (m_iAdjMovedIndex == 1 && PlayRate >= 0.4)
			{
				m_MotionTrailOn = true;
				m_pMotionTrail->Add_MotionBuffer(m_pTransformCom->Get_WorldFloat4x4(), _float4(1.f, 0.f, 0.f, 1.f), 1.f);
				m_iAdjMovedIndex++;
			}
			else if (m_iAdjMovedIndex == 2 && PlayRate >= 0.8)
			{
				//플레이어 주변 랜덤 생성
				_Vector PlayerPos = m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS);

				CUtilityMgr* pUtil = GetSingle(CUtilityMgr);

				//랜덤방향
				_Vector vDir = XMVector3Normalize((m_pPlayerTransform->Get_MatrixState(CTransform::STATE_LOOK) * pUtil->RandomFloat(-1, 1) + m_pPlayerTransform->Get_MatrixState(CTransform::STATE_RIGHT) * pUtil->RandomFloat(-1, 1)));

				PlayerPos = PlayerPos + (vDir * 4);

				m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, PlayerPos);
				m_iAdjMovedIndex++;

			}
			break;
		}
		case 15:
		{
			if (m_iAdjMovedIndex == 0 && PlayRate > 0)
			{
				m_MotionTrailOn = true;
				m_bColliderAttackOn = true;

				m_pTransformCom->LookAt(m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS));

				_Vector PlayerPos = m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS);

				CUtilityMgr* pUtil = GetSingle(CUtilityMgr);

				_Vector vDir = XMVector3Normalize((m_pPlayerTransform->Get_MatrixState(CTransform::STATE_LOOK) * pUtil->RandomFloat(-1, 1) + m_pPlayerTransform->Get_MatrixState(CTransform::STATE_RIGHT) * pUtil->RandomFloat(-1, 1)));

				PlayerPos = PlayerPos + (vDir * 2);

				m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, PlayerPos);

				m_iAdjMovedIndex++;
			}
			else if (m_iAdjMovedIndex == 1 && PlayRate > 0)
			{
				m_MotionTrailOn = true;
				m_pMotionTrail->Add_MotionBuffer(m_pTransformCom->Get_WorldFloat4x4(), _float4(1.f, 0.f, 0.f, 1.f), 1.f);
				m_iAdjMovedIndex++;
			}
			else if (m_iAdjMovedIndex == 2 && PlayRate > 0.1)
			{
				m_MotionTrailOn = false;

				m_iAdjMovedIndex++;
			}
			break;
		}
		case 16:
		{
			m_bColliderAttackOn = true;

			m_MotionTrailOn = false;
			_Vector vTarget = XMVector3Normalize(m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS) - m_pTransformCom->Get_MatrixState(CTransform::STATE_POS));
			m_pTransformCom->Turn_Dir(vTarget, 0.7f);
			
			if (PlayRate > 0 && PlayRate <= 0.95)
			{
				_float fSpeed = g_pGameInstance->Easing_Return(TYPE_SinInOut, TYPE_SinInOut, 0, 2.852698f, (_float)PlayRate, 0.95f); // PlayRate - 0.266666 and 0.5 - 0.266666
				m_pTransformCom->Move_Forward(dDeltaTime * fSpeed, m_pNavigationCom);
				m_dAcceleration = 0.8;
			}
			break;
		}
		case 17:
		{
			m_bColliderAttackOn = true;

			m_MotionTrailOn = false;
			_Vector vTarget = XMVector3Normalize(m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS) - m_pTransformCom->Get_MatrixState(CTransform::STATE_POS));
			m_pTransformCom->Turn_Dir(vTarget, 0.7f);
			if (PlayRate > 0 && PlayRate <= 0.95)
			{
				_float fSpeed = g_pGameInstance->Easing_Return(TYPE_SinInOut, TYPE_SinInOut, 0, 2.852698f, (_float)PlayRate, 0.95f); // PlayRate - 0.266666 and 0.5 - 0.266666
				m_pTransformCom->Move_Forward(dDeltaTime * fSpeed, m_pNavigationCom);
				m_dAcceleration = 0.8;
			}
			break;
		}
		case 18:
		{
			if (m_iAdjMovedIndex == 0 && PlayRate >= 0.75)
			{

				m_MotionTrailOn = true;
				m_pMotionTrail->Add_MotionBuffer(m_pTransformCom->Get_WorldFloat4x4(), _float4(1.f, 0.f, 0.f, 1.f), 1.f);

				_Vector PlayerPos = m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS);

				CUtilityMgr* pUtil = GetSingle(CUtilityMgr);

				_Vector vDir = XMVector3Normalize((m_pPlayerTransform->Get_MatrixState(CTransform::STATE_LOOK) * pUtil->RandomFloat(-1, 1) + m_pPlayerTransform->Get_MatrixState(CTransform::STATE_RIGHT) * pUtil->RandomFloat(-1, 1)));

				PlayerPos = PlayerPos + (vDir * 4);

				m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, PlayerPos);

				m_iAdjMovedIndex++;

			}
			break;
		}
		}

	}

	m_iOldAnimIndex = iNowAnimIndex;
	return S_OK;
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
}