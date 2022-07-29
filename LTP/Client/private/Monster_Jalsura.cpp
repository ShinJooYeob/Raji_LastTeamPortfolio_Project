#include "stdafx.h"
#include "..\public\Monster_Jalsura.h"
#include "Monster_Texture_Bullet.h"
#include "HpUI.h"


CMonster_Jalsura::CMonster_Jalsura(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CMonster(pDevice, pDeviceContext)
{
}

CMonster_Jalsura::CMonster_Jalsura(const CMonster_Jalsura & rhs)
	: CMonster(rhs)
{
}

HRESULT CMonster_Jalsura::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));


	return S_OK;
}

HRESULT CMonster_Jalsura::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components());

	if (pArg != nullptr)
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, *((_float3*)pArg));

	m_pTransformCom->Rotation_CW(XMVectorSet(0, 1, 0, 0), XMConvertToRadians(170));


	SetUp_Info();


	//////////////////testPosition
	m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, _float3(216.357f, 29.2f, 188.583f));

	m_pNavigationCom->FindCellIndex(m_pTransformCom->Get_MatrixState(CTransform::STATE_POS));

	return S_OK;
}

_int CMonster_Jalsura::Update(_double dDeltaTime)
{

	if (__super::Update(dDeltaTime) < 0)return -1;

	if (g_pGameInstance->Get_DIKeyState(DIK_X)&DIS_Down)
	{

	}

	if (m_fHP <= 0)
	{
		m_bLookAtOn = false;
		m_pDissolve->Update_Dissolving(dDeltaTime);
		m_pDissolve->Set_DissolveOn(false, 2.f);

		m_dDissolveTime += dDeltaTime;

		if (m_bDieSound == false && m_dDissolveTime >= 1.)
		{
			g_pGameInstance->Play3D_Sound(TEXT("EH_Wave_Jalsura_Die.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_MONSTER, 0.7f);
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

	if (m_pHPUI != nullptr)
		m_pHPUI->Update(dDeltaTime);

	Update_Collider(dDeltaTime);

	FAILED_CHECK(Update_ParticleTransform(dDeltaTime));

	return _int();
}

_int CMonster_Jalsura::LateUpdate(_double dDeltaTime)
{
	if (__super::LateUpdate(dDeltaTime) < 0)return -1;

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

	m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, m_pNavigationCom->Get_NaviPosition(m_pTransformCom->Get_MatrixState(CTransform::STATE_POS)));

	if (m_pHPUI != nullptr)
		m_pHPUI->LateUpdate(dDeltaTime);

	return _int();
}

_int CMonster_Jalsura::Render()
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

_int CMonster_Jalsura::LateRender()
{
	if (__super::LateRender() < 0)
		return -1;

	return _int();
}

void CMonster_Jalsura::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
	if (CollisionTypeID::CollisionType_Player == eConflictedObjCollisionType)
	{
		_Vector vDamageDir = XMVector3Normalize(pConflictedCollider->Get_ColliderPosition(iConflictedObjColliderIndex).XMVector() - m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS));
		pConflictedObj->Take_Damage(this, 1.f, vDamageDir, m_bOnKnockbackCol, m_fKnockbackColPower);
		pConflictedCollider->Set_Conflicted(1.f);
	}
}

_float CMonster_Jalsura::Take_Damage(CGameObject * pTargetObject, _float fDamageAmount, _fVector vDamageDir, _bool bKnockback, _float fKnockbackPower)
{
	_uint iNowAnimIndex = m_pModel->Get_NowAnimIndex();

	m_pHPUI->Set_ADD_HitCount((_int)fDamageAmount);
	m_fHP += -fDamageAmount;

	m_dSpecial_CoolTime = 0;
	m_dOnceCoolTime = 0;
	m_dInfinity_CoolTime = 0;

	m_bIOnceAnimSwitch = true;
	if (bKnockback == false)
	{
		m_bKnockbackOn = false;
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

	return _float();
}

HRESULT CMonster_Jalsura::SetUp_Info()
{

	m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, _float3(2.f, 0.f, 2.f));


	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	m_pPlayerTransform = static_cast<CTransform*>(pGameInstance->Get_Commponent_By_LayerIndex(m_eNowSceneNum, TAG_LAY(Layer_Player), TAG_COM(Com_Transform)));

	RELEASE_INSTANCE(CGameInstance);


	return S_OK;
}

HRESULT CMonster_Jalsura::SetUp_Fight(_double dDeltaTime)
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

HRESULT CMonster_Jalsura::SetUp_Collider()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_Collider), (CComponent**)&m_pColliderCom));


	/////////////////m_pColliderCom!@!@#$@!#$@#$@$!@%#$%@#$%%^^W@!
	COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(3.f, 3.f, 3.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	ATTACHEDESC tAttachedDesc;
	tAttachedDesc.Initialize_AttachedDesc(this, "sk_eye", _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), _float3(0.f, 0.f, 0.f));
	m_vecAttachedDesc.push_back(tAttachedDesc);

	return S_OK;
}

HRESULT CMonster_Jalsura::Update_Collider(_double dDeltaTime)
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

HRESULT CMonster_Jalsura::PlayAnim(_double dDeltaTime)
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

HRESULT CMonster_Jalsura::CoolTime_Manager(_double dDeltaTime)
{
	//한번만 동작하는 애니메이션

	m_dOnceCoolTime += dDeltaTime;

	if (m_dOnceCoolTime > 4)
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
		m_iInfinityPattern = rand() % 4;


		m_dInfinity_CoolTime = 0;
	}

	return S_OK;
}

HRESULT CMonster_Jalsura::Once_AnimMotion(_double dDeltaTime)
{

	switch (m_iOncePattern)
	{
	case 0:
		m_iOnceAnimNumber = 2;
		break;
	case 1:
		m_iOnceAnimNumber = 2;
		break;
	case 2:
		m_iOnceAnimNumber = 2;
		break;
	case 40:
		m_iOnceAnimNumber = 1;
		break;
	case 41:
		m_iOnceAnimNumber = 3;
		break;
	}

	return S_OK;
}

HRESULT CMonster_Jalsura::Pattern_Change()
{

	m_iOncePattern += 1;

	if (m_iOncePattern > 3)
	{
		m_iOncePattern = rand() % 3; //OncePattern Random
	}


	return S_OK;
}

HRESULT CMonster_Jalsura::Infinity_AnimMotion(_double dDeltaTime)
{
	switch (m_iInfinityPattern)
	{
	case 0:
		m_iInfinityAnimNumber = 0;
		break;
	case 1:
		m_pTransformCom->Move_Forward(dDeltaTime * 0.6, m_pNavigationCom);
		m_iInfinityAnimNumber = 0;
		break;
	case 2:
		m_pTransformCom->Move_Backward(dDeltaTime * 0.6, m_pNavigationCom);
		m_iInfinityAnimNumber = 0;
		break;
	case 3:
		m_pTransformCom->Move_Left(dDeltaTime * 0.6, m_pNavigationCom);
		m_iInfinityAnimNumber = 0;
		break;
	case 4:
		m_pTransformCom->Move_Right(dDeltaTime * 0.6, m_pNavigationCom);
		m_iInfinityAnimNumber = 0;
		break;
	}

	return S_OK;
}

HRESULT CMonster_Jalsura::Special_Trigger(_double dDeltaTime)
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

HRESULT CMonster_Jalsura::Ready_ParticleDesc()
{
		m_pTextureParticleTransform = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
		m_pMeshParticleTransform = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
		NULL_CHECK_RETURN(m_pTextureParticleTransform, E_FAIL);
		NULL_CHECK_RETURN(m_pMeshParticleTransform, E_FAIL);

		m_pTextureParticleTransform->Set_TurnSpeed(1);
		m_pMeshParticleTransform->Set_TurnSpeed(XMConvertToRadians(90));
		CUtilityMgr* pUtil = GetSingle(CUtilityMgr);

		//0
		m_vecTextureParticleDesc.push_back(pUtil->Get_TextureParticleDesc(L"Monster_Jalsura_Particle"));
		m_vecTextureParticleDesc[0].FollowingTarget = m_pTextureParticleTransform;
		//1
		m_vecTextureParticleDesc.push_back(pUtil->Get_TextureParticleDesc(L"Monster_Jalsura_Particle2"));
		m_vecTextureParticleDesc[1].FollowingTarget = m_pTextureParticleTransform;

		m_vecTextureParticleDesc[0].TotalParticleTime = m_vecTextureParticleDesc[1].TotalParticleTime = 2.f;
		m_vecTextureParticleDesc[0].ParticleSize = m_vecTextureParticleDesc[1].ParticleSize = _float3(0.3f, 0.5f, 0.3f);
		//2
		m_vecTextureParticleDesc.push_back(pUtil->Get_TextureParticleDesc(L"Monster_Jalsura_Particle3"));
		m_vecTextureParticleDesc[2].FollowingTarget = m_pMeshParticleTransform;
		m_vecTextureParticleDesc[2].iFollowingDir = FollowingDir_Look;

		

		//pUtil->Create_TextureInstance(m_eNowSceneNum, m_vecTextureParticleDesc[0]);
		//pUtil->Create_TextureInstance(m_eNowSceneNum, m_vecTextureParticleDesc[1]);
















		//////////////////////////////////////////////////////////////////////////
		// 0
		
		NONINSTNESHEFTDESC tNIMEDesc;


		tNIMEDesc.eMeshType = Prototype_Mesh_ConeMesh;
		tNIMEDesc.fMaxTime_Duration = 0.35f;
		tNIMEDesc.fAppearTime = 0.175f;

		tNIMEDesc.noisingdir = _float2(0, 1);

		tNIMEDesc.NoiseTextureIndex = 381;
		tNIMEDesc.MaskTextureIndex = 33;
		tNIMEDesc.iDiffuseTextureIndex = 338;
		tNIMEDesc.m_iPassIndex = 19;
		tNIMEDesc.vEmissive = _float4(1, 0.5f, 1.f, 0);
		tNIMEDesc.vLimLight = _float4(1, 1, 1, 1);
		tNIMEDesc.NoiseTextureIndex = 381;
		tNIMEDesc.vColor = _float3(1.0, 0, 0);

		tNIMEDesc.RotAxis = FollowingDir_Up;
		tNIMEDesc.RotationSpeedPerSec = -1080.f;
		tNIMEDesc.vSize = _float3(0.75f, -0.075f, 0.75f);

		m_vecNonMeshParticleDesc.push_back(tNIMEDesc);

	return S_OK;
}

HRESULT CMonster_Jalsura::Update_ParticleTransform(_double fDeltaTime)
{
	//fTransformAngle += (_float)fDeltaTime * 1080.f;
	//
	m_pTextureParticleTransform->Set_Matrix(m_pTransformCom->Get_WorldMatrix());
	m_pTextureParticleTransform->MovetoDir_bySpeed(XMVectorSet(0, 1, 0, 0), 1.5f, 1.f);
	m_pTextureParticleTransform->MovetoDir_bySpeed(m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK), 0.5f, 1.f);

	m_pMeshParticleTransform->Set_Matrix(m_pTextureParticleTransform->Get_WorldMatrix());
	m_pMeshParticleTransform->Turn_CW(m_pMeshParticleTransform->Get_MatrixState(CTransform::STATE_RIGHT), 0.999f);

	//m_pTextureParticleTransform->Turn_CW(m_pTextureParticleTransform->Get_MatrixState(CTransform::STATE_LOOK), XMConvertToRadians(fTransformAngle));
	//
	return S_OK;
}



HRESULT CMonster_Jalsura::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_Monster_Jalsura), TAG_COM(Com_Model), (CComponent**)&m_pModel));
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


	CDissolve::DISSOLVEDESC DissolveDesc;
	DissolveDesc.pModel = m_pModel;
	DissolveDesc.eDissolveModelType = CDissolve::DISSOLVE_ANIM;
	DissolveDesc.pShader = m_pShaderCom;
	DissolveDesc.RampTextureIndex = 10;
	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Dissolve), TAG_COM(Com_Dissolve), (CComponent**)&m_pDissolve, &DissolveDesc));


	SetUp_Collider();

	return S_OK;
}

HRESULT CMonster_Jalsura::Adjust_AnimMovedTransform(_double dDeltaTime)
{
	_uint iNowAnimIndex = m_pModel->Get_NowAnimIndex();
	_double PlayRate = m_pModel->Get_PlayRate();

	if (iNowAnimIndex != m_iOldAnimIndex || PlayRate > 0.98)
	{
		m_iAdjMovedIndex = 0;

		m_bLookAtOn = true;
		m_dAcceleration = 1;

		m_iSoundIndex = 0;

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
		case 0:
		{
			m_dSoundTime += dDeltaTime;
			if (m_dSoundTime >= 1)
			{
				g_pGameInstance->Play3D_Sound(TEXT("EH_Jalsura_Secondary_Wing_Flap_02.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_MONSTER, 0.7f);
				m_dSoundTime = 0;
			}
			break;
		}
		case 1:
		{
			if (m_iAdjMovedIndex == 0 && PlayRate > 0 && m_bKnockbackOn == false)
			{
				m_dAcceleration = 0.7;
				m_iAdjMovedIndex++;

				if (m_iSoundIndex == 0)
				{
					g_pGameInstance->Play3D_Sound(TEXT("EH_Jalsura_Ground_Hit_02.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_MONSTER, 0.5f);
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
						g_pGameInstance->Play3D_Sound(TEXT("EH_Jalsura_Ground_Hit_02.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_MONSTER, 0.5f);
						m_iSoundIndex++;
					}
				}
				else if (0.f < PlayRate && PlayRate <= 0.8)
				{
					if (PlayRate >= 0 && PlayRate <= 0.8)
						m_pTransformCom->Move_Backward(dDeltaTime* 0.5, m_pNavigationCom);

					m_fKnockbackDir.y = 0;

					m_pTransformCom->Turn_Dir(m_fKnockbackDir.XMVector(), 0.9f);
				}
			}
			break;
		}
		case 2:
		{
#define JalsuraColorChangeTime 0.15f
			if (PlayRate <= 0 && m_iAdjMovedIndex == 0)
			{
				CUtilityMgr* pUtil = GetSingle(CUtilityMgr);

				fRimLightPassedTime = 0;
				vOldRimLightColor = _float4(0);
				vTargetRimLightColor = pUtil->RandomFloat3(0,1);


				pUtil->Create_TextureInstance(m_eNowSceneNum, m_vecTextureParticleDesc[0]);
				pUtil->Create_TextureInstance(m_eNowSceneNum, m_vecTextureParticleDesc[1]);
				pUtil->Create_TextureInstance(m_eNowSceneNum, m_vecTextureParticleDesc[2]);
				m_iAdjMovedIndex++;
			}
			else if (PlayRate < 0.4842105)
			{
				fRimLightPassedTime += (_float)dDeltaTime;

				if (fRimLightPassedTime < JalsuraColorChangeTime)
				{
					_float3 vRimLightColor = g_pGameInstance->Easing_Vector(TYPE_Linear,
						vOldRimLightColor, vTargetRimLightColor, fRimLightPassedTime, JalsuraColorChangeTime);

					_float Rate = _float(PlayRate / 0.4842105f);

					Set_LimLight_N_Emissive(_float4(vRimLightColor, Rate),
						_float4(Rate, Rate *0.2f, Rate, 1));
				}
				else
				{

					vOldRimLightColor = vTargetRimLightColor;
					vTargetRimLightColor = GetSingle(CUtilityMgr)->RandomFloat3(0, 1);

					_float Rate = _float(PlayRate / 0.4842105f);

					Set_LimLight_N_Emissive(_float4(_float3(vOldRimLightColor), Rate),
						_float4(Rate, Rate *0.2f, Rate, 1));
				}

			}
			else if (m_iAdjMovedIndex == 1)
			{
				m_bLookAtOn = false;
				vOldRimLightColor = vTargetRimLightColor = Get_LimLightValue();

				CMonster_Texture_Bullet::MONSTER_TEXTURE_BULLETDESC Monster_Texture_BulletDesc;

				Monster_Texture_BulletDesc.iBulletTextureNumber = CMonster_Texture_Bullet::JALSURA_BULLET;
				Monster_Texture_BulletDesc.fSpeedPerSec = 10;
				Monster_Texture_BulletDesc.fScale = _float3(1.f, 1.f, 1.5f);

				Monster_Texture_BulletDesc.Object_Transform = m_pTransformCom;
				Monster_Texture_BulletDesc.fPositioning = _float3(0.f,1.2f, -0.3f);
				Monster_Texture_BulletDesc.vColor = _float4(_float3(vOldRimLightColor), 0.5f);
				Monster_Texture_BulletDesc.Object = this;

				Monster_Texture_BulletDesc.dDuration = 1;

				FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_MonsterBullet), TAG_OP(Prototype_Object_Monster_Texture_Bullet), &Monster_Texture_BulletDesc));

				//////////////////EH Collider


				ZeroMemory(&Monster_Texture_BulletDesc, sizeof(CMonster_Texture_Bullet::MONSTER_TEXTURE_BULLETDESC));
				Monster_Texture_BulletDesc.iBulletTextureNumber = CMonster_Texture_Bullet::NONTEXTURE_OBB;
				Monster_Texture_BulletDesc.fSpeedPerSec = 30;
				Monster_Texture_BulletDesc.fScale = _float3(11.5f, 1.f, 1.f);

				Monster_Texture_BulletDesc.Object_Transform = m_pTransformCom;
				Monster_Texture_BulletDesc.fPositioning = _float3(0.f, 1.f, 0.f);
				Monster_Texture_BulletDesc.Object = this;

				Monster_Texture_BulletDesc.dDuration = 1;

				FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_MonsterBullet), TAG_OP(Prototype_Object_Monster_Texture_Bullet), &Monster_Texture_BulletDesc));
				////////////////////


				m_vecNonMeshParticleDesc[0].vPosition = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS) +
					m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK) * 1.f +
					m_pTransformCom->Get_MatrixState(CTransform::STATE_UP) * 1.25f;
				m_vecNonMeshParticleDesc[0].vLookDir = m_vecNonMeshParticleDesc[0].vPosition.XMVector() - (
					m_pTransformCom->Get_MatrixState(CTransform::STATE_POS) -
					m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK) * 1.f +
					m_pTransformCom->Get_MatrixState(CTransform::STATE_UP) * 100000.f);

				m_vecNonMeshParticleDesc[0].vLimLight = _float4(_float3(vOldRimLightColor), 1);


				g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_PlayerEffect), TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonMeshParticleDesc[0]);

				fRimLightPassedTime = 0;

				FAILED_CHECK(g_pGameInstance->Play3D_Sound(TEXT("EH_Jalsura_Lazer_03.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_MONSTER, 1.f));

				m_iAdjMovedIndex++;
			}
			else
			{
				//fRimLightPassedTime += (_float)dDeltaTime;

				_float3 vRimLightColor = g_pGameInstance->Easing_Vector(TYPE_Linear,
					vOldRimLightColor, _float3(0), _float(PlayRate - 0.4842105), 0.5157895f);

				_float Rate = 1.f - _float((PlayRate - 0.4842105f) / 0.5157895f);

				Set_LimLight_N_Emissive(_float4(vRimLightColor, Rate),
					_float4(Rate, Rate* 0.2f, Rate, 1));
			}

			if (m_iSoundIndex == 0 && PlayRate > 0)
			{
				g_pGameInstance->Play3D_Sound(TEXT("EH_Wave_JalsuraAttack_Movement.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_SUBEFFECT, 0.5f);
				m_iSoundIndex++;
			}else if (m_iSoundIndex == 1 && PlayRate >= 0.2105)
			{
				g_pGameInstance->Play3D_Sound(TEXT("EH_Wave_JalsuraAttack_Laser.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_SUBEFFECT, 0.5f);
				m_iSoundIndex++;
			}else if (m_iSoundIndex == 2 && PlayRate >= 0.7368)
			{
				g_pGameInstance->Play3D_Sound(TEXT("EH_Jalsura_Steam_01.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_SUBEFFECT, 0.5f);
				m_iSoundIndex++;
			}
			break;
		}
		case 3:
		{
			m_bLookAtOn = false;
			break;
		}
		}

		
	}

	m_iOldAnimIndex = iNowAnimIndex;
	return S_OK;
}

CMonster_Jalsura * CMonster_Jalsura::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CMonster_Jalsura*	pInstance = NEW CMonster_Jalsura(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CMonster_Jalsura");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMonster_Jalsura::Clone(void * pArg)
{
	CMonster_Jalsura*	pInstance = NEW CMonster_Jalsura(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CMonster_Jalsura");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMonster_Jalsura::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModel);


	Safe_Release(m_pColliderCom);
	Safe_Release(m_pHPUI);
	Safe_Release(m_pDissolve);

	Safe_Release(m_pTextureParticleTransform);
	Safe_Release(m_pMeshParticleTransform);
}