#include "stdafx.h"
#include "..\public\Monster_Tezabsura_Minion.h"
#include "Monster_Bullet_Universal.h"
#include "HpUI.h"

CMonster_Tezabsura_Minion::CMonster_Tezabsura_Minion(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CMonster(pDevice, pDeviceContext)
{
}

CMonster_Tezabsura_Minion::CMonster_Tezabsura_Minion(const CMonster_Tezabsura_Minion & rhs)
	: CMonster(rhs)
{
}

HRESULT CMonster_Tezabsura_Minion::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));


	return S_OK;
}

HRESULT CMonster_Tezabsura_Minion::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components());

	if (pArg != nullptr)
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, *((_float3*)pArg));

	m_pTransformCom->Rotation_CW(XMVectorSet(0, 1, 0, 0), XMConvertToRadians(170));


	SetUp_Info();


	m_fJumpPower = 4.5f;


	//////////////////testPosition
	m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, _float3(216.357f, 29.2f, 188.583f));

	m_pNavigationCom->FindCellIndex(m_pTransformCom->Get_MatrixState(CTransform::STATE_POS));

	return S_OK;
}

_int CMonster_Tezabsura_Minion::Update(_double dDeltaTime)
{

	if (__super::Update(dDeltaTime) < 0)return -1;

	//마지막 인자의 bBlockAnimUntilReturnChange에는 true로 시작해서 정상작동이 된다면 false가 된다.
	//m_pModel->Change_AnimIndex();
	//m_pModel->Change_AnimIndex_ReturnTo(); //어떤 애니메이션을 돌리고 특정 애니메이션으로 보냄
	//m_pModel->Change_AnimIndex_ReturnTo_Must(); //중간에 애니메이션을 캔슬하고 다른 애니메이션을 동작시킴
	//m_pModel->Change_AnimIndex_UntilTo(); //1~5까지 돌린다고 명령어를 입력하면 1~5까지 돌아감
	//m_pModel->Change_AnimIndex_UntilNReturn();//1~5까지 돌리고 난 이후 특정 애니메이션으로 Return 시킬 경우
	//m_pModel->Change_AnimIndex_UntilNReturn_Must(); //1~5까지 돌리고 난 이후 특정 애니메이션으로 Return 시킬 수 있지만 다른 애니메이션을 동작시킬 수 있음



	PlayAnim(dDeltaTime);
	Jumping(dDeltaTime);

	m_bIsOnScreen = g_pGameInstance->IsNeedToRender(m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS), m_fFrustumRadius);

	//if (m_pModel->Get_NowAnimIndex() == 8 ||m_pModel->Get_NowAnimIndex() == 9 || m_pModel->Get_NowAnimIndex() == 10)
	//{
	//FAILED_CHECK(m_pModel->Update_AnimationClip(dDeltaTime * m_dAcceleration * 1.6452f, m_bIsOnScreen));
	//}
	//else
	//{
	//FAILED_CHECK(m_pModel->Update_AnimationClip(dDeltaTime * m_dAcceleration, m_bIsOnScreen));
	//}

	FAILED_CHECK(m_pModel->Update_AnimationClip(dDeltaTime * m_dAcceleration, m_bIsOnScreen));

	FAILED_CHECK(Adjust_AnimMovedTransform(dDeltaTime));

	if (m_pHPUI != nullptr)
		m_pHPUI->Update(dDeltaTime);

	Update_Collider(dDeltaTime);

	return _int();
}

_int CMonster_Tezabsura_Minion::LateUpdate(_double dDeltaTime)
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
#endif

	if (m_pHPUI != nullptr)
		m_pHPUI->LateUpdate(dDeltaTime);

	return _int();
}

_int CMonster_Tezabsura_Minion::Render()
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

_int CMonster_Tezabsura_Minion::LateRender()
{
	if (__super::LateRender() < 0)
		return -1;

	return _int();
}

void CMonster_Tezabsura_Minion::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
	if (CollisionTypeID::CollisionType_Player == eConflictedObjCollisionType)
	{
		_Vector vDamageDir = XMVector3Normalize(pConflictedCollider->Get_ColliderPosition(iConflictedObjColliderIndex).XMVector() - m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS));
		pConflictedObj->Take_Damage(this, 1.f, vDamageDir, m_bOnKnockbackCol, m_fKnockbackColPower);
		pConflictedCollider->Set_Conflicted(1.f);
	}
}

_float CMonster_Tezabsura_Minion::Take_Damage(CGameObject * pTargetObject, _float fDamageAmount, _fVector vDamageDir, _bool bKnockback, _float fKnockbackPower)
{
	_uint iNowAnimIndex = m_pModel->Get_NowAnimIndex();

	if (iNowAnimIndex >= 8 && iNowAnimIndex <= 10)
		return _float();

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
		m_bKnockbackOn = true;
		m_iOncePattern = 40;

		XMStoreFloat3(&m_fKnockbackDir, vDamageDir);
	}

	if (m_fHP < 5 && m_iBoolOnce == 0)
	{
		m_iOncePattern = 41;
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

HRESULT CMonster_Tezabsura_Minion::SetUp_Info()
{

	m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, _float3(2.f, 0.f, 2.f));


	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	m_pPlayerTransform = static_cast<CTransform*>(pGameInstance->Get_Commponent_By_LayerIndex(m_eNowSceneNum, TAG_LAY(Layer_Player), TAG_COM(Com_Transform)));

	RELEASE_INSTANCE(CGameInstance);


	return S_OK;
}

HRESULT CMonster_Tezabsura_Minion::SetUp_Collider()
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
	tAttachedDesc.Initialize_AttachedDesc(this, "pelvis", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(-0.f, 0.11513f, -1.4121f));
	m_vecAttachedDesc.push_back(tAttachedDesc);



	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "head", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(0.f, 0.05226f, -2.3082f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pColliderCom->Set_ParantBuffer();


	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.5f, 1.5f, 1.5f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "spine_01", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(0.f, 0.088488f, -1.5709f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pColliderCom->Set_ParantBuffer();


	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "calf_r", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(-0.20107f, 0.07767f, -0.81758f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pColliderCom->Set_ParantBuffer();


	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.f, 1.f, 1.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "calf_l", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(0.20107f, 0.07767f, -0.81758f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pColliderCom->Set_ParantBuffer();


	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.5f, 0.5f, 0.5f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "lowerarm_r", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(-0.61184f, 0.26749f, -1.9888f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pColliderCom->Set_ParantBuffer();


	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.5f, 0.5f, 0.5f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "lowerarm_twist_02_r", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(-0.84824f, 0.23265f, -1.9768f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pColliderCom->Set_ParantBuffer();


	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.5f, 0.5f, 0.5f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "lowerarm_l", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(0.61184f, 0.26749f, -1.9888f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pColliderCom->Set_ParantBuffer();

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.5f, 0.5f, 0.5f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "lowerarm_twist_02_l", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(0.84824f, 0.23265f, -1.9768f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pColliderCom->Set_ParantBuffer();

	return S_OK;
}

HRESULT CMonster_Tezabsura_Minion::SetUp_Fight(_double dDeltaTime)
{
	m_fDistance = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS).Get_Distance(m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS));



	if (m_bLookAtOn)
	{
		//m_pTransformCom->LookAt(m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS));


		_Vector vTarget = XMVector3Normalize(m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS) - m_pTransformCom->Get_MatrixState(CTransform::STATE_POS));

		//m_pTransformCom->Turn_Dir(XMVector3Normalize(m_pTransformCom->Get_MatrixScale(CTransform::STATE_LOOK)*0.9 + vTarget* 0.1));
		m_pTransformCom->Turn_Dir(vTarget, 0.9f);
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

HRESULT CMonster_Tezabsura_Minion::Update_Collider(_double dDeltaTime)
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

HRESULT CMonster_Tezabsura_Minion::PlayAnim(_double dDeltaTime)
{
	SetUp_Fight(dDeltaTime);

	CoolTime_Manager(dDeltaTime);

	if (true == m_bIOnceAnimSwitch)
	{
		Once_AnimMotion(dDeltaTime);
		_uint i = m_pModel->Get_NowAnimIndex();
		switch (i)
		{
		case 1:
			m_pModel->Change_AnimIndex(m_iOnceAnimNumber, 0.f);
			break;
		case 9:
			m_pModel->Change_AnimIndex(m_iOnceAnimNumber, 0.f);
			break;
		default:
			m_pModel->Change_AnimIndex(m_iOnceAnimNumber, 0.15f); //1도 넣으면 좋을듯 // (i == 1 || i == 8 || i == 9 || i == 10) ? 0 :
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

HRESULT CMonster_Tezabsura_Minion::CoolTime_Manager(_double dDeltaTime)
{
	//한번만 동작하는 애니메이션

	m_dOnceCoolTime += dDeltaTime;
	m_dSpecial_CoolTime += dDeltaTime;

	if (m_dOnceCoolTime > 2 || m_bComboAnimSwitch == true)
	{
		m_dOnceCoolTime = 0;
		m_dInfinity_CoolTime = 0;

		if (m_bComboAnimSwitch == false)
		{
			Special_Trigger(dDeltaTime);
		}
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
		m_iInfinityPattern = rand() % 7;


		m_dInfinity_CoolTime = 0;
	}

	return S_OK;
}

HRESULT CMonster_Tezabsura_Minion::Once_AnimMotion(_double dDeltaTime)
{
	switch (m_iOncePattern)
	{
	case 0:
		m_iOnceAnimNumber = 12; //Attack
		m_bComboAnimSwitch = false;
		break;
	case 1:
		m_iOnceAnimNumber = 8; //JumpStart
		m_bComboAnimSwitch = true;
		break;
	case 2:
		m_iOnceAnimNumber = 9; //JumpLoop
		m_bComboAnimSwitch = true;
		break;
	case 3:
		m_iOnceAnimNumber = 10; //JumpEnd
		m_bComboAnimSwitch = true;
		break;
	case 4:
		m_iOnceAnimNumber = 12; //Attack
		m_bComboAnimSwitch = false;
		break;
	case 5:
		m_iOnceAnimNumber = 8; //JumpStart
		m_bComboAnimSwitch = true;
		break;
	case 6:
		m_iOnceAnimNumber = 9; //JumpLoop
		m_bComboAnimSwitch = true;
		break;
	case 7:
		m_iOnceAnimNumber = 10; //JumpEnd
		m_bComboAnimSwitch = true;
		break;
	case 8:
		m_iOnceAnimNumber = 12; //Attack
		m_bComboAnimSwitch = false;
		break;

	case 30:
		m_iOnceAnimNumber = 11;
		break;
	case 40:
		m_iOnceAnimNumber = 7;
		break;
	case 41:
		m_iOnceAnimNumber = 3;
		break;
	}

	return S_OK;
}

HRESULT CMonster_Tezabsura_Minion::Pattern_Change()
{

	m_iOncePattern += 1;

	if (m_iOncePattern > 8)
	{
		m_iOncePattern = 0; //OncePattern Random
	}


	return S_OK;
}

HRESULT CMonster_Tezabsura_Minion::Infinity_AnimMotion(_double dDeltaTime)
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
	}

	return S_OK;
}

HRESULT CMonster_Tezabsura_Minion::Special_Trigger(_double dDeltaTime)
{


	if (m_fDistance < 2 && m_dSpecial_CoolTime > 10)
	{
		m_dSpecial_CoolTime = 0;
		m_dOnceCoolTime = 0;
		m_dInfinity_CoolTime = 0;

		m_bIOnceAnimSwitch = true;
		m_iOncePattern = 30;
	}


	return S_OK;
}

HRESULT CMonster_Tezabsura_Minion::Jumping(_double dDeltaTime)
{

	_uint iNowAnimIndex = m_pModel->Get_NowAnimIndex();
	_double PlayRate = m_pModel->Get_PlayRate();

	//if (iNowAnimIndex >= 8 && iNowAnimIndex <= 10) 이게 맞는것
	//{
	//	m_bLookAtOn = false;

	//	_uint TargetAnimIndex = iNowAnimIndex - 8;

	//	if (PlayRate <=0 && iNowAnimIndex == 8)
	//	{
	//		m_fJumpTempPos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
	//	}

	//	_float fJumpY = 0;
	//	if (PlayRate + TargetAnimIndex >= 0.5f && PlayRate + TargetAnimIndex <= 2.5f)
	//	{
	//		fJumpY  = GetSingle(CGameInstance)->Easing_Return(TYPE_SinOut, TYPE_QuadInOut, 0, m_fJumpPower, PlayRate + TargetAnimIndex - 0.5f, 2.f);
	//	}
	//	else
	//	{
	//		fJumpY = 0;
	//	}
	//	m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, m_fJumpTempPos.XMVector() + XMVectorSet(0, fJumpY, 0, 0));
	//}

	if (iNowAnimIndex >= 8 && iNowAnimIndex <= 10)
	{
		m_bLookAtOn = false;

		_uint TargetAnimIndex = iNowAnimIndex - 8;

		if (PlayRate <= 0 && iNowAnimIndex == 8)
		{
			m_fJumpTempPos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
		}

		_float fJumpY = 0;
		if (PlayRate + TargetAnimIndex >= 0.5f && PlayRate + TargetAnimIndex <= 2.5f)
		{
			fJumpY = GetSingle(CGameInstance)->Easing_Return(TYPE_SinOut, TYPE_QuadInOut, 0, m_fJumpPower, (_float)PlayRate + TargetAnimIndex - 0.5f, 2.f);

			m_pTransformCom->Move_Forward(dDeltaTime*0.8);
		}
		else
		{
			fJumpY = 0;
		}


		_float3		fPosition = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);

		fPosition.y = m_fJumpTempPos.y + fJumpY;
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, XMLoadFloat3(&fPosition));
	}

	return S_OK;
}

HRESULT CMonster_Tezabsura_Minion::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_Monster_Tezabsura_Minion), TAG_COM(Com_Model), (CComponent**)&m_pModel));
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

HRESULT CMonster_Tezabsura_Minion::Adjust_AnimMovedTransform(_double dDeltaTime)
{
	_uint iNowAnimIndex = m_pModel->Get_NowAnimIndex();
	_double PlayRate = m_pModel->Get_PlayRate();

	if (iNowAnimIndex != m_iOldAnimIndex || PlayRate > 0.95)
	{
		m_iAdjMovedIndex = 0;

		m_bLookAtOn = true;
		m_dAcceleration = 1;

		if (PlayRate > 0.95 && m_bIOnceAnimSwitch == true)
		{
			m_bIOnceAnimSwitch = false;
			m_dOnceCoolTime = 0;
			m_dInfinity_CoolTime = 0;
		}
	}

	if (PlayRate <= 0.95) //애니메이션의 비율 즉, 0.98은 거의 끝나가는 시점
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
		case 0:
		{
			if (PlayRate > 0)
			{
				m_bLookAtOn = false;
			}

			break;
		}
		case 1:
			if (PlayRate > 0)
			{
				m_pTransformCom->Move_Forward(dDeltaTime * 1.05, m_pNavigationCom);
			}
			break;
		case 3:
		{
			m_bLookAtOn = false;
			break;
		}
		case 7:
		{
			if (m_iAdjMovedIndex == 0 && PlayRate > 0 && m_bKnockbackOn == false)
			{
				m_dAcceleration = 0.7;
				m_iAdjMovedIndex++;
			}
			if (m_bKnockbackOn == true)
			{
				if (m_iAdjMovedIndex == 0 && PlayRate > 0)
				{
					m_bLookAtOn = false;
					m_dAcceleration = 0.7;
					m_iAdjMovedIndex++;
				}
				else if (0.f < PlayRate && PlayRate <= 0.8636)
				{
					if (PlayRate >= 0.32 && PlayRate <= 0.68)
						m_pTransformCom->Move_Backward(dDeltaTime* 0.8, m_pNavigationCom);

					m_fKnockbackDir.y = 0;

					m_pTransformCom->Turn_Dir(m_fKnockbackDir.XMVector(), 0.9f);
				}
			}
			break;
		}
		case 8:
		{
			if (m_iAdjMovedIndex == 0)
			{
				m_dAcceleration = 1.6452f;
				m_iAdjMovedIndex++;
			}
			break;
		}
		case 9:
		{
			if (m_iAdjMovedIndex == 0)
			{
				m_dAcceleration = 1.6452f;
				m_iAdjMovedIndex++;
			}
			break;
		}
		case 10:
		{
			if (m_iAdjMovedIndex == 0)
			{
				m_dAcceleration = 1.6452f;
				m_iAdjMovedIndex++;
			}
			break;
		}
		case 11:
			if (PlayRate >= 0.416666 && PlayRate <= 0.69444)
			{
				m_bLookAtOn = false;
				m_pTransformCom->Move_Forward(dDeltaTime * 1.5);
				m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, m_pNavigationCom->Get_NaviPosition(m_pTransformCom->Get_MatrixState(CTransform::STATE_POS)));
			}
			break;
		case 12:
			if (m_iAdjMovedIndex == 0 && PlayRate >= 0.57142)
			{
				m_bLookAtOn = false;

				CMonster_Bullet_Universal::MONSTER_BULLET_UNIVERSALDESC Monster_BulletDesc;

				Monster_BulletDesc.iBulletMeshNumber = CMonster_Bullet_Universal::TEZABSURA_MINION_BULLET;
				Monster_BulletDesc.fSpeedPerSec = 10;
				Monster_BulletDesc.fScale = _float3(0.75f, 0.75f, 0.75f);

				Monster_BulletDesc.Object_Transform = m_pTransformCom;
				Monster_BulletDesc.fPositioning = _float3(0.001f, 1.f, 1.5f);


				Monster_BulletDesc.Object = this;

				Monster_BulletDesc.dDuration = 15;

				Monster_BulletDesc.bBornAttachOn = true;
				Monster_BulletDesc.pBoneName = "jaw_01";

				FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_MonsterBullet), TAG_OP(Prototype_Object_Monster_Bullet_Universal), &Monster_BulletDesc));






				m_iAdjMovedIndex++;
			}
		default:
			break;
		}

	}

	m_iOldAnimIndex = iNowAnimIndex;
	return S_OK;
}

CMonster_Tezabsura_Minion * CMonster_Tezabsura_Minion::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CMonster_Tezabsura_Minion*	pInstance = NEW CMonster_Tezabsura_Minion(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CMonster_Tezabsura_Minion");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMonster_Tezabsura_Minion::Clone(void * pArg)
{
	CMonster_Tezabsura_Minion*	pInstance = NEW CMonster_Tezabsura_Minion(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CMonster_Tezabsura_Minion");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMonster_Tezabsura_Minion::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModel);
}
