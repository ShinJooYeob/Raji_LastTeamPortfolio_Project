#include "stdafx.h"
#include "..\public\Chiedtian.h"
#include "Chiedtuan_Weapon.h"

#include "HpUI.h"

CChiedtian::CChiedtian(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CBoss(pDevice, pDeviceContext)
{
}

CChiedtian::CChiedtian(const CChiedtian & rhs)
	: CBoss(rhs)
{
}

HRESULT CChiedtian::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));


	return S_OK;
}

HRESULT CChiedtian::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components());

	if (pArg != nullptr)
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, *((_float3*)pArg));

	m_pTransformCom->Rotation_CW(XMVectorSet(0, 1, 0, 0), XMConvertToRadians(170));

	m_pTransformCom->Scaled_All(_float3(1.5f, 1.5f, 1.5f));

	m_pModel->Change_AnimIndex(1);

	m_fAttackCoolTime = 2.f;
	m_fSkillCoolTime = 5.f;
	m_bIsHit = false;

	m_pPlayerObj = (CGameObject*)g_pGameInstance->Get_GameObject_By_LayerIndex(m_eNowSceneNum, TEXT("Layer_Player"));

	CChiedtuan_Weapon::WEAPOPNDESC WeaponDesc;

	////TR
	//WeaponDesc.m_eAttachedDesc.Initialize_AttachedDesc(this, "middle_metacarpal_r", XMVectorSet(10.f,10.f,10.f, 0.f), XMVectorSet(170.f, 0.f, 0.f, 0.f), XMVectorSet(-3.00f *10.f, -0.250f*10.f, -4.320f*10.f, 1.f));
	//WeaponDesc.m_KatanaPOS = CChiedtuan_Weapon::KATANA_TR;
	//g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TEXT("Weapon"), TAG_OP(Prototype_Object_Boss_ChiedtianWeapon), &WeaponDesc);

	////RL
	//WeaponDesc.m_eAttachedDesc.Initialize_AttachedDesc(this, "middle_metacarpal_l", XMVectorSet(10.f, 10.f, 10.f, 0.f), XMVectorSet(170.f, -90.f, 0.f, 0.f), XMVectorSet(3.120f * 10.f, -0.670f * 10.f, -4.240f * 10.f, 1.f));
	//WeaponDesc.m_KatanaPOS = CChiedtuan_Weapon::KATANA_TL;
	//g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TEXT("Weapon"), TAG_OP(Prototype_Object_Boss_ChiedtianWeapon), &WeaponDesc);

	////BR
	////WeaponDesc.m_eAttachedDesc.Initialize_AttachedDesc(this, "thigh_twist_01_r", XMVectorSet(8.f, 8.f, 8.f, 0.f), XMVectorSet(0.f, -90.f, 0.f, 0.f), XMVectorSet(-1.250f * 8.f, -2.140f * 8.f, -3.180f * 8.f, 1.f));
	//WeaponDesc.m_eAttachedDesc.Initialize_AttachedDesc(this, "thigh_twist_01_r", XMVectorSet(8.f, 8.f, 8.f, 0.f), XMVectorSet(-55.f, -80.f, 50.f, 0.f), XMVectorSet(-1.050f * 8.f,-1.430f* 8.f,-3.230f* 8.f,0));
	//WeaponDesc.m_KatanaPOS = CChiedtuan_Weapon::KATANA_BR;
	//g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TEXT("Weapon"), TAG_OP(Prototype_Object_Boss_ChiedtianWeapon), &WeaponDesc);
	//CChiedtuan_Weapon* Weapon = (CChiedtuan_Weapon*)g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TEXT("Weapon"));
	//Weapon->Set_WaistgirdScal();

	////BL
	//WeaponDesc.m_eAttachedDesc.Initialize_AttachedDesc(this, "thigh_twist_01_l", XMVectorSet(8.f, 8.f, 8.f, 0.f), XMVectorSet(-55.f, -75.f, 50.f, 0.f), XMVectorSet(1.050f * 8.f, -1.430f* 8.f, -3.230f* 8.f, 0));
	//WeaponDesc.m_KatanaPOS = CChiedtuan_Weapon::KATANA_BL;
	//g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TEXT("Weapon"), TAG_OP(Prototype_Object_Boss_ChiedtianWeapon), &WeaponDesc);
	//Weapon = (CChiedtuan_Weapon*)g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TEXT("Weapon"));
	//Weapon->Set_WaistgirdScal();


	CChiedtuan_Weapon* ChiedtuanWeapon = nullptr;

	//주무기
	WeaponDesc.BossObj = this;
	WeaponDesc.m_eAttachedDesc.Initialize_AttachedDesc(this, "middle_metacarpal_r", XMVectorSet(10.f, 10.f, 10.f, 0.f), XMVectorSet(170.f, 0.f, 0.f, 0.f), XMVectorSet(-3.00f *10.f, -0.250f*10.f, -4.320f*10.f, 1.f));
	WeaponDesc.m_KatanaPOS = CChiedtuan_Weapon::KATANA_TR;
	g_pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&ChiedtuanWeapon), m_eNowSceneNum, TAG_OP(Prototype_Object_Boss_ChiedtianWeapon), &WeaponDesc);
	m_pMainWeapons.push_back(ChiedtuanWeapon);

	WeaponDesc.BossObj = this;
	WeaponDesc.m_eAttachedDesc.Initialize_AttachedDesc(this, "middle_metacarpal_l", XMVectorSet(10.f, 10.f, 10.f, 0.f), XMVectorSet(170.f, -90.f, 0.f, 0.f), XMVectorSet(3.120f * 10.f, -0.670f * 10.f, -4.240f * 10.f, 1.f));
	WeaponDesc.m_KatanaPOS = CChiedtuan_Weapon::KATANA_TL;
	g_pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&ChiedtuanWeapon), m_eNowSceneNum, TAG_OP(Prototype_Object_Boss_ChiedtianWeapon), &WeaponDesc);
	m_pMainWeapons.push_back(ChiedtuanWeapon);

	//서브무기
	WeaponDesc.BossObj = this;
	WeaponDesc.m_eAttachedDesc.Initialize_AttachedDesc(this, "thigh_twist_01_r", XMVectorSet(8.f, 8.f, 8.f, 0.f), XMVectorSet(-55.f, -80.f, 50.f, 0.f), XMVectorSet(-1.050f * 8.f, -1.430f* 8.f, -3.230f* 8.f, 0));
	WeaponDesc.m_KatanaPOS = CChiedtuan_Weapon::KATANA_BR;
	g_pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&ChiedtuanWeapon), m_eNowSceneNum, TAG_OP(Prototype_Object_Boss_ChiedtianWeapon), &WeaponDesc);
	m_pSubWeapons.push_back(ChiedtuanWeapon);

	WeaponDesc.BossObj = this;
	WeaponDesc.m_eAttachedDesc.Initialize_AttachedDesc(this, "thigh_twist_01_l", XMVectorSet(8.f, 8.f, 8.f, 0.f), XMVectorSet(-55.f, -75.f, 50.f, 0.f), XMVectorSet(1.050f * 8.f, -1.430f* 8.f, -3.230f* 8.f, 0));
	WeaponDesc.m_KatanaPOS = CChiedtuan_Weapon::KATANA_BL;
	g_pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&ChiedtuanWeapon), m_eNowSceneNum, TAG_OP(Prototype_Object_Boss_ChiedtianWeapon), &WeaponDesc);
	m_pSubWeapons.push_back(ChiedtuanWeapon);

	CHpUI::HPDesc HpDesc;
	HpDesc.m_HPType = CHpUI::HP_MONSTER;
	HpDesc.m_pObjcect = this;
	HpDesc.m_vPos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
	HpDesc.m_Dimensions = 3.f;

	g_pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&m_pHPUI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_HpUI), &HpDesc);



	m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, _float3(0, 0, 0));
	//m_pNavigationCom->FindCellIndex(m_pTransformCom->Get_MatrixState(CTransform::STATE_POS));

	_int iRandom = rand() % 3 + 1;

	wstring teampString;
	teampString = L"JJB_Chieftain_Intro100%_" + to_wstring(iRandom) + L".wav";

	g_pGameInstance->Play3D_Sound((_tchar*)teampString.c_str(), g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);

	return S_OK;
}

_int CChiedtian::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)return -1;

	m_fAttackCoolTime -= (_float)fDeltaTime;
	m_fSkillCoolTime -= (_float)fDeltaTime;
	m_fJumpTime -= (_float)fDeltaTime;

	m_fNarration -= (_float)fDeltaTime;

	if (m_fNarration <= 0)
	{
		m_fNarration = 10.f;

		_int iRandom = rand() % 11;

		wstring teampString;
		teampString = L"JJB_Chieftain_Naration_" + to_wstring(iRandom) + L".wav";

		g_pGameInstance->Play3D_Sound((_tchar*)teampString.c_str(), g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);

	}
	
	if (m_bIsFireAttack)
		m_fFireTime -= (_float)fDeltaTime;

	if (m_fFireTime <= 0 && m_bIsFireAttack)
	{
		m_fFireTime = 10.f;
		m_bIsFireAttack = false;

		m_pModel->Change_AnimIndex_ReturnTo(14, 1);
	}

	if (m_bIsSpinAttack)
	{
		m_fSpinTime -= (_float)fDeltaTime;

		if (m_fSpinTime > 4.f)
		{
			if (m_fSpinSpeed > 10.f)
			{
				m_fSpinSpeed = 10.f;
				m_fAnimmultiple = 1.4f;
			}
			else
			{
				m_fSpinSpeed += (_float)fDeltaTime* 5.f;
				m_fAnimmultiple = (m_fSpinSpeed *0.1f)+0.4f;
			}
		}
		else if (m_fSpinTime < 4.f)
		{
			if (m_fSpinSpeed <= 0.f)
			{
				m_fSpinSpeed = 0.1f;
				m_fAnimmultiple = 0.2f;
			}
			else
			{
				m_fSpinSpeed -= (_float)fDeltaTime*3.f;
				m_fAnimmultiple = (m_fSpinSpeed *0.1f)+0.1f;
			}

		}
	}

	if (m_fSpinTime <= 0 && m_bIsSpinAttack)
	{
		m_fSpinTime = 14.f;
		m_bIsSpinAttack = false;

		m_fAttackCoolTime = 1.f;
		m_fSkillCoolTime = 5.f;


		for (_int i = 0; i < m_pMainWeapons.size(); ++i)
			m_pMainWeapons[i]->Set_IsAttackState(false);

		for (_int i = 0; i < m_pSubWeapons.size(); ++i)
			m_pSubWeapons[i]->Set_IsAttackState(false);

		CChiedtuan_Weapon::WEAPOPNDESC WeaponDesc;

		//CChiedtuan_Weapon* Weapon = (CChiedtuan_Weapon*)g_pGameInstance->Get_GameObject_By_LayerIndex(m_eNowSceneNum, TEXT("Weapon"), 2);
		CChiedtuan_Weapon* Weapon = m_pSubWeapons[0];
		WeaponDesc.m_eAttachedDesc.Initialize_AttachedDesc(this, "thigh_twist_01_r", XMVectorSet(8.f, 8.f, 8.f, 0.f), XMVectorSet(0.f, -90.f, 0.f, 0.f), XMVectorSet(-1.250f * 8.f, -2.140f * 8.f, -3.180f * 8.f, 1.f));
		WeaponDesc.m_KatanaPOS = CChiedtuan_Weapon::KATANA_BR;
		Weapon->Set_WeaponDesc(WeaponDesc);
		Weapon->Set_WaistgirdScal();

		//Weapon = (CChiedtuan_Weapon*)g_pGameInstance->Get_GameObject_By_LayerIndex(m_eNowSceneNum, TEXT("Weapon"), 3);
		Weapon = m_pSubWeapons[1];
		WeaponDesc.m_eAttachedDesc.Initialize_AttachedDesc(this, "thigh_twist_01_l", XMVectorSet(8.f, 8.f, 8.f, 0.f), XMVectorSet(-55.f, -80.f, 50.f, 0.f), XMVectorSet(1.070f * 8.f, -1.160f * 8.f, -2.590f * 8.f, 1.f));
		WeaponDesc.m_KatanaPOS = CChiedtuan_Weapon::KATANA_BL;
		Weapon->Set_WeaponDesc(WeaponDesc);
		Weapon->Set_WaistgirdScal();

		m_fAnimmultiple = 1.f;
		m_pModel->Change_AnimIndex(1, 0.8f);

	}

	CTransform* PlayerTransform = (CTransform*)m_pPlayerObj->Get_Component(TAG_COM(Com_Transform));
	_float3 PlayerPos = PlayerTransform->Get_MatrixState(CTransform::STATE_POS);
	PlayerPos.y = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS).y;

	_float3 TargetDir = XMVector3Normalize(XMLoadFloat3(&PlayerPos) - m_pTransformCom->Get_MatrixState(CTransform::STATE_POS));
	m_vAngle = XMVector3Dot(XMLoadFloat3(&TargetDir), XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK)));

	m_fRange = XMVectorGetX(XMVector3Length(XMLoadFloat3(&PlayerPos) - m_pTransformCom->Get_MatrixState(CTransform::STATE_POS)));

	if (m_bIsLookAt)
	{
		_Vector Dir = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
		Dir = XMVector3Normalize(PlayerPos.XMVector() - XMVectorSetY(Dir, PlayerPos.y));
		m_pTransformCom->Turn_Dir(Dir, 0.90f);
	}

	//if (m_bIsWalk)
	//	m_fWalkTime -= (_float)fDeltaTime;

	//if (m_bIsWalk && m_fWalkTime <= 0.f)
	//{
	//	m_bIsWalk = false;
	//	m_bIsLookAt = true;
	//	m_fWalkTime = 3.f;
	//	m_fAttackCoolTime = 1.f;
	//	m_fSkillCoolTime = 1.f;
	//	m_pModel->Change_AnimIndex(1);
	//}

	if (!m_bIsAttack && m_fRange > 8.f && !m_bIsHit)
	{
		m_bIsLookAt = true;
		m_pModel->Change_AnimIndex(2);
	}

	if (g_pGameInstance->Get_DIKeyState(DIK_N)& DIS_Down)
	{
		m_fTestHPIndex += 0.2f;
	}
	if (m_fTestHPIndex >= 1.4f)
		m_bIsHalf = true;




	if (m_fHP <= 0)
	{
		m_bIsAttack = true;
		m_pModel->Change_AnimIndex(3, 1.f);
	}


	////맞았을때
	//if (m_bIsHit)
	//{
	//	m_bIsHit = false;
	//	m_bIsAttack = true;
	//	//m_pModel->Change_AnimIndex_UntilNReturn(2, 3, 1);
	//}
	//점프
	if (!m_bIsHit && !m_bIsAttack && m_fRange < 8.f &&m_fJumpTime <= 0)
	{
		m_bIsAttack = true;
		m_pModel->Change_AnimIndex_ReturnTo(10, 1);
	}
	//일반 공격
	else if (m_fAttackCoolTime <= 0 && m_fRange > 2.f && m_fRange < 8.f &&!m_bIsAttack && !m_bIsHit)
	{
		m_bIsAttack = true;
		m_bIsBasicAttack = true;

		//if (m_fRange < 15.f)
		//{
		//	m_bIsBackJump = true;
		//	m_pModel->Change_AnimIndex_ReturnTo(10, 5);
		//}
		//else
		//{
		//	m_bIsBackJump = false;
		//	m_pModel->Change_AnimIndex_ReturnTo(5, 0);
		//}

		m_pModel->Change_AnimIndex_ReturnTo(5, 1);

	}
	//스킬 공격
	/*else if (m_fSkillCoolTime <= 0 && !m_bIsAttack && !m_bIsHit)
	{
		_int iRandom = (_int)GetSingle(CUtilityMgr)->RandomFloat(0.f, 2.9f);
		m_bIsAttack = true;
		m_bISkill = true;
		iRandom = 0;

		switch (iRandom)
		{
		case ATTACK_FIRE:
		{
			m_pModel->Change_AnimIndex_ReturnTo(12, 13);

		}
			break;
		case ATTACK_SPIN:
		{
			m_pModel->Change_AnimIndex_UntilNReturn(7, 8, 9);
		}
			break;

		case ATTACK_WHINING:
		{
			m_pModel->Change_AnimIndex_ReturnTo(6, 1);
		}
			break;

		}
		++m_iTest;

		if (m_iTest > 2)
			m_iTest = 0;
	}*/


	m_bIsOnScreen = g_pGameInstance->IsNeedToRender(m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS), m_fFrustumRadius);
	FAILED_CHECK(m_pModel->Update_AnimationClip(fDeltaTime * (m_fAnimmultiple* m_fTestHPIndex), m_bIsOnScreen));
	FAILED_CHECK(Adjust_AnimMovedTransform(fDeltaTime));

	m_pCollider->Update_ConflictPassedTime(fDeltaTime);

	_uint iNumCollider = m_pCollider->Get_NumColliderBuffer();

	for (_uint i = 0; i < iNumCollider; i++)
		m_pCollider->Update_Transform(i, m_vecAttachedDesc[i].Caculate_AttachedBoneMatrix_BlenderFixed());

	//FireSkill_Collider
	m_pFireCollider->Update_ConflictPassedTime(fDeltaTime);
	m_pFireCollider->Update_Transform(0, m_FireAttachedDesc.Caculate_AttachedBoneMatrix_BlenderFixed());

	//Jump_Collider
	m_pJumpCollider->Update_ConflictPassedTime(fDeltaTime);
	m_pJumpCollider->Update_Transform(0, m_JumpAttachedDesc.Caculate_AttachedBoneMatrix_BlenderFixed());

	if (m_bIsFireAttack)
	{
		FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_MonsterWeapon, this, m_pFireCollider));
	}

	if (m_bIsJump)
	{
		FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_MonsterWeapon, this, m_pJumpCollider));
	}

	FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_Monster, this, m_pCollider));
	//FAILED_CHECK(g_pGameInstance->Add_RepelGroup(m_pTransformCom, 1.5f, m_pNavigationCom));

	if (!m_bIsMainWeaponOff)
	{
		for (auto& Weapon : m_pMainWeapons)
			Weapon->Update(fDeltaTime);
	}

	for (auto& SubWeapon : m_pSubWeapons)
		SubWeapon->Update(fDeltaTime);

	if (m_pHPUI != nullptr)
		m_pHPUI->Update(fDeltaTime);

	return _int();
}

_int CChiedtian::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)return -1;


	FAILED_CHECK(m_pRendererCom->Add_ShadowGroup(CRenderer::SHADOW_ANIMMODEL, this, m_pTransformCom, m_pShaderCom, m_pModel));
	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
	//FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pCollider));
	FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pFireCollider));
	FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pJumpCollider));
	m_vOldPos = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS);
	//g_pGameInstance->Set_TargetPostion(PLV_PLAYER, m_vOldPos);

	if (!m_bIsMainWeaponOff) 
	{
		for (auto& Weapon : m_pMainWeapons)
			Weapon->LateUpdate(fDeltaTime);

		for (auto& SubWeapon : m_pSubWeapons)
			SubWeapon->LateUpdate(fDeltaTime);
	}

	//m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, m_pNavigationCom->Get_NaviPosition(m_pTransformCom->Get_MatrixState(CTransform::STATE_POS)));
	
	if (m_pHPUI != nullptr)
		m_pHPUI->LateUpdate(fDeltaTime);

	return _int();
}

_int CChiedtian::Render()
{
	if (__super::Render() < 0)		return -1;

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
#ifdef _DEBUG
	//m_pNavigationCom->Render(m_pTransformCom);
#endif // _DEBUG
	return _int();
}

_int CChiedtian::LateRender()
{
	if (__super::LateRender() < 0)		return -1;

	return _int();
}

void CChiedtian::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
	if (CollisionTypeID::CollisionType_Player == eConflictedObjCollisionType)
	{
		if (m_pFireCollider == pMyCollider)
		{
			_Vector vDamageDir = XMVector3Normalize(pConflictedCollider->Get_ColliderPosition(iConflictedObjColliderIndex).XMVector() - m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS));
			pConflictedObj->Take_Damage(this, 1.f, vDamageDir, m_bOnKnockbackCol, m_fKnockbackColPower);
			pConflictedCollider->Set_Conflicted(1.f);
		}
		else if (m_pJumpCollider == pMyCollider)
		{
			_Vector vDamageDir = XMVector3Normalize(pConflictedCollider->Get_ColliderPosition(iConflictedObjColliderIndex).XMVector() - m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS));
			pConflictedObj->Take_Damage(this, 1.f, vDamageDir, m_bOnKnockbackCol, m_fKnockbackColPower);
			pConflictedCollider->Set_Conflicted(1.f);
		}
	}
}

_float CChiedtian::Take_Damage(CGameObject * pTargetObject, _float fDamageAmount, _fVector vDamageDir, _bool bKnockback, _float fKnockbackPower)
{
	m_fHP -= fDamageAmount;

	return _float();
}

HRESULT CChiedtian::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_Boss_Chieftian), TAG_COM(Com_Model), (CComponent**)&m_pModel));
	FAILED_CHECK(m_pModel->Change_AnimIndex(1));


	CTransform::TRANSFORMDESC tDesc = {};

	tDesc.fMovePerSec = 5;
	tDesc.fRotationPerSec = XMConvertToRadians(60);
	tDesc.fScalingPerSec = 1;
	tDesc.vPivot = _float3(0, 0, 0);

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));

	/* For.Com_Navigation */
	CNavigation::NAVIDESC		NaviDesc;
	NaviDesc.iCurrentIndex = 0;

	FAILED_CHECK(__super::Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Navigation), TAG_COM(Com_Navaigation), (CComponent**)&m_pNavigationCom, &NaviDesc));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_Collider), (CComponent**)&m_pCollider));

	COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(9.f, 9.f, 9.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	ATTACHEDESC tAttachedDesc;
	tAttachedDesc.Initialize_AttachedDesc(this, "spine_02", _float3(1), _float3(0), _float3(0.f, -0.04408f, -3.10981f));
	m_vecAttachedDesc.push_back(tAttachedDesc);

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(6.f, 6.f, 6.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 1.4f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "head", _float3(1), _float3(0), _float3(0.f, 0.012524f, -4.42935f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pCollider->Set_ParantBuffer();

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(6.f, 6.f, 6.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 2.5f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "pelvis", _float3(1), _float3(0), _float3(0.f, 0.027885f, -2.54343f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pCollider->Set_ParantBuffer();



	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(2.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, -0.1f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "head", _float3(1), _float3(0), _float3(0.f, 0.012524f, -4.42935f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pCollider->Set_ParantBuffer(1);

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(3.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, -0.1f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "spine_01", _float3(1), _float3(0), _float3(0.f, -0.033538f, -2.83053f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pCollider->Set_ParantBuffer(1);

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.8f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "calf_r", _float3(1), _float3(0), _float3(-0.443741f, 0.047085f, -1.39788f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pCollider->Set_ParantBuffer(2);

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.8f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "calf_l", _float3(1), _float3(0), _float3(0.44374f, 0.047085f, -1.39788f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pCollider->Set_ParantBuffer(2);

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.8f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "foot_r", _float3(1), _float3(0), _float3(-0.612995f, 0.226194f, -0.264389f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pCollider->Set_ParantBuffer(2);

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.8f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "foot_l", _float3(1), _float3(0), _float3(0.612994f, 0.226194f, -0.264393f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pCollider->Set_ParantBuffer(2);

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.3f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "upperarm_r", _float3(1), _float3(0), _float3(-0.570974f, 0.137702f, -3.78514f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pCollider->Set_ParantBuffer(1);

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.3f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "upperarm_l", _float3(1), _float3(0), _float3(0.570976f, 0.137702f, -3.78514f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pCollider->Set_ParantBuffer(1);

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.3f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "lowerarm_r", _float3(1), _float3(0), _float3(-1.16276f, 0.060726f, -3.44879f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pCollider->Set_ParantBuffer(1);

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.3f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "lowerarm_l", _float3(1), _float3(0), _float3(1.16276f, 0.060726f, -3.44879f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pCollider->Set_ParantBuffer(1);

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.3f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "lowerarm_twist_01_r", _float3(1), _float3(0), _float3(-1.75266f, -0.14289f, -3.13727f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pCollider->Set_ParantBuffer(1);

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.3f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "lowerarm_twist_01_l", _float3(1), _float3(0), _float3(1.75266f, -0.14289f, -3.13727f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pCollider->Set_ParantBuffer(1);


	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_ColliderSub), (CComponent**)&m_pFireCollider));

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(5.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, -4.f, 0.f, 1);
	FAILED_CHECK(m_pFireCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "root", _float3(1), _float3(0), _float3(0, 0, 0));
	m_FireAttachedDesc = tAttachedDesc;

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_ColliderSubSub), (CComponent**)&m_pJumpCollider));

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(4.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pJumpCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "root", _float3(1), _float3(0), _float3(0, 0, 0));
	m_JumpAttachedDesc = tAttachedDesc;



	return S_OK;
}

HRESULT CChiedtian::Adjust_AnimMovedTransform(_double fDeltatime)
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
			//Idle
			if (PlayRate > 0.f)
			{
				m_bIsAttack = false;
				m_bISkill = false;
				//m_bIsLookAt = true;
				//m_bIsBackJump = false;

			}
			break;
		case 1://애니메이션 인덱스마다 잡아주면 됨
			if (PlayRate > 0.f)
			{
				m_bIsAttack = false;
				m_bISkill = false;
				//m_bIsLookAt = true;
				//m_bIsBackJump = false;

			}
			break;
		case 2:
		{
			m_pTransformCom->Move_Forward(fDeltatime);
			
			if (m_iAdjMovedIndex == 0 && PlayRate > 0.1612903225)
			{
				g_pGameInstance->Play3D_Sound(L"JJB_Chieftain_Footstep_01.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);
				++m_iAdjMovedIndex;
			}

			if (m_iAdjMovedIndex == 1 && PlayRate > 0.5483870967)
			{
				g_pGameInstance->Play3D_Sound(L"JJB_Chieftain_Footstep_02.wav", g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_MONSTER, 0.7f);
				++m_iAdjMovedIndex;
			}
		}
			break;

		case 3:
			break;

		case 4:
		{

		}

		break;

		case 5:
		{
			if (m_iAdjMovedIndex == 0 && PlayRate < 0.2926829268)
			{
				m_bIsLookAt = false;
				m_iAdjMovedIndex++;
			}
			else if (m_iAdjMovedIndex == 1 && PlayRate > 0.529411764)
			{
				m_pMainWeapons[0]->Set_IsAttackState(false);
				m_iAdjMovedIndex++;
			}

			if (PlayRate > 0.215686274 && PlayRate < 0.529411764)
			{
				m_pMainWeapons[0]->Set_IsAttackState(true);
			}


			//if (PlayRate < 0.1372549 && PlayRate > 0.3725490)
			//{
			//	//무기 회전 시킬려고 한거 같은데???
			//	//CChiedtuan_Weapon* Weapon = (CChiedtuan_Weapon*)g_pGameInstance->Get_GameObject_By_LayerIndex(m_eNowSceneNum, TEXT("Weapon"), 0);
			//}
		}

		break;

		case 6:
		{
			if (m_iAdjMovedIndex == 0 && PlayRate > 0.0526315)
			{
				for (_int i = 0; i < m_pMainWeapons.size(); ++i)
				{
					m_pMainWeapons[i]->Set_IsAttackState(true);
				}
				++m_iAdjMovedIndex;
			}

			if (PlayRate > 0.22666666 && PlayRate < 0.53333333)
			{
				m_pTransformCom->Move_Forward(fDeltatime/*, m_pNavigationCom*/);
			}
			else if (PlayRate > 0.8 && PlayRate < 0.92)
			{
				m_pTransformCom->Move_Forward(fDeltatime/*, m_pNavigationCom*/);
			}
		}
			break;

		case 7://SpinAttackStartAnim
		{
			m_bIsLookAt = false;

			if (m_iAdjMovedIndex == 0 && PlayRate > 0.0526315)
			{
				CChiedtuan_Weapon::WEAPOPNDESC WeaponDesc;

				//CChiedtuan_Weapon* Weapon = (CChiedtuan_Weapon*)g_pGameInstance->Get_GameObject_By_LayerIndex(m_eNowSceneNum, TEXT("Weapon"), 2);
				CChiedtuan_Weapon* Weapon = m_pSubWeapons[0];
				WeaponDesc.m_eAttachedDesc.Initialize_AttachedDesc(this, "middle_02_r_02", XMVectorSet(9.f, 9.f, 9.f, 0.f), XMVectorSet(90.f, 65.f, 90.f, 0.f), XMVectorSet(-2.580f * 9.f, -1.870f * 9.f, -3.470f * 9.f, 1.f));
				WeaponDesc.m_KatanaPOS = CChiedtuan_Weapon::KATANA_BR;
				Weapon->Set_WeaponDesc(WeaponDesc);
				Weapon->Set_SpinScal();

				//Weapon = (CChiedtuan_Weapon*)g_pGameInstance->Get_GameObject_By_LayerIndex(m_eNowSceneNum, TEXT("Weapon"), 3);
				Weapon = m_pSubWeapons[1];
				WeaponDesc.m_eAttachedDesc.Initialize_AttachedDesc(this, "middle_02_l_02", XMVectorSet(10.f, 10.f, 10.f, 0.f), XMVectorSet(50.f, 90.f, 0.f, 0.f), XMVectorSet(2.230f * 10.f, -1.990f* 10.f, -3.500f* 10.f, 1.f));
				WeaponDesc.m_KatanaPOS = CChiedtuan_Weapon::KATANA_BL;
				Weapon->Set_WeaponDesc(WeaponDesc);
				Weapon->Set_SpinScal();


				++m_iAdjMovedIndex;
			}

		}

		break;

		case 9://SpinAttackAnim
		{
			_float3 MonsterPos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);

			CTransform* PlayerTransform = (CTransform*)m_pPlayerObj->Get_Component(TAG_COM(Com_Transform));
			_float3 PlayerPos = PlayerTransform->Get_MatrixState(CTransform::STATE_POS);

			PlayerPos.y = MonsterPos.y;

			_float3 vGoalDir = (PlayerPos.XMVector() - MonsterPos.XMVector());
			m_pTransformCom->Turn_CW(XMVectorSet(0.f,1.f,0.f,0.f), fDeltatime * m_fSpinSpeed +0.01f);

			if(m_fSpinSpeed > 0.1f)
				m_pTransformCom->MovetoDir(XMLoadFloat3(&vGoalDir), fDeltatime * 0.3/*, m_pNavigationCom*/);

			m_fAnimmultiple = (m_fSpinSpeed *0.1f);
		}

		break;

		case 10://Jump Animation
		{
			if (m_iAdjMovedIndex == 0 && PlayRate < 0.2926829268)
			{
				m_bIsLookAt = false;
				m_iAdjMovedIndex++;
			}
			/*if (m_bIsBackJump)
			{
				if (PlayRate > 0.317073170 && PlayRate < 0.63414634146)
				{
					m_bIsLookAt = true;

					_float3 MonsterPos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);

					_float3 GoalPos = XMLoadFloat3(&MonsterPos) + (XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK)) * -2.f);

					_float3 vGoalDir = (GoalPos.XMVector() - MonsterPos.XMVector());

					_float fLength = g_pGameInstance->Easing(TYPE_CircularIn, 0.f, vGoalDir.Get_Lenth(), (_float)PlayRate - 0.317073170f, 0.31707317146f);

					m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, (MonsterPos.XMVector() + vGoalDir.Get_Nomalize() * fLength));
				}
			}
			else
			{
				if (PlayRate > 0.317073170 && PlayRate < 0.63414634146)
				{
					m_bIsLookAt = true;

					_float3 MonsterPos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);

					CTransform* PlayerTransform = (CTransform*)m_pPlayerObj->Get_Component(TAG_COM(Com_Transform));
					_float3 PlayerPos = PlayerTransform->Get_MatrixState(CTransform::STATE_POS);

					PlayerPos.y = MonsterPos.y;

					_float3 vGoalDir = (PlayerPos.XMVector() - MonsterPos.XMVector());

					_float fLength = g_pGameInstance->Easing(TYPE_CircularIn, 0.f, vGoalDir.Get_Lenth(), (_float)PlayRate - 0.317073170f, 0.31707317146f);

					m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, (MonsterPos.XMVector() + vGoalDir.Get_Nomalize() * fLength));
				}
			}*/
			if (PlayRate > 0.317073170 && PlayRate < 0.63414634146)
			{
				m_bIsLookAt = true;

				_float3 MonsterPos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);

				CTransform* PlayerTransform = (CTransform*)m_pPlayerObj->Get_Component(TAG_COM(Com_Transform));
				_float3 PlayerPos = PlayerTransform->Get_MatrixState(CTransform::STATE_POS);

				PlayerPos.y = MonsterPos.y;

				_float3 vGoalDir = (PlayerPos.XMVector() - MonsterPos.XMVector());

				_float fLength = g_pGameInstance->Easing(TYPE_CircularIn, 0.f, vGoalDir.Get_Lenth(), (_float)PlayRate - 0.317073170f, 0.31707317146f);

				m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, (MonsterPos.XMVector() + vGoalDir.Get_Nomalize() * fLength));
			}
			if (m_iAdjMovedIndex == 1 && PlayRate > 0.63414634146)
			{
				m_bIsLookAt = false;
				m_iAdjMovedIndex++;
			}

			if (m_iAdjMovedIndex == 2 && PlayRate > 0.682926)
			{
				m_bIsJump = true;
				m_iAdjMovedIndex++;
			}
			if (m_iAdjMovedIndex == 3 && PlayRate > 0.71)
			{
				m_bIsJump = false;
				m_iAdjMovedIndex++;
			}
		}
		break;

		case 12: //FireAttackStartAnim
		{
			m_bIsLookAt = false;
			m_fAnimmultiple = 1.3f;

			//무기없애기
			if (PlayRate > 0.0411392 && m_iAdjMovedIndex == 0)
			{
				m_bIsMainWeaponOff = true;
				++m_iAdjMovedIndex;
			}
		}
		break;

		case 13: //FireAttack
		{
			m_bIsLookAt = true;
			m_pTransformCom->Move_Forward(fDeltatime * 0.7f/*, m_pNavigationCom*/);

		}
		break;

		case 14: //FireAttackEnd
		{
			m_bIsLookAt = false;
			m_fAnimmultiple = 1.3f;

			
			//무기생성
			if (PlayRate > 0.8227848 && m_iAdjMovedIndex == 0)
			{
				m_bIsMainWeaponOff = false;
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
			if (m_fRange > 12.f && !m_bIsAttack && !m_bIsWalk)
			{
				m_bIsWalk = true;
				m_bIsAttack = true;
				m_bIsLookAt = false;

				_Vector LookDir = XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK))
					+ XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::STATE_RIGHT) * GetSingle(CUtilityMgr)->RandomFloat(-0.7f, 0.7f));

				m_pTransformCom->LookDir(LookDir);
				m_pModel->Change_AnimIndex(2);
			}
		}
		if (iNowAnimIndex == 1)
		{
			if (m_fRange > 12.f && !m_bIsAttack && !m_bIsWalk)
			{
				m_bIsWalk = true;
				m_bIsAttack = true;
				m_bIsLookAt = false;

				_Vector LookDir = XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK))
					+ XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::STATE_RIGHT) * GetSingle(CUtilityMgr)->RandomFloat(-0.7f, 0.7f));

				m_pTransformCom->LookDir(LookDir);
				m_pModel->Change_AnimIndex(2);
			}
		}
		if (iNowAnimIndex == 3)
		{
			Set_IsDead();
		}
		if (iNowAnimIndex == 5)
		{
			//m_bIsLookAt = true;
			m_bIsBasicAttack = false;
			m_fAttackCoolTime = 3.f;
			m_fSkillCoolTime = 1.f;

			m_pModel->Change_AnimIndex(1);
		}
		if (iNowAnimIndex == 6)
		{
			for (_int i = 0; i < m_pMainWeapons.size(); ++i)
			{
				m_pMainWeapons[i]->Set_IsAttackState(false);
			}
			//m_bIsLookAt = true;
			m_fAttackCoolTime = 1.f;
			m_fSkillCoolTime = 6.f;
			m_fJumpTime = 0.5f;

		}
		if (iNowAnimIndex == 8)
		{
			m_bIsSpinAttack = true;

			for (_int i = 0; i < m_pMainWeapons.size(); ++i)
				m_pMainWeapons[i]->Set_IsAttackState(true);

			for (_int i = 0; i < m_pSubWeapons.size(); ++i)
				m_pSubWeapons[i]->Set_IsAttackState(true);

			m_fSpinTime = 14.f;
		}
		if (iNowAnimIndex == 9)
		{
			if (!m_bIsSpinAttack)
				m_bIsSpinAttack = true;
		}
		if (iNowAnimIndex == 10)
		{

			//m_bIsLookAt = true;
			m_fJumpTime = 3.f;

		}
		if (iNowAnimIndex == 12)
		{
			//m_bIsLookAt = true;
			m_bIsFireAttack = true;
			m_fFireTime = 10.f;
			m_fAnimmultiple = 1.f;
		}
		if (iNowAnimIndex == 14)
		{
			//m_bIsLookAt = true;
			m_fAttackCoolTime = 0.5f;
			m_fSkillCoolTime = 6.f;
			m_fAnimmultiple = 1.f;
		}
	}


	m_iOldAnimIndex = iNowAnimIndex;
	return S_OK;
}

CChiedtian * CChiedtian::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CChiedtian*	pInstance = NEW CChiedtian(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CChiedtian");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CChiedtian::Clone(void * pArg)
{
	CChiedtian*	pInstance = NEW CChiedtian(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created  Clone CChiedtian");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CChiedtian::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModel);
	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pCollider);
	Safe_Release(m_pFireCollider);
	Safe_Release(m_pJumpCollider);

	for (auto& Weapon : m_pMainWeapons)
		Safe_Release(Weapon);

	for (auto& SubWeapon : m_pSubWeapons)
		Safe_Release(SubWeapon);

	m_pMainWeapons.clear();
	m_pSubWeapons.clear();

	Safe_Release(m_pHPUI);
}
