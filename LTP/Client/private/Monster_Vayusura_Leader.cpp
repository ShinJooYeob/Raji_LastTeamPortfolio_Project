#include "stdafx.h"
#include "..\public\Monster_Vayusura_Leader.h"
#include "Monster_Bullet_Universal.h"
#include "HpUI.h"

CMonster_Vayusura_Leader::CMonster_Vayusura_Leader(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CMonster(pDevice, pDeviceContext)
{
}

CMonster_Vayusura_Leader::CMonster_Vayusura_Leader(const CMonster_Vayusura_Leader & rhs)
	: CMonster(rhs)
{
}

HRESULT CMonster_Vayusura_Leader::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));


	return S_OK;
}

HRESULT CMonster_Vayusura_Leader::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components());

	if (pArg != nullptr)
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, *((_float3*)pArg));

	m_pTransformCom->Rotation_CW(XMVectorSet(0, 1, 0, 0), XMConvertToRadians(170));


	SetUp_Info();


#ifdef _DEBUG
	///////////////////test

	m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, _float3(493.f, 7.100010f, 103.571f)); //Stage2

	//m_pNavigationCom->FindCellIndex(m_pTransformCom->Get_MatrixState(CTransform::STATE_POS));
	///////////////////
#endif

	return S_OK;
}

_int CMonster_Vayusura_Leader::Update(_double dDeltaTime)
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

		if (m_bDieSound == false && m_dDissolveTime >= 1.)
		{
			g_pGameInstance->Play3D_Sound(TEXT("EH_M1_739.mp3"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_MONSTER, 0.5f);
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
	FAILED_CHECK(m_pModel->Update_AnimationClip(dDeltaTime * m_dAcceleration, m_bIsOnScreen));
	FAILED_CHECK(Adjust_AnimMovedTransform(dDeltaTime));

	
	if (m_pHPUI != nullptr)
		m_pHPUI->Update(dDeltaTime);

	Update_Collider(dDeltaTime);

	//////////////////////
	Player_Comparison(dDeltaTime);
	//////////////////////

	////Delete
	//m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, _float3(216.357f, 29.2f, 185.583f));
	
	return _int();

}

_int CMonster_Vayusura_Leader::LateUpdate(_double dDeltaTime)
{
	if (__super::LateUpdate(dDeltaTime) < 0)return -1;
	if (__super::LateUpdate(dDeltaTime) == UPDATE_SKIP)
		return UPDATE_SKIP;
	//////////
	if (m_bIsOnScreen)
	{
		FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
	}
	//////////

	//FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
	FAILED_CHECK(m_pRendererCom->Add_ShadowGroup(CRenderer::SHADOW_ANIMMODEL, this, m_pTransformCom, m_pShaderCom, m_pModel, nullptr, m_pDissolve));
	m_vOldPos = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS);

#ifdef _DEBUG
	FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pColliderCom));
#endif

	if (m_pHPUI != nullptr)
	{
		m_pHPUI->LateUpdate(dDeltaTime);
	}
	return _int();
}

_int CMonster_Vayusura_Leader::Render()
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

_int CMonster_Vayusura_Leader::LateRender()
{
	if (__super::LateRender() < 0)
		return -1;
	if (__super::LateRender() == UPDATE_SKIP)
		return UPDATE_SKIP;

	return _int();
}

void CMonster_Vayusura_Leader::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
	if (CollisionTypeID::CollisionType_Player == eConflictedObjCollisionType)
	{
		_Vector vDamageDir = XMVector3Normalize(pConflictedCollider->Get_ColliderPosition(iConflictedObjColliderIndex).XMVector() - m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS));
		pConflictedObj->Take_Damage(this, 1.f, vDamageDir, m_bOnKnockbackCol, m_fKnockbackColPower);
		pConflictedCollider->Set_Conflicted(1.f);

		Safe_Release(m_BulletObj);

	}
}

_float CMonster_Vayusura_Leader::Take_Damage(CGameObject * pTargetObject, _float fDamageAmount, _fVector vDamageDir, _bool bKnockback, _float fKnockbackPower)
{
	m_pHPUI->Set_ADD_HitCount((_int)fDamageAmount);
	m_fHP += -fDamageAmount;
	m_bIOnceAnimSwitch = true;

	m_bStopCoolTimeOn = true;


	m_bAttackCancelOn = true;
	m_iOncePattern = 40;


	if (0 >= m_fHP)
	{
		return -1.f;
	}

	return _float();
}

HRESULT CMonster_Vayusura_Leader::SetUp_Info()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	m_pPlayerTransform = static_cast<CTransform*>(pGameInstance->Get_Commponent_By_LayerIndex(m_eNowSceneNum, TAG_LAY(Layer_Player), TAG_COM(Com_Transform)));

	RELEASE_INSTANCE(CGameInstance);


	return S_OK;
}

HRESULT CMonster_Vayusura_Leader::SetUp_Collider()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_Collider), (CComponent**)&m_pColliderCom));

	/////////////////m_pColliderCom!@!@#$@!#$@#$@$!@%#$%@#$%%^^W@!
	COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(5.f, 5.f, 5.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	ATTACHEDESC tAttachedDesc;
	tAttachedDesc.Initialize_AttachedDesc(this, "spine_01", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(0.f, 0.033178f, -0.45067f));
	m_vecAttachedDesc.push_back(tAttachedDesc);

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "head", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(0.f, -0.010466f, -0.6897f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pColliderCom->Set_ParantBuffer();

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "spine_02", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(0.f, -0.031359f, -0.49888f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pColliderCom->Set_ParantBuffer();

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "calf_r", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(-0.0445425f, -0.046337f, -0.21746f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pColliderCom->Set_ParantBuffer();

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "lowerarm_r", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(-0.20648f, 0.069229f, -0.51353f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pColliderCom->Set_ParantBuffer();

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "middle_01_r", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(-0.39241f, 0.083289f, -0.65896f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pColliderCom->Set_ParantBuffer();

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "index_02_r", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(-0.6475f, 0.08306f, -0.69517f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pColliderCom->Set_ParantBuffer();


	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "lowerarm_l", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(0.20648f, 0.069229f, -0.51353f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pColliderCom->Set_ParantBuffer();

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "middle_01_l", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(0.39241f, 0.083289f, -0.65896f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pColliderCom->Set_ParantBuffer();

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "index_02_l", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(0.6475f, 0.08306f, -0.69517f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pColliderCom->Set_ParantBuffer();


	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(4.f, 4.f, 4.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "spine_01", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(0.f, 0.033178f, -0.45067f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pColliderCom->Set_ParantBuffer();
	//////////////////////////////////////////////////////

	return S_OK;
}

HRESULT CMonster_Vayusura_Leader::SetUp_Fight(_double dDeltaTime)
{
	m_fDistance = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS).Get_Distance(m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS));


	m_fPlayerPos = m_pPlayerTransform->Get_MatrixState_Float3(CTransform::STATE_POS);


	if (m_bLookAtOn)
	{

		_Vector vTarget = XMVector3Normalize(m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS) - m_pTransformCom->Get_MatrixState(CTransform::STATE_POS));

		m_pTransformCom->Turn_Dir(vTarget, 0.95f);
	}

	return S_OK;
}

HRESULT CMonster_Vayusura_Leader::Update_Collider(_double dDeltaTime)
{
	m_pColliderCom->Update_ConflictPassedTime(dDeltaTime);

	//Collider
	_uint	iNumCollider = m_pColliderCom->Get_NumColliderBuffer();
	for (_uint i = 0; i < iNumCollider; i++)
		m_pColliderCom->Update_Transform(i, m_vecAttachedDesc[i].Caculate_AttachedBoneMatrix_BlenderFixed());

	FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_Monster, this, m_pColliderCom));

	if (m_bRepelOff != true)
		FAILED_CHECK(g_pGameInstance->Add_RepelGroup(m_pTransformCom, 1.f));

	return S_OK;
}

HRESULT CMonster_Vayusura_Leader::PlayAnim(_double dDeltaTime)
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

HRESULT CMonster_Vayusura_Leader::CoolTime_Manager(_double dDeltaTime)
{
	//한번만 동작하는 애니메이션

	if (m_bStopCoolTimeOn == false)
	{
		m_dOnceCoolTime += dDeltaTime;
		m_dInfinity_CoolTime += dDeltaTime;
	}

	if (m_dOnceCoolTime > 6)
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
	if (m_dInfinity_CoolTime >= 2.5)
	{
		m_iInfinityPattern = rand() % 4;


		CUtilityMgr* pUtil = GetSingle(CUtilityMgr);

		XMStoreFloat4(&m_fDirection, XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK) * pUtil->RandomFloat(-1, 1) + m_pTransformCom->Get_MatrixState(CTransform::STATE_RIGHT) * pUtil->RandomFloat(-1, 1)));

		if (m_fDistance > 8)
		{
			m_iInfinityPattern = 10;
		}

		m_dInfinity_CoolTime = 0;
	}

	return S_OK;
}

HRESULT CMonster_Vayusura_Leader::Once_AnimMotion(_double dDeltaTime)
{
	// #DEBUG PatternSET
	m_iOncePattern = 0;
	if (KEYPRESS(DIK_B))
		m_iOncePattern = 0;

	switch (m_iOncePattern)
	{
	case 0:
		m_iOnceAnimNumber = 6; //Vayusura_Dive
		break;
	case 1:
		m_iOnceAnimNumber = 6; //Vayusura_Dive
		break;
	case 40:
	Set_LimLight_N_Emissive();
		m_iOnceAnimNumber = 4; //Vayusura_Hit
		break;
		case 41:
		Set_LimLight_N_Emissive();
		break;

	}

	return S_OK;
}

HRESULT CMonster_Vayusura_Leader::Pattern_Change()
{

	m_iOncePattern += 1;

	if (m_iOncePattern > 1)
	{
		m_iOncePattern = 0;
	}


	return S_OK;
}

HRESULT CMonster_Vayusura_Leader::Infinity_AnimMotion(_double dDeltaTime)
{
	switch (m_iInfinityPattern)
	{
	case 0:
		if (m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS).y < m_fPlayerPos.y + 4 ||
			m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS).y > m_fPlayerPos.y + 2)
		{
			m_pTransformCom->MovetoDir(XMLoadFloat4(&_float4(0.f, 1.f, 0.f, 0.f)), dDeltaTime * 0.5);

			m_pTransformCom->MovetoDir(XMLoadFloat4(&m_fDirection), dDeltaTime * 0.5);
		}
		m_iInfinityAnimNumber = 0; // flapping
		break;
	case 1:
		if (m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS).y < m_fPlayerPos.y + 4 ||
			m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS).y > m_fPlayerPos.y + 2)
		{
			m_pTransformCom->MovetoDir(XMLoadFloat4(&_float4(0.f, 1.f, 0.f, 0.f)), -dDeltaTime * 0.5);

			m_pTransformCom->MovetoDir(XMLoadFloat4(&m_fDirection), dDeltaTime * 0.5);
		}
		m_iInfinityAnimNumber = 0;
		break;
	case 2:
		if (m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS).y < m_fPlayerPos.y + 4 ||
			m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS).y > m_fPlayerPos.y + 2)
		{

			m_pTransformCom->MovetoDir(XMLoadFloat4(&m_fDirection), dDeltaTime * 0.5);
		}
		m_iInfinityAnimNumber = 1;
		break;
	case 3:
		if (m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS).y < m_fPlayerPos.y + 4||
			m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS).y > m_fPlayerPos.y + 2)
		{

			m_pTransformCom->MovetoDir(XMLoadFloat4(&m_fDirection), dDeltaTime * 0.5);
		}
		m_iInfinityAnimNumber = 1;
		break;
	case 10:
	{
		_float3 fTempPlayerPos = m_fPlayerPos;

		fTempPlayerPos.y += 5;

		_Vector vTarget = XMVector3Normalize(XMLoadFloat3(&fTempPlayerPos) - m_pTransformCom->Get_MatrixState(CTransform::STATE_POS));
		//m_pTransformCom->Turn_Dir(vTarget, 0.9f);
		if (m_fDistance > 7)
		{
			m_pTransformCom->Turn_Dir(vTarget, 0.9f);

			m_pTransformCom->MovetoDir(vTarget, dDeltaTime );
			m_iInfinityAnimNumber = 2;
		}
		else {

		}
		break;
	}
	}

	return S_OK;
}

HRESULT CMonster_Vayusura_Leader::Player_Comparison(_double dDeltaTime)
{
	_float3 fPlayerPos = m_pPlayerTransform->Get_MatrixState_Float3(CTransform::STATE_POS);
	_float3 fMonsterPos = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS);

	if (fPlayerPos.y + 1 >= fMonsterPos.y)
	{
		fMonsterPos.y = fPlayerPos.y + 1;
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, fMonsterPos);
	}

	if (fPlayerPos.y + 4 <= fMonsterPos.y)
	{
		fMonsterPos.y = fPlayerPos.y + 4;
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, fMonsterPos);
	}

	return S_OK;

}

HRESULT CMonster_Vayusura_Leader::Ready_ParticleDesc()
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

HRESULT CMonster_Vayusura_Leader::Update_Particle(_double timer)
{
	_Matrix mat_World = m_pTransformCom->Get_WorldMatrix();
	//	ATTACHEDESC boneDesc = m_vecAttachedDesc[0]->Get_WeaponDesc().eAttachedDesc;
	//	_Vector Vec_WeaponPos = boneDesc.Get_AttachedBoneWorldPosition_BlenderFixed();
	//	_Matrix mat_Weapon = boneDesc.Caculate_AttachedBoneMatrix_BlenderFixed();

	mat_World.r[0] = XMVector3Normalize(mat_World.r[0]);
	mat_World.r[1] = XMVector3Normalize(mat_World.r[1]);
	mat_World.r[2] = XMVector3Normalize(mat_World.r[2]);

	mat_World.r[3] = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS).XMVector();
	m_pTextureParticleTransform_Demo1->Set_Matrix(mat_World); // Head

	if (m_BulletObj)
	{
		CTransform* trans = (CTransform*)m_BulletObj->Get_Component(TAG_COM(Com_Transform));
		m_pTextureParticleTransform_Demo2->Set_Matrix(trans->Get_WorldMatrix());

	}

	m_dealyEffect_Time -= timer;

	if (m_dealyEffect_Rain && m_dealyEffect_Time <= 0)
	{
		m_dealyEffect_Rain = false;

		{

			INSTMESHDESC testMesh = GETPARTICLE->Get_EffectSetting_Mesh(CPartilceCreateMgr::E_MESHINST_EFFECTJ::MESHINST_EFFECTJ_BOW_Q_ICE2,
				Prototype_Mesh_SM_RainDrop,
				3.f,
				0.5f,
				_float4(0.0f, 0.0f, 1.0f, 1),
				_float4(1.0f, 1.0f, 1.0f, 1),
				1,
				//_float3(0.3f, 0.5f, 0.3f),
				//_float3(0.3f, 0.5f, 0.3f),
				_float3(0.15f, 0.4f, 0.15f),
				_float3(0.15f, 0.4f, 0.15f),
				1);

			// testMesh.fDistortionNoisingPushPower = 0;
			testMesh.Particle_Power = -20.0f;
			// testMesh.ePassID = MeshPass_MaskingNoising;
			// testMesh.eParticleTypeID = InstanceEffect_Fountain;
			testMesh.eInstanceCount = Prototype_ModelInstance_64;
			// _float randpower = GetSingle(CUtilityMgr)->RandomFloat(3, 8);
			// 
			// testMesh.Particle_Power = randpower;
			// testMesh.iNoiseTextureIndex = NONNOISE;
			// 
			testMesh.ParticleStartRandomPosMin = _float3(-5, 10, -5);
			testMesh.ParticleStartRandomPosMax = _float3(5, 10,5);
			_Matrix mat = m_pTextureParticleTransform_Demo2->Get_WorldMatrix();
			_Vector pos = mat.r[3];
			testMesh.vFixedPosition = pos;

			GETPARTICLE->Create_MeshInst_DESC(testMesh, m_eNowSceneNum);
		}

		{
			INSTMESHDESC testMesh = GETPARTICLE->Get_EffectSetting_Mesh(CPartilceCreateMgr::E_MESHINST_EFFECTJ::MESHINST_EFFECTJ_BOW_Q_PLANE,
				Prototype_Mesh_circle,
				3.0f,
				0.3f,
				_float4(0.15f, 0.38f, 0.92f, 1),
				_float4(0.15f, 0.38f, 0.92f, 0.0f),
				1,
				_float3(1.0f),
				_float3(15.0f),
				1);

			// testMesh.fDistortionNoisingPushPower = 0;
			// testMesh.Particle_Power = ;
			// testMesh.eParticleTypeID = InstanceEffect_Fountain;
			testMesh.eInstanceCount = Prototype_ModelInstance_8;
			testMesh.ePassID = MeshPass_MaskingNoising;
			// testMesh.Particle_Power = randpower;
			testMesh.iMaskingTextureIndex = 64;
			testMesh.iNoiseTextureIndex = 102;
			testMesh.vEmissive_SBB = _float3(1, 0, 0);

			testMesh.ParticleStartRandomPosMin = _float3(-5, 0.0f, -5);
			testMesh.ParticleStartRandomPosMax = _float3(5, 0.0f, 5);
			_Matrix mat = m_pPlayerTransform->Get_WorldMatrix();
			_Vector pos = mat.r[3];
			testMesh.vFixedPosition = pos;
			testMesh.vNoisePushingDir = _float2(-1, 0);
			// GETPARTICLE->Create_MeshInst_DESC(testMesh, m_eNowSceneNum);

			//testMesh.iMaskingTextureIndex = 200;
			//GETPARTICLE->Create_MeshInst_DESC(testMesh, m_eNowSceneNum);

			//testMesh.iMaskingTextureIndex = 144;
			//GETPARTICLE->Create_MeshInst_DESC(testMesh, m_eNowSceneNum);
		}

	}



	if (KEYDOWN(DIK_V))
	{
	//	Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_MONSTER_VL_Cash3, m_pTextureParticleTransform_Demo2);

		{

		INSTMESHDESC testMesh = GETPARTICLE->Get_EffectSetting_Mesh(CPartilceCreateMgr::E_MESHINST_EFFECTJ::MESHINST_EFFECTJ_BOW_Q_ICE2,
			Prototype_Mesh_SM_RainDrop,
			3.f,
			0.5f,
			_float4(1.0f, 1.0f, 1.0f, 1),
			_float4(0.0f, 0.0f, 0.0f, 1),
			0,
			_float3(0.3f,0.5f,0.3f),
			_float3(0.3f, 0.5f, 0.3f),
			1);

		// testMesh.fDistortionNoisingPushPower = 0;
		testMesh.Particle_Power = -20.0f;
		// testMesh.ePassID = MeshPass_MaskingNoising;
		// testMesh.eParticleTypeID = InstanceEffect_Fountain;
		testMesh.eInstanceCount = Prototype_ModelInstance_128;
		// _float randpower = GetSingle(CUtilityMgr)->RandomFloat(3, 8);
		// 
		// testMesh.Particle_Power = randpower;
		// testMesh.iNoiseTextureIndex = NONNOISE;
		// 
		testMesh.ParticleStartRandomPosMin = _float3(-5,10 , -5);
		testMesh.ParticleStartRandomPosMax = _float3(5, 10 , 5);
		_Matrix mat = m_pTransformCom->Get_WorldMatrix();
		_Vector pos = mat.r[3];
		testMesh.vFixedPosition = pos;

		// GETPARTICLE->Create_MeshInst_DESC(testMesh, m_eNowSceneNum);
		}

		{

			INSTMESHDESC testMesh = GETPARTICLE->Get_EffectSetting_Mesh(CPartilceCreateMgr::E_MESHINST_EFFECTJ::MESHINST_EFFECTJ_BOW_Q_PLANE,
				Prototype_Mesh_circle,
				0,
				0.3f,
				_float4(0.43f, 0.84f, 0.88f, 1),
				_float4(0.81f, 0.95f, 0.96f, 0.5f),
				3,
				_float3(1.0f),
				_float3(15.0f),
				1);

			// testMesh.fDistortionNoisingPushPower = 0;
			// testMesh.Particle_Power = ;
			// testMesh.eParticleTypeID = InstanceEffect_Fountain;
			 testMesh.eInstanceCount = Prototype_ModelInstance_8;
			 testMesh.ePassID = MeshPass_MaskingNoising_Bright;
			// testMesh.Particle_Power = randpower;
			 testMesh.iMaskingTextureIndex = 179;
			 testMesh.iNoiseTextureIndex = 105;
			 testMesh.vEmissive_SBB = _float3(1, 0, 0);

			testMesh.ParticleStartRandomPosMin = _float3(-5, 0.1f, -5);
			testMesh.ParticleStartRandomPosMax = _float3(5, 0.1f, 5);
			_Matrix mat = m_pTransformCom->Get_WorldMatrix();
			_Vector pos = mat.r[3];
			testMesh.vFixedPosition = pos;
			testMesh.vNoisePushingDir = _float2(-1, 0);
			GETPARTICLE->Create_MeshInst_DESC(testMesh, m_eNowSceneNum);

			testMesh.iMaskingTextureIndex = 200;
			GETPARTICLE->Create_MeshInst_DESC(testMesh, m_eNowSceneNum);

			testMesh.iMaskingTextureIndex = 144;
			GETPARTICLE->Create_MeshInst_DESC(testMesh, m_eNowSceneNum);


		}



	}

	if (KEYDOWN(DIK_C))
	{
	}



	return S_OK;

}

HRESULT CMonster_Vayusura_Leader::Play_SpawnEffect()
{
	return S_OK;
}

void CMonster_Vayusura_Leader::Set_Play_MeshEffect_Colbullet(bool bParticle)
{
	if (bParticle)
	{
	//	Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_MONSTER_VL_Cash2, m_pTextureParticleTransform_Demo2);
	//	Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_MONSTER_VL_Cash1, m_pTextureParticleTransform_Demo2);
		Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_MONSTER_VL_Cash3, m_pTextureParticleTransform_Demo2);

		m_dealyEffect_Rain = true;
		m_dealyEffect_Time = 1.0f;

		

	}
	if (m_BulletMeshEffect)
		m_BulletMeshEffect->Set_IsDead();
	m_BulletMeshEffect = nullptr;
	Safe_Release(m_BulletObj);

}

HRESULT CMonster_Vayusura_Leader::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_Monster_Vayusura_Leader), TAG_COM(Com_Model), (CComponent**)&m_pModel));
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
	HpDesc.m_Dimensions = 1.2f;
	m_fMaxHP = 15.f;
	m_fHP = m_fMaxHP;
	g_pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&m_pHPUI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_HpUI), &HpDesc);

	CDissolve::DISSOLVEDESC DissolveDesc;
	DissolveDesc.pModel = m_pModel;
	DissolveDesc.eDissolveModelType = CDissolve::DISSOLVE_ANIM;
	DissolveDesc.pShader = m_pShaderCom;
	DissolveDesc.RampTextureIndex = 8;
	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Dissolve), TAG_COM(Com_Dissolve), (CComponent**)&m_pDissolve, &DissolveDesc));

	SetUp_Collider();

	return S_OK;
}

HRESULT CMonster_Vayusura_Leader::Adjust_AnimMovedTransform(_double dDeltaTime)
{
	_uint iNowAnimIndex = m_pModel->Get_NowAnimIndex();
	_double PlayRate = m_pModel->Get_PlayRate();

	if (iNowAnimIndex != m_iOldAnimIndex || PlayRate > 0.95)
	{
		m_iAdjMovedIndex = 0;

		m_bLookAtOn = true;
		m_EffectAdjust = 0;

		m_iSoundIndex = 0;
		m_bStopCoolTimeOn = false;
		if (PlayRate > 0.95 && m_bIOnceAnimSwitch == true)
		{
			m_bIOnceAnimSwitch = false;
			m_dOnceCoolTime = 0;
			m_dInfinity_CoolTime = 0;
		}
	}

	if (PlayRate <= 0.95) //애니메이션의 비율 즉, 0.98은 거의 끝나가는 시점
	{


		switch (iNowAnimIndex)
		{
		case 0:
		{
			if (m_iSoundIndex == 0 && PlayRate >= 0.3777)
			{
				g_pGameInstance->Play3D_Sound(TEXT("EH_Vayusura_Dive_01.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_SUBEFFECT, 0.3f);
				m_iSoundIndex++;
			}
			break;
		}
		case 1:
		{
			if (m_iSoundIndex == 0 && PlayRate >= 0.4166)
			{
				g_pGameInstance->Play3D_Sound(TEXT("EH_Vayusura_Dive_01.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_SUBEFFECT, 0.3f);
				m_iSoundIndex++;
			}
			break;
		}
		case 2:
		{
			if (m_iSoundIndex == 0 && PlayRate >= 0.3888)
			{
				g_pGameInstance->Play3D_Sound(TEXT("EH_Vayusura_Dive_01.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_SUBEFFECT, 0.3f);
				m_iSoundIndex++;
			}
			break;
		}
		case 4:
		{
			if (m_iAdjMovedIndex == 0)
			{
				m_dAcceleration = 1;
				m_iAdjMovedIndex++;
			}
			if (PlayRate > 0 && PlayRate <= 0.95)
			{
				m_bIOnceAnimSwitch = true;

				if (m_iSoundIndex == 0)
				{
					g_pGameInstance->Play3D_Sound(TEXT("EH_Vayusura_Screech_02.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_MONSTER, 0.5f);
					m_iSoundIndex++;
				}
			}
			else if (PlayRate >= 0.95)
			{
				m_bIOnceAnimSwitch = false;
			}


			break;
		}
		case 6:
		{
			_float Value = g_pGameInstance->Easing_Return(TYPE_Linear, TYPE_Linear, 0, 1, (_float)PlayRate, 0.9f);
			Value = max(min(Value, 1.f), 0.f);
			Set_LimLight_N_Emissive(_float4(1.0f, 0.0f, 0.0f, Value), _float4(Value, Value*0.7f, Value, 0.9f));

			if (m_iAdjMovedIndex == 0 && PlayRate > 0) // 이렇게 되면 이전 애니메이션에서 보간되는 시간 끝나자 마자 바로 들어옴 즉, PlayRate의 0은 >= 하지말고 >로 하셈
			{
				m_bAttackFrieOn = false;
				m_bAttackCancelOn = false;

				CMonster_Bullet_Universal::MONSTER_BULLET_UNIVERSALDESC Monster_BulletDesc;

				Monster_BulletDesc.iBulletMeshNumber = CMonster_Bullet_Universal::VAYUSURA_LEADER_BULLET;
				Monster_BulletDesc.fSpeedPerSec = 10;
				Monster_BulletDesc.fScale = _float3(0.1f, 0.1f, 0.1f);

				Monster_BulletDesc.Object_Transform = m_pTransformCom;
				Monster_BulletDesc.fPositioning = _float3(-0.0016327f, 0.0032063f, 0.023993f);


				Monster_BulletDesc.Object = this;

				Monster_BulletDesc.dDuration = 15;

				Monster_BulletDesc.bBornAttachOn = true;
				Monster_BulletDesc.pBoneName = "heel_twist_01_r";

				
				Set_Bullet(g_pGameInstance->Add_GameObject_GetObject(m_eNowSceneNum, TAG_LAY(Layer_MonsterBullet), TAG_OP(Prototype_Object_Monster_Bullet_Universal), &Monster_BulletDesc));
				Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_MONSTER_VL_Cash0, m_pTextureParticleTransform_Demo2);
				m_BulletMeshEffect = (CGameObject*)GETPARTICLE->GetMeshEffect();
				if (m_BulletMeshEffect == nullptr)
					DEBUGBREAK;

				//g_pGameInstance->Play3D_Sound(TEXT("EH_M1_1145.mp3"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_MONSTER, 0.5f);


				m_iAdjMovedIndex++; //애니메이션이 동작할 때 한번만 발동시키기 위해 ++시킨다.
			}

			else if (m_iAdjMovedIndex == 1 && PlayRate >= 0.792411)
			{
				m_bAttackFrieOn = true;
			}

			if (m_EffectAdjust == 0 && PlayRate > 0.1f)
			{
				m_EffectAdjust++;
			}
			if (m_iSoundIndex == 0)
			{
				g_pGameInstance->Play3D_Sound(TEXT("EH_M1_1215.mp3"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_MONSTER, 0.5f);
				m_iSoundIndex++;
			}
			else if (m_iSoundIndex == 1 && PlayRate >= 0.6987)
			{
				g_pGameInstance->Play3D_Sound(TEXT("EH_M1_1290.mp3"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_SUBEFFECT, 0.5f);
				m_iSoundIndex++;
			}
		}
			break;
		}

	}

	m_iOldAnimIndex = iNowAnimIndex;
	return S_OK;
}

CMonster_Vayusura_Leader * CMonster_Vayusura_Leader::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CMonster_Vayusura_Leader*	pInstance = NEW CMonster_Vayusura_Leader(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CMonster_Vayusura_Leader");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMonster_Vayusura_Leader::Clone(void * pArg)
{
	CMonster_Vayusura_Leader*	pInstance = NEW CMonster_Vayusura_Leader(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CMonster_Vayusura_Leader");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMonster_Vayusura_Leader::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModel);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pHPUI);
	Safe_Release(m_pDissolve);

	Safe_Release(m_pTextureParticleTransform_Demo1);
	Safe_Release(m_pTextureParticleTransform_Demo2);
	Safe_Release(m_pTextureParticleTransform_Demo3);
	Safe_Release(m_pTextureParticleTransform_Demo4);
	Safe_Release(m_BulletObj);
	
}