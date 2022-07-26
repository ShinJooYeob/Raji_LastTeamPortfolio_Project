#include "stdafx.h"
#include "..\public\Mahabalasura.h"

#include "Mahabalasura_Weapon.h"
#include "Mahabalasura_Arms.h"
#include "CopyMahabalasura.h"
#include "HpUI.h"

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

	//Set_LimLight_N_Emissive(_float4(), 0);

	m_fAttackCoolTime = 2.f;
	m_fSkillCoolTime = 5.f;
	m_bIsHit = false;


	m_pPlayerObj = (CGameObject*)g_pGameInstance->Get_GameObject_By_LayerIndex(m_eNowSceneNum,TEXT("Layer_Player"));
	m_pPlayerTransform = (CTransform*)m_pPlayerObj->Get_Component(TAG_COM(Com_Transform));

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
			ArmsDesc.Initialize_AttachedDesc(this, "upperarm_l", XMVectorSet(1.f, 1.f, 1.f, 0.f), XMVectorSet(-10.f, 190.f, 60.f, 0.f), XMVectorSet(0.343f, 0.088f, -2.215f, 1.f));
			break;
		case 1:
			ArmsDesc.Initialize_AttachedDesc(this, "upperarm_l", XMVectorSet(1.f, 1.f, 1.f, 0.f), XMVectorSet(-10.f, 180.f, 60.f, 0.f), XMVectorSet(0.501f, 0.099f, -2.149f, 1.f));
			break;
		case 2:
			ArmsDesc.Initialize_AttachedDesc(this, "upperarm_l", XMVectorSet(1.f, 1.f, 1.f, 0.f), XMVectorSet(-10.f, 160.f, 60.f, 0.f), XMVectorSet(0.659f, 0.110f, -2.083f, 1.f));
			break;
		case 3:
			ArmsDesc.Initialize_AttachedDesc(this, "upperarm_l", XMVectorSet(1.f, 1.f, 1.f, 0.f), XMVectorSet(-10.f, 140.f, 30.f, 0.f), XMVectorSet(0.661f, 0.001f, -1.846f, 1.f));
			break;
		case 4:
			ArmsDesc.Initialize_AttachedDesc(this, "upperarm_l", XMVectorSet(1.f, 1.f, 1.f, 0.f), XMVectorSet(-20.f, 180.f, 0.f, 0.f), XMVectorSet(0.792f, -0.191f, -1.997f, 1.f));
			break;
		case 5:
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


	g_pGameInstance->Play3D_Sound(L"JJB_MrM_Intro.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);
	return S_OK;
}

_int CMahabalasura::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)return -1;

	m_pDissolveCom->Update_Dissolving(fDeltaTime);

	if (m_pDissolveCom->Get_IsFadeIn() == false && m_pDissolveCom->Get_DissolvingRate() >= 1.0)
	{
		Set_IsDead();
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
			m_fSkillCoolTime = 5.f;
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
	if (m_fRange < 4.f && !m_bIsAttack && m_fAttackCoolTime <=0 && !m_bIsHit && m_fHP > 0)
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

	//스킬공격
	if (!m_bIsAttack && m_fSkillCoolTime <= 0 && !m_bIsHit && m_fHP > 0)
	{
			_int iRandom = (_int)GetSingle(CUtilityMgr)->RandomFloat(0, 3.9f);
			m_bIsAttack = true;

		iRandom = mOnlyPattern; // DEBUG
		//iRandom = 3;
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
			m_bIsTeleport = true;
			m_bIsTeleportSound = false;
			m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, _float3(0));
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
			m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, _float3(0));
			m_pModel->Change_AnimIndex_ReturnTo(9, 0);
		}
		break;

		case SKILL_COPY:
		{
			m_pModel->Change_AnimIndex_ReturnTo(7, 0);
		}
		break;

		}
		++TestNumber;
		if (TestNumber > 3)
			TestNumber = 0;

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
		m_pDissolveCom->Set_DissolveOn(false, 6.4f);

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

	m_pTextureParticleTransform1->Set_Matrix(mat_Spear);


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
		m_iAdjMovedIndex = 0;

	
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
				m_bIsLookAt = true;
				m_pTransformCom->Move_Forward(fDeltatime * 1.5);
			}
			else if (PlayRate > 0.36448598f && PlayRate < 0.45f)
			{
				if (m_iAdjMovedIndex == 1)
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
				m_bIsLookAt = true;
				m_pTransformCom->Move_Forward(fDeltatime * 1.5);
			}
			else if (PlayRate > 0.551401869f && PlayRate < 0.60f)
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
						g_pGameInstance->Play3D_Sound(L"JJB_MrM_Trishul_Swing_03.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);
					}
					++m_iAdjMovedIndex;
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


			if (m_iAdjMovedIndex == 0 && PlayRate > 0.1f)
			{
				mPatternCount2++;
				if(mPatternCount2 == 1)
					Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_BOSS_Mahabalasura_SKILLHAND_0, m_pTextureParticleTransform);

				m_iAdjMovedIndex++;
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

				Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_BOSS_Mahabalasura_SKILLCOPY_0, m_pTextureParticleTransform);

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
			if (m_iAdjMovedIndex == 0 && PlayRate > 0.1692307f)
			{
				g_pGameInstance->Play3D_Sound(L"JJB_MrM_Primary_Attack.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);
			
				CMahabalasura_Weapon::WEAPONDESC InstanceDesc;
				InstanceDesc.m_CloneType = CMahabalasura_Weapon::CLONE_INSTANCE;
				InstanceDesc.Pos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
				InstanceDesc.m_eAttachedDesc.Initialize_AttachedDesc(this, "middle_metacarpal_r", XMVectorSet(100.f, 200.f, 150.f, 0.f), XMVectorSet(0, 0.f, 180.f, 0.f), XMVectorSet(-0.950 * 100.f, -0.160 * 200.f, -1.550 * 150.f, 1.f));
				FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TEXT("Layer_Spear"), TAG_OP(Prototype_Object_Boss_MahabalasuraWeapon), &InstanceDesc));
				CMahabalasura_Weapon* Spear = (CMahabalasura_Weapon*)g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TEXT("Layer_Spear"));
				Spear->Set_InstanceWeapon(0);
				++m_iAdjMovedIndex;
			}
			if (m_iAdjMovedIndex == 1 && PlayRate > 0.246153f)
			{
				CMahabalasura_Weapon* Spear = (CMahabalasura_Weapon*)g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TEXT("Layer_Spear"));
				Spear->Set_InstanceWeapon(1);

				Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_BOSS_Mahabalasura_SKILLRAIN_0, m_pTextureParticleTransform);

				++m_iAdjMovedIndex;
			}
			if (m_iAdjMovedIndex == 2 && PlayRate > 0.353846f)
			{
				CMahabalasura_Weapon* Spear = (CMahabalasura_Weapon*)g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TEXT("Layer_Spear"));
				Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_BOSS_Mahabalasura_SKILLRAIN_2, m_pTextureParticleTransform);

				Spear->Set_InstanceWeapon(2);
				++m_iAdjMovedIndex;
			}
			if (m_iAdjMovedIndex == 3 && PlayRate > 0.7f)
			{
				Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_BOSS_Mahabalasura_SKILLRAIN_1, m_pTextureParticleTransform);

				CMahabalasura_Weapon* Spear = (CMahabalasura_Weapon*)g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TEXT("Layer_Spear"));
				Spear->Set_IsStab();
				g_pGameInstance->Play3D_Sound(L"JJB_MrM_Lightning_01.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);

				++m_iAdjMovedIndex;
			}

		}
		break;

		case 11:
		{
			float Value = g_pGameInstance->Easing_Return(TYPE_Linear, TYPE_Linear, 0, 1, (_float)PlayRate, 0.9f);
			Value = max(min(Value, 1.f), 0.f);
			Set_LimLight_N_Emissive(_float4(0.0f, 0.00f, 1.0f, Value), _float4(Value, Value*0.7f, Value, 0.9f));


			if (m_iAdjMovedIndex == 0)
			{
				m_bIsTeleport = true;
				m_bIsTeleportSound = false;

				m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, _float3(0));

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

				CTransform* PlayerTransform = (CTransform*)m_pPlayerObj->Get_Component(TAG_COM(Com_Transform));
				_float3 PlayerPos = PlayerTransform->Get_MatrixState(CTransform::STATE_POS);

				CMahabalasura_Weapon::WEAPONDESC Desc;
				Desc.m_CloneType = CMahabalasura_Weapon::CLONE_SKILL;
				PlayerPos.y -= 5.f;
				Desc.Pos = PlayerPos;
				FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TEXT("Layer_Spear"), TAG_OP(Prototype_Object_Boss_MahabalasuraWeapon), &Desc));
				//g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TEXT("Layer_Spear"));

				// CTransform* SpearTransform = (CTransform*)m_pSpear->Get_Component(TAG_COM(Com_Transform));


				Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_BOSS_Mahabalasura_SKILLSPEAR_0, m_pTransformCom);
				Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_BOSS_Mahabalasura_SKILLSPEAR_1, m_pTextureParticleTransform2);
				Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_BOSS_Mahabalasura_SKILLSPEAR_2, m_pTransformCom);

				++m_iAdjMovedIndex;
			}

			if (PlayRate > 0.258620689 && m_iAdjMovedIndex == 3)
			{
				//g_pGameInstance->Play3D_Sound(L"JJB_MrM_Secondary_Attack.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);

				++m_iAdjMovedIndex;
			}
			if (PlayRate > 0.36781609 && m_iAdjMovedIndex == 4)
			{
				g_pGameInstance->Play3D_Sound(L"JJB_MrM_Secondary_Attack.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);

				CTransform* PlayerTransform = (CTransform*)m_pPlayerObj->Get_Component(TAG_COM(Com_Transform));
				_float3 PlayerPos = PlayerTransform->Get_MatrixState(CTransform::STATE_POS);

				CMahabalasura_Weapon::WEAPONDESC Desc;
				Desc.m_CloneType = CMahabalasura_Weapon::CLONE_SKILL;
				PlayerPos.y -= 5.f;
				Desc.Pos = PlayerPos;
				FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TEXT("Layer_Spear"), TAG_OP(Prototype_Object_Boss_MahabalasuraWeapon), &Desc));
				//g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TEXT("Layer_Spear"));

				Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_BOSS_Mahabalasura_SKILLSPEAR_0, m_pTransformCom);
				Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_BOSS_Mahabalasura_SKILLSPEAR_1, m_pTextureParticleTransform2);
				Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_BOSS_Mahabalasura_SKILLSPEAR_2, m_pTransformCom);
				++m_iAdjMovedIndex;
			}
			if (PlayRate > 0.45402298 && m_iAdjMovedIndex == 5)
			{
				//g_pGameInstance->Play3D_Sound(L"JJB_MrM_Secondary_Attack.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);

				++m_iAdjMovedIndex;
			}

			if (PlayRate > 0.57471264 && m_iAdjMovedIndex == 6)
			{
				g_pGameInstance->Play3D_Sound(L"JJB_MrM_Secondary_Attack.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);
			
				CTransform* PlayerTransform = (CTransform*)m_pPlayerObj->Get_Component(TAG_COM(Com_Transform));
				_float3 PlayerPos = PlayerTransform->Get_MatrixState(CTransform::STATE_POS);

				CMahabalasura_Weapon::WEAPONDESC Desc;
				Desc.m_CloneType = CMahabalasura_Weapon::CLONE_SKILL;
				PlayerPos.y -= 5.f;
				Desc.Pos = PlayerPos;
				FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TEXT("Layer_Spear"), TAG_OP(Prototype_Object_Boss_MahabalasuraWeapon), &Desc));
				//g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TEXT("Layer_Spear"));

				Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_BOSS_Mahabalasura_SKILLSPEAR_0, m_pTransformCom);
				Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_BOSS_Mahabalasura_SKILLSPEAR_1, m_pTextureParticleTransform2);
				Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_BOSS_Mahabalasura_SKILLSPEAR_2, m_pTransformCom);
				++m_iAdjMovedIndex;
			}

			if (PlayRate > 0.655172413 && m_iAdjMovedIndex == 7)
			{
				//g_pGameInstance->Play3D_Sound(L"JJB_MrM_Secondary_Attack.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);

				++m_iAdjMovedIndex;
			}
			if (PlayRate > 0.7758620 && m_iAdjMovedIndex == 8)
			{
				g_pGameInstance->Play3D_Sound(L"JJB_MrM_Secondary_Attack.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);

				CTransform* PlayerTransform = (CTransform*)m_pPlayerObj->Get_Component(TAG_COM(Com_Transform));
				_float3 PlayerPos = PlayerTransform->Get_MatrixState(CTransform::STATE_POS);

				CMahabalasura_Weapon::WEAPONDESC Desc;
				Desc.m_CloneType = CMahabalasura_Weapon::CLONE_SKILL;
				PlayerPos.y -= 5.f;
				Desc.Pos = PlayerPos;
				FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TEXT("Layer_Spear"), TAG_OP(Prototype_Object_Boss_MahabalasuraWeapon), &Desc));
				//g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TEXT("Layer_Spear"));

				Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_BOSS_Mahabalasura_SKILLSPEAR_0, m_pTransformCom);
				Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_BOSS_Mahabalasura_SKILLSPEAR_1, m_pTextureParticleTransform2);
				Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_BOSS_Mahabalasura_SKILLSPEAR_2, m_pTransformCom);

				++m_iAdjMovedIndex;
			}

			if (PlayRate > 0.8563218390 && m_iAdjMovedIndex == 9)
			{
				//g_pGameInstance->Play3D_Sound(L"JJB_MrM_Secondary_Attack.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);

				++m_iAdjMovedIndex;
			}
			if (PlayRate > 0.977011 && m_iAdjMovedIndex == 10)
			{
				g_pGameInstance->Play3D_Sound(L"JJB_MrM_Secondary_Attack.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);

				CTransform* PlayerTransform = (CTransform*)m_pPlayerObj->Get_Component(TAG_COM(Com_Transform));
				_float3 PlayerPos = PlayerTransform->Get_MatrixState(CTransform::STATE_POS);

				CMahabalasura_Weapon::WEAPONDESC Desc;
				Desc.m_CloneType = CMahabalasura_Weapon::CLONE_SKILL;
				PlayerPos.y -= 5.f;
				Desc.Pos = PlayerPos;
				FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TEXT("Layer_Spear"), TAG_OP(Prototype_Object_Boss_MahabalasuraWeapon), &Desc));
				//g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TEXT("Layer_Spear"));

				Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_BOSS_Mahabalasura_SKILLSPEAR_0, m_pTransformCom);
				Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_BOSS_Mahabalasura_SKILLSPEAR_1, m_pTextureParticleTransform2);
				Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_BOSS_Mahabalasura_SKILLSPEAR_2, m_pTransformCom);
				++m_iAdjMovedIndex;
			}
			if (PlayRate > 0.977011 && m_iAdjMovedIndex == 11)
			{

				++m_iAdjMovedIndex;
			}


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
			m_fAttackCoolTime = 2.f;
			m_fSkillCoolTime = 5.f;
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

	for (auto& Arm : m_pArms)
	{
		Safe_Release(Arm);
	}
	m_pArms.clear();

	Safe_Release(m_pHPUI);
}
