#include "stdafx.h"
#include "..\public\Monster_Gadasura_Black.h"
#include "Monster_Bullet_Universal.h"
#include "Monster_Weapon_Universal.h"
#include "Monster_Texture_Bullet.h"

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


	return S_OK;
}

_int CMonster_Gadasura_Black::Update(_double dDeltaTime)
{

	if (__super::Update(dDeltaTime) < 0)return -1;

	//마지막 인자의 bBlockAnimUntilReturnChange에는 true로 시작해서 정상작동이 된다면 false가 된다.
	//m_pModel->Change_AnimIndex();
	//m_pModel->Change_AnimIndex_ReturnTo(); //어떤 애니메이션을 돌리고 특정 애니메이션으로 보냄
	//m_pModel->Change_AnimIndex_ReturnTo_Must(); //중간에 애니메이션을 캔슬하고 다른 애니메이션을 동작시킴
	//m_pModel->Change_AnimIndex_UntilTo(); //1~5까지 돌린다고 명령어를 입력하면 1~5까지 돌아감
	//m_pModel->Change_AnimIndex_UntilNReturn();//1~5까지 돌리고 난 이후 특정 애니메이션으로 Return 시킬 경우
	//m_pModel->Change_AnimIndex_UntilNReturn_Must(); //1~5까지 돌리고 난 이후 특정 애니메이션으로 Return 시킬 수 있지만 다른 애니메이션을 동작시킬 수 있음

	 
	//camera moving test!@#!@#!@$#@$!@$@!$@!$@!#$@!#$@!#$@!#$!@%#$%#@%#@%#@%#@^^$%^#$^#$^%&
	//PlayAnim(dDeltaTime);

	m_bIsOnScreen = g_pGameInstance->IsNeedToRender(m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS), m_fFrustumRadius);
	FAILED_CHECK(m_pModel->Update_AnimationClip(dDeltaTime * m_dAcceleration, m_bIsOnScreen));
	FAILED_CHECK(Adjust_AnimMovedTransform(dDeltaTime));

	////////////////Motion Test
	m_pMotionTrail->Update_MotionTrail(dDeltaTime);
	///////////////////////////
	//@#$#$@%#@$%#@$%#$%#$%#$%#$%#%#$%#$%@#^$#%^%$&%^&%*^%&*^%*^%&*^%*^%&!@$@!$!@$!@$!@$!@$
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
	//////////////Motion Test
	FAILED_CHECK(m_pRendererCom->Add_TrailGroup(CRenderer::TRAIL_MOTION, m_pMotionTrail));
	////////////////

	FAILED_CHECK(m_pRendererCom->Add_ShadowGroup(CRenderer::SHADOW_ANIMMODEL, this, m_pTransformCom, m_pShaderCom, m_pModel));
	m_vOldPos = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS);

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
	MonsterWeaponDesc.eAttachedDesc.Initialize_AttachedDesc(this, "skd_r_wrist", _float3(1, 1, 1), _float3(0, 0, 0), _float3(-1.690f, -0.050f, -2.020f));
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_Monster_Weapon), TAG_OP(Prototype_Object_Monster_Weapon_Universal), &MonsterWeaponDesc));


	return S_OK;
}

HRESULT CMonster_Gadasura_Black::SetUp_Fight(_double dDeltaTime)
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
		m_iInfinityPattern = rand() % 7;


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
		m_iOnceAnimNumber = 15; //Rage Run
		m_bComboAnimSwitch = true;
		break;
	case 6:
		m_iOnceAnimNumber = 21; //Rage Run Attack
		m_bComboAnimSwitch = true;
		break;
	case 7:
		m_iOnceAnimNumber = 19; //smash Attack
		m_bComboAnimSwitch = false;
		break;
	case 8:
		m_iOnceAnimNumber = 18; //Attack2
		m_bComboAnimSwitch = false;
		break;
	case 9:
		m_iOnceAnimNumber = 4; //Left Move
		m_bComboAnimSwitch = false;
		break;
	case 10:
		m_iOnceAnimNumber = 25; //Rage Run
		m_bComboAnimSwitch = true;
		break;
	case 11:
		m_iOnceAnimNumber = 14; //Rage Run
		m_bComboAnimSwitch = true;
		break;
	case 12:
		m_iOnceAnimNumber = 15; //Rage Run
		m_bComboAnimSwitch = true;
		break;
	case 13:
		m_iOnceAnimNumber = 21; //Rage Run Attack
		m_bComboAnimSwitch = true;
		break;
	case 14:
		m_iOnceAnimNumber = 22; //Taunt
		m_bComboAnimSwitch = false;
		break;
	case 15:
		m_iOnceAnimNumber = 18; //Attack2
		m_bComboAnimSwitch = false;
		break;
	case 16:
		m_iOnceAnimNumber = 19; //smash Attack
		m_bComboAnimSwitch = true;
		break;
	case 17:
		m_iOnceAnimNumber = 5; //Back
		m_bComboAnimSwitch = false;
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

	if (m_iOncePattern >= 17)
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

HRESULT CMonster_Gadasura_Black::Adjust_AnimMovedTransform(_double dDeltaTime)
{
	_uint iNowAnimIndex = m_pModel->Get_NowAnimIndex();
	_double PlayRate = m_pModel->Get_PlayRate();

	if (iNowAnimIndex != m_iOldAnimIndex || PlayRate > 0.98)
	{
		m_iAdjMovedIndex = 0;
		m_dAcceleration = 1;

		m_bLookAtOn = true;

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
				m_pTransformCom->Move_Right(dDeltaTime * 1);
			break;
		}
		case 4:
		{
			if (PlayRate >= 0.22222 && PlayRate <= 0.44444)
				m_pTransformCom->Move_Left(dDeltaTime * 1);
			break;
		}
		case 5:
		{
			if (PlayRate >= 0.266666 && PlayRate <= 0.5)
			{
				_float fSpeed = g_pGameInstance->Easing(TYPE_QuinticOut, 1.8f, 0.8f, (_float)PlayRate - 0.266666f, 0.2334f); // PlayRate - 0.266666 and 0.5 - 0.266666
				m_pTransformCom->Move_Backward(dDeltaTime * fSpeed);
			}
			break;
		}
		case 14:
		{
			m_pTransformCom->Move_Forward(dDeltaTime * 1.5);
			break;
		}
		case 15:
		{
			m_pTransformCom->Move_Forward(dDeltaTime * 1.5);
			break;
		}
		case 17:
		{
			m_bLookAtOn = false;
			break;
		}
		case 18:
		{
			m_bLookAtOn = false;
			break;
		}
		case 19:
		{
			m_bLookAtOn = false;

			if (PlayRate > 0 && PlayRate <= 0.539215)
			{
				m_dAcceleration = 2.5;
			}
			else {
				m_dAcceleration = 1;
			}

			if (m_iAdjMovedIndex == 0 && PlayRate >= 0.539215)
			{
				CMonster_Texture_Bullet::MONSTER_TEXTURE_BULLETDESC Monster_Texture_BulletDesc;

				Monster_Texture_BulletDesc.iBulletTextureNumber = CMonster_Texture_Bullet::GADASURA_TERRAIN_BULLET;
				Monster_Texture_BulletDesc.fSpeedPerSec = 10;
				Monster_Texture_BulletDesc.fScale = _float3(1.f, 1.f, 1.f);

				Monster_Texture_BulletDesc.Object_Transform = m_pTransformCom;
				Monster_Texture_BulletDesc.fPositioning = _float3(0.f, 0.3f, 2.f);


				Monster_Texture_BulletDesc.Object = this;

				Monster_Texture_BulletDesc.dDuration = 0.7;

				FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_MonsterBullet), TAG_OP(Prototype_Object_Monster_Texture_Bullet), &Monster_Texture_BulletDesc));
				m_iAdjMovedIndex++;
			}
			break;
		}
		case 20:
		{
			m_bLookAtOn = false;
			if (m_iAdjMovedIndex == 0 && PlayRate >= 0.4205607)
			{
				CMonster_Bullet_Universal::MONSTER_BULLET_UNIVERSALDESC Monster_BulletDesc;

				Monster_BulletDesc.iBulletMeshNumber = CMonster_Bullet_Universal::GADASURA_SINKHOLE;
				Monster_BulletDesc.fSpeedPerSec = 15.f;
				Monster_BulletDesc.fScale = _float3(5.f, 5.f, 5.f);

				Monster_BulletDesc.Object_Transform = m_pTransformCom;
				Monster_BulletDesc.fPositioning = _float3(0.f, 0.f, 1.2f);


				Monster_BulletDesc.Object = this;

				Monster_BulletDesc.dDuration = 5;

				Monster_BulletDesc.bBornAttachOn = true;
				Monster_BulletDesc.pBoneName = "skd_l_ball";

				FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_MonsterBullet), TAG_OP(Prototype_Object_Monster_Bullet_Universal), &Monster_BulletDesc));

				m_iAdjMovedIndex++;
			}
			break;
		}
		case 21:
		{
			if (PlayRate <= 0.588235)
			{
				m_pTransformCom->Move_Forward(dDeltaTime * 1.5);
			}
			break;
		}
		case 22:
		{
			m_bLookAtOn = false;
			break;
		}
		case 24:
		{
			m_pTransformCom->Move_Forward(dDeltaTime * 0.4);

			break;
		}
		case 28:
		{

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
	CMonster_Gadasura_Black*	pInstance = new CMonster_Gadasura_Black(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CMonster_Gadasura_Black");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMonster_Gadasura_Black::Clone(void * pArg)
{
	CMonster_Gadasura_Black*	pInstance = new CMonster_Gadasura_Black(*this);

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
	////////////Motion Test
	Safe_Release(m_pMotionTrail);
	////////////
}