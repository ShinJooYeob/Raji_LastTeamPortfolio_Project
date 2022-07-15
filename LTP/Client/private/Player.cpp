#include "stdafx.h"
#include "..\Public\Player.h"
#include "Timer.h"
#include "Camera_Main.h"
#include "ShellingArrow.h"
#include "PlayerWeapon_Bow.h"
#include "PlayerWeapon_Spear.h"
#include "PlayerWeapon_Arrow.h"
#include "ShellingSkillRange.h"
#include "SpearWave.h"
#include "PlayerWeapon_Shield.h"
#include "PlayerWeapon_Chakra.h"
#include "physx\Collider_PhysX_Joint.h"

#include "TestLedgeTrigger.h"

#include "HpUI.h"

CPlayer::CPlayer(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CPlayer::CPlayer(const CPlayer & rhs)
	: CGameObject(rhs)
{
}

HRESULT CPlayer::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CPlayer::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components());

	if (pArg != nullptr)
	{
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, *((_float3*)pArg));
	}

	m_pTransformCom->Rotation_CW(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(170.f));

	if (m_eNowSceneNum == 7)
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, _float3(1.f, 0.f, 0.f));

	FAILED_CHECK(SetUp_EtcInfo());

	FAILED_CHECK(SetUp_PlayerWeapons());

	FAILED_CHECK(SetUp_PlayerEffects()); 

	FAILED_CHECK(Ready_ParticleDesc());

	Set_IsOcllusion(true);

	//CHpUI::HPDesc HpDesc;
	//HpDesc.m_HPType = CHpUI::HP_RAJI;
	//HpDesc.m_pObjcect = this;
	//HpDesc.m_vPos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);

	//g_pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)m_pHPUI, m_eNowSceneNum, TAG_OP(Prototype_Object_Boss_ChiedtianWeapon), &HpDesc);

	//m_szNameTag = L"Raji";
	//"Layer_Player";
	return S_OK;
}

_int CPlayer::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0) return -1;

	// Test
	if (g_pGameInstance->Get_DIKeyState(DIK_I) & DIS_Down)
	{
		// JUMP
		//m_eCurState = STATE_JUMP;
		//m_pModel->Change_AnimIndex(BASE_ANIM_JUMP);

		// CURTAIN
		//Set_State_CurtainStart(fDeltaTime);

		// WALL RUN
		/*static _bool bDebugBoolean = false;
		Set_State_WallRunStart(bDebugBoolean, fDeltaTime);
		bDebugBoolean = !bDebugBoolean;*/

		// CURTAIN
		//Set_State_CurtainStart(fDeltaTime);

		// PILLAR
		//Set_State_PillarStart(fDeltaTime);
		Take_Damage(this, 10.f, XMVectorSet(1.f, 0.f, 0.f, 0.f), true, 10.f);
	}
	//if (g_pGameInstance->Get_DIKeyState(DIK_U) & DIS_Down)
	//{
	//	m_pModel->Change_AnimIndex(PILLAR_ANIM_TOP_CLIMB);
	//	//Set_State_PillarStart(fDeltaTime);
	//}
	
	if (g_pGameInstance->Get_DIKeyState(DIK_Z) & DIS_Down)
	{
		m_pDissolveCom->Set_DissolveOn(false, 5.5f);
	}
	if (g_pGameInstance->Get_DIKeyState(DIK_X) & DIS_Down)
	{
		m_pDissolveCom->Set_DissolveOn(true, 1.5f);
	}
	_Vector vScale = m_pTransformCom->Get_Scale();

	// Check Player Key Input
	Check_PlayerKeyInput(fDeltaTime);


	// Check MotionTrail
	m_fInterval_MotionTrail -= (_float)fDeltaTime;
	if (m_fInterval_MotionTrail < 0)
	{
		m_fInterval_MotionTrail = 0.1f;
		CheckOn_MotionTrail();
	}


	// Reset AnimSpeed
	m_fAnimSpeed = 1.f;


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////Process Player State Logic
		switch (m_eCurState)
		{
		case EPLAYER_STATE::STATE_IDLE:
			FAILED_CHECK(Update_State_Idle(fDeltaTime));
			break;
		case EPLAYER_STATE::STATE_MOV:
			FAILED_CHECK(Update_State_Move(fDeltaTime));
			break;
		case EPLAYER_STATE::STATE_EVASION:
			FAILED_CHECK(Update_State_Evasion(fDeltaTime));
			break;
		case EPLAYER_STATE::STATE_ATTACK:
			FAILED_CHECK(Update_State_Attack(fDeltaTime));
			break;
		case EPLAYER_STATE::STATE_JUMP:
			FAILED_CHECK(Update_State_Jump(fDeltaTime));
			break;
		case EPLAYER_STATE::STATE_UTILITYSKILL:
			FAILED_CHECK(Update_State_UtilitySkill(fDeltaTime));
			break;
		case EPLAYER_STATE::STATE_ULTIMATESKILL:
			FAILED_CHECK(Update_State_UltimateSkill(fDeltaTime));
			break;
		case EPLAYER_STATE::STATE_TAKE_DAMAGE:
			FAILED_CHECK(Update_State_Damage(fDeltaTime));
			break;
		case EPLAYER_STATE::STATE_DEAD:
			FAILED_CHECK(Update_State_Dead(fDeltaTime));
			break;
		case EPLAYER_STATE::STATE_EXECUTION:
			FAILED_CHECK(Update_State_Execution(fDeltaTime));
			break;
		case EPLAYER_STATE::STATE_PARKOUR:
			FAILED_CHECK(Update_State_Parkour(fDeltaTime));
			break;
		case EPLAYER_STATE::STATE_CURTAIN:
			FAILED_CHECK(Update_State_Curtain(fDeltaTime));
			break;
		case EPLAYER_STATE::STATE_WALLRUN:
			FAILED_CHECK(Update_State_WallRun(fDeltaTime));
			break;
		case EPLAYER_STATE::STATE_PILLAR:
			FAILED_CHECK(Update_State_Pillar(fDeltaTime));
			break;
		case EPLAYER_STATE::STATE_PETAL:
			FAILED_CHECK(Update_State_Petal(fDeltaTime));
			break;
		default:
			MSGBOX("CPlayer::Update : Unknown Player Cur_State Value");
			break;
		}
	
	// Update Player Anim
	if(true == m_bUpdateAnimation)
	{
		FAILED_CHECK(m_pModel->Update_AnimationClip(fDeltaTime * m_fAnimSpeed, m_bIsOnScreen));
	}

	// Anim Control
	Adjust_AnimMovedTransform(fDeltaTime);

	// Motion Trail
	m_pMotionTrail->Update_MotionTrail(fDeltaTime);

	// Update Attach CamPos
	Update_AttachCamPos();

	// Update PhysX
	if (m_pHeadJoint)
		m_pHeadJoint->Update_BeforeSimulation();

	// CameraShake Test
	if (g_pGameInstance->Get_DIKeyState(DIK_P) & DIS_Down)
	{
		m_pMainCamera->Start_CameraShaking_Thread(1.f, 10.f, 0.018f);
	}
	if (g_pGameInstance->Get_DIKeyState(DIK_O) & DIS_Down)
	{
		m_pMainCamera->Start_CameraShaking_Fov(57.f, 2.f, 0.2f);
	}
	if (g_pGameInstance->Get_DIKeyState(DIK_K) & DIS_Down)
	{
		CCamera_Main::CAMERASHAKEDIRDESC tCameraShakeDirDesc;
		tCameraShakeDirDesc.fTotalTime = 1.f;
		tCameraShakeDirDesc.fPower = 10.f;
		tCameraShakeDirDesc.fChangeDirectioninterval = 0.018f;
		tCameraShakeDirDesc.fShakingDir = XMVector3Normalize(m_pMainCamera->Get_Camera_Transform()->Get_MatrixState(CTransform::TransformState::STATE_UP));
		m_pMainCamera->Start_CameraShaking_Dir_Thread(&tCameraShakeDirDesc);
	}
	if (g_pGameInstance->Get_DIKeyState(DIK_L) & DIS_Down)
	{
		CCamera_Main::CAMERASHAKEDIRDESC tCameraShakeDirDesc;
		tCameraShakeDirDesc.fTotalTime = 1.f;
		tCameraShakeDirDesc.fPower = 10.f;
		tCameraShakeDirDesc.fChangeDirectioninterval = 0.018f;
		tCameraShakeDirDesc.fShakingDir = XMVector3Normalize(m_pMainCamera->Get_Camera_Transform()->Get_MatrixState(CTransform::TransformState::STATE_RIGHT));
		m_pMainCamera->Start_CameraShaking_Dir_Thread(&tCameraShakeDirDesc);
	}
	if (g_pGameInstance->Get_DIKeyState(DIK_J) & DIS_Down)
	{
		CCamera_Main::CAMERASHAKEROTDESC tCameraShakeRotDesc;
		tCameraShakeRotDesc.fTotalTime = 1.f;
		tCameraShakeRotDesc.fPower = 1.f; 
		tCameraShakeRotDesc.fChangeDirectioninterval = 0.05f;
		tCameraShakeRotDesc.fShakingRotAxis = m_pMainCamera->Get_CamTransformCom()->Get_MatrixState(CTransform::TransformState::STATE_UP);
		m_pMainCamera->Start_CameraShaking_Rot_Thread(&tCameraShakeRotDesc);
	}
	//


	// Update Targeting
	Update_Targeting(fDeltaTime);
	FAILED_CHECK(m_pDissolveCom->Update_Dissolving(fDeltaTime));


	m_pCollider->Update_ConflictPassedTime(fDeltaTime);

	//m_pCollider->Get_ColliderPosition()

	_uint iNumCollider = m_pCollider->Get_NumColliderBuffer();

	for (_uint i = 0; i < iNumCollider; i++)
		m_pCollider->Update_Transform(i, m_vecAttachedDesc[i].Caculate_AttachedBoneMatrix_BlenderFixed());
	
	FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_Player, this, m_pCollider));
	m_IsConfilicted = false;
	return _int();
}

_int CPlayer::LateUpdate(_double fDeltaTimer)
{
	if (__super::LateUpdate(fDeltaTimer) < 0)return -1;

	//if (m_IsConfilicted == true)
	//	m_pCollider->Set_Conflicted(0.5f);


	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
	FAILED_CHECK(m_pRendererCom->Add_ShadowGroup(CRenderer::SHADOW_ANIMMODEL, this, m_pTransformCom, m_pShaderCom, m_pModel, nullptr,m_pDissolveCom));
	FAILED_CHECK(m_pRendererCom->Add_TrailGroup(CRenderer::TRAIL_MOTION, m_pMotionTrail));

	FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pCollider));
	

	m_vOldPos = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS);
	g_pGameInstance->Set_TargetPostion(PLV_PLAYER, m_vOldPos);

	// Update PhysX
	if (m_pHeadJoint)
		m_pHeadJoint->Update_AfterSimulation();


	if (true == m_bOnNavigation)
	{
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, m_pNavigationCom->Get_NaviPosition(m_pTransformCom->Get_MatrixState(CTransform::STATE_POS)));
	}

	return _int();
}

_int CPlayer::Render()
{
	if (__super::Render() < 0)		return -1;

	NULL_CHECK_RETURN(m_pModel, E_FAIL);

	CGameInstance* pInstance = GetSingle(CGameInstance);
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));

	FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));



	FAILED_CHECK(m_pDissolveCom->Render(3));


	//_uint NumMaterial = m_pModel->Get_NumMaterial();
	//
	//for (_uint i = 0; i < NumMaterial; i++)
	//{
	//	for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; j++)
	//		FAILED_CHECK(m_pModel->Bind_OnShader(m_pShaderCom, i, j, MODLETEXTYPE(j)));
	//
	//	FAILED_CHECK(m_pModel->Render(m_pShaderCom, 3, i, "g_BoneMatrices"));
	//}


#ifdef _DEBUG
	m_pNavigationCom->Render(m_pTransformCom);
//	if (m_pHeadJoint)
//		m_pHeadJoint->Render();
#endif // _DEBUG

	return _int();
}

_int CPlayer::LateRender()
{

	return _int();
}

void CPlayer::CollisionTriger(_uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
	//m_IsConfilicted = true;
	//if (iMyColliderIndex == 2)
	//{
	//
	//}
	//
	//if (!lstrcmp(pConflictedObj->Get_NameTag(), "Ä¡ÇÁÅ¸ÀÌÅº"))
	//{
	//	//
	//}

	//pConflictedObj->Get_NowHP() < 10

	//eConflictedObjCollisionType
	//m_pCollider->Get_ColliderPosition(iMyColliderIndex);
	//pConflictedCollider->Get_ColliderPosition(iConflictedObjColliderIndex);

	//pConflictedObj->Get_NameTag();

}

_fVector CPlayer::Get_BonePos(const char * pBoneName)
{
	_Matrix BoneMatrix = m_pModel->Get_BoneMatrix(pBoneName);
	//BoneMatrix  = XMMatrixTranspose(BoneMatrix);
	_Matrix TransformMatrix = BoneMatrix * m_pTransformCom->Get_WorldMatrix();
	_Vector vPos, vRot, vScale;
	XMMatrixDecompose(&vScale, &vRot, &vPos, TransformMatrix);

	return vPos;
}

_fMatrix CPlayer::Get_BoneMatrix(const char * pBoneName)
{
	_Matrix BoneMatrix = m_pModel->Get_BoneMatrix(pBoneName);
	_Matrix TransformMatrix = BoneMatrix * m_pTransformCom->Get_WorldMatrix();

	return TransformMatrix;
}

void CPlayer::Set_CurParkourTrigger(CTriggerObject * pParkourTrigger, CTriggerObject* pCauser)
{
	if (nullptr == pParkourTrigger && m_pCurParkourTrigger != pCauser)
		return;

	if (nullptr != m_pCurParkourTrigger)
	{
		m_pPreParkourTrigger = m_pCurParkourTrigger;
	}
	m_pCurParkourTrigger = pParkourTrigger;
}

_float CPlayer::Take_Damage(CGameObject * pTargetObject, _float fDamageAmount, _fVector vDamageDir, _bool bKnockback, _float fKnockbackPower)
{
	if (STATE_TAKE_DAMAGE == m_eCurState || STATE_DEAD == m_eCurState)
	{
		return 0.f;
	}

	if (0.f < fDamageAmount)
	{
		fDamageAmount *= -1.f;
	}

	_float fRemainHP = Add_NowHP(fDamageAmount);

	if (0.f >= fRemainHP)
	{
		if (true == bKnockback)
		{
			Set_State_DamageStart(fKnockbackPower, vDamageDir);
		}
		else
		{
			Set_State_DeathStart();
		}
	}
	else
	{
		if (true == bKnockback)
		{
			Set_State_DamageStart(fKnockbackPower, vDamageDir);
		}
	}


	return fRemainHP;
}

_float CPlayer::Apply_Damage(CGameObject * pTargetObject, _float fDamageAmount, _bool bKnockback)
{
	return _float();
}

void CPlayer::Set_State_IdleStart(_double fDeltaTime)
{
	Set_PlayerState(STATE_IDLE);
	m_bOnNavigation = true;
	switch (m_eCurWeapon)
	{
	case EWEAPON_TYPE::WEAPON_NONE:
	case EWEAPON_TYPE::WEAPON_SPEAR:
	case EWEAPON_TYPE::WEAPON_BOW:
	case EWEAPON_TYPE::WEAPON_CHAKRA:
		m_pModel->Change_AnimIndex(BASE_ANIM_IDLE, 0.15f, true);
		break;
	case EWEAPON_TYPE::WEAPON_SWORD:
		m_pModel->Change_AnimIndex(BASE_ANIM_IDLE_SWORD, 0.15f, true);
	}
}

void CPlayer::Set_State_MoveStart(_double fDeltaTime)
{
	Move(m_eInputDir, fDeltaTime);
	m_bOnNavigation = true;
	Set_PlayerState(STATE_MOV);

	switch (m_eCurWeapon)
	{
	case EWEAPON_TYPE::WEAPON_SPEAR:
		m_pModel->Change_AnimIndex(BASE_ANIM_RUN_F, 0.1f);
		break;
	case EWEAPON_TYPE::WEAPON_NONE:
	case EWEAPON_TYPE::WEAPON_BOW:
	case EWEAPON_TYPE::WEAPON_CHAKRA:
		m_pModel->Change_AnimIndex(BASE_ANIM_RUN_BOW, 0.1f);
		break;
	case EWEAPON_TYPE::WEAPON_SWORD:
		m_pModel->Change_AnimIndex(BASE_ANIM_RUN_F_SWORD, 0.1f);
	}

}

void CPlayer::Set_State_DodgeStart(_double fDeltaTime)
{
	m_bPressedDodgeKey = false;
	Set_PlayerState(STATE_EVASION);
	Set_TurnInputDir();
	m_pModel->Change_AnimIndex(BASE_ANIM_DODGE_ROLL, 0.1f, true);
}

void CPlayer::Set_State_MainAttackStart(_double fDeltaTime)
{
	if (WEAPON_CHAKRA == m_eCurWeapon || WEAPON_NONE == m_eCurWeapon)
		return;

	if (EWEAPON_TYPE::WEAPON_BOW != m_eCurWeapon)
		m_bPressedMainAttackKey = false;

	if (true == m_bPressedPowerAttackKey)
	{
		m_bPlayPowerAttack = true;
		m_bPressedPowerAttackKey = false;
	}
	Set_PlayerState(STATE_ATTACK);

	if (true == m_bAttackEnd)
	{
		Set_MainAttackAnim(m_bPlayJumpAttack);
		m_bAttackEnd = false;
	}
}

void CPlayer::Set_State_UtilitySkillStart(_double fDeltaTime)
{
	if (WEAPON_CHAKRA == m_eCurWeapon || WEAPON_NONE == m_eCurWeapon)
		return;

	Set_PlayerState(STATE_UTILITYSKILL);
	m_eCurUtilityState = UTILITY_START;

	switch (m_eCurWeapon)
	{
	case WEAPON_SPEAR:
		m_pModel->Change_AnimIndex_ReturnTo(SPEAR_ANIM_THROW_START, SPEAR_ANIM_THROW_LOOP, 0.1f, true);
		break;
	case WEAPON_BOW:
		m_pModel->Change_AnimIndex_ReturnTo(BOW_ANIM_UTILITY_START, BOW_ANIM_UTILITY_LOOP, 0.1f, true);
		break;
	case WEAPON_SWORD:
		m_pModel->Change_AnimIndex(SWORD_ANIM_SHIELD_IDLE, 0.2f, true);
		break;
	}
}

void CPlayer::Set_State_UltimateSkillStart(_double fDeltaTime)
{
	if (WEAPON_CHAKRA == m_eCurWeapon || WEAPON_NONE == m_eCurWeapon)
		return;

	Set_PlayerState(STATE_ULTIMATESKILL);

	switch (m_eCurWeapon)
	{
	case WEAPON_SPEAR:
		m_pModel->Change_AnimIndex(SPEAR_ANIM_ULTIMATE, 0.1f, true);
		break;
	case WEAPON_BOW:
		m_pModel->Change_AnimIndex(BOW_ANIM_ULTIMATE, 0.1f, true);
		break;
	case WEAPON_SWORD:
		m_pModel->Change_AnimIndex(SWORD_ANIM_ULTIMATE, 0.1f, true);
		break;
	}
}

void CPlayer::Set_State_TurnBackStart(_double fDeltaTime)
{
	m_pModel->Change_AnimIndex_ReturnTo_Must(BASE_ANIM_TURN_BACK, BASE_ANIM_IDLE, 0.1f, true);
	m_bPlayTurnBackAnim = true;
	Set_PlayerState(STATE_MOV);

	m_pHeadJoint->Add_ForceDir(m_fMovDir.InverseXMVector(), 100);
	
}

void CPlayer::Set_State_ParkourStart(_double fDeltaTime)
{
	m_eCurState = STATE_PARKOUR;
	m_eCurParkourState = m_pCurParkourTrigger->Get_ParkourTriggerType();

	switch (m_eCurParkourState)
	{
	case CTriggerObject::EParkourTriggerType::PACUR_LEDGE:
		if (CTestLedgeTrigger::ELedgeTriggerState::STATE_LAST_LEDGE == static_cast<CTestLedgeTrigger*>(m_pCurParkourTrigger)->Get_LedgeType())
		{
			m_eCurLedgeState = LEDGE_HANGING_CLIMBDOWN;
			m_pModel->Change_AnimIndex(LEDGE_ANIM_HANGING_CLIMBDOWN);
		}
		else 
		{
			m_eCurLedgeState = LEDGE_JUMP;
			m_pModel->Change_AnimIndex(LEDGE_ANIM_JUMP);
		}
		break;
	}
}

void CPlayer::Set_State_CurtainStart(_double fDeltaTime)
{
	m_pModel->Change_AnimIndex(CURTAIN_ANIM_TRANSITION);
	m_eCurState = STATE_CURTAIN;
}

void CPlayer::Set_State_WallRunStart(_bool bRightDir, _double fDeltaTime)
{
	if (false == bRightDir)
	{
		m_pModel->Change_AnimIndex(WALLRUN_ANIM_LEFT);
	}
	else
	{
		m_pModel->Change_AnimIndex(WALLRUN_ANIM_RIGHT);
	}
	m_eCurState = STATE_WALLRUN;
}

void CPlayer::Set_State_PillarStart(_double fDeltaTime)
{
	m_pModel->Change_AnimIndex(PILLAR_ANIM_GRAB);
	m_eCurState = STATE_PILLAR;
}

void CPlayer::Set_State_PetalStart(_double fDeltaTime)
{
	m_pModel->Change_AnimIndex(PETAL_ANIM_PLUCK);
	m_eCurState = STATE_PETAL;
	m_eCurPetalState = PETAL_PLUCK;
}

void CPlayer::Set_State_DamageStart(_float fKnockbackPower, _fVector vDamageDir)
{
	m_eCurState = STATE_TAKE_DAMAGE;
	m_fKnockbackPower = fKnockbackPower;
	m_fKnockbackDir = XMVector3Normalize(XMVectorSetY(vDamageDir, 0.f));
	m_pModel->Change_AnimIndex(DAMAGE_ANIM_DAMAGE);
}

void CPlayer::Set_State_DeathStart()
{
	m_eCurState = STATE_DEAD;
	m_pModel->Change_AnimIndex(DAMAGE_ANIM_DEATH, 0.1f);
}

void CPlayer::Update_AttachCamPos()
{
	_Vector vMyPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
	_float3 vBonePos = Get_BonePos("skd_hip");
	XMStoreFloat3(&m_fAttachCamPos, vMyPos + m_fAttachCamPos_Offset.XMVector());

	CCamera_Main* pMainCam = ((CCamera_Main*)(g_pGameInstance->Get_GameObject_By_LayerIndex(SCENE_STATIC, TAG_LAY(Layer_Camera_Main))));
	_float _fCameraArmLength = pMainCam->Get_TargetArmLength();
	CTransform* pCamTransform = pMainCam->Get_Camera_Transform();

	XMStoreFloat3(&m_fAttachCamPos, m_fAttachCamPos.XMVector() - pCamTransform->Get_MatrixState(CTransform::TransformState::STATE_LOOK) * _fCameraArmLength);

	m_fAttachCamLook = XMVector3Normalize((XMVectorSetW(vBonePos.XMVector() - m_fAttachCamPos.XMVector(), 0.f))) + m_fAttachCamLook_Offset.XMVector();
}

HRESULT CPlayer::Update_CamLookPoint(_double fDeltaTime)
{
	return E_NOTIMPL;
}

HRESULT CPlayer::Update_State_Idle(_double fDeltaTime)
{
	if (MOVDIR_END != m_eInputDir)
	{
		Set_State_MoveStart(fDeltaTime);
	}
	else
	{
		Set_State_IdleStart(fDeltaTime);
	}


	/** Change State By KeyInput */
	if (true == m_bPressedDodgeKey)
	{
		if (nullptr != m_pCurParkourTrigger)
		{
			Set_State_ParkourStart(fDeltaTime);
		}
		else
		{
			Set_State_DodgeStart(fDeltaTime);
		}
	}
	else if (true == m_bPressedInteractKey)
	{
		Set_State_PetalStart(fDeltaTime);
		/*if (nullptr != m_pCurParkourTrigger && CTestLedgeTrigger::ELedgeTriggerState::STATE_LAST_LEDGE == static_cast<CTestLedgeTrigger*>(m_pCurParkourTrigger)->Get_LedgeType())
		{
			Set_State_ParkourStart(fDeltaTime);
		}*/
	}
	else if (true == m_bPressedMainAttackKey || true == m_bPressedPowerAttackKey)
	{
		Set_State_MainAttackStart(fDeltaTime);
	}
	else if (true == m_bPressedUtilityKey)
	{
		Set_State_UtilitySkillStart(fDeltaTime);
	}
	else if (true == m_bPressedUltimateKey)
	{
		Set_State_UltimateSkillStart(fDeltaTime);
	}
	else
	{
		Check_SwapWeapon_KeyInput(fDeltaTime);
	}
	//


	return _int();
}

HRESULT CPlayer::Update_State_Move(_double fDeltaTime)
{
	if (true == m_bPlayTurnBackAnim)
	{
		Turn_Back(fDeltaTime);
		if (false == m_pModel->Get_IsHavetoBlockAnimChange())
		{
			m_bPlayTurnBackAnim = false;
		}
	}
	else
	{
		m_fAnimSpeed = 1.2f;

		if (MOVDIR_END != m_eInputDir)
		{
			Set_State_MoveStart(fDeltaTime);
		}
		else
		{
			Set_State_IdleStart(fDeltaTime);
		}

		_Vector vMyNormalizedLook = m_pTransformCom->Get_MatrixState_Normalized(CTransform::TransformState::STATE_LOOK);
		_Vector vDot = XMVector3Dot(m_fMovDir.XMVector(), vMyNormalizedLook);

		if (false == m_bPlayTurnBackAnim && XMVectorGetX(vDot) <= -0.7 && 0.8f < m_fCurTime_PressedMoveKeyDuration)
		{
			m_fCurTime_PressedMoveKeyDuration = 0.f;
			Set_State_TurnBackStart(fDeltaTime);
			return _int();
		}


		/** Change State By KeyInput */
		if (true == m_bPressedDodgeKey)
		{ 
			if (nullptr != m_pCurParkourTrigger)
			{
				Set_State_ParkourStart(fDeltaTime);
			}
			else
			{
				Set_State_DodgeStart(fDeltaTime);
			}
		}
		else if (true == m_bPressedMainAttackKey || true == m_bPressedPowerAttackKey)
		{
			Set_State_MainAttackStart(fDeltaTime);
		}
		else if (true == m_bPressedUtilityKey)
		{
			Set_State_UtilitySkillStart(fDeltaTime);
		}
		else if (true == m_bPressedUltimateKey)
		{
			Set_State_UltimateSkillStart(fDeltaTime);
		}
		else
		{
			Check_SwapWeapon_KeyInput(fDeltaTime);
		}
		//
	}

	return _int();
}

HRESULT CPlayer::Update_State_Jump(_double fDeltaTime)
{
	_float fCurAnimRate = (_float)m_pModel->Get_PlayRate();

	if (0.f <= fCurAnimRate)
	{
		if (0.194f <= fCurAnimRate && 0.694f >= fCurAnimRate)
		{
			m_fFallingAcc += 0.02267f;
			_float fPos_y = m_fJumpStart_Y + (5.f * m_fFallingAcc - 9.8f * m_fFallingAcc * m_fFallingAcc * 0.5f);

			m_pTransformCom->Move_Forward(fDeltaTime * 1.6f); 
			_Vector vMyPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
			vMyPos = XMVectorSetY(vMyPos, fPos_y);
			m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, vMyPos);
		}

		if (0.98f <= fCurAnimRate)
		{
			Set_State_IdleStart(fDeltaTime);
			m_fFallingAcc = 0.f;
		}
	}
	return S_OK;
}

HRESULT CPlayer::Update_State_Attack(_double fDeltaTime)
{
	switch (m_eCurWeapon)
	{
	case EWEAPON_TYPE::WEAPON_SPEAR:
		if (true == m_bAttackEnd && true == m_bPressedPowerAttackKey)
		{
			m_bPlayPowerAttack = true;
		}

		if (true == m_bAttackEnd)
		{
			Set_MainAttackAnim(m_bPlayJumpAttack);
			m_bAttackEnd = false;
		}

		Attack_Spear(fDeltaTime);
		break;
	case EWEAPON_TYPE::WEAPON_BOW:
		if (true == m_bAttackEnd || EBOWMAINATK_STATE::BOWMAINATK_START == m_eCurBowMainAtkState)
		{
			Set_MainAttackAnim(m_bPlayJumpAttack);
			m_bAttackEnd = false;
		}

		if (true == m_bAttackEnd && true == m_bPressedPowerAttackKey && EBOWMAINATK_STATE::BOWMAINATK_SHOT != m_eCurBowMainAtkState)
		{
			m_bPlayPowerAttack = true;
		}


		Attack_Bow(fDeltaTime);
		break;
	case EWEAPON_TYPE::WEAPON_SWORD:
		if (true == m_bAttackEnd && true == m_bPressedPowerAttackKey)
		{
			m_bPlayPowerAttack = true;
		}

		if (true == m_bAttackEnd)
		{
			Set_MainAttackAnim(m_bPlayJumpAttack);
			m_bAttackEnd = false;
		}

		Attack_Sword(fDeltaTime);
		break;
	}


	return S_OK;
}

HRESULT CPlayer::Update_State_UtilitySkill(_double fDeltaTime)
{
	switch (m_eCurWeapon)
	{
	case EWEAPON_TYPE::WEAPON_SPEAR:
		Javelin(fDeltaTime);
		break;
	case EWEAPON_TYPE::WEAPON_BOW:
		Shelling(fDeltaTime);
		break;
	case EWEAPON_TYPE::WEAPON_SWORD:
		Shield_Mode(fDeltaTime);
		break;
	}
	return S_OK;
}

HRESULT CPlayer::Update_State_UltimateSkill(_double fDeltaTime)
{
	switch (m_eCurWeapon)
	{
	case EWEAPON_TYPE::WEAPON_SPEAR:
		Spear_Ultimate(fDeltaTime);
		break;
	case EWEAPON_TYPE::WEAPON_BOW:
		Bow_Ultimate(fDeltaTime);
		break;
	case EWEAPON_TYPE::WEAPON_SWORD:
		Sword_Ultimate(fDeltaTime);
		break;
	}
	return S_OK;
}

HRESULT CPlayer::Update_State_Evasion(_double fDeltaTime)
{
	Play_DodgeAnim();
	Dodge(fDeltaTime);
	return S_OK;
}

HRESULT CPlayer::Update_State_Parkour(_double fDeltaTime)
{
	//
	switch (m_eCurParkourState)
	{
	case CTriggerObject::EParkourTriggerType::PACUR_LEDGE:
		Ledging(fDeltaTime);
		break;
	case CTriggerObject::EParkourTriggerType::PACUR_END:
		break;
	}

	return S_OK;
}

HRESULT CPlayer::Update_State_Curtain(_double fDeltaTime)
{
	_float fAnimPlayRate = (_float)m_pModel->Get_PlayRate();

	switch (m_pModel->Get_NowAnimIndex())
	{
	case CURTAIN_ANIM_TRANSITION:
	{
		if (0.f < fAnimPlayRate)
		{
			if (0.689f <= fAnimPlayRate && 0.862 >= fAnimPlayRate)
			{
				m_pTransformCom->Move_Forward(fDeltaTime * 0.1f);
			}
			else if (0.98f <= fAnimPlayRate)
			{
				m_pModel->Change_AnimIndex(CURTAIN_ANIM_FALLING);
			}
		}

	}
		break;
	case CURTAIN_ANIM_FALLING:
	{
		m_pTransformCom->Move_Down(fDeltaTime * 2.f);
		_Vector vMyPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
		_float fMyPos_Y = XMVectorGetY(vMyPos);
		if (0.f >= fMyPos_Y)
		{
			vMyPos = XMVectorSetY(vMyPos, 0.f);
			m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, vMyPos);

			Set_State_IdleStart(fDeltaTime);
		}

	}
		break;
	}

	return S_OK;
}

HRESULT CPlayer::Update_State_WallRun(_double fDeltaTime)
{
	_float fAnimPlayRate = (_float)m_pModel->Get_PlayRate();
	if (0.f < fAnimPlayRate)
	{
		if (0.074f <= fAnimPlayRate && 0.746f >= fAnimPlayRate)
		{
			m_pTransformCom->Move_Forward(fDeltaTime);
		}
		if (0.98f < fAnimPlayRate)
		{
			Set_State_IdleStart(fDeltaTime);
		}
	}

	return S_OK;
}

HRESULT CPlayer::Update_State_Pillar(_double fDeltaTime)
{
	_float fAnimPlayRate = (_float)m_pModel->Get_PlayRate();

	switch (m_pModel->Get_NowAnimIndex())
	{
	case PILLAR_ANIM_GRAB:
	{
		if (0.f < fAnimPlayRate)
		{
			if (0.214f <= fAnimPlayRate && 0.392f >= fAnimPlayRate)
			{
				m_pTransformCom->Move_Forward(fDeltaTime * 0.1f);
				m_pTransformCom->Move_Up(fDeltaTime * 0.5f);
			}
			else if (0.98f < fAnimPlayRate)
			{
				m_pModel->Change_AnimIndex(PILLAR_ANIM_IDLE);
			}
		}
	}
		break;
	case PILLAR_ANIM_IDLE:
	{
		if (true == m_bPressedInteractKey)
		{
			m_pModel->Change_AnimIndex(PILLAR_ANIM_MOVE_DOWN);
		}
		else 
		{
			if (true == m_bPressedDodgeKey)
			{
				m_fFallingStart_Y = XMVectorGetY(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS));
				m_pModel->Change_AnimIndex(PILLAR_ANIM_JUMP);
			}
			else if (MOVDIR_F == m_eInputDir)
			{
				// Check Pillar's Top Col
				//if (g_pGameInstance->Get_DIKeyState(DIK_E) & DIS_Down)
				//{
				//	m_pModel->Change_AnimIndex(PILLAR_ANIM_TOP_CLIMB);
				//}
				//else
				//{
					m_pModel->Change_AnimIndex(PILLAR_ANIM_CLIMB_UP);
				//}
			}
			else if (MOVDIR_B == m_eInputDir)
			{
				m_pModel->Change_AnimIndex(PILLAR_ANIM_CLIMB_DOWN);
			}
			else if (MOVDIR_L == m_eInputDir)
			{
				m_pModel->Change_AnimIndex(PILLAR_ANIM_ROT_CLOCK);
			}
			else if (MOVDIR_R == m_eInputDir)
			{
				m_pModel->Change_AnimIndex(PILLAR_ANIM_ROT_ANTICLOCK);
			}
		}
	}
		break;
	case PILLAR_ANIM_CLIMB_UP:
	{
		if (0.f < fAnimPlayRate)
		{
			if (0.16f <= fAnimPlayRate && 0.6f >= fAnimPlayRate)
			{
				m_pTransformCom->Move_Up(fDeltaTime); 
			}
			else if (0.98f < fAnimPlayRate)
			{
				m_pModel->Change_AnimIndex(PILLAR_ANIM_IDLE);
			}
		}
	}
		break;
	case PILLAR_ANIM_CLIMB_DOWN:
	{
		if (0.f < fAnimPlayRate)
		{
			if (0.4f <= fAnimPlayRate && 0.84f >= fAnimPlayRate)
			{
				m_pTransformCom->Move_Down(fDeltaTime * 0.5f);
			}
			else if (0.98f < fAnimPlayRate)
			{
				m_pModel->Change_AnimIndex(PILLAR_ANIM_IDLE, 0.f);
			}
		}
	}
		break;
	case PILLAR_ANIM_ROT_CLOCK:
	{
		if (0.f < fAnimPlayRate)
		{
			m_pTransformCom->Turn_CCW(XMVectorSet(0.f, 1.f, 0.f, 0.f), fDeltaTime);
			if (0.95f < fAnimPlayRate)
			{
				m_pModel->Change_AnimIndex(PILLAR_ANIM_IDLE);
			}
		}
	}
		break;
	case PILLAR_ANIM_ROT_ANTICLOCK:
	{
		if (0.f < fAnimPlayRate)
		{
			m_pTransformCom->Turn_CW(XMVectorSet(0.f, 1.f, 0.f, 0.f), fDeltaTime);
			if (0.95f < fAnimPlayRate)
			{
				m_pModel->Change_AnimIndex(PILLAR_ANIM_IDLE);
			}
		}
	}
		break;
	case PILLAR_ANIM_MOVE_DOWN:
	{
		m_fFallingAcc += 0.1f;
		if (5.f <= m_fFallingAcc)
		{
			m_fFallingAcc = 5.f;
		}
		m_pTransformCom->Move_Down(fDeltaTime * m_fFallingAcc);
		
		if (g_pGameInstance->Get_DIKeyState(DIK_E) & DIS_Up)
		{
			m_pModel->Change_AnimIndex(PILLAR_ANIM_IDLE);
			m_fFallingAcc = 0.f;
		}
	}
		break;
	case PILLAR_ANIM_TOP_CLIMB:
	{
		if (0.f < fAnimPlayRate)
		{
			if (0.22f <= fAnimPlayRate && 0.5f >= fAnimPlayRate)
			{
				m_pTransformCom->Move_Up(fDeltaTime * 0.5f);
			}
			else if (0.98f < fAnimPlayRate)
			{
				m_pModel->Change_AnimIndex(PILLAR_ANIM_TOP_CLIMB_IDLE);
			}
		}
	}
		break;
	case PILLAR_ANIM_TOP_CLIMB_IDLE:
	{
		if (0.f < fAnimPlayRate)
		{
			if (true == m_bPressedDodgeKey)
			{
				m_fFallingStart_Y = XMVectorGetY(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS));
				m_pModel->Change_AnimIndex(PILLAR_ANIM_TOP_JUMP);
			}
			else if (true == m_bPressedInteractKey)
			{
				m_pModel->Change_AnimIndex(PILLAR_ANIM_DOWN_CLIMB);
			}
			else if (MOVDIR_L == m_eInputDir)
			{
				m_pModel->Change_AnimIndex(PILLAR_ANIM_TOP_CLIMB_ROT_CLOCK);
			}
			else if (MOVDIR_R == m_eInputDir)
			{
				m_pModel->Change_AnimIndex(PILLAR_ANIM_TOP_CLIMB_ROT_ANTICLOCK);
			}
		}
	}
		break;
	case PILLAR_ANIM_TOP_CLIMB_ROT_CLOCK:
	{
		if (0.f < fAnimPlayRate)
		{
			if (0.98f < fAnimPlayRate || MOVDIR_L != m_eInputDir)
			{
				m_pModel->Change_AnimIndex(PILLAR_ANIM_TOP_CLIMB_IDLE, 0.18f);
			}
			m_pTransformCom->Turn_CCW(XMVectorSet(0.f, 1.f, 0.f, 0.f), fDeltaTime);
		}
	}
		break;
	case PILLAR_ANIM_TOP_CLIMB_ROT_ANTICLOCK:
	{
		if (0.f < fAnimPlayRate)
		{
			if (0.98f < fAnimPlayRate || MOVDIR_R != m_eInputDir)
			{
				m_pModel->Change_AnimIndex(PILLAR_ANIM_TOP_CLIMB_IDLE, 0.18f);
			}

			m_pTransformCom->Turn_CW(XMVectorSet(0.f, 1.f, 0.f, 0.f), fDeltaTime);
		}
	}
		break;
	case PILLAR_ANIM_DOWN_CLIMB:
	{
		if (0.f < fAnimPlayRate)
		{
			if (0.5f <= fAnimPlayRate && 0.72f >= fAnimPlayRate)
			{
				m_pTransformCom->Move_Down(fDeltaTime * 0.5f);
			}
			else if (0.98f < fAnimPlayRate)
			{
				m_pModel->Change_AnimIndex(PILLAR_ANIM_IDLE);
			}
		}
	}
		break;
	case PILLAR_ANIM_JUMP:
	{
		if (0.f < fAnimPlayRate)
		{
			if (0.12f > fAnimPlayRate)
			{
				m_fAnimSpeed = 2.f;
			}
			else if (false == m_bActionSwitch && 0.12f <= fAnimPlayRate)
			{
				m_bActionSwitch = true;
				m_pTransformCom->Turn_Direct(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(180.f));
			}
			else if (true == m_bActionSwitch)
			{
				m_fFallingAcc += 0.03f;
				m_pTransformCom->Move_Forward(fDeltaTime * 2.f);
				_Vector vMyPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
				_float fPrePos_Y = XMVectorGetY(vMyPos);
				_float fPos_y = m_fFallingStart_Y + (8.f * m_fFallingAcc - 9.8f * m_fFallingAcc * m_fFallingAcc * 0.5f);
				if (fPrePos_Y >= fPos_y)
				{
					m_pModel->Change_AnimIndex(LEDGE_ANIM_FALLING);
					m_bActionSwitch = false;
					break;
				}
				vMyPos = XMVectorSetY(vMyPos, fPos_y);
				m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, vMyPos);
			}
		}
	}
		break;
	case PILLAR_ANIM_TOP_JUMP:
	{
		if (0.f < fAnimPlayRate)
		{
			m_fFallingAcc += 0.03f;
			m_pTransformCom->Move_Forward(fDeltaTime * 2.f);
			_Vector vMyPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
			_float fPrePos_Y = XMVectorGetY(vMyPos);
			_float fPos_y = m_fFallingStart_Y + (8.f * m_fFallingAcc - 9.8f * m_fFallingAcc * m_fFallingAcc * 0.5f);
			if (fPrePos_Y >= fPos_y)
			{
				m_pModel->Change_AnimIndex(LEDGE_ANIM_FALLING);
				m_bActionSwitch = false;
				break;
			}
			vMyPos = XMVectorSetY(vMyPos, fPos_y);
			m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, vMyPos);
		}
	}
		break;
	case LEDGE_ANIM_FALLING:
	{
		m_fFallingAcc += 0.03f;
		m_pTransformCom->Move_Forward(fDeltaTime * 1.5f);
		_Vector vMyPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
		_float fPrePos_Y = XMVectorGetY(vMyPos);
		_float fPos_y = m_fFallingStart_Y + (8.f * m_fFallingAcc - 9.8f * m_fFallingAcc * m_fFallingAcc * 0.5f);
		vMyPos = XMVectorSetY(vMyPos, fPos_y);
		m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, vMyPos);
		
		if (0.f >= XMVectorGetY(vMyPos))
		{
			m_fFallingAcc = 0.f;
			Set_State_IdleStart(fDeltaTime);
		}
	}
		break;
	}
	return S_OK;
}

HRESULT CPlayer::Update_State_Petal(_double fDeltaTime)
{
	_float fAnimPlayRate = (_float)m_pModel->Get_PlayRate();

	switch (m_eCurPetalState)
	{
	case PETAL_PLUCK:
	{
		if (0.98f < fAnimPlayRate)
		{
			m_pModel->Change_AnimIndex(BASE_ANIM_IDLE);
			m_eCurPetalState = PETAL_IDLE;
		}
	}
		break;
	case PETAL_IDLE:
	{
		if (true == m_bPressedUtilityKey)
		{
			m_eCurPetalState = PETAL_THROW_LOOP;
			m_pModel->Change_AnimIndex(PETAL_ANIM_THROW_LOOP);
		}
		else if (MOVDIR_END != m_eInputDir)
		{
			Set_State_MoveStart(fDeltaTime);
			Set_PlayerState(STATE_PETAL);
			m_eCurPetalState = PETAL_WALK;
		}
	}
		break;
	case PETAL_WALK:
	{
		if (true == m_bPressedUtilityKey)
		{
			m_eCurPetalState = PETAL_THROW_LOOP;
			m_pModel->Change_AnimIndex(PETAL_ANIM_THROW_LOOP);
			break;
		}
		else if (MOVDIR_END == m_eInputDir)
		{
			Set_State_IdleStart(fDeltaTime);
			Set_PlayerState(STATE_PETAL);
			m_eCurPetalState = PETAL_IDLE;
			break;
		}
		Set_State_MoveStart(fDeltaTime);
		Set_PlayerState(STATE_PETAL);
	}
		break;
	case PETAL_THROW_LOOP:
	{
		if (g_pGameInstance->Get_DIKeyState(DIK_LSHIFT) & DIS_Up)
		{
			m_pModel->Change_AnimIndex(BASE_ANIM_IDLE);
			m_eCurPetalState = PETAL_IDLE;
			break;
		}
		else if (true == m_bPressedMainAttackKey)
		{
			m_pModel->Change_AnimIndex(PETAL_ANIM_THROW_THROW);
			m_eCurPetalState = PETAL_THROW_THROW;
			break;
		}

		//LookAt_MousePos();
	}
		break;
	case PETAL_THROW_THROW:
	{
		if (0.98f < fAnimPlayRate)
		{
			Set_State_IdleStart(fDeltaTime);
		}
	}
		break;
	}
	return S_OK;
}

HRESULT CPlayer::Update_State_Damage(_double fDeltaTime)
{
	_float fAnimPlayRate = (_float)m_pModel->Get_PlayRate();
	m_fAnimSpeed = 2.f;
	if (0.f < fAnimPlayRate)
	{
		if (0.155f >= fAnimPlayRate)
		{
			m_fAnimSpeed = 1.5f;
			_float fKnockbackPower = g_pGameInstance->Easing(TYPE_CubicOut, 0.f, m_fKnockbackPower, fAnimPlayRate, 0.155f);
			m_pTransformCom->MovetoDir_bySpeed(m_fKnockbackDir.XMVector(), fKnockbackPower, fDeltaTime);
			m_pTransformCom->Turn_Dir(m_fKnockbackDir.XMVector() * -1.f, 0.7f);
		}
		else //if (0.5f <= fAnimPlayRate)
		{
			if(0.f >= m_fHP && 0.98f <= fAnimPlayRate)
			{
				Set_State_DeathStart();
				return S_OK;
			}
			else if(0.f < m_fHP && 0.5f <= fAnimPlayRate)
			{
				if (MOVDIR_END == m_eInputDir)
				{
					Set_State_IdleStart(fDeltaTime);
				}
				else
				{
					Set_State_MoveStart(fDeltaTime);
				}
			}
		}
	}
	return _int();
}

HRESULT CPlayer::Update_State_Execution(_double fDeltaTime)
{
	return _int();
}

HRESULT CPlayer::Update_State_Dead(_double fDeltaTime)
{
	_float fAnimPlayRate = (_float)m_pModel->Get_PlayRate();
	if (0.f < fAnimPlayRate)
	{
		if (0.98f < fAnimPlayRate)
		{
			m_bUpdateAnimation = false;
		}
	}
	return _int();
}

_bool CPlayer::Check_InputDirIsForward()
{
	_Vector vMyRight = XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_RIGHT));
	_Vector vCamRight = XMVector3Normalize(m_pMainCameraTransform->Get_MatrixState(CTransform::TransformState::STATE_RIGHT));

	if (0.5f <= XMVectorGetX(XMVector3Dot(vMyRight, vCamRight)))
	{
		return true;
	}
	return false;
}

_bool CPlayer::Check_PlayerKeyInput(_double fDeltaTime)
{
	Check_Mov_KeyInput(fDeltaTime);
	Check_Action_KeyInput(fDeltaTime);
	Check_ChangeCameraView_KeyInput_ForDebug(fDeltaTime);

	return true;
}

_bool CPlayer::Check_Mov_KeyInput(_double fDeltaTime)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	_bool bChekKey = false;
	_int iMov_F = 0, iMov_R = 0;

	if (pGameInstance->Get_DIKeyState(DIK_W) & DIS_Press)
	{
		iMov_F += 1;
		bChekKey = true;
	}
	if (pGameInstance->Get_DIKeyState(DIK_S) & DIS_Press)
	{
		iMov_F -= 1;
		bChekKey = true;
	}
	if (pGameInstance->Get_DIKeyState(DIK_A) & DIS_Press)
	{
		iMov_R -= 1;
		bChekKey = true;
	}
	if (pGameInstance->Get_DIKeyState(DIK_D) & DIS_Press)
	{
		iMov_R += 1;
		bChekKey = true;
	}


	// For TurnBack Control
	if (0.f != iMov_F || 0.f != iMov_R)
	{
		m_fCurTime_PressedMoveKeyDuration += 0.1f;
		if (m_fCurTime_PressedMoveKeyDuration >= m_fMaxTime_PressedMoveKeyDuration)
		{
			m_fCurTime_PressedMoveKeyDuration = m_fMaxTime_PressedMoveKeyDuration;
		}
	} 
	else
	{
		m_fCurTime_PressedMoveKeyDuration -= 0.1f;
		if (m_fCurTime_PressedMoveKeyDuration <= 0.f)
		{
			m_fCurTime_PressedMoveKeyDuration = 0.f;
		}
	}
	//

	Set_InputDir(iMov_F, iMov_R, fDeltaTime);

	RELEASE_INSTANCE(CGameInstance);
	return bChekKey;
}

_bool CPlayer::Check_ChangeCameraView_KeyInput_ForDebug(_double fDeltaTime)
{
	_int iInputDir = 0;
	if (g_pGameInstance->Get_DIKeyState(DIK_RBRACKET) & DIS_Down)
	{
		iInputDir += 1;
	}
	else if (g_pGameInstance->Get_DIKeyState(DIK_LBRACKET) & DIS_Down)
	{
		iInputDir -= 1;
	}

	if (0 != iInputDir)
	{
		m_iCurCamViewIndex += iInputDir;
		if (0.f >= m_iCurCamViewIndex)
		{
			m_iCurCamViewIndex = 0;
		}
		else if (m_iMaxCamViewIndex <= m_iCurCamViewIndex)
		{
			m_iCurCamViewIndex = m_iMaxCamViewIndex - 1;
		}

		switch (m_iCurCamViewIndex)
		{
		case 0:
			// Base Point of view
			m_pMainCamera->Lock_CamLook(false);
			m_fAttachCamPos_Offset = _float3(0.f, 1.5f, -2.f);
			m_fAttachCamLook_Offset = _float3(0.f, 0.f, 0.f);
			break;
		case 1:
			// Side Scroll Point of view
			m_pMainCamera->Lock_CamLook(true, XMVectorSet(0.f, 0.f, 1.f, 1.f));
			m_fAttachCamPos_Offset = _float3(0.f, 2.f, -2.f);
			m_fAttachCamLook_Offset = _float3(0.f, 0.f, 0.f);
			break;
		case 2:
			// Snake Boss Point of view
			m_pMainCamera->Lock_CamLook(true, XMVectorSet(0.f, 0.f, 1.f, 1.f));
			m_fAttachCamPos_Offset = _float3(0.f, 5.f, -10.f);
			m_fAttachCamLook_Offset = _float3(0.f, 0.f, 0.f);
			break;
		case 3:
			// First Person Point of view
			m_pMainCamera->Lock_CamLook(true, XMVectorSet(0.f, 0.f, 1.f, 1.f));
			m_fAttachCamPos_Offset = _float3(0.5f, 1.28f, 2.f);
			m_fAttachCamLook_Offset = _float3(0.f, 0.f, 0.f);
			break;
		}
	}

	return _bool();
}

_bool CPlayer::Check_Action_KeyInput(_double fDeltaTime)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	m_bPressedUltimateKey = false;

	if (pGameInstance->Get_DIKeyState(DIK_E) & DIS_Down)
	{
		m_bPressedInteractKey = true;
	}
	else
	{
		m_bPressedInteractKey = false;
	}

	if (pGameInstance->Get_DIKeyState(DIK_SPACE) & DIS_Down)
	{
		m_bPressedDodgeKey = true;
	}
	else
	{
		m_bPressedDodgeKey = false;
	}

	if (EWEAPON_TYPE::WEAPON_BOW == m_eCurWeapon)
	{
		if (pGameInstance->Get_DIMouseButtonState(CInput_Device::MBS_LBUTTON) & DIS_Down)
		{
			if (false == m_bDodging && false == m_bPlayPowerAttack)
			{
				m_bPressedMainAttackKey = true;
				m_bPressedPowerAttackKey = false;
			}
		}
		else if (pGameInstance->Get_DIMouseButtonState(CInput_Device::MBS_LBUTTON) & DIS_Up)
		{
			m_bPressedMainAttackKey = false;
		}
		else if (pGameInstance->Get_DIMouseButtonState(CInput_Device::MBS_RBUTTON) & DIS_Down)
		{
			m_bPressedPowerAttackKey = true;
		}
		else
		{
			m_bPressedPowerAttackKey = false;
		}
	}
	else
	{
		if (pGameInstance->Get_DIMouseButtonState(CInput_Device::MBS_LBUTTON) & DIS_Down)
		{
			m_bPressedMainAttackKey = true;
			m_bPressedPowerAttackKey = false;
			m_bPlayPowerAttack = false;
		}
		else if (pGameInstance->Get_DIMouseButtonState(CInput_Device::MBS_RBUTTON) & DIS_Down)
		{
			m_bPressedMainAttackKey = false;
			m_bPressedPowerAttackKey = true;
		}
		else
		{
			m_bPressedMainAttackKey = false;
			m_bPressedPowerAttackKey = false;
		}
	}


	if (pGameInstance->Get_DIKeyState(DIK_LSHIFT) & DIS_Down)
	{
		m_bPressedUtilityKey = true;
	}
	else if (pGameInstance->Get_DIKeyState(DIK_LSHIFT) & DIS_Up)
	{
		m_bPressedUtilityKey = false;
	}

	if (pGameInstance->Get_DIKeyState(DIK_Q) & DIS_Up)
	{
		m_bPressedUltimateKey = true;
	}

	RELEASE_INSTANCE(CGameInstance);
	return false;
}

_bool CPlayer::Check_SwapWeapon_KeyInput(_double fDeltaTime)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (WEAPON_CHAKRA == m_eCurWeapon)
	{
		if (CPlayerWeapon_Chakra::EChakraState::CHAKRA_IDLE != static_cast<CPlayerWeapon_Chakra*>(m_pPlayerWeapons[WEAPON_CHAKRA - 1])->Get_ChakraState())
		{
			RELEASE_INSTANCE(CGameInstance);
			return false;
		}
	}

	if (pGameInstance->Get_DIKeyState(DIK_1) & DIS_Down)
	{
		if (m_eCurWeapon > EWEAPON_TYPE::WEAPON_NONE)
		{
			m_pPlayerWeapons[m_eCurWeapon - 1]->Set_BlockUpdate(true);
			m_pPlayerWeapons[WEAPON_SHIELD - 1]->Set_BlockUpdate(true);
		}

		m_eCurWeapon = EWEAPON_TYPE::WEAPON_SPEAR;
		m_pPlayerWeapons[0]->Set_BlockUpdate(false);
	}
	else if (pGameInstance->Get_DIKeyState(DIK_2) & DIS_Down)
	{
		if (m_eCurWeapon > EWEAPON_TYPE::WEAPON_NONE)
		{
			m_pPlayerWeapons[m_eCurWeapon - 1]->Set_BlockUpdate(true);
			m_pPlayerWeapons[WEAPON_SHIELD - 1]->Set_BlockUpdate(true);
		}

		m_eCurWeapon = EWEAPON_TYPE::WEAPON_BOW;
		m_pPlayerWeapons[1]->Set_BlockUpdate(false);
	}
	else if (pGameInstance->Get_DIKeyState(DIK_3) & DIS_Down)
	{
		if (m_eCurWeapon > EWEAPON_TYPE::WEAPON_NONE)
		{
			m_pPlayerWeapons[m_eCurWeapon - 1]->Set_BlockUpdate(true);
			m_pPlayerWeapons[WEAPON_SHIELD - 1]->Set_BlockUpdate(true);
		}

		m_eCurWeapon = EWEAPON_TYPE::WEAPON_SWORD;
		m_pPlayerWeapons[m_eCurWeapon - 1]->Set_BlockUpdate(false);
		m_pPlayerWeapons[WEAPON_SHIELD - 1]->Set_BlockUpdate(false);
	}
	else if (pGameInstance->Get_DIKeyState(DIK_4) & DIS_Down)
	{
		if (m_eCurWeapon > EWEAPON_TYPE::WEAPON_NONE)
		{
			m_pPlayerWeapons[m_eCurWeapon - 1]->Set_BlockUpdate(true);
			m_pPlayerWeapons[WEAPON_SHIELD - 1]->Set_BlockUpdate(true);
		}

		m_eCurWeapon = EWEAPON_TYPE::WEAPON_CHAKRA;
		m_pPlayerWeapons[m_eCurWeapon - 1]->Set_BlockUpdate(false);
	}

	RELEASE_INSTANCE(CGameInstance);
	return true;
}


void CPlayer::Move(EINPUT_MOVDIR eMoveDir, _double fDeltaTime)
{
	if (MOVDIR_END == eMoveDir)
		return;

	CCamera_Main* pMainCam = (CCamera_Main*)(g_pGameInstance->Get_GameObject_By_LayerIndex(SCENE_STATIC, TAG_LAY(Layer_Camera_Main)));
	_Vector vMyNormalizedLook = m_pTransformCom->Get_MatrixState_Normalized(CTransform::TransformState::STATE_LOOK);

	_float fDiagonalSpeed = 1.f;
	_float fTurnRate = 0.85f;
	_float fMoveRate = (_float)fDeltaTime;


	_Vector vMovDir;
	switch (eMoveDir)
	{
	case EINPUT_MOVDIR::MOVDIR_F:
	{
		vMovDir = pMainCam->Get_CameraState_Normalize(CTransform::TransformState::STATE_LOOK);
		vMovDir = XMVectorSetY(vMovDir, XMVectorGetY(vMyNormalizedLook));
		break;
	}
	case EINPUT_MOVDIR::MOVDIR_B:
	{
		vMovDir = pMainCam->Get_CameraState_Normalize(CTransform::TransformState::STATE_LOOK) * -1;
		vMovDir = XMVectorSetY(vMovDir, XMVectorGetY(vMyNormalizedLook));
		break;
	}
	case EINPUT_MOVDIR::MOVDIR_L:
	{
		vMovDir = pMainCam->Get_CameraState_Normalize(CTransform::TransformState::STATE_RIGHT) * -1;
		vMovDir = XMVectorSetY(vMovDir, XMVectorGetY(vMyNormalizedLook));
		break;
	}
	case EINPUT_MOVDIR::MOVDIR_R:
	{
		vMovDir = pMainCam->Get_CameraState_Normalize(CTransform::TransformState::STATE_RIGHT);
		vMovDir = XMVectorSetY(vMovDir, XMVectorGetY(vMyNormalizedLook));
		break;
	}
	case EINPUT_MOVDIR::MOVDIR_FR:
	{
		vMovDir = pMainCam->Get_CameraState_Normalize(CTransform::TransformState::STATE_LOOK);
		vMovDir += pMainCam->Get_CameraState_Normalize(CTransform::TransformState::STATE_RIGHT);
		XMVector3Normalize(vMovDir);
		vMovDir = XMVectorSetW(vMovDir, 0.f);
		vMovDir = XMVectorSetY(vMovDir, XMVectorGetY(vMyNormalizedLook));
		fMoveRate *= fDiagonalSpeed;
		break;
	}
	case EINPUT_MOVDIR::MOVDIR_FL:
	{
		vMovDir = pMainCam->Get_CameraState_Normalize(CTransform::TransformState::STATE_LOOK);
		vMovDir += (pMainCam->Get_CameraState_Normalize(CTransform::TransformState::STATE_RIGHT) * -1);
		XMVector3Normalize(vMovDir);
		vMovDir = XMVectorSetW(vMovDir, 0.f);
		vMovDir = XMVectorSetY(vMovDir, XMVectorGetY(vMyNormalizedLook));
		fMoveRate *= fDiagonalSpeed;
		break;
	}
	case EINPUT_MOVDIR::MOVDIR_BR:
	{
		vMovDir = pMainCam->Get_CameraState_Normalize(CTransform::TransformState::STATE_LOOK) * -1;
		vMovDir += pMainCam->Get_CameraState_Normalize(CTransform::TransformState::STATE_RIGHT);
		XMVector3Normalize(vMovDir);
		vMovDir = XMVectorSetW(vMovDir, 0.f);
		vMovDir = XMVectorSetY(vMovDir, XMVectorGetY(vMyNormalizedLook));
		fMoveRate *= fDiagonalSpeed;
		break;
	}
	case EINPUT_MOVDIR::MOVDIR_BL:
	{
		vMovDir = pMainCam->Get_CameraState_Normalize(CTransform::TransformState::STATE_LOOK) * -1;
		vMovDir += pMainCam->Get_CameraState_Normalize(CTransform::TransformState::STATE_RIGHT) * -1;
		XMVector3Normalize(vMovDir);
		vMovDir = XMVectorSetW(vMovDir, 0.f);
		vMovDir = XMVectorSetY(vMovDir, XMVectorGetY(vMyNormalizedLook));
		fMoveRate *= fDiagonalSpeed;
		break;
	}
	}

	if (false == m_bPlayTurnBackAnim)
	{
		m_pTransformCom->MovetoDir(vMovDir, fMoveRate, m_pNavigationCom);
		m_pTransformCom->Turn_Dir(vMovDir, fTurnRate);
	}

	m_fMovDir = XMVector3Normalize(vMovDir);
}

void CPlayer::Move_NotTurn(EINPUT_MOVDIR eMoveDir, _double fDeltaTime)
{
	if (MOVDIR_END == eMoveDir)
		return;

	CCamera_Main* pMainCam = (CCamera_Main*)(g_pGameInstance->Get_GameObject_By_LayerIndex(SCENE_STATIC, TAG_LAY(Layer_Camera_Main)));
	_Vector vMyNormalizedLook = m_pTransformCom->Get_MatrixState_Normalized(CTransform::TransformState::STATE_LOOK);

	_float fDiagonalSpeed = 1.f;
	_float fTurnRate = 0.85f;
	_float fMoveRate = (_float)fDeltaTime;


	_Vector vMovDir;
	switch (eMoveDir)
	{
	case EINPUT_MOVDIR::MOVDIR_F:
	{
		vMovDir = pMainCam->Get_CameraState_Normalize(CTransform::TransformState::STATE_LOOK);
		vMovDir = XMVectorSetY(vMovDir, XMVectorGetY(vMyNormalizedLook));
		break;
	}
	case EINPUT_MOVDIR::MOVDIR_B:
	{
		vMovDir = pMainCam->Get_CameraState_Normalize(CTransform::TransformState::STATE_LOOK) * -1;
		vMovDir = XMVectorSetY(vMovDir, XMVectorGetY(vMyNormalizedLook));
		break;
	}
	case EINPUT_MOVDIR::MOVDIR_L:
	{
		vMovDir = pMainCam->Get_CameraState_Normalize(CTransform::TransformState::STATE_RIGHT) * -1;
		vMovDir = XMVectorSetY(vMovDir, XMVectorGetY(vMyNormalizedLook));
		break;
	}
	case EINPUT_MOVDIR::MOVDIR_R:
	{
		vMovDir = pMainCam->Get_CameraState_Normalize(CTransform::TransformState::STATE_RIGHT);
		vMovDir = XMVectorSetY(vMovDir, XMVectorGetY(vMyNormalizedLook));
		break;
	}
	case EINPUT_MOVDIR::MOVDIR_FR:
	{
		vMovDir = pMainCam->Get_CameraState_Normalize(CTransform::TransformState::STATE_LOOK);
		vMovDir += pMainCam->Get_CameraState_Normalize(CTransform::TransformState::STATE_RIGHT);
		XMVector3Normalize(vMovDir);
		vMovDir = XMVectorSetW(vMovDir, 0.f);
		vMovDir = XMVectorSetY(vMovDir, XMVectorGetY(vMyNormalizedLook));
		fMoveRate *= fDiagonalSpeed;
		break;
	}
	case EINPUT_MOVDIR::MOVDIR_FL:
	{
		vMovDir = pMainCam->Get_CameraState_Normalize(CTransform::TransformState::STATE_LOOK);
		vMovDir += (pMainCam->Get_CameraState_Normalize(CTransform::TransformState::STATE_RIGHT) * -1);
		XMVector3Normalize(vMovDir);
		vMovDir = XMVectorSetW(vMovDir, 0.f);
		vMovDir = XMVectorSetY(vMovDir, XMVectorGetY(vMyNormalizedLook));
		fMoveRate *= fDiagonalSpeed;
		break;
	}
	case EINPUT_MOVDIR::MOVDIR_BR:
	{
		vMovDir = pMainCam->Get_CameraState_Normalize(CTransform::TransformState::STATE_LOOK) * -1;
		vMovDir += pMainCam->Get_CameraState_Normalize(CTransform::TransformState::STATE_RIGHT);
		XMVector3Normalize(vMovDir);
		vMovDir = XMVectorSetW(vMovDir, 0.f);
		vMovDir = XMVectorSetY(vMovDir, XMVectorGetY(vMyNormalizedLook));
		fMoveRate *= fDiagonalSpeed;
		break;
	}
	case EINPUT_MOVDIR::MOVDIR_BL:
	{
		vMovDir = pMainCam->Get_CameraState_Normalize(CTransform::TransformState::STATE_LOOK) * -1;
		vMovDir += pMainCam->Get_CameraState_Normalize(CTransform::TransformState::STATE_RIGHT) * -1;
		XMVector3Normalize(vMovDir);
		vMovDir = XMVectorSetW(vMovDir, 0.f);
		vMovDir = XMVectorSetY(vMovDir, XMVectorGetY(vMyNormalizedLook));
		fMoveRate *= fDiagonalSpeed;
		break;
	}
	}

	m_pTransformCom->MovetoDir(vMovDir, fMoveRate);

	m_fMovDir = XMVector3Normalize(vMovDir);
}

void CPlayer::Turn_Back(_double fDeltaTime)
{
	_float fRate = (_float)m_pModel->Get_PlayRate();

	if (0.f <= fRate && 0.384615384615384f >= fRate)
	{
		m_fAnimSpeed = 1.5f;
		m_pTransformCom->Turn_Dir(m_pTransformCom->Get_MatrixState_Normalized(CTransform::TransformState::STATE_LOOK) * -1.f, 0.72f);
	}
}

void CPlayer::Dodge(_double fDeltaTime)
{
	_float fAnimPlayRate = (_float)m_pModel->Get_PlayRate();
	switch (m_pModel->Get_NowAnimIndex())
	{
	case BASE_ANIM_DODGE_ROLL:
	{
		// On MotionTrail
		m_bOnNavigation = true;
		if (0.f < fAnimPlayRate && 0.52f > fAnimPlayRate)
		{
			m_bOn_MotionTrail = true;
		}
		else
		{
			m_bOn_MotionTrail = false;
		}

		if (true == m_pModel->Get_IsHavetoBlockAnimChange() && 0.148148f > fAnimPlayRate)
		{
			m_fAnimSpeed = 4.f;
		}
		else if (true == m_pModel->Get_IsHavetoBlockAnimChange() && 0.148148f <= fAnimPlayRate && 0.5925925f >= fAnimPlayRate)
		{
			_float MoveSpeed = g_pGameInstance->Easing_Return(TYPE_QuadOut, TYPE_QuarticIn, 0.f, 2.0f, fAnimPlayRate - 0.148148f, 0.4444445f);
			m_fAnimSpeed = 1.6f;

			m_pTransformCom->Move_Forward(fDeltaTime * MoveSpeed, m_pNavigationCom);
		}
		else if (true == m_pModel->Get_IsHavetoBlockAnimChange() && 0.5925925f < fAnimPlayRate)
		{
			m_fAnimSpeed = 3.5f;
			m_pModel->Set_BlockAnim(false);
			m_bOn_MotionTrail = false;
		}
		//

		////////////////////Next Combo Check //////////////////////
		// 1) Check to Input Next Combo Command
		Check_NextComboCommand();

		// 2) Check to Next Combo Behavior
		if ((m_eCurWeapon == EWEAPON_TYPE::WEAPON_SPEAR && 0.45f <= fAnimPlayRate && 0.52f >= fAnimPlayRate) ||
			(m_eCurWeapon == EWEAPON_TYPE::WEAPON_BOW && 0.51f <= fAnimPlayRate) ||
			(m_eCurWeapon == EWEAPON_TYPE::WEAPON_SWORD && 0.45f <= fAnimPlayRate && 0.52f >= fAnimPlayRate))
		{
			if (false == m_bPlayNextCombo)
				return;

			if (true == m_bReadyMainAttackCombo)
			{
				m_pModel->Set_BlockAnim(false);
				m_bPlayMainAttackCombo = true;
				m_bOn_MotionTrail = false;
			}
		}
		else if (0.52f <= fAnimPlayRate)
		{
			if (true == m_bReadyDodgeCombo)
			{
				m_pModel->Set_BlockAnim(false);
				m_bPlayDodgeCombo = true;
				m_bOn_MotionTrail = false;
			}
		}
	}
		break;
	case BASE_ANIM_DODGE_CARTWHEEL:
	{
		m_bOnNavigation = true;
		// On MotionTrail
		if (0.f < fAnimPlayRate && 0.65f > fAnimPlayRate)
		{
			m_bOn_MotionTrail = true;
		}
		else
		{
			m_bOn_MotionTrail = false;
		}

		if (0.f < fAnimPlayRate && 0.66f >= fAnimPlayRate)
		{
			m_fAnimSpeed = 1.6f;

			_float fMoveSpeed = 0.f;
			if (0.22f > fAnimPlayRate)			fMoveSpeed = 1.f;
			else if (0.4f > fAnimPlayRate)		fMoveSpeed = 0.2f;
			else if (0.66f >= fAnimPlayRate)	fMoveSpeed = 2.5f;

			m_pTransformCom->Move_Forward(fDeltaTime * fMoveSpeed, m_pNavigationCom);
		}
		else if (fAnimPlayRate && 0.7f < fAnimPlayRate)
		{
			m_fAnimSpeed = 1.f;
			m_pModel->Set_BlockAnim(false);
			m_bOn_MotionTrail = false;
		}


		////////////////////Next Combo Check //////////////////////
		// 1) Check to Input Next Combo Command
		Check_NextComboCommand();

		// 2) Check to Next Combo Behavior
		if ((m_eCurWeapon == EWEAPON_TYPE::WEAPON_SPEAR && 0.5f <= fAnimPlayRate && 0.65f >= fAnimPlayRate) ||
			(m_eCurWeapon == EWEAPON_TYPE::WEAPON_BOW && 0.65f <= fAnimPlayRate) ||
			(m_eCurWeapon == EWEAPON_TYPE::WEAPON_SWORD && 0.5f <= fAnimPlayRate && 0.65f >= fAnimPlayRate))
		{
			if (false == m_bPlayNextCombo)
				return;

			if (true == m_bReadyMainAttackCombo)
			{
				m_pModel->Set_BlockAnim(false);
				m_bPlayMainAttackCombo = true;
				m_bOn_MotionTrail = false;
			}
		}
		else if (0.61f <= fAnimPlayRate)
		{
			if (true == m_bReadyDodgeCombo)
			{
				m_pModel->Set_BlockAnim(false);
				m_bPlayDodgeCombo = true;
				m_bOn_MotionTrail = false;
			}
		}
	}
		break;
	case BASE_ANIM_DODGE_FLIP:
	{
		m_bOnNavigation = true;
		// On MotionTrail
		if (0.f < fAnimPlayRate && 0.62f > fAnimPlayRate)
		{
			m_bOn_MotionTrail = true;
		}
		else
		{
			m_bOn_MotionTrail = false;
		}


		if (true == m_pModel->Get_IsHavetoBlockAnimChange() && 0.24f > fAnimPlayRate)
		{
			m_fAnimSpeed = 4.f;
		}
		else if (true == m_pModel->Get_IsHavetoBlockAnimChange() && 0.24f <= fAnimPlayRate && 0.64f >= fAnimPlayRate)
		{
			_float MoveSpeed = g_pGameInstance->Easing_Return(TYPE_QuadOut, TYPE_QuarticIn, 0.f, 2.7f, fAnimPlayRate - 0.24f, 0.4f);
			m_fAnimSpeed = 1.4f;
			m_pTransformCom->Move_Forward(fDeltaTime * MoveSpeed, m_pNavigationCom);
		}
		else if (true == m_pModel->Get_IsHavetoBlockAnimChange() && fAnimPlayRate && 0.64f < fAnimPlayRate)
		{
			m_fAnimSpeed = 3.5f;
			m_pModel->Set_BlockAnim(false);
			m_bOn_MotionTrail = false;
		}


		// Next Combo Check
		Check_NextComboCommand();

		if ((m_eCurWeapon == EWEAPON_TYPE::WEAPON_SPEAR && 0.5f <= fAnimPlayRate && 0.8f >= fAnimPlayRate) ||
			(m_eCurWeapon == EWEAPON_TYPE::WEAPON_BOW && 0.55f <= fAnimPlayRate) ||
			(m_eCurWeapon == EWEAPON_TYPE::WEAPON_SWORD && 0.55f <= fAnimPlayRate && 0.8f >= fAnimPlayRate))
		{
			if (false == m_bPlayNextCombo)
				return;

			if (true == m_bReadyMainAttackCombo)
			{
				m_pModel->Set_BlockAnim(false);
				m_bPlayMainAttackCombo = true;
				m_bOn_MotionTrail = false;
			}
		}
		else if (0.62f <= fAnimPlayRate)
		{
			if (true == m_bReadyDodgeCombo)
			{
				m_pModel->Set_BlockAnim(false);
				m_bPlayDodgeCombo = true;
				m_bOn_MotionTrail = false;
			}
		}
	}
		break;
	}

	m_bDodging = true;
}

void CPlayer::Attack_Spear(_double fDeltaTime)
{
	_float fAnimPlayRate = (_float)m_pModel->Get_PlayRate();

	switch (m_pModel->Get_NowAnimIndex())
	{
		//// Main Attack ////
	case SPEAR_ANIM_MAIN_ATK_COMBO_0:
	{
		m_bOnNavigation = true;
		if (fAnimPlayRate <= 0.714f)
		{
			m_fAnimSpeed = 2.8f;
		}
		else
		{
			m_fAnimSpeed = 1.f;
		}

		if (true == m_pModel->Get_IsHavetoBlockAnimChange() && 0.714f <= fAnimPlayRate && 0.892f >= fAnimPlayRate)
		{
			_float MoveSpeed = g_pGameInstance->Easing_Return(TYPE_QuadOut, TYPE_QuarticIn, 0.f, 0.6f, fAnimPlayRate - 0.714f, 0.178f);
			m_pTransformCom->Move_Forward(fDeltaTime * MoveSpeed, m_pNavigationCom);
		}

		// Turn On Weapon Trail
		if (false == m_bTrailSwitch && m_pModel->Get_PlayRate() >= 0.607f)
		{
			m_bTrailSwitch = true;
			m_pPlayerWeapons[WEAPON_SPEAR - 1]->Active_Trail(true);
		}
		//

		////////////////////Next Combo Check //////////////////////
		// 1) Check to Input Next Combo Command
		Check_NextComboCommand();

		// 2) Check to Next Combo Behavior
		if (true == m_bPlayNextCombo)
		{
			if (0.92f <= fAnimPlayRate)
			{
				if (true == Change_NextCombo())
				{
					m_pPlayerWeapons[WEAPON_SPEAR - 1]->Active_Trail(false);
					m_bTrailSwitch = false;
				}
			}
			else if (0.85f <= fAnimPlayRate)
			{
				if (false == m_bPlayNextCombo)
					return;

				if (true == m_bReadyDodgeCombo)
				{
					m_pModel->Set_BlockAnim(false);
					m_bPlayDodgeCombo = true;
					m_pPlayerWeapons[WEAPON_SPEAR - 1]->Active_Trail(false);
					m_bTrailSwitch = false;
				}
			}
		}
		/////////////////////////////////////////////////////////


		// Look At Mouse Pos
		if (0.53f <= fAnimPlayRate)
		{
			LookAt_MousePos();
		}
		//
	}
	break;
	case SPEAR_ANIM_MAIN_ATK_COMBO_1:
	{
		m_bOnNavigation = true;
		if (fAnimPlayRate <= 0.6666f)
		{
			m_fAnimSpeed = 2.2f;
		}
		else
		{
			m_fAnimSpeed = 1.0f;
		}

		if (true == m_pModel->Get_IsHavetoBlockAnimChange() && 0.740f <= fAnimPlayRate && 0.851f >= fAnimPlayRate)
		{
			_float MoveSpeed = g_pGameInstance->Easing_Return(TYPE_QuadOut, TYPE_QuarticIn, 0.f, 2.f, fAnimPlayRate - 0.740f, 0.111f);
			m_pTransformCom->Move_Forward(fDeltaTime * MoveSpeed, m_pNavigationCom);
		}

		// Turn On Weapon Trail
		if (false == m_bTrailSwitch && m_pModel->Get_PlayRate() >= 0.44f)
		{
			m_bTrailSwitch = true;
			m_pPlayerWeapons[WEAPON_SPEAR - 1]->Active_Trail(true);
		}
		//


		////////////////////Next Combo Check //////////////////////
		// 1) Check to Input Next Combo Command
		Check_NextComboCommand();

		// 2) Check to Next Combo Behavior
		if (true == m_bPlayNextCombo)
		{
			if (0.92f <= fAnimPlayRate)
			{
				if (true == Change_NextCombo())
				{
					m_bTrailSwitch = false;
					m_pPlayerWeapons[WEAPON_SPEAR - 1]->Active_Trail(false);
				}
			}
			else if (0.85f <= fAnimPlayRate)
			{
				if (true == m_bReadyDodgeCombo)
				{
					m_pModel->Set_BlockAnim(false);
					m_bPlayDodgeCombo = true;
					m_bTrailSwitch = false;
					m_pPlayerWeapons[WEAPON_SPEAR - 1]->Active_Trail(false);
				}
			}
		}
		/////////////////////////////////////////////////////////



		// Look At Mouse Pos
		if (0.73f <= fAnimPlayRate)
		{
			LookAt_MousePos();
		}
		//
	}
	break;
	case SPEAR_ANIM_MAIN_ATK_COMBO_2:
	{
		m_bOnNavigation = true;
		// Turn On Weapon Trail
		if (m_pModel->Get_PlayRate() >= 0.661f)
		{
			m_bTrailSwitch = false;
			m_pPlayerWeapons[WEAPON_SPEAR - 1]->Active_Trail(false);
		}
		else if (false == m_bTrailSwitch && m_pModel->Get_PlayRate() >= 0.44f)
		{
			m_bTrailSwitch = true;
			m_pPlayerWeapons[WEAPON_SPEAR - 1]->Active_Trail(true);

			//m_vecTextureParticleDesc[0].vFixedPosition = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
			//m_vecTextureParticleDesc[0].vPowerDirection = _float3()

			m_vecTextureParticleDesc[0].FollowingTarget = m_pTransformCom;
			m_vecTextureParticleDesc[0].iFollowingDir = FollowingDir_Look;

			GetSingle(CUtilityMgr)->Create_TextureInstance(m_eNowSceneNum,m_vecTextureParticleDesc[0]);
		}
		//

		if (fAnimPlayRate <= 0.169)
		{
			m_fAnimSpeed = 6.5f;
		}
		else
		{
			m_fAnimSpeed = 1.5f;
		}

		if (true == m_pModel->Get_IsHavetoBlockAnimChange() && 0.254f <= fAnimPlayRate && 0.508f >= fAnimPlayRate)
		{
			_float MoveSpeed = g_pGameInstance->Easing_Return(TYPE_QuadOut, TYPE_QuarticIn, 0.f, 2.0f, fAnimPlayRate - 0.254f, 0.254f);
			m_pTransformCom->Move_Forward(fDeltaTime * MoveSpeed, m_pNavigationCom);
		}
		else if (true == m_pModel->Get_IsHavetoBlockAnimChange() && fAnimPlayRate && 0.9f < fAnimPlayRate)
		{
			m_fAnimSpeed = 1.5f;
			m_pModel->Set_BlockAnim(false);
			m_bAttackEnd = true;
			m_bTrailSwitch = false;
		}

		////////////////////Next Combo Check //////////////////////
		// 1) Check to Input Next Combo Command
		Check_NextComboCommand();

		// 2) Check to Next Combo Behavior
		if (true == m_bPlayNextCombo)
		{
			if (0.66f <= fAnimPlayRate)
			{
				if (true == Change_NextCombo())
				{
					m_bTrailSwitch = false;
				}
			}
			else if (0.6f <= fAnimPlayRate)
			{
				if (true == m_bReadyDodgeCombo)
				{
					m_pModel->Set_BlockAnim(false);
					m_bPlayDodgeCombo = true;
					m_bTrailSwitch = false;
				}
			}
		}
		/////////////////////////////////////////////////////////


		// Look At Mouse Pos
		if (0.203f <= fAnimPlayRate)
		{
			LookAt_MousePos();
		}
		//

		// Active CameraShake
		if (true == m_bActive_ActionCameraShake && 0.6f >= fAnimPlayRate && 0.54f <= fAnimPlayRate)
		{
			m_pMainCamera->Start_CameraShaking_Fov(58.f, 2.5f, 0.1f);
			m_bActive_ActionCameraShake = false;
		}
		//
	}
	break;
	case SPEAR_ANIM_MAIN_ATK_COMBO_2_JUMPATTACK:
	{
		m_bOnNavigation = true;
		// On MotionTrail
		if (0.13f < fAnimPlayRate)
		{
			m_bOn_MotionTrail = false;
		}
		else
		{
			m_bOn_MotionTrail = true;
		}


		// Turn On Weapon Trail
		if (m_pModel->Get_PlayRate() >= 0.257f)
		{
			m_bTrailSwitch = false;
			m_pPlayerWeapons[WEAPON_SPEAR - 1]->Active_Trail(false);
		}
		else if (false == m_bTrailSwitch && m_pModel->Get_PlayRate() > 0.057f)
		{
			m_bTrailSwitch = true;
			m_pPlayerWeapons[WEAPON_SPEAR - 1]->Active_Trail(true);
		}
		//

		m_fAnimSpeed = 1.8f;

		if (0.25f >= fAnimPlayRate)
		{
			_float MoveSpeed = g_pGameInstance->Easing(TYPE_SinInOut, 1.5f, 0.f, fAnimPlayRate, 0.25f);
			m_pTransformCom->Move_Forward(fDeltaTime * MoveSpeed, m_pNavigationCom);
		}
		else if (true == m_pModel->Get_IsHavetoBlockAnimChange() && fAnimPlayRate && 0.9f < fAnimPlayRate)
		{
			m_fAnimSpeed = 1.8f;
			m_pModel->Set_BlockAnim(false);
			m_bAttackEnd = true;
		}


		////////////////////Next Combo Check //////////////////////
		Check_NextComboCommand();

		if (true == m_bPlayNextCombo)
		{
			if (0.66f <= fAnimPlayRate)
			{
				m_bPlayJumpAttack = false;
				Change_NextCombo();
			}
			else if (0.6f <= fAnimPlayRate && 0.66f > fAnimPlayRate)
			{
				if (true == m_bReadyDodgeCombo)
				{
					m_pModel->Set_BlockAnim(false);
					m_bPlayDodgeCombo = true;
					m_bPlayJumpAttack = false;
				}
			}
		}
		/////////////////////////////////////////////////////////



		// Look At Mouse Pos
		if (0.f <= fAnimPlayRate)
		{
			LookAt_MousePos();
		}
		//

		// Active CameraShake
		if (true == m_bActive_ActionCameraShake && 0.3f >= fAnimPlayRate && 0.28f <= fAnimPlayRate)
		{
			m_pMainCamera->Start_CameraShaking_Fov(58.f, 2.5f, 0.1f);
			m_bActive_ActionCameraShake = false;
		}
		//
	}
	break;
	case SPEAR_ANIM_MAIN_ATK_COMBO_1_JUMPATTACK:
	{
		m_bOnNavigation = true;
		// On MotionTrail
		if (0.0f < fAnimPlayRate)
		{
			m_bOn_MotionTrail = false;
		}
		else
		{
			m_bOn_MotionTrail = true;
		}


		// Turn On Weapon Trail
		if (false == m_bTrailSwitch && fAnimPlayRate > 0.058f)
		{
			m_bTrailSwitch = true;
			m_pPlayerWeapons[WEAPON_SPEAR - 1]->Active_Trail(true);
		}
		//

		m_fAnimSpeed = 1.2f;

		if (0.25f >= fAnimPlayRate)
		{
			_float MoveSpeed = g_pGameInstance->Easing(TYPE_SinInOut, 1.5f, 0.f, fAnimPlayRate, 0.25f);
			m_pTransformCom->Move_Forward(fDeltaTime * MoveSpeed, m_pNavigationCom);
		}
		else if (true == m_pModel->Get_IsHavetoBlockAnimChange() && fAnimPlayRate && 0.9f < fAnimPlayRate)
		{
			m_fAnimSpeed = 1.5f;
			m_pModel->Set_BlockAnim(false);
			m_bAttackEnd = true;
			m_bTrailSwitch = false;
			m_pPlayerWeapons[WEAPON_SPEAR - 1]->Active_Trail(false);
		}


		////////////////////Next Combo Check //////////////////////
		Check_NextComboCommand();

		if (true == m_bPlayNextCombo)
		{
			if (0.66f <= fAnimPlayRate)
			{
				m_bPlayJumpAttack = false;
				if (true == Change_NextCombo())
				{
					m_bTrailSwitch = false;
					m_pPlayerWeapons[WEAPON_SPEAR - 1]->Active_Trail(false);
				}
			}
			else if (0.6f <= fAnimPlayRate && 0.66f > fAnimPlayRate)
			{
				if (true == m_bReadyDodgeCombo)
				{
					m_pModel->Set_BlockAnim(false);
					m_bPlayDodgeCombo = true;
					m_bPlayJumpAttack = false;
					m_bTrailSwitch = false;
					m_pPlayerWeapons[WEAPON_SPEAR - 1]->Active_Trail(false);
				}
			}
		}
		/////////////////////////////////////////////////////////

		// Look At Mouse Pos
		if (0.f <= fAnimPlayRate)
		{
			LookAt_MousePos();
		}
		//
	}
	break;
	case SPEAR_ANIM_MAIN_ATK_COMBO_0_JUMPATTACK:
	{
		m_bOnNavigation = true;
		// On MotionTrail
		if (0.0f < fAnimPlayRate)
		{
			m_bOn_MotionTrail = false;
		}
		else
		{
			m_bOn_MotionTrail = true;
		}


		// Turn On Weapon Trail
		if (false == m_bTrailSwitch && fAnimPlayRate > 0.058f)
		{
			m_bTrailSwitch = true;
			m_pPlayerWeapons[WEAPON_SPEAR - 1]->Active_Trail(true);
		}
		//

		m_fAnimSpeed = 1.5f;

		if (0.25f >= fAnimPlayRate)
		{
			_float MoveSpeed = g_pGameInstance->Easing(TYPE_SinInOut, 1.5f, 0.f, fAnimPlayRate, 0.25f);
			m_pTransformCom->Move_Forward(fDeltaTime * MoveSpeed, m_pNavigationCom);
		}
		else if (true == m_pModel->Get_IsHavetoBlockAnimChange() && fAnimPlayRate && 0.9f < fAnimPlayRate)
		{
			m_fAnimSpeed = 1.5f;
			m_pModel->Set_BlockAnim(false);
			m_bAttackEnd = true;
			m_bTrailSwitch = false;
			m_pPlayerWeapons[WEAPON_SPEAR - 1]->Active_Trail(false);
		}


		////////////////////Next Combo Check //////////////////////
		Check_NextComboCommand();

		if (true == m_bPlayNextCombo)
		{
			if (0.66f <= fAnimPlayRate)
			{
				m_bPlayJumpAttack = false;
				if (true == Change_NextCombo())
				{
					m_bTrailSwitch = false;
					m_pPlayerWeapons[WEAPON_SPEAR - 1]->Active_Trail(false);
				}
			}
			else if (0.6f <= fAnimPlayRate && 0.66f > fAnimPlayRate)
			{
				if (true == m_bReadyDodgeCombo)
				{
					m_pModel->Set_BlockAnim(false);
					m_bPlayDodgeCombo = true;
					m_bPlayJumpAttack = false;
					m_bTrailSwitch = false;
					m_pPlayerWeapons[WEAPON_SPEAR - 1]->Active_Trail(false);
				}
			}
		}
		/////////////////////////////////////////////////////////


		// Look At Mouse Pos
		if (0.f <= fAnimPlayRate)
		{
			LookAt_MousePos();
		}
		//
	}
	break;
	case SPEAR_ANIM_MAIN_ATK_COMBO_0_RECORVERY:
	case SPEAR_ANIM_MAIN_ATK_COMBO_1_RECORVERY:
	{
		m_bOnNavigation = true;
		m_pPlayerWeapons[WEAPON_SPEAR - 1]->Active_Trail(false);
		if (false == m_pModel->Get_IsAnimChanging())
		{
			m_bTrailSwitch = false;
			m_bAttackEnd = true;
		}
	}
	break;


	//// Power Attack ////
	case SPEAR_ANIM_POWER_ATK_COMBO_0:
	{
		m_bOnNavigation = true;
		if (fAnimPlayRate <= 0.409f)
		{
			m_fAnimSpeed = 2.f;
		}
		else
		{
			m_fAnimSpeed = 1.f;
		}

		if (true == m_pModel->Get_IsHavetoBlockAnimChange() && 0.409f <= fAnimPlayRate && 0.681f >= fAnimPlayRate)
		{
			_float MoveSpeed = g_pGameInstance->Easing_Return(TYPE_QuadOut, TYPE_QuarticIn, 0.f, 1.f, fAnimPlayRate - 0.409f, 0.272f);
			m_pTransformCom->Move_Forward(fDeltaTime * MoveSpeed, m_pNavigationCom);
		}
		else if (true == m_pModel->Get_IsHavetoBlockAnimChange() && fAnimPlayRate && 0.9f < fAnimPlayRate)
		{
			m_fAnimSpeed = 1.5f;
			m_pModel->Set_BlockAnim(false);
			m_bAttackEnd = true;
		}

		////////////////////Next Combo Check //////////////////////
		Check_NextComboCommand();

		if (true == m_bPlayNextCombo)
		{
			if (0.86f <= fAnimPlayRate)
			{
				Change_NextCombo();
			}
			else if (0.68f <= fAnimPlayRate)
			{
				if (false == m_bPlayNextCombo)
					return;

				if (true == m_bReadyDodgeCombo)
				{
					m_pModel->Set_BlockAnim(false);
					m_bPlayDodgeCombo = true;
				}
			}
		}
		/////////////////////////////////////////////////////////



		// Look At Mouse Pos 
		if (0.53f <= fAnimPlayRate)
		{
			LookAt_MousePos();
		}
		//
	}
	break;
	case SPEAR_ANIM_POWER_ATK_COMBO_1:
	{
		m_bOnNavigation = true;
		// Turn On Weapon Trail
		if (fAnimPlayRate > 0.564f)
		{
			m_bTrailSwitch = false;
			m_pPlayerWeapons[WEAPON_SPEAR - 1]->Active_Trail(false);
		}
		else if (false == m_bTrailSwitch && fAnimPlayRate > 0.358f)
		{
			m_bTrailSwitch = true;
			m_pPlayerWeapons[WEAPON_SPEAR - 1]->Active_Trail(true);

			CSpearWave::SPEARWAVEDESC tSpearWaveDesc;
			tSpearWaveDesc.fStartPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
			tSpearWaveDesc.fStartPos.y += 0.8f;
			tSpearWaveDesc.fLookDir = XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK));
			g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_PlayerSkill), TAG_OP(Prototype_PlayerSkill_SpearWave), &tSpearWaveDesc);
		}
		//

		if (fAnimPlayRate <= 0.358f)
		{
			m_fAnimSpeed = 3.f;
		}
		else
		{
			m_fAnimSpeed = 1.f;
		}

		if (true == m_pModel->Get_IsHavetoBlockAnimChange() && 0.358f <= fAnimPlayRate && 0.615f >= fAnimPlayRate)
		{
			_float MoveSpeed = g_pGameInstance->Easing_Return(TYPE_QuadOut, TYPE_QuarticIn, 0.f, 1.f, fAnimPlayRate - 0.358f, 0.257f);
			m_pTransformCom->Move_Forward(fDeltaTime * MoveSpeed, m_pNavigationCom);
		}
		else if (true == m_pModel->Get_IsHavetoBlockAnimChange() && fAnimPlayRate && 0.9f < fAnimPlayRate)
		{
			m_fAnimSpeed = 1.5f;
			m_pModel->Set_BlockAnim(false);
			m_bAttackEnd = true;
		}

		////////////////////Next Combo Check //////////////////////
		Check_NextComboCommand();

		if (true == m_bPlayNextCombo)
		{
			if (0.7f <= fAnimPlayRate)
			{
				Change_NextCombo();
			}
			else if (0.68f <= fAnimPlayRate)
			{
				if (false == m_bPlayNextCombo)
					return;

				if (true == m_bReadyDodgeCombo)
				{
					m_pModel->Set_BlockAnim(false);
					m_bPlayDodgeCombo = true;
				}
			}
		}
		/////////////////////////////////////////////////////////



		// Look At Mouse Pos 
		if (0.53f <= fAnimPlayRate)
		{
			LookAt_MousePos();
		}
		//
	}
	break;
	case SPEAR_ANIM_POWER_ATK_COMBO_2:
	{
		m_bOnNavigation = true;
		static_cast<CPlayerWeapon_Spear*>(m_pPlayerWeapons[WEAPON_SPEAR - 1])->Change_Pivot(CPlayerWeapon_Spear::ESpearPivot::SPEAR_PIVOT_TAKEDOWN);
		if (fAnimPlayRate <= 0.425f)
		{
			m_fAnimSpeed = 2.f;
		}
		else if (fAnimPlayRate >= 0.78f)
		{
			static_cast<CPlayerWeapon_Spear*>(m_pPlayerWeapons[WEAPON_SPEAR - 1])->Change_Pivot(CPlayerWeapon_Spear::ESpearPivot::SPEAR_PIVOT_NORMAL);
		}
		else
		{
			m_fAnimSpeed = 1.f;
		}

		if (true == m_pModel->Get_IsHavetoBlockAnimChange() && fAnimPlayRate && 0.9f < fAnimPlayRate)
		{
			m_fAnimSpeed = 1.5f;
			m_pModel->Set_BlockAnim(false);
			m_bAttackEnd = true;
		}

		////////////////////Next Combo Check //////////////////////
		Check_NextComboCommand();

		if (true == m_bPlayNextCombo)
		{
			if (0.86f <= fAnimPlayRate)
			{
				Change_NextCombo();
			}
			else if (0.68f <= fAnimPlayRate)
			{
				if (false == m_bPlayNextCombo)
					return;

				if (true == m_bReadyDodgeCombo)
				{
					static_cast<CPlayerWeapon_Spear*>(m_pPlayerWeapons[WEAPON_SPEAR - 1])->Change_Pivot(CPlayerWeapon_Spear::ESpearPivot::SPEAR_PIVOT_NORMAL);
					m_pModel->Set_BlockAnim(false);
					m_bPlayDodgeCombo = true;
				}
			}
		}
		/////////////////////////////////////////////////////////

		// Look At Mouse Pos
		if (0.53f <= fAnimPlayRate)
		{
			LookAt_MousePos();
		}
		//

		// Active CameraShake
		if (true == m_bActive_ActionCameraShake && 0.55f >= fAnimPlayRate && 0.53f <= fAnimPlayRate)
		{
			m_pMainCamera->Start_CameraShaking_Fov(58.f, 2.5f, 0.1f);
			m_bActive_ActionCameraShake = false;
		}
		//
	}
	break;
	case SPEAR_ANIM_POWER_ATK_COMBO_0_JUMPATTACK:
	{
		m_bOnNavigation = true;
		// On MotionTrail
		if (0.f < fAnimPlayRate)
		{
			m_bOn_MotionTrail = false;
		}
		else
		{
			m_bOn_MotionTrail = true;
		}


		m_fAnimSpeed = 1.f;

		if (0.41f >= fAnimPlayRate)
		{
			_float MoveSpeed = g_pGameInstance->Easing(TYPE_SinInOut, 1.5f, 0.f, fAnimPlayRate, 0.25f);
			m_pTransformCom->Move_Forward(fDeltaTime * MoveSpeed, m_pNavigationCom);
		}
		else if (true == m_pModel->Get_IsHavetoBlockAnimChange() && fAnimPlayRate && 0.9f < fAnimPlayRate)
		{
			m_fAnimSpeed = 1.f;
			m_pModel->Set_BlockAnim(false);
			m_bAttackEnd = true;
		}


		////////////////////Next Combo Check //////////////////////
		Check_NextComboCommand();

		if (true == m_bPlayNextCombo)
		{
			if (0.66f <= fAnimPlayRate)
			{
				m_bPlayJumpAttack = false;
				Change_NextCombo();
			}
			else if (0.6f <= fAnimPlayRate && 0.66f > fAnimPlayRate)
			{
				if (true == m_bReadyDodgeCombo)
				{
					m_pModel->Set_BlockAnim(false);
					m_bPlayDodgeCombo = true;
					m_bPlayJumpAttack = false;
				}
			}
		}
		/////////////////////////////////////////////////////////


		// Look At Mouse Pos
		if (0.f <= fAnimPlayRate)
		{
			LookAt_MousePos();
		}
		//
	}
	break;
	case SPEAR_ANIM_POWER_ATK_COMBO_1_JUMPATTACK:
	{
		m_bOnNavigation = true;
		// On MotionTrail
		if (0.1f < fAnimPlayRate)
		{
			m_bOn_MotionTrail = false;
		}
		else
		{
			m_bOn_MotionTrail = true;
		}


		// Turn On Weapon Trail
		if (fAnimPlayRate > 0.5f)
		{
			m_bTrailSwitch = false;
			m_pPlayerWeapons[WEAPON_SPEAR - 1]->Active_Trail(false);
		}
		else if (false == m_bTrailSwitch && fAnimPlayRate > 0.058f)
		{
			m_bTrailSwitch = true;
			m_pPlayerWeapons[WEAPON_SPEAR - 1]->Active_Trail(true);

			CSpearWave::SPEARWAVEDESC tSpearWaveDesc;
			tSpearWaveDesc.fStartPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
			tSpearWaveDesc.fStartPos.y += 0.8f;
			tSpearWaveDesc.fLookDir = XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK));
			g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_PlayerSkill), TAG_OP(Prototype_PlayerSkill_SpearWave), &tSpearWaveDesc);
		}
		//

		m_fAnimSpeed = 1.5f;

		if (0.25f >= fAnimPlayRate)
		{
			_float MoveSpeed = g_pGameInstance->Easing(TYPE_SinInOut, 1.5f, 0.f, fAnimPlayRate, 0.25f);
			m_pTransformCom->Move_Forward(fDeltaTime * MoveSpeed, m_pNavigationCom);
		}
		else if (true == m_pModel->Get_IsHavetoBlockAnimChange() && fAnimPlayRate && 0.9f < fAnimPlayRate)
		{
			m_fAnimSpeed = 1.5f;
			m_pModel->Set_BlockAnim(false);
			m_bAttackEnd = true;
		}


		////////////////////Next Combo Check //////////////////////
		Check_NextComboCommand();

		if (true == m_bPlayNextCombo)
		{
			if (0.66f <= fAnimPlayRate)
			{
				m_bPlayJumpAttack = false;
				Change_NextCombo();
			}
			else if (0.6f <= fAnimPlayRate && 0.66f > fAnimPlayRate)
			{
				if (true == m_bReadyDodgeCombo)
				{
					m_pModel->Set_BlockAnim(false);
					m_bPlayDodgeCombo = true;
					m_bPlayJumpAttack = false;
				}
			}
		}
		/////////////////////////////////////////////////////////


		// Look At Mouse Pos
		if (0.f <= fAnimPlayRate)
		{
			LookAt_MousePos();
		}
		//
	}
	break;
	case SPEAR_ANIM_POWER_ATK_COMBO_2_JUMPATTACK:
	{
		m_bOnNavigation = true;
		// On MotionTrail
		if (0.05f < fAnimPlayRate)
		{
			m_bOn_MotionTrail = false;
		}
		else
		{
			m_bOn_MotionTrail = true;
		}


		static_cast<CPlayerWeapon_Spear*>(m_pPlayerWeapons[WEAPON_SPEAR - 1])->Change_Pivot(CPlayerWeapon_Spear::ESpearPivot::SPEAR_PIVOT_TAKEDOWN);
		m_fAnimSpeed = 1.5f;

		if (0.25f >= fAnimPlayRate)
		{
			_float MoveSpeed = g_pGameInstance->Easing(TYPE_SinInOut, 1.5f, 0.f, fAnimPlayRate, 0.25f);
			m_pTransformCom->Move_Forward(fDeltaTime * MoveSpeed, m_pNavigationCom);
		}
		else if (true == m_pModel->Get_IsHavetoBlockAnimChange() && fAnimPlayRate && 0.9f < fAnimPlayRate)
		{
			m_fAnimSpeed = 1.5f;
			m_pModel->Set_BlockAnim(false);
			m_bAttackEnd = true;
		}


		////////////////////Next Combo Check //////////////////////
		Check_NextComboCommand();

		if (true == m_bPlayNextCombo)
		{
			if (0.66f <= fAnimPlayRate)
			{
				m_bPlayJumpAttack = false;
				Change_NextCombo();
			}
			else if (0.6f <= fAnimPlayRate && 0.66f > fAnimPlayRate)
			{
				if (true == m_bReadyDodgeCombo)
				{
					m_pModel->Set_BlockAnim(false);
					m_bPlayDodgeCombo = true;
					m_bPlayJumpAttack = false;
				}
			}
		}
		/////////////////////////////////////////////////////////


		// Look At Mouse Pos
		if (0.f <= fAnimPlayRate)
		{
			LookAt_MousePos();
		}
		//

		// Active CameraShake
		if (true == m_bActive_ActionCameraShake && 0.23f <= fAnimPlayRate)
		{
			m_pMainCamera->Start_CameraShaking_Fov(58.f, 2.5f, 0.1f);
			m_bActive_ActionCameraShake = false;
		}
		//
	}
	break;
	}
	m_bMainAttacking = true;
}

void CPlayer::Attack_Bow(_double fDeltaTime)
{
	// if main atk or power atk
	if (false == m_bPlayPowerAttack)
	{
		switch (m_eCurBowMainAtkState)
		{
		case BOWMAINATK_START:
		{
			m_fAnimSpeed = 1.f;

			if (0.307f < m_pModel->Get_PlayRate() && false == m_bAnimChangeSwitch)
			{
				m_bAnimChangeSwitch = true;

				CPlayerWeapon::PlayerWeaponDesc eWeaponDesc;
				eWeaponDesc.eAttachedDesc.Initialize_AttachedDesc(this, "skd_r_palm", _float3(1, 1, 1), _float3(0, 0, 0), _float3(0.f, 0.f, 0.0f));
				eWeaponDesc.eWeaponState = CPlayerWeapon::EWeaponState::STATE_EQUIP;
				g_pGameInstance->Add_GameObject_To_Layer(g_pGameInstance->Get_TargetSceneNum(), TAG_LAY(Layer_PlayerWeapon), TAG_OP(Prototype_PlayerWeapon_Arrow), &eWeaponDesc);
				CPlayerWeapon_Arrow* pBowArrow = static_cast<CPlayerWeapon_Arrow*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_PlayerWeapon)));
				pBowArrow->Set_State(CPlayerWeapon_Arrow::Arrow_State_NormalReady);
			}
			else if (0.96f <= m_pModel->Get_PlayRate())
			{
				m_eCurBowMainAtkState = BOWMAINATK_LOOP;
				m_pModel->Change_AnimIndex(BOW_ANIM_MAIN_ATK_LOOP, 0.1f, false);
				m_bAnimChangeSwitch = false;
				m_pTransformCom->Set_MoveSpeed(1.3f);
			}
			LookAt_MousePos();

			static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->PlayAnim_NormalAttack_Ready();
		}
			break;
		case BOWMAINATK_LOOP:
		{
			m_fAnimSpeed = 0.8f;

			// Cal Bow Range
			m_fChargingTime += (_float)g_fDeltaTime;
			if (m_fChargingTime > 1.f)
			{
				m_fArrowRange = 30.f;
			}
			else
			{
				m_pMainCamera->Start_CameraShaking_Thread(0.1f, 2.f - m_fChargingTime, 0.015f);
				m_fArrowRange = 12.f;
			}
			//

			if (false == m_bPressedMainAttackKey)
			{
				m_eCurBowMainAtkState = BOWMAINATK_SHOT;
				m_pModel->Change_AnimIndex(BOW_ANIM_MAIN_ATK_SHOT, 0.1f, false);

				CPlayerWeapon_Arrow* pBowArrow = static_cast<CPlayerWeapon_Arrow*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_PlayerWeapon)));
				pBowArrow->Set_State(CPlayerWeapon_Arrow::Arrow_State_NormalShot, m_fArrowRange);
				pBowArrow->Active_Trail(false);
				pBowArrow->LookAtDir(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK));
			}
			else
			{
				Move_NotTurn(m_eInputDir, fDeltaTime);

				if (MOVDIR_END == m_eInputDir)
				{
					m_pModel->Change_AnimIndex(BOW_ANIM_MAIN_ATK_LOOP, 0.1f, true);
				}
				else
				{
					_Vector vMyLook = XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK));
					_Vector vDot = XMVector3Dot(m_fMovDir.XMVector(), vMyLook);
					_float fDot = XMVectorGetX(vDot);

					if (fDot > 0.5f)
					{
						m_pModel->Change_AnimIndex(BOW_ANIM_MAIN_ATK_LOOP_RUN_F, 0.1f, true);
					}
					else if (fDot < -0.5f)
					{
						m_pModel->Change_AnimIndex(BOW_ANIM_MAIN_ATK_LOOP_RUN_B, 0.1f, true);
					}
					else
					{
						_Vector vMyRight = XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_RIGHT));
						vDot = XMVector3Dot(m_fMovDir.XMVector(), vMyRight);
						fDot = XMVectorGetX(vDot);
						if (fDot > 0)
						{
							m_pModel->Change_AnimIndex(BOW_ANIM_MAIN_ATK_LOOP_RUN_R, 0.1f, true);
						}
						else
						{
							m_pModel->Change_AnimIndex(BOW_ANIM_MAIN_ATK_LOOP_RUN_L, 0.1f, true);
						}
					}
				}
			}
			LookAt_MousePos();
		}
			break;
		case BOWMAINATK_SHOT:
		{
			CPlayerWeapon_Arrow* pBowArrow = static_cast<CPlayerWeapon_Arrow*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_PlayerWeapon)));
			pBowArrow->Active_Trail(true);
			m_fChargingTime = 0.f;
			m_fAnimSpeed = 1.f;
			static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->PlayAnim_NormalAttack_Shot();
			if (0.9f <= m_pModel->Get_PlayRate())
			{
				m_eCurBowMainAtkState = BOWMAINATK_START;
				Set_State_IdleStart(fDeltaTime);
				m_bAttackEnd = true;
				m_pTransformCom->Set_MoveSpeed(5.f);
			}
		}
			break;
		}
	}
	else
	{
		_float fAnimPlayRate = (_float)m_pModel->Get_PlayRate();

		switch (m_pModel->Get_NowAnimIndex())
		{
		case BOW_ANIM_POWER_COMBO_0:
		{
			// On MotionTrail
			if (0.1f > fAnimPlayRate && true == m_bPlayJumpAttack)
			{
				m_bOn_MotionTrail = true;
			}
			else
			{
				m_bOn_MotionTrail = false;
			}

			m_fAnimSpeed = 1.5f;

			// Bow Anim Control
			if (false == m_bAnimChangeSwitch && 0.1 > fAnimPlayRate)
			{
				static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->PlayAnim_UtilityAttack_Loop();
				static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->Set_AnimSpeed(5.f);
				m_bAnimChangeSwitch = true;
			}
			else if (true == m_bAnimChangeSwitch && 0.2 < fAnimPlayRate)
			{
				static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->PlayAnim_UtilityAttack_Shot();
				m_bAnimChangeSwitch = false;

				// Shot Arrow //
				CPlayerWeapon::PlayerWeaponDesc eWeaponDesc;
				eWeaponDesc.eAttachedDesc.Initialize_AttachedDesc(this, "skd_r_palm", _float3(1, 1, 1), _float3(0, 0, 0), _float3(0.f, 0.f, 0.0f));
				eWeaponDesc.eWeaponState = CPlayerWeapon::EWeaponState::STATE_EQUIP;
				g_pGameInstance->Add_GameObject_To_Layer(g_pGameInstance->Get_TargetSceneNum(), TAG_LAY(Layer_PlayerWeapon), TAG_OP(Prototype_PlayerWeapon_Arrow), &eWeaponDesc);
				CPlayerWeapon_Arrow* pBowArrow = static_cast<CPlayerWeapon_Arrow*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_PlayerWeapon)));
				pBowArrow->Set_State_PowerShot_Combo_0(m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK), 0);

				eWeaponDesc.eAttachedDesc.Initialize_AttachedDesc(this, "skd_r_palm", _float3(1, 1, 1), _float3(0, 0, 0), _float3(0.f, 0.f, 0.0f));
				eWeaponDesc.eWeaponState = CPlayerWeapon::EWeaponState::STATE_EQUIP;
				g_pGameInstance->Add_GameObject_To_Layer(g_pGameInstance->Get_TargetSceneNum(), TAG_LAY(Layer_PlayerWeapon), TAG_OP(Prototype_PlayerWeapon_Arrow), &eWeaponDesc);
				pBowArrow = static_cast<CPlayerWeapon_Arrow*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_PlayerWeapon)));
				pBowArrow->Set_State_PowerShot_Combo_0(m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK), 1);

				eWeaponDesc.eAttachedDesc.Initialize_AttachedDesc(this, "skd_r_palm", _float3(1, 1, 1), _float3(0, 0, 0), _float3(0.f, 0.f, 0.0f));
				eWeaponDesc.eWeaponState = CPlayerWeapon::EWeaponState::STATE_EQUIP;
				g_pGameInstance->Add_GameObject_To_Layer(g_pGameInstance->Get_TargetSceneNum(), TAG_LAY(Layer_PlayerWeapon), TAG_OP(Prototype_PlayerWeapon_Arrow), &eWeaponDesc);
				pBowArrow = static_cast<CPlayerWeapon_Arrow*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_PlayerWeapon)));
				pBowArrow->Set_State_PowerShot_Combo_0(m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK), 2);
				//

			}
			else if (0.3 < fAnimPlayRate)
			{
				static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->PlayAnim_Idle();
			}
			//


			if (0.277f >= fAnimPlayRate)
			{
				_float MoveSpeed = g_pGameInstance->Easing_Return(TYPE_QuadOut, TYPE_QuarticIn, 0.f, 1.f, fAnimPlayRate, 0.277f);
				m_pTransformCom->Move_Forward(fDeltaTime * MoveSpeed);
			}
			else if (0.277f < fAnimPlayRate)
			{
				m_fAnimSpeed = 3.f;
				if (0.9f < fAnimPlayRate)
				{
					m_fAnimSpeed = 1.5f;
					m_pModel->Set_BlockAnim(false);
					m_bAttackEnd = true;
					static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->PlayAnim_Idle();
					m_bAnimChangeSwitch = false;
					m_bActionSwitch = false;
				}
			}

			////////////////////Next Combo Check //////////////////////
			Check_NextComboCommand();

			if (true == m_bPlayNextCombo)
			{
				if (0.86f <= fAnimPlayRate)
				{
					m_bPlayJumpAttack = false;
					if (true == Change_NextCombo())
					{
						m_bAnimChangeSwitch = false;
						m_bActionSwitch = false;
					}
				}
				else if (0.68f <= fAnimPlayRate)
				{
					if (false == m_bPlayNextCombo)
						return;

					if (true == m_bReadyDodgeCombo)
					{
						m_pModel->Set_BlockAnim(false);
						m_bPlayDodgeCombo = true;
						m_bPlayJumpAttack = false;
						static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->PlayAnim_Idle();
						m_bAnimChangeSwitch = false;
						m_bActionSwitch = false;
					}
				}
			}
			/////////////////////////////////////////////////////////



			// Look At Mouse Pos 
			if (0.2f >= fAnimPlayRate)
			{
				LookAt_MousePos();
			}
			//

		}
		break;
		case BOW_ANIM_POWER_COMBO_1:
		case BOW_ANIM_POWER_COMBO_1_JUMP:
		{
			if (0.f > fAnimPlayRate && true == m_bPlayJumpAttack)
			{
				m_bOn_MotionTrail = true;
			}
			else
			{
				m_bOn_MotionTrail = false;
			}


			m_fAnimSpeed = 2.f;


			// Bow Anim Control
			if (false == m_bAnimChangeSwitch && 0.387f < fAnimPlayRate)
			{
				static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->PlayAnim_UtilityAttack_Ready();
				static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->Set_AnimSpeed(6.f);
				m_bAnimChangeSwitch = true;
			}
			else if (true == m_bAnimChangeSwitch && 0.806f < fAnimPlayRate)
			{
				static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->PlayAnim_UtilityAttack_Shot();

				CPlayerWeapon::PlayerWeaponDesc eWeaponDesc;

				if (false == m_bActionSwitch)
				{
					m_bActionSwitch = true;
					eWeaponDesc.eAttachedDesc.Initialize_AttachedDesc(this, "skd_r_palm", _float3(1, 1, 1), _float3(0, 0, 0), _float3(0.f, 0.f, 0.0f));
					eWeaponDesc.eWeaponState = CPlayerWeapon::EWeaponState::STATE_EQUIP;
					g_pGameInstance->Add_GameObject_To_Layer(g_pGameInstance->Get_TargetSceneNum(), TAG_LAY(Layer_PlayerWeapon), TAG_OP(Prototype_PlayerWeapon_Arrow), &eWeaponDesc);
					CPlayerWeapon_Arrow* pBowArrow = static_cast<CPlayerWeapon_Arrow*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_PlayerWeapon)));
					pBowArrow->Set_State_PowerShot_Combo_1(m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK));
				}
			}
			else if (0.806f < fAnimPlayRate)
			{
				static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->PlayAnim_Idle();
				m_bActionSwitch = false;
			}
			//


			if (0.277f < fAnimPlayRate)
			{
				m_fAnimSpeed = 2.5f;
				if (0.9f < fAnimPlayRate)
				{
					m_fAnimSpeed = 1.f;
					m_pModel->Set_BlockAnim(false);
					m_bAttackEnd = true;
					m_bAnimChangeSwitch = false;
					static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->PlayAnim_Idle();
					m_bActionSwitch = false;
					return;
				}
			}

			//////////////////// Next Combo Check //////////////////////
			Check_NextComboCommand();

			if (true == m_bPlayNextCombo)
			{
				if (0.85f <= fAnimPlayRate)
				{
					m_bPlayJumpAttack = false;
					if (true == Change_NextCombo())
					{
						m_bActionSwitch = false;
					}
				}
				else if (0.8f <= fAnimPlayRate)
				{
					if (false == m_bPlayNextCombo)
						return;

					if (true == m_bReadyDodgeCombo)
					{
						m_pModel->Set_BlockAnim(false);
						m_bPlayDodgeCombo = true;
						m_bPlayJumpAttack = false;
						m_bAnimChangeSwitch = false;
						m_bActionSwitch = false;
						static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->PlayAnim_Idle();
					}
				}
			}
			/////////////////////////////////////////////////////////



			// Look At Mouse Pos 
			if (0.555f >= fAnimPlayRate)
			{
				LookAt_MousePos();
			}
			//
		}
		break;
		case BOW_ANIM_POWER_COMBO_2:
		{
			m_fAnimSpeed = 1.3f;


			// Bow Anim Control
			if (true == m_bAnimChangeSwitch && 0.625 < fAnimPlayRate)
			{
				m_bAnimChangeSwitch = false;
				static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->PlayAnim_UtilityAttack_Shot();
				static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->Set_AnimSpeed(5.f);

				if (false == m_bActionSwitch)
				{
					CPlayerWeapon::PlayerWeaponDesc eWeaponDesc;
					m_bActionSwitch = true;
					eWeaponDesc.eAttachedDesc.Initialize_AttachedDesc(this, "skd_r_palm", _float3(1, 1, 1), _float3(0, 0, 0), _float3(0.f, 0.f, 0.0f));
					eWeaponDesc.eWeaponState = CPlayerWeapon::EWeaponState::STATE_EQUIP;
					g_pGameInstance->Add_GameObject_To_Layer(g_pGameInstance->Get_TargetSceneNum(), TAG_LAY(Layer_PlayerWeapon), TAG_OP(Prototype_PlayerWeapon_Arrow), &eWeaponDesc);
					CPlayerWeapon_Arrow* pBowArrow = static_cast<CPlayerWeapon_Arrow*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_PlayerWeapon)));
					pBowArrow->Set_State_PowerShot_Combo_2(XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK)), 10.f);
				}
			}
			else if (false == m_bAnimChangeSwitch && 0.6 < fAnimPlayRate)
			{
				m_bAnimChangeSwitch = true;
				static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->PlayAnim_UtilityAttack_Ready();
				static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->Set_AnimSpeed(5.f);
			}
			else if (true == m_bAnimChangeSwitch && 0.525 < fAnimPlayRate)
			{
				m_bAnimChangeSwitch = false;
				static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->PlayAnim_UtilityAttack_Shot();
				static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->Set_AnimSpeed(5.f);

				if (true == m_bActionSwitch)
				{
					CPlayerWeapon::PlayerWeaponDesc eWeaponDesc;
					m_bActionSwitch = false;
					eWeaponDesc.eAttachedDesc.Initialize_AttachedDesc(this, "skd_r_palm", _float3(1, 1, 1), _float3(0, 0, 0), _float3(0.f, 0.f, 0.0f));
					eWeaponDesc.eWeaponState = CPlayerWeapon::EWeaponState::STATE_EQUIP;
					g_pGameInstance->Add_GameObject_To_Layer(g_pGameInstance->Get_TargetSceneNum(), TAG_LAY(Layer_PlayerWeapon), TAG_OP(Prototype_PlayerWeapon_Arrow), &eWeaponDesc);
					CPlayerWeapon_Arrow* pBowArrow = static_cast<CPlayerWeapon_Arrow*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_PlayerWeapon)));
					pBowArrow->Set_State_PowerShot_Combo_2(XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK)), 15.f);
				}
			}
			else if (false == m_bAnimChangeSwitch && 0.5 < fAnimPlayRate)
			{
				m_bAnimChangeSwitch = true;
				static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->PlayAnim_UtilityAttack_Ready();
				static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->Set_AnimSpeed(5.f);
			}
			else if (true == m_bAnimChangeSwitch && 0.425 < fAnimPlayRate)
			{
				m_bAnimChangeSwitch = false;
				static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->PlayAnim_UtilityAttack_Shot();
				static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->Set_AnimSpeed(5.f);

				if (false == m_bActionSwitch)
				{
					CPlayerWeapon::PlayerWeaponDesc eWeaponDesc;
					m_bActionSwitch = true;
					eWeaponDesc.eAttachedDesc.Initialize_AttachedDesc(this, "skd_r_palm", _float3(1, 1, 1), _float3(0, 0, 0), _float3(0.f, 0.f, 0.0f));
					eWeaponDesc.eWeaponState = CPlayerWeapon::EWeaponState::STATE_EQUIP;
					g_pGameInstance->Add_GameObject_To_Layer(g_pGameInstance->Get_TargetSceneNum(), TAG_LAY(Layer_PlayerWeapon), TAG_OP(Prototype_PlayerWeapon_Arrow), &eWeaponDesc);
					CPlayerWeapon_Arrow* pBowArrow = static_cast<CPlayerWeapon_Arrow*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_PlayerWeapon)));
					pBowArrow->Set_State_PowerShot_Combo_2(XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK)), 20.f);
				}
			}
			else if (false == m_bAnimChangeSwitch && 0.375 < fAnimPlayRate)
			{
				static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->PlayAnim_UtilityAttack_Ready();
				static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->Set_AnimSpeed(5.f);
				m_bAnimChangeSwitch = true;
			}
			//


			if (0.277f >= fAnimPlayRate)
			{
				_float MoveSpeed = g_pGameInstance->Easing_Return(TYPE_QuadOut, TYPE_QuarticIn, 0.f, 1.f, fAnimPlayRate, 0.277f);
				m_pTransformCom->Move_Forward(fDeltaTime * MoveSpeed);
			}
			else if (0.277f < fAnimPlayRate)
			{
				if (0.9f < fAnimPlayRate)
				{
					m_fAnimSpeed = 1.5f;
					m_pModel->Set_BlockAnim(false);
					m_bAttackEnd = true;
					m_bAnimChangeSwitch = false;
					m_bActionSwitch = false;
				}
			}

			////////////////////Next Combo Check //////////////////////
			Check_NextComboCommand();

			if (true == m_bPlayNextCombo)
			{
				if (0.86f <= fAnimPlayRate)
				{
					m_bPlayJumpAttack = false;
					if (true == Change_NextCombo())
					{
						m_bActionSwitch = false;
					}
				}
				else if (0.68f <= fAnimPlayRate)
				{
					if (false == m_bPlayNextCombo)
						return;

					if (true == m_bReadyDodgeCombo)
					{
						m_pModel->Set_BlockAnim(false);
						m_bPlayDodgeCombo = true;
						m_bPlayJumpAttack = false;
						m_bAnimChangeSwitch = false;
						m_bActionSwitch = false;
					}
				}
			}
			/////////////////////////////////////////////////////////



			// Look At Mouse Pos 
			if (0.555f >= fAnimPlayRate)
			{
				LookAt_MousePos();
			}
			//
		}
		break;
		case BOW_ANIM_POWER_COMBO_2_JUMP:
		{
			if (0.2f > fAnimPlayRate)
			{
				m_bOn_MotionTrail = true;
			}
			else
			{
				m_bOn_MotionTrail = false;
			}

			if (false == m_bActionSwitch && 0.12f <= fAnimPlayRate)
			{
				CPlayerWeapon::PlayerWeaponDesc eWeaponDesc;
				m_bActionSwitch = true;
				eWeaponDesc.eAttachedDesc.Initialize_AttachedDesc(this, "skd_r_palm", _float3(1, 1, 1), _float3(0, 0, 0), _float3(0.f, 0.f, 0.0f));
				eWeaponDesc.eWeaponState = CPlayerWeapon::EWeaponState::STATE_EQUIP;
				g_pGameInstance->Add_GameObject_To_Layer(g_pGameInstance->Get_TargetSceneNum(), TAG_LAY(Layer_PlayerWeapon), TAG_OP(Prototype_PlayerWeapon_Arrow), &eWeaponDesc);
				CPlayerWeapon_Arrow* pBowArrow = static_cast<CPlayerWeapon_Arrow*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_PlayerWeapon)));
				pBowArrow->Set_State_PowerShot_Combo_2(XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK)), 10.f);

				g_pGameInstance->Add_GameObject_To_Layer(g_pGameInstance->Get_TargetSceneNum(), TAG_LAY(Layer_PlayerWeapon), TAG_OP(Prototype_PlayerWeapon_Arrow), &eWeaponDesc);
				pBowArrow = static_cast<CPlayerWeapon_Arrow*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_PlayerWeapon)));
				pBowArrow->Set_State_PowerShot_Combo_2(XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK)), 15.f);

				g_pGameInstance->Add_GameObject_To_Layer(g_pGameInstance->Get_TargetSceneNum(), TAG_LAY(Layer_PlayerWeapon), TAG_OP(Prototype_PlayerWeapon_Arrow), &eWeaponDesc);
				pBowArrow = static_cast<CPlayerWeapon_Arrow*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_PlayerWeapon)));
				pBowArrow->Set_State_PowerShot_Combo_2(XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK)), 20.f);
			}


			m_fAnimSpeed = 1.f;


			// Bow Anim Control
			if (true == m_bAnimChangeSwitch && 0.625 < fAnimPlayRate)
			{
				m_bAnimChangeSwitch = false;
				static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->PlayAnim_UtilityAttack_Shot();
				static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->Set_AnimSpeed(5.f);
			}
			else if (false == m_bAnimChangeSwitch && 0.6 < fAnimPlayRate)
			{
				m_bAnimChangeSwitch = true;
				static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->PlayAnim_UtilityAttack_Ready();
				static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->Set_AnimSpeed(5.f);
			}
			else if (true == m_bAnimChangeSwitch && 0.525 < fAnimPlayRate)
			{
				m_bAnimChangeSwitch = false;
				static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->PlayAnim_UtilityAttack_Shot();
				static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->Set_AnimSpeed(5.f);
			}
			else if (false == m_bAnimChangeSwitch && 0.5 < fAnimPlayRate)
			{
				m_bAnimChangeSwitch = true;
				static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->PlayAnim_UtilityAttack_Ready();
				static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->Set_AnimSpeed(5.f);
			}
			else if (true == m_bAnimChangeSwitch && 0.425 < fAnimPlayRate)
			{
				m_bAnimChangeSwitch = false;
				static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->PlayAnim_UtilityAttack_Shot();
				static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->Set_AnimSpeed(5.f);
			}
			else if (false == m_bAnimChangeSwitch && 0.375 < fAnimPlayRate)
			{
				static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->PlayAnim_UtilityAttack_Ready();
				static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->Set_AnimSpeed(5.f);
				m_bAnimChangeSwitch = true;
			}
			//


			if (0.277f >= fAnimPlayRate)
			{
				_float MoveSpeed = g_pGameInstance->Easing_Return(TYPE_QuadOut, TYPE_QuarticIn, 0.f, 1.f, fAnimPlayRate, 0.277f);
				m_pTransformCom->Move_Forward(fDeltaTime * MoveSpeed);
			}
			else if (0.277f < fAnimPlayRate)
			{
				if (0.9f < fAnimPlayRate)
				{
					m_fAnimSpeed = 1.f;
					m_pModel->Set_BlockAnim(false);
					m_bAttackEnd = true;
					m_bAnimChangeSwitch = false;
					m_bActionSwitch = false;
				}
			}

			////////////////////Next Combo Check //////////////////////
			Check_NextComboCommand();

			if (true == m_bPlayNextCombo)
			{
				if (0.86f <= fAnimPlayRate)
				{
					m_bPlayJumpAttack = false;
					if (true == Change_NextCombo())
					{
						m_bActionSwitch = false;
					}
				}
				else if (0.68f <= fAnimPlayRate)
				{
					if (false == m_bPlayNextCombo)
						return;

					if (true == m_bReadyDodgeCombo)
					{
						m_pModel->Set_BlockAnim(false);
						m_bPlayDodgeCombo = true;
						m_bPlayJumpAttack = false;
						m_bAnimChangeSwitch = false;
						m_bActionSwitch = false;
					}
				}
			}
			/////////////////////////////////////////////////////////



			//// Look At Mouse Pos 
			if (0.12f > fAnimPlayRate)
			{
				LookAt_MousePos();
			}
			////
		}
		break;
		}
	}

}

void CPlayer::Attack_Sword(_double fDeltaTime)
{
	_float fAnimPlayRate = (_float)m_pModel->Get_PlayRate();

	switch (m_pModel->Get_NowAnimIndex())
	{
		//// Main Attack ////
	case SWORD_ANIM_MAIN_ATK_COMBO_0:
	{
		//// Turn On Weapon Trail
		if (true == m_bTrailSwitch && m_pModel->Get_PlayRate() >= 0.6f)
		{
			m_pPlayerWeapons[WEAPON_SWORD - 1]->Active_Trail(false);
		}
		else if (false == m_bTrailSwitch && m_pModel->Get_PlayRate() >= 0.339f)
		{
			m_bTrailSwitch = true;
			m_pPlayerWeapons[WEAPON_SWORD - 1]->Active_Trail(true);
		}
		////

		m_fAnimSpeed = 1.5f;

		if (0.9f < fAnimPlayRate)
		{
			m_fAnimSpeed = 1.5f;
			m_pModel->Set_BlockAnim(false);
			m_bAttackEnd = true;
			m_bTrailSwitch = false;
			m_pPlayerWeapons[WEAPON_SWORD - 1]->Active_Trail(false);
		}


		////////////////////Next Combo Check //////////////////////
		// 1) Check to Input Next Combo Command
		Check_NextComboCommand();

		// 2) Check to Next Combo Behavior
		if (true == m_bPlayNextCombo)
		{
			if (0.57f <= fAnimPlayRate)
			{
				if (true == Change_NextCombo())
				{
					m_bTrailSwitch = false;
					m_pPlayerWeapons[WEAPON_SWORD - 1]->Active_Trail(false);
				}
			}
			else if (0.5f <= fAnimPlayRate)
			{
				if (false == m_bPlayNextCombo)
					return;

				if (true == m_bReadyDodgeCombo)
				{
					m_pModel->Set_BlockAnim(false);
					m_bPlayDodgeCombo = true;
					m_bTrailSwitch = false;
					m_pPlayerWeapons[WEAPON_SWORD - 1]->Active_Trail(false);
				}
			}
		}

		/////////////////////////////////////////////////////////



		// Look At Mouse Pos
		if (0.464f >= fAnimPlayRate)
		{
			LookAt_MousePos();
		}
		//
	}
	break;
	case SWORD_ANIM_MAIN_ATK_COMBO_1:
	{
		//// Turn On Weapon Trail
		if (true == m_bTrailSwitch && m_pModel->Get_PlayRate() >= 0.694f)
		{
			m_pPlayerWeapons[WEAPON_SWORD - 1]->Active_Trail(false);
		}
		else if (false == m_bTrailSwitch && m_pModel->Get_PlayRate() >= 0.472f)
		{
			m_bTrailSwitch = true;
			m_pPlayerWeapons[WEAPON_SWORD - 1]->Active_Trail(true);
		}
		////

		m_fAnimSpeed = 1.f;

		if (0.16f >= fAnimPlayRate)
		{
			m_fAnimSpeed = 1.5f;
		}
		else if (0.16f < fAnimPlayRate && 0.26f >= fAnimPlayRate)
		{
			m_fAnimSpeed = 1.3f;
		}
		else
		{
			m_fAnimSpeed = 1.2f;
		}

		if (0.f < fAnimPlayRate && 0.47f >= fAnimPlayRate)
		{
			_float MoveSpeed = g_pGameInstance->Easing_Return(TYPE_QuadOut, TYPE_QuarticIn, 0.f, 1.5f, fAnimPlayRate, 0.47f);
			m_pTransformCom->Move_Forward(fDeltaTime * MoveSpeed);
		}
		else if (0.9f < fAnimPlayRate)
		{
			m_fAnimSpeed = 1.5f;
			m_pModel->Set_BlockAnim(false);
			m_bAttackEnd = true;
			m_bTrailSwitch = false;
			m_pPlayerWeapons[WEAPON_SWORD - 1]->Active_Trail(false);
		}


		////////////////////Next Combo Check //////////////////////
		// 1) Check to Input Next Combo Command
		Check_NextComboCommand();

		// 2) Check to Next Combo Behavior
		if (true == m_bPlayNextCombo)
		{
			if (0.643f <= fAnimPlayRate)
			{
				if (true == Change_NextCombo())
				{
					m_bTrailSwitch = false;
					m_pPlayerWeapons[WEAPON_SWORD - 1]->Active_Trail(false);
				}
			}
			else if (0.55f <= fAnimPlayRate)
			{
				if (true == m_bReadyDodgeCombo)
				{
					m_pModel->Set_BlockAnim(false);
					m_bPlayDodgeCombo = true;
					m_bTrailSwitch = false;
					m_pPlayerWeapons[WEAPON_SWORD - 1]->Active_Trail(false);
				}
			}
		}
		/////////////////////////////////////////////////////////



		// Look At Mouse Pos
		if (0.5f >= fAnimPlayRate)
		{
			LookAt_MousePos();
		}
		//
	}
	break;
	case SWORD_ANIM_MAIN_ATK_COMBO_2:
	{
		//// Turn On Weapon Trail
		if (true == m_bTrailSwitch && m_pModel->Get_PlayRate() >= 0.512f)
		{
			m_pPlayerWeapons[WEAPON_SWORD - 1]->Active_Trail(false);
		}
		else if (false == m_bTrailSwitch && m_pModel->Get_PlayRate() >= 0.365f)
		{
			m_bTrailSwitch = true;
			m_pPlayerWeapons[WEAPON_SWORD - 1]->Active_Trail(true);
		}
		////

		m_fAnimSpeed = 1.f;

		if (0.16f >= fAnimPlayRate)
		{
			m_fAnimSpeed = 1.5f;
		}
		else if (0.16f < fAnimPlayRate && 0.26f >= fAnimPlayRate)
		{
			m_fAnimSpeed = 1.3f;
		}
		else
		{
			m_fAnimSpeed = 1.8f;
		}

		if (0.f < fAnimPlayRate && 0.47f >= fAnimPlayRate)
		{
			_float MoveSpeed = g_pGameInstance->Easing_Return(TYPE_QuadOut, TYPE_QuarticIn, 0.f, 1.6f, fAnimPlayRate, 0.47f);
			m_pTransformCom->Move_Forward(fDeltaTime * MoveSpeed);
		}
		else if (0.9f < fAnimPlayRate)
		{
			m_fAnimSpeed = 1.5f;
			m_pModel->Set_BlockAnim(false);
			m_bAttackEnd = true;
			m_bTrailSwitch = false;
			m_pPlayerWeapons[WEAPON_SWORD - 1]->Active_Trail(false);
		}


		////////////////////Next Combo Check //////////////////////
		// 1) Check to Input Next Combo Command
		Check_NextComboCommand();

		// 2) Check to Next Combo Behavior
		if (true == m_bPlayNextCombo)
		{
			if (0.66f <= fAnimPlayRate)
			{
				if (true == Change_NextCombo())
				{
					m_bTrailSwitch = false;
					m_pPlayerWeapons[WEAPON_SWORD - 1]->Active_Trail(false);
				}
			}
			else if (0.6f <= fAnimPlayRate)
			{
				if (true == m_bReadyDodgeCombo)
				{
					m_pModel->Set_BlockAnim(false);
					m_bPlayDodgeCombo = true;
					m_bTrailSwitch = false;
					m_pPlayerWeapons[WEAPON_SWORD - 1]->Active_Trail(false);
				}
			}
		}
		/////////////////////////////////////////////////////////


		// Look At Mouse Pos
		if (0.5f >= fAnimPlayRate)
		{
			LookAt_MousePos();
		}
		//

		// Active CameraShake
		if (true == m_bActive_ActionCameraShake && 0.45f <= fAnimPlayRate)
		{
			m_pMainCamera->Start_CameraShaking_Fov(57.f, 1.f, 0.1f);
			m_bActive_ActionCameraShake = false;
		}
		//
	}
	break;
	case SWORD_ANIM_MAIN_ATK_COMBO_2_JUMPATTACK:
	{
		if (0.2f > fAnimPlayRate)
		{
			m_bOn_MotionTrail = true;
		}
		else
		{
			m_bOn_MotionTrail = false;
		}

		//// Turn On Weapon Trail
		if (true == m_bTrailSwitch && m_pModel->Get_PlayRate() >= 0.357f)
		{
			m_pPlayerWeapons[WEAPON_SWORD - 1]->Active_Trail(false);
		}
		else if (false == m_bTrailSwitch && m_pModel->Get_PlayRate() > 0.107f)
		{
			m_bTrailSwitch = true;
			m_pPlayerWeapons[WEAPON_SWORD - 1]->Active_Trail(true);
		}
		////

		m_fAnimSpeed = 1.5f;

		if (0.25f >= fAnimPlayRate)
		{
			_float MoveSpeed = g_pGameInstance->Easing(TYPE_SinInOut, 1.5f, 0.f, fAnimPlayRate, 0.25f);
			m_pTransformCom->Move_Forward(fDeltaTime * MoveSpeed);
		}
		else if (true == m_pModel->Get_IsHavetoBlockAnimChange() && fAnimPlayRate && 0.9f < fAnimPlayRate)
		{
			m_fAnimSpeed = 1.5f;
			m_pModel->Set_BlockAnim(false);
			m_bAttackEnd = true;
			m_bTrailSwitch = false;
			m_pPlayerWeapons[WEAPON_SWORD - 1]->Active_Trail(false);
		}


		////////////////////Next Combo Check //////////////////////
		Check_NextComboCommand();

		if (true == m_bPlayNextCombo)
		{
			if (0.85f <= fAnimPlayRate)
			{
				m_bPlayJumpAttack = false;
				if (true == Change_NextCombo())
				{
					m_bTrailSwitch = false;
					m_pPlayerWeapons[WEAPON_SWORD - 1]->Active_Trail(false);
				}
			}
			else if (0.8f <= fAnimPlayRate)
			{
				if (true == m_bReadyDodgeCombo)
				{
					m_pModel->Set_BlockAnim(false);
					m_bPlayDodgeCombo = true;
					m_bPlayJumpAttack = false;
					m_bTrailSwitch = false;
					m_pPlayerWeapons[WEAPON_SWORD - 1]->Active_Trail(false);
				}
			}
		}
		/////////////////////////////////////////////////////////


		// Look At Mouse Pos
		if (0.5f >= fAnimPlayRate)
		{
			LookAt_MousePos();
		}
		//

		// Active CameraShake
		if (true == m_bActive_ActionCameraShake && 0.25f <= fAnimPlayRate)
		{
			m_pMainCamera->Start_CameraShaking_Fov(57.f, 1.f, 0.1f);
			m_bActive_ActionCameraShake = false;
		}
		//
	}
	break;
	case SWORD_ANIM_MAIN_ATK_COMBO_1_JUMPATTACK:
	{
		if (0.2f > fAnimPlayRate)
		{
			m_bOn_MotionTrail = true;
		}
		else
		{
			m_bOn_MotionTrail = false;
		}


		//// Turn On Weapon Trail
		if (true == m_bTrailSwitch && m_pModel->Get_PlayRate() >= 0.52f)
		{
			m_pPlayerWeapons[WEAPON_SWORD - 1]->Active_Trail(false);
		}
		else if (false == m_bTrailSwitch && m_pModel->Get_PlayRate() > 0.28f)
		{
			m_bTrailSwitch = true;
			m_pPlayerWeapons[WEAPON_SWORD - 1]->Active_Trail(true);
		}
		////

		m_fAnimSpeed = 1.2f;

		if (0.25f >= fAnimPlayRate)
		{
			_float MoveSpeed = g_pGameInstance->Easing(TYPE_SinInOut, 1.5f, 0.f, fAnimPlayRate, 0.25f);
			m_pTransformCom->Move_Forward(fDeltaTime * MoveSpeed);
		}
		else if (true == m_pModel->Get_IsHavetoBlockAnimChange() && fAnimPlayRate && 0.9f < fAnimPlayRate)
		{
			m_fAnimSpeed = 1.5f;
			m_pModel->Set_BlockAnim(false);
			m_bAttackEnd = true;
			m_bTrailSwitch = false;
			m_pPlayerWeapons[WEAPON_SWORD - 1]->Active_Trail(false);
		}


		////////////////////Next Combo Check //////////////////////
		Check_NextComboCommand();

		if (true == m_bPlayNextCombo)
		{
			if (0.66f <= fAnimPlayRate)
			{
				m_bPlayJumpAttack = false;
				if (true == Change_NextCombo())
				{
					m_bTrailSwitch = false;
					m_pPlayerWeapons[WEAPON_SWORD - 1]->Active_Trail(false);
				}
			}
			else if (0.6f <= fAnimPlayRate && 0.66f > fAnimPlayRate)
			{
				if (true == m_bReadyDodgeCombo)
				{
					m_pModel->Set_BlockAnim(false);
					m_bPlayDodgeCombo = true;
					m_bPlayJumpAttack = false;
					m_bTrailSwitch = false;
					m_pPlayerWeapons[WEAPON_SWORD - 1]->Active_Trail(false);
				}
			}
		}
		/////////////////////////////////////////////////////////

		// Look At Mouse Pos
		if (0.f <= fAnimPlayRate)
		{
			LookAt_MousePos();
		}
		//
	}
	break;
	case SWORD_ANIM_MAIN_ATK_COMBO_0_JUMPATTACK:
	{
		if (0.2f > fAnimPlayRate)
		{
			m_bOn_MotionTrail = true;
		}
		else
		{
			m_bOn_MotionTrail = false;
		}


		if (0.1f > fAnimPlayRate)
		{
			m_bOn_MotionTrail = true;
		}
		else
		{
			m_bOn_MotionTrail = false;
		}

		//// Turn On Weapon Trail
		if (true == m_bTrailSwitch && m_pModel->Get_PlayRate() >= 0.454f)
		{
			m_pPlayerWeapons[WEAPON_SWORD - 1]->Active_Trail(false);
		}
		else if (false == m_bTrailSwitch && m_pModel->Get_PlayRate() > 0.15f)
		{
			m_bTrailSwitch = true;
			m_pPlayerWeapons[WEAPON_SWORD - 1]->Active_Trail(true);
		}
		////

		m_fAnimSpeed = 1.5f;

		if (0.25f >= fAnimPlayRate)
		{
			_float MoveSpeed = g_pGameInstance->Easing(TYPE_SinInOut, 1.5f, 0.f, fAnimPlayRate, 0.25f);
			m_pTransformCom->Move_Forward(fDeltaTime * MoveSpeed);
		}
		else if (true == m_pModel->Get_IsHavetoBlockAnimChange() && fAnimPlayRate && 0.9f < fAnimPlayRate)
		{
			m_fAnimSpeed = 1.5f;
			m_pModel->Set_BlockAnim(false);
			m_bAttackEnd = true;
			m_bTrailSwitch = false;
			m_pPlayerWeapons[WEAPON_SWORD - 1]->Active_Trail(false);
		}


		////////////////////Next Combo Check //////////////////////
		Check_NextComboCommand();

		if (true == m_bPlayNextCombo)
		{
			if (0.66f <= fAnimPlayRate)
			{
				m_bPlayJumpAttack = false;
				if (true == Change_NextCombo())
				{
					m_bTrailSwitch = false;
					m_pPlayerWeapons[WEAPON_SWORD - 1]->Active_Trail(false);
				}
			}
			else if (0.6f <= fAnimPlayRate && 0.66f > fAnimPlayRate)
			{
				if (true == m_bReadyDodgeCombo)
				{
					m_pModel->Set_BlockAnim(false);
					m_bPlayDodgeCombo = true;
					m_bPlayJumpAttack = false;
					m_bTrailSwitch = false;
					m_pPlayerWeapons[WEAPON_SWORD - 1]->Active_Trail(false);
				}
			}
		}
		/////////////////////////////////////////////////////////


		// Look At Mouse Pos
		if (0.f <= fAnimPlayRate)
		{
			LookAt_MousePos();
		}
		//
	}
	break;

	//// Power Attack ////
	case SWORD_ANIM_POWER_ATK_COMBO_0:
	{
		m_fAnimSpeed = 1.f;

		if (0.f < fAnimPlayRate && 0.277f >= fAnimPlayRate)
		{
			_float MoveSpeed = g_pGameInstance->Easing(TYPE_QuadOut, 2.5f, 0.f, fAnimPlayRate, 0.277f);
			m_pTransformCom->Move_Forward(fDeltaTime * MoveSpeed);
			LookAt_MousePos();
		}
		else if (0.277f < fAnimPlayRate && 0.9f > fAnimPlayRate)
		{
			m_fAnimSpeed = 2.f;
		}
		else if (0.9f < fAnimPlayRate)
		{
			m_fAnimSpeed = 1.5f;
			m_pModel->Set_BlockAnim(false);
			m_bAttackEnd = true;
		}

		////////////////////Next Combo Check //////////////////////
		Check_NextComboCommand();

		if (true == m_bPlayNextCombo)
		{
			if (0.72f <= fAnimPlayRate)
			{
				Change_NextCombo();
			}
			else if (0.65f <= fAnimPlayRate)
			{
				if (false == m_bPlayNextCombo)
					return;

				if (true == m_bReadyDodgeCombo)
				{
					m_pModel->Set_BlockAnim(false);
					m_bPlayDodgeCombo = true;
				}
			}
		}
		/////////////////////////////////////////////////////////
	}
	break;
	case SWORD_ANIM_POWER_ATK_COMBO_1:
	{
		// 
		if (false == m_bAnimChangeSwitch)
		{
			m_bAnimChangeSwitch = true;
			_Vector vThrowStartPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
			vThrowStartPos = XMVectorSetY(vThrowStartPos, XMVectorGetY(vThrowStartPos) + 1.f);
			_Vector vPlayerLook = XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK));
			vThrowStartPos += vPlayerLook * 1.5f;
			static_cast<CPlayerWeapon_Shield*>(m_pPlayerWeapons[WEAPON_SHIELD - 1])->Start_ThrowMode(vThrowStartPos, 5.f);

			m_pMainCamera->Start_CameraShaking_Fov(57.f, 1.f, 0.1f);
		}
		else
		{
			_Vector vThrowStartPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
			vThrowStartPos = XMVectorSetY(vThrowStartPos, XMVectorGetY(vThrowStartPos) + 1.f);
			_Vector vPlayerLook = XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK));
			static_cast<CPlayerWeapon_Shield*>(m_pPlayerWeapons[WEAPON_SHIELD - 1])->Update_ThrowPos(vThrowStartPos, vPlayerLook, fAnimPlayRate);
		}


		if (0.205f >= fAnimPlayRate)
		{
			m_fAnimSpeed = 1.5f;
		}
		else if (0.205f < fAnimPlayRate && 0.529f >= fAnimPlayRate)
		{
			m_fAnimSpeed = 2.5f;
			_float MoveSpeed = g_pGameInstance->Easing(TYPE_QuadOut, 0.f, 1.f, fAnimPlayRate - 0.205f, 0.324f);
			m_pTransformCom->Move_Forward(fDeltaTime * MoveSpeed);
			LookAt_MousePos();
		}
		else if (0.529f < fAnimPlayRate && 0.9f > fAnimPlayRate)
		{
			m_fAnimSpeed = 1.5f;
		}
		else if (0.9f < fAnimPlayRate)
		{
			m_fAnimSpeed = 1.5f;
			m_pModel->Set_BlockAnim(false);
			m_bAttackEnd = true;
			static_cast<CPlayerWeapon_Shield*>(m_pPlayerWeapons[WEAPON_SHIELD - 1])->End_ThrowMode();
			m_bAnimChangeSwitch = false;
		}

		////////////////////Next Combo Check //////////////////////
		Check_NextComboCommand();

		if (true == m_bPlayNextCombo)
		{
			if (0.72f <= fAnimPlayRate)
			{
				if (true == Change_NextCombo())
				{
					static_cast<CPlayerWeapon_Shield*>(m_pPlayerWeapons[WEAPON_SHIELD - 1])->End_ThrowMode();
					m_bAnimChangeSwitch = false;
				}
			}
			else if (0.65f <= fAnimPlayRate)
			{
				if (false == m_bPlayNextCombo)
					return;

				if (true == m_bReadyDodgeCombo)
				{
					m_pModel->Set_BlockAnim(false);
					m_bPlayDodgeCombo = true;
					static_cast<CPlayerWeapon_Shield*>(m_pPlayerWeapons[WEAPON_SHIELD - 1])->End_ThrowMode();
					m_bAnimChangeSwitch = false;
				}
			}
		}
		/////////////////////////////////////////////////////////
	}
	break;
	case SWORD_ANIM_POWER_ATK_COMBO_2:
	{
		if (false == m_bAnimChangeSwitch)
		{
			_Vector vPutOnPos = (XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK)) * 1.2f) + m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
			static_cast<CPlayerWeapon_Shield*>(m_pPlayerWeapons[WEAPON_SHIELD - 1])->Start_SmashMode(vPutOnPos);
			m_bAnimChangeSwitch = true;
		}

		if (fAnimPlayRate <= 0.425f)
		{
			m_fAnimSpeed = 1.5f;
		}
		else
		{
			m_fAnimSpeed = 1.f;
		}


		if (0.9f < fAnimPlayRate)
		{
			m_fAnimSpeed = 1.5f;
			m_pModel->Set_BlockAnim(false);
			m_bAttackEnd = true;
			m_bAnimChangeSwitch = false;
			return;
		}
		else if (0.8f < fAnimPlayRate)
		{
			static_cast<CPlayerWeapon_Shield*>(m_pPlayerWeapons[WEAPON_SHIELD - 1])->End_SmashMode();
		}
		////////////////////Next Combo Check //////////////////////
		Check_NextComboCommand();

		if (true == m_bPlayNextCombo)
		{
			if (0.66f <= fAnimPlayRate)
			{
				if (true == Change_NextCombo())
				{
					static_cast<CPlayerWeapon_Shield*>(m_pPlayerWeapons[WEAPON_SHIELD - 1])->End_SmashMode();
					m_bAnimChangeSwitch = false;
				}
			}
			else if (0.6f <= fAnimPlayRate)
			{
				if (false == m_bPlayNextCombo)
					return;

				if (true == m_bReadyDodgeCombo)
				{
					m_pModel->Set_BlockAnim(false);
					m_bPlayDodgeCombo = true;
					static_cast<CPlayerWeapon_Shield*>(m_pPlayerWeapons[WEAPON_SHIELD - 1])->End_SmashMode();
					m_bAnimChangeSwitch = false;
				}
			}
		}
		/////////////////////////////////////////////////////////

		// Active CameraShake
		if (true == m_bActive_ActionCameraShake && 0.4f <= fAnimPlayRate)
		{
			m_pMainCamera->Start_CameraShaking_Fov(58.f, 3.f, 0.1f);
			m_bActive_ActionCameraShake = false;
		}
		//
	}
	break;
	case SWORD_ANIM_POWER_ATK_COMBO_0_JUMPATTACK:
	{
		if (0.15f > fAnimPlayRate)
		{
			m_bOn_MotionTrail = true;
		}
		else
		{
			m_bOn_MotionTrail = false;
		}

		m_fAnimSpeed = 1.f;

		if (0.f < fAnimPlayRate && 0.277f >= fAnimPlayRate)
		{
			_float MoveSpeed = g_pGameInstance->Easing(TYPE_QuadOut, 2.5f, 0.f, fAnimPlayRate, 0.277f);
			m_pTransformCom->Move_Forward(fDeltaTime * MoveSpeed);
			LookAt_MousePos();
		}
		else if (0.277f < fAnimPlayRate && 0.9f > fAnimPlayRate)
		{
			m_fAnimSpeed = 2.f;
		}
		else if (0.9f < fAnimPlayRate)
		{
			m_fAnimSpeed = 1.5f;
			m_pModel->Set_BlockAnim(false);
			m_bAttackEnd = true;
		}

		////////////////////Next Combo Check //////////////////////
		Check_NextComboCommand();

		if (true == m_bPlayNextCombo)
		{
			if (0.66f <= fAnimPlayRate)
			{
				m_bPlayJumpAttack = false;
				Change_NextCombo();
			}
			else if (0.6f <= fAnimPlayRate && 0.66f > fAnimPlayRate)
			{
				if (true == m_bReadyDodgeCombo)
				{
					m_pModel->Set_BlockAnim(false);
					m_bPlayDodgeCombo = true;
					m_bPlayJumpAttack = false;
				}
			}
		}
		/////////////////////////////////////////////////////////


		// Look At Mouse Pos
		if (0.f <= fAnimPlayRate)
		{
			LookAt_MousePos();
		}
		//
	}
	break;
	case SWORD_ANIM_POWER_ATK_COMBO_1_JUMPATTACK:
	{
		if (0.1f > fAnimPlayRate)
		{
			m_bOn_MotionTrail = true;
		}
		else
		{
			m_bOn_MotionTrail = false;
		}


		m_fAnimSpeed = 2.5f;

		if (false == m_bAnimChangeSwitch)
		{
			m_bAnimChangeSwitch = true;
			_Vector vThrowStartPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
			vThrowStartPos = XMVectorSetY(vThrowStartPos, XMVectorGetY(vThrowStartPos) + 1.f);
			_Vector vPlayerLook = XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK));
			vThrowStartPos += vPlayerLook * 1.5f;
			static_cast<CPlayerWeapon_Shield*>(m_pPlayerWeapons[WEAPON_SHIELD - 1])->Start_ThrowMode(vThrowStartPos, 5.f);

			m_pMainCamera->Start_CameraShaking_Fov(57.f, 1.f, 0.1f);
		}
		else
		{
			_Vector vThrowStartPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
			vThrowStartPos = XMVectorSetY(vThrowStartPos, XMVectorGetY(vThrowStartPos) + 1.f);
			_Vector vPlayerLook = XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK));
			static_cast<CPlayerWeapon_Shield*>(m_pPlayerWeapons[WEAPON_SHIELD - 1])->Update_ThrowPos(vThrowStartPos, vPlayerLook, fAnimPlayRate);
		}

		if (0.25f >= fAnimPlayRate)
		{
			_float MoveSpeed = g_pGameInstance->Easing(TYPE_SinInOut, 1.5f, 0.f, fAnimPlayRate, 0.25f);
			m_pTransformCom->Move_Forward(fDeltaTime * MoveSpeed);
		}
		else if (true == m_pModel->Get_IsHavetoBlockAnimChange() && fAnimPlayRate && 0.9f < fAnimPlayRate)
		{
			m_fAnimSpeed = 2.5f;
			m_pModel->Set_BlockAnim(false);
			m_bAttackEnd = true;
			m_bAnimChangeSwitch = false;
			static_cast<CPlayerWeapon_Shield*>(m_pPlayerWeapons[WEAPON_SHIELD - 1])->End_ThrowMode();
		}


		////////////////////Next Combo Check //////////////////////
		Check_NextComboCommand();

		if (true == m_bPlayNextCombo)
		{
			if (0.66f <= fAnimPlayRate)
			{
				m_bPlayJumpAttack = false;
				if (true == Change_NextCombo())
				{
					m_bAnimChangeSwitch = false;
					static_cast<CPlayerWeapon_Shield*>(m_pPlayerWeapons[WEAPON_SHIELD - 1])->End_ThrowMode();
				}
			}
			else if (0.5f <= fAnimPlayRate && 0.66f > fAnimPlayRate)
			{
				if (true == m_bReadyDodgeCombo)
				{
					m_pModel->Set_BlockAnim(false);
					m_bPlayDodgeCombo = true;
					m_bPlayJumpAttack = false;
					m_bAnimChangeSwitch = false;
					static_cast<CPlayerWeapon_Shield*>(m_pPlayerWeapons[WEAPON_SHIELD - 1])->End_ThrowMode();
				}
			}
		}
		/////////////////////////////////////////////////////////


		// Look At Mouse Pos
		if (0.f <= fAnimPlayRate)
		{
			LookAt_MousePos();
		}
		//
	}
	break;
	case SWORD_ANIM_POWER_ATK_COMBO_2_JUMPATTACK:
	{
		if (0.1f > fAnimPlayRate)
		{
			m_bOn_MotionTrail = true;
		}
		else
		{
			m_bOn_MotionTrail = false;
		}


		m_fAnimSpeed = 1.5f;

		if (false == m_bAnimChangeSwitch)
		{
			_Vector vPutOnPos = (XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK)) * 3.7f) + m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
			static_cast<CPlayerWeapon_Shield*>(m_pPlayerWeapons[WEAPON_SHIELD - 1])->Start_SmashMode(vPutOnPos);
			m_bAnimChangeSwitch = true;
		}

		if (0.25f >= fAnimPlayRate)
		{
			_float MoveSpeed = g_pGameInstance->Easing(TYPE_SinInOut, 1.5f, 0.f, fAnimPlayRate, 0.25f);
			m_pTransformCom->Move_Forward(fDeltaTime * MoveSpeed);
		}
		else if (0.9f < fAnimPlayRate)
		{
			m_fAnimSpeed = 1.5f;
			m_pModel->Set_BlockAnim(false);
			m_bAttackEnd = true;
			m_bAnimChangeSwitch = false;
			static_cast<CPlayerWeapon_Shield*>(m_pPlayerWeapons[WEAPON_SHIELD - 1])->End_SmashMode();
		}
		else if (0.8f < fAnimPlayRate)
		{
			static_cast<CPlayerWeapon_Shield*>(m_pPlayerWeapons[WEAPON_SHIELD - 1])->End_SmashMode();
		}

		////////////////////Next Combo Check //////////////////////
		Check_NextComboCommand();

		if (true == m_bPlayNextCombo)
		{
			if (0.66f <= fAnimPlayRate)
			{
				m_bPlayJumpAttack = false;
				if (true == Change_NextCombo())
				{
					m_bAnimChangeSwitch = false;
					static_cast<CPlayerWeapon_Shield*>(m_pPlayerWeapons[WEAPON_SHIELD - 1])->End_SmashMode();

				}
			}
			else if (0.6f <= fAnimPlayRate && 0.66f > fAnimPlayRate)
			{
				if (true == m_bReadyDodgeCombo)
				{
					m_bAnimChangeSwitch = false;
					m_pModel->Set_BlockAnim(false);
					m_bPlayDodgeCombo = true;
					m_bPlayJumpAttack = false;
					static_cast<CPlayerWeapon_Shield*>(m_pPlayerWeapons[WEAPON_SHIELD - 1])->End_SmashMode();
				}
			}
		}
		/////////////////////////////////////////////////////////

		// Active CameraShake
		if (true == m_bActive_ActionCameraShake && 0.15f <= fAnimPlayRate)
		{
			m_pMainCamera->Start_CameraShaking_Fov(58.f, 3.f, 0.1f);
			m_bActive_ActionCameraShake = false;
		}
		//
	}
	break;
	}
	m_bMainAttacking = true;
}

void CPlayer::Javelin(_double fDeltaTime)
{
	switch (m_eCurUtilityState)
	{
	case UTILITY_START:
		static_cast<CPlayerWeapon_Spear*>(m_pPlayerWeapons[WEAPON_SPEAR - 1])->Change_Pivot(CPlayerWeapon_Spear::ESpearPivot::SPEAR_PIVOT_THROW);
		m_fAnimSpeed = 5.f;
		if (false == m_pModel->Get_IsHavetoBlockAnimChange())
		{
			m_eCurUtilityState = UTILITY_LOOP;
		}
		m_pTransformCom->Set_MoveSpeed(1.4f);
		break;
	case UTILITY_LOOP:
		m_fAnimSpeed = 1.12f;

		if (false == m_bPressedUtilityKey)
		{
			m_pModel->Change_AnimIndex(BASE_ANIM_IDLE, 0.1f, true);
			m_eCurUtilityState = UTILITY_END;
		}
		else if (true == m_bPressedMainAttackKey)
		{
			m_pModel->Change_AnimIndex(SPEAR_ANIM_THROW_END, 0.1f, true);
			m_eCurUtilityState = UTILITY_ACTIVE;
		}
		else
		{
			Move_NotTurn(m_eInputDir, fDeltaTime);

			if (MOVDIR_END == m_eInputDir)
			{
				m_pModel->Change_AnimIndex(SPEAR_ANIM_THROW_LOOP, 0.1f, true);
			}
			else
			{
				_Vector vMyLook = XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK));
				_Vector vDot = XMVector3Dot(m_fMovDir.XMVector(), vMyLook);
				_float fDot = XMVectorGetX(vDot);

				if (fDot > 0.5f)
				{
					m_pModel->Change_AnimIndex(SPEAR_ANIM_THROW_LOOP_MOV_F, 0.1f, true);
				}
				else if (fDot < -0.5f)
				{
					m_pModel->Change_AnimIndex(SPEAR_ANIM_THROW_LOOP_MOV_B, 0.1f, true);
				}
				else
				{
					_Vector vMyRight = XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_RIGHT));
					vDot = XMVector3Dot(m_fMovDir.XMVector(), vMyRight);
					fDot = XMVectorGetX(vDot);
					if (fDot > 0)
					{
						m_pModel->Change_AnimIndex(SPEAR_ANIM_THROW_LOOP_MOV_R, 0.1f, true);
					}
					else
					{
						m_pModel->Change_AnimIndex(SPEAR_ANIM_THROW_LOOP_MOV_L, 0.1f, true);
					}
				}
			}
		}
		LookAt_MousePos();
		break;
	case UTILITY_ACTIVE:
		m_fAnimSpeed = 1.8f;
		Throw_Spear(fDeltaTime);
		break;
	case UTILITY_END:
		static_cast<CPlayerWeapon_Spear*>(m_pPlayerWeapons[WEAPON_SPEAR - 1])->Change_Pivot(CPlayerWeapon_Spear::ESpearPivot::SPEAR_PIVOT_NORMAL);
		m_fAnimSpeed = 2.f;
		m_eCurUtilityState = UTILITY_START;
		Set_State_IdleStart(fDeltaTime);
		static_cast<CPlayerWeapon_Spear*>(m_pPlayerWeapons[WEAPON_SPEAR - 1])->Throw_End();
		m_pTransformCom->Set_MoveSpeed(5.f);
		break;
	}
}

void CPlayer::Throw_Spear(_double fDeltaTime)
{
	if (0.588f <= m_pModel->Get_PlayRate())
	{
		if (true == m_bPressedUtilityKey)
			m_eCurUtilityState = UTILITY_LOOP;
		else
		{
			Set_State_IdleStart(fDeltaTime);
			static_cast<CPlayerWeapon_Spear*>(m_pPlayerWeapons[WEAPON_SPEAR - 1])->Change_Pivot(CPlayerWeapon_Spear::ESpearPivot::SPEAR_PIVOT_NORMAL);
			m_pTransformCom->Set_MoveSpeed(5.f);
		}
		static_cast<CPlayerWeapon_Spear*>(m_pPlayerWeapons[WEAPON_SPEAR - 1])->Throw_End();
		m_bThrowSpear = false;
	}
	else if (false == m_bThrowSpear && 0.14f >= m_pModel->Get_PlayRate() && 0.1f <= m_pModel->Get_PlayRate())
	{
		m_bThrowSpear = true;
		_Vector vPlayerLook = XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK));
		static_cast<CPlayerWeapon_Spear*>(m_pPlayerWeapons[WEAPON_SPEAR - 1])->Throw_Start(vPlayerLook);

		m_pMainCamera->Start_CameraShaking_Fov(58.f, 2.f, 0.1f);
	}
}

void CPlayer::Spear_Ultimate(_double fDeltaTime)
{
	static_cast<CPlayerWeapon_Spear*>(m_pPlayerWeapons[WEAPON_SPEAR - 1])->Change_Pivot(CPlayerWeapon_Spear::ESpearPivot::SPEAR_PIVOT_TAKEDOWN);
	_float fAnimPlayRate = (_float)m_pModel->Get_PlayRate();
	m_fAnimSpeed = 1.f;

	// Active CameraShake
	if (true == m_bActive_ActionCameraShake && 0.45f >= fAnimPlayRate  && 0.4f <= fAnimPlayRate)
	{
		m_pMainCamera->Start_CameraShaking_Fov(58.f, 3.f, 0.5f);
		m_bActive_ActionCameraShake = false;
	}
	//

	if (0.98f <= fAnimPlayRate)
	{
		static_cast<CPlayerWeapon_Spear*>(m_pPlayerWeapons[WEAPON_SPEAR - 1])->Change_Pivot(CPlayerWeapon_Spear::ESpearPivot::SPEAR_PIVOT_NORMAL);
		Set_State_IdleStart(fDeltaTime);
		m_bActive_ActionCameraShake = true;
	}


}

void CPlayer::Shelling(_double fDeltaTime)
{
	switch (m_eCurUtilityState)
	{
	case UTILITY_START:
		m_fAnimSpeed = 1.f;

		if (false == m_pShellingSkillRange->Get_IsActive())
		{
			m_pShellingSkillRange->Set_Active(true);
		}

		if (false == m_bAnimChangeSwitch && 0.44f < m_pModel->Get_PlayRate())
		{
			m_bAnimChangeSwitch = true;

			CPlayerWeapon::PlayerWeaponDesc eWeaponDesc;
			eWeaponDesc.eAttachedDesc.Initialize_AttachedDesc(this, "skd_r_palm", _float3(1, 1, 1), _float3(0, 0, 0), _float3(0.f, 0.f, 0.0f));
			eWeaponDesc.eWeaponState = CPlayerWeapon::EWeaponState::STATE_EQUIP;
			g_pGameInstance->Add_GameObject_To_Layer(g_pGameInstance->Get_TargetSceneNum(), TAG_LAY(Layer_PlayerWeapon), TAG_OP(Prototype_PlayerWeapon_Arrow), &eWeaponDesc);
			CPlayerWeapon_Arrow* pBowArrow = static_cast<CPlayerWeapon_Arrow*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_PlayerWeapon)));
			pBowArrow->Set_State(CPlayerWeapon_Arrow::Arrow_State_UtilityReady);
		}

		if (false == m_pModel->Get_IsHavetoBlockAnimChange())
		{
			m_eCurUtilityState = UTILITY_LOOP;
			m_bAnimChangeSwitch = false;
		}
		else if (false == m_bPressedUtilityKey)
		{
			m_eCurUtilityState = UTILITY_END;
			static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->PlayAnim_Idle();
			m_bAnimChangeSwitch = false;

			if (0.44f < m_pModel->Get_PlayRate())
			{
				CPlayerWeapon_Arrow* pBowArrow = static_cast<CPlayerWeapon_Arrow*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_PlayerWeapon)));
				pBowArrow->Set_IsDead();
			}
			return;
		}

		static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->PlayAnim_UtilityAttack_Ready();
		break;
	case UTILITY_LOOP:
		m_fCurTime_ShellingDelay += (_float)g_pGameInstance->Get_DeltaTime(TEXT("Player_Timer_ShellingShot_Delay"));

		m_fAnimSpeed = 1.f;

		if (false == m_bPressedUtilityKey)
		{
			m_fCurTime_ShellingDelay = 0.f;
			m_pModel->Change_AnimIndex(BOW_ANIM_UTILITY_SHOT, 0.1f);
			m_eCurUtilityState = UTILITY_END;

			CPlayerWeapon_Arrow* pBowArrow = static_cast<CPlayerWeapon_Arrow*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_PlayerWeapon)));
			pBowArrow->Set_IsDead();
		}
		else if (m_fCurTime_ShellingDelay >= m_fMaxTime_ShellingDelay && true == m_bPressedMainAttackKey)
		{
			m_fCurTime_ShellingDelay = 0.f;
			m_pModel->Change_AnimIndex(BOW_ANIM_UTILITY_SHOT, 0.1f, true);
			m_eCurUtilityState = UTILITY_ACTIVE;
			static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->PlayAnim_UtilityAttack_Shot();

			CPlayerWeapon_Arrow* pBowArrow = static_cast<CPlayerWeapon_Arrow*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_PlayerWeapon)));
			pBowArrow->Set_State(CPlayerWeapon_Arrow::Arrow_State_UtilityShot);
			pBowArrow->Set_TargetPos(m_pShellingSkillRange->Get_AttackPoint());
			m_pMainCamera->Start_CameraShaking_Fov(58.f, 2.f, 0.03f);
		}
		LookAt_MousePos();
		break;
	case UTILITY_ACTIVE:
		m_fAnimSpeed = 1.5f;
		Shot_Shelling(fDeltaTime);
		break;
	case UTILITY_END:
		m_pShellingSkillRange->Set_Active(false);
		m_fAnimSpeed = 2.f;
		if (0.9f <= m_pModel->Get_PlayRate())
		{
			m_eCurUtilityState = UTILITY_START;
			Set_State_IdleStart(fDeltaTime);
			static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->PlayAnim_Idle();
		}
		break;
	}
}

void CPlayer::Shot_Shelling(_double fDeltaTime)
{
	if (0.277f <= m_pModel->Get_PlayRate())
	{
		m_eCurUtilityState = UTILITY_LOOP;
		m_pModel->Change_AnimIndex(BOW_ANIM_UTILITY_LOOP);
		static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->PlayAnim_UtilityAttack_Loop();

		CPlayerWeapon::PlayerWeaponDesc eWeaponDesc;
		eWeaponDesc.eAttachedDesc.Initialize_AttachedDesc(this, "skd_r_palm", _float3(1, 1, 1), _float3(0, 0, 0), _float3(0.f, 0.f, 0.0f));
		eWeaponDesc.eWeaponState = CPlayerWeapon::EWeaponState::STATE_EQUIP;
		g_pGameInstance->Add_GameObject_To_Layer(g_pGameInstance->Get_TargetSceneNum(), TAG_LAY(Layer_PlayerWeapon), TAG_OP(Prototype_PlayerWeapon_Arrow), &eWeaponDesc);
		CPlayerWeapon_Arrow* pBowArrow = static_cast<CPlayerWeapon_Arrow*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_PlayerWeapon)));
		pBowArrow->Set_State(CPlayerWeapon_Arrow::Arrow_State_UtilityReady);
	}
}

void CPlayer::Bow_Ultimate(_double fDeltaTime)
{
	m_fAnimSpeed = 1.f;

	if (0.98f <= m_pModel->Get_PlayRate())
	{
		Set_State_IdleStart(fDeltaTime);
	}
	else if (0.774f <= m_pModel->Get_PlayRate())
	{
		static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->PlayAnim_Idle();
		static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->Set_AnimSpeed(3.f);
		m_pMainCamera->Start_CameraShaking_Thread(2.5f, 5.f, 0.01f);
	}
	else if (true == m_bAnimChangeSwitch && 0.574f <= m_pModel->Get_PlayRate())
	{
		m_bAnimChangeSwitch = false;
		static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->PlayAnim_UtilityAttack_Shot();
		static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->Set_AnimSpeed(1.f);

		CPlayerWeapon_Arrow* pBowArrow = static_cast<CPlayerWeapon_Arrow*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_PlayerWeapon)));
		pBowArrow->Set_State_Ultimate_Post_Shot();
		m_pMainCamera->Start_CameraShaking_Fov(57.f, 3.f, 0.1f);
	}
	else if (false == m_bAnimChangeSwitch && 0.446f <= m_pModel->Get_PlayRate() && 0.574f >= m_pModel->Get_PlayRate())
	{
		m_bAnimChangeSwitch = true;
		static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->PlayAnim_UtilityAttack_Ready();
		static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->Set_AnimSpeed(2.f);

		CPlayerWeapon::PlayerWeaponDesc eWeaponDesc;
		eWeaponDesc.eAttachedDesc.Initialize_AttachedDesc(this, "skd_r_palm", _float3(1, 1, 1), _float3(0, 0, 0), _float3(0.f, 0.f, 0.0f));
		eWeaponDesc.eWeaponState = CPlayerWeapon::EWeaponState::STATE_EQUIP;
		g_pGameInstance->Add_GameObject_To_Layer(g_pGameInstance->Get_TargetSceneNum(), TAG_LAY(Layer_PlayerWeapon), TAG_OP(Prototype_PlayerWeapon_Arrow), &eWeaponDesc);
		CPlayerWeapon_Arrow* pBowArrow = static_cast<CPlayerWeapon_Arrow*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_PlayerWeapon)));
		pBowArrow->Set_State_Ultimate_Post_Ready();
	}
	else if (true == m_bAnimChangeSwitch && 0.191f <= m_pModel->Get_PlayRate() && 0.446f >= m_pModel->Get_PlayRate())
	{
		m_bAnimChangeSwitch = false;
		static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->PlayAnim_NormalAttack_Shot();

		CPlayerWeapon_Arrow* pBowArrow = static_cast<CPlayerWeapon_Arrow*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_PlayerWeapon)));
		pBowArrow->Set_State_Ultimate_Pre_Shot();
		m_pMainCamera->Start_CameraShaking_Fov(57.f, 3.f, 0.1f);
	}
	else if (false == m_bAnimChangeSwitch && 0.106f <= m_pModel->Get_PlayRate() && 0.191f >= m_pModel->Get_PlayRate())
	{
		m_bAnimChangeSwitch = true;
		static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->PlayAnim_NormalAttack_Ready();

		CPlayerWeapon::PlayerWeaponDesc eWeaponDesc;
		eWeaponDesc.eAttachedDesc.Initialize_AttachedDesc(this, "skd_r_palm", _float3(1, 1, 1), _float3(0, 0, 0), _float3(0.f, 0.f, 0.0f));
		eWeaponDesc.eWeaponState = CPlayerWeapon::EWeaponState::STATE_EQUIP;
		g_pGameInstance->Add_GameObject_To_Layer(g_pGameInstance->Get_TargetSceneNum(), TAG_LAY(Layer_PlayerWeapon), TAG_OP(Prototype_PlayerWeapon_Arrow), &eWeaponDesc);
		CPlayerWeapon_Arrow* pBowArrow = static_cast<CPlayerWeapon_Arrow*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_PlayerWeapon)));
		pBowArrow->Set_State_Ultimate_Pre_Ready();
	}
}

void CPlayer::Shield_Mode(_double fDeltaTime)
{
	switch (m_eCurUtilityState)
	{
	case UTILITY_START:
		m_fAnimSpeed = 1.f;
		if (0.f < m_pModel->Get_PlayRate())
		{
			m_pTransformCom->Set_MoveSpeed(1.f);
			m_eCurUtilityState = UTILITY_LOOP;
		}
		break;
	case UTILITY_LOOP:
		m_fAnimSpeed = 1.5f;

		if (false == m_bPressedUtilityKey)
		{
			m_pModel->Change_AnimIndex(BASE_ANIM_IDLE, 0.1f, true);
			m_eCurUtilityState = UTILITY_END;
		}
		else
		{
			Move(m_eInputDir, fDeltaTime);
			if (MOVDIR_END == m_eInputDir)
				m_pModel->Change_AnimIndex(SWORD_ANIM_SHIELD_IDLE, 0.1f, false);
			else
				m_pModel->Change_AnimIndex(SWORD_ANIM_SHIELD_WALK_F, 0.1f, false);
		}
		break;
	case UTILITY_ACTIVE:
		break;
	case UTILITY_END:
		m_fAnimSpeed = 2.f;
		m_eCurUtilityState = UTILITY_START;
		Set_State_IdleStart(fDeltaTime);
		m_pTransformCom->Set_MoveSpeed(5.f);
		break;
	}
}

void CPlayer::Sword_Ultimate(_double fDeltaTime)
{
	m_fAnimSpeed = 1.f;
	_float fAnimPlayRate = (_float)m_pModel->Get_PlayRate();

	if (false == m_bAnimChangeSwitch && 0.25f <= fAnimPlayRate)
	{
		m_bAnimChangeSwitch = true;
		_float fTargetPos_Y = XMVectorGetY(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS)) + 4.f;
		static_cast<CPlayerWeapon_Shield*>(m_pPlayerWeapons[WEAPON_SHIELD - 1])->Start_UltimateMode(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS), fTargetPos_Y);
	}

	if (0.533 <= fAnimPlayRate && 0.666 >= fAnimPlayRate)
	{
		m_fAnimSpeed = 0.4f;
		if (false == m_bActive_ActionCameraShake)
		{
			m_pMainCamera->Start_CameraShaking_Fov(58.f, 1.f, 0.6f);
			m_pMainCamera->Start_CameraShaking_Thread(2.4f, 4.f, 0.005f);
			m_bActive_ActionCameraShake = true;
		}
	}

	if (0.98f <= fAnimPlayRate)
	{
		m_bAnimChangeSwitch = false;
		Set_State_IdleStart(fDeltaTime);
		static_cast<CPlayerWeapon_Shield*>(m_pPlayerWeapons[WEAPON_SHIELD - 1])->End_UltimateMode();
		m_bActive_ActionCameraShake = true;
		return;
	}



	//// Active CameraShake
	if (true == m_bActive_ActionCameraShake && 0.22f <= fAnimPlayRate && 0.5 > fAnimPlayRate)
	{
		m_pMainCamera->Start_CameraShaking_Fov(58.f, 3.f, 0.1f);
		m_bActive_ActionCameraShake = false;
	}
}

void CPlayer::LookAtInputDir(_double fDeltaTime)
{
	if (MOVDIR_END == m_eInputDir)
		return;

	CCamera_Main* pMainCam = (CCamera_Main*)(g_pGameInstance->Get_GameObject_By_LayerIndex(SCENE_STATIC, TAG_LAY(Layer_Camera_Main)));
	_Vector vMyNormalizedLook = m_pTransformCom->Get_MatrixState_Normalized(CTransform::TransformState::STATE_LOOK);

	_float fDiagonalSpeed = 1.f;
	_float fTurnRate = 0.9f;
	_float fMoveRate = (_float)fDeltaTime;

	_Vector vMovDir;
	switch (m_eInputDir)
	{
	case EINPUT_MOVDIR::MOVDIR_F:
	{
		vMovDir = pMainCam->Get_CameraState_Normalize(CTransform::TransformState::STATE_LOOK);
		vMovDir = XMVectorSetY(vMovDir, XMVectorGetY(vMyNormalizedLook));
		break;
	}
	case EINPUT_MOVDIR::MOVDIR_B:
	{
		vMovDir = pMainCam->Get_CameraState_Normalize(CTransform::TransformState::STATE_LOOK) * -1;
		vMovDir = XMVectorSetY(vMovDir, XMVectorGetY(vMyNormalizedLook));
		break;
	}
	case EINPUT_MOVDIR::MOVDIR_L:
	{
		vMovDir = pMainCam->Get_CameraState_Normalize(CTransform::TransformState::STATE_RIGHT) * -1;
		vMovDir = XMVectorSetY(vMovDir, XMVectorGetY(vMyNormalizedLook));
		break;
	}
	case EINPUT_MOVDIR::MOVDIR_R:
	{
		vMovDir = pMainCam->Get_CameraState_Normalize(CTransform::TransformState::STATE_RIGHT);
		vMovDir = XMVectorSetY(vMovDir, XMVectorGetY(vMyNormalizedLook));
		break;
	}
	case EINPUT_MOVDIR::MOVDIR_FR:
	{
		vMovDir = pMainCam->Get_CameraState_Normalize(CTransform::TransformState::STATE_LOOK);
		vMovDir += pMainCam->Get_CameraState_Normalize(CTransform::TransformState::STATE_RIGHT);
		XMVector3Normalize(vMovDir);
		vMovDir = XMVectorSetW(vMovDir, 0.f);
		vMovDir = XMVectorSetY(vMovDir, XMVectorGetY(vMyNormalizedLook));
		break;
	}
	case EINPUT_MOVDIR::MOVDIR_FL:
	{
		vMovDir = pMainCam->Get_CameraState_Normalize(CTransform::TransformState::STATE_LOOK);
		vMovDir += (pMainCam->Get_CameraState_Normalize(CTransform::TransformState::STATE_RIGHT) * -1);
		XMVector3Normalize(vMovDir);
		vMovDir = XMVectorSetW(vMovDir, 0.f);
		vMovDir = XMVectorSetY(vMovDir, XMVectorGetY(vMyNormalizedLook));
		break;
	}
	case EINPUT_MOVDIR::MOVDIR_BR:
	{
		vMovDir = pMainCam->Get_CameraState_Normalize(CTransform::TransformState::STATE_LOOK) * -1;
		vMovDir += pMainCam->Get_CameraState_Normalize(CTransform::TransformState::STATE_RIGHT);
		XMVector3Normalize(vMovDir);
		vMovDir = XMVectorSetW(vMovDir, 0.f);
		vMovDir = XMVectorSetY(vMovDir, XMVectorGetY(vMyNormalizedLook));
		break;
	}
	case EINPUT_MOVDIR::MOVDIR_BL:
	{
		vMovDir = pMainCam->Get_CameraState_Normalize(CTransform::TransformState::STATE_LOOK) * -1;
		vMovDir += pMainCam->Get_CameraState_Normalize(CTransform::TransformState::STATE_RIGHT) * -1;
		XMVector3Normalize(vMovDir);
		vMovDir = XMVectorSetW(vMovDir, 0.f);
		vMovDir = XMVectorSetY(vMovDir, XMVectorGetY(vMyNormalizedLook));
		break;
	}
	}

	m_pTransformCom->Turn_Dir(vMovDir, fTurnRate);
}

void CPlayer::Ledging(_double fDeltaTime)
{
	_float fCurAnimRate = (_float)m_pModel->Get_PlayRate();

	switch (m_eCurLedgeState)
	{
	case EPARKOUR_LEDGESTATE::LEDGE_JUMP:
	{
		m_fAnimSpeed = 1.5f;
		if (0.f < fCurAnimRate)
		{
			if (0.338f <= fCurAnimRate)
			{
				if (nullptr != m_pCurParkourTrigger)
				{
					if (CTestLedgeTrigger::ELedgeTriggerState::STATE_LEDGE == static_cast<CTestLedgeTrigger*>(m_pCurParkourTrigger)->Get_LedgeType() ||
						CTestLedgeTrigger::ELedgeTriggerState::STATE_LAST_LEDGE == static_cast<CTestLedgeTrigger*>(m_pCurParkourTrigger)->Get_LedgeType())
					{
						m_eCurLedgeState = LEDGE_HANGING_IDLE;
						m_pModel->Change_AnimIndex(LEDGE_ANIM_HANGING_IDLE);
						m_pPreParkourTrigger = nullptr;
						m_pCurParkourTrigger = nullptr;
						return;
					}
				}
			}


			if (0.1f <= fCurAnimRate && 0.4f >= fCurAnimRate)
			{
				if (true == m_bPressedDodgeKey)
				{
					m_eCurLedgeState = EPARKOUR_LEDGESTATE::LEDGE_DOUBLEJUMP;
					m_pModel->Change_AnimIndex(LEDGE_ANIM_DOUBLEJUMP);
					m_fFallingStart_Y = XMVectorGetY(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS));
					return;
				}
			}

			if (0.36f >= fCurAnimRate)
			{
				m_pTransformCom->Move_Up(fDeltaTime * 0.7f);
			}
			else if (0.4f <= fCurAnimRate && 0.66f >= fCurAnimRate)
			{
				m_fAnimSpeed = (fCurAnimRate < 0.52f) ?
					g_pGameInstance->Easing_Return(TYPE_Linear, TYPE_Linear, 1.5f, 2.f, fCurAnimRate, 0.8f)
					: g_pGameInstance->Easing_Return(TYPE_Linear, TYPE_Linear, 2.f, 0.6f, fCurAnimRate - 0.52f, 0.8f);

				m_pTransformCom->Move_Up(fDeltaTime * 1.f);
				m_pTransformCom->Move_Backward(fDeltaTime * 0.5f);
				m_fFallingStart_Y = XMVectorGetY(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS));
			}
			else if (0.66f < fCurAnimRate && 0.75f >= fCurAnimRate)
			{
				m_fAnimSpeed = 0.6f;
				m_pTransformCom->Move_Backward(fDeltaTime * 0.7f);
				_float fPos_y = g_pGameInstance->Easing(TYPE_QuadIn, m_fFallingStart_Y, m_fJumpStart_Y, fCurAnimRate - 0.66f, 0.088f);
				_Vector vMyPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
				vMyPos = XMVectorSetY(vMyPos, fPos_y);
				m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, vMyPos);
			}
			else if (0.98f <= fCurAnimRate)
			{
				_Vector vMyPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
				vMyPos = XMVectorSetY(vMyPos, m_fJumpStart_Y);
				m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, vMyPos);

				m_eCurState = STATE_IDLE;
				Set_State_IdleStart(fDeltaTime);
				m_fFallingAcc = 0.f;
			}
		}
	}
		break;
	case EPARKOUR_LEDGESTATE::LEDGE_DOUBLEJUMP:
	{
		m_fAnimSpeed = 1.5f;
		if (0.f < fCurAnimRate)
		{
			if (0.031f < fCurAnimRate && 0.9f >= fCurAnimRate)
			{
				if (0.359f <= fCurAnimRate)
				{
					m_fAnimSpeed = 0.f;
				}

				_float fPos_y = m_fFallingStart_Y + (6.f * m_fFallingAcc - 9.8f * m_fFallingAcc * m_fFallingAcc * 0.5f);
				m_fFallingAcc += 0.03f;
				
				if (m_fJumpStart_Y >= fPos_y)
				{
					fPos_y = m_fJumpStart_Y;
					m_fAnimSpeed = 5.f;
				}
				else
				{
					m_pTransformCom->Move_Backward(fDeltaTime * 1.f);
				}
				_Vector vPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
				vPos = XMVectorSetY(vPos, fPos_y);
				m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, vPos);
			}
			else if (0.98f <= fCurAnimRate)
			{
				_Vector vMyPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
				vMyPos = XMVectorSetY(vMyPos, m_fJumpStart_Y);
				m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, vMyPos);

				m_eCurState = STATE_IDLE;
				Set_State_IdleStart(fDeltaTime);
				m_pPreParkourTrigger = nullptr;
				m_pCurParkourTrigger = nullptr;
				m_fFallingAcc = 0.f;
			}
		}
	}
		break;
	case EPARKOUR_LEDGESTATE::LEDGE_HANGING_IDLE:
	{
		m_pPreParkourTrigger = m_pCurParkourTrigger;

		// Check to input FallingKey
		if (0.f < fCurAnimRate && true == m_bPressedInteractKey)
		{
			m_eCurLedgeState = LEDGE_HANGING_FALLINGDOWN;
			m_pModel->Change_AnimIndex(LEDGE_ANIM_FALLING);
			m_fFallingStart_Y = XMVectorGetY(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS));
			m_fJumpPower = 0.f;
			return;
		} 
		else if (0.f < fCurAnimRate && true == m_bPressedDodgeKey)
		{
			if (nullptr != m_pCurParkourTrigger && CTestLedgeTrigger::ELedgeTriggerState::STATE_LAST_LEDGE == static_cast<CTestLedgeTrigger*>(m_pCurParkourTrigger)->Get_LedgeType())
			{
				m_eCurLedgeState = LEDGE_HANGING_CLIMBUP;
				m_pModel->Change_AnimIndex(LEDGE_ANIM_CLAMB_UP);
				return;
			}
		}

		if (LEDGE_ANIM_HANGING_REACHOUT_UP == m_pModel->Get_NowAnimIndex()) 
		{
			if (m_ePreInputMovDir != m_eInputDir)
			{
				m_pModel->Change_AnimIndex(LEDGE_ANIM_HANGING_IDLE);
			}
			else if (true == m_bPressedDodgeKey)
			{
				m_fFallingStart_Y = XMVectorGetY(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS));
				m_eCurLedgeState = EPARKOUR_LEDGESTATE::LEDGE_HANGING_JUMPUP;
				m_pModel->Change_AnimIndex(LEDGE_ANIM_FALLING);
				m_fJumpPower = 8.f;
				m_pPreParkourTrigger = nullptr;
				m_pCurParkourTrigger = nullptr;
			}
		}
		else
		{
			if (LEDGE_ANIM_HANGING_REACHOUT_BACK_IDLE == m_pModel->Get_NowAnimIndex())
			{
				if (true == m_bPressedDodgeKey)
				{
					m_eCurLedgeState = EPARKOUR_LEDGESTATE::LEDGE_DOUBLEJUMP;
					m_pModel->Change_AnimIndex(LEDGE_ANIM_DOUBLEJUMP);
					m_fFallingStart_Y = XMVectorGetY(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS));
					return;
				}
			}

			// Check to input MoveKey
			if ((MOVDIR_F == m_eInputDir) || (MOVDIR_B == m_eInputDir) || (MOVDIR_L == m_eInputDir) || (MOVDIR_R == m_eInputDir))
			{
				m_eCurLedgeState = LEDGE_HANGING_MOVE;
				if (true == Check_InputDirIsForward())
				{
					if ((MOVDIR_L == m_eInputDir) || (MOVDIR_R == m_eInputDir))
					{
						m_eLedgingMoveDir = m_eInputDir;
						m_bLedgingMove = true;
					}
					else
					{
						m_bLedgingMove = false;
						if (MOVDIR_B == m_eInputDir)
						{
							m_fAnimSpeed = 1.f;
							m_pModel->Change_AnimIndex(LEDGE_ANIM_HANGING_REACHOUT_BACK_IDLE);
							m_eCurLedgeState = LEDGE_HANGING_IDLE;
						}
						else
						{
							m_pModel->Change_AnimIndex(LEDGE_ANIM_HANGING_REACHOUT_UP);
							m_eCurLedgeState = LEDGE_HANGING_IDLE;
						}
					}
				}
				else
				{
					if ((MOVDIR_F == m_eInputDir) || (MOVDIR_B == m_eInputDir))
					{
						_Vector vCamLook = XMVector3Normalize(m_pMainCameraTransform->Get_MatrixState(CTransform::TransformState::STATE_LOOK));
						_Vector vPlayerRight = XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_RIGHT));
						_float fDot = XMVectorGetX(XMVector3Dot(vCamLook, vPlayerRight));

						if (0.f < fDot)
						{
							m_eLedgingMoveDir = (MOVDIR_F == m_eInputDir ? MOVDIR_R : MOVDIR_L);
							m_bLedgingMove = true;
						}
						else
						{
							m_eLedgingMoveDir = (MOVDIR_F == m_eInputDir ? MOVDIR_L : MOVDIR_R);
							m_bLedgingMove = true;
						}
					}
					else
					{
						_Vector vCamRight = XMVector3Normalize(m_pMainCameraTransform->Get_MatrixState(CTransform::TransformState::STATE_RIGHT));
						_Vector vPlayerLook = XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK));
						_float fDot = XMVectorGetX(XMVector3Dot(vCamRight, vPlayerLook));

						m_bLedgingMove = false;
						if (0.f < fDot)
						{
							if (MOVDIR_L == m_eInputDir)
							{
								m_fAnimSpeed = 1.f;
								m_pModel->Change_AnimIndex(LEDGE_ANIM_HANGING_REACHOUT_BACK_IDLE);
								m_eCurLedgeState = LEDGE_HANGING_IDLE;
							}
							else
							{
								m_pModel->Change_AnimIndex(LEDGE_ANIM_HANGING_REACHOUT_UP);
								m_eCurLedgeState = LEDGE_HANGING_IDLE;
							}
						}
						else
						{
							if (MOVDIR_R == m_eInputDir)
							{
								m_fAnimSpeed = 1.f;
								m_pModel->Change_AnimIndex(LEDGE_ANIM_HANGING_REACHOUT_BACK_IDLE);
								m_eCurLedgeState = LEDGE_HANGING_IDLE;
							}
							else
							{
								m_pModel->Change_AnimIndex(LEDGE_ANIM_HANGING_REACHOUT_UP);
								m_eCurLedgeState = LEDGE_HANGING_IDLE;
							}
						}
					}
				}

				if (true == m_bLedgingMove)
				{
					if (MOVDIR_L == m_eLedgingMoveDir)
					{
						m_pModel->Change_AnimIndex(LEDGE_ANIM_HANGING_MOVE_LEFT);
					}
					else
					{
						m_pModel->Change_AnimIndex(LEDGE_ANIM_HANGING_MOVE_RIGHT);
					}
					m_bLedgingMove = false;
				}

				return;
			}
			//
		}
	}
		break;
	case EPARKOUR_LEDGESTATE::LEDGE_HANGING_MOVE:
	{
		if (0.f < fCurAnimRate)
		{
			switch (m_pModel->Get_NowAnimIndex())
			{
			case LEDGE_ANIM_HANGING_MOVE_LEFT:
				m_pTransformCom->Move_Left(fDeltaTime * 0.1f);
				break;
			case LEDGE_ANIM_HANGING_MOVE_RIGHT:
				m_pTransformCom->Move_Right(fDeltaTime * 0.1f);
				break;
			}

			if (0.98f <= fCurAnimRate)
			{
				m_eCurLedgeState = LEDGE_HANGING_IDLE;
				m_pModel->Change_AnimIndex(LEDGE_ANIM_HANGING_IDLE);
			}
		}
	}
		break;
	case EPARKOUR_LEDGESTATE::LEDGE_HANGING_TURN:
	{
		if (0.f < fCurAnimRate)
		{
			//m_pTransformCom->Turn_Dir();
			if (0.98f <= fCurAnimRate)
			{
				//complete turn
				m_eCurLedgeState = LEDGE_HANGING_IDLE;
				m_pModel->Change_AnimIndex(LEDGE_ANIM_HANGING_IDLE);
			}
		}
	}
	break;
	case EPARKOUR_LEDGESTATE::LEDGE_HANGING_JUMPUP:
	{
		_Vector vMyPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
		_float fPrePos_Y = XMVectorGetY(vMyPos);
		m_fFallingAcc += 0.03f;
		_float fPos_y = m_fFallingStart_Y + (m_fJumpPower * m_fFallingAcc - 9.8f * m_fFallingAcc * m_fFallingAcc * 0.5f);

		if (fPrePos_Y >= fPos_y)
		{
			m_eCurLedgeState = LEDGE_HANGING_FALLINGDOWN;
			m_pModel->Change_AnimIndex(LEDGE_ANIM_FALLING);
		}	

		_Vector vPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
		vPos = XMVectorSetY(vPos, fPos_y);
		m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, vPos);
	}
		break;
	case EPARKOUR_LEDGESTATE::LEDGE_HANGING_FALLING:
	{
		m_pTransformCom->Move_Down(fDeltaTime * 2.f);
		_Vector vMyPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
		_float fPos_Y = XMVectorGetY(vMyPos);
		if (fPos_Y <= m_fJumpStart_Y)
		{
			vMyPos = XMVectorSetY(vMyPos, m_fJumpStart_Y);
			m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, vMyPos);

			Set_State_IdleStart(fDeltaTime);
			m_pPreParkourTrigger = nullptr;
			m_pCurParkourTrigger = nullptr;
		}
		

		_float fPos_y = m_fFallingStart_Y + (6.f * m_fFallingAcc - 9.8f * m_fFallingAcc * m_fFallingAcc * 0.5f);
		m_fFallingAcc += 0.03f;

		if (m_fJumpStart_Y >= fPos_y)
		{
			fPos_y = m_fJumpStart_Y;
			m_fAnimSpeed = 5.f;
		}
		else
		{
			m_pTransformCom->Move_Backward(fDeltaTime * 1.f);
		}
		_Vector vPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
		vPos = XMVectorSetY(vPos, fPos_y);
		m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, vPos);

	}
		break;
	case EPARKOUR_LEDGESTATE::LEDGE_HANGING_FALLINGDOWN:
	{
		if (nullptr != m_pCurParkourTrigger && m_pPreParkourTrigger != m_pCurParkourTrigger)
		{
			if (CTestLedgeTrigger::ELedgeTriggerState::STATE_LEDGE == static_cast<CTestLedgeTrigger*>(m_pCurParkourTrigger)->Get_LedgeType() ||
				CTestLedgeTrigger::ELedgeTriggerState::STATE_LAST_LEDGE == static_cast<CTestLedgeTrigger*>(m_pCurParkourTrigger)->Get_LedgeType())
			{
				m_eCurLedgeState = LEDGE_HANGING_IDLE; 
				m_pModel->Change_AnimIndex(LEDGE_ANIM_HANGING_IDLE);
				m_fFallingAcc = 0.f;
				m_fFallingStart_Y = 0.f;
				return;
			}
		}

		_Vector vMyPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
		_float fPos_Y = XMVectorGetY(vMyPos);
		if (fPos_Y <= m_fJumpStart_Y)
		{
			vMyPos = XMVectorSetY(vMyPos, m_fJumpStart_Y);
			m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, vMyPos);
			m_fFallingAcc = 0.f;
			Set_State_IdleStart(fDeltaTime);
			m_fFallingStart_Y = 0.f;
			m_pPreParkourTrigger = nullptr;
			m_pCurParkourTrigger = nullptr;
			return;
		}

		_float fPos_y = m_fFallingStart_Y + (m_fJumpPower * m_fFallingAcc - 9.8f * m_fFallingAcc * m_fFallingAcc * 0.5f);
		m_fFallingAcc += 0.03f;

		if (m_fJumpStart_Y >= fPos_y)
		{
			fPos_y = m_fJumpStart_Y;
			m_fAnimSpeed = 5.f;
		}
	
		_Vector vPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
		vPos = XMVectorSetY(vPos, fPos_y);
		m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, vPos);
	}
		break;
	case EPARKOUR_LEDGESTATE::LEDGE_HANGING_CLIMBUP:
	{
		if (0.f < fCurAnimRate)
		{
			if (0.8f > fCurAnimRate)
			{
				m_pTransformCom->Move_Up(fDeltaTime * 0.1f);
			}
			else if (0.98f <= fCurAnimRate)
			{
				m_eCurState = STATE_IDLE;
				Set_State_IdleStart(fDeltaTime);
				m_fFallingAcc = 0.f;
				m_pPreParkourTrigger = nullptr;
				m_pCurParkourTrigger = nullptr;
			}
		}
	}
		break;
	case EPARKOUR_LEDGESTATE::LEDGE_HANGING_CLIMBDOWN:
	{
		if (0.f < fCurAnimRate)
		{
			if (0.8f > fCurAnimRate)
			{
				m_pTransformCom->Move_Down(fDeltaTime * 0.1f);
			}
			else if (0.98f <= fCurAnimRate)
			{
				m_eCurLedgeState = LEDGE_HANGING_IDLE;
				m_pModel->Change_AnimIndex(LEDGE_ANIM_HANGING_IDLE);
			}
		}
	}
	break;
	}
}

void CPlayer::Play_DodgeAnim()
{
	switch (m_iCurCombo)
	{
	case 1:
		m_pModel->Change_AnimIndex(BASE_ANIM_DODGE_ROLL, 0.f, true);
		break;
	case 2:
		m_pModel->Change_AnimIndex(BASE_ANIM_DODGE_CARTWHEEL, 0.1f, true);
		break;
	case 3:
		m_pModel->Change_AnimIndex(BASE_ANIM_DODGE_FLIP, 0.1f, true);
		break;
	}
}

void CPlayer::Set_InputDir(_int iAxis_F, _int iAxis_R, _double fDeltaTime)
{
	m_ePreInputMovDir = m_eInputDir;
	if (0 < iAxis_F)
	{
		if (0 < iAxis_R)
		{
			m_eInputDir = MOVDIR_FR;
		}
		else if (0 > iAxis_R)
		{
			m_eInputDir = MOVDIR_FL;
		}
		else
		{
			m_eInputDir = MOVDIR_F;
		}
	}
	else if (0 > iAxis_F)
	{
		if (0 < iAxis_R)
		{
			m_eInputDir = MOVDIR_BR;
		}
		else if (0 > iAxis_R)
		{
			m_eInputDir = MOVDIR_BL;
		}
		else
		{
			m_eInputDir = MOVDIR_B;
		}
	}
	else if (0 < iAxis_R)
	{
		m_eInputDir = MOVDIR_R;
	}
	else if (0 > iAxis_R)
	{
		m_eInputDir = MOVDIR_L;
	}
	else
	{
		m_eInputDir = MOVDIR_END;
	}

}

void CPlayer::Set_PlayerState(EPLAYER_STATE eState)
{
	/**
	*		Player State enums
	*		STATE_IDLE, STATE_MOV, STATE_COMBO_ACTION, STATE_TAKE_DAMAGE, STATE_EXECUTION, STATE_DEAD, STATE_END
	*/
	switch (eState)
	{
	case EPLAYER_STATE::STATE_IDLE:
		m_eCurState = eState;
		break;
	case EPLAYER_STATE::STATE_MOV:
		m_eCurState = eState;
		break;
	case EPLAYER_STATE::STATE_ATTACK:
		m_eCurState = eState;
		break;
	case EPLAYER_STATE::STATE_JUMPATTACK:
		m_eCurState = eState;
		break;
	case EPLAYER_STATE::STATE_UTILITYSKILL:
		m_eCurState = eState;
		break;
	case EPLAYER_STATE::STATE_ULTIMATESKILL:
		m_eCurState = eState;
		break;
	case EPLAYER_STATE::STATE_EVASION:
		m_eCurState = eState;
		break;
	case EPLAYER_STATE::STATE_PETAL:
		m_eCurState = eState;
		break;
	}
}

void CPlayer::Set_TurnInputDir()
{
	if (MOVDIR_END == m_eInputDir)
	{
		return;
	}

	m_pTransformCom->LookDir(m_fMovDir.XMVector());
}

void CPlayer::Set_TurnInputDir_CalDir()
{
	if (MOVDIR_END == m_eInputDir)
	{
		return;
	}

	CCamera_Main* pMainCam = (CCamera_Main*)(g_pGameInstance->Get_GameObject_By_LayerIndex(SCENE_STATIC, TAG_LAY(Layer_Camera_Main)));
	_Vector vMyNormalizedLook = m_pTransformCom->Get_MatrixState_Normalized(CTransform::TransformState::STATE_LOOK);

	_Vector vMovDir;
	switch (m_eInputDir)
	{
	case EINPUT_MOVDIR::MOVDIR_F:
	{
		vMovDir = pMainCam->Get_CameraState_Normalize(CTransform::TransformState::STATE_LOOK);
		vMovDir = XMVectorSetY(vMovDir, XMVectorGetY(vMyNormalizedLook));
		break;
	}
	case EINPUT_MOVDIR::MOVDIR_B:
	{
		vMovDir = pMainCam->Get_CameraState_Normalize(CTransform::TransformState::STATE_LOOK) * -1;
		vMovDir = XMVectorSetY(vMovDir, XMVectorGetY(vMyNormalizedLook));
		break;
	}
	case EINPUT_MOVDIR::MOVDIR_L:
	{
		vMovDir = pMainCam->Get_CameraState_Normalize(CTransform::TransformState::STATE_RIGHT) * -1;
		vMovDir = XMVectorSetY(vMovDir, XMVectorGetY(vMyNormalizedLook));
		break;
	}
	case EINPUT_MOVDIR::MOVDIR_R:
	{
		vMovDir = pMainCam->Get_CameraState_Normalize(CTransform::TransformState::STATE_RIGHT);
		vMovDir = XMVectorSetY(vMovDir, XMVectorGetY(vMyNormalizedLook));
		break;
	}
	case EINPUT_MOVDIR::MOVDIR_FR:
	{
		vMovDir = pMainCam->Get_CameraState_Normalize(CTransform::TransformState::STATE_LOOK);
		vMovDir += pMainCam->Get_CameraState_Normalize(CTransform::TransformState::STATE_RIGHT);
		XMVector3Normalize(vMovDir);
		vMovDir = XMVectorSetW(vMovDir, 0.f);
		vMovDir = XMVectorSetY(vMovDir, XMVectorGetY(vMyNormalizedLook));
		break;
	}
	case EINPUT_MOVDIR::MOVDIR_FL:
	{
		vMovDir = pMainCam->Get_CameraState_Normalize(CTransform::TransformState::STATE_LOOK);
		vMovDir += (pMainCam->Get_CameraState_Normalize(CTransform::TransformState::STATE_RIGHT) * -1);
		XMVector3Normalize(vMovDir);
		vMovDir = XMVectorSetW(vMovDir, 0.f);
		vMovDir = XMVectorSetY(vMovDir, XMVectorGetY(vMyNormalizedLook));
		break;
	}
	case EINPUT_MOVDIR::MOVDIR_BR:
	{
		vMovDir = pMainCam->Get_CameraState_Normalize(CTransform::TransformState::STATE_LOOK) * -1;
		vMovDir += pMainCam->Get_CameraState_Normalize(CTransform::TransformState::STATE_RIGHT);
		XMVector3Normalize(vMovDir);
		vMovDir = XMVectorSetW(vMovDir, 0.f);
		vMovDir = XMVectorSetY(vMovDir, XMVectorGetY(vMyNormalizedLook));
		break;
	}
	case EINPUT_MOVDIR::MOVDIR_BL:
	{
		vMovDir = pMainCam->Get_CameraState_Normalize(CTransform::TransformState::STATE_LOOK) * -1;
		vMovDir += pMainCam->Get_CameraState_Normalize(CTransform::TransformState::STATE_RIGHT) * -1;
		XMVector3Normalize(vMovDir);
		vMovDir = XMVectorSetW(vMovDir, 0.f);
		vMovDir = XMVectorSetY(vMovDir, XMVectorGetY(vMyNormalizedLook));
		break;
	}
	}

	m_fMovDir = vMovDir;
	m_pTransformCom->LookDir(m_fMovDir.XMVector());
}

void CPlayer::Set_MainAttackAnim(_bool bJumpAttack)
{
	switch (m_eCurWeapon)
	{
	case EWEAPON_TYPE::WEAPON_SPEAR:
		if (1 == m_iCurCombo)
		{
			if (false == m_bPlayPowerAttack)
			{
				if (true == bJumpAttack)
					m_pModel->Change_AnimIndex(SPEAR_ANIM_MAIN_ATK_COMBO_0_JUMPATTACK, 0.2f, true);
				else
					m_pModel->Change_AnimIndex_ReturnTo(SPEAR_ANIM_MAIN_ATK_COMBO_0, SPEAR_ANIM_MAIN_ATK_COMBO_0_RECORVERY, 0.1f, true);
			}
			else
			{
				if (true == bJumpAttack)
					m_pModel->Change_AnimIndex(SPEAR_ANIM_POWER_ATK_COMBO_0_JUMPATTACK, 0.2f, true);
				else
					m_pModel->Change_AnimIndex(SPEAR_ANIM_POWER_ATK_COMBO_0, 0.1f, true);
			}
		}
		else if (2 == m_iCurCombo)
		{
			if (false == m_bPlayPowerAttack)
			{
				if (true == bJumpAttack)
					m_pModel->Change_AnimIndex(SPEAR_ANIM_MAIN_ATK_COMBO_1_JUMPATTACK, 0.2f, true);
				else
					m_pModel->Change_AnimIndex_ReturnTo(SPEAR_ANIM_MAIN_ATK_COMBO_1, SPEAR_ANIM_MAIN_ATK_COMBO_1_RECORVERY, 0.1f, true);
			}
			else
			{
				if (true == bJumpAttack)
					m_pModel->Change_AnimIndex(SPEAR_ANIM_POWER_ATK_COMBO_1_JUMPATTACK, 0.2f, true);
				else
					m_pModel->Change_AnimIndex(SPEAR_ANIM_POWER_ATK_COMBO_1, 0.1f, true);
			}
		}
		else if (3 == m_iCurCombo)
		{
			if (false == m_bPlayPowerAttack)
			{
				if (true == bJumpAttack)
					m_pModel->Change_AnimIndex(SPEAR_ANIM_MAIN_ATK_COMBO_2_JUMPATTACK, 0.2f, true);
				else
					m_pModel->Change_AnimIndex(SPEAR_ANIM_MAIN_ATK_COMBO_2, 0.1f, true);
			}
			else
			{
				if (true == bJumpAttack)
					m_pModel->Change_AnimIndex(SPEAR_ANIM_POWER_ATK_COMBO_2_JUMPATTACK, 0.2f, true);
				else
					m_pModel->Change_AnimIndex(SPEAR_ANIM_POWER_ATK_COMBO_2, 0.1f, true);
			}
		}
		break;
	case EWEAPON_TYPE::WEAPON_BOW:
		if (false == m_bPlayPowerAttack)
		{
			m_fAnimSpeed = 12.f;
			m_pModel->Change_AnimIndex(BOW_ANIM_MAIN_ATK_START, 0.1f, true);
			m_bAttackEnd = false;
		}
		else
		{
			if (1 == m_iCurCombo)
			{
				if (true == bJumpAttack)
					m_pModel->Change_AnimIndex(BOW_ANIM_POWER_COMBO_0, 0.3f, true);
				else
					m_pModel->Change_AnimIndex(BOW_ANIM_POWER_COMBO_0, 0.1f, true);
			}
			else if (2 == m_iCurCombo)
			{
				if (true == bJumpAttack)
					m_pModel->Change_AnimIndex(BOW_ANIM_POWER_COMBO_1_JUMP, 0.1f, true);
				else
					m_pModel->Change_AnimIndex(BOW_ANIM_POWER_COMBO_1, 0.1f, true);
			}
			else if (3 == m_iCurCombo)
			{
				if (true == bJumpAttack)
					m_pModel->Change_AnimIndex(BOW_ANIM_POWER_COMBO_2_JUMP, 0.1f, true);
				else
					m_pModel->Change_AnimIndex(BOW_ANIM_POWER_COMBO_2, 0.1f, true);
			}
		}
		break;

	case EWEAPON_TYPE::WEAPON_SWORD:
		if (1 == m_iCurCombo)
		{
			if (false == m_bPlayPowerAttack)
			{
				if (true == bJumpAttack)
					m_pModel->Change_AnimIndex(SWORD_ANIM_MAIN_ATK_COMBO_0_JUMPATTACK, 0.2f, true);
				else
					m_pModel->Change_AnimIndex(SWORD_ANIM_MAIN_ATK_COMBO_0, 0.1f, true);
			}
			else
			{
				if (true == bJumpAttack)
					m_pModel->Change_AnimIndex(SWORD_ANIM_POWER_ATK_COMBO_0_JUMPATTACK, 0.1f, true);
				else
					m_pModel->Change_AnimIndex(SWORD_ANIM_POWER_ATK_COMBO_0, 0.1f, true);
			}
		}
		else if (2 == m_iCurCombo)
		{
			if (false == m_bPlayPowerAttack)
			{
				if (true == bJumpAttack)
					m_pModel->Change_AnimIndex(SWORD_ANIM_MAIN_ATK_COMBO_1_JUMPATTACK, 0.2f, true);
				else
					m_pModel->Change_AnimIndex(SWORD_ANIM_MAIN_ATK_COMBO_1, 0.1f, true);
			}
			else
			{
				if (true == bJumpAttack)
					m_pModel->Change_AnimIndex(SWORD_ANIM_POWER_ATK_COMBO_1_JUMPATTACK, 0.1f, true);
				else
					m_pModel->Change_AnimIndex(SWORD_ANIM_POWER_ATK_COMBO_1, 0.1f, true);
			}
		}
		else if (3 == m_iCurCombo)
		{
			if (false == m_bPlayPowerAttack)
			{
				if (true == bJumpAttack)
					m_pModel->Change_AnimIndex(SWORD_ANIM_MAIN_ATK_COMBO_2_JUMPATTACK, 0.1f, true);
				else
					m_pModel->Change_AnimIndex(SWORD_ANIM_MAIN_ATK_COMBO_2, 0.1f, true);
			}
			else
			{
				if (true == bJumpAttack)
					m_pModel->Change_AnimIndex(SWORD_ANIM_POWER_ATK_COMBO_2_JUMPATTACK, 0.28f, true);
				else
					m_pModel->Change_AnimIndex(SWORD_ANIM_POWER_ATK_COMBO_2, 0.1f, true);
			}
		}
		break;
	}
}

void CPlayer::Set_PhysX_Head()
{
	// 머리 뼈 정보 넘기기
	CCollider_PhysX_Base::PHYSXDESC_JOINT  createJoint;

	const _uint length = 9;
	string mBoneNames[length] =
	{
		"skd_head",
		"skd_hair01",
		"skd_hair02", "skd_hair03",
		"skd_hair04","skd_hair05",
		"skd_hair06",
		"skd_hair07", "skd_hairEnd"
	};

	createJoint.mBoneNames = mBoneNames;
	createJoint.mLength = length;
	createJoint.mGameObject = this;
	createJoint.eShapeType = E_GEOMAT_SPEHE;
	createJoint.mMainScale = _float3(0.5f, 0.5f, 0.5f);
	createJoint.mActorScale = _float3(0.5f, 0.5f, 0.5f);
	createJoint.mSeparation = 0.0f;
	createJoint.mAttachModel = m_pModel;

	m_pHeadJoint->Set_ColiderDesc_Hair(createJoint);
}

void CPlayer::Update_Targeting(_double fDeltaTime)
{
	switch (m_eCur_TargetingState)
	{
	case ETARGETING_STATE::TARGETING_SEARCH:
	{
		Targeting_Search();
	}
	break;

	case ETARGETING_STATE::TARGETING_LOOP:
	{
		Targeting_Loop();
	}
	break;
	}
}

void CPlayer::Targeting_Search()
{
	// Check Aleady Cam TargetingMode
	if (ECameraMode::CAM_MODE_TARGETING == m_pMainCamera->Get_CameraMode())
	{
		return;
	}

	// Check Exist UniqMonster
	auto* UniqMonsters = g_pGameInstance->Get_ObjectList_from_Layer(m_eNowSceneNum, TAG_LAY(Layer_Unique_Monster));
	if (nullptr == UniqMonsters || UniqMonsters->size() <= 0)
	{
		return;
	}

	// Check Nearest UniqMonster
	_float			fCur_NearestDist = 10.f;
	CGameObject*	pTarget_UniqMonster = nullptr;
	_Vector			vPlayerPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
	for (auto& UniqMonster : *UniqMonsters)
	{
		_Vector vUniqMonsterPos = static_cast<CTransform*>(UniqMonster->Get_Component(TAG_COM(Com_Transform)))->Get_MatrixState(CTransform::TransformState::STATE_POS);
		_Vector vDist = XMVector3Length(vPlayerPos - vUniqMonsterPos);
		if (fCur_NearestDist >= XMVectorGetX(vDist))
		{
			fCur_NearestDist = XMVectorGetX(vDist);
			pTarget_UniqMonster = UniqMonster;
		}
	}

	// Check Failed Search In Range
	if (nullptr == pTarget_UniqMonster)
	{
		return;
	}

	m_pMainCamera->Set_CameraMode(ECameraMode::CAM_MODE_TARGETING);
	m_eCur_TargetingState = ETARGETING_STATE::TARGETING_LOOP;
	m_pTargetingMonster = pTarget_UniqMonster;
	m_pTargetingMonster_Transform = static_cast<CTransform*>(m_pTargetingMonster->Get_Component(TAG_COM(Com_Transform)));

	Targeting_Loop();
}

void CPlayer::Targeting_Loop()
{
	_Vector vPlayerPos = Get_BonePos("skd_hip");
	_Vector vPlayerPos2 = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
	_Vector vTargetPos = m_pTargetingMonster_Transform->Get_MatrixState(CTransform::TransformState::STATE_POS);
	_Vector vCenterPos = (vPlayerPos + vTargetPos) * 0.5f;

	_float fDist = XMVectorGetX(XMVector3Length(vPlayerPos - vTargetPos));// *0.15f;
	if (fDist > 20.f)
	{
		m_pMainCamera->Set_CameraMode(ECameraMode::CAM_MODE_NOMAL);
		m_eCur_TargetingState = ETARGETING_STATE::TARGETING_SEARCH;
		m_pMainCamera->Set_CameraLookWeight(0.9f);
		m_pMainCamera->Set_CameraMoveWeight(0.9f);
		return;
	}  
	else 
	{
		m_pMainCamera->Set_CameraLookWeight(0.98f);
		m_pMainCamera->Set_CameraMoveWeight(0.98f);
		fDist *= 2.5f;
		if (fDist > 25.f)
		{
			fDist = 25.f;
		}
	}

	_Vector vLookDir = XMVectorSet(0.f, -0.5f, 0.3f, 0.f) * (fDist + 5.f);

	// Send to Center pos to Main Camera
	m_pMainCamera->Set_TargetingPoint(vCenterPos);
	m_pMainCamera->Set_TargetingLook(vLookDir);

}

void CPlayer::CheckOn_MotionTrail()
{
	if (true == m_bOn_MotionTrail)
	{
		Active_MotionTrail();
	}
}

void CPlayer::Active_MotionTrail()
{
	m_pMotionTrail->Add_MotionBuffer(m_pTransformCom->Get_WorldFloat4x4(), _float4(1.f, 0.1f, 0.1f, 1.f), 0.4f);
}


void CPlayer::Check_NextComboCommand()
{
	if (true == m_bPressedPowerAttackKey)
	{
		m_bReadyMainAttackCombo = true;
		m_bReadyDodgeCombo = false;
		m_bPlayNextCombo = true;
		m_bPlayPowerAttack = true;
	}
	else if (true == m_bPressedMainAttackKey)
	{
		m_bReadyMainAttackCombo = true;
		m_bReadyDodgeCombo = false;
		m_bPlayNextCombo = true;
	}
	else if (true == m_bPressedDodgeKey)
	{
		m_bReadyMainAttackCombo = false;
		m_bReadyDodgeCombo = true;
		m_bPlayNextCombo = true;
	}
}

_bool CPlayer::Change_NextCombo()
{
	if (false == m_bPlayNextCombo)
		return false;

	m_pModel->Set_BlockAnim(false);

	if (true == m_bReadyMainAttackCombo)
	{
		m_bPlayMainAttackCombo = true;
		return true;
	}

	return false;
}

void CPlayer::LookAt_MousePos()
{
	/*_Vector		vResult = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	_float3		fRayDir;
	_float3		fRayPos;

	POINT		ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	_uint		iViewportsNum = 0;
	m_pDeviceContext->RSGetViewports(&iViewportsNum, NULL);

	D3D11_VIEWPORT*		ViewPortDesc = new D3D11_VIEWPORT[iViewportsNum];
	m_pDeviceContext->RSGetViewports(&iViewportsNum, ViewPortDesc);


	_float4		vMousePos;
	vMousePos.x = ptMouse.x / (ViewPortDesc[0].Width * 0.5f) - 1;
	vMousePos.y = ptMouse.y / -(ViewPortDesc[0].Height * 0.5f) + 1;
	vMousePos.z = 0.f;
	vMousePos.w = 1.f;

	_Vector vecMousePos = XMLoadFloat4(&vMousePos);


	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_Matrix		ProjMatrixInverse;
	ProjMatrixInverse = pGameInstance->Get_Transform_Matrix(PLM_PROJ);
	ProjMatrixInverse = XMMatrixInverse(nullptr, ProjMatrixInverse);
	vecMousePos = XMVector4Transform(vecMousePos, ProjMatrixInverse);
	memcpy(&fRayDir, &(vecMousePos - XMVectorSet(0.f, 0.f, 0.f, 1.f)), sizeof(_float3));
	fRayPos = _float3(0.f, 0.f, 0.f);

	_Vector		vRayDir = XMVectorSet(fRayDir.x, fRayDir.y, fRayDir.z, 0.f);
	_Vector		vRayPos = XMVectorSet(fRayPos.x, fRayPos.y, fRayPos.z, 1.f);
	_Matrix		ViewMatrixInverse;
	ViewMatrixInverse = pGameInstance->Get_Transform_Matrix(PLM_VIEW);
	ViewMatrixInverse = XMMatrixInverse(nullptr, ViewMatrixInverse);
	vRayDir = XMVector3TransformNormal(vRayDir, ViewMatrixInverse);
	vRayPos = XMVector3TransformCoord(vRayPos, ViewMatrixInverse);

	XMStoreFloat3(&fRayDir, vRayDir);
	XMStoreFloat3(&fRayPos, vRayPos);

	RELEASE_INSTANCE(CGameInstance);

	_Vector vMyPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
	_Vector vMyLook = XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK));

	CCamera_Main* pMainCam = ((CCamera_Main*)(g_pGameInstance->Get_GameObject_By_LayerIndex(SCENE_STATIC, TAG_LAY(Layer_Camera_Main))));
	_Vector vCamPos = pMainCam->Get_CameraState(CTransform::TransformState::STATE_POS);
	_Vector vDist = XMVector3Length(vCamPos - vMyPos);
	_float fCameraDist = XMVectorGetX(vDist);


	vResult = fRayPos.XMVector() + (fRayDir.XMVector() * fCameraDist);

	XMVectorSetY(vResult, XMVectorGetY(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS)));


	_Vector vNewLook = XMVector3Normalize(vResult - vMyPos);
	vNewLook = XMVectorSetY(vNewLook, XMVectorGetY(vMyLook));
	m_pTransformCom->Turn_Dir(vNewLook, 0.85f);

	Safe_Delete_Array(ViewPortDesc);*/

	POINT ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);


	_Vector vCursorPos = XMVectorSet(
		(_float(ptMouse.x) / (g_iWinCX * 0.5f)) - 1.f,
		(_float(ptMouse.y) / -(g_iWinCY * 0.5f)) + 1.f,
		0, 1.f);

	_Matrix InvProjMat = XMMatrixInverse(nullptr, g_pGameInstance->Get_Transform_Matrix(PLM_PROJ));

	_Vector vRayDir = XMVector4Transform(vCursorPos, InvProjMat) - XMVectorSet(0, 0, 0, 1);

	_Matrix InvViewMat = XMMatrixInverse(nullptr, g_pGameInstance->Get_Transform_Matrix(PLM_VIEW));
	vRayDir = XMVector3TransformNormal(vRayDir, InvViewMat);


	CCamera_Main* pMainCam = ((CCamera_Main*)(g_pGameInstance->Get_GameObject_By_LayerIndex(SCENE_STATIC, TAG_LAY(Layer_Camera_Main))));
	_Vector vCamPos = pMainCam->Get_Camera_Transform()->Get_MatrixState(CTransform::STATE_POS);

	_float3 fResult;
	if (XMVectorGetY(vCamPos) * XMVectorGetY(vRayDir) < 0)
	{
		_float fPos_Y = XMVectorGetY(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS));
		_float Scale = (XMVectorGetY(vCamPos) - fPos_Y) / -(XMVectorGetY(vRayDir));

		_float3 vTargetPos = vCamPos + (Scale)* vRayDir;

		fResult.x = vTargetPos.x;
		fResult.y = fPos_Y;
		fResult.z = vTargetPos.z;
	}

	_Vector vTargetDir = XMVector3Normalize(XMLoadFloat3(&fResult) - m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS));
	m_pTransformCom->Turn_Dir(vTargetDir, 0.85f);
}

_fVector CPlayer::Get_MousePos()
{

	return _fVector();
}

HRESULT CPlayer::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_Player), TAG_COM(Com_Model), (CComponent**)&m_pModel));

	
	FAILED_CHECK(m_pModel->Change_AnimIndex(0));



	CTransform::TRANSFORMDESC tDesc = {};

	tDesc.fMovePerSec = 5;
	tDesc.fRotationPerSec = XMConvertToRadians(60);
	tDesc.fScalingPerSec = 1;
	tDesc.vPivot = _float3(0, 0, 0);

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));

	CMotionTrail::MOTIONTRAILDESC tMotionDesc;

	tMotionDesc.iNumTrailCount = 3;
	tMotionDesc.pModel = m_pModel;
	tMotionDesc.pShader = m_pShaderCom;
	tMotionDesc.iPassIndex = 5;

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_MotionTrail), TAG_COM(Com_MotionTrail), (CComponent**)&m_pMotionTrail, &tMotionDesc));

	// For PhysX
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider_PhysX_Joint), TAG_COM(Com_Collider_PhysX), (CComponent**)&m_pHeadJoint));
	Set_PhysX_Head();



	CDissolve::DISSOLVEDESC	tDissolveDesc;

	tDissolveDesc.eDissolveModelType = CDissolve::DISSOLVE_ANIM;
	tDissolveDesc.pModel = m_pModel;
	tDissolveDesc.pShader = m_pShaderCom;
	tDissolveDesc.RampTextureIndex = 1;

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Dissolve), TAG_COM(Com_Dissolve), (CComponent**)&m_pDissolveCom, &tDissolveDesc));



	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_Collider), (CComponent**)&m_pCollider));
	
	COLLIDERDESC			ColliderDesc;

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(3.0f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));

	ATTACHEDESC tAttachedDesc;
	tAttachedDesc.Initialize_AttachedDesc(this, "skd_hip", _float3(1), _float3(0), _float3(-0.074084f, -0.861011f, -75.1948f));
	m_vecAttachedDesc.push_back(tAttachedDesc);



	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, -1.f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_OBB, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "skd_r_wrist", _float3(1), _float3(0), _float3(-58.667f, -0.60365f, -114.675f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pCollider->Set_ParantBuffer();


	CNavigation::NAVIDESC NaviDesc;
	NaviDesc.iCurrentIndex = 0;
	if (FAILED(__super::Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Navigation), TAG_COM(Com_Navaigation), (CComponent**)&m_pNavigationCom, &NaviDesc)))
		return E_FAIL;

	m_pNavigationCom->FindCellIndex(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS));









	return S_OK;
}

HRESULT CPlayer::SetUp_EtcInfo()
{
	//
	m_eCurWeapon = EWEAPON_TYPE::WEAPON_NONE;
	m_eCurAnim = EPLAYERANIM_BASE::BASE_ANIM_IDLE;
	m_eCurState = EPLAYER_STATE::STATE_IDLE;

	ZeroMemory(m_pPlayerWeapons, sizeof(CPlayerWeapon*) * (WEAPON_END - 1));
	//


	// Release Skill Timer //
	static bool TimerIsSpwaned = false;
	if (!TimerIsSpwaned)
	{
		TimerIsSpwaned = true;
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
		pGameInstance->Add_Timer(TEXT("Player_Timer_ShellingShot_Delay"));
		RELEASE_INSTANCE(CGameInstance);
	}
	m_fMaxTime_ShellingDelay = 0.5f;
	m_fCurTime_ShellingDelay = 0.f;
	//


	// Setting Camera
	m_iMaxCamViewIndex = 4;
	m_pMainCamera = (CCamera_Main*)(g_pGameInstance->Get_GameObject_By_LayerIndex(SCENE_STATIC, TAG_LAY(Layer_Camera_Main)));
	m_pMainCamera->Lock_CamLook(false);
	m_fAttachCamPos_Offset = _float3(0.f, 1.5f, -2.f);
	m_fAttachCamLook_Offset = _float3(0.f, 0.f, 0.f);
	Update_AttachCamPos();
	m_pMainCameraTransform = m_pMainCamera->Get_CamTransformCom();
	// Pressed Move Key Amount
	m_fMaxTime_PressedMoveKeyDuration = 1.f;

	return S_OK;
}

HRESULT CPlayer::SetUp_PlayerWeapons()
{
	// Create Player Weapon Spear //
	CPlayerWeapon::PlayerWeaponDesc eWeaponDesc;
	eWeaponDesc.eAttachedDesc.Initialize_AttachedDesc(this, "skd_r_palm", _float3(1, 1, 1), _float3(-97, -120, -60), _float3(-0.661f, -0.04f, -1.133f));
	eWeaponDesc.eWeaponState = CPlayerWeapon::EWeaponState::STATE_EQUIP;
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(g_pGameInstance->Get_TargetSceneNum(), TAG_LAY(Layer_PlayerWeapon), TAG_OP(Prototype_PlayerWeapon_Spear), &eWeaponDesc));
	m_pPlayerWeapons[WEAPON_SPEAR - 1] = (CPlayerWeapon*)(g_pGameInstance->Get_GameObject_By_LayerIndex(g_pGameInstance->Get_TargetSceneNum(), TAG_LAY(Layer_PlayerWeapon)));
	m_pPlayerWeapons[WEAPON_SPEAR - 1]->Set_BlockUpdate(true);

	// Create Player Weapon Bow //
	eWeaponDesc.eAttachedDesc.Initialize_AttachedDesc(this, "skd_l_palm", _float3(0, 0, 0), _float3(0, 0, 0), _float3(0.f, 0.f, 0.0f));
	eWeaponDesc.eWeaponState = CPlayerWeapon::EWeaponState::STATE_EQUIP;
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(g_pGameInstance->Get_TargetSceneNum(), TAG_LAY(Layer_PlayerWeapon), TAG_OP(Prototype_PlayerWeapon_Bow), &eWeaponDesc));
	m_pPlayerWeapons[WEAPON_BOW - 1] = (CPlayerWeapon*)(g_pGameInstance->Get_GameObject_By_LayerIndex(g_pGameInstance->Get_TargetSceneNum(), TAG_LAY(Layer_PlayerWeapon), 1));
	m_pPlayerWeapons[WEAPON_BOW - 1]->Set_BlockUpdate(true);

	//// Create Player Weapon Sword //
	eWeaponDesc.eAttachedDesc.Initialize_AttachedDesc(this, "skd_r_palm", _float3(1, 1, 1), _float3(0, 0, 0), _float3(0.f, 0.f, 0.0f));
	eWeaponDesc.eWeaponState = CPlayerWeapon::EWeaponState::STATE_EQUIP;
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(g_pGameInstance->Get_TargetSceneNum(), TAG_LAY(Layer_PlayerWeapon), TAG_OP(Prototype_PlayerWeapon_Sword), &eWeaponDesc));
	m_pPlayerWeapons[WEAPON_SWORD - 1] = (CPlayerWeapon*)(g_pGameInstance->Get_GameObject_By_LayerIndex(g_pGameInstance->Get_TargetSceneNum(), TAG_LAY(Layer_PlayerWeapon), 2));
	m_pPlayerWeapons[WEAPON_SWORD - 1]->Set_BlockUpdate(true);

	//// Create Player Weapon Chakra //
	eWeaponDesc.eAttachedDesc.Initialize_AttachedDesc(this, "skd_l_palm", _float3(1.3f, 1.3f, 1.3f), _float3(0, 0, 0), _float3(0.f, 0.f, 0.0f));
	eWeaponDesc.eWeaponState = CPlayerWeapon::EWeaponState::STATE_EQUIP;
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(g_pGameInstance->Get_TargetSceneNum(), TAG_LAY(Layer_PlayerWeapon), TAG_OP(Prototype_PlayerWeapon_Chakra), &eWeaponDesc));
	m_pPlayerWeapons[WEAPON_CHAKRA - 1] = (CPlayerWeapon*)(g_pGameInstance->Get_GameObject_By_LayerIndex(g_pGameInstance->Get_TargetSceneNum(), TAG_LAY(Layer_PlayerWeapon), 3));
	m_pPlayerWeapons[WEAPON_CHAKRA - 1]->Set_BlockUpdate(true);

	//// Create Player Weapon Shield //
	eWeaponDesc.eAttachedDesc.Initialize_AttachedDesc(this, "skd_l_palm", _float3(1, 1, 1), _float3(0, 0, 0), _float3(0.f, 0.f, 0.0f));
	eWeaponDesc.eWeaponState = CPlayerWeapon::EWeaponState::STATE_EQUIP;
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(g_pGameInstance->Get_TargetSceneNum(), TAG_LAY(Layer_PlayerWeapon), TAG_OP(Prototype_PlayerWeapon_Shield), &eWeaponDesc));
	m_pPlayerWeapons[WEAPON_SHIELD - 1] = (CPlayerWeapon*)(g_pGameInstance->Get_GameObject_By_LayerIndex(g_pGameInstance->Get_TargetSceneNum(), TAG_LAY(Layer_PlayerWeapon), 4));
	m_pPlayerWeapons[WEAPON_SHIELD - 1]->Set_BlockUpdate(true);

	return S_OK;
}

HRESULT CPlayer::SetUp_PlayerEffects()
{
	CShellingSkillRange::SHELLINGSKILLRANGEDESC		tShellingSkillRangeDesc;
	tShellingSkillRangeDesc.fInitPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
	tShellingSkillRangeDesc.fMaxDist = 18.f;
	tShellingSkillRangeDesc.fSpeed = 3.f;
	tShellingSkillRangeDesc.fStartDist = 2.f;
	tShellingSkillRangeDesc.pOwner = this;

	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_PlayerEffect), TAG_OP(Prototype_PlayerEffect_ShellingRange), &tShellingSkillRangeDesc));
	m_pShellingSkillRange = (CShellingSkillRange*)(g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_PlayerEffect)));
	m_pShellingSkillRange->Set_Active(false);
	return S_OK;
}

HRESULT CPlayer::Adjust_AnimMovedTransform(_double fDeltatime)
{
	_uint iNowAnimIndex = m_pModel->Get_NowAnimIndex();
	_double PlayRate = m_pModel->Get_PlayRate();

	if (iNowAnimIndex != m_iOldAnimIndex || PlayRate > 0.98)
	{
		m_iAdjMovedIndex = 0;
	}


	if (PlayRate <= 0.98 || 1.f == PlayRate)
	{
		switch (iNowAnimIndex)
		{
		case BASE_ANIM_DODGE_CARTWHEEL:
		case BASE_ANIM_DODGE_FLIP:
		case BASE_ANIM_DODGE_ROLL:
		{
			if (false == m_pModel->Get_IsHavetoBlockAnimChange())
			{
				// if Not Input Next Combo Command, Change to Idle
				if (false == m_bPlayNextCombo)
				{
					m_fAnimSpeed = 1.f;
					Set_PlayerState(STATE_IDLE);
					m_pModel->Change_AnimIndex(BASE_ANIM_IDLE);
					m_iCurCombo = 1;
					m_bDodging = false;
				}
				else
				{
					m_bPlayNextCombo = false;
					m_bPressedDodgeKey = false;

					if (true == m_bPlayMainAttackCombo)				// Change to Attack Combo
					{
						m_bPlayJumpAttack = true;
						m_bPlayMainAttackCombo = false;
						m_bReadyMainAttackCombo = false;
						Set_State_MainAttackStart(fDeltatime);
						m_bDodging = false;
					}
					else											// Change to Dodge Combo
					{
						m_bPlayDodgeCombo = false;
						m_bReadyDodgeCombo = false;
						m_iCurCombo = (m_iCurCombo % m_iMaxCombo) + 1;
						Set_TurnInputDir_CalDir();
						m_bDodging = false;
					}
				}

			}
			else
			{
				LookAtInputDir(fDeltatime);
			}
		}
		break;
		case SPEAR_ANIM_MAIN_ATK_COMBO_0:
		case SPEAR_ANIM_MAIN_ATK_COMBO_0_RECORVERY:
		case SPEAR_ANIM_MAIN_ATK_COMBO_1:
		case SPEAR_ANIM_MAIN_ATK_COMBO_1_RECORVERY:
		case SPEAR_ANIM_MAIN_ATK_COMBO_2:
		case SPEAR_ANIM_MAIN_ATK_COMBO_2_RECORVERY:
		case SPEAR_ANIM_MAIN_ATK_COMBO_0_JUMPATTACK:
		case SPEAR_ANIM_MAIN_ATK_COMBO_1_JUMPATTACK:
		case SPEAR_ANIM_MAIN_ATK_COMBO_2_JUMPATTACK:
		case SPEAR_ANIM_POWER_ATK_COMBO_0:
		case SPEAR_ANIM_POWER_ATK_COMBO_1:
		case SPEAR_ANIM_POWER_ATK_COMBO_2:
		case SPEAR_ANIM_POWER_ATK_COMBO_0_JUMPATTACK:
		case SPEAR_ANIM_POWER_ATK_COMBO_1_JUMPATTACK:
		case SPEAR_ANIM_POWER_ATK_COMBO_2_JUMPATTACK:

		case BOW_ANIM_POWER_COMBO_0:
		case BOW_ANIM_POWER_COMBO_1:
		case BOW_ANIM_POWER_COMBO_2:
		case BOW_ANIM_POWER_COMBO_1_JUMP:
		case BOW_ANIM_POWER_COMBO_2_JUMP:

		case SWORD_ANIM_MAIN_ATK_COMBO_0:
		case SWORD_ANIM_MAIN_ATK_COMBO_1:
		case SWORD_ANIM_MAIN_ATK_COMBO_2:
		case SWORD_ANIM_POWER_ATK_COMBO_0:
		case SWORD_ANIM_POWER_ATK_COMBO_1:
		case SWORD_ANIM_POWER_ATK_COMBO_2:
		case SWORD_ANIM_MAIN_ATK_COMBO_0_JUMPATTACK:
		case SWORD_ANIM_MAIN_ATK_COMBO_1_JUMPATTACK:
		case SWORD_ANIM_MAIN_ATK_COMBO_2_JUMPATTACK:
		case SWORD_ANIM_POWER_ATK_COMBO_0_JUMPATTACK:
		case SWORD_ANIM_POWER_ATK_COMBO_1_JUMPATTACK:
		case SWORD_ANIM_POWER_ATK_COMBO_2_JUMPATTACK:
		{
			if (false == m_pModel->Get_IsHavetoBlockAnimChange())
			{
				static_cast<CPlayerWeapon_Spear*>(m_pPlayerWeapons[WEAPON_SPEAR - 1])->Change_Pivot(CPlayerWeapon_Spear::ESpearPivot::SPEAR_PIVOT_NORMAL);
				// If Input Next Combo Command
				if (true == m_bPlayNextCombo)
				{
					m_bPlayNextCombo = false;

					if (true == m_bPlayDodgeCombo)				// Change to Dodge Combo
					{
						m_bPlayDodgeCombo = false;
						m_bReadyDodgeCombo = false;
						Set_State_DodgeStart(fDeltatime);
						Set_TurnInputDir_CalDir();
						m_bPlayPowerAttack = false;
						m_bAttackEnd = true;
					}
					else										// Change to Attack Combo 
					{
						m_bPlayMainAttackCombo = false;
						m_bAttackEnd = true;
					}
					m_iCurCombo = (m_iCurCombo % m_iMaxCombo) + 1;
				}
				else if (MOVDIR_END != m_eInputDir && true == m_bAttackEnd)			// If not input Command & input Mov Command
				{
					Set_PlayerState(STATE_MOV);
					m_pModel->Change_AnimIndex(BASE_ANIM_RUN_F, 0.1f);
					m_iCurCombo = 1;
					m_bMainAttacking = false;
					m_bPlayJumpAttack = false;
					m_bPlayPowerAttack = false;
				}
				else if (MOVDIR_END == m_eInputDir && true == m_bAttackEnd)			// If not input Command & not input Mov Command
				{
					Set_PlayerState(STATE_IDLE);
					m_pModel->Change_AnimIndex(BASE_ANIM_IDLE, 0.2f);
					m_iCurCombo = 1;
					m_bMainAttacking = false;
					m_bPlayJumpAttack = false;
					m_bPlayPowerAttack = false;
				}

				if (true == m_bAttackEnd)
				{
					m_bActive_ActionCameraShake = true;
				}
			}
		}
		break;
		}
	}


	m_iOldAnimIndex = iNowAnimIndex;
	return S_OK;
}

HRESULT CPlayer::Ready_ParticleDesc()
{
	m_pTextureParticleTransform = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_COM(Com_Transform));
	m_pMeshParticleTransform = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_COM(Com_Transform));
	NULL_CHECK_RETURN(m_pTextureParticleTransform, E_FAIL);
	NULL_CHECK_RETURN(m_pMeshParticleTransform, E_FAIL);


	CUtilityMgr* pUtil = GetSingle(CUtilityMgr);

	m_vecTextureParticleDesc.push_back(pUtil->Get_TextureParticleDesc(L"SpearNormalAttack"));
	m_vecTextureParticleDesc[m_vecTextureParticleDesc.size() - 1].FollowingTarget = m_pTextureParticleTransform;
	m_vecTextureParticleDesc[m_vecTextureParticleDesc.size() - 1].iFollowingDir = FollowingDir_Look;


	return S_OK;
}

CPlayer * CPlayer::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CPlayer*	pInstance = NEW CPlayer(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CPlayer");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CPlayer::Clone(void * pArg)
{
	CPlayer*	pInstance = NEW CPlayer(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CPlayer");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPlayer::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModel);
	Safe_Release(m_pTransformCom);

	Safe_Release(m_pMotionTrail);
	Safe_Release(m_pDissolveCom);
	
	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pHeadJoint);

	Safe_Release(m_pCollider);
	Safe_Release(m_pHPUI);

	Safe_Release(m_pTextureParticleTransform);
	Safe_Release(m_pMeshParticleTransform);


}
