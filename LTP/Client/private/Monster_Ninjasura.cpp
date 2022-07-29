#include "stdafx.h"
#include "..\public\Monster_Ninjasura.h"
#include "Monster_Bullet_Universal.h"

CMonster_Ninjasura::CMonster_Ninjasura(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CMonster(pDevice, pDeviceContext)
{
}

CMonster_Ninjasura::CMonster_Ninjasura(const CMonster_Ninjasura & rhs)
	: CMonster(rhs)
{
}

HRESULT CMonster_Ninjasura::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));


	return S_OK;
}

HRESULT CMonster_Ninjasura::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components());

	if (pArg != nullptr)
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, *((_float3*)pArg));


	m_pTransformCom->Scaled_All(_float3(1.5f, 1.5f, 1.5f));
	m_pTransformCom->Rotation_CW(XMVectorSet(0, 1, 0, 0), XMConvertToRadians(170));


	SetUp_Info();


	return S_OK;
}

_int CMonster_Ninjasura::Update(_double dDeltaTime)
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

	m_bIsOnScreen = g_pGameInstance->IsNeedToRender(m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS), m_fFrustumRadius);
	FAILED_CHECK(m_pModel->Update_AnimationClip(dDeltaTime * m_dAcceleration, m_bIsOnScreen));
	FAILED_CHECK(Adjust_AnimMovedTransform(dDeltaTime));

	//////////////Motion Test
	m_pMotionTrail->Update_MotionTrail(dDeltaTime);
	/////////////////////////
	return _int();
}

_int CMonster_Ninjasura::LateUpdate(_double dDeltaTime)
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

	FAILED_CHECK(m_pRendererCom->Add_ShadowGroup(CRenderer::SHADOW_ANIMMODEL, this, m_pTransformCom, m_pShaderCom, m_pModel));
	m_vOldPos = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS);

	return _int();
}

_int CMonster_Ninjasura::Render()
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

_int CMonster_Ninjasura::LateRender()
{
	if (__super::LateRender() < 0)
		return -1;

	return _int();
}

void CMonster_Ninjasura::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
	if (CollisionTypeID::CollisionType_Player == eConflictedObjCollisionType)
	{
		_Vector vDamageDir = XMVector3Normalize(pConflictedCollider->Get_ColliderPosition(iConflictedObjColliderIndex).XMVector() - m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS));
		pConflictedObj->Take_Damage(this, 1.f, vDamageDir, m_bOnKnockbackCol, m_fKnockbackColPower);
		pConflictedCollider->Set_Conflicted(1.f);
	}
}

_float CMonster_Ninjasura::Take_Damage(CGameObject * pTargetObject, _float fDamageAmount, _fVector vDamageDir, _bool bKnockback, _float fKnockbackPower)
{
	if (0 >= m_fHP)
	{
		return -1.f;
	}

	return _float();
}

HRESULT CMonster_Ninjasura::SetUp_Info()
{

	m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, _float3(2.f, 0.f, 2.f));


	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	m_pPlayerTransform = static_cast<CTransform*>(pGameInstance->Get_Commponent_By_LayerIndex(m_eNowSceneNum, TAG_LAY(Layer_Player), TAG_COM(Com_Transform)));

	RELEASE_INSTANCE(CGameInstance);


	return S_OK;
}

HRESULT CMonster_Ninjasura::SetUp_Fight(_double dDeltaTime)
{
	m_fDistance = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS).Get_Distance(m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS));


	if (m_fDistance < 1)
	{
		switch (m_iInfinityAnimNumber)
		{
		case 1:
			m_pTransformCom->Move_Backward(dDeltaTime * 0.6);
			break;
		case 21:
			m_pTransformCom->Move_Backward(dDeltaTime * 1.2);
			break;
		default:
			m_pTransformCom->Move_Backward(dDeltaTime);
			break;

		}
	}

	if (m_bLookAtOn)
	{
		//m_pTransformCom->LookAt(m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS));


		_Vector vTarget = XMVector3Normalize(m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS) - m_pTransformCom->Get_MatrixState(CTransform::STATE_POS));

		//m_pTransformCom->Turn_Dir(XMVector3Normalize(m_pTransformCom->Get_MatrixScale(CTransform::STATE_LOOK)*0.9 + vTarget* 0.1));
		m_pTransformCom->Turn_Dir(vTarget, 0.9f);
	}



	return S_OK;
}

HRESULT CMonster_Ninjasura::PlayAnim(_double dDeltaTime)
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
		case 3:
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
			m_pModel->Change_AnimIndex(m_iOnceAnimNumber, 0.2f);
			break;
		default:
			m_pModel->Change_AnimIndex(m_iOnceAnimNumber,0.15f);
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

HRESULT CMonster_Ninjasura::CoolTime_Manager(_double dDeltaTime)
{
	//한번만 동작하는 애니메이션

	m_dOnceCoolTime += dDeltaTime;
	m_dSpecial_CoolTime += dDeltaTime;

	if (m_dOnceCoolTime > 2 || m_bComboAnimSwitch == true)
	{
		m_dOnceCoolTime = 0;
		m_dInfinity_CoolTime = 0;

		//if (m_bComboAnimSwitch == false)
		//{
		//	Special_Trigger(dDeltaTime);
		//}
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
		//m_iInfinityPattern = rand() % 7;


		m_dInfinity_CoolTime = 0;
		m_dOnceCoolTime += 10;
	}

	return S_OK;
}

HRESULT CMonster_Ninjasura::Once_AnimMotion(_double dDeltaTime)
{
	switch (m_iOncePattern)
	{
	case 0:
		m_iOnceAnimNumber = 20;
		m_bComboAnimSwitch = true;
		break;
	case 1:
		m_iOnceAnimNumber = 2; //Rush Ready
		m_bComboAnimSwitch = true;
		break;
	case 2:
		m_iOnceAnimNumber = 3; //Rush start
		m_bComboAnimSwitch = true;
		break;
	case 3:
		m_iOnceAnimNumber = 11; //Attack1 Ready
		m_bComboAnimSwitch = true;
		break;
	case 4:
		m_iOnceAnimNumber = 12; //Attack1 Fire
		m_bComboAnimSwitch = false;
		break;
	case 5:
		m_iOnceAnimNumber = 20;
		m_bComboAnimSwitch = true;
		break;
	case 6:
		m_iOnceAnimNumber = 2; //Rush Ready
		m_bComboAnimSwitch = true;
		break;
	case 7:
		m_iOnceAnimNumber = 3; //Rush start
		m_bComboAnimSwitch = true;
		break;
	case 8:
		m_iOnceAnimNumber = 19; //throw Knife
		m_bComboAnimSwitch = false;
		break;
	case 9:
		m_iOnceAnimNumber = 20;
		m_bComboAnimSwitch = true;
		break;
	case 10:
		m_iOnceAnimNumber = 2; //Rush Ready
		m_bComboAnimSwitch = true;
		break;
	case 11:
		m_iOnceAnimNumber = 3; //Rush start
		m_bComboAnimSwitch = true;
		break;
	case 12:
		m_iOnceAnimNumber = 15; //Spinning start
		m_bComboAnimSwitch = true;
		break;
	case 13:
		m_iOnceAnimNumber = 16; //Spinning start
		m_bComboAnimSwitch = true;
		break;
	case 14:
		m_iOnceAnimNumber = 17; //Spinning start
		m_bComboAnimSwitch = true;
		break;
	case 15:
		m_iOnceAnimNumber = 16; //Spinning start
		m_bComboAnimSwitch = true;
		break;
	case 16:
		m_iOnceAnimNumber = 18; //Spinning start
		m_bComboAnimSwitch = false;
		break;
	case 17:
		m_iOnceAnimNumber = 20;
		m_bComboAnimSwitch = true;
		break;
	case 18:
		m_iOnceAnimNumber = 2; //Rush Ready
		m_bComboAnimSwitch = true;
		break;
	case 19:
		m_iOnceAnimNumber = 3; //Rush start
		m_bComboAnimSwitch = true;
		break;
	case 20:
		m_iOnceAnimNumber = 13; //Attack2 Ready
		m_bComboAnimSwitch = true;
		break;
	case 21:
		m_iOnceAnimNumber = 14; //Attack2 Fire
		m_bComboAnimSwitch = false;
		break;
	}

	return S_OK;
}

HRESULT CMonster_Ninjasura::Pattern_Change()
{

	m_iOncePattern += 1;

	if (m_iOncePattern >= 22)
	{
		m_iOncePattern = 0; //OncePattern Random
	}


	return S_OK;
}

HRESULT CMonster_Ninjasura::Infinity_AnimMotion(_double dDeltaTime)
{
	switch (m_iInfinityPattern)
	{
	case 0:
		m_iInfinityAnimNumber = 0;
		break;
	case 1:
		m_iInfinityAnimNumber = 0;
		break;
	case 2:
		m_iInfinityAnimNumber = 0;
		break;
	case 3:
		m_iInfinityAnimNumber = 0;
		break;
	case 4:
		m_iInfinityAnimNumber = 0;
		break;
	case 5:
		m_iInfinityAnimNumber = 0;
		break;
	case 6:
		m_iInfinityAnimNumber = 0;
		break;
	default:
		m_iInfinityAnimNumber = 0;
		break;
	}

	return S_OK;
}

HRESULT CMonster_Ninjasura::Special_Trigger(_double dDeltaTime)
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
HRESULT CMonster_Ninjasura::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_Monster_Ninjasura), TAG_COM(Com_Model), (CComponent**)&m_pModel));
	FAILED_CHECK(m_pModel->Change_AnimIndex(0));


	CTransform::TRANSFORMDESC tDesc = {};

	tDesc.fMovePerSec = 5;
	tDesc.fRotationPerSec = XMConvertToRadians(60);
	tDesc.vPivot = _float3(0, 0, 0);
	tDesc.fScalingPerSec = 1;

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));

	////////////Motion Test
	CMotionTrail::MOTIONTRAILDESC tMotionDesc;

	tMotionDesc.iNumTrailCount = 6;
	tMotionDesc.pModel = m_pModel;
	tMotionDesc.pShader = m_pShaderCom;
	tMotionDesc.iPassIndex = 5;

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_MotionTrail), TAG_COM(Com_MotionTrail), (CComponent**)&m_pMotionTrail, &tMotionDesc));
	///////////////////////////


	return S_OK;
}

HRESULT CMonster_Ninjasura::Adjust_AnimMovedTransform(_double dDeltaTime)
{
	_uint iNowAnimIndex = m_pModel->Get_NowAnimIndex();
	_double PlayRate = m_pModel->Get_PlayRate();

	if (iNowAnimIndex != m_iOldAnimIndex || PlayRate > 0.95)
	{
		m_iAdjMovedIndex = 0;

		m_bLookAtOn = false;

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
		switch (iNowAnimIndex)
		{
		case 3:
		{
			m_bLookAtOn = false;
			if (m_iAdjMovedIndex == 0)
			{
				m_iMoveNumber = rand() % 5;
				m_pTransformCom->LookAt(m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS));
				m_iAdjMovedIndex++;
			}
			if (PlayRate <= 0.1)
			{
				m_MotionTrailTime += dDeltaTime;
				if (m_MotionTrailTime > 0.03)
				{
					m_MotionTrailOn = true;

					CUtilityMgr* pUtil = GetSingle(CUtilityMgr);

					if (false == m_bTurnMotion)
					{
						XMStoreFloat4(&m_fDirection, XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK) * pUtil->RandomFloat(0, 1) + m_pTransformCom->Get_MatrixState(CTransform::STATE_RIGHT) * pUtil->RandomFloat(-1, 0)));
						m_bTurnMotion = true;
					}
					else {
						XMStoreFloat4(&m_fDirection, XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK) * pUtil->RandomFloat(0, 1) + m_pTransformCom->Get_MatrixState(CTransform::STATE_RIGHT) * pUtil->RandomFloat(0, 1)));
						m_bTurnMotion = false;
					}

					m_pMotionTrail->Add_MotionBuffer(m_pTransformCom->Get_WorldFloat4x4(), _float4(0.45f, 0.f, 1.f, 1.f), 1.f);
					m_MotionTrailTime = 0;
				}
				m_pTransformCom->MovetoDir_bySpeed(XMLoadFloat4(&m_fDirection), 60.f, dDeltaTime);
			}
			else {
				m_bIOnceAnimSwitch = false;
				m_MotionTrailOn = false;
			}

			break;
		}
		case 4:
		{
			m_bLookAtOn = false;

			if (m_iAdjMovedIndex == 0)
			{
				m_iMoveNumber = rand() % 2;

				m_iAdjMovedIndex++;

				m_pTransformCom->LookAt(m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS));
			}
			if (PlayRate <= 0.1)
			{

				switch (m_iMoveNumber)
				{
				case 0:
					m_pTransformCom->Move_Right(dDeltaTime * 4);
					break;
				case 1:
					m_pTransformCom->Move_Left(dDeltaTime * 4);
					break;
				default:
					break;
				}
			}
			break;
		}
		case 11:
		{
			if (PlayRate <= 0.75)
			{
				m_MotionTrailOn = true;
				m_pTransformCom->LookAt(m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS));

				m_MotionTrailTime += dDeltaTime;
				if (m_MotionTrailTime > 0.1)
				{
					m_pMotionTrail->Add_MotionBuffer(m_pTransformCom->Get_WorldFloat4x4(), _float4(0.45f, 0.f, 1.f, 1.f), 1.f);
					m_MotionTrailTime = 0;
				}
			}
			break;
		}
		case 12:
		{
			m_bLookAtOn = false;
			if (PlayRate <= 0.20)
			{
				m_MotionTrailTime += dDeltaTime;
				if (m_MotionTrailTime > 0.05)
				{
					m_pMotionTrail->Add_MotionBuffer(m_pTransformCom->Get_WorldFloat4x4(), _float4(0.45f, 0.f, 1.f, 1.f), 1.f);
					m_MotionTrailTime = 0;
				}

				m_pTransformCom->Move_Forward(dDeltaTime * 10);
			}
			else {
				m_MotionTrailOn = false;
				if (m_iAdjMovedIndex == 0)
				{
					m_pTransformCom->LookAt(m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS));
					m_iAdjMovedIndex++;
				}
			}
			break;
		}
		case 13:
		{
			if (PlayRate <= 0.83333)
			{
				m_MotionTrailOn = true;
				m_pTransformCom->LookAt(m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS));

				m_MotionTrailTime += dDeltaTime;
				if (m_MotionTrailTime > 0.1)
				{
					m_pMotionTrail->Add_MotionBuffer(m_pTransformCom->Get_WorldFloat4x4(), _float4(0.45f, 0.f, 1.f, 1.f), 1.f);
					m_MotionTrailTime = 0;
				}
			}
			break;
		}
		case 14:
		{
			m_bLookAtOn = false;
			if (PlayRate <= 0.20)
			{
				m_MotionTrailTime += dDeltaTime;
				if (m_MotionTrailTime > 0.05)
				{
					m_pMotionTrail->Add_MotionBuffer(m_pTransformCom->Get_WorldFloat4x4(), _float4(0.45f, 0.f, 1.f, 1.f), 1.f);
					m_MotionTrailTime = 0;
				}

				m_pTransformCom->Move_Forward(dDeltaTime * 5);
			}
			else {
				m_MotionTrailOn = false;
				if (m_iAdjMovedIndex == 0)
				{
					m_pTransformCom->LookAt(m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS));
					m_iAdjMovedIndex++;
				}
			}
			break;
		}
		case 16:
		{
			//m_pTransformCom->LookAt(m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS));
			_Vector vTarget = XMVector3Normalize(m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS) - m_pTransformCom->Get_MatrixState(CTransform::STATE_POS));
			m_pTransformCom->Turn_Dir(vTarget, 0.7f);

			if (PlayRate > 0 && PlayRate <= 0.95)
			{
				_float fSpeed = g_pGameInstance->Easing_Return(TYPE_SinInOut, TYPE_SinInOut, 0, 2.852698f, (_float)PlayRate, 0.95f); // PlayRate - 0.266666 and 0.5 - 0.266666
				m_pTransformCom->Move_Forward(dDeltaTime * fSpeed);
				m_dAcceleration = 0.8;
			}
			break;
		}
		case 17:
		{
			//m_pTransformCom->LookAt(m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS));
			_Vector vTarget = XMVector3Normalize(m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS) - m_pTransformCom->Get_MatrixState(CTransform::STATE_POS));
			m_pTransformCom->Turn_Dir(vTarget, 0.7f);

			if (PlayRate > 0 && PlayRate <= 0.95)
			{
				_float fSpeed = g_pGameInstance->Easing_Return(TYPE_SinInOut, TYPE_SinInOut, 0, 2.852698f, (_float)PlayRate, 0.95f); // PlayRate - 0.266666 and 0.5 - 0.266666
				m_pTransformCom->Move_Forward(dDeltaTime * fSpeed);
				m_dAcceleration = 0.8;
			}
			break;
		}
		case 19:
		{
			if (PlayRate > 0)
			{
				_Vector vTarget = XMVector3Normalize(m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS) - m_pTransformCom->Get_MatrixState(CTransform::STATE_POS));

				m_pTransformCom->Turn_Dir(vTarget, 0.9f);
			}
			if (m_iAdjMovedIndex == 0 && PlayRate >= 0.60714)
			{

				CMonster_Bullet_Universal::MONSTER_BULLET_UNIVERSALDESC Monster_BulletDesc;

				Monster_BulletDesc.iBulletMeshNumber = CMonster_Bullet_Universal::NINJASURA_KNIFE;
				Monster_BulletDesc.fSpeedPerSec = 15.f;
				Monster_BulletDesc.fScale = _float3(0.9f, 0.9f, 0.9f);

				Monster_BulletDesc.Object_Transform = m_pTransformCom;
				Monster_BulletDesc.fPositioning = _float3(0.5f, 3.f, 1.5f); //여기의 값을 바꾸면 밑의 fPos.y도 바꿔줘야함


				Monster_BulletDesc.Object = this;

				Monster_BulletDesc.dDuration = 10;

				Monster_BulletDesc.bBornAttachOn = true;
				Monster_BulletDesc.pBoneName = "lowerarm_twist_01_r";


				_float3 fPos = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS);

				fPos.y += 3.f;


				_Vector vLook = XMVector3Normalize(m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS) - XMLoadFloat3(&fPos) );

				XMStoreFloat3(&Monster_BulletDesc.fLook, XMVector3Normalize(vLook * 0.85f + m_pTransformCom->Get_MatrixState(CTransform::STATE_RIGHT) * -0.15f));
				FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_MonsterBullet), TAG_OP(Prototype_Object_Monster_Bullet_Universal), &Monster_BulletDesc));

				XMStoreFloat3(&Monster_BulletDesc.fLook, XMVector3Normalize(vLook * 1.f + m_pTransformCom->Get_MatrixState(CTransform::STATE_RIGHT) * 0.f));
				FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_MonsterBullet), TAG_OP(Prototype_Object_Monster_Bullet_Universal), &Monster_BulletDesc));


				XMStoreFloat3(&Monster_BulletDesc.fLook, XMVector3Normalize(vLook * 0.85f + m_pTransformCom->Get_MatrixState(CTransform::STATE_RIGHT) * 0.15f));
				FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_MonsterBullet), TAG_OP(Prototype_Object_Monster_Bullet_Universal), &Monster_BulletDesc));

				m_iAdjMovedIndex++;
			}
			break;
		}
		case 20:
		{
			if (PlayRate >= 0.35714 && PlayRate <= 0.76785)
			{
				m_bLookAtOn = true;
			}
			else {
				m_bLookAtOn = false;
			}
		}
		}

	}

	m_iOldAnimIndex = iNowAnimIndex;
	return S_OK;
}

CMonster_Ninjasura * CMonster_Ninjasura::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CMonster_Ninjasura*	pInstance = NEW CMonster_Ninjasura(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CMonster_Ninjasura");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMonster_Ninjasura::Clone(void * pArg)
{
	CMonster_Ninjasura*	pInstance = NEW CMonster_Ninjasura(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CMonster_Ninjasura");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMonster_Ninjasura::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModel);
	////////////Motion Test
	Safe_Release(m_pMotionTrail);
	////////////
}