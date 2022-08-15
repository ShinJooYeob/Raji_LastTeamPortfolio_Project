#include "stdafx.h"
#include "..\public\Mahabalasura.h"

#include "Mahabalasura_Weapon.h"
#include "Mahabalasura_Arms.h"
#include "CopyMahabalasura.h"
#include "Mahabalasura_SpearWave.h"
#include "Mahabalasura_AttackSpear.h"
#include "HpUI.h"

#include "Player.h"
#include "Camera_Main.h"
#include "InstanceMonsterBatchTrigger.h"

#include "ParticleCollider.h"
#include "Particle_ColliderInOut.h"

CMahabalasura::CMahabalasura(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CBoss(pDevice, pDeviceContext)
{
}

CMahabalasura::CMahabalasura(const CMahabalasura & rhs)
	: CBoss(rhs)
{
}

HRESULT CMahabalasura::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));


	return S_OK;
}

HRESULT CMahabalasura::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components());

	if (pArg != nullptr)
	{
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, *((_float3*)pArg));
	}

	//Set_LimLight_N_Emissive(_float4(), 0);

	m_fAttackCoolTime = 2.f;
	m_fSkillCoolTime = 5.f;
	m_bIsHit = false;


	//ATTACHEDESC WeaponDesc;
	CMahabalasura_Weapon::WEAPONDESC Desc;
	Desc.m_CloneType = CMahabalasura_Weapon::CLONE_BOSS;
	Desc.m_eAttachedDesc.Initialize_AttachedDesc(this, "middle_metacarpal_r", XMVectorSet(100.f, 200.f, 150.f, 0.f), XMVectorSet(0, 0.f, 180.f, 0.f), XMVectorSet(-0.950 * 100.f, -0.160 * 200.f, -1.550 * 150.f, 1.f));

	g_pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&m_pSpear), m_eNowSceneNum, TAG_OP(Prototype_Object_Boss_MahabalasuraWeapon), &Desc);

	//WeaponDesc.Initialize_AttachedDesc(this, "middle_metacarpal_r", XMVectorSet(100.f, 200.f, 150.f, 0.f), XMVectorSet(0, 0.f, 180.f, 0.f), XMVectorSet(-0.950 * 100.f,-0.160 * 200.f,-1.550 * 150.f,1.f));
/*	g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TEXT("Weapon"), TAG_OP(Prototype_Object_Boss_MahabalasuraWeapon), &Desc);
	m_pSpear = (CMahabalasura_Weapon*)g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TEXT("Weapon"))*/;

	
	ATTACHEDESC ArmsDesc;
	CMahabalasura_Arms* Arm = nullptr;

	for (_int i = 0; i < 6; ++i)
	{
		switch (i)
		{
		case 0:
			//ArmsDesc.Initialize_AttachedDesc(this, "upperarm_l", _float3(1), _float3(0), XMVectorSet(0.362445f, 0.036896f, -2.13791f, 1.f));
			ArmsDesc.Initialize_AttachedDesc(this, "upperarm_l", XMVectorSet(1.f, 1.f, 1.f, 0.f), XMVectorSet(-10.f, 190.f, 60.f, 0.f), XMVectorSet(0.343f, 0.088f, -2.215f, 1.f));
			break;
		case 1:
			//ArmsDesc.Initialize_AttachedDesc(this, "upperarm_l", _float3(1), _float3(0), XMVectorSet(0.362445f, 0.036896f, -2.13791f, 1.f));
			ArmsDesc.Initialize_AttachedDesc(this, "upperarm_l", XMVectorSet(1.f, 1.f, 1.f, 0.f), XMVectorSet(-10.f, 180.f, 60.f, 0.f), XMVectorSet(0.501f, 0.099f, -2.149f, 1.f));
			break;
		case 2:
			//ArmsDesc.Initialize_AttachedDesc(this, "upperarm_l", _float3(1), _float3(0), XMVectorSet(0.362445f, 0.036896f, -2.13791f, 1.f));
			ArmsDesc.Initialize_AttachedDesc(this, "upperarm_l", XMVectorSet(1.f, 1.f, 1.f, 0.f), XMVectorSet(-10.f, 160.f, 60.f, 0.f), XMVectorSet(0.659f, 0.110f, -2.083f, 1.f));
			break;
		case 3:
			//ArmsDesc.Initialize_AttachedDesc(this, "upperarm_l", _float3(1), _float3(0), XMVectorSet(0.362445f, 0.036896f, -2.13791f, 1.f));
			ArmsDesc.Initialize_AttachedDesc(this, "upperarm_l", XMVectorSet(1.f, 1.f, 1.f, 0.f), XMVectorSet(-10.f, 140.f, 30.f, 0.f), XMVectorSet(0.661f, 0.001f, -1.846f, 1.f));
			break;
		case 4:
			//ArmsDesc.Initialize_AttachedDesc(this, "upperarm_l", _float3(1), _float3(0), XMVectorSet(0.362445f, 0.036896f, -2.13791f, 1.f));
			ArmsDesc.Initialize_AttachedDesc(this, "upperarm_l", XMVectorSet(1.f, 1.f, 1.f, 0.f), XMVectorSet(-20.f, 180.f, 0.f, 0.f), XMVectorSet(0.792f, -0.191f, -1.997f, 1.f));
			break;
		case 5:
			//ArmsDesc.Initialize_AttachedDesc(this, "upperarm_l", _float3(1), _float3(0), XMVectorSet(0.362445f, 0.036896f, -2.13791f, 1.f));
			ArmsDesc.Initialize_AttachedDesc(this, "upperarm_l", XMVectorSet(1.f, 1.f, 1.f, 0.f), XMVectorSet(-20.f, 180.f, 0.f, 0.f), XMVectorSet(0.792f, -0.191f, -1.697f, 1.f));
			break;
		}

		g_pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&Arm), m_eNowSceneNum, TAG_OP(Prototype_Object_Boss_MahabalasuraArms), &ArmsDesc);
		m_pArms.push_back(Arm);
	}

	m_fMaxHP = 10.f;
	m_fHP = 10.f;

	CHpUI::HPDesc HpDesc;
	HpDesc.m_HPType = CHpUI::HP_MONSTER;
	HpDesc.m_pObjcect = this;
	HpDesc.m_vPos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
	HpDesc.m_Dimensions = 2.f;

	g_pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&m_pHPUI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_HpUI), &HpDesc);




	// JH
	m_pPlayerObj = g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, Tag_Layer(Layer_Player));
	m_pPlayerTransform = static_cast<CTransform*>(m_pPlayerObj->Get_Component(Tag_Component(COMPONENTID::Com_Transform)));
	//g_pGameInstance->Play3D_Sound(L"JJB_MrM_Intro.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);
	m_bBlockUpdate = true;
	m_pTransformCom->Rotation_CCW(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(180.f));
	m_fAttachCamPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
	m_fAttachCamPos.y += 2.f;
	m_fAttachCamPos.z -= 4.f;
	m_pModel->Change_AnimIndex(0);
	m_iCutSceneStep = 0;
	m_fDelayTime = 3.f; 
	//

	return S_OK;
}

_int CMahabalasura::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)return -1;

	// JH
	if (true == m_bBlockUpdate)
	{
		Update_Direction(fDeltaTime);

		_Vector vTestPos = GetSingle(CUtilityMgr)->Get_MainCamera()->Get_CamTransformCom()->Get_MatrixState(CTransform::TransformState::STATE_POS);

		if (false == m_bBlockAnim)
		{
			FAILED_CHECK(m_pModel->Update_AnimationClip(fDeltaTime * (m_fAnimmultiple), m_bIsOnScreen));
		}

		return _int();
	}
	else if (m_fHP <= 0.f)
	{
		Update_Dead(fDeltaTime);
		m_pSpear->Update(fDeltaTime);
		m_pDissolveCom->Update_Dissolving(fDeltaTime);
		FAILED_CHECK(m_pModel->Update_AnimationClip(fDeltaTime * (m_fAnimmultiple), m_bIsOnScreen));
		return _int();
	}
	//




	if (g_pGameInstance->Get_DIKeyState(DIK_Z)&DIS_Down)
	{

		{




		}



	}

	_float3 test = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);

	m_pDissolveCom->Update_Dissolving(fDeltaTime);

	if (m_pDissolveCom->Get_IsFadeIn() == false && m_pDissolveCom->Get_DissolvingRate() >= 1.0)
	{
		Set_IsDead();

		// JH
		static_cast<CPlayer*>(m_pPlayerObj)->Set_Targeting(nullptr);
		GetSingle(CUtilityMgr)->Get_MainCamera()->Set_CameraMode(ECameraMode::CAM_MODE_NOMAL);
		//
	}

	if (m_fHP <= 0 && !m_bIsHit)
		m_bIsHit = true;

	if (m_bIsStartTeleport)
	{
		if (m_fTeleportDelay <= 0 && m_bIsTeleport)
		{
			if (m_bIsTeleportAttack)
			{
				g_pGameInstance->Play3D_Sound(L"JJB_MrM_Trishul_Swing_02.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);
				m_bIsTeleportAttack = false;
			}
			m_fTeleportDelay = 0.5f;
			m_bIsTeleport = false;
		}

		if (m_bIsTeleport)
		{
			if(!m_bIsCopySkill)
				m_fTeleportDelay -= (_float)fDeltaTime;
			if (!m_bIsTeleportSound)
			{
				m_bIsTeleportSound = true;
				g_pGameInstance->Play3D_Sound(L"JJB_MrM_Teleport_01.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);
			}
			
			return _int();
		}
	}

	m_fAttackCoolTime -= (_float)fDeltaTime;
	m_fSkillCoolTime -= (_float)fDeltaTime;

	m_fNarrationTime -= (_float)fDeltaTime;

	if (m_fNarrationTime <= 0)
	{
		m_fNarrationTime = 12.f;

		_int iRandom = rand() % 8;

		wstring teampString;
		teampString = L"JJB_Naration_" + to_wstring(iRandom) + L".wav";

		g_pGameInstance->Play3D_Sound((_tchar*)teampString.c_str(), g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);

	}


	if (m_bIsArmAttack)
	{
		m_fArmAttackTime -= (_float)fDeltaTime;

		if (m_fArmAttackTime <= 0)
		{
			m_bIsArmAttack = false;
			m_fAttackCoolTime = 3.f;
			m_fSkillCoolTime = 8.f;
			m_fArmAttackTime = 10.f;
			m_pModel->Change_AnimIndex(0);
		}
	}

	if (m_bIsLookAt)
	{
		CTransform* PlayerTransform = (CTransform*)m_pPlayerObj->Get_Component(TAG_COM(Com_Transform));
		_float3 PlayerPos = PlayerTransform->Get_MatrixState(CTransform::STATE_POS);

		PlayerPos.y = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS).y;
		_Vector Dir = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
		Dir = XMVector3Normalize(PlayerPos.XMVector() - XMVectorSetY(Dir, PlayerPos.y));
		m_pTransformCom->Turn_Dir(Dir, 0.90f);

		m_fRange = XMVectorGetZ(XMVector3Length(XMLoadFloat3(&PlayerPos) - m_pTransformCom->Get_MatrixState(CTransform::STATE_POS)));
	}

	
	if (!m_bIsAttack && !m_bIsHit && m_bIsWalk && !m_bIsDIeAnimStart)
	{
		if(m_fRange < 2 && m_fAttackCoolTime > 0)
			m_pModel->Change_AnimIndex(0);
		else
		{
			m_pTransformCom->Move_Forward(fDeltaTime * 0.7);

			m_pModel->Change_AnimIndex(2);
		}
	}
	//맞았을때
	if (m_bIsHit)
	{
		if (m_fHP <= 0 && !m_bIsDIeAnimStart)
		{
			m_bIsLookAt = false;
			m_bIsDIeAnimStart = true;
			m_pModel->Change_AnimIndex_ReturnTo(4, 1);
		}
		else if(m_fHP > 0)
		{
			_int iRandom = rand() % 2;

			wstring teampString;
			teampString = L"JJB_MrM_Hurt_" + to_wstring(iRandom) + L".wav";

			g_pGameInstance->Play3D_Sound((_tchar*)teampString.c_str(), g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);

			m_bIsHit = false;
			m_bIsAttack = true;

			--m_fHP;
			m_pHPUI->Set_ADD_HitCount();

			m_pModel->Change_AnimIndex_ReturnTo(4, 0);
		}
	}
	//기본공격
	if (m_fRange < 8.f && !m_bIsAttack && m_fAttackCoolTime <=0 && !m_bIsHit && m_fHP > 0)
	{
		m_bIsAttack = true;

		m_bIsWalk = false;
		//m_bIsLookAt = false;
		m_pModel->Change_AnimIndex_ReturnTo(5, 0);
	}
	
	// #DEBUG
	if (KEYDOWN(DIK_B))
	{
		mOnlyPattern++;
		if (mOnlyPattern >= 4)
			mOnlyPattern = 0;
	}


	/////////스킬공격
	if (!m_bIsAttack && m_fSkillCoolTime <= 0 && !m_bIsHit && m_fHP > 0)
	{
			_int iRandom = (_int)GetSingle(CUtilityMgr)->RandomFloat(0, 3.9f);
			m_bIsAttack = true;

		iRandom = mOnlyPattern; // DEBUG
		iRandom = 3;
		switch (iRandom)
		{
		case SKILL_SPEAR:
		{
			m_bIsLookAt = false;
			m_pModel->Change_AnimIndex_ReturnTo(11, 0);
		}
		break;
		case SKILL_HAND:
		{

			m_vecNonInstMeshDesc[4].fMaxTime_Duration = m_vecNonInstMeshDesc[5].fMaxTime_Duration = 12.5f;
			g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_Particle),
				TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[4]);
			g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_Particle),
				TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[5]);

			{
				Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_BOSS_Mahabalasura_PLANE2, m_pTransformCom);
				Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_BOSS_Mahabalasura_PLANE3, m_pTransformCom);

			}

			m_bIsTeleport = true;
			m_bIsTeleportSound = false;
			//_float3 Pos = m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS);
			//_int iTemp = rand() % 2 == 0 ? -1 : 1;
			//Pos = _float3(Pos.x + (iTemp * GetSingle(CUtilityMgr)->RandomFloat(1.f, 2.5f)), Pos.y, Pos.z + (iTemp * GetSingle(CUtilityMgr)->RandomFloat(1.f, 2.5f)));

			m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, _float3(101.721f, 34.260f, 323.105f));
			m_bIsArmAttack = true;
			mPatternCount2 = 0;
			for (auto& Arm : m_pArms)
				Arm->Reset_AttackTime();
			m_pModel->Change_AnimIndex(6);
		}
		break;

		case SKILL_SPEAR_RAIN:
		{
			m_bIsTeleport = true;
			m_bIsTeleportSound = false;
			//_float3 Pos = m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS);
			//_int iTemp = rand() % 2 == 0 ? -1 : 1;
			//Pos = _float3(Pos.x + (iTemp * GetSingle(CUtilityMgr)->RandomFloat(1.f, 2.5f)), Pos.y, Pos.z + (iTemp * GetSingle(CUtilityMgr)->RandomFloat(1.f, 2.5f)));

			m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, _float3(101.721f, 34.260f, 323.105f));
			m_pModel->Change_AnimIndex_ReturnTo(9, 0);
			m_iRainAttackCount = 0;
		}
		break;

		case SKILL_COPY:
		{
			m_pModel->Change_AnimIndex_ReturnTo(7, 0);
		}
		break;

		}
	}

	if (g_pGameInstance->Get_DIKeyState(DIK_M)& DIS_Down)
		m_bIsHit = true;


	m_bIsOnScreen = g_pGameInstance->IsNeedToRender(m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS), m_fFrustumRadius);
	FAILED_CHECK(m_pModel->Update_AnimationClip(fDeltaTime * (m_fAnimmultiple), m_bIsOnScreen));
	FAILED_CHECK(Adjust_AnimMovedTransform(fDeltaTime));

	m_pCollider->Update_ConflictPassedTime(fDeltaTime);

	_uint iNumCollider = m_pCollider->Get_NumColliderBuffer();

	for (_uint i = 0; i < iNumCollider; i++)
		m_pCollider->Update_Transform(i, m_vecAttachedDesc[i].Caculate_AttachedBoneMatrix_BlenderFixed());

	FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_Monster, this, m_pCollider));


	m_pSpear->Update(fDeltaTime);

	if (m_bIsArmAttack)
	{
		for (auto& Arm : m_pArms)
		{
			Arm->Update(fDeltaTime);
		}
	}

	if (m_pHPUI != nullptr)
		m_pHPUI->Update(fDeltaTime);

	if (m_bIsDissolveStart)
	{
		if (m_bInstanceMonsterDieSwitch == false)
		{
			CInstanceMonsterBatchTrigger* pMonsterBatchTrigger = static_cast<CInstanceMonsterBatchTrigger*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_InstanceMonsterTrigger)));
			if (nullptr != pMonsterBatchTrigger)
			{
				pMonsterBatchTrigger->Set_MonsterAllDie(true);
			}
			m_bInstanceMonsterDieSwitch = true;
		}
		m_pDissolveCom->Set_DissolveOn(false, 6.4f);
	}

	return _int();
}

_int CMahabalasura::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)return -1;

	if (m_bIsStartTeleport)
	{
		if (m_bIsTeleport)
		{
			return _int();
		}
	}

	FAILED_CHECK(m_pRendererCom->Add_ShadowGroup(CRenderer::SHADOW_ANIMMODEL, this, m_pTransformCom, m_pShaderCom, m_pModel));
	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
	m_vOldPos = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS);
	FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pCollider));
	//g_pGameInstance->Set_TargetPostion(PLV_PLAYER, m_vOldPos);

	m_pSpear->LateUpdate(fDeltaTime);

	if (m_bIsArmAttack)
	{
		for (auto& Arm : m_pArms)
		{
			Arm->LateUpdate(fDeltaTime);
		}
	}

	if (m_pHPUI != nullptr)
		m_pHPUI->LateUpdate(fDeltaTime);

	return _int();
}

_int CMahabalasura::Render()
{
	if (__super::Render() < 0)		return -1;


	NULL_CHECK_RETURN(m_pModel, E_FAIL);

	CGameInstance* pInstance = GetSingle(CGameInstance);
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));


	FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));


	FAILED_CHECK(m_pDissolveCom->Render(3));

	//_uint NumMaterial = m_pModel->Get_NumMaterial();

	//for (_uint i = 0; i < NumMaterial; i++)
	//{
	//	//if(i == 10)continue;

	//	for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; j++)
	//		FAILED_CHECK(m_pModel->Bind_OnShader(m_pShaderCom, i, j, MODLETEXTYPE(j)));

	//	FAILED_CHECK(m_pModel->Render(m_pShaderCom, 3, i, "g_BoneMatrices"));
	//}

	return _int();
}

_int CMahabalasura::LateRender()
{
	if (__super::LateRender() < 0)		return -1;

	return _int();
}

void CMahabalasura::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{

}

_float CMahabalasura::Take_Damage(CGameObject * pTargetObject, _float fDamageAmount, _fVector vDamageDir, _bool bKnockback, _float fKnockbackPower)
{
	m_fHP -= fDamageAmount;

	m_pHPUI->Set_ADD_HitCount();

	return _float();
}

HRESULT CMahabalasura::Ready_ParticleDesc()
{
	m_pTextureParticleTransform = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
	NULL_CHECK_RETURN(m_pTextureParticleTransform, E_FAIL);

	// SpearPos
	m_pTextureParticleTransform1 = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
	NULL_CHECK_RETURN(m_pTextureParticleTransform1, E_FAIL);

	// PlayerPos
	m_pTextureParticleTransform2 = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
	NULL_CHECK_RETURN(m_pTextureParticleTransform2, E_FAIL);



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


	//////////////////////////////////////////////////////////////////////////
	{
		//0
		{

			NONINSTNESHEFTDESC tNIMEDesc;

			tNIMEDesc.MoveDir = FollowingDir_Look;
			tNIMEDesc.MoveSpeed = -5.f;


			tNIMEDesc.eMeshType = Prototype_Mesh_Spear_SecondAttack;
			tNIMEDesc.fMaxTime_Duration = 0.3f;

			tNIMEDesc.fAppearTime = 0.15f;

			tNIMEDesc.noisingdir = _float2(-1, 0);

			tNIMEDesc.NoiseTextureIndex = 249;
			tNIMEDesc.MaskTextureIndex = 64;
			tNIMEDesc.iDiffuseTextureIndex = 365;
			tNIMEDesc.m_iPassIndex = 17;
			tNIMEDesc.vEmissive = _float4(1.f, 1.f, 1.f, 0);
			tNIMEDesc.vLimLight = _float4(0.10015625f, 0.005625f, 0.206875f, 0.2f);
			tNIMEDesc.vColor = _float3(1.0f, 1, 1);

			tNIMEDesc.RotAxis = FollowingDir_Look;
			tNIMEDesc.RotationSpeedPerSec = -480.f;
			tNIMEDesc.vSize = _float3(0.02f, 0.02f, 0.075f);
			m_vecNonInstMeshDesc.push_back(tNIMEDesc);
		}
		//1
		{
			NONINSTNESHEFTDESC tNIMEDesc;
			tNIMEDesc.eMeshType = Prototype_Mesh_Spear_NormalEffect;
			tNIMEDesc.fMaxTime_Duration = 0.35f;
			tNIMEDesc.fAppearTime = 0.175f;

			tNIMEDesc.noisingdir = _float2(0, -1);

			tNIMEDesc.NoiseTextureIndex = 381;
			tNIMEDesc.MaskTextureIndex = 10;
			tNIMEDesc.iDiffuseTextureIndex = 365;
			tNIMEDesc.m_iPassIndex = 19;
			tNIMEDesc.vEmissive = _float4(1.f, 1.f, 1.f, 0);
			tNIMEDesc.vLimLight = _float4(0.10015625f, 0.005625f, 0.206875f, 0.2f);
			tNIMEDesc.vColor = _float3(1.0f, 1, 1);




			tNIMEDesc.RotAxis = FollowingDir_Right;
			tNIMEDesc.RotationSpeedPerSec = -180;
			tNIMEDesc.StartRot = -0.5f;
			tNIMEDesc.vSize = _float3(1.f, -2, 2.f);
			tNIMEDesc.MoveDir = FollowingDir_Look;
			tNIMEDesc.MoveSpeed = 0.f;

			m_vecNonInstMeshDesc.push_back(tNIMEDesc);

		}
		//2
		{

			NONINSTNESHEFTDESC tNIMEDesc;

			tNIMEDesc.eMeshType = Prototype_Mesh_Spear_NormalEffect;
			tNIMEDesc.fAppearTime = 0.175f;
			tNIMEDesc.fMaxTime_Duration = 0.35f;
			tNIMEDesc.noisingdir = _float2(0, -1);

			tNIMEDesc.NoiseTextureIndex = 381;
			tNIMEDesc.MaskTextureIndex = 10;
			tNIMEDesc.iDiffuseTextureIndex = 365;
			tNIMEDesc.m_iPassIndex = 19;
			tNIMEDesc.vEmissive = _float4(1.f, 1.f, 1.f, 0);
			tNIMEDesc.vLimLight = _float4(0.10015625f, 0.005625f, 0.206875f, 0.2f);
			tNIMEDesc.vColor = _float3(1.0f, 1, 1);


			tNIMEDesc.RotAxis = FollowingDir_Right;
			tNIMEDesc.RotationSpeedPerSec = 180;
			tNIMEDesc.StartRot = -0.5f;
			tNIMEDesc.vSize = _float3(1.f, 2, 2.f);
			tNIMEDesc.MoveDir = FollowingDir_Look;
			tNIMEDesc.MoveSpeed = 0.f;

			m_vecNonInstMeshDesc.push_back(tNIMEDesc);
		}
		//3
		{
				NONINSTNESHEFTDESC tNIMEDesc;



				tNIMEDesc.eMeshType = Prototype_Mesh_ConeMesh;
				tNIMEDesc.fMaxTime_Duration = 0.35f;
				tNIMEDesc.fAppearTime = 0.175f;

				tNIMEDesc.noisingdir = _float2(0, -1);

				tNIMEDesc.NoiseTextureIndex = 381;
				tNIMEDesc.MaskTextureIndex = 10;
				tNIMEDesc.iDiffuseTextureIndex = 365;
				tNIMEDesc.m_iPassIndex = 19;
				tNIMEDesc.vEmissive = _float4(1.f, 1.f, 1.f, 0);
				tNIMEDesc.vLimLight = _float4(0.10015625f, 0.005625f, 0.206875f, 0.2f);
				tNIMEDesc.vColor = _float3(1.0f, 1, 1);

				tNIMEDesc.RotAxis = FollowingDir_Up;
				tNIMEDesc.RotationSpeedPerSec = 1080.f;
				tNIMEDesc.vSize = _float3(2.f, -0.2f, 2.f);

				m_vecNonInstMeshDesc.push_back(tNIMEDesc);

		}
		//04
		{
			NONINSTNESHEFTDESC tNIMEDesc;

			tNIMEDesc.vPosition = _float3(101.721f, 33.260f, 323.105f);
			tNIMEDesc.vLookDir = _float3(0, -1, 0);

			tNIMEDesc.eMeshType = Prototype_Mesh_Half_Sheild;
			tNIMEDesc.fMaxTime_Duration = 7.25f;


			tNIMEDesc.fAppearTime = 0.0001f;

			tNIMEDesc.noisingdir = _float2(0, -1).XMVector()*0.015f;

			tNIMEDesc.NoiseTextureIndex = 381;
			tNIMEDesc.MaskTextureIndex = 59;
			tNIMEDesc.iDiffuseTextureIndex = 156;
			tNIMEDesc.m_iPassIndex = 23;
			tNIMEDesc.vEmissive = _float4(0.1f, 0.5f, 1.f, 0);
			tNIMEDesc.vLimLight = _float4(1, 1, 0.2f, 0);
			tNIMEDesc.vColor = _float3(1.f, 0, 0);

			tNIMEDesc.RotAxis = FollowingDir_Look;
			tNIMEDesc.RotationSpeedPerSec = 0.f;
			tNIMEDesc.OnceStartRot = 90;

			tNIMEDesc.SizeSpeed = 10.f;
			tNIMEDesc.vSizingRUL = _float3(1, 1, 1);
			tNIMEDesc.vSizieLimit = _float3(1.f, 1.f, 1.f);
			tNIMEDesc.vSize = _float3(0.01f, 0.01f, 0.01f);

			tNIMEDesc.fAlphaTestValue = 0.0f;

			tNIMEDesc.m_bReverseSizing = true;
			tNIMEDesc.m_bReverseSizingTimer = 2.f;


			m_vecNonInstMeshDesc.push_back(tNIMEDesc);
		}
		//05
		{

			NONINSTNESHEFTDESC tNIMEDesc;

			tNIMEDesc.vPosition = _float3(101.721f, 34.260f, 323.105f);
			tNIMEDesc.vLookDir = _float3(0, -1, 0);

			tNIMEDesc.eMeshType = Prototype_Mesh_Half_Sheild;
			tNIMEDesc.fMaxTime_Duration = 2.25f;


			tNIMEDesc.fAppearTime = 0.0001f;

			tNIMEDesc.noisingdir = _float2(0, -1).XMVector()*0.015f;

			tNIMEDesc.NoiseTextureIndex = 381;
			tNIMEDesc.MaskTextureIndex = 59;
			tNIMEDesc.iDiffuseTextureIndex = 156;
			tNIMEDesc.m_iPassIndex = 23;
			tNIMEDesc.vEmissive = _float4(0.1f, 0.5f, 1.f, 0);
			tNIMEDesc.vLimLight = _float4(1, 1, 0.2f, 0);
			tNIMEDesc.vColor = _float3(1.f, 0, 0);

			tNIMEDesc.RotAxis = FollowingDir_Look;
			tNIMEDesc.RotationSpeedPerSec = 0.f;
			tNIMEDesc.OnceStartRot = 90;

			tNIMEDesc.SizeSpeed = 10.f;
			tNIMEDesc.vSizingRUL = _float3(1, 1, 0);
			tNIMEDesc.vSizieLimit = _float3(1.f, 1.f, 0.000001f);
			tNIMEDesc.vSize = _float3(0.01f, 0.01f, 0.000001f);

			tNIMEDesc.fAlphaTestValue = 0.0f;

			tNIMEDesc.m_bReverseSizing = true;
			tNIMEDesc.m_bReverseSizingTimer = 2.f;

			m_vecNonInstMeshDesc.push_back(tNIMEDesc);
		}
		//6
		{
			NONINSTNESHEFTDESC tNIMEDesc;
			tNIMEDesc.eMeshType = Prototype_Mesh_Lightning_01;
			tNIMEDesc.fMaxTime_Duration = 1.f;

			tNIMEDesc.fAppearTime = 0.25f;

			tNIMEDesc.noisingdir = _float2(0, -1);

			tNIMEDesc.NoiseTextureIndex = 381;
			tNIMEDesc.MaskTextureIndex = 109;
			tNIMEDesc.iDiffuseTextureIndex = 273;
			tNIMEDesc.m_iPassIndex = 24;
			tNIMEDesc.vEmissive = _float4(1, 0.5f, 1.f, 0);
			tNIMEDesc.vLimLight = _float4(1, 1, 0.2f, 1);
			tNIMEDesc.vColor = _float3(1.f, 0, 0);

			tNIMEDesc.RotAxis = FollowingDir_Up;
			tNIMEDesc.RotationSpeedPerSec = 0.f;
			tNIMEDesc.vSize = _float3(0.1f, 0.1f, -0.2f);

			tNIMEDesc.fAlphaTestValue = 1.f;
			m_vecNonInstMeshDesc.push_back(tNIMEDesc);
		}
		//7
		{
			NONINSTNESHEFTDESC tNIMEDesc;
			tNIMEDesc.eMeshType = Prototype_Mesh_Beacon_Shape;
			tNIMEDesc.fMaxTime_Duration = 7.25f;


			tNIMEDesc.fAppearTime = 0.75f;

			tNIMEDesc.noisingdir = _float2(0, 1);

			tNIMEDesc.NoiseTextureIndex = 381;
			tNIMEDesc.MaskTextureIndex = 63;
			tNIMEDesc.iDiffuseTextureIndex = 275;
			tNIMEDesc.m_iPassIndex = 17;
			tNIMEDesc.vEmissive = _float4(1, 0.5f, 1.f, 0);
			tNIMEDesc.vLimLight = _float4(0.98046875f, 0.93359375f, 0.19140625f, 1.f);
			tNIMEDesc.vColor = _float3(0.98046875f, 0.93359375f, 0.19140625f);

			tNIMEDesc.RotAxis = FollowingDir_Look;
			tNIMEDesc.RotationSpeedPerSec = 0.f;
			tNIMEDesc.vSize = _float3(0.03f);
			m_vecNonInstMeshDesc.push_back(tNIMEDesc);
		}
		//8
		{
			NONINSTNESHEFTDESC tNIMEDesc;



			tNIMEDesc.eMeshType = Prototype_Mesh_Spark_Mesh2;

			tNIMEDesc.fMaxTime_Duration = 1.f;
			tNIMEDesc.fAppearTime = 0.5f;


			tNIMEDesc.noisingdir = _float2(0, 1);

			tNIMEDesc.NoiseTextureIndex = 350;
			tNIMEDesc.MaskTextureIndex = 63;
			tNIMEDesc.iDiffuseTextureIndex = 275;
			tNIMEDesc.m_iPassIndex = 17;
			tNIMEDesc.vEmissive = _float4(1, 0.5f, 1.f, 0);
			tNIMEDesc.vLimLight = _float4(0.98046875f, 0.93359375f, 0.19140625f, 1.f);
			tNIMEDesc.NoiseTextureIndex = 381;
			tNIMEDesc.vColor = _float3(0.98046875f, 0.93359375f, 0.19140625f);

			tNIMEDesc.RotAxis = FollowingDir_Look;
			tNIMEDesc.RotationSpeedPerSec = 0.f;
			tNIMEDesc.vSize = _float3(0.0070f, 0.0100f, 0.0070f);
			m_vecNonInstMeshDesc.push_back(tNIMEDesc);


		}
		//9
		{
			NONINSTNESHEFTDESC tNIMEDesc;
			tNIMEDesc.eMeshType = Prototype_Mesh_JY_Tornado;
			tNIMEDesc.fMaxTime_Duration = 2.9583333f;

			tNIMEDesc.fAppearTime = 0.75f;

			tNIMEDesc.noisingdir = _float2(0, 1);

			tNIMEDesc.NoiseTextureIndex = 374;
			tNIMEDesc.MaskTextureIndex = 61;
			tNIMEDesc.iDiffuseTextureIndex = 305;
			tNIMEDesc.m_iPassIndex = 17;
			tNIMEDesc.vEmissive = _float4(1, 0.5f, 1.f, 0);
			tNIMEDesc.vLimLight = _float4(1, 0, 0, 1);
			tNIMEDesc.NoiseTextureIndex = 381;
			tNIMEDesc.vColor = _float3(1.0, 0, 0);

			tNIMEDesc.RotAxis = FollowingDir_Up;
			tNIMEDesc.RotationSpeedPerSec = 360.f;
			tNIMEDesc.vSize = _float3(1.5f, 0.2f, 1.5f);

			m_vecNonInstMeshDesc.push_back(tNIMEDesc);
		}
		//10
		{
			NONINSTNESHEFTDESC tNIMEDesc;

			tNIMEDesc.vPosition = _float3(101.721f, 34.260f, 323.105f);
			tNIMEDesc.vLookDir = _float3(1, 0, 0);


			tNIMEDesc.eMeshType = Prototype_Mesh_JYBall;


			tNIMEDesc.fMaxTime_Duration = 0.5f;
			tNIMEDesc.fAppearTime = 0.01f;

			tNIMEDesc.noisingdir = _float2(1.f, 0.f).Get_Nomalize() * 0.03f;
			tNIMEDesc.fDistortionNoisingPushPower = 3.f;
			tNIMEDesc.NoiseTextureIndex = 200;
			tNIMEDesc.MaskTextureIndex = 59;
			tNIMEDesc.iDiffuseTextureIndex = 370;
			tNIMEDesc.m_iPassIndex = 23;
			tNIMEDesc.vEmissive = _float4(1, 0.5f, 1.f, 0);
			tNIMEDesc.vLimLight = _float4(0.19140625f, 0.98046875f, 0.19140625f, 1.f);
			tNIMEDesc.vColor = _float3(0.48046875f, 0.19140625f, 0.19140625f);

			tNIMEDesc.RotAxis = FollowingDir_Up;
			tNIMEDesc.RotationSpeedPerSec = 360.f;
			tNIMEDesc.vSize = _float3(0.1f, 0.1f, 0.1f);

			tNIMEDesc.SizeSpeed = 15.f;
			tNIMEDesc.vSizingRUL = _float3(1, 1, 1);

			m_vecNonInstMeshDesc.push_back(tNIMEDesc);
		}
	}

	return S_OK;

}

HRESULT CMahabalasura::Update_Particle(_double timer)
{
	_Matrix mat_Trans = m_pTransformCom->Get_WorldMatrix();

	mat_Trans.r[0] = XMVector3Normalize(mat_Trans.r[0]);
	mat_Trans.r[1] = XMVector3Normalize(mat_Trans.r[1]);
	mat_Trans.r[2] = XMVector3Normalize(mat_Trans.r[2]);

	mat_Trans.r[3] = m_pCollider->Get_ColliderPosition(1).XMVector();

	_Matrix mat_head = m_vecAttachedDesc[3].Caculate_AttachedBoneMatrix_BlenderFixed();
	m_pTextureParticleTransform->Set_Matrix(mat_head);


	_Matrix mat_Spear = m_pSpear->Get_Transform()->Get_WorldMatrix();
	mat_Spear.r[0] = XMVector3Normalize(mat_Spear.r[0]);
	mat_Spear.r[1] = XMVector3Normalize(mat_Spear.r[1]);
	mat_Spear.r[2] = XMVector3Normalize(mat_Spear.r[2]);

	//m_pTextureParticleTransform1->Set_Matrix(mat_Spear);


	if (m_pPlayerTransform)
	{
		_Matrix mat_Player = m_pPlayerTransform->Get_WorldMatrix();
		mat_Player.r[0] = _float3(1,0,0).XMVector();
		mat_Player.r[1] = _float3(0,1,0).XMVector();
		mat_Player.r[2] = _float3(0,0,1).XMVector();

		m_pTextureParticleTransform2->Set_Matrix(mat_Player);
	}
	


	return S_OK;
}

void CMahabalasura::Change_AnimIndex(_uint iAnimIndex)
{
	switch (iAnimIndex)
	{
	case 7:
	{
		m_bOnLookAtMe = true;
		m_iCutSceneStep = 0;
	}
		break;
	default:
	{
		m_pModel->Change_AnimIndex(iAnimIndex);
	}
	}
}

void CMahabalasura::Set_OnCamLookAtMe(_bool bOn)
{
	m_bOnLookAtMe = bOn;
}

void CMahabalasura::Update_Direction(_double fDeltaTime)
{
	_float fAnimPlayRate = (_float)m_pModel->Get_PlayRate();

	switch (m_pModel->Get_NowAnimIndex())
	{
	case 0:
	{
		if (0 == m_iCutSceneStep)
		{
			m_fDelayTime -= (_float)fDeltaTime;
			if (0 >= m_fDelayTime)
			{
				m_iCutSceneStep = 1;
			}
		}
		else if (1 == m_iCutSceneStep)
		{
			GetSingle(CUtilityMgr)->Get_MainCamera()->Set_FocusTarget(this);
			GetSingle(CUtilityMgr)->Get_MainCamera()->Set_CameraLookAt_Turn(_float3(0.f, 0.1f, 1.f).XMVector());
			m_bOnLookAtMe = false;
			m_fAttachCamPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
			m_fAttachCamPos.y += 2.f;
			m_fAttachCamPos.z -= 4.f;
			m_iCutSceneStep = 2;
			m_fDelayTime = 4.5f;
			g_pGameInstance->Play3D_Sound(L"JJB_MrM_Intro.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 1.f);
		}
		else if (2 == m_iCutSceneStep)
		{
			m_fDelayTime -= (_float)fDeltaTime;
			if (0.f >= m_fDelayTime)
			{
				static_cast<CPlayer*>(m_pPlayerObj)->Set_State_StopActionEnd();
				GetSingle(CUtilityMgr)->Get_MainCamera()->Set_FocusTarget(m_pPlayerObj);
				static_cast<CPlayer*>(m_pPlayerObj)->Set_Targeting(this);
				static_cast<CPlayer*>(m_pPlayerObj)->Set_TargetingLookDir(_float3(_float3(0.f, -0.5f, 1.f)));
				static_cast<CPlayer*>(m_pPlayerObj)->Set_MaxBossTargetingDist(20.f);
				static_cast<CPlayer*>(m_pPlayerObj)->Set_MinBossTargetingDist(0.f);
				m_bBlockUpdate = false;
				m_pRendererCom->OnOff_PostPorcessing_byParameter(POSTPROCESSING_CAMMOTIONBLUR, false);

				g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, Tag_Layer(LAYERID::Layer_MazeDoor))->Set_IsDead();
				m_fDelayTime = 3.f;
				m_iCutSceneStep = 0;
			}
		}
	}
		break;
	}
}

void CMahabalasura::Update_Dead(_double fDeltaTime)
{
	switch (m_iCutSceneStep)
	{
	case 0:
	{
		m_pModel->Change_AnimIndex(0);
		
		if (false == m_bOncePlaySound)
		{
			g_pGameInstance->Play3D_Sound(L"Jino_MrM_Dead_0.wav", m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS), CHANNELID::CHANNEL_MONSTER, 1.f);
			m_bOncePlaySound = true;
		}
		m_fDelayTime -= (_float)fDeltaTime;
		if (0.f >= m_fDelayTime)
		{
			m_pDissolveCom->Set_DissolveOn(false, 1.f);
			m_pSpear->Set_Dissolve_Appear(false);

			if (true == m_bOncePlaySound)
			{
				g_pGameInstance->Play3D_Sound(L"JJB_MrM_Teleport_01.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 1.f);
				m_bOncePlaySound = false;
			}
			m_fDelayTime = 3.5f;
			m_iCutSceneStep = 1;
		}
		GetSingle(CUtilityMgr)->Get_MainCamera()->Set_FocusTarget(this);
		GetSingle(CUtilityMgr)->Get_MainCamera()->Set_CameraMode(ECameraMode::CAM_MODE_NOMAL);
		static_cast<CPlayer*>(m_pPlayerObj)->Set_State_StopActionStart();
	}
		break;
	case 1:
	{
		m_fDelayTime -= (_float)fDeltaTime;
		if (0.f >= m_fDelayTime)
		{
			Set_LimLight_N_Emissive(_float4(0.0f), _float4(0.f));
			m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, _float3(103.3f, 67.f, 387.203f));
			m_pTransformCom->LookDir(XMVectorSet(0.f, 0.f, -1.f, 0.f));
			m_fDelayTime = 2.f;
			m_iCutSceneStep = 2;
		}
	}
		break;
	case 2:
	{
		m_fDelayTime -= (_float)fDeltaTime;
		if (0.f >= m_fDelayTime)
		{
			g_pGameInstance->Play3D_Sound(L"JJB_MrM_Teleport_01.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 1.f);
			m_pDissolveCom->Set_DissolveOn(true, 1.f);
			m_fDelayTime = 3.f;
			m_iCutSceneStep = 3;
			m_pTransformCom->Set_MoveSpeed(1.f);
		}
	}
		break;
	case 3:
	{
		m_fDelayTime -= (_float)fDeltaTime;
		if (0.f >= m_fDelayTime)
		{
			m_iCutSceneStep = 4;
			m_fDelayTime = 6.f;
			m_pModel->Change_AnimIndex(1);
			m_pDissolveCom->Set_DissolveOn(false, 6.f);
		}
	}
		break;
	case 4:
	{
		m_fDelayTime -= (_float)fDeltaTime;
		m_pTransformCom->Move_Up(fDeltaTime);
		m_pTransformCom->Set_MoveSpeed(m_pTransformCom->Get_MoveSpeed() + (_float)fDeltaTime);

		if (false == m_bOncePlaySound)
		{
			g_pGameInstance->Play3D_Sound(L"Jino_MrM_Dead_1.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 1.f);
			m_bOncePlaySound = true;
		}
		if (0.f >= m_fDelayTime)
		{
			Set_IsDead();
			GetSingle(CUtilityMgr)->Get_MainCamera()->Set_FocusTarget(m_pPlayerObj);
			static_cast<CPlayer*>(m_pPlayerObj)->Set_State_StopActionEnd();
		}
	}
		break;
	}

	m_fAttachCamPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
	m_fAttachCamPos.y += 2.f;
	m_fAttachCamPos.z -= 4.f;
	
}

HRESULT CMahabalasura::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_Boss_Mahabalasura), TAG_COM(Com_Model), (CComponent**)&m_pModel));
	FAILED_CHECK(m_pModel->Change_AnimIndex(0));

	CDissolve::DISSOLVEDESC DissolveDesc;
	DissolveDesc.pModel = m_pModel;
	DissolveDesc.eDissolveModelType = CDissolve::DISSOLVE_ANIM;
	DissolveDesc.pShader = m_pShaderCom;
	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Dissolve), TAG_COM(Com_Dissolve), (CComponent**)&m_pDissolveCom, &DissolveDesc));


	CTransform::TRANSFORMDESC tDesc = {};

	tDesc.fMovePerSec = 5;
	tDesc.fRotationPerSec = XMConvertToRadians(60);
	tDesc.fScalingPerSec = 1;
	tDesc.vPivot = _float3(0, 0, 0);

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_Collider), (CComponent**)&m_pCollider));

	COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(4.f, 4.f, 4.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	ATTACHEDESC tAttachedDesc;
	tAttachedDesc.Initialize_AttachedDesc(this, "pelvis", _float3(1), _float3(0), _float3(0.f, -0.032385f, -1.5257f));
	m_vecAttachedDesc.push_back(tAttachedDesc);

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.7f, 0.7f, 0.7f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "head", _float3(1), _float3(0), _float3(0.f, -0.030919f, -2.49767f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pCollider->Set_ParantBuffer();

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.2f, 1.2f, 1.2f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "spine_02", _float3(1), _float3(0), _float3(0.f, -0.0666f, -1.86665f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pCollider->Set_ParantBuffer();

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.5f, 1.5f, 1.5f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.6f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "pelvis", _float3(1), _float3(0), _float3(0.f, -0.032385f, -1.5257f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pCollider->Set_ParantBuffer();



	return S_OK;
}

HRESULT CMahabalasura::Adjust_AnimMovedTransform(_double fDeltatime)
{
	_uint iNowAnimIndex = m_pModel->Get_NowAnimIndex();
	_double PlayRate = m_pModel->Get_PlayRate();

	if (iNowAnimIndex != m_iOldAnimIndex || PlayRate > 0.98)
	{
		m_iAdjMovedIndex = 0;
		m_EffectAdjust = 0;
	}
	
	if (PlayRate <= 0.98)
	{
		switch (iNowAnimIndex)
		{
		case 0:
			//IDLE
			if (PlayRate > 0.f)
			{
				m_bIsAttack = false;
				m_bIsLookAt = true;
				m_bIsWalk = true;

				if (m_bIsSpearTurn)
				{
					CTransform* SpearTransform = (CTransform*)m_pSpear->Get_Component(TAG_COM(Com_Transform));
					SpearTransform->Rotation_CW(XMVectorSet(0, 0, 1.f, 0), XMConvertToRadians(0));
					/*_float Angle = g_pGameInstance->Easing(0, 180.f, 0, PlayRate, 9.5f);

					CTransform* SpearTransform = (CTransform*)m_pSpear->Get_Component(TAG_COM(Com_Transform));
					SpearTransform->Rotation_CW(XMVectorSet(0, 0, 1.f, 0), XMConvertToRadians(Angle));*/
				}
			}
			break;
		case 1:
		{
			if (PlayRate > 0.f && m_iAdjMovedIndex == 0)
			{
				m_bIsDissolveStart = true;
				m_pSpear->Set_IsDissolveStart();
				++m_iAdjMovedIndex;
			}
		}
			break;

		case 5:
		{
			m_bIsLookAt = false;
			m_fAnimmultiple = 1.3f;
			m_pSpear->Set_IsAttackState(true);

			if (PlayRate > 0.1401869f && PlayRate < 0.20f)
			{
				if (m_iAdjMovedIndex == 0)
				{



					if (Get_NowHP() <= m_fMaxHP/2.f)
					{
						m_bIsTeleport = true;
						m_bIsTeleportSound = false;
						m_bIsTeleportAttack = true;

						_float3 Pos = m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS);
						_int iTemp = rand() % 2 == 0 ? -1 : 1;
						Pos = _float3(Pos.x + (iTemp * GetSingle(CUtilityMgr)->RandomFloat(1.f, 2.5f)), Pos.y, Pos.z + (iTemp * GetSingle(CUtilityMgr)->RandomFloat(1.f, 2.5f)));

						m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, Pos);

					}
					else
					{
						g_pGameInstance->Play3D_Sound(L"JJB_MrM_Trishul_Swing_01.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);
					}
					++m_iAdjMovedIndex;
				}
				else if (m_iAdjMovedIndex == 1)
				{

					++m_iAdjMovedIndex;
					{
						CGameInstance* pInstance = g_pGameInstance;
						m_vecNonInstMeshDesc[0].vPosition = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS)
							+ m_pTransformCom->Get_MatrixState(CTransform::STATE_UP) * 1.0f
							+ m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK) * 10.2f;
						-m_pTransformCom->Get_MatrixState(CTransform::STATE_RIGHT) * 0.5f;

						m_vecNonInstMeshDesc[0].vLookDir = XMVector3Normalize((m_pTransformCom->Get_MatrixState(CTransform::STATE_POS)
							+ m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK)
							+ m_pTransformCom->Get_MatrixState(CTransform::STATE_UP) * 2.5f
							+ m_pTransformCom->Get_MatrixState(CTransform::STATE_RIGHT) * 1.25f)
							- m_vecNonInstMeshDesc[0].vPosition.XMVector());

						pInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_Particle), TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[0]);

						CGameObject* Obj = g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_Particle));
						CTransform* Transform = (CTransform*)Obj->Get_Component(TAG_COM(Com_Transform));

						CParticleCollider::SETTINGCOLLIDER ColliderDesc;
						ColliderDesc.ColliderType = COLLIDER_OBB;
						ColliderDesc.ColliderDesc.vScale = _float3(20.5f, 20.5f, 450.5f);
						ColliderDesc.ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
						ColliderDesc.ColliderDesc.vPosition = _float4(0.f, -1.f, 90.f, 1.f);
						ColliderDesc.pTargetTransform = Transform;

						g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_ParticleCollider),
							TAG_OP(Prototype_Object_ParticleCollider), &ColliderDesc);

						m_vecNonInstMeshDesc[0].vLookDir =
							XMVector3Normalize((m_pTransformCom->Get_MatrixState(CTransform::STATE_POS)
								+ m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK)
								+ m_pTransformCom->Get_MatrixState(CTransform::STATE_UP) * 2.5f
								- m_pTransformCom->Get_MatrixState(CTransform::STATE_RIGHT) * 1.25f)
								- m_vecNonInstMeshDesc[0].vPosition.XMVector());

						pInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_Particle), TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[0]);

						m_vecNonInstMeshDesc[0].vLookDir =
							XMVector3Normalize((m_pTransformCom->Get_MatrixState(CTransform::STATE_POS)
								+ m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK)
								+ m_pTransformCom->Get_MatrixState(CTransform::STATE_UP) * 0.5f
								+ m_pTransformCom->Get_MatrixState(CTransform::STATE_RIGHT) * 1.25f)
								- m_vecNonInstMeshDesc[0].vPosition.XMVector());

						pInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_Particle), TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[0]);


						m_vecNonInstMeshDesc[0].vLookDir =
							XMVector3Normalize((m_pTransformCom->Get_MatrixState(CTransform::STATE_POS)
								+ m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK)
								+ m_pTransformCom->Get_MatrixState(CTransform::STATE_UP) * 0.5f
								- m_pTransformCom->Get_MatrixState(CTransform::STATE_RIGHT) * 1.25f)
								- m_vecNonInstMeshDesc[0].vPosition.XMVector());

						pInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_Particle), TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[0]);

						Obj = g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_Particle));
						Transform = (CTransform*)Obj->Get_Component(TAG_COM(Com_Transform));

						ColliderDesc.ColliderType = COLLIDER_OBB;
						ColliderDesc.ColliderDesc.vScale = _float3(20.5f, 20.5f, 450.5f);
						ColliderDesc.ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
						ColliderDesc.ColliderDesc.vPosition = _float4(0.f, 1.f, 90.f, 1.f);
						ColliderDesc.pTargetTransform = Transform;

						g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_ParticleCollider),
							TAG_OP(Prototype_Object_ParticleCollider), &ColliderDesc);
					}

				}

				m_bIsLookAt = true;
				m_pTransformCom->Move_Forward(fDeltatime * 1.5);
			}
			else if (PlayRate > 0.36448598f && PlayRate < 0.45f)
			{
				if (m_iAdjMovedIndex == 2)
				{

					
					if (Get_NowHP() <= m_fMaxHP / 2.f)
					{
						m_bIsTeleport = true;
						m_bIsTeleportSound = false;
						m_bIsTeleportAttack = true;

						_float3 Pos = m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS);
						_int iTemp = rand() % 2 == 0 ? -1 : 1;
						Pos = _float3(Pos.x + (iTemp * GetSingle(CUtilityMgr)->RandomFloat(1.f, 2.5f)), Pos.y, Pos.z + (iTemp * GetSingle(CUtilityMgr)->RandomFloat(1.f, 2.5f)));

						m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, Pos);
					}
					else
					{
						g_pGameInstance->Play3D_Sound(L"JJB_MrM_Trishul_Swing_02.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);
					}
					++m_iAdjMovedIndex;
				}
				else if (m_iAdjMovedIndex == 3)
				{

					{

						CGameInstance* pInstance = g_pGameInstance;
						m_pTextureParticleTransform1->Set_Matrix(m_pTransformCom->Get_WorldMatrix());

						m_vecNonInstMeshDesc[1].vPosition = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS) +
							m_pTransformCom->Get_MatrixState(CTransform::STATE_UP) * 3.5f
							+ m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK) * 4.5f;
						m_vecNonInstMeshDesc[1].vLookDir = -m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK);


						pInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_Particle),
							TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[1]);



						m_vecNonInstMeshDesc[2].vPosition = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS) +
							m_pTransformCom->Get_MatrixState(CTransform::STATE_UP) * 1.f
							+ m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK) * 4.5f;
						m_vecNonInstMeshDesc[2].vLookDir = -m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK);

						pInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_Particle),
							TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[2]);


						m_vecNonInstMeshDesc[3].vPosition = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS) +
							m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK) * 4.f +
							m_pTransformCom->Get_MatrixState(CTransform::STATE_UP) * 1.25f;
						m_vecNonInstMeshDesc[3].vLookDir = m_vecNonInstMeshDesc[3].vPosition.XMVector() - (
							m_pTransformCom->Get_MatrixState(CTransform::STATE_POS) -
							m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK) * 4.f +
							m_pTransformCom->Get_MatrixState(CTransform::STATE_UP) * 100000.f);

						g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_Particle),
							TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[3]);

						CGameObject* Obj = g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_Particle));
						CTransform* Transform = (CTransform*)Obj->Get_Component(TAG_COM(Com_Transform));

						CParticleCollider::SETTINGCOLLIDER ColliderDesc;
						ColliderDesc.ColliderType = COLLIDER_SPHERE;
						ColliderDesc.ColliderDesc.vScale = _float3(3.5f, 3.5f, 3.5f);
						ColliderDesc.ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
						ColliderDesc.ColliderDesc.vPosition = _float4(0.f, 1.f, 0.f, 1.f);
						ColliderDesc.pTargetTransform = Transform;

						g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_ParticleCollider),
							TAG_OP(Prototype_Object_ParticleCollider), &ColliderDesc);

					}
					++m_iAdjMovedIndex;
				}
				else if (PlayRate > 0.40448598f && m_iAdjMovedIndex == 4)
				{

					{

						CGameInstance* pInstance = g_pGameInstance;

						m_vecNonInstMeshDesc[1].vPosition = m_pTextureParticleTransform1->Get_MatrixState(CTransform::STATE_POS) +
							m_pTextureParticleTransform1->Get_MatrixState(CTransform::STATE_UP) * 3.5f
							+ m_pTextureParticleTransform1->Get_MatrixState(CTransform::STATE_LOOK) * 10.5f;
						m_vecNonInstMeshDesc[1].vLookDir = -m_pTextureParticleTransform1->Get_MatrixState(CTransform::STATE_LOOK);


						pInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_Particle),
							TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[1]);



						m_vecNonInstMeshDesc[2].vPosition = m_pTextureParticleTransform1->Get_MatrixState(CTransform::STATE_POS) +
							m_pTextureParticleTransform1->Get_MatrixState(CTransform::STATE_UP) * 1.f
							+ m_pTextureParticleTransform1->Get_MatrixState(CTransform::STATE_LOOK) * 10.5f;
						m_vecNonInstMeshDesc[2].vLookDir = -m_pTextureParticleTransform1->Get_MatrixState(CTransform::STATE_LOOK);

						pInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_Particle),
							TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[2]);


						m_vecNonInstMeshDesc[3].vPosition = m_pTextureParticleTransform1->Get_MatrixState(CTransform::STATE_POS) +
							m_pTextureParticleTransform1->Get_MatrixState(CTransform::STATE_LOOK) * 10.f +
							m_pTextureParticleTransform1->Get_MatrixState(CTransform::STATE_UP) * 1.25f;
						m_vecNonInstMeshDesc[3].vLookDir = m_vecNonInstMeshDesc[3].vPosition.XMVector() - (
							m_pTextureParticleTransform1->Get_MatrixState(CTransform::STATE_POS) -
							m_pTextureParticleTransform1->Get_MatrixState(CTransform::STATE_LOOK) * 10.f +
							m_pTextureParticleTransform1->Get_MatrixState(CTransform::STATE_UP) * 100000.f);

						g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_Particle),
							TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[3]);

						CGameObject* Obj = g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_Particle));
						CTransform* Transform = (CTransform*)Obj->Get_Component(TAG_COM(Com_Transform));

						CParticleCollider::SETTINGCOLLIDER ColliderDesc;
						ColliderDesc.ColliderType = COLLIDER_SPHERE;
						ColliderDesc.ColliderDesc.vScale = _float3(3.5f, 3.5f, 3.5f);
						ColliderDesc.ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
						ColliderDesc.ColliderDesc.vPosition = _float4(0.f, 1.f, 0.f, 1.f);
						ColliderDesc.pTargetTransform = Transform;

						g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_ParticleCollider),
							TAG_OP(Prototype_Object_ParticleCollider), &ColliderDesc);


					}
					++m_iAdjMovedIndex;
				}
				else if (PlayRate > 0.44448598f && m_iAdjMovedIndex == 5)
				{
					{

						CGameInstance* pInstance = g_pGameInstance;

						m_vecNonInstMeshDesc[1].vPosition = m_pTextureParticleTransform1->Get_MatrixState(CTransform::STATE_POS) +
							m_pTextureParticleTransform1->Get_MatrixState(CTransform::STATE_UP) * 3.5f
							+ m_pTextureParticleTransform1->Get_MatrixState(CTransform::STATE_LOOK) * 16.5f;
						m_vecNonInstMeshDesc[1].vLookDir = -m_pTextureParticleTransform1->Get_MatrixState(CTransform::STATE_LOOK);


						pInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_Particle),
							TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[1]);



						m_vecNonInstMeshDesc[2].vPosition = m_pTextureParticleTransform1->Get_MatrixState(CTransform::STATE_POS) +
							m_pTextureParticleTransform1->Get_MatrixState(CTransform::STATE_UP) * 1.f
							+ m_pTextureParticleTransform1->Get_MatrixState(CTransform::STATE_LOOK) * 16.5f;
						m_vecNonInstMeshDesc[2].vLookDir = -m_pTextureParticleTransform1->Get_MatrixState(CTransform::STATE_LOOK);

						pInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_Particle),
							TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[2]);


						m_vecNonInstMeshDesc[3].vPosition = m_pTextureParticleTransform1->Get_MatrixState(CTransform::STATE_POS) +
							m_pTextureParticleTransform1->Get_MatrixState(CTransform::STATE_LOOK) * 16.f +
							m_pTextureParticleTransform1->Get_MatrixState(CTransform::STATE_UP) * 1.25f;
						m_vecNonInstMeshDesc[3].vLookDir = m_vecNonInstMeshDesc[3].vPosition.XMVector() - (
							m_pTextureParticleTransform1->Get_MatrixState(CTransform::STATE_POS) -
							m_pTextureParticleTransform1->Get_MatrixState(CTransform::STATE_LOOK) * 16.f +
							m_pTextureParticleTransform1->Get_MatrixState(CTransform::STATE_UP) * 100000.f);

						g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_Particle),
							TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[3]);

						CGameObject* Obj = g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_Particle));
						CTransform* Transform = (CTransform*)Obj->Get_Component(TAG_COM(Com_Transform));

						CParticleCollider::SETTINGCOLLIDER ColliderDesc;
						ColliderDesc.ColliderType = COLLIDER_SPHERE;
						ColliderDesc.ColliderDesc.vScale = _float3(3.5f, 3.5f, 3.5f);
						ColliderDesc.ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
						ColliderDesc.ColliderDesc.vPosition = _float4(0.f, 1.f, 0.f, 1.f);
						ColliderDesc.pTargetTransform = Transform;

						g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_ParticleCollider),
							TAG_OP(Prototype_Object_ParticleCollider), &ColliderDesc);

					}
			
					++m_iAdjMovedIndex;
				}
				m_bIsLookAt = true;
				m_pTransformCom->Move_Forward(fDeltatime * 1.5);
			}
			else if (PlayRate > 0.551401869f && PlayRate < 0.60f)
			{
				if (m_iAdjMovedIndex == 6)
				{


					if (Get_NowHP() <= m_fMaxHP / 2.f)
					{
						m_bIsTeleport = true;
						m_bIsTeleportSound = false;
						m_bIsTeleportAttack = true;

						_float3 Pos = m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS);
						_int iTemp = rand() % 2 == 0 ? -1 : 1;
						Pos = _float3(Pos.x + (iTemp * GetSingle(CUtilityMgr)->RandomFloat(1.f, 2.5f)), Pos.y, Pos.z + (iTemp * GetSingle(CUtilityMgr)->RandomFloat(1.f, 2.5f)));

						m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, Pos);

					}
					else
					{
						g_pGameInstance->Play3D_Sound(L"JJB_MrM_Trishul_Swing_03.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);
					}
					++m_iAdjMovedIndex;
				}
				else if (m_iAdjMovedIndex == 7)
				{
					++m_iAdjMovedIndex;

					CMahabalasura_SpearWave::SPEARWAVEDESC tSpearWaveDesc;
					tSpearWaveDesc.fStartPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
					tSpearWaveDesc.fStartPos.y += 1.8f;
					tSpearWaveDesc.iDir = 0;
					tSpearWaveDesc.fLookDir = XMVector3Normalize(XMVectorSetY(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK),0));
					g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_MonsterBullet),
						TAG_OP(Prototype_Object_Boss_MahabalasuraSpearWave), &tSpearWaveDesc);

					tSpearWaveDesc.fLookDir = XMVector3Normalize(XMVectorSetY(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK) * 10.f +
						m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_RIGHT) * 5.f, 0));
					tSpearWaveDesc.iDir = 2;
					g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_MonsterBullet),
						TAG_OP(Prototype_Object_Boss_MahabalasuraSpearWave), &tSpearWaveDesc);

					tSpearWaveDesc.fLookDir = XMVector3Normalize(XMVectorSetY(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK) * 10.f -
						m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_RIGHT) * 5.f, 0));
					tSpearWaveDesc.iDir =1;
					g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_MonsterBullet),
						TAG_OP(Prototype_Object_Boss_MahabalasuraSpearWave), &tSpearWaveDesc);


				}
				m_bIsLookAt = true;
				m_pTransformCom->Move_Forward(fDeltatime * 1.5);
			}

			if (PlayRate > 0.766355140f && m_iAdjMovedIndex == 3)
			{
				m_pSpear->Set_IsAttackState(false);
				++m_iAdjMovedIndex;
			}
		}
		break;

		case 6:
		{
			float Value = g_pGameInstance->Easing_Return(TYPE_Linear, TYPE_Linear, 0, 1, (_float)PlayRate, 0.9f);
			Value = max(min(Value, 1.f), 0.f);
			Set_LimLight_N_Emissive(_float4(0.0f, 0.00f, 1.0f, Value), _float4(Value, Value*0.7f, Value, 0.9f));


			if (m_iAdjMovedIndex == 0 && PlayRate > 0.0f)
			{

				_Vector RotAxis = XMVector3Normalize(XMVectorSetY(m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), 0)
					- XMVectorSetY(m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS), 0));

				if (m_pPlayerTransform->Get_MatrixState_Float3(CTransform::STATE_POS).z > m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS).z)
					RotAxis *= -1.f;
				//RotAxis = _float3(1, 0, 0).XMVector();
				CMahabalasura_Arms::Reset_StaticCountNRotAxis(RotAxis);

				m_iAdjMovedIndex++;
			}


			if (m_EffectAdjust == 0 && PlayRate > 0.1f)
			{
				mPatternCount2++;
				//if (mPatternCount2 == 1)
				//	Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_BOSS_Mahabalasura_SKILLHAND_0, m_pTextureParticleTransform);

				m_EffectAdjust++;
			}
				
		}
		break;

		case 7:
		{
			if (m_iAdjMovedIndex == 0 && PlayRate > 0.1)
			{
				_int iRandom = rand() % 2 + 1;

				wstring teampString;
				teampString = L"JJB_MrM_Make_clone_" + to_wstring(iRandom) + L".wav";

				g_pGameInstance->Play3D_Sound((_tchar*)teampString.c_str(), g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);

			//	Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_BOSS_Mahabalasura_SKILLCOPY_0, m_pTextureParticleTransform);

				++m_iAdjMovedIndex;
			}

			if (m_iAdjMovedIndex == 1 && PlayRate > 0.112676056)
			{
				g_pGameInstance->Play3D_Sound(L"JJB_MrM_Claw.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);
				++m_iAdjMovedIndex;
			}

			
			if (m_iAdjMovedIndex == 2 && PlayRate > 0.5076923f)
			{
				FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TEXT("Layer_CopyBoss"), TAG_OP(Prototype_Object_Boss_MahabalasuraCopy)));
				CCopyMahabalasura* CopyBoss = (CCopyMahabalasura*)g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum,TAG_LAY(Layer_Boss));

				m_bIsTeleport = true;
				m_bIsTeleportSound = false;
				m_bIsCopySkill = true;

				++m_iAdjMovedIndex;
			}


		}
		break;  
		
		case 9:
		{
			float Value = g_pGameInstance->Easing_Return(TYPE_Linear, TYPE_Linear, 0, 3, (_float)PlayRate, 0.9f);
			Value = max(min(Value, 1.f), 0.f);
			Set_LimLight_N_Emissive(_float4(1.f, 0.2f, 0.2f, Value), _float4(Value, Value*0.7f, Value, 0.9f));

			if (m_iAdjMovedIndex == 0 && PlayRate > 0.f)
			{
				if (m_iRainAttackCount == 0)
				{
					m_vecNonInstMeshDesc[4].fMaxTime_Duration = m_vecNonInstMeshDesc[5].fMaxTime_Duration = 6.0f;
					g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_Particle),
						TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[4]);
					g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_Particle),
						TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[5]);

				}
					m_vecNonInstMeshDesc[9].vPosition = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
					m_vecNonInstMeshDesc[9].vLookDir = m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK);

					g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_Particle),
						TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[9]);



				m_iAdjMovedIndex++;
			}

			if (m_iAdjMovedIndex == 1 && PlayRate > 0.1692307f)
			{


				g_pGameInstance->Play3D_Sound(L"JJB_MrM_Primary_Attack.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);
			

				if (m_iRainAttackCount == 0)
				{

					CMahabalasura_AttackSpear::MBSASD tDesc;
					tDesc.vPosition = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
					tDesc.fTotalTime = 5.f;
					tDesc.iKinds = 0;

					FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TEXT("Layer_Spear"), TAG_OP(Prototype_Object_Boss_MahabalasuraATKSPR), &tDesc));
					m_iRainAttackCount++;
				}
				else if(m_iRainAttackCount == 1)
				{

					CMahabalasura_AttackSpear::MBSASD tDesc;
					tDesc.vPosition = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
					tDesc.fTotalTime = 5.f;
					tDesc.iKinds = 1;
					FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TEXT("Layer_Spear"), TAG_OP(Prototype_Object_Boss_MahabalasuraATKSPR), &tDesc));


					m_iRainAttackCount++;

				}

				//CMahabalasura_Weapon::WEAPONDESC InstanceDesc;
				//InstanceDesc.m_CloneType = CMahabalasura_Weapon::CLONE_INSTANCE;
				//InstanceDesc.Pos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
				//InstanceDesc.m_eAttachedDesc.Initialize_AttachedDesc(this, "middle_metacarpal_r", XMVectorSet(100.f, 200.f, 150.f, 0.f), XMVectorSet(0, 0.f, 180.f, 0.f), XMVectorSet(-0.950 * 100.f, -0.160 * 200.f, -1.550 * 150.f, 1.f));
				//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TEXT("Layer_Spear"), TAG_OP(Prototype_Object_Boss_MahabalasuraWeapon), &InstanceDesc));
				//CMahabalasura_Weapon* Spear = (CMahabalasura_Weapon*)g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TEXT("Layer_Spear"));
				//Spear->Set_InstanceWeapon(0);




				++m_iAdjMovedIndex;
			}
			if (m_iAdjMovedIndex == 2 && PlayRate > 0.246153f)
			{
				//CMahabalasura_Weapon* Spear = (CMahabalasura_Weapon*)g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TEXT("Layer_Spear"));
				//Spear->Set_InstanceWeapon(1);

				//Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_BOSS_Mahabalasura_SKILLRAIN_0, m_pTextureParticleTransform);
			//	Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_BOSS_Mahabalasura_SKILLRAIN_0, m_pTransformCom);

				++m_iAdjMovedIndex;
			}
			if (m_iAdjMovedIndex == 3 && PlayRate > 0.353846f)
			{
				//CMahabalasura_Weapon* Spear = (CMahabalasura_Weapon*)g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TEXT("Layer_Spear"));

				//Spear->Set_InstanceWeapon(2);
				//Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_BOSS_Mahabalasura_SKILLRAIN_2, m_pTextureParticleTransform);


				++m_iAdjMovedIndex;
			}
			if (m_iAdjMovedIndex == 4 && PlayRate > 0.7f)
			{
				//Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_BOSS_Mahabalasura_SKILLRAIN_1, m_pTextureParticleTransform);

				//CMahabalasura_Weapon* Spear = (CMahabalasura_Weapon*)g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TEXT("Layer_Spear"));
				//Spear->Set_IsStab();
				g_pGameInstance->Play3D_Sound(L"JJB_MrM_Lightning_01.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);

				++m_iAdjMovedIndex;
			}

		}
		break;

		case 11:
		{
			float Value = g_pGameInstance->Easing_Return(TYPE_Linear, TYPE_Linear, 0, 20, (_float)PlayRate, 0.9f);
			Value = max(min(Value, 1.f), 0.f);
			Set_LimLight_N_Emissive(_float4(1.f, 1.f, 0.2f, Value), _float4(Value, Value*0.7f, Value, 0.9f));


			if (m_iAdjMovedIndex == 0)
			{
				m_bIsTeleport = true;
				m_bIsTeleportSound = false;

				//_float3 Pos = m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS);
				//_int iTemp = rand() % 2 == 0 ? -1 : 1;
				//Pos = _float3(Pos.x + (iTemp * GetSingle(CUtilityMgr)->RandomFloat(1.f, 2.5f)), Pos.y, Pos.z + (iTemp * GetSingle(CUtilityMgr)->RandomFloat(1.f, 2.5f)));

				m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, _float3(101.721f, 34.260f, 323.105f));

				++m_iAdjMovedIndex;

			}


			if (PlayRate > 0.f && PlayRate < 0.1321839)
			{
				m_bIsSpearTurn = true;

				_float Angle = g_pGameInstance->Easing(0, 0, 180.f, (_float)PlayRate, 0.1321f);

				CTransform* SpearTransform =  (CTransform*)m_pSpear->Get_Component(TAG_COM(Com_Transform));
				SpearTransform->Rotation_CW(XMVectorSet(1.f, 0, 00.f, 0), XMConvertToRadians(Angle));
			}
			if (PlayRate > 0.16091954 && m_iAdjMovedIndex == 1)
			{
				g_pGameInstance->Play3D_Sound(L"JJB_MrM_Secondary_Attack.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);
				
				++m_iAdjMovedIndex;
			}
			if (PlayRate > 0.16091954 && m_iAdjMovedIndex == 2)
			{

				//CTransform* PlayerTransform = (CTransform*)m_pPlayerObj->Get_Component(TAG_COM(Com_Transform));
				//_float3 PlayerPos = PlayerTransform->Get_MatrixState(CTransform::STATE_POS);
				//
				//CMahabalasura_Weapon::WEAPONDESC Desc;
				//Desc.m_CloneType = CMahabalasura_Weapon::CLONE_SKILL;
				//PlayerPos.y -= 5.f;
				//Desc.Pos = PlayerPos;
				//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TEXT("Layer_Spear"), TAG_OP(Prototype_Object_Boss_MahabalasuraWeapon), &Desc));
				//g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TEXT("Layer_Spear"));

				// CTransform* SpearTransform = (CTransform*)m_pSpear->Get_Component(TAG_COM(Com_Transform));

				m_vecNonInstMeshDesc[4].fMaxTime_Duration = m_vecNonInstMeshDesc[5].fMaxTime_Duration = 7.25f;
				g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_Particle),
					TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[4]);
				g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_Particle),
					TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[5]);


				m_vecNonInstMeshDesc[7].vPosition = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
				m_vecNonInstMeshDesc[7].vLookDir = -(m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK));
				g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_Particle),
					TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[7]);

				++m_iAdjMovedIndex;
			}

			if (PlayRate > 0.258620689 && m_iAdjMovedIndex == 3)
			{
				//g_pGameInstance->Play3D_Sound(L"JJB_MrM_Secondary_Attack.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);

				m_vecNonInstMeshDesc[8].vPosition = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS)
					+ m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK) * 0.15f
					+ m_pTransformCom->Get_MatrixState(CTransform::STATE_RIGHT) * 0.15f;
				m_vecNonInstMeshDesc[8].vLookDir = -(m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK));
				g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_Particle),
					TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[8]);


				++m_iAdjMovedIndex;
			}
			if (PlayRate > 0.36781609 && m_iAdjMovedIndex == 4)
			{
				g_pGameInstance->Play3D_Sound(L"JJB_MrM_Secondary_Attack.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);

				CTransform* PlayerTransform = (CTransform*)m_pPlayerObj->Get_Component(TAG_COM(Com_Transform));
				_Vector PlayerPos = PlayerTransform->Get_MatrixState(CTransform::STATE_POS);

				CUtilityMgr* pUtil = GetSingle(CUtilityMgr);



				for (size_t i = 0; i < 25; i++)
				{
					m_vecNonInstMeshDesc[6].eMeshType = COMPONENTPROTOTYPEID(Prototype_Mesh_Lightning_01 + rand() % 3);


					m_vecNonInstMeshDesc[6].vPosition = PlayerPos + XMVectorSetY(pUtil->RandomFloat3(-15,15).XMVector()	,0);
					m_vecNonInstMeshDesc[6].vLookDir =
						XMVector3Normalize(XMVectorSet(0,8,0,0) + XMVectorSetY(pUtil->RandomFloat3(-1, 1).XMVector(), 0));


					g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_Particle),
						TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[6]);

					CGameObject* Obj = g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_Particle));
					CTransform* Transform = (CTransform*)Obj->Get_Component(TAG_COM(Com_Transform));

					CParticleCollider::SETTINGCOLLIDER ColliderDesc;
					ColliderDesc.ColliderType = COLLIDER_SPHERE;
					ColliderDesc.ColliderDesc.vScale = _float3(7.f, 7.f, 7.f);
					ColliderDesc.ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
					ColliderDesc.ColliderDesc.vPosition = _float4(0.f, 1.f, 0.f, 1.f);
					ColliderDesc.fWaitingTime = 0.7f;
					ColliderDesc.pTargetTransform = Transform;

					g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_ParticleCollider),
						TAG_OP(Prototype_Object_ParticleCollider), &ColliderDesc);

				}


				//CMahabalasura_Weapon::WEAPONDESC Desc;
				//Desc.m_CloneType = CMahabalasura_Weapon::CLONE_SKILL;
				//PlayerPos.y -= 5.f;
				//Desc.Pos = PlayerPos;
				//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TEXT("Layer_Spear"), TAG_OP(Prototype_Object_Boss_MahabalasuraWeapon), &Desc));
				////g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TEXT("Layer_Spear"));

				//Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_BOSS_Mahabalasura_SKILLSPEAR_0, m_pTransformCom);
				//Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_BOSS_Mahabalasura_SKILLSPEAR_1, m_pTextureParticleTransform2);
				//Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_BOSS_Mahabalasura_SKILLSPEAR_2, m_pTransformCom);
				++m_iAdjMovedIndex;
			}
			if (PlayRate > 0.45402298 && m_iAdjMovedIndex == 5)
			{
				//g_pGameInstance->Play3D_Sound(L"JJB_MrM_Secondary_Attack.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);
				m_vecNonInstMeshDesc[8].vPosition = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS)
					+ m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK) * 0.15f
					+ m_pTransformCom->Get_MatrixState(CTransform::STATE_RIGHT) * 0.15f;
				m_vecNonInstMeshDesc[8].vLookDir = -(m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK));
				g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_Particle),
					TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[8]);
				++m_iAdjMovedIndex;
			}

			if (PlayRate > 0.57471264 && m_iAdjMovedIndex == 6)
			{
				g_pGameInstance->Play3D_Sound(L"JJB_MrM_Secondary_Attack.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);
			
				CTransform* PlayerTransform = (CTransform*)m_pPlayerObj->Get_Component(TAG_COM(Com_Transform));

				_Vector PlayerPos = PlayerTransform->Get_MatrixState(CTransform::STATE_POS);

				CUtilityMgr* pUtil = GetSingle(CUtilityMgr);



				for (size_t i = 0; i < 25; i++)
				{
					m_vecNonInstMeshDesc[6].eMeshType = COMPONENTPROTOTYPEID(Prototype_Mesh_Lightning_01 + rand() % 3);


					m_vecNonInstMeshDesc[6].vPosition = PlayerPos + XMVectorSetY(pUtil->RandomFloat3(-15, 15).XMVector(), 0);
					m_vecNonInstMeshDesc[6].vLookDir =
						XMVector3Normalize(XMVectorSet(0, 8, 0, 0) + XMVectorSetY(pUtil->RandomFloat3(-1, 1).XMVector(), 0));


					g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_Particle),
						TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[6]);

					CGameObject* Obj = g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_Particle));
					CTransform* Transform = (CTransform*)Obj->Get_Component(TAG_COM(Com_Transform));

					CParticleCollider::SETTINGCOLLIDER ColliderDesc;
					ColliderDesc.ColliderType = COLLIDER_SPHERE;
					ColliderDesc.ColliderDesc.vScale = _float3(7.f, 7.f, 7.f);
					ColliderDesc.ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
					ColliderDesc.ColliderDesc.vPosition = _float4(0.f, 1.f, 0.f, 1.f);
					ColliderDesc.fWaitingTime = 0.7f;
					ColliderDesc.pTargetTransform = Transform;

					g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_ParticleCollider),
						TAG_OP(Prototype_Object_ParticleCollider), &ColliderDesc);
				}




				//_float3 PlayerPos = PlayerTransform->Get_MatrixState(CTransform::STATE_POS);
				//
				//CMahabalasura_Weapon::WEAPONDESC Desc;
				//Desc.m_CloneType = CMahabalasura_Weapon::CLONE_SKILL;
				//PlayerPos.y -= 5.f;
				//Desc.Pos = PlayerPos;
				//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TEXT("Layer_Spear"), TAG_OP(Prototype_Object_Boss_MahabalasuraWeapon), &Desc));
				//g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TEXT("Layer_Spear"));

				++m_iAdjMovedIndex;
			}

			if (PlayRate > 0.655172413 && m_iAdjMovedIndex == 7)
			{
				//g_pGameInstance->Play3D_Sound(L"JJB_MrM_Secondary_Attack.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);
				m_vecNonInstMeshDesc[8].vPosition = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS)
					+ m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK) * 0.15f
					+ m_pTransformCom->Get_MatrixState(CTransform::STATE_RIGHT) * 0.15f;
				m_vecNonInstMeshDesc[8].vLookDir = -(m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK));
				g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_Particle),
					TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[8]);
				++m_iAdjMovedIndex;
			}
			if (PlayRate > 0.7758620 && m_iAdjMovedIndex == 8)
			{
				g_pGameInstance->Play3D_Sound(L"JJB_MrM_Secondary_Attack.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);

				CTransform* PlayerTransform = (CTransform*)m_pPlayerObj->Get_Component(TAG_COM(Com_Transform));

				_Vector PlayerPos = PlayerTransform->Get_MatrixState(CTransform::STATE_POS);

				CUtilityMgr* pUtil = GetSingle(CUtilityMgr);



				for (size_t i = 0; i < 25; i++)
				{
					m_vecNonInstMeshDesc[6].eMeshType = COMPONENTPROTOTYPEID(Prototype_Mesh_Lightning_01 + rand() % 3);


					m_vecNonInstMeshDesc[6].vPosition = PlayerPos + XMVectorSetY(pUtil->RandomFloat3(-15, 15).XMVector(), 0);
					m_vecNonInstMeshDesc[6].vLookDir =
						XMVector3Normalize(XMVectorSet(0, 8, 0, 0) + XMVectorSetY(pUtil->RandomFloat3(-1, 1).XMVector(), 0));


					g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_Particle),
						TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[6]);

					CGameObject* Obj = g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_Particle));
					CTransform* Transform = (CTransform*)Obj->Get_Component(TAG_COM(Com_Transform));

					CParticleCollider::SETTINGCOLLIDER ColliderDesc;
					ColliderDesc.ColliderType = COLLIDER_SPHERE;
					ColliderDesc.ColliderDesc.vScale = _float3(7.f, 7.f, 7.f);
					ColliderDesc.ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
					ColliderDesc.ColliderDesc.vPosition = _float4(0.f, 1.f, 0.f, 1.f);
					ColliderDesc.fWaitingTime = 0.7f;
					ColliderDesc.pTargetTransform = Transform;

					g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_ParticleCollider),
						TAG_OP(Prototype_Object_ParticleCollider), &ColliderDesc);
				}



				//_float3 PlayerPos = PlayerTransform->Get_MatrixState(CTransform::STATE_POS);
				//
				//CMahabalasura_Weapon::WEAPONDESC Desc;
				//Desc.m_CloneType = CMahabalasura_Weapon::CLONE_SKILL;
				//PlayerPos.y -= 5.f;
				//Desc.Pos = PlayerPos;
				//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TEXT("Layer_Spear"), TAG_OP(Prototype_Object_Boss_MahabalasuraWeapon), &Desc));
				//g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TEXT("Layer_Spear"));

				++m_iAdjMovedIndex;
			}

			if (PlayRate > 0.8563218390 && m_iAdjMovedIndex == 9)
			{




				//g_pGameInstance->Play3D_Sound(L"JJB_MrM_Secondary_Attack.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);
				m_vecNonInstMeshDesc[8].vPosition = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS)
					+ m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK) * 0.15f
					+ m_pTransformCom->Get_MatrixState(CTransform::STATE_RIGHT) * 0.15f;
				m_vecNonInstMeshDesc[8].vLookDir = -(m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK));
				g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_Particle),
					TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[8]);
				++m_iAdjMovedIndex;
			}
			if (PlayRate > 0.977011 && m_iAdjMovedIndex == 10)
			{
				g_pGameInstance->Play3D_Sound(L"JJB_MrM_Secondary_Attack.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);


				CTransform* PlayerTransform = (CTransform*)m_pPlayerObj->Get_Component(TAG_COM(Com_Transform));

				_Vector PlayerPos = PlayerTransform->Get_MatrixState(CTransform::STATE_POS);

				CUtilityMgr* pUtil = GetSingle(CUtilityMgr);



				for (size_t i = 0; i < 25; i++)
				{
					m_vecNonInstMeshDesc[6].eMeshType = COMPONENTPROTOTYPEID(Prototype_Mesh_Lightning_01 + rand() % 3);


					m_vecNonInstMeshDesc[6].vPosition = PlayerPos + XMVectorSetY(pUtil->RandomFloat3(-15, 15).XMVector(), 0);
					m_vecNonInstMeshDesc[6].vLookDir =
						XMVector3Normalize(XMVectorSet(0, 8, 0, 0) + XMVectorSetY(pUtil->RandomFloat3(-1, 1).XMVector(), 0));


					g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_Particle),
						TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[6]);

					CGameObject* Obj = g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_Particle));
					CTransform* Transform = (CTransform*)Obj->Get_Component(TAG_COM(Com_Transform));

					CParticleCollider::SETTINGCOLLIDER ColliderDesc;
					ColliderDesc.ColliderType = COLLIDER_SPHERE;
					ColliderDesc.ColliderDesc.vScale = _float3(7.f, 7.f, 7.f);
					ColliderDesc.ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
					ColliderDesc.ColliderDesc.vPosition = _float4(0.f, 1.f, 0.f, 1.f);
					ColliderDesc.fWaitingTime = 0.7f;
					ColliderDesc.pTargetTransform = Transform;

					g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_ParticleCollider),
						TAG_OP(Prototype_Object_ParticleCollider), &ColliderDesc);
				}



				//_float3 PlayerPos = PlayerTransform->Get_MatrixState(CTransform::STATE_POS);
				//
				//CMahabalasura_Weapon::WEAPONDESC Desc;
				//Desc.m_CloneType = CMahabalasura_Weapon::CLONE_SKILL;
				//PlayerPos.y -= 5.f;
				//Desc.Pos = PlayerPos;
				//FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TEXT("Layer_Spear"), TAG_OP(Prototype_Object_Boss_MahabalasuraWeapon), &Desc));
				//g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TEXT("Layer_Spear"));

				++m_iAdjMovedIndex;
			}

			//// EFFECT
			//if (m_EffectAdjust == 0 && PlayRate > 0.16091954)
			//{

			//	Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_BOSS_Mahabalasura_SKILLSPEAR_0, m_pTransformCom);
			//	Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_BOSS_Mahabalasura_SKILLSPEAR_1, m_pTextureParticleTransform2);
			//	Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_BOSS_Mahabalasura_SKILLSPEAR_2, m_pTransformCom);

			//	m_EffectAdjust++;
			//}

			//if (m_EffectAdjust == 1 && PlayRate > 0.36781609)
			//{
			//	Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_BOSS_Mahabalasura_SKILLSPEAR_0, m_pTransformCom);
			//	Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_BOSS_Mahabalasura_SKILLSPEAR_1, m_pTextureParticleTransform2);
			//		Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_BOSS_Mahabalasura_SKILLSPEAR_2, m_pTransformCom);

			//	m_EffectAdjust++;
			//}
			//if (m_EffectAdjust == 2 && PlayRate > 0.57471264)
			//{
			//	Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_BOSS_Mahabalasura_SKILLSPEAR_0, m_pTransformCom);
			//	Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_BOSS_Mahabalasura_SKILLSPEAR_1, m_pTextureParticleTransform2);
			//	Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_BOSS_Mahabalasura_SKILLSPEAR_2, m_pTransformCom);
			//	m_EffectAdjust++;
			//}
			//if (m_EffectAdjust == 3 && PlayRate > 0.7758620)
			//{
			//	Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_BOSS_Mahabalasura_SKILLSPEAR_0, m_pTransformCom);
			//	Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_BOSS_Mahabalasura_SKILLSPEAR_1, m_pTextureParticleTransform2);
			//	Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_BOSS_Mahabalasura_SKILLSPEAR_2, m_pTransformCom);

			//	m_EffectAdjust++;
			//}
			//if (m_EffectAdjust == 4 && PlayRate > 0.977011)
			//{
			//	Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_BOSS_Mahabalasura_SKILLSPEAR_0, m_pTransformCom);
			//	Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_BOSS_Mahabalasura_SKILLSPEAR_1, m_pTextureParticleTransform2);
			//	Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_BOSS_Mahabalasura_SKILLSPEAR_2, m_pTransformCom);

			//	m_EffectAdjust++;

			//}
	

			// after Middle Test
			//if (m_EffectAdjust == 0 && PlayRate > 0.03f)
			//{
			//	m_EffectAdjust++;
			//}

			//if (m_EffectAdjust == 1 && PlayRate > 0.03f)
			//{
			//	m_EffectAdjust++;
			//}


			//if (m_EffectAdjust == 2 && PlayRate > 0.03f)
			//{
			//	m_EffectAdjust++;
			//}


			//if (m_EffectAdjust == 3 && PlayRate > 0.03f)
			//{
			//	m_EffectAdjust++;
			//}


			//if (m_EffectAdjust == 4 && PlayRate > 0.03f)
			//{
			//	m_EffectAdjust++;
			//}


			//if (m_EffectAdjust == 5 && PlayRate > 0.03f)
			//{
			//	m_EffectAdjust++;
			//}


			//if (m_EffectAdjust == 6 && PlayRate > 0.03f)
			//{
			//	m_EffectAdjust++;
			//}

			//if (m_EffectAdjust == 7 && PlayRate > 0.03f)
			//{
			//	m_EffectAdjust++;
			//}


			//if (m_EffectAdjust == 8 && PlayRate > 0.03f)
			//{
			//	m_EffectAdjust++;
			//}


			//if (m_EffectAdjust == 9 && PlayRate > 0.03f)
			//{
			//	m_EffectAdjust++;
			//}


			//if (m_EffectAdjust == 10 && PlayRate > 0.03f)
			//{
			//	m_EffectAdjust++;
			//}


			

			

		}
		break;


		}
	}
	else
	{
		if (iNowAnimIndex == 0)
		{
		}
		if (iNowAnimIndex == 1)
		{
			m_pModel->Change_AnimIndex(0);
		}
		if (iNowAnimIndex == 4)
		{
			m_bIsAttack = false;
			m_fAttackCoolTime = 2.f;
			m_fSkillCoolTime = 5.f;
		}
		if (iNowAnimIndex == 5)
		{
			m_pSpear->Set_IsAttackState(false);
			m_fAttackCoolTime = 4.f;
			//m_fSkillCoolTime += 2.f;
			m_fAnimmultiple = 1.f;

		}
		if (iNowAnimIndex == 6)
		{
		}
		if (iNowAnimIndex == 7)
		{
			m_fAttackCoolTime = 2.f;
			m_fSkillCoolTime = 5.f;
		}
		if (iNowAnimIndex == 9)
		{
			if(m_iRainAttackCount == 1)
			{
				m_pModel->Change_AnimIndex_ReturnTo_Must(9, 0);
			}
			else
			{
				m_fAttackCoolTime = 2.f;
				m_fSkillCoolTime = 5.f;
			}
		}
		if (iNowAnimIndex == 11)
		{
			m_fAttackCoolTime = 2.f;
			m_fSkillCoolTime = 5.f;
		}
	}

	m_iOldAnimIndex = iNowAnimIndex;
	return S_OK;
}

CMahabalasura * CMahabalasura::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CMahabalasura*	pInstance = NEW CMahabalasura(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CMahabalasura");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMahabalasura::Clone(void * pArg)
{
	CMahabalasura*	pInstance = NEW CMahabalasura(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created  Clone CMahabalasura");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMahabalasura::Free()
{
	__super::Free();



	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModel);
	Safe_Release(m_pCollider);
	Safe_Release(m_pDissolveCom);

	Safe_Release(m_pSpear);

	Safe_Release(m_pTextureParticleTransform);
	Safe_Release(m_pTextureParticleTransform1);
	Safe_Release(m_pTextureParticleTransform2);


	for (auto& Arm : m_pArms)
	{
		Safe_Release(Arm);
	}
	m_pArms.clear();

	Safe_Release(m_pHPUI);
}
