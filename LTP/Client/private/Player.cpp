#include "stdafx.h"
#include "..\Public\Player.h"
#include "Timer.h"
#include "Camera_Main.h"
#include "ShellingArrow.h"
#include "PlayerWeapon_Bow.h"
#include "PlayerWeapon_Spear.h"
#include "PlayerWeapon_Arrow.h"
#include "PlayerWeapon_Sword.h"
#include "ShellingSkillRange.h"
#include "SpearWave.h"
#include "PlayerWeapon_Shield.h"
#include "PlayerWeapon_Chakra.h"
#include "physx\Collider_PhysX_Joint.h"
#include "TriggerObject.h"
#include "TestLedgeTrigger.h"

#include "HpUI.h"
#include "SkillUI.h"

#include "NonInstanceMeshEffect.h"
#include "PartilceCreateMgr.h"



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

	FAILED_CHECK(SetUp_EtcInfo());

	FAILED_CHECK(SetUp_PlayerWeapons());

	FAILED_CHECK(SetUp_PlayerEffects()); 

	FAILED_CHECK(Ready_ParticleDesc());

	Set_IsOcllusion(true);



	CHpUI::HPDesc HpDesc;
	HpDesc.m_HPType = CHpUI::HP_RAJI;
	HpDesc.m_pObjcect = this;
	HpDesc.m_vPos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
	HpDesc.m_Dimensions = 0.63f;

	g_pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&m_pHPUI), m_eNowSceneNum, TAG_OP(Prototype_Object_UI_HpUI), &HpDesc);


	//m_pRendererCom->OnOff_PostPorcessing(POSTPROCESSING_DEBUGCOLLIDER);

	g_pGameInstance->Add_GameObject_Out_of_Manager((CGameObject**)(&m_pSkillUI), m_eNowSceneNum, TAG_OP(Prototype_Object_SkillUI));

	Set_LimLight_N_Emissive(_float4(0, 0, 0, 0), _float4(0.5f, 0, 0, 0));
	return S_OK;
}

_int CPlayer::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0) return -1;


	// TEST CODE
	_uint iCurNavIndex = m_pNavigationCom->Get_CurNavCellIndex();
	_float test = m_pNavigationCom->Get_NaviHeight(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS));
	//

	_Vector vLook = XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK));

#pragma region For Debug Inputkey
	{


		if (g_pGameInstance->Get_DIKeyState(DIK_Z)&DIS_Down)
		{

			NONINSTNESHEFTDESC tNIMEDesc;
			tNIMEDesc.vPosition = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
			tNIMEDesc.vLookDir = m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK);

			tNIMEDesc.eMeshType = Prototype_Mesh_Big_Sword;
			tNIMEDesc.fMaxTime_Duration = 0.7f;


			tNIMEDesc.fAppearTime = 0.35f;

			tNIMEDesc.noisingdir = _float2(0, 1);

			tNIMEDesc.NoiseTextureIndex = 350;
			tNIMEDesc.MaskTextureIndex = 5;
			tNIMEDesc.iDiffuseTextureIndex = 378;
			tNIMEDesc.m_iPassIndex = 19;
			tNIMEDesc.vEmissive = _float4(1, 0.5f, 1.f, 0);
			tNIMEDesc.vLimLight = _float4(0.98046875f, 0.93359375f, 0.19140625f, 1.f);
			tNIMEDesc.NoiseTextureIndex = 381;
			tNIMEDesc.vColor = _float3(0.98046875f, 0.93359375f, 0.19140625f);

			tNIMEDesc.RotAxis = FollowingDir_Right;
			tNIMEDesc.OnceStartRot = -90.f;

			tNIMEDesc.RotationSpeedPerSec = 0.f;
			tNIMEDesc.vSize = _float3(0.01f, 0.03f, 0.01f);

			tNIMEDesc.MoveDir = FollowingDir_Look;
			tNIMEDesc.MoveSpeed = 0;

			g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_PlayerEffect), TAG_OP(Prototype_NonInstanceMeshEffect), &tNIMEDesc);
			//tNIMEDesc.vPosition = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS) + XMVectorSet(10, 0, 0, 0);
			//tNIMEDesc.vLookDir = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_UP);
			//tNIMEDesc.eMeshType = Prototype_Mesh_Lightning_02;
			//g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_PlayerEffect), TAG_OP(Prototype_NonInstanceMeshEffect), &tNIMEDesc);
			//tNIMEDesc.vPosition = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS) + XMVectorSet(20,0,0,0);
			//tNIMEDesc.vLookDir = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_UP);
			//tNIMEDesc.eMeshType = Prototype_Mesh_Lightning_03;
			//g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_PlayerEffect), TAG_OP(Prototype_NonInstanceMeshEffect), &tNIMEDesc);


			//m_pMainCamera->Set_CameraMode(CAM_MODE_FREE);
			//
			//{
			//	//g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_PlayerEffect), TAG_OP(Prototype_NonInstanceMeshEffect), &tNIMEDesc);
			//}
		}



		
		if (g_pGameInstance->Get_DIKeyState(DIK_X)&DIS_Down)
		{
			NONINSTNESHEFTDESC tNIMEDesc;
			tNIMEDesc.vPosition = _float3(60.f, 34.26f, 323.380f);
			tNIMEDesc.vLookDir = _float3(0, 1, 0);

			tNIMEDesc.eMeshType = Prototype_Mesh_Plat_Wall;
			tNIMEDesc.fMaxTime_Duration = 1.25f;

			tNIMEDesc.fAppearTime = 0.25f;

			tNIMEDesc.noisingdir = _float2(0, -1);

			tNIMEDesc.NoiseTextureIndex = 381;
			tNIMEDesc.MaskTextureIndex = 109;
			tNIMEDesc.iDiffuseTextureIndex = 271;
			tNIMEDesc.m_iPassIndex = 19;
			tNIMEDesc.vEmissive = _float4(0, 0.5f, 1.f, 0);
			tNIMEDesc.vLimLight = _float4(1, 1, 0.2f, 0);
			tNIMEDesc.vColor = _float3(1.f, 0, 0);

			tNIMEDesc.RotAxis = FollowingDir_Up;
			tNIMEDesc.RotationSpeedPerSec = 0.f;
			tNIMEDesc.vSize = _float3(4.f, 2.f, -0.0001f);

			tNIMEDesc.fAlphaTestValue = 0.0f;

			g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_PlayerEffect),
				TAG_OP(Prototype_NonInstanceMeshEffect), &tNIMEDesc);

		}

		if (g_pGameInstance->Get_DIKeyState(DIK_P) & DIS_Down)
		{
			m_bPowerOverwhelming = !m_bPowerOverwhelming;
		}

		if (g_pGameInstance->Get_DIKeyState(DIK_I) & DIS_Down)
		{
			Resurrection();
		}

		if (g_pGameInstance->Get_DIKeyState(DIK_O) & DIS_Down)
		{
			SetUp_CheckPoint_CurPos();
		}
	}
#pragma endregion


	// Check Cur Pos Navi's Cell Option
	Check_CurNaviCellOption();

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


	//Process Player State Logic
		switch (m_eCurState)
		{
		case EPLAYER_STATE::STATE_SLEEP:
			FAILED_CHECK(Update_State_Sleep(fDeltaTime));
			break;
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
		case EPLAYER_STATE::STATE_FALL:
			FAILED_CHECK(Update_State_Fall(fDeltaTime));
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
		case EPLAYER_STATE::STATE_ELEVATOR:
			FAILED_CHECK(Update_State_Elevator(fDeltaTime));
			break;
		case EPLAYER_STATE::STATE_STOPACTION:
			FAILED_CHECK(Update_State_StopAction(fDeltaTime));
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

	//// CameraShake Test
	{
	/*if (g_pGameInstance->Get_DIKeyState(DIK_L) & DIS_Down)
	{
		m_pMainCamera->Start_CameraShaking_Thread(1.f, 10.f, 0.018f);
	}*/
		if (g_pGameInstance->Get_DIKeyState(DIK_L) & DIS_Down)
		{
			//m_pMainCamera->Start_CameraShaking_Fov(57.f, 2.f, 0.2f, true);
			
			GetSingle(CUtilityMgr)->SlowMotionStart(1.f, 0.1f);
			m_pMainCamera->Set_MaxTargetArmLength(3.f);
		}
		else if (g_pGameInstance->Get_DIKeyState(DIK_L) & DIS_Up)
		{
			m_pMainCamera->Set_MaxTargetArmLength(10.f);
		}
	//if (g_pGameInstance->Get_DIKeyState(DIK_L) & DIS_Down)
	//{
	//	CCamera_Main::CAMERASHAKEDIRDESC tCameraShakeDirDesc;
	//	tCameraShakeDirDesc.fTotalTime = 1.f;
	//	tCameraShakeDirDesc.fPower = 10.f;
	//	tCameraShakeDirDesc.fChangeDirectioninterval = 0.018f;
	//	tCameraShakeDirDesc.fShakingDir = XMVector3Normalize(m_pMainCamera->Get_Camera_Transform()->Get_MatrixState(CTransform::TransformState::STATE_UP));
	//	m_pMainCamera->Start_CameraShaking_Dir_Thread(&tCameraShakeDirDesc, true);
	//}
	//if (g_pGameInstance->Get_DIKeyState(DIK_L) & DIS_Down)
	//{
	//	CCamera_Main::CAMERASHAKEDIRDESC tCameraShakeDirDesc;
	//	tCameraShakeDirDesc.fTotalTime = 1.f;
	//	tCameraShakeDirDesc.fPower = 10.f;
	//	tCameraShakeDirDesc.fChangeDirectioninterval = 0.018f;
	//	tCameraShakeDirDesc.fShakingDir = XMVector3Normalize(m_pMainCamera->Get_Camera_Transform()->Get_MatrixState(CTransform::TransformState::STATE_RIGHT));
	//	m_pMainCamera->Start_CameraShaking_Dir_Thread(&tCameraShakeDirDesc);
	//}
	//if (g_pGameInstance->Get_DIKeyState(DIK_L) & DIS_Down)
	//{
	//	CCamera_Main::CAMERASHAKEROTDESC tCameraShakeRotDesc;
	//	tCameraShakeRotDesc.fTotalTime = 1.f;
	//	tCameraShakeRotDesc.fPower = 1.f; 
	//	tCameraShakeRotDesc.fChangeDirectioninterval = 0.05f;
	//	tCameraShakeRotDesc.fShakingRotAxis = m_pMainCamera->Get_CamTransformCom()->Get_MatrixState(CTransform::TransformState::STATE_UP);
	//	m_pMainCamera->Start_CameraShaking_Rot_Thread(&tCameraShakeRotDesc, true);
	//}
	// 


	if (g_pGameInstance->Get_DIKeyState(DIK_K) & DIS_Down)
	{
		m_bIsSkillUI = !m_bIsSkillUI;
		}
	if (m_bIsSkillUI)
		{
		m_pSkillUI->Update(fDeltaTime);
		}
	}

	// Update Targeting
	Update_Targeting(fDeltaTime);
	FAILED_CHECK(m_pDissolveCom->Update_Dissolving(fDeltaTime));

	Update_Collider(fDeltaTime);
	Update_Partilce_Position();

	Update_HPUI(fDeltaTime);

	return _int();
}

_int CPlayer::LateUpdate(_double fDeltaTimer)
{
	if (__super::LateUpdate(fDeltaTimer) < 0)return -1;

	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
	FAILED_CHECK(m_pRendererCom->Add_ShadowGroup(CRenderer::SHADOW_ANIMMODEL, this, m_pTransformCom, m_pShaderCom, m_pModel, nullptr,m_pDissolveCom));
	FAILED_CHECK(m_pRendererCom->Add_TrailGroup(CRenderer::TRAIL_MOTION, m_pMotionTrail));
	//FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pCollider));
	//FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pCollider_Parkur));

	m_vOldPos = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS);
	g_pGameInstance->Set_TargetPostion(PLV_PLAYER, m_vOldPos);

	// Update PhysX
	if (m_pHeadJoint)
	{
		m_pHeadJoint->Update_AfterSimulation();
	}

	if (true == m_bOnNavigation)
	{
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, m_pNavigationCom->Get_NaviPosition(m_pTransformCom->Get_MatrixState(CTransform::STATE_POS)));
	}

	LateUpdate_HPUI(fDeltaTimer);

	if (m_bIsSkillUI)
	{
		m_pSkillUI->LateUpdate(fDeltaTimer);
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

	FAILED_CHECK(m_pDissolveCom->Render(13));

#ifdef _DEBUG
	//m_pNavigationCom->Render(m_pTransformCom); 
//	if (m_pHeadJoint)
//		m_pHeadJoint->Render();
#endif // _DEBUG

	return _int();
}

_int CPlayer::LateRender()
{

	return _int();
}

void CPlayer::CollisionTriger(class CCollider* pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{

}

_fVector CPlayer::Get_BonePos(const char * pBoneName)
{
	_Matrix BoneMatrix = m_pModel->Get_BoneMatrix(pBoneName);
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

_bool CPlayer::Get_IsLedgeReachBackState()
{
	return m_bLedge_ReachBackState;
}

_int CPlayer::Get_CurPlayAnimation()
{
	return m_pModel->Get_NowAnimIndex();
}

void CPlayer::SetUp_CheckPoint_CurPos()
{
	m_fResurrectPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
	m_iResurrectNavIndex = m_pNavigationCom->Get_CurNavCellIndex();
}

void CPlayer::SetUp_CheckPoint(_fVector vCurPos, _uint iNavIndex)
{
	m_fResurrectPos = vCurPos;
	m_iResurrectNavIndex = iNavIndex;
}

void CPlayer::Resurrection()
{
	Set_State_IdleStart(g_fDeltaTime);
	m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, m_fResurrectPos);
	m_pNavigationCom->Set_CurNavCellIndex(m_iResurrectNavIndex);
	m_pHPUI->Set_HitCount(0);
	m_fFallingAcc = 0.f;
	m_pMainCamera->Set_CameraMode(ECameraMode::CAM_MODE_NOMAL);
	m_bUpdateAnimation = true;
}

void CPlayer::Set_CurParkourTrigger(CTriggerObject * pParkourTrigger)
{
	m_pCurParkourTrigger = pParkourTrigger;
}

void CPlayer::Set_PlayerNavIndex(_uint iNavIndex)
{
	m_pNavigationCom->Set_CurNavCellIndex(iNavIndex);
}

void CPlayer::Set_PillarBlockClimbUp(_bool bBlock, _float vBlockLimitHeight)
{
	m_bBlockClimbUp = bBlock;
	m_fPillarClimbUpBlockHeight = vBlockLimitHeight;
}

void CPlayer::Set_FallingDead(_bool bFallingDead)
{
	m_bOnNavigation = false;
	m_bFallingDead = bFallingDead;
}

void CPlayer::Set_PosY(_float fPos_y)
{
	_Vector vPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
	m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, XMVectorSetY(vPos, fPos_y));
}

_float CPlayer::Take_Damage(CGameObject * pTargetObject, _float fDamageAmount, _fVector vDamageDir, _bool bKnockback, _float fKnockbackPower)
{
	//if (STATE_STOPACTION == m_eCurState)
	//{
	//	return 0.f;
	//}
	//else if (STATE_EVASION == m_eCurState)
	//{
	//	GetSingle(CUtilityMgr)->SlowMotionStart(1.f, 0.1f);
	//	return 0.f;
	//}

	//GetSingle(CUtilityMgr)->Get_MainCamera()->Start_CameraShaking_Fov(57.f, 1.8f, 0.2f, true);
	//if (STATE_TAKE_DAMAGE == m_eCurState || STATE_DEAD == m_eCurState || true == m_bPowerOverwhelming)
	//{
	//	return 0.f;
	//}

	//if (true == m_bShieldMode)
	//{
	//	_int iSelectSoundFileIndex = rand() % 2;
	//	_tchar pSoundFile[MAXLEN] = TEXT("");
	//	swprintf_s(pSoundFile, TEXT("Jino_Raji_Shield_Block_%d.wav"), iSelectSoundFileIndex);
	//	g_pGameInstance->Play3D_Sound(pSoundFile, m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 0.7f);
	//	return 0.f;
	//}

	//if (0.f < fDamageAmount)
	//{
	//	fDamageAmount *= -1.f;
	//}

	//m_pHPUI->Set_ADD_HitCount((_int)fDamageAmount * -1);

	//_float fRemainHP = Add_NowHP(fDamageAmount);

	//if (0.f >= fRemainHP)
	//{
	//	if (true == bKnockback)
	//	{
	//		Set_State_DamageStart(fKnockbackPower, vDamageDir);
	//	}
	//	else
	//	{
	//		m_pMainCamera->Set_CameraMode(ECameraMode::CAM_MODE_FIX);
	//		Set_State_DeathStart();
	//		return fRemainHP;
	//	}
	//}
	//else
	//{
	//	if (true == bKnockback)
	//	{
	//		Set_State_DamageStart(fKnockbackPower, vDamageDir);
	//	}
	//}

	//_int iSelectSoundFileIndex = rand() % 9;
	//_tchar pSoundFile[MAXLEN] = TEXT("");
	//swprintf_s(pSoundFile, TEXT("Jino_Raji_Hit_%d.wav"), iSelectSoundFileIndex);
	//g_pGameInstance->Play3D_Sound(pSoundFile, m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 0.7f);
	////"Jino_Raji_Hit_%d.wav"

	//return fRemainHP;
	return _float();
}

_float CPlayer::Apply_Damage(CGameObject * pTargetObject, _float fDamageAmount, _bool bKnockback)
{
	return _float();
}

void CPlayer::Set_State_FirstStart()
{
	m_eCurState = STATE_SLEEP;
	m_bOnNavigation = true;
	m_pModel->Change_AnimIndex(BASE_ANIM_SLEEP);
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
		if (m_bMehsArrow == false)
		{
			CTransform* effecttrans = static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->Get_EffectTransform();

			FAILED_CHECK_NONERETURN(static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->Set_Play_MeshParticle(
				CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_ARROW_BOW_UP, effecttrans, &m_bMehsArrow));

			FAILED_CHECK_NONERETURN(static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->Set_Play_MeshParticle(
				CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_ARROW_BOW_SHIFT_PLANE, m_pTransformCom));

			FAILED_CHECK_NONERETURN(static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->Set_Play_MeshParticle(
				CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_ARROW_BOW_SHIFT_ICE, m_pTransformCom));

		}
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
	{
		m_pModel->Change_AnimIndex(SPEAR_ANIM_ULTIMATE, 0.1f, true);



		 m_vecNonInstMeshDesc[3].vPosition = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS) 	+ m_pTransformCom->Get_MatrixState(CTransform::STATE_UP) * 0.5f;
		m_vecNonInstMeshDesc[3].vLookDir = m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK);
		m_vecTextureParticleDesc[6].vFixedPosition = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS) + m_pTransformCom->Get_MatrixState(CTransform::STATE_UP) * 0.25f;

		GetSingle(CUtilityMgr)->Create_TextureInstance(m_eNowSceneNum, m_vecTextureParticleDesc[6]);
		g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_PlayerEffect),
			TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[3]);
		
		g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Spear_Ultimate_Around.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 0.7f);
	}
		break;
	case WEAPON_BOW:
		m_pModel->Change_AnimIndex(BOW_ANIM_ULTIMATE, 0.1f, true);
		break;
	case WEAPON_SWORD:
	{
		m_pModel->Change_AnimIndex(SWORD_ANIM_ULTIMATE, 0.1f, true);

		m_vecNonInstMeshDesc[6].vPosition = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
		m_vecNonInstMeshDesc[6].vLookDir = -(m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK));
		g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_PlayerEffect), TAG_OP(Prototype_NonInstanceMeshEffect),
			&m_vecNonInstMeshDesc[6]);
	}
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

	m_eCurLedgeState = LEDGE_HANGING_IDLE;
	m_pModel->Change_AnimIndex(LEDGE_ANIM_HANGING_IDLE);

	//m_eCurParkourState = m_pCurParkourTrigger->Get_ParkourTriggerType();

	//switch (m_eCurParkourState)
	//{
	//case CTriggerObject::EParkourTriggerType::PACUR_LEDGE:
		//if (CTestLedgeTrigger::ELedgeTriggerState::STATE_LAST_LEDGE == static_cast<CTestLedgeTrigger*>(m_pCurParkourTrigger)->Get_LedgeType())
		//{
//			m_eCurLedgeState = LEDGE_HANGING_CLIMBDOWN;
			//m_pModel->Change_AnimIndex(LEDGE_ANIM_HANGING_CLIMBDOWN);
		//}
		//else 
		//{
//			m_eCurLedgeState = LEDGE_JUMP;
			//m_pModel->Change_AnimIndex(LEDGE_ANIM_JUMP);
		//}
//		break;
//	}
}

void CPlayer::Set_State_LedgeClimbDownStart(_float3 fLookDir, _double fDeltaTime)
{
	m_eCurState = STATE_PARKOUR;
	m_fLookDir = fLookDir;
	m_eCurParkourState = CTriggerObject::EParkourTriggerType::PACUR_LEDGE;
	m_eCurLedgeState = LEDGE_HANGING_CLIMBDOWN;
	m_pModel->Change_AnimIndex(LEDGE_ANIM_HANGING_CLIMBDOWN);

}

void CPlayer::Set_State_LedgeClimbUpStart(_double fDeltaTime)
{
	m_eCurState = STATE_PARKOUR;
	m_eCurParkourState = CTriggerObject::EParkourTriggerType::PACUR_LEDGE;
	m_eCurLedgeState = LEDGE_HANGING_CLIMBUP;
	m_pModel->Change_AnimIndex(LEDGE_ANIM_CLAMB_UP);
	_Vector vPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
	_Vector vLook = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK);
	vPos = XMVectorSetY(vPos, XMVectorGetY(vPos) + 0.11f);
	vPos -= (vLook * 0.06f);
	m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, vPos);
}

void CPlayer::Set_State_WallJump()
{
	m_eCurState = STATE_PARKOUR;
	m_eCurParkourState = CTriggerObject::EParkourTriggerType::PACUR_LEDGE;
	m_eCurLedgeState = LEDGE_JUMP;
	m_pModel->Change_AnimIndex(LEDGE_ANIM_JUMP);
	m_fJumpStart_Y = XMVectorGetY(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS));
}

CPlayer::EPLAYER_STATE CPlayer::Get_PlayerState()
{
	return m_eCurState;
}

void CPlayer::Set_State_CurtainStart(_double fDeltaTime)
{
	m_pModel->Change_AnimIndex(CURTAIN_ANIM_TRANSITION);
	m_eCurState = STATE_CURTAIN;
}

void CPlayer::Set_State_WallRunStart(_bool bAnimDir, _float3 fStartPos, _float3 fEndPos, _double fDeltaTime)
{
	m_fWallRunStartPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
	m_fWallRunEndPos = fEndPos;

	if (false == bAnimDir)
	{
		m_pModel->Change_AnimIndex(WALLRUN_ANIM_LEFT);
	}
	else
	{
		m_pModel->Change_AnimIndex(WALLRUN_ANIM_LEFT);
		//m_pModel->Change_AnimIndex(WALLRUN_ANIM_RIGHT);
	}
	m_eCurState = STATE_WALLRUN;
}

void CPlayer::Set_State_ElevatorStart()
{
	m_pModel->Change_AnimIndex(BASE_ANIM_IDLE);
	m_eCurState = STATE_ELEVATOR;
	m_bOnNavigation = false;
}

void CPlayer::Set_State_ElevatorEnd()
{
	Set_State_IdleStart(g_fDeltaTime);
	m_bOnNavigation = true;
}

void CPlayer::Set_State_StopActionStart()
{
	m_pModel->Change_AnimIndex(BASE_ANIM_IDLE);
	m_eCurState = STATE_STOPACTION;
}

void CPlayer::Set_State_StopActionEnd()
{
	Set_State_IdleStart(g_fDeltaTime);
}

void CPlayer::Set_State_PillarStart(_double fDeltaTime)
{
	if (STATE_PILLAR != m_eCurState)
	{
		m_pModel->Change_AnimIndex(PILLAR_ANIM_GRAB);
		m_eCurState = STATE_PILLAR;

		_Vector vPillarPos = static_cast<CTransform*>(m_pCurParkourTrigger->Get_Component(TAG_COM(Com_Transform)))->Get_MatrixState(CTransform::TransformState::STATE_POS);
		_Vector vPlayerPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
		vPillarPos = XMVectorSetY(vPillarPos, XMVectorGetY(vPlayerPos));

		_Vector vDir = XMVector3Normalize(vPlayerPos - vPillarPos);
		m_fPillarParkourInitPos = vPillarPos + (vDir * 0.35f);
	}
}

void CPlayer::Set_State_PillarGrabStart(_bool bTurnReflect, _double fDeltaTime)
{

	_Vector vPillarPos = static_cast<CTransform*>(m_pCurParkourTrigger->Get_Component(TAG_COM(Com_Transform)))->Get_MatrixState(CTransform::TransformState::STATE_POS);
	_Vector vPlayerPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
	vPillarPos = XMVectorSetY(vPillarPos, XMVectorGetY(vPlayerPos));

	_Vector vDir;
	if (STATE_JUMP != m_eCurState && STATE_FALL != m_eCurState)
	{
		vDir = XMVector3Normalize(vPlayerPos - vPillarPos);
		m_fPillarParkourInitPos = vPillarPos + (vDir * 0.35f);

		_Vector vLookDir = XMVector3Normalize(vPlayerPos - vPillarPos);

		m_pTransformCom->LookDir(vLookDir);
		m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, m_fPillarParkourInitPos);
	}
	else
	{
		vDir = XMVector3Normalize(vPlayerPos - vPillarPos);
		m_fPillarParkourInitPos = vPillarPos + (vDir * -0.35f);

		_Vector vLookDir = XMVector3Normalize(vPillarPos - vPlayerPos);

		m_pTransformCom->LookDir(vLookDir);
		m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, m_fPillarParkourInitPos);
		
	}

	m_pTransformCom->Turn_Direct(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(180.f));
	m_pModel->Change_AnimIndex(PILLAR_ANIM_IDLE);
	m_eCurState = STATE_PILLAR;
}

void CPlayer::Set_State_PetalStart(_double fDeltaTime)
{
	m_pModel->Change_AnimIndex(PETAL_ANIM_PLUCK);
	m_eCurState = STATE_PETAL;
	m_eCurPetalState = PETAL_PLUCK;
}

void CPlayer::Set_State_JumpStart(_double fDeltaTime)
{
	m_eCurState = STATE_JUMP;
	//m_pModel->Change_AnimIndex(BASE_ANIM_JUMP);
	m_pModel->Change_AnimIndex(BASE_ANIM_JUMP_READY);
	m_fJumpStart_Y = XMVectorGetY(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS));

	m_fJumpPower = 0.1f;
}

void CPlayer::Set_State_FallingStart(_double fDeltaTime)
{
}

void CPlayer::Set_State_DamageStart(_float fKnockbackPower, _fVector vDamageDir)
{
	// Reset State Values
	m_iCurCombo = 0;
	m_bDodging = false;
	m_bAttackEnd = true;
	m_bPlayNextCombo = false;
	m_bMainAttacking = false;
	m_bPlayDodgeCombo = false;
	m_bPlayJumpAttack = false;
	m_bReadyDodgeCombo = false;
	m_bPlayPowerAttack = false;
	m_bOncePlaySwingSound = false;
	m_bPlayMainAttackCombo = false;
	m_bReadyMainAttackCombo = false;
	//


	m_eCurState = STATE_TAKE_DAMAGE;
	m_fKnockbackPower = fKnockbackPower;
	m_fKnockbackDir = XMVector3Normalize(XMVectorSetY(vDamageDir, 0.f));
	m_pModel->Change_AnimIndex(DAMAGE_ANIM_DAMAGE);
}

void CPlayer::Set_State_DeathStart()
{
	m_eCurState = STATE_DEAD;
	m_pModel->Change_AnimIndex(DAMAGE_ANIM_DEATH, 0.1f);

	g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Death.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 0.5f);
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

void CPlayer::Set_Targeting(CGameObject * pTarget)
{
	m_pMainCamera->Set_CameraMode(ECameraMode::CAM_MODE_TARGETING);
	m_eCur_TargetingState = ETARGETING_STATE::TARGETING_BOSS;
	m_pTargetingMonster = pTarget;
	m_pTargetingMonster_Transform = static_cast<CTransform*>(m_pTargetingMonster->Get_Component(TAG_COM(Com_Transform)));
}

HRESULT CPlayer::Update_CamLookPoint(_double fDeltaTime)
{
	return E_NOTIMPL;
}

HRESULT CPlayer::Update_State_Sleep(_double fDeltaTime)
{
	_float fAnimRate = (_float)m_pModel->Get_PlayRate();

	switch (m_pModel->Get_NowAnimIndex())
	{
	case BASE_ANIM_SLEEP:
	{
		if (true == m_bPressedInteractKey)
		{
			m_pModel->Change_AnimIndex(BASE_ANIM_WAKEUP);
		}
	}
		break;
	case BASE_ANIM_WAKEUP:
	{
		m_fAnimSpeed = 0.2f;
		if (fAnimRate > 0.97f)
		{
			Set_State_IdleStart(fDeltaTime);
		}
	}
		break;
	}
	return S_OK;
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
		/*if (nullptr != m_pCurParkourTrigger)
		{
			Set_State_ParkourStart(fDeltaTime);
		}
		else
		{*/
		//}

		if (CCell::CELL_JUMPZONE == m_eCurPosNavCellOption)
		{
			Set_State_JumpStart(fDeltaTime);
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
			/*if (nullptr != m_pCurParkourTrigger)
			{
				Set_State_ParkourStart(fDeltaTime);
			}*/
			if (CCell::CELL_JUMPZONE == m_eCurPosNavCellOption)
			{
				Set_State_JumpStart(fDeltaTime);
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
	m_bOnNavigation = false;
	_float fCurAnimRate = (_float)m_pModel->Get_PlayRate();

	switch (m_pModel->Get_NowAnimIndex())
	{
	case BASE_ANIM_JUMP_READY:
	{
		if (g_pGameInstance->Get_DIKeyState(DIK_SPACE) & DIS_Up)
		{
			m_pModel->Change_AnimIndex(BASE_ANIM_JUMP_JUMPING);
		}
		else if (0.9f < fCurAnimRate)
		{
			m_pModel->Change_AnimIndex(BASE_ANIM_JUMP_CHARGING);
		}
	}
		break;
	case BASE_ANIM_JUMP_CHARGING:
	{
		if (g_pGameInstance->Get_DIKeyState(DIK_SPACE) & DIS_Press)
		{
			m_fJumpPower += 0.04f;
			if (m_fJumpPower > 2.f)
			{
				m_fJumpPower = 2.f;
			}
		}
		else
		{
			m_pModel->Change_AnimIndex(BASE_ANIM_JUMP_JUMPING);
		}
	}
		break;
	case BASE_ANIM_JUMP_JUMPING:
	{
		if (0.f < fCurAnimRate && 0.55f >= fCurAnimRate)
		{
			m_fFallingAcc += 0.02267f;
			_float fPos_y = m_fJumpStart_Y + (5 * m_fFallingAcc - 9.8f * m_fFallingAcc * m_fFallingAcc * 0.5f);

			m_pTransformCom->Move_Forward(fDeltaTime * m_fJumpPower, m_pNavigationCom, true);
			_Vector vMyPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
			vMyPos = XMVectorSetY(vMyPos, fPos_y);
			m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, vMyPos);
		}

			_float fMyPos_Y = XMVectorGetY(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS));
			if (fMyPos_Y < m_pNavigationCom->Get_NaviHeight(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS)) 
				&& CCell::CELL_BLOCKZONE != m_pNavigationCom->Get_CurCellOption())
			{
				m_bOnNavigation = true;
				Set_State_IdleStart(fDeltaTime);
				m_fFallingAcc = 0.f;
				return S_OK;
			}

			if (0.55f < fCurAnimRate)
			{
				_float fMyPos_Y = XMVectorGetY(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS));
				if (fMyPos_Y > m_pNavigationCom->Get_NaviHeight(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS))
					|| CCell::CELL_BLOCKZONE == m_pNavigationCom->Get_CurCellOption())
				{
					m_pModel->Change_AnimIndex(LEDGE_ANIM_FALLING, 0.5f);
					m_eCurState = STATE_FALL;
				}
			}
	}
		break;
	}

	return S_OK;
}

HRESULT CPlayer::Update_State_Fall(_double fDeltaTime)
{
	m_bOnNavigation = false;
	m_fAnimSpeed = 1.f;
	m_fFallingAcc += 0.03f;
	_Vector vMyPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
	_float fPrePos_y = XMVectorGetY(vMyPos);
	_float fPos_y = m_fJumpStart_Y + (5 * m_fFallingAcc - 9.8f * m_fFallingAcc * m_fFallingAcc * 0.5f);

	m_pTransformCom->Move_Forward(fDeltaTime * (m_fJumpPower * 0.8f), m_pNavigationCom, true);
	vMyPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
	_float fOnNavPos_Y = m_pNavigationCom->Get_NaviHeight(vMyPos);
	if (fPrePos_y >= fOnNavPos_Y
		&& fPos_y <= fOnNavPos_Y
		&& CCell::CELL_BLOCKZONE != m_pNavigationCom->Get_CurCellOption()) 
	{
		vMyPos = XMVectorSetY(vMyPos, fOnNavPos_Y); 
		Set_State_IdleStart(fDeltaTime);
		m_fFallingAcc = 0.f;
		m_fJumpPower = 0.f;
		return S_OK;
	}
	else
	{
		vMyPos = XMVectorSetY(vMyPos, fPos_y);
	}
	m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, vMyPos);

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
	m_bOnNavigation = false;
	m_fAnimSpeed = 1.f;
	_float fAnimPlayRate = (_float)m_pModel->Get_PlayRate();
	if (0.f < fAnimPlayRate)
	{
		if (0.1f <= fAnimPlayRate && 0.746f >= fAnimPlayRate)
		{
			_float3 fPos = g_pGameInstance->Easing_Vector(TYPE_Linear, m_fWallRunStartPos, m_fWallRunEndPos, fAnimPlayRate - 0.1f, 0.646f);
			m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, fPos);
		}
		if (0.98f < fAnimPlayRate) 
		{
			m_bOnNavigation = true;
			m_pTransformCom->Move_Forward(0.00001f, m_pNavigationCom, false);
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
		m_bOnNavigation = false;
		_Vector vPillarPos = static_cast<CTransform*>(m_pCurParkourTrigger->Get_Component(TAG_COM(Com_Transform)))->Get_MatrixState(CTransform::TransformState::STATE_POS);
		_Vector vPlayerPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
		vPillarPos = XMVectorSetY(vPillarPos, XMVectorGetY(vPlayerPos));
		_Vector vLookDir = vPillarPos - vPlayerPos;
		m_pTransformCom->Turn_Dir(vLookDir, 0.9f);
		m_pTransformCom->MovetoTarget_ErrRange(m_fPillarParkourInitPos.XMVector(), fDeltaTime, 0.1f);
		
		if (0.f < fAnimPlayRate)
		{
			if (0.214f <= fAnimPlayRate && 0.392f >= fAnimPlayRate)
			{
				m_pTransformCom->Move_Up(fDeltaTime * 1.f);
				m_fPillarParkourInitPos.y = XMVectorGetY(vPlayerPos);
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
		m_bOnNavigation = false;
		if (true == m_bPressedInteractKey)
		{
			_float fPlayerPos_y = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS).y;
			if (true == m_bBlockClimbUp && fPlayerPos_y + 0.2f > m_fPillarClimbUpBlockHeight)
			{
				m_pModel->Change_AnimIndex(PILLAR_ANIM_TOP_CLIMB);
			}
			else
			{
				m_pModel->Change_AnimIndex(PILLAR_ANIM_MOVE_DOWN);
			}
		}
		else 
		{
			if (true == m_bPressedDodgeKey)
			{
				m_fFallingStart_Y = XMVectorGetY(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS));
				m_fFallingAcc = 0.f;
				m_pModel->Change_AnimIndex(PILLAR_ANIM_JUMP);
			}
			else if (MOVDIR_F == m_eInputDir)
			{
				// Check Pillar's Top Col
				m_pModel->Change_AnimIndex(PILLAR_ANIM_CLIMB_UP);
				//
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
		m_bBlockClimbUp = false;
	}
		break;
	case PILLAR_ANIM_CLIMB_UP:
	{
		if (0.f < fAnimPlayRate)
		{
			if (0.16f <= fAnimPlayRate && 0.6f >= fAnimPlayRate)
			{
				m_pTransformCom->Move_Up(fDeltaTime * 0.5f);

				if (true == m_bBlockClimbUp)
				{
					_Vector vPlayerPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
					_float fCur_PlayerHeight = XMVectorGetY(vPlayerPos);
					if (m_fPillarClimbUpBlockHeight <= fCur_PlayerHeight)
					{
						vPlayerPos = XMVectorSetY(vPlayerPos, m_fPillarClimbUpBlockHeight);
						m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, vPlayerPos);
					}
				}

			}
			else if (0.98f < fAnimPlayRate)
			{
				m_pModel->Change_AnimIndex(PILLAR_ANIM_IDLE);
				m_bBlockClimbUp = false;
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
				m_pTransformCom->Move_Down(fDeltaTime * 0.3f);
				_Vector vPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
				_float	fPos_y = XMVectorGetY(vPos);
				_float	fNavPos_y = m_pNavigationCom->Get_NaviHeight(vPos);
				if (fPos_y <= (fNavPos_y + 0.8f))
				{
					vPos = XMVectorSetY(vPos, fNavPos_y + 0.8f);
					m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, vPos);
				}
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
			_Vector vPillarPos = static_cast<CTransform*>(m_pCurParkourTrigger->Get_Component(TAG_COM(Com_Transform)))->Get_MatrixState(CTransform::TransformState::STATE_POS);
			_Vector vPlayerPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
			vPillarPos = XMVectorSetY(vPillarPos, XMVectorGetY(vPlayerPos));
			m_pTransformCom->Turn_Revolution_CCW(vPillarPos, -0.35f, fDeltaTime); 
			if (0.95f < fAnimPlayRate)
			{
				m_pModel->Change_AnimIndex(PILLAR_ANIM_IDLE);
			}
			else if (MOVDIR_L != m_eInputDir)
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
			_Vector vPillarPos = static_cast<CTransform*>(m_pCurParkourTrigger->Get_Component(TAG_COM(Com_Transform)))->Get_MatrixState(CTransform::TransformState::STATE_POS);
			_Vector vPlayerPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
			vPillarPos = XMVectorSetY(vPillarPos, XMVectorGetY(vPlayerPos));
			m_pTransformCom->Turn_Revolution_CW(vPillarPos, -0.35f, fDeltaTime);

			if (0.95f < fAnimPlayRate)
			{
				m_pModel->Change_AnimIndex(PILLAR_ANIM_IDLE);
			}
			else if (MOVDIR_R != m_eInputDir)
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
		_Vector vPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
		_float	fPos_y = XMVectorGetY(vPos);
		_float	fNavPos_y = m_pNavigationCom->Get_NaviHeight(vPos);
		if (fPos_y <= (fNavPos_y + 0.8f))
		{
			vPos = XMVectorSetY(vPos, fNavPos_y + 0.8f);
			m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, vPos);
		}

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
				_Vector vPillarPos = static_cast<CTransform*>(m_pCurParkourTrigger->Get_Component(TAG_COM(Com_Transform)))->Get_MatrixState(CTransform::TransformState::STATE_POS);
				_Vector vPlayerPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
				vPillarPos = XMVectorSetY(vPillarPos, m_fPillarClimbUpBlockHeight + 0.5f);
				m_pTransformCom->MovetoTarget_ErrRange(vPillarPos, fDeltaTime, 0.1f);
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
				m_fFallingAcc = 0.f;
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
				m_pTransformCom->Move_Backward(fDeltaTime * 0.2f);
				/*_Vector vPillarPos = static_cast<CTransform*>(m_pCurParkourTrigger->Get_Component(TAG_COM(Com_Transform)))->Get_MatrixState(CTransform::TransformState::STATE_POS);
				_Vector vPlayerPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
				vPillarPos = XMVectorSetY(vPillarPos, m_fPillarClimbUpBlockHeight + 0.5f);
				m_pTransformCom->MovetoTarget_ErrRange(vPillarPos, fDeltaTime, 0.1f);*/
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
		m_bOnNavigation = false;
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
				m_pTransformCom->Move_Forward(fDeltaTime * 1.f, m_pNavigationCom, true);
				_Vector vMyPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
				_float fPrePos_Y = XMVectorGetY(vMyPos);
				_float fPos_y = m_fFallingStart_Y + (8.f * m_fFallingAcc - 9.8f * m_fFallingAcc * m_fFallingAcc * 0.5f);
				if (fPrePos_Y >= fPos_y)
				{
					m_pModel->Change_AnimIndex(LEDGE_ANIM_FALLING);
					m_bActionSwitch = false;
				}
				vMyPos = XMVectorSetY(vMyPos, fPos_y);
				m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, vMyPos);
			}
		}
	}
		break;
	case PILLAR_ANIM_TOP_JUMP:
	{
		m_bOnNavigation = false;
		if (0.f < fAnimPlayRate)
		{
			m_fFallingAcc += 0.03f;
			m_pTransformCom->Move_Forward(fDeltaTime * 1.f, m_pNavigationCom, true);
			_Vector vMyPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
			_float fPrePos_Y = XMVectorGetY(vMyPos);
			_float fPos_y = m_fFallingStart_Y + (8.f * m_fFallingAcc - 9.8f * m_fFallingAcc * m_fFallingAcc * 0.5f);
			if (fPrePos_Y >= fPos_y)
			{
				/*m_fFallingStart_Y = XMVectorGetY(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS));
				m_fFallingAcc = 0.f;*/
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
		m_bOnNavigation = false;
		m_fFallingAcc += 0.03f;
		m_pTransformCom->Move_Forward(fDeltaTime * 1.f, m_pNavigationCom, true);
		_Vector vMyPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
		_float fPrePos_Y = XMVectorGetY(vMyPos);
		_float fPos_y = m_fFallingStart_Y + (8.f * m_fFallingAcc - 9.8f * m_fFallingAcc * m_fFallingAcc * 0.5f);	
		_float fNavPos_y = m_pNavigationCom->Get_NaviHeight(vMyPos);

		if (fNavPos_y <= fPrePos_Y && fNavPos_y >= fPos_y && CCell::CELL_BLOCKZONE != m_pNavigationCom->Get_CurCellOption())  
		{
			m_bOnNavigation = true;
			fPos_y = fNavPos_y;
			m_fFallingAcc = 0.f;
			Set_State_IdleStart(fDeltaTime);
		}

		vMyPos = XMVectorSetY(vMyPos, fPos_y);
		m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, vMyPos);
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

HRESULT CPlayer::Update_State_Elevator(_double fDeltaTime)
{
	return S_OK;
}

HRESULT CPlayer::Update_State_StopAction(_double fDeltaTime)
{

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
			m_pTransformCom->MovetoDir_bySpeed(m_fKnockbackDir.XMVector(), fKnockbackPower, fDeltaTime, m_pNavigationCom);
			m_pTransformCom->Turn_Dir(m_fKnockbackDir.XMVector() * -1.f, 0.7f);
		}
		else
		{
			if(0.f >= m_fHP && 0.98f <= fAnimPlayRate)
			{
				m_pMainCamera->Set_CameraMode(ECameraMode::CAM_MODE_FIX);
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

	if (true == m_bFallingDead)
	{
		m_pTransformCom->Move_Down(fDeltaTime * 2.5f);
	}

	if (0.f < fAnimPlayRate)
	{
		if (0.98f < fAnimPlayRate)
		{
			m_bUpdateAnimation = false;

			if (false == m_bOncePlaySound)
			{
				m_bOncePlaySound = true;
				g_pGameInstance->PlaySoundW(TEXT("Jino_GameOver_Sound_0.wav"), CHANNELID::CHANNEL_EFFECT, 1.f);
				g_pGameInstance->PlaySoundW(TEXT("Jino_GameOver_Sound_1.wav"), CHANNELID::CHANNEL_EFFECT, 1.f);
			}

			m_fResurrectionDelay += (_float)fDeltaTime;
			if (3.f <= m_fResurrectionDelay)
			{
				m_fResurrectionDelay = 0.f;
				Resurrection();
			}
		}
	}
	return _int();
}

HRESULT CPlayer::Update_Collider(_double fDeltaTime)
{
	m_pCollider->Update_ConflictPassedTime(fDeltaTime);
	_uint iNumCollider = m_pCollider->Get_NumColliderBuffer();
	for (_uint i = 0; i < iNumCollider; i++)
		m_pCollider->Update_Transform(i, m_vecAttachedDesc[i].Caculate_AttachedBoneMatrix_BlenderFixed());

	if (true == m_bActiveCollider)
	{
		FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_Player, this, m_pCollider));
	}

	switch (m_eCurState)
	{
	case STATE_ATTACK:
	case STATE_JUMPATTACK:
	case STATE_UTILITYSKILL:
	case STATE_ULTIMATESKILL:
		break;
	default:
		FAILED_CHECK(g_pGameInstance->Add_RepelGroup(m_pTransformCom, 0.5f, m_pNavigationCom));
		break;
	}

	m_IsConfilicted = false;


	_Matrix mat = m_pTransformCom->Get_WorldMatrix();
	_Vector vPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
	_Vector vLook = XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK));

	m_pCollider_Parkur->Update_ConflictPassedTime(fDeltaTime);
	iNumCollider = m_pCollider_Parkur->Get_NumColliderBuffer();
	mat.r[3] += _float3(0.f, 0.5f, 0.f).XMVector();
	m_pCollider_Parkur->Update_Transform(0, mat);

	mat.r[3] = vPos + (vLook * 0.14f) + _float3(0.f, 1.73f, 0.f).XMVector();
	m_pCollider_Parkur->Update_Transform(1, mat);
	
	mat.r[3] = vPos + (vLook * 0.14f) + _float3(0.f, 1.67f, 0.f).XMVector();
	m_pCollider_Parkur->Update_Transform(2, mat);

	FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_PlayerParkur, this, m_pCollider_Parkur));

	return S_OK;
}

HRESULT CPlayer::Update_HPUI(_double fDeltaTime)
{
	if (m_pHPUI != nullptr)
		m_pHPUI->Update(fDeltaTime);

	return S_OK;
}

HRESULT CPlayer::LateUpdate_HPUI(_double fDeltaTime)
{
	if (m_pHPUI != nullptr)
	{
		m_pHPUI->LateUpdate(fDeltaTime);
	}

	return S_OK;
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

void CPlayer::Check_CurNaviCellOption()
{
	m_eCurPosNavCellOption = m_pNavigationCom->Get_CurCellOption();

	if (CCell::CELL_HIDE_ON_BUSH == m_eCurPosNavCellOption)
	{
		m_bPlayerHide = true;
	}
	else
	{
		m_bPlayerHide = false;
	}
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
		m_pMainCamera->Set_CameraMode(CAM_MODE_NOMAL);
			m_pMainCamera->Lock_CamLook(false);
			m_fAttachCamPos_Offset = _float3(0.f, 1.5f, -2.f);
			m_fAttachCamLook_Offset = _float3(0.f, 0.f, 0.f);

			//m_pMainCamera->Lock_CamLook(true, XMVectorSet(0.f, -1.f, 1.f, 1.f));
			//m_fAttachCamPos_Offset = _float3(0.f, 3.f, 0.f);
			//m_fAttachCamLook_Offset = _float3(0.f, 0.f, 0.f);

		/*m_pMainCamera->Lock_CamLook(false);
		m_fAttachCamPos_Ofsfset = _float3(0.f, 0.f, -0.5f);
		m_fAttachCamLook_Offset = _float3(0.f, 0.f, 0.f);*/

		//m_iCurCamViewIndex += iInputDir;
		//if (0.f >= m_iCurCamViewIndex)
		//{
		//	m_iCurCamViewIndex = 0;
		//}
		//else if (m_iMaxCamViewIndex <= m_iCurCamViewIndex)
		//{
		//	m_iCurCamViewIndex = m_iMaxCamViewIndex - 1;
		//}

		//switch (m_iCurCamViewIndex)
		//{
		//case 0:
		//	// Base Point of view
		//	m_pMainCamera->Lock_CamLook(false);
		//	m_fAttachCamPos_Offset = _float3(0.f, 1.5f, -2.f);
		//	m_fAttachCamLook_Offset = _float3(0.f, 0.f, 0.f);
		//	break;
		//case 1:
		//	// Side Scroll Point of view
		//	m_pMainCamera->Lock_CamLook(true, XMVectorSet(0.f, 0.f, 1.f, 1.f));
		//	m_fAttachCamPos_Offset = _float3(0.f, 2.f, -2.f);
		//	m_fAttachCamLook_Offset = _float3(0.f, 0.f, 0.f);
		//	break;
		//case 2:
		//	// Snake Boss Point of view
		//	m_pMainCamera->Lock_CamLook(true, XMVectorSet(0.f, 0.f, 1.f, 1.f));
		//	m_fAttachCamPos_Offset = _float3(0.f, 5.f, -10.f);
		//	m_fAttachCamLook_Offset = _float3(0.f, 0.f, 0.f);
		//	break;
		//case 3:
		//	// First Person Point of view
		//	m_pMainCamera->Lock_CamLook(true, XMVectorSet(0.f, 0.f, 1.f, 1.f));
		//	m_fAttachCamPos_Offset = _float3(0.5f, 1.28f, 2.f);
		//	m_fAttachCamLook_Offset = _float3(0.f, 0.f, 0.f);
		//	break;
		//}
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
		m_bMehsArrow = false;
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
	CGameInstance* pGameInstance = GetSingle(CGameInstance);

	if (pGameInstance->Get_DIKeyState(DIK_1) & DIS_Down && m_eCurWeapon != EWEAPON_TYPE::WEAPON_SPEAR)
	{
		if (m_eCurWeapon > EWEAPON_TYPE::WEAPON_NONE)
		{
			if (false == m_pPlayerWeapons[m_eCurWeapon - 1]->AbleToChangeWeapon())	return true;
			m_pPlayerWeapons[m_eCurWeapon - 1]->Dissolve_Out(0.5f);
			if (m_eCurWeapon == EWEAPON_TYPE::WEAPON_SWORD)		m_pPlayerWeapons[EWEAPON_TYPE::WEAPON_SHIELD - 1]->Dissolve_Out(0.5f);
		}

		g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Swap_Weapon.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 0.7f);
		m_eCurWeapon = EWEAPON_TYPE::WEAPON_SPEAR;
		m_pPlayerWeapons[m_eCurWeapon - 1]->Dissolve_In(0.5f);

		FAILED_CHECK_NONERETURN(static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->Set_Dead_Transform(0));
		FAILED_CHECK_NONERETURN(static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->Set_Dead_Transform(1));
	}
	else if (pGameInstance->Get_DIKeyState(DIK_2) & DIS_Down && m_eCurWeapon != EWEAPON_TYPE::WEAPON_BOW)
	{
		if (m_eCurWeapon > EWEAPON_TYPE::WEAPON_NONE)
		{
			if (false == m_pPlayerWeapons[m_eCurWeapon - 1]->AbleToChangeWeapon())	return true;
			m_pPlayerWeapons[m_eCurWeapon - 1]->Dissolve_Out(0.5f);
			if (m_eCurWeapon == EWEAPON_TYPE::WEAPON_SWORD)		m_pPlayerWeapons[EWEAPON_TYPE::WEAPON_SHIELD - 1]->Dissolve_Out(0.5f);
		}

		g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Swap_Weapon.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 0.7f);
		m_eCurWeapon = EWEAPON_TYPE::WEAPON_BOW;
		m_pPlayerWeapons[m_eCurWeapon - 1]->Dissolve_In(0.5f);

		FAILED_CHECK_NONERETURN(static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->Set_Play_Particle_Must(0));
		FAILED_CHECK_NONERETURN(static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->Set_Play_Particle_Must(1));

	}
	else if (pGameInstance->Get_DIKeyState(DIK_3) & DIS_Down && m_eCurWeapon != EWEAPON_TYPE::WEAPON_SWORD)
	{
		if (m_eCurWeapon > EWEAPON_TYPE::WEAPON_NONE)
		{
			if (false == m_pPlayerWeapons[m_eCurWeapon - 1]->AbleToChangeWeapon())	return true;
			m_pPlayerWeapons[m_eCurWeapon - 1]->Dissolve_Out(0.5f);
		}

		g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Swap_Weapon.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 0.7f);
		m_eCurWeapon = EWEAPON_TYPE::WEAPON_SWORD;
		m_pPlayerWeapons[m_eCurWeapon - 1]->Dissolve_In(0.5f);
		m_pPlayerWeapons[EWEAPON_TYPE::WEAPON_SHIELD - 1]->Dissolve_In(0.5f);

		FAILED_CHECK_NONERETURN(static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->Set_Dead_Transform(0));
		FAILED_CHECK_NONERETURN(static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->Set_Dead_Transform(1));
	}
	else if (pGameInstance->Get_DIKeyState(DIK_4) & DIS_Down && m_eCurWeapon != EWEAPON_TYPE::WEAPON_CHAKRA)
	{
		if (m_eCurWeapon > EWEAPON_TYPE::WEAPON_NONE)
		{
			if (false == m_pPlayerWeapons[m_eCurWeapon - 1]->AbleToChangeWeapon())	return true;
			m_pPlayerWeapons[m_eCurWeapon - 1]->Dissolve_Out(0.1f);
			if (m_eCurWeapon == EWEAPON_TYPE::WEAPON_SWORD)		m_pPlayerWeapons[EWEAPON_TYPE::WEAPON_SHIELD - 1]->Dissolve_Out(0.1f);
		}

		g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Swap_Weapon.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 0.7f);
		m_eCurWeapon = EWEAPON_TYPE::WEAPON_CHAKRA;
		m_pPlayerWeapons[m_eCurWeapon - 1]->Dissolve_In(0.1f);

		FAILED_CHECK_NONERETURN(static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->Set_Dead_Transform(0));
		FAILED_CHECK_NONERETURN(static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->Set_Dead_Transform(1));
	}

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

	m_pTransformCom->MovetoDir(vMovDir, fMoveRate, m_pNavigationCom);

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
		//m_bActiveCollider = false;
		// Play Sound
		if (false == m_bOncePlaySound && 0.1f < fAnimPlayRate)
		{
			m_bOncePlaySound = true;
			g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Dodge_0.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 0.7f);
		}
		

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
			m_bOncePlaySound = false;
			m_bActiveCollider = true;
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
				m_bOncePlaySound = false;
				m_bActiveCollider = true;
			}
		}
		else if (0.52f <= fAnimPlayRate)
		{
			if (true == m_bReadyDodgeCombo)
			{
				m_pModel->Set_BlockAnim(false);
				m_bPlayDodgeCombo = true;
				m_bOn_MotionTrail = false;
				m_bOncePlaySound = false;
				m_bActiveCollider = true;
			}
		}
	}
		break;
	case BASE_ANIM_DODGE_CARTWHEEL:
	{
		//m_bActiveCollider = false;
		// Play Sound
		if (false == m_bOncePlaySound && 0.f < fAnimPlayRate && 0.4f > fAnimPlayRate)
		{
			m_bOncePlaySound = true;
			g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Dodge_1.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 1.f);
		}
		else if (true == m_bOncePlaySound && 0.4f <= fAnimPlayRate)
		{
			m_bOncePlaySound = false;
			g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Dodge_1_2.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 1.f);
		}

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
			m_bOncePlaySound = false;
			m_bActiveCollider = true;
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
				m_bOncePlaySound = false;
				m_bActiveCollider = true;
			}
		}
		else if (0.61f <= fAnimPlayRate)
		{
			if (true == m_bReadyDodgeCombo)
			{
				m_pModel->Set_BlockAnim(false);
				m_bPlayDodgeCombo = true;
				m_bOn_MotionTrail = false;
				m_bOncePlaySound = false;
				m_bActiveCollider = true;
			}
		}
	}
		break;
	case BASE_ANIM_DODGE_FLIP:
	{
		//m_bActiveCollider = false;
		// Play Sound
		if (false == m_bOncePlaySound && 0.f < fAnimPlayRate && 0.4f > fAnimPlayRate)
		{
			m_bOncePlaySound = true;
			g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Dodge_2.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 1.f);
		}


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
			m_bOncePlaySound = false;
			m_bActiveCollider = true;
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
				m_bOncePlaySound = false;
				m_bActiveCollider = true;
			}
		}
		else if (0.62f <= fAnimPlayRate)
		{
			if (true == m_bReadyDodgeCombo)
			{
				m_pModel->Set_BlockAnim(false);
				m_bPlayDodgeCombo = true;
				m_bOn_MotionTrail = false;
				m_bOncePlaySound = false;
				m_bActiveCollider = true;
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
		// Play Sound
		if (false == m_bOncePlaySwingSound && 0.4f < fAnimPlayRate)
		{
			m_bOncePlaySwingSound = true;
			
			g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Trishul_Pri_Fire_Swing_0.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 1.f);
			g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Trishul_Swing_1.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 0.7f);
		}

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
			m_pPlayerWeapons[WEAPON_SPEAR - 1]->Active_Collision();
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
					m_pPlayerWeapons[WEAPON_SPEAR - 1]->DeActive_Collision();
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
					m_pPlayerWeapons[WEAPON_SPEAR - 1]->DeActive_Collision();
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
		// Play Sound
		if (false == m_bOncePlaySwingSound && 0.6f < fAnimPlayRate)
		{
			m_bOncePlaySwingSound = true;
			g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Trishul_Pri_Fire_Swing_1.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 1.f);
			g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Trishul_Swing_1.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 0.7f);
		}

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
			m_pPlayerWeapons[WEAPON_SPEAR - 1]->Active_Collision();
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
					m_pPlayerWeapons[WEAPON_SPEAR - 1]->DeActive_Collision();
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
					m_pPlayerWeapons[WEAPON_SPEAR - 1]->DeActive_Collision();
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
		m_pPlayerWeapons[WEAPON_SPEAR - 1]->DeActive_Collision_3();


		// Play Sound
		if (false == m_bOncePlaySwingSound && 0.48f < fAnimPlayRate)
		{
			m_bOncePlaySwingSound = true;
			g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Trishul_Pri_Fire_Swing_2.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 1.f);
			g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Trishul_Swing_1.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 0.7f);

			m_vecNonInstMeshDesc[0].vPosition= m_vecNonInstMeshDesc[1].vPosition = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS) +
				m_pTransformCom->Get_MatrixState_Normalized(CTransform::STATE_LOOK) * 0.95f
				- m_pTransformCom->Get_MatrixState_Normalized(CTransform::STATE_UP) * 0.251f;

			m_vecNonInstMeshDesc[1].vLookDir = -m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK);
			g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_PlayerEffect), TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[0]);
			g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_PlayerEffect), TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[1]);

			m_pPlayerWeapons[WEAPON_SPEAR - 1]->Active_Collision_3();
		}

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
			

			g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Trishul_Swing_2_GroundHit.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 1.f);
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
					m_pPlayerWeapons[WEAPON_SPEAR - 1]->DeActive_Collision();
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
			CCamera_Main::CAMERASHAKEDIRDESC tCameraShakeDirDesc;
			tCameraShakeDirDesc.fTotalTime = 0.4f;
			tCameraShakeDirDesc.fPower = 10.f;
			tCameraShakeDirDesc.fChangeDirectioninterval = 0.01f;
			_float3 fShakeDir = GetSingle(CUtilityMgr)->RandomFloat3(-1.f, 1.f).XMVector();
			fShakeDir.y = 0.f;
			tCameraShakeDirDesc.fShakingDir = XMVector3Normalize(fShakeDir.XMVector());
			GetSingle(CUtilityMgr)->Get_MainCamera()->Start_CameraShaking_Dir_Thread(&tCameraShakeDirDesc, true);

			CCamera_Main::CAMERASHAKEROTDESC tCameraShakeRotDesc;
			tCameraShakeRotDesc.fTotalTime = 0.05f;
			tCameraShakeRotDesc.fPower = 1.5f;
			tCameraShakeRotDesc.fChangeDirectioninterval = 0.05f;
			tCameraShakeRotDesc.fShakingRotAxis = m_pMainCamera->Get_CamTransformCom()->Get_MatrixState(CTransform::TransformState::STATE_RIGHT);
			m_pMainCamera->Start_CameraShaking_Rot_Thread(&tCameraShakeRotDesc, false);

			m_bActive_ActionCameraShake = false;
		}
		//
	}
	break;
	case SPEAR_ANIM_MAIN_ATK_COMBO_2_JUMPATTACK:
	{
		m_pPlayerWeapons[WEAPON_SPEAR - 1]->DeActive_Collision_3();

		// Play Sound
		if (false == m_bOncePlaySwingSound && 0.025f < fAnimPlayRate && 0.2f > fAnimPlayRate)
		{
			m_bOncePlaySwingSound = true;
			g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Trishul_Pri_Fire_Swing_2.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 1.f);
			g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Trishul_Swing_1.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 0.7f);


			m_vecNonInstMeshDesc[0].vPosition = m_vecNonInstMeshDesc[1].vPosition = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS) +
				m_pTransformCom->Get_MatrixState_Normalized(CTransform::STATE_LOOK) * 0.95f
				- m_pTransformCom->Get_MatrixState_Normalized(CTransform::STATE_UP) * 0.251f;
			m_vecNonInstMeshDesc[1].vLookDir = -m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK);
			g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_PlayerEffect), TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[0]);
			g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_PlayerEffect), TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[1]);

		}
		else if (true == m_bOncePlaySwingSound && 0.2f <= fAnimPlayRate)
		{
			m_bOncePlaySwingSound = false;
			g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Trishul_Swing_2_GroundHit.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 1.f);
			m_pPlayerWeapons[WEAPON_SPEAR - 1]->Active_Collision_3();
		}

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
			CCamera_Main::CAMERASHAKEDIRDESC tCameraShakeDirDesc;
			tCameraShakeDirDesc.fTotalTime = 0.4f;
			tCameraShakeDirDesc.fPower = 10.f;
			tCameraShakeDirDesc.fChangeDirectioninterval = 0.01f;
			_float3 fShakeDir = GetSingle(CUtilityMgr)->RandomFloat3(-1.f, 1.f).XMVector();
			fShakeDir.y = 0.f;
			tCameraShakeDirDesc.fShakingDir = XMVector3Normalize(fShakeDir.XMVector());
			GetSingle(CUtilityMgr)->Get_MainCamera()->Start_CameraShaking_Dir_Thread(&tCameraShakeDirDesc, true);

			CCamera_Main::CAMERASHAKEROTDESC tCameraShakeRotDesc;
			tCameraShakeRotDesc.fTotalTime = 0.05f;
			tCameraShakeRotDesc.fPower = 1.5f;
			tCameraShakeRotDesc.fChangeDirectioninterval = 0.05f;
			tCameraShakeRotDesc.fShakingRotAxis = m_pMainCamera->Get_CamTransformCom()->Get_MatrixState(CTransform::TransformState::STATE_RIGHT);
			m_pMainCamera->Start_CameraShaking_Rot_Thread(&tCameraShakeRotDesc, false);

			m_bActive_ActionCameraShake = false;
		}
		//
	}
	break;
	case SPEAR_ANIM_MAIN_ATK_COMBO_1_JUMPATTACK:
	{
		// Play Sound
		if (false == m_bOncePlaySwingSound && 0.15f < fAnimPlayRate)
		{
			m_bOncePlaySwingSound = true;
			g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Trishul_Pri_Fire_Swing_1.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 1.f);
			g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Trishul_Swing_1.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 0.7f);
		}

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
			m_pPlayerWeapons[WEAPON_SPEAR - 1]->Active_Collision();
		}
		//

		m_fAnimSpeed = 1.2f;

		if (0.25f >= fAnimPlayRate)
		{
			_float MoveSpeed = g_pGameInstance->Easing(TYPE_SinInOut, 1.5f, 0.f, fAnimPlayRate, 0.25f);
			m_pTransformCom->Move_Forward(fDeltaTime * MoveSpeed, m_pNavigationCom);
		}
		else if (fAnimPlayRate && 0.9f < fAnimPlayRate)
		{
			m_fAnimSpeed = 1.5f;
			m_pModel->Set_BlockAnim(false);
			m_bAttackEnd = true;
			m_bTrailSwitch = false;
			m_pPlayerWeapons[WEAPON_SPEAR - 1]->Active_Trail(false);
			m_pPlayerWeapons[WEAPON_SPEAR - 1]->DeActive_Collision();
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
					m_pPlayerWeapons[WEAPON_SPEAR - 1]->DeActive_Collision();
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
					m_pPlayerWeapons[WEAPON_SPEAR - 1]->DeActive_Collision();
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
		// Play Sound
		if (false == m_bOncePlaySwingSound && 0.035f < fAnimPlayRate)
		{
			m_bOncePlaySwingSound = true;

			g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Trishul_Pri_Fire_Swing_0.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 1.f);
			g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Trishul_Swing_1.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 0.7f);
		}

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
			m_pPlayerWeapons[WEAPON_SPEAR - 1]->Active_Collision();
		}
		//

		m_fAnimSpeed = 1.5f;

		if (0.25f >= fAnimPlayRate)
		{
			_float MoveSpeed = g_pGameInstance->Easing(TYPE_SinInOut, 1.5f, 0.f, fAnimPlayRate, 0.25f);
			m_pTransformCom->Move_Forward(fDeltaTime * MoveSpeed, m_pNavigationCom);
		}
		else if (0.9f < fAnimPlayRate)
		{
			m_fAnimSpeed = 1.5f;
			m_pModel->Set_BlockAnim(false);
			m_bAttackEnd = true;
			m_bTrailSwitch = false;
			m_pPlayerWeapons[WEAPON_SPEAR - 1]->Active_Trail(false);
			m_pPlayerWeapons[WEAPON_SPEAR - 1]->DeActive_Collision();
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
					m_pPlayerWeapons[WEAPON_SPEAR - 1]->DeActive_Collision();
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
					m_pPlayerWeapons[WEAPON_SPEAR - 1]->DeActive_Collision();
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
		m_pPlayerWeapons[WEAPON_SPEAR - 1]->DeActive_Collision();
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
		if (true == m_bActionSwitch)
		{
			m_pPlayerWeapons[WEAPON_SPEAR - 1]->DeActive_Collision_2();
		}

		if (false == m_bOncePlaySwingSound && 0.4f < fAnimPlayRate)
		{
			m_bOncePlaySwingSound = true;


			m_vecNonInstMeshDesc[4].vPosition = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS)
				+ m_pTransformCom->Get_MatrixState(CTransform::STATE_UP) * 1.0f
				+ m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK) * 2.2f;
			-m_pTransformCom->Get_MatrixState(CTransform::STATE_RIGHT) * 0.5f;

			m_vecNonInstMeshDesc[4].vLookDir = -(m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK)
				+ m_pTransformCom->Get_MatrixState(CTransform::STATE_UP) * 0.5f
				+ m_pTransformCom->Get_MatrixState(CTransform::STATE_RIGHT) * 0.25f);

			g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_PlayerEffect), TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[4]);

			m_vecNonInstMeshDesc[4].vLookDir = -(m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK)
				//- m_pTransformCom->Get_MatrixState(CTransform::STATE_UP) * 1.0f
				+ m_pTransformCom->Get_MatrixState(CTransform::STATE_RIGHT) * 0.25f);

			g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_PlayerEffect), TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[4]);

			m_vecNonInstMeshDesc[4].vLookDir = -(m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK)
				+ m_pTransformCom->Get_MatrixState(CTransform::STATE_UP) * 0.5f
				- m_pTransformCom->Get_MatrixState(CTransform::STATE_RIGHT) * 0.25f);

			g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_PlayerEffect), TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[4]);


			m_vecNonInstMeshDesc[4].vLookDir = -(m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK)
				//- m_pTransformCom->Get_MatrixState(CTransform::STATE_UP) * 1.0f
				- m_pTransformCom->Get_MatrixState(CTransform::STATE_RIGHT) * 0.25f);

			g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_PlayerEffect), TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[4]);



			g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Trishul_Fire_Swing_0.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 0.7f);
		}

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
			m_pPlayerWeapons[WEAPON_SPEAR - 1]->Active_Collision_2();
			m_bActionSwitch = true;
		}
		else if (true == m_pModel->Get_IsHavetoBlockAnimChange() && fAnimPlayRate && 0.9f < fAnimPlayRate)
		{
			m_fAnimSpeed = 1.5f;
			m_pModel->Set_BlockAnim(false);
			m_bAttackEnd = true;
			m_bActionSwitch = false;
		}

		////////////////////Next Combo Check //////////////////////
		Check_NextComboCommand();

		if (true == m_bPlayNextCombo)
		{
			if (0.86f <= fAnimPlayRate)
			{
				Change_NextCombo();
				m_bActionSwitch = false;
			}
			else if (0.68f <= fAnimPlayRate)
			{
				if (false == m_bPlayNextCombo)
					return;

				if (true == m_bReadyDodgeCombo)
				{
					m_pModel->Set_BlockAnim(false);
					m_bPlayDodgeCombo = true;
					m_bActionSwitch = false;
				}
			}
		}
		/////////////////////////////////////////////////////////



		// Look At Mouse Pos 
		if (false == m_bOncePlaySwingSound)
		{
			LookAt_MousePos(0.6f);
		}
		//
	}
	break;
	case SPEAR_ANIM_POWER_ATK_COMBO_1:
	{
		if (false == m_bOncePlaySwingSound && 0.2f < fAnimPlayRate)
		{
			m_bOncePlaySwingSound = true;
			g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Trishul_Swing_0.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 1.f);
			g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Trishul_Fire_Swing_1.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 0.7f);
		}

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
		if (false == m_bOncePlaySwingSound && 0.4f < fAnimPlayRate)
		{
			m_bOncePlaySwingSound = true;
			g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Trishul_Fire_Swing_2_GroundHit.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 1.f);
			g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Trishul_Fire_Swing_2.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 0.7f);
		}

		static _bool bParticleChecker = false;

		if (true == bParticleChecker)
		{
			m_pPlayerWeapons[WEAPON_SPEAR - 1]->DeActive_Collision_1();
		}

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
			
			if (fAnimPlayRate < 0.5525f)
			{
				bParticleChecker = false;
			}
			else if (!bParticleChecker)
			{
				//m_vecTextureParticleDesc[2].SizeChageFrequency = 1;
				//m_vecTextureParticleDesc[2].EachParticleLifeTime = 0.4f;
				//m_vecTextureParticleDesc[2].ePassID = InstancePass_Distortion_DiffuseMix;
				m_vecNonInstMeshDesc[5].vPosition = m_vecTextureParticleDesc[1].vFixedPosition = m_vecTextureParticleDesc[2].vFixedPosition =
					m_vecTextureParticleDesc[3].vFixedPosition = m_vecTextureParticleDesc[4].vFixedPosition = m_vecTextureParticleDesc[5].vFixedPosition=
					m_pTransformCom->Get_MatrixState(CTransform::STATE_POS) + m_pTransformCom->Get_MatrixState_Normalized(CTransform::STATE_LOOK) * 0.85f
					+ m_pTransformCom->Get_MatrixState_Normalized(CTransform::STATE_UP) * 0.251f;

				//m_vecTextureParticleDesc[4].ePassID = InstancePass_OriginColor;
				//m_vecTextureParticleDesc[4].ParticleSize = _float3(4.f);
				//m_vecTextureParticleDesc[4].EachParticleLifeTime = 1.f;

				m_vecTextureParticleDesc[2].vFixedPosition
					= m_vecTextureParticleDesc[2].vFixedPosition.XMVector() - m_pTransformCom->Get_MatrixState_Normalized(CTransform::STATE_UP) * 0.001f;
				m_vecNonInstMeshDesc[5].vLookDir = m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK);


				GetSingle(CUtilityMgr)->Create_TextureInstance(m_eNowSceneNum, m_vecTextureParticleDesc[1]);		
				GetSingle(CUtilityMgr)->Create_TextureInstance(m_eNowSceneNum, m_vecTextureParticleDesc[2]);
				GetSingle(CUtilityMgr)->Create_TextureInstance(m_eNowSceneNum, m_vecTextureParticleDesc[3]);
				GetSingle(CUtilityMgr)->Create_TextureInstance(m_eNowSceneNum, m_vecTextureParticleDesc[4]);
				GetSingle(CUtilityMgr)->Create_TextureInstance(m_eNowSceneNum, m_vecTextureParticleDesc[5]);

				g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_PlayerEffect), TAG_OP(Prototype_NonInstanceMeshEffect),
					&m_vecNonInstMeshDesc[5]);

				bParticleChecker = true;
				m_pPlayerWeapons[WEAPON_SPEAR - 1]->Active_Collision_1();
			}
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
			//m_pMainCamera->Start_CameraShaking_Fov(58.f, 2.5f, 0.1f, true);
			//m_bActive_ActionCameraShake = false;
			CCamera_Main::CAMERASHAKEDIRDESC tCameraShakeDirDesc;
			tCameraShakeDirDesc.fTotalTime = 0.4f;
			tCameraShakeDirDesc.fPower = 10.f;
			tCameraShakeDirDesc.fChangeDirectioninterval = 0.01f;
			_float3 fShakeDir = GetSingle(CUtilityMgr)->RandomFloat3(-1.f, 1.f).XMVector();
			fShakeDir.y = 0.f;
			tCameraShakeDirDesc.fShakingDir = XMVector3Normalize(fShakeDir.XMVector());
			GetSingle(CUtilityMgr)->Get_MainCamera()->Start_CameraShaking_Dir_Thread(&tCameraShakeDirDesc, true);

			CCamera_Main::CAMERASHAKEROTDESC tCameraShakeRotDesc;
			tCameraShakeRotDesc.fTotalTime = 0.05f;
			tCameraShakeRotDesc.fPower = 2.f;
			tCameraShakeRotDesc.fChangeDirectioninterval = 0.05f;
			tCameraShakeRotDesc.fShakingRotAxis = m_pMainCamera->Get_CamTransformCom()->Get_MatrixState(CTransform::TransformState::STATE_RIGHT);
			m_pMainCamera->Start_CameraShaking_Rot_Thread(&tCameraShakeRotDesc, false);

			m_bActive_ActionCameraShake = false;
		}
		//
	}
	break;
	case SPEAR_ANIM_POWER_ATK_COMBO_0_JUMPATTACK:
	{
		// Active Damage Collider
		if (true == m_bActionSwitch)
		{
			m_pPlayerWeapons[WEAPON_SPEAR - 1]->DeActive_Collision_2();
		}
		else if(0.25f <= fAnimPlayRate)
		{
			m_pPlayerWeapons[WEAPON_SPEAR - 1]->Active_Collision_2();
			m_bActionSwitch = true;
		}
		//

		if (false == m_bOncePlaySwingSound && 0.12f < fAnimPlayRate)
		{
			m_bOncePlaySwingSound = true;

			m_vecNonInstMeshDesc[4].vPosition = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS)
				+ m_pTransformCom->Get_MatrixState(CTransform::STATE_UP) * 1.0f
				+ m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK) * 2.2f;
			-m_pTransformCom->Get_MatrixState(CTransform::STATE_RIGHT) * 0.5f;

			m_vecNonInstMeshDesc[4].vLookDir = -(m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK)
				+ m_pTransformCom->Get_MatrixState(CTransform::STATE_UP) * 0.5f
				+ m_pTransformCom->Get_MatrixState(CTransform::STATE_RIGHT) * 0.25f);

			g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_PlayerEffect), TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[4]);

			m_vecNonInstMeshDesc[4].vLookDir = -(m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK)
				//- m_pTransformCom->Get_MatrixState(CTransform::STATE_UP) * 1.0f
				+ m_pTransformCom->Get_MatrixState(CTransform::STATE_RIGHT) * 0.25f);

			g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_PlayerEffect), TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[4]);

			m_vecNonInstMeshDesc[4].vLookDir = -(m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK)
				+ m_pTransformCom->Get_MatrixState(CTransform::STATE_UP) * 0.5f
				- m_pTransformCom->Get_MatrixState(CTransform::STATE_RIGHT) * 0.25f);

			g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_PlayerEffect), TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[4]);


			m_vecNonInstMeshDesc[4].vLookDir = -(m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK)
				//- m_pTransformCom->Get_MatrixState(CTransform::STATE_UP) * 1.0f
				- m_pTransformCom->Get_MatrixState(CTransform::STATE_RIGHT) * 0.25f);

			g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_PlayerEffect), TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[4]);




			g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Trishul_Fire_Swing_0.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 0.7f);
		}

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
			m_bActionSwitch = false;
		}


		////////////////////Next Combo Check //////////////////////
		Check_NextComboCommand();

		if (true == m_bPlayNextCombo)
		{
			if (0.66f <= fAnimPlayRate)
			{
				m_bPlayJumpAttack = false;
				Change_NextCombo();
				m_bActionSwitch = false;
			}
			else if (0.6f <= fAnimPlayRate && 0.66f > fAnimPlayRate)
			{
				if (true == m_bReadyDodgeCombo)
				{
					m_pModel->Set_BlockAnim(false);
					m_bPlayDodgeCombo = true;
					m_bPlayJumpAttack = false;
					m_bActionSwitch = false;
				}
			}
		}
		/////////////////////////////////////////////////////////


		// Look At Mouse Pos
		if (false == m_bActionSwitch)
		{
			LookAt_MousePos(0.6f);
		}
		//
	}
	break;
	case SPEAR_ANIM_POWER_ATK_COMBO_1_JUMPATTACK:
	{
		if (false == m_bOncePlaySwingSound && 0.2f < fAnimPlayRate)
		{
			m_bOncePlaySwingSound = true;
			g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Trishul_Swing_0.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 1.f);
			g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Trishul_Fire_Swing_1.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 0.7f);
		}

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
		if (true == m_bOncePlaySwingSound)
		{
			m_pPlayerWeapons[WEAPON_SPEAR - 1]->DeActive_Collision_1();
		}

		if (false == m_bOncePlaySwingSound && 0.1f < fAnimPlayRate)
		{
			m_bOncePlaySwingSound = true;
			g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Trishul_Fire_Swing_2_GroundHit.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 1.f);
			g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Trishul_Fire_Swing_2.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 0.7f);

			m_vecNonInstMeshDesc[5].vPosition = m_vecTextureParticleDesc[1].vFixedPosition = m_vecTextureParticleDesc[2].vFixedPosition =
				m_vecTextureParticleDesc[3].vFixedPosition = m_vecTextureParticleDesc[4].vFixedPosition = m_vecTextureParticleDesc[5].vFixedPosition =
				m_pTransformCom->Get_MatrixState(CTransform::STATE_POS) + m_pTransformCom->Get_MatrixState_Normalized(CTransform::STATE_LOOK) * 0.85f
				+ m_pTransformCom->Get_MatrixState_Normalized(CTransform::STATE_UP) * 0.251f;

			//m_vecTextureParticleDesc[4].ePassID = InstancePass_OriginColor;
			//m_vecTextureParticleDesc[4].ParticleSize = _float3(4.f);
			//m_vecTextureParticleDesc[4].EachParticleLifeTime = 1.f;

			m_vecTextureParticleDesc[2].vFixedPosition
				= m_vecTextureParticleDesc[2].vFixedPosition.XMVector() - m_pTransformCom->Get_MatrixState_Normalized(CTransform::STATE_UP) * 0.001f;

			m_vecNonInstMeshDesc[5].vLookDir = m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK);

			GetSingle(CUtilityMgr)->Create_TextureInstance(m_eNowSceneNum, m_vecTextureParticleDesc[1]);
			GetSingle(CUtilityMgr)->Create_TextureInstance(m_eNowSceneNum, m_vecTextureParticleDesc[2]);
			GetSingle(CUtilityMgr)->Create_TextureInstance(m_eNowSceneNum, m_vecTextureParticleDesc[3]);
			GetSingle(CUtilityMgr)->Create_TextureInstance(m_eNowSceneNum, m_vecTextureParticleDesc[4]);
			GetSingle(CUtilityMgr)->Create_TextureInstance(m_eNowSceneNum, m_vecTextureParticleDesc[5]);
			g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_PlayerEffect), TAG_OP(Prototype_NonInstanceMeshEffect),
				&m_vecNonInstMeshDesc[5]);

			m_pPlayerWeapons[WEAPON_SPEAR - 1]->Active_Collision_1();
		}

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
			m_bOncePlaySwingSound = false;
		}


		////////////////////Next Combo Check //////////////////////
		Check_NextComboCommand();

		if (true == m_bPlayNextCombo)
		{
			if (0.66f <= fAnimPlayRate)
			{
				m_bPlayJumpAttack = false;
				Change_NextCombo();
				m_bOncePlaySwingSound = false;
			}
			else if (0.6f <= fAnimPlayRate && 0.66f > fAnimPlayRate)
			{
				if (true == m_bReadyDodgeCombo)
				{
					m_pModel->Set_BlockAnim(false);
					m_bPlayDodgeCombo = true;
					m_bPlayJumpAttack = false;
					m_bOncePlaySwingSound = false;
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
			CCamera_Main::CAMERASHAKEDIRDESC tCameraShakeDirDesc;
			tCameraShakeDirDesc.fTotalTime = 0.4f;
			tCameraShakeDirDesc.fPower = 10.f;
			tCameraShakeDirDesc.fChangeDirectioninterval = 0.01f;
			_float3 fShakeDir = GetSingle(CUtilityMgr)->RandomFloat3(-1.f, 1.f).XMVector();
			fShakeDir.y = 0.f;
			tCameraShakeDirDesc.fShakingDir = XMVector3Normalize(fShakeDir.XMVector());
			GetSingle(CUtilityMgr)->Get_MainCamera()->Start_CameraShaking_Dir_Thread(&tCameraShakeDirDesc, true);


			CCamera_Main::CAMERASHAKEROTDESC tCameraShakeRotDesc;
			tCameraShakeRotDesc.fTotalTime = 0.05f;
			tCameraShakeRotDesc.fPower = 2.f;
			tCameraShakeRotDesc.fChangeDirectioninterval = 0.05f;
			tCameraShakeRotDesc.fShakingRotAxis = m_pMainCamera->Get_CamTransformCom()->Get_MatrixState(CTransform::TransformState::STATE_RIGHT);
			m_pMainCamera->Start_CameraShaking_Rot_Thread(&tCameraShakeRotDesc, false);

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
			if (false == m_bOncePlaySound)
			{
				m_bOncePlaySound = true;
				g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Bow_Pull.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 0.7f);
			}

			m_bOnNavigation = true;

			m_fAnimSpeed = 1.f;

			if (0.307f < m_pModel->Get_PlayRate() && false == m_bAnimChangeSwitch)
			{
				m_bAnimChangeSwitch = true;

				CPlayerWeapon::PlayerWeaponDesc eWeaponDesc;
				eWeaponDesc.eAttachedDesc.Initialize_AttachedDesc(this, "skd_r_palm", _float3(1, 1, 1), _float3(0, 0, 0), _float3(0.f, 0.f, 0.f));
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
				m_bOncePlaySound = false;

				g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Arrow_Charging.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 0.7f);
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

			if (m_fChargingTime > 0.1f)
			{
			//	m_vecTextureParticleDesc[2].bEmissive = _float3(0);
			//	m_vecTextureParticleDesc[3].;
				FAILED_CHECK_NONERETURN(static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->Set_Play_Particle(2));
				FAILED_CHECK_NONERETURN(static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->Set_Play_Particle(3));

				_Matrix TargetMat = m_pTransformCom->Get_WorldMatrix();
				TargetMat.r[0] = XMVector3Normalize(TargetMat.r[0]);
				TargetMat.r[1] = XMVector3Normalize(TargetMat.r[1]);
				_Sfloat3 Look = TargetMat.r[2] = XMVector3Normalize(TargetMat.r[2]);
				_Sfloat3 FixOffset = _Sfloat3(0, -0.3f, -0.3f);
				_Vector FixPos = TargetMat.r[3] + TargetMat.r[0] * FixOffset.x + TargetMat.r[1] * FixOffset.y + TargetMat.r[2] * FixOffset.z;
				INSTPARTICLEDESC& value4 = static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->Get_VecParticle(4);				
				value4.vPowerDirection = Look;
				static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->Set_VecParticle(4, value4);

				FAILED_CHECK_NONERETURN(static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->Set_Play_Particle(4, FixPos));

			}
			if (m_fChargingTime > 1.f)
			{
				m_fArrowRange = 30.f;
			
				if (m_bMehsArrow == false)
				{
					CTransform* effecttrans = static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->Get_EffectTransform();

					FAILED_CHECK_NONERETURN(static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->Set_Play_MeshParticle(
						CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_ARROW_BOW1, effecttrans, &m_bMehsArrow));
				}
				
			}

			else
			{
				m_pMainCamera->Start_CameraShaking_Thread(0.1f, 2.f - m_fChargingTime, 0.015f, true);
				m_fArrowRange = 12.f;
			}
			//
			 
			if (false == m_bPressedMainAttackKey)
			{
				g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Bow_Shoot.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 0.7f);
				if (30.f > m_fArrowRange)
				{
					g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Bow_Normal_Shoot.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 1.f);
				}
				else
				{
					g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Bow_Charging_Shoot.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 0.7f);
				}

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
			m_bMehsArrow = false;
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
			// Play Sound
			if (false == m_bOncePlaySound && 0.15f <= fAnimPlayRate)
			{
				{
					CTransform* effecttrans = static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->Get_EffectTransform();
					FAILED_CHECK_NONERETURN(static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])
						->Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_ARROW_BOW_R, effecttrans));
				}

				m_bOncePlaySound = true;
				g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Arrow_Power_0.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 1.f);
			}

			m_bOnNavigation = true;

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

				FAILED_CHECK_NONERETURN(static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->Set_Play_Particle(5));

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
				m_pTransformCom->Move_Forward(fDeltaTime * MoveSpeed, m_pNavigationCom);
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
					m_bOncePlaySound = false;
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
						m_bOncePlaySound = false;
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
						m_bOncePlaySound = false;
					}
				}
			}
			/////////////////////////////////////////////////////////



			// Look At Mouse Pos 
			if (0.2f >= fAnimPlayRate)
			{
				LookAt_MousePos(0.6f);
			}
			//

		}
		break;
		case BOW_ANIM_POWER_COMBO_1:
		case BOW_ANIM_POWER_COMBO_1_JUMP:
		{
			// Play Sound
			if (false == m_bOncePlaySound && 0.8f <= fAnimPlayRate)
			{
				m_bOncePlaySound = true;
				g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Arrow_Power_2.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 1.f);
				
				{
					CTransform* effecttrans = static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->Get_EffectTransform();
					FAILED_CHECK_NONERETURN(static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])
						->Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_ARROW_BOW_R, effecttrans));

					FAILED_CHECK_NONERETURN(static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->Set_Play_MeshParticle(
						CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_ARROW_BOW_R_JUMP_WING1, m_pTextureParticleTransform));

					FAILED_CHECK_NONERETURN(static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->Set_Play_MeshParticle(
						CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_ARROW_BOW_R_JUMP_WING2, m_pTextureParticleTransform));

					FAILED_CHECK_NONERETURN(static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->Set_Play_Particle(5));


				}
			}

			m_bOnNavigation = true;

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

				_Matrix TargetMat = m_pTransformCom->Get_WorldMatrix();
				FAILED_CHECK_NONERETURN(static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->Set_Play_Particle(6, TargetMat.r[3]));


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
				m_bOncePlaySound = false;
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
					m_bOncePlaySound = false;
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
						m_bOncePlaySound = false;
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
						m_bOncePlaySound = false;
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
			// Play Sound
			if (false == m_bOncePlaySound && 0.52f <= fAnimPlayRate)
			{
				m_bOncePlaySound = true;
				g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Arrow_Power_1.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 1.f);

				{
					CTransform* effecttrans = static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->Get_EffectTransform();
					FAILED_CHECK_NONERETURN(static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])
						->Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_ARROW_BOW_R, effecttrans));

				}
			}

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
				m_pTransformCom->Move_Forward(fDeltaTime * MoveSpeed, m_pNavigationCom);
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
					m_bOncePlaySound = false;
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
						m_bOncePlaySound = false;
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
						m_bOncePlaySound = false;
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
			// Play Sound
			if (false == m_bOncePlaySound && 0.2f <= fAnimPlayRate)
			{
				m_bOncePlaySound = true;
				g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Arrow_Power_1.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 1.f);

				FAILED_CHECK_NONERETURN(static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->Set_Play_MeshParticle(
					CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_ARROW_BOW_R_JUMP_WING1, m_pTextureParticleTransform));

				FAILED_CHECK_NONERETURN(static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->Set_Play_MeshParticle(
					CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_ARROW_BOW_R_JUMP_WING2, m_pTextureParticleTransform));

			}

			m_bOnNavigation = true;

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
				m_pTransformCom->Move_Forward(fDeltaTime * MoveSpeed, m_pNavigationCom);
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
		m_bOnNavigation = true;
		//// Main Attack ////
	case SWORD_ANIM_MAIN_ATK_COMBO_0:
	{


		// Play Sound
		if (false == m_bOncePlaySound && 0.2f < fAnimPlayRate)
		{
			m_bOncePlaySound = true;
			g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Sword_Swing_0.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 0.7f);
			g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Sword_Swing_Thunder_0.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 0.3f);
		}


		//// Turn On Weapon Trail
		if (true == m_bTrailSwitch && m_pModel->Get_PlayRate() >= 0.6f)
		{
			m_pPlayerWeapons[WEAPON_SWORD - 1]->Active_Trail(false);
			m_pPlayerWeapons[WEAPON_SWORD - 1]->DeActive_Collision();
		}
		else if (false == m_bTrailSwitch && m_pModel->Get_PlayRate() >= 0.339f)
		{
			m_bTrailSwitch = true;
			m_pPlayerWeapons[WEAPON_SWORD - 1]->Active_Trail(true);
			m_pPlayerWeapons[WEAPON_SWORD - 1]->Active_Collision();
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
			m_pPlayerWeapons[WEAPON_SWORD - 1]->DeActive_Collision();
			m_bOncePlaySound = false;
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
					m_pPlayerWeapons[WEAPON_SWORD - 1]->DeActive_Collision();
					m_bOncePlaySound = false;
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
					m_pPlayerWeapons[WEAPON_SWORD - 1]->DeActive_Collision();
					m_bOncePlaySound = false;
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
		// Play Sound
		if (false == m_bOncePlaySound && 0.5f < fAnimPlayRate)
		{
			m_bOncePlaySound = true;
			g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Sword_Swing_1.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 0.7f);
			g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Sword_Swing_Thunder_1.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 0.3f);
		}

		m_bOnNavigation = true;
		//// Turn On Weapon Trail
		if (true == m_bTrailSwitch && m_pModel->Get_PlayRate() >= 0.694f)
		{
			m_pPlayerWeapons[WEAPON_SWORD - 1]->Active_Trail(false);
			m_pPlayerWeapons[WEAPON_SWORD - 1]->DeActive_Collision();
		}
		else if (false == m_bTrailSwitch && m_pModel->Get_PlayRate() >= 0.472f)
		{
			m_bTrailSwitch = true;
			m_pPlayerWeapons[WEAPON_SWORD - 1]->Active_Trail(true);
			m_pPlayerWeapons[WEAPON_SWORD - 1]->Active_Collision();
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
			m_pTransformCom->Move_Forward(fDeltaTime * MoveSpeed, m_pNavigationCom);
		}
		else if (0.9f < fAnimPlayRate)
		{
			m_fAnimSpeed = 1.5f;
			m_pModel->Set_BlockAnim(false);
			m_bAttackEnd = true;
			m_bTrailSwitch = false;
			m_pPlayerWeapons[WEAPON_SWORD - 1]->Active_Trail(false);
			m_pPlayerWeapons[WEAPON_SWORD - 1]->DeActive_Collision();
			m_bOncePlaySound = false;
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
					m_pPlayerWeapons[WEAPON_SWORD - 1]->DeActive_Collision();
					m_bOncePlaySound = false;
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
					m_pPlayerWeapons[WEAPON_SWORD - 1]->DeActive_Collision();
					m_bOncePlaySound = false;
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
		m_pPlayerWeapons[WEAPON_SWORD - 1]->DeActive_Collision_1();
		// Play Sound
		if (false == m_bOncePlaySound && 0.45f < fAnimPlayRate && m_pModel->Get_PlayRate() < 0.512f)
		{
			m_bOncePlaySound = true;
			g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Sword_Swing_2.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 0.7f);
			g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Sword_Swing_Thunder_0.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 0.3f);
		}
		else if (true == m_bOncePlaySound && m_pModel->Get_PlayRate() >= 0.512f)
		{
			m_bOncePlaySound = false;
			g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Sword_Swing_2_GroundHit.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 0.7f);
		

			m_vecNonInstMeshDesc[12].vPosition = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS) +
				m_pTransformCom->Get_MatrixState(CTransform::STATE_UP) * 2.5f
				+ m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK) * 1.5f;
			m_vecNonInstMeshDesc[12].vLookDir = -m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK);
			g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_PlayerEffect),
				TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[12]);

			m_vecNonInstMeshDesc[13].vPosition = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS)
				+ m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK) * 1.5f;
			m_vecNonInstMeshDesc[13].vLookDir = -m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK);
			g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_PlayerEffect),
				TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[13]);

			m_vecNonInstMeshDesc[14].vPosition = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS) +
				m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK) * 1.f +
				m_pTransformCom->Get_MatrixState(CTransform::STATE_UP) * 1.25f;
			m_vecNonInstMeshDesc[14].vLookDir = m_vecNonInstMeshDesc[14].vPosition.XMVector() - (
				m_pTransformCom->Get_MatrixState(CTransform::STATE_POS) -
				m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK) * 1.f +
				m_pTransformCom->Get_MatrixState(CTransform::STATE_UP) * 100000.f);

			g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_PlayerEffect),
				TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[14]);
		
			m_pPlayerWeapons[WEAPON_SWORD - 1]->Active_Collision_1();
		}

		m_bOnNavigation = true;
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
			m_pTransformCom->Move_Forward(fDeltaTime * MoveSpeed, m_pNavigationCom);
		}
		else if (0.9f < fAnimPlayRate)
		{
			m_fAnimSpeed = 1.5f;
			m_pModel->Set_BlockAnim(false);
			m_bAttackEnd = true;
			m_bTrailSwitch = false;
			m_pPlayerWeapons[WEAPON_SWORD - 1]->Active_Trail(false);
			m_pPlayerWeapons[WEAPON_SWORD - 1]->DeActive_Collision_1();
			m_bOncePlaySound = false;
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
					m_pPlayerWeapons[WEAPON_SWORD - 1]->DeActive_Collision_1();
					m_bOncePlaySound = false;
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
					m_pPlayerWeapons[WEAPON_SWORD - 1]->DeActive_Collision_1();
					m_bOncePlaySound = false;
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
			CCamera_Main::CAMERASHAKEDIRDESC tCameraShakeDirDesc;
			tCameraShakeDirDesc.fTotalTime = 0.4f;
			tCameraShakeDirDesc.fPower = 10.f;
			tCameraShakeDirDesc.fChangeDirectioninterval = 0.01f;
			_float3 fShakeDir = GetSingle(CUtilityMgr)->RandomFloat3(-1.f, 1.f).XMVector();
			fShakeDir.y = 0.f;
			tCameraShakeDirDesc.fShakingDir = XMVector3Normalize(fShakeDir.XMVector());
			GetSingle(CUtilityMgr)->Get_MainCamera()->Start_CameraShaking_Dir_Thread(&tCameraShakeDirDesc, true);

			CCamera_Main::CAMERASHAKEROTDESC tCameraShakeRotDesc;
			tCameraShakeRotDesc.fTotalTime = 0.05f;
			tCameraShakeRotDesc.fPower = 1.5f;
			tCameraShakeRotDesc.fChangeDirectioninterval = 0.05f;
			tCameraShakeRotDesc.fShakingRotAxis = m_pMainCamera->Get_CamTransformCom()->Get_MatrixState(CTransform::TransformState::STATE_RIGHT);
			m_pMainCamera->Start_CameraShaking_Rot_Thread(&tCameraShakeRotDesc, false);

			m_pMainCamera->Start_CameraShaking_Fov(57.f, 1.f, 0.1f, true);
			m_bActive_ActionCameraShake = false;
		}
		//
	}
	break;
	case SWORD_ANIM_MAIN_ATK_COMBO_2_JUMPATTACK:
	{
		//m_pPlayerWeapons[WEAPON_SWORD - 1]->DeActive_Collision_1();
		// Play Sound
		if (false == m_bOncePlaySound && 0.1f < fAnimPlayRate && m_pModel->Get_PlayRate() < 0.357f)
		{
			m_bOncePlaySound = true;
			g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Sword_Swing_2.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 0.7f);
			g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Sword_Swing_Thunder_0.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 0.3f);
		}
		else if (true == m_bOncePlaySound && m_pModel->Get_PlayRate() >= 0.357f)
		{
			m_bOncePlaySound = false;
			g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Sword_Swing_2_GroundHit.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 0.7f);
	

			m_vecNonInstMeshDesc[12].vPosition = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS) +
				m_pTransformCom->Get_MatrixState(CTransform::STATE_UP) * 2.5f
				+ m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK) * 1.5f;
			m_vecNonInstMeshDesc[12].vLookDir = -m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK);
			g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_PlayerEffect),
				TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[12]);

			m_vecNonInstMeshDesc[13].vPosition = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS)
				+ m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK) * 1.5f;
			m_vecNonInstMeshDesc[13].vLookDir = -m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK);
			g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_PlayerEffect),
				TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[13]);

			m_vecNonInstMeshDesc[14].vPosition = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS) +
				m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK) * 1.f +
				m_pTransformCom->Get_MatrixState(CTransform::STATE_UP) * 1.25f;
			m_vecNonInstMeshDesc[14].vLookDir = m_vecNonInstMeshDesc[14].vPosition.XMVector() - (
				m_pTransformCom->Get_MatrixState(CTransform::STATE_POS) -
				m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK) * 1.f +
				m_pTransformCom->Get_MatrixState(CTransform::STATE_UP) * 100000.f);

			g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_PlayerEffect),
				TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[14]);

			m_pPlayerWeapons[WEAPON_SWORD - 1]->Active_Collision_1();
		}

		m_bOnNavigation = true;
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
			m_pPlayerWeapons[WEAPON_SWORD - 1]->DeActive_Collision();
		}
		else if (false == m_bTrailSwitch && m_pModel->Get_PlayRate() > 0.107f)
		{
			m_bTrailSwitch = true;
			m_pPlayerWeapons[WEAPON_SWORD - 1]->Active_Trail(true);
			m_pPlayerWeapons[WEAPON_SWORD - 1]->Active_Collision();
		}
		////

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
			m_pPlayerWeapons[WEAPON_SWORD - 1]->Active_Trail(false);
			m_pPlayerWeapons[WEAPON_SWORD - 1]->DeActive_Collision();
			m_bOncePlaySound = false;
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
					m_pPlayerWeapons[WEAPON_SWORD - 1]->DeActive_Collision();
					m_bOncePlaySound = false;
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
					m_pPlayerWeapons[WEAPON_SWORD - 1]->DeActive_Collision();
					m_bOncePlaySound = false;
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
			CCamera_Main::CAMERASHAKEDIRDESC tCameraShakeDirDesc;
			tCameraShakeDirDesc.fTotalTime = 0.4f;
			tCameraShakeDirDesc.fPower = 10.f;
			tCameraShakeDirDesc.fChangeDirectioninterval = 0.01f;
			_float3 fShakeDir = GetSingle(CUtilityMgr)->RandomFloat3(-1.f, 1.f).XMVector();
			fShakeDir.y = 0.f;
			tCameraShakeDirDesc.fShakingDir = XMVector3Normalize(fShakeDir.XMVector());
			GetSingle(CUtilityMgr)->Get_MainCamera()->Start_CameraShaking_Dir_Thread(&tCameraShakeDirDesc, true);

			CCamera_Main::CAMERASHAKEROTDESC tCameraShakeRotDesc;
			tCameraShakeRotDesc.fTotalTime = 0.05f;
			tCameraShakeRotDesc.fPower = 1.5f;
			tCameraShakeRotDesc.fChangeDirectioninterval = 0.05f;
			tCameraShakeRotDesc.fShakingRotAxis = m_pMainCamera->Get_CamTransformCom()->Get_MatrixState(CTransform::TransformState::STATE_RIGHT);
			m_pMainCamera->Start_CameraShaking_Rot_Thread(&tCameraShakeRotDesc, false);

			m_pMainCamera->Start_CameraShaking_Fov(57.f, 1.f, 0.1f, true);
			m_bActive_ActionCameraShake = false;
		}
		//
	}
	break;
	case SWORD_ANIM_MAIN_ATK_COMBO_1_JUMPATTACK:
	{
		// Play Sound
		if (false == m_bOncePlaySound && 0.2f < fAnimPlayRate)
		{
			m_bOncePlaySound = true;
			g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Sword_Swing_1.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 0.7f);
			g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Sword_Swing_Thunder_1.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 0.3f);
		}

		m_bOnNavigation = true;
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
			m_pPlayerWeapons[WEAPON_SWORD - 1]->DeActive_Collision();
		}
		else if (false == m_bTrailSwitch && m_pModel->Get_PlayRate() > 0.28f)
		{
			m_bTrailSwitch = true;
			m_pPlayerWeapons[WEAPON_SWORD - 1]->Active_Trail(true);
			m_pPlayerWeapons[WEAPON_SWORD - 1]->Active_Collision();
		}
		////

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
			m_pPlayerWeapons[WEAPON_SWORD - 1]->Active_Trail(false);
			m_pPlayerWeapons[WEAPON_SWORD - 1]->DeActive_Collision();
			m_bOncePlaySound = false;
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
					m_pPlayerWeapons[WEAPON_SWORD - 1]->DeActive_Collision();
					m_bOncePlaySound = false;
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
					m_pPlayerWeapons[WEAPON_SWORD - 1]->DeActive_Collision();
					m_bOncePlaySound = false;
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
		// Play Sound
		if (false == m_bOncePlaySound && 0.2f < fAnimPlayRate)
		{
			m_bOncePlaySound = true;
			g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Sword_Swing_0.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 0.7f);
			g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Sword_Swing_Thunder_0.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 0.3f);
		}

		m_bOnNavigation = true;

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
			m_pPlayerWeapons[WEAPON_SWORD - 1]->DeActive_Collision();
		}
		else if (false == m_bTrailSwitch && m_pModel->Get_PlayRate() > 0.15f)
		{
			m_bTrailSwitch = true;
			m_pPlayerWeapons[WEAPON_SWORD - 1]->Active_Trail(true);
			m_pPlayerWeapons[WEAPON_SWORD - 1]->Active_Collision();
		}
		////

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
			m_pPlayerWeapons[WEAPON_SWORD - 1]->Active_Trail(false);
			m_pPlayerWeapons[WEAPON_SWORD - 1]->DeActive_Collision();
			m_bOncePlaySound = false;
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
					m_pPlayerWeapons[WEAPON_SWORD - 1]->DeActive_Collision();
					m_bOncePlaySound = false;
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
					m_pPlayerWeapons[WEAPON_SWORD - 1]->DeActive_Collision();
					m_bOncePlaySound = false;
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
		// Play Sound
		if (false == m_bOncePlaySound && 0.1f < fAnimPlayRate)
		{
			m_vecNonInstMeshDesc[11].vPosition = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS) +
				m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK) * 0.5f +
				m_pTransformCom->Get_MatrixState(CTransform::STATE_UP) * 1.f;

			m_vecNonInstMeshDesc[11].vLookDir = m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK);

			g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_PlayerEffect),
				TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[11]);


			m_bOncePlaySound = true; 
			g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Sword_PowerSwing_0.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 0.7f);
			g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Sword_Swing_Thunder_2.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 0.3f);

			static_cast<CPlayerWeapon_Sword*>(m_pPlayerWeapons[WEAPON_SWORD - 1])->Set_ShieldBashAttack(true);
		}
		
		if(0.1f > fAnimPlayRate)
		{
			LookAt_MousePos(0.6f);	
		}

		m_bOnNavigation = true;

		m_fAnimSpeed = 1.f;

		if (0.f < fAnimPlayRate && 0.277f >= fAnimPlayRate)
		{
			_float MoveSpeed = g_pGameInstance->Easing(TYPE_QuadOut, 2.5f, 0.f, fAnimPlayRate, 0.277f);
			m_pTransformCom->Move_Forward(fDeltaTime * MoveSpeed, m_pNavigationCom);
		}
		else if (0.277f < fAnimPlayRate && 0.9f > fAnimPlayRate)
		{
			m_pPlayerWeapons[WEAPON_SWORD - 1]->Active_Collision();
			m_fAnimSpeed = 2.f;
		}
		else if (0.9f < fAnimPlayRate)
		{
			m_fAnimSpeed = 1.5f;
			m_pModel->Set_BlockAnim(false);
			m_bAttackEnd = true;
			m_pPlayerWeapons[WEAPON_SWORD - 1]->DeActive_Collision();
			static_cast<CPlayerWeapon_Sword*>(m_pPlayerWeapons[WEAPON_SWORD - 1])->Set_ShieldBashAttack(false);
			m_bOncePlaySound = false;
		}

		////////////////////Next Combo Check //////////////////////
		Check_NextComboCommand();

		if (true == m_bPlayNextCombo)
		{
			if (0.72f <= fAnimPlayRate)
			{
				Change_NextCombo();
				m_pPlayerWeapons[WEAPON_SWORD - 1]->DeActive_Collision();
				static_cast<CPlayerWeapon_Sword*>(m_pPlayerWeapons[WEAPON_SWORD - 1])->Set_ShieldBashAttack(false);
				m_bOncePlaySound = false;
			}
			else if (0.65f <= fAnimPlayRate)
			{
				if (false == m_bPlayNextCombo)
					return;

				if (true == m_bReadyDodgeCombo)
				{
					m_pModel->Set_BlockAnim(false);
					m_bPlayDodgeCombo = true;
					m_pPlayerWeapons[WEAPON_SWORD - 1]->DeActive_Collision();
					static_cast<CPlayerWeapon_Sword*>(m_pPlayerWeapons[WEAPON_SWORD - 1])->Set_ShieldBashAttack(false);
					m_bOncePlaySound = false;
				}
			}
		}
		/////////////////////////////////////////////////////////


	}
	break;
	case SWORD_ANIM_POWER_ATK_COMBO_1:
	{
		m_bOnNavigation = true;

		// 
		if (false == m_bAnimChangeSwitch)
		{
			m_bAnimChangeSwitch = true;
			_Vector vThrowStartPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
			vThrowStartPos = XMVectorSetY(vThrowStartPos, XMVectorGetY(vThrowStartPos) + 1.f);
			_Vector vPlayerLook = XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK));
			vThrowStartPos += vPlayerLook * 1.5f;
			static_cast<CPlayerWeapon_Shield*>(m_pPlayerWeapons[WEAPON_SHIELD - 1])->Start_ThrowMode(vThrowStartPos, 5.f);

			m_pMainCamera->Start_CameraShaking_Fov(57.f, 1.f, 0.1f, true);

			g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Shield_Throw.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 0.7f);
			g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Shield_Throw_Elect.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 0.7f);
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
			m_pTransformCom->Move_Forward(fDeltaTime * MoveSpeed, m_pNavigationCom);
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
		// Play Sound
		if (false == m_bOncePlaySound && 0.3f < fAnimPlayRate)
		{
			m_bOncePlaySound = true;
			g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Sword_Swing_2.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 0.7f);
			g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Sword_PowerSwing_2_GroundHit.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 0.7f);
		}

		m_bOnNavigation = true;

		if (false == m_bActionSwitch && 0.4f < fAnimPlayRate)
		{
			m_bActionSwitch = true;
		}

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
			m_bActionSwitch = false;
			m_bOncePlaySound = false;
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
					m_bActionSwitch = false;
					m_bOncePlaySound = false;
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
					m_bActionSwitch = false;
					m_bOncePlaySound = false;
				}
			}
		}
		/////////////////////////////////////////////////////////

		// Active CameraShake
		if (true == m_bActive_ActionCameraShake && 0.4f <= fAnimPlayRate)
		{
			CCamera_Main::CAMERASHAKEDIRDESC tCameraShakeDirDesc;
			tCameraShakeDirDesc.fTotalTime = 0.4f;
			tCameraShakeDirDesc.fPower = 10.f;
			tCameraShakeDirDesc.fChangeDirectioninterval = 0.02f;
			_float3 fShakeDir = GetSingle(CUtilityMgr)->RandomFloat3(-1.f, 1.f).XMVector();
			fShakeDir.y = 0.f;
			tCameraShakeDirDesc.fShakingDir = XMVector3Normalize(fShakeDir.XMVector());
			GetSingle(CUtilityMgr)->Get_MainCamera()->Start_CameraShaking_Dir_Thread(&tCameraShakeDirDesc, true);

			CCamera_Main::CAMERASHAKEROTDESC tCameraShakeRotDesc;
			tCameraShakeRotDesc.fTotalTime = 0.05f;
			tCameraShakeRotDesc.fPower = 1.5f;
			tCameraShakeRotDesc.fChangeDirectioninterval = 0.05f;
			tCameraShakeRotDesc.fShakingRotAxis = m_pMainCamera->Get_CamTransformCom()->Get_MatrixState(CTransform::TransformState::STATE_RIGHT);
			m_pMainCamera->Start_CameraShaking_Rot_Thread(&tCameraShakeRotDesc, false);

			m_bActive_ActionCameraShake = false;			
			m_pPlayerWeapons[WEAPON_SWORD - 1]->EffectParticleOn(0, &m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_LOOK));


		}
		//
	}
	break;
	case SWORD_ANIM_POWER_ATK_COMBO_0_JUMPATTACK:
	{
		// Play Sound
		if (false == m_bOncePlaySound && 0.1f < fAnimPlayRate)
		{
			m_vecNonInstMeshDesc[11].vPosition = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS) +
				m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK) * 0.5f +
				m_pTransformCom->Get_MatrixState(CTransform::STATE_UP) * 1.f;

			m_vecNonInstMeshDesc[11].vLookDir = m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK);

			g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_PlayerEffect),
				TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[11]);


			m_bOncePlaySound = true;
			g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Sword_PowerSwing_0.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 0.7f);
			g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Sword_Swing_Thunder_2.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 0.3f);
		}


		if (0.1f > fAnimPlayRate)
		{
			LookAt_MousePos(0.6f);
		}

		m_bOnNavigation = true;

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
			m_pTransformCom->Move_Forward(fDeltaTime * MoveSpeed, m_pNavigationCom);
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
			m_bOncePlaySound = false;
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
					m_bOncePlaySound = false;
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
		m_bOnNavigation = true;

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

			m_pMainCamera->Start_CameraShaking_Fov(57.f, 1.f, 0.1f, true);

			g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Shield_Throw.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 0.7f);
			g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Shield_Throw_Elect.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 0.7f);
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
			m_pTransformCom->Move_Forward(fDeltaTime * MoveSpeed, m_pNavigationCom);
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
		// Play Sound
		if (false == m_bOncePlaySound && 0.05f < fAnimPlayRate)
		{
			m_bOncePlaySound = true;
			g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Sword_Swing_2.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 0.7f);
			g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Sword_PowerSwing_2_GroundHit.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 0.7f);
		}


		m_bOnNavigation = true;

		if (0.1f > fAnimPlayRate)
		{
			m_bOn_MotionTrail = true;
		}
		else
		{
			m_bOn_MotionTrail = false;
		}


		m_fAnimSpeed = 1.5f;

		if (false == m_bAnimChangeSwitch && 0.12f <= fAnimPlayRate)
		{
			_Vector vPutOnPos = (XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK)) * 1.7f) + m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
			static_cast<CPlayerWeapon_Shield*>(m_pPlayerWeapons[WEAPON_SHIELD - 1])->Start_SmashMode(vPutOnPos);
			m_bAnimChangeSwitch = true;
		}

		if (0.25f >= fAnimPlayRate) 
		{
			_float MoveSpeed = g_pGameInstance->Easing(TYPE_SinInOut, 1.5f, 0.f, fAnimPlayRate, 0.25f);
			m_pTransformCom->Move_Forward(fDeltaTime * MoveSpeed, m_pNavigationCom);
		}
		else if (0.9f < fAnimPlayRate)
		{
			m_fAnimSpeed = 1.5f;
			m_pModel->Set_BlockAnim(false);
			m_bAttackEnd = true;
			m_bAnimChangeSwitch = false;
			static_cast<CPlayerWeapon_Shield*>(m_pPlayerWeapons[WEAPON_SHIELD - 1])->End_SmashMode();
			m_bOncePlaySound = false;
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
					m_bOncePlaySound = false;

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
					m_bOncePlaySound = false;
				}
			}
		}
		/////////////////////////////////////////////////////////

		// Active CameraShake
		if (true == m_bActive_ActionCameraShake && 0.15f <= fAnimPlayRate)
		{
			CCamera_Main::CAMERASHAKEDIRDESC tCameraShakeDirDesc;
			tCameraShakeDirDesc.fTotalTime = 0.4f;
			tCameraShakeDirDesc.fPower = 10.f;
			tCameraShakeDirDesc.fChangeDirectioninterval = 0.02f;
			_float3 fShakeDir = GetSingle(CUtilityMgr)->RandomFloat3(-1.f, 1.f).XMVector();
			fShakeDir.y = 0.f;
			tCameraShakeDirDesc.fShakingDir = XMVector3Normalize(fShakeDir.XMVector());
			GetSingle(CUtilityMgr)->Get_MainCamera()->Start_CameraShaking_Dir_Thread(&tCameraShakeDirDesc, true);

			CCamera_Main::CAMERASHAKEROTDESC tCameraShakeRotDesc;
			tCameraShakeRotDesc.fTotalTime = 0.05f;
			tCameraShakeRotDesc.fPower = 1.5f;
			tCameraShakeRotDesc.fChangeDirectioninterval = 0.05f;
			tCameraShakeRotDesc.fShakingRotAxis = m_pMainCamera->Get_CamTransformCom()->Get_MatrixState(CTransform::TransformState::STATE_RIGHT);
			m_pMainCamera->Start_CameraShaking_Rot_Thread(&tCameraShakeRotDesc, false);

			m_pPlayerWeapons[WEAPON_SWORD - 1]->EffectParticleOn(0, &m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_LOOK));

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
	m_bOnNavigation = true;

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
		m_pRendererCom->OnOff_PostPorcessing_byParameter(POSTPROCESSINGID::POSTPROCESSING_CAMMOTIONBLUR, false);
	}
	else if (false == m_bThrowSpear && 0.14f >= m_pModel->Get_PlayRate() && 0.1f <= m_pModel->Get_PlayRate())
	{
		m_bThrowSpear = true;
		_Vector vPlayerLook = XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK));
		static_cast<CPlayerWeapon_Spear*>(m_pPlayerWeapons[WEAPON_SPEAR - 1])->Throw_Start(vPlayerLook);

		m_pRendererCom->OnOff_PostPorcessing_byParameter(POSTPROCESSINGID::POSTPROCESSING_CAMMOTIONBLUR, true);
		m_pMainCamera->Start_CameraShaking_Fov(57.f, 2.f, 0.1f, true);
	}
}

void CPlayer::Spear_Ultimate(_double fDeltaTime)
{
	static_cast<CPlayerWeapon_Spear*>(m_pPlayerWeapons[WEAPON_SPEAR - 1])->Change_Pivot(CPlayerWeapon_Spear::ESpearPivot::SPEAR_PIVOT_TAKEDOWN);
	_float fAnimPlayRate = (_float)m_pModel->Get_PlayRate();
	m_fAnimSpeed = 1.f;

	// Sound
	if (false == m_bOncePlaySound && 0.32f <= fAnimPlayRate)
	{
		m_bOncePlaySound = true;
		g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Trishul_Ultimate_Ground_Hit.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 0.8f);
		g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Trishul_Ultimate_FireLayer.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 0.8f);
	}

	// Active CameraShake
	if (true == m_bActive_ActionCameraShake && 0.45f >= fAnimPlayRate  && 0.4f <= fAnimPlayRate)
	{
		m_fAnimSpeed = 0.6f;
		CCamera_Main::CAMERASHAKEROTDESC tCameraShakeRotDesc;
		tCameraShakeRotDesc.fTotalTime = 0.05f;
		tCameraShakeRotDesc.fPower = 2.f;
		tCameraShakeRotDesc.fChangeDirectioninterval = 0.05f;
		tCameraShakeRotDesc.fShakingRotAxis = m_pMainCamera->Get_CamTransformCom()->Get_MatrixState(CTransform::TransformState::STATE_RIGHT);
		m_pMainCamera->Start_CameraShaking_Rot_Thread(&tCameraShakeRotDesc, false);

		m_pMainCamera->Start_CameraShaking_Fov(58.f, 3.f, 1.9f, true);
		m_bActive_ActionCameraShake = false;

		m_pPlayerWeapons[WEAPON_SPEAR - 1]->Active_Collision_4();
		m_pRendererCom->OnOff_PostPorcessing_byParameter(POSTPROCESSINGID::POSTPROCESSING_CAMMOTIONBLUR, true);
	}

	static _bool bEffectChecker = false;

	if(fAnimPlayRate < 0.2f) bEffectChecker = false;
	else if (!bEffectChecker)
	{
		bEffectChecker = true;
		m_vecNonInstMeshDesc[2].vPosition = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS) +
			m_pTransformCom->Get_MatrixState_Normalized(CTransform::STATE_LOOK) * 0.95f
			- m_pTransformCom->Get_MatrixState_Normalized(CTransform::STATE_UP) * 0.251f;

		m_vecNonInstMeshDesc[2].vLookDir =
			XMVector3Normalize((m_pTransformCom->Get_MatrixState(CTransform::STATE_POS) - m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK)
				+ m_pTransformCom->Get_MatrixState(CTransform::STATE_UP) * 10.f) - m_vecNonInstMeshDesc[2].vPosition.XMVector());

		g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_PlayerEffect), TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[2]);
		GetSingle(CUtilityMgr)->SlowMotionStart(1.5f, 0.1f);
	}

	//

	if (0.97f <= fAnimPlayRate)
	{
		static_cast<CPlayerWeapon_Spear*>(m_pPlayerWeapons[WEAPON_SPEAR - 1])->Change_Pivot(CPlayerWeapon_Spear::ESpearPivot::SPEAR_PIVOT_NORMAL);
		Set_State_IdleStart(fDeltaTime);
		m_bActive_ActionCameraShake = true;
		m_pPlayerWeapons[WEAPON_SPEAR - 1]->DeActive_Collision_4();
		m_bOncePlaySound = false;
		m_pRendererCom->OnOff_PostPorcessing_byParameter(POSTPROCESSINGID::POSTPROCESSING_CAMMOTIONBLUR, false);
	}
}

void CPlayer::Shelling(_double fDeltaTime)
{
	switch (m_eCurUtilityState)
	{
	case UTILITY_START:
		m_fAnimSpeed = 1.f;

		if (false == m_bOncePlaySound)
		{
			m_bOncePlaySound = true;
			g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Bow_Pull.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 0.7f);
		}

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
			m_bOncePlaySound = false;
		}
		else if (false == m_bPressedUtilityKey)
		{
			m_eCurUtilityState = UTILITY_END;
			m_bOncePlaySound = false;
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
		m_fCurTime_ShellingDelay += (_float)g_fDeltaTime;//(_float)g_pGameInstance->Get_DeltaTime(TEXT("Player_Timer_ShellingShot_Delay"));

		m_fAnimSpeed = 1.f;

		{
			CPlayerWeapon_Arrow* pBowArrow = static_cast<CPlayerWeapon_Arrow*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_PlayerWeapon)));
			if (pBowArrow)
			{
				FAILED_CHECK_NONERETURN(pBowArrow->Set_Play_Particle
				(3, _fVector(), _float3::Zero()));
			}
		}

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
			m_pMainCamera->Start_CameraShaking_Fov(58.f, 2.f, 0.03f, true);
			
			g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Bow_Shelling_Shoot.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 0.7f);
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
		m_bAnimChangeSwitch = false;
	}
	else if (0.774f <= m_pModel->Get_PlayRate())
	{
		static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->PlayAnim_Idle();
		static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->Set_AnimSpeed(3.f);
	}
	else if (true == m_bAnimChangeSwitch && 0.574f <= m_pModel->Get_PlayRate())
	{
		m_bAnimChangeSwitch = false; 
		static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->PlayAnim_UtilityAttack_Shot();
		static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->Set_AnimSpeed(1.f);

		CPlayerWeapon_Arrow* pBowArrow = static_cast<CPlayerWeapon_Arrow*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_PlayerWeapon)));
		pBowArrow->Set_State_Ultimate_Post_Shot();
		m_pMainCamera->Start_CameraShaking_Fov(56.f, 1.f, 4.f, true);

		{
			CTransform* effecttrans = static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->Get_EffectTransform();

			_Vector FixPos =  pBowArrow->Get_Transform_Hand()->Get_MatrixState_Float3(CTransform::STATE_POS).XMVector();
			FAILED_CHECK_NONERETURN(static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->Set_Play_Particle(7, FixPos));

			FAILED_CHECK_NONERETURN(static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])
				->Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_ARROW_BOW_SP_BOW, effecttrans));

			FAILED_CHECK_NONERETURN(static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])
				->Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_ARROW_BOW_SP_TON, m_pTextureParticleTransform));

			FAILED_CHECK_NONERETURN(static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])
				->Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_ARROW_BOW_SP_ICES, m_pTextureParticleTransform));
			
			FAILED_CHECK_NONERETURN(static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])
				->Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_ARROW_BOW_SP_PLANE3, m_pTextureParticleTransform));

			
			INSTMESHDESC instancedesc;
			instancedesc.FollowingTarget = nullptr;

			instancedesc = GETPARTICLE->Get_TypeDesc_MeshInstance(CPartilceCreateMgr::MESHINST_EFFECTJ_BOW_Q_ICE);
			instancedesc.vFixedPosition = FixPos;
			GETPARTICLE->Create_MeshInst_DESC(instancedesc, m_eNowSceneNum);

			instancedesc = GETPARTICLE->Get_TypeDesc_MeshInstance(CPartilceCreateMgr::MESHINST_EFFECTJ_BOW_Q_ICE2);
			instancedesc.vFixedPosition = FixPos;
			GETPARTICLE->Create_MeshInst_DESC(instancedesc, m_eNowSceneNum);

			instancedesc = GETPARTICLE->Get_TypeDesc_MeshInstance(CPartilceCreateMgr::MESHINST_EFFECTJ_BOW_Q_PLANE);
			instancedesc.FollowingTarget = m_pTextureParticleTransform;
			instancedesc.iFollowingDir = FollowingDir_Up;
			instancedesc.TotalParticleTime = 3;
			instancedesc.ParticleStartRandomPosMin = _float3(-5,0,-5);
			instancedesc.ParticleStartRandomPosMax = _float3(5, 0, 5);;

			GETPARTICLE->Create_MeshInst_DESC(instancedesc, m_eNowSceneNum);
			// Set Ultimate Attack Col
			static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->Set_UltimateAttackPos(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS));
			m_pPlayerWeapons[WEAPON_BOW - 1]->Active_Collision();
			//
		}
	}
	else if (false == m_bAnimChangeSwitch && 0.446f <= m_pModel->Get_PlayRate() && 0.574f >= m_pModel->Get_PlayRate())
	{
		GetSingle(CUtilityMgr)->SlowMotionStart(1.f, 0.2f);
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
		m_pMainCamera->Start_CameraShaking_Fov(57.f, 2.f, 0.1f, true);


		{
			CTransform* effecttrans = static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->Get_EffectTransform();

			FAILED_CHECK_NONERETURN(static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])
				->Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_ARROW_BOW_SP_MOVE_SPEHERE, m_pTextureParticleTransform));

			FAILED_CHECK_NONERETURN(static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])
				->Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_ARROW_BOW_SP_BOW, effecttrans));

			g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Bow_Ultimate.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 1.f);
		}

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

		{
			//FAILED_CHECK_NONERETURN(static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])
			//	->Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_ARROW_BOW_SP_PLANE, m_pTextureParticleTransform));

			FAILED_CHECK_NONERETURN(static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])
				->Set_Play_MeshParticle(CPartilceCreateMgr::E_MESH_EFFECTJ::MESHEFFECT_ARROW_BOW_SP_PLANE2, m_pTextureParticleTransform));

		
		}

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
			g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Shield_StartBlock.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 0.7f);
			m_pTransformCom->Set_MoveSpeed(1.f);
			m_eCurUtilityState = UTILITY_LOOP;
		}
		break;
	case UTILITY_LOOP:
		m_fAnimSpeed = 1.5f;

		m_bShieldMode = true;

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
		m_bShieldMode = false;
		break;
	}
}

void CPlayer::Sword_Ultimate(_double fDeltaTime)
{
	m_fAnimSpeed = 1.f;
	_float fAnimPlayRate = (_float)m_pModel->Get_PlayRate();

	// Sound
	if (false == m_bOncePlaySound && 0.5f <= fAnimPlayRate)
	{
		m_bOncePlaySound = true;
		g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Sword_Ultimate.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 0.7f);
	}


	if (false == m_bActionSwitch && 0.25f <= fAnimPlayRate)
	{
		m_bActionSwitch = true;

		g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Sowrd_Ultimate_Charging.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 0.7f);
		g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Sowrd_Ultimate_Charging2.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 0.7f);
	}


	if (false == m_bAnimChangeSwitch && 0.25f <= fAnimPlayRate)
	{
		m_bAnimChangeSwitch = true;
		_float fTargetPos_Y = XMVectorGetY(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS)) + 4.f;
		static_cast<CPlayerWeapon_Shield*>(m_pPlayerWeapons[WEAPON_SHIELD - 1])->Start_UltimateMode(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS), fTargetPos_Y);
		GetSingle(CUtilityMgr)->SlowMotionStart(1.5f, 0.1f);


		{

			m_vecNonInstMeshDesc[7].vPosition = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);

			m_vecNonInstMeshDesc[7].vLookDir = -(m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK));

			g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_PlayerEffect), TAG_OP(Prototype_NonInstanceMeshEffect),
				&m_vecNonInstMeshDesc[7]);
		}


	}

	static _bool bEffectChecker = false;

	if (fAnimPlayRate < 0.5f) bEffectChecker = false;
	else if (!bEffectChecker)
	{
		bEffectChecker = true;

		m_vecNonInstMeshDesc[10].vPosition = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS)
			+ (m_pTransformCom->Get_MatrixState(CTransform::STATE_UP)) * 4.0f;
		m_vecNonInstMeshDesc[10].vLookDir = -m_pTransformCom->Get_MatrixState(CTransform::STATE_UP);
		g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_PlayerEffect), TAG_OP(Prototype_NonInstanceMeshEffect),
			&m_vecNonInstMeshDesc[10]);
	}


	if (0.533 <= fAnimPlayRate && 0.666 >= fAnimPlayRate)
	{ 
		m_fAnimSpeed = 0.2f;
		if (false == m_bActive_ActionCameraShake)
		{
			m_bActive_ActionCameraShake = true;


			CCamera_Main::CAMERASHAKEROTDESC tCameraShakeRotDesc;
			tCameraShakeRotDesc.fTotalTime = 0.05f;
			tCameraShakeRotDesc.fPower = 2.f;
			tCameraShakeRotDesc.fChangeDirectioninterval = 0.05f;
			tCameraShakeRotDesc.fShakingRotAxis = m_pMainCamera->Get_CamTransformCom()->Get_MatrixState(CTransform::TransformState::STATE_RIGHT);
			m_pMainCamera->Start_CameraShaking_Rot_Thread(&tCameraShakeRotDesc, false);

			m_pMainCamera->Start_CameraShaking_Fov(58.f, 2.5f, 2.f, true);


			m_pRendererCom->OnOff_PostPorcessing_byParameter(POSTPROCESSINGID::POSTPROCESSING_CAMMOTIONBLUR, true);




			m_vecNonInstMeshDesc[8].vPosition = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS)
				+ (m_pTransformCom->Get_MatrixState(CTransform::STATE_UP)) * 4.0f;
			m_vecNonInstMeshDesc[8].vLookDir = (m_pTransformCom->Get_MatrixState(CTransform::STATE_RIGHT));
			g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_PlayerEffect),
				TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[8]);


			m_vecNonInstMeshDesc[9].vPosition = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS)
				+ (XMVectorSet(0, 1, 0, 0)) * 4.0f;


			m_vecNonInstMeshDesc[9].vLookDir = ((m_vecNonInstMeshDesc[9].vPosition.XMVector()) - (m_pTransformCom->Get_MatrixState(CTransform::STATE_POS) +
				m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK) * 1.5f
				));


			g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_PlayerEffect),
				TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecNonInstMeshDesc[9]);

			static_cast<CPlayerWeapon_Sword*>(m_pPlayerWeapons[WEAPON_SWORD - 1])->Active_Collision_3();

			g_pGameInstance->Play3D_Sound(TEXT("Jino_Raji_Sword_Ultimate_Smash_Heavy.wav"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_PLAYER, 1.f);
		}
	}
	else if(fAnimPlayRate > 0.8f)
	{
		//m_pRendererCom->OnOff_PostPorcessing_byParameter(POSTPROCESSINGID::POSTPROCESSING_CAMMOTIONBLUR, false);
	}

	if (0.98f <= fAnimPlayRate)
	{
		m_bAnimChangeSwitch = false;
		Set_State_IdleStart(fDeltaTime);
		static_cast<CPlayerWeapon_Shield*>(m_pPlayerWeapons[WEAPON_SHIELD - 1])->End_UltimateMode();
		static_cast<CPlayerWeapon_Sword*>(m_pPlayerWeapons[WEAPON_SWORD - 1])->DeActive_Collision_3();
		m_bActive_ActionCameraShake = true;
		m_bOncePlaySound = false;
		m_bActionSwitch = false;
		m_pRendererCom->OnOff_PostPorcessing_byParameter(POSTPROCESSINGID::POSTPROCESSING_CAMMOTIONBLUR, false);
		return;
	}



	//// Active CameraShake
	if (true == m_bActive_ActionCameraShake && 0.22f <= fAnimPlayRate && 0.5 > fAnimPlayRate)
	{
		m_pMainCamera->Start_CameraShaking_Fov(58.f, 3.f, 0.1f, true);
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
		m_bOnNavigation = false;
		m_fAnimSpeed = 1.5f; 
		if (0.f < fCurAnimRate)
		{
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
				m_pTransformCom->Move_Up(fDeltaTime * 1.5f);
			}
			else if (0.4f <= fCurAnimRate && 0.66f >= fCurAnimRate)
			{
				m_fAnimSpeed = (fCurAnimRate < 0.52f) ?
					g_pGameInstance->Easing_Return(TYPE_Linear, TYPE_Linear, 1.5f, 2.f, fCurAnimRate, 0.8f)
					: g_pGameInstance->Easing_Return(TYPE_Linear, TYPE_Linear, 2.f, 0.6f, fCurAnimRate - 0.52f, 0.8f);

				m_pTransformCom->Move_Up(fDeltaTime * 1.2f);
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
				m_bOnNavigation = true;
			}
		}
	}
		break;
	case EPARKOUR_LEDGESTATE::LEDGE_DOUBLEJUMP:
	{
		m_bLedge_ReachBackState = false;
		m_bOnNavigation = false;
		m_fAnimSpeed = 1.5f;
		if (0.f < fCurAnimRate)
		{
			if (0.031f < fCurAnimRate && 0.7f >= fCurAnimRate)
			{
				if (0.359f <= fCurAnimRate)
				{
					m_eCurLedgeState = LEDGE_HANGING_FALLINGDOWN;
					m_pModel->Change_AnimIndex(LEDGE_ANIM_FALLING, 0.1f);
					m_fJumpPower = 6.f;
					return;
				}

				_float fPos_y = m_fFallingStart_Y + (6.f * m_fFallingAcc - 9.8f * m_fFallingAcc * m_fFallingAcc * 0.5f);
				m_fFallingAcc += 0.02f;
			
				m_pTransformCom->Move_Backward(fDeltaTime * 1.f);

				_Vector vPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
				vPos = XMVectorSetY(vPos, fPos_y);
				m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, vPos);
			}
		}
	}
		break;
	case EPARKOUR_LEDGESTATE::LEDGE_HANGING_IDLE:
	{
		m_bLedge_ReachBackState = false;
		m_bOnNavigation = false;
		m_fAnimSpeed = 1.f;
		
		if (0.f < fCurAnimRate && true == m_bPressedInteractKey)
		{
			m_eCurLedgeState = LEDGE_HANGING_FALLINGDOWN;
			m_pModel->Change_AnimIndex(LEDGE_ANIM_FALLING);
			m_fFallingStart_Y = XMVectorGetY(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS));
			m_fJumpPower = 0.f;
			m_fFallingAcc = 0.f;
			return;
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
				m_fJumpPower = 12.f;
				m_fFallingAcc = 0.f;
			}
		}
		else
		{
			if (LEDGE_ANIM_HANGING_REACHOUT_BACK_IDLE == m_pModel->Get_NowAnimIndex())
			{
				m_bLedge_ReachBackState = true;
				if (true == m_bPressedDodgeKey)
				{
					m_eCurLedgeState = EPARKOUR_LEDGESTATE::LEDGE_DOUBLEJUMP;
					m_pModel->Change_AnimIndex(LEDGE_ANIM_DOUBLEJUMP);
					m_fFallingStart_Y = XMVectorGetY(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS));
					m_fFallingAcc = 0.f;
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
		m_bOnNavigation = false;
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
		m_bOnNavigation = false;
		m_pTransformCom->Move_Down(fDeltaTime * 2.f);
		_Vector vMyPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
		_float fPos_Y = XMVectorGetY(vMyPos);
		if (fPos_Y <= m_fJumpStart_Y)
		{
			vMyPos = XMVectorSetY(vMyPos, m_fJumpStart_Y);
			m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, vMyPos);

			Set_State_IdleStart(fDeltaTime);
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
		m_bOnNavigation = false;

		_float fPrePos_y = XMVectorGetY(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS));
		_float fPos_y = m_fFallingStart_Y + (m_fJumpPower * m_fFallingAcc - 9.8f * m_fFallingAcc * m_fFallingAcc * 0.5f);
		_float fNav_y = m_pNavigationCom->Get_NaviHeight(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS));

		m_fFallingAcc += 0.03f;
		if (fNav_y >= fPos_y && fPrePos_y >= fNav_y /*(fNav_y -0.1f) <= fPos_y*/ &&
			CCell::CELL_OPTION::CELL_NOMAL == m_pNavigationCom->Get_CurCellOption())
		{
			fPos_y = fNav_y;
			m_fAnimSpeed = 5.f;
			m_fFallingAcc = 0.f;
			m_bOnNavigation = true;
			Set_State_IdleStart(fDeltaTime);
		}
	
		_Vector vPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
		vPos = XMVectorSetY(vPos, fPos_y);
		m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, vPos);
	}
		break;
	case EPARKOUR_LEDGESTATE::LEDGE_HANGING_CLIMBUP:
	{
		m_bOnNavigation = false;
		if (0.f < fCurAnimRate)
		{
			if (0.256f <= fCurAnimRate && 0.307f >= fCurAnimRate)
			{
				m_pTransformCom->Move_Forward(fDeltaTime * 0.9f);
			} 
			 
			
			if (0.128f < fCurAnimRate && 0.358f >= fCurAnimRate)
			{
				if(0.282f >= fCurAnimRate)
					m_pTransformCom->Move_Up(fDeltaTime * 0.5f);
				else
					m_pTransformCom->Move_Up(fDeltaTime * 0.6f);
			}
				
			if (0.512f <= fCurAnimRate && 0.820f >= fCurAnimRate)
			{
				m_pTransformCom->Move_Up(fDeltaTime * 0.2f);
			}
			//m_pTransformCom->Turn_Dir(m_fLookDir.XMVector(), 0.9f);

			/*if (0.1f > fCurAnimRate)
			{
				m_pTransformCom->MovetoDir(m_fLookDir.XMVector(), fDeltaTime * 0.38f);
			}

			if ((0.179f <= fCurAnimRate && 0.384f >= fCurAnimRate))
			{
				m_pTransformCom->Move_Up(fDeltaTime * 0.5f);
			}
			else if (0.76f <= fCurAnimRate)
			{
				m_pTransformCom->Move_Up(fDeltaTime * 0.65f);
			}
			*/
			if (0.9f <= fCurAnimRate)
			{
				m_eCurState = STATE_IDLE;
				Set_State_IdleStart(fDeltaTime);
				m_bOnNavigation = true;
			}
		}
	}
		break;
	case EPARKOUR_LEDGESTATE::LEDGE_HANGING_CLIMBDOWN:
	{
		m_bOnNavigation = false;
		if (0.f < fCurAnimRate)
		{
			m_pTransformCom->Turn_Dir(m_fLookDir.XMVector(), 0.9f);
			if (0.1f > fCurAnimRate)
			{
				m_pTransformCom->MovetoDir(m_fLookDir.XMVector() * -1, fDeltaTime * 0.35f);
			}
			
			if ((0.179f <= fCurAnimRate && 0.384f >= fCurAnimRate))
			{
				m_pTransformCom->Move_Down(fDeltaTime * 0.5f);
			}
			else if (0.76f <= fCurAnimRate)
			{
				m_pTransformCom->Move_Down(fDeltaTime * 0.65f);
			}

			if (0.9f <= fCurAnimRate)
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

void CPlayer::Set_JumpPower(_float fJumpPower)
{
	m_fJumpPower = fJumpPower;
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

	case ETARGETING_STATE::TARGETING_BOSS:
	{
		Targeting_Boss();
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
	if (fDist > 25.f)
	{
		m_pMainCamera->Set_CameraMode(ECameraMode::CAM_MODE_NOMAL);
		m_eCur_TargetingState = ETARGETING_STATE::TARGETING_SEARCH;
		/*m_pMainCamera->Set_CameraLookWeight(0.9f);
		m_pMainCamera->Set_CameraMoveWeight(0.9f);*/
		return;
	} 
	else 
	{
		/*m_pMainCamera->Set_CameraLookWeight(0.98f);
		m_pMainCamera->Set_CameraMoveWeight(0.98f);*/
		fDist *= 2.5f;
		if (fDist > 25.f)
		{
			fDist = 25.f;
		}
		else if (fDist < 9)
		{
			fDist = 9.f;
		}
	}

	_Vector vLookDir = XMVectorSet(0.f, -0.5f, 0.3f, 0.f) * (fDist + 5.f);

	// Send to Center pos to Main Camera
	m_pMainCamera->Set_TargetingPoint(vCenterPos);
	m_pMainCamera->Set_TargetingLook(vLookDir);

}

void CPlayer::Targeting_Boss()
{
	_Vector vPlayerPos = Get_BonePos("skd_hip");
	_Vector vTargetPos = m_pTargetingMonster_Transform->Get_MatrixState(CTransform::TransformState::STATE_POS);
	_Vector vCenterPos = (vPlayerPos + vTargetPos) * 0.5f;

	_float fDist = XMVectorGetX(XMVector3Length(vPlayerPos - vTargetPos));// *0.15f;
	
		m_pMainCamera->Set_CameraLookWeight(0.99f);
		m_pMainCamera->Set_CameraMoveWeight(0.99f);
		fDist *= 2.5f;
		if (fDist > 35.f)
		{
			fDist = 35.f;
		}

	_Vector vLookDir = XMVectorSet(0.f, -0.5f, 0.3f, 0.f) * (fDist + 4.5f);

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

void CPlayer::LookAt_MousePos(_float fWeight)
{
	if (true == GetSingle(CUtilityMgr)->Get_IsSlowed())
	{
		return;
	}
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

		_Vector vTargetDir = XMVector3Normalize(XMLoadFloat3(&fResult) - m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS));
		m_pTransformCom->Turn_Dir(vTargetDir, fWeight);
	}

}

_fVector CPlayer::Get_MousePos()
{

	return _fVector();
}

_bool CPlayer::Is_Hiding()
{
	return m_bPlayerHide;
}

CPlayer::EPARKOUR_LEDGESTATE CPlayer::Get_LedgeState()
{
	return m_eCurLedgeState;
}

void CPlayer::Set_CurParkurLedge(CTestLedgeTrigger* pTargetLedge)
{
	m_pCurParkourTrigger = pTargetLedge;
}

CTriggerObject * CPlayer::Get_CurParkurTriger()
{
	return m_pCurParkourTrigger;
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
	ColliderDesc.vScale = _float3(1.5f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	ATTACHEDESC tAttachedDesc;
	tAttachedDesc.Initialize_AttachedDesc(this, "skd_hip", _float3(1), _float3(0), _float3(-0.074084f, -0.861011f, -75.1948f));
	m_vecAttachedDesc.push_back(tAttachedDesc);



	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.2f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, -0.1f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "skd_head", _float3(1), _float3(0), _float3(0.024161f, -0.499942f, -127.252f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pCollider->Set_ParantBuffer();

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.2f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "skd_chest", _float3(1), _float3(0), _float3(-0.041261f, -1.80535f, -109.114f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pCollider->Set_ParantBuffer();

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.1f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "skd_r_arm", _float3(1), _float3(0), _float3(-11.7516f, -0.395438f, -114.675f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pCollider->Set_ParantBuffer();

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.1f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "skd_l_arm", _float3(1), _float3(0), _float3(11.6691f, -0.395438f, -114.675f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pCollider->Set_ParantBuffer();

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.2f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "skd_spine01", _float3(1), _float3(0), _float3(0.024159f, -1.80535f, -88.5722f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pCollider->Set_ParantBuffer();

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.2f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "skd_r_leg", _float3(1), _float3(0), _float3(-6.28502f, -0.74237f, -75.1888f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pCollider->Set_ParantBuffer();

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.2f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "skd_l_leg", _float3(1), _float3(0), _float3(6.13641f, -0.74237f, -75.1206f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pCollider->Set_ParantBuffer();

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.5f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.0f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "skd_l_knee", _float3(1), _float3(0), _float3(6.1366f, -5.7681f, -37.9986f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pCollider->Set_ParantBuffer();

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.5f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.0f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "skd_r_knee", _float3(1), _float3(0), _float3(-6.28494f, -5.51913f, -38.0339f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pCollider->Set_ParantBuffer();
	
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.1f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.0f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "skd_l_ball", _float3(1), _float3(0), _float3(6.13672f, -9.69943f, -0.96666f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pCollider->Set_ParantBuffer();

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.1f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.0f, 0.f, 1);
	FAILED_CHECK(m_pCollider->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "skd_r_ball", _float3(1), _float3(0), _float3(-6.28489f, -9.6995f, -0.966615f));
	m_vecAttachedDesc.push_back(tAttachedDesc);
	m_pCollider->Set_ParantBuffer();


	// Parkur Collider
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_Collider_1), (CComponent**)&m_pCollider_Parkur));
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(3.5f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pCollider_Parkur->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "skd_hip", _float3(1), _float3(0), _float3(0.f));
	m_vecAttachedDesc_Parkur.push_back(tAttachedDesc);

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.8f, 0.05f, 0.05f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pCollider_Parkur->Add_ColliderBuffer(COLLIDER_OBB, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "skd_root", _float3(1), _float3(0), _float3(0.f));
	m_vecAttachedDesc_Parkur.push_back(tAttachedDesc);
	m_pCollider_Parkur->Set_ParantBuffer();

	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(0.8f, 0.05f, 0.05f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1);
	FAILED_CHECK(m_pCollider_Parkur->Add_ColliderBuffer(COLLIDER_OBB, &ColliderDesc));
	tAttachedDesc = ATTACHEDESC();
	tAttachedDesc.Initialize_AttachedDesc(this, "skd_root", _float3(1), _float3(0), _float3(0.f));
	m_vecAttachedDesc_Parkur.push_back(tAttachedDesc);
	m_pCollider_Parkur->Set_ParantBuffer();
	//

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
	
	m_pMainCameraTransform = m_pMainCamera->Get_CamTransformCom();
	Update_AttachCamPos();

	// Pressed Move Key Amount
	m_fMaxTime_PressedMoveKeyDuration = 1.f;


	// HP
	m_fMaxHP = 9.f;
	m_fHP = m_fMaxHP;

	

	return S_OK;
}

HRESULT CPlayer::SetUp_PlayerWeapons()
{
	// Create Player Weapon Spear //
	CPlayerWeapon::PlayerWeaponDesc eWeaponDesc;
	eWeaponDesc.eAttachedDesc.Initialize_AttachedDesc(this, "skd_r_palm", _float3(1, 1, 1), _float3(-97, -120, -60), _float3(-0.661f, -0.04f, -1.133f));
	eWeaponDesc.eWeaponState = CPlayerWeapon::EWeaponState::STATE_EQUIP;
	eWeaponDesc.pOwner = this;
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(g_pGameInstance->Get_TargetSceneNum(), TAG_LAY(Layer_PlayerWeapon), TAG_OP(Prototype_PlayerWeapon_Spear), &eWeaponDesc));
	m_pPlayerWeapons[WEAPON_SPEAR - 1] = (CPlayerWeapon*)(g_pGameInstance->Get_GameObject_By_LayerIndex(g_pGameInstance->Get_TargetSceneNum(), TAG_LAY(Layer_PlayerWeapon)));
	m_pPlayerWeapons[WEAPON_SPEAR - 1]->Dissolve_Out(0.f);

	//// Create Player Weapon Bow //
	eWeaponDesc.eAttachedDesc.Initialize_AttachedDesc(this, "skd_l_palm", _float3(0, 0, 0), _float3(0, 0, 0), _float3(0.f, 0.f, 0.0f));
	eWeaponDesc.eWeaponState = CPlayerWeapon::EWeaponState::STATE_EQUIP;
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(g_pGameInstance->Get_TargetSceneNum(), TAG_LAY(Layer_PlayerWeapon), TAG_OP(Prototype_PlayerWeapon_Bow), &eWeaponDesc));
	m_pPlayerWeapons[WEAPON_BOW - 1] = (CPlayerWeapon*)(g_pGameInstance->Get_GameObject_By_LayerIndex(g_pGameInstance->Get_TargetSceneNum(), TAG_LAY(Layer_PlayerWeapon), 1));
	m_pPlayerWeapons[WEAPON_BOW - 1]->Dissolve_Out(0.f);

	////// Create Player Weapon Sword //
	eWeaponDesc.eAttachedDesc.Initialize_AttachedDesc(this, "skd_r_palm", _float3(1, 1, 1), _float3(0, 0, 0), _float3(0.f, 0.f, 0.0f));
	eWeaponDesc.eWeaponState = CPlayerWeapon::EWeaponState::STATE_EQUIP;
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(g_pGameInstance->Get_TargetSceneNum(), TAG_LAY(Layer_PlayerWeapon), TAG_OP(Prototype_PlayerWeapon_Sword), &eWeaponDesc));
	m_pPlayerWeapons[WEAPON_SWORD - 1] = (CPlayerWeapon*)(g_pGameInstance->Get_GameObject_By_LayerIndex(g_pGameInstance->Get_TargetSceneNum(), TAG_LAY(Layer_PlayerWeapon), 2));
	m_pPlayerWeapons[WEAPON_SWORD - 1]->Dissolve_Out(0.f);
	
	////// Create Player Weapon Chakra //
	eWeaponDesc.eAttachedDesc.Initialize_AttachedDesc(this, "skd_l_palm", _float3(1.3f, 1.3f, 1.3f), _float3(0, 0, 0), _float3(0.f, 0.f, 0.0f));
	eWeaponDesc.eWeaponState = CPlayerWeapon::EWeaponState::STATE_EQUIP;
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(g_pGameInstance->Get_TargetSceneNum(), TAG_LAY(Layer_PlayerWeapon), TAG_OP(Prototype_PlayerWeapon_Chakra), &eWeaponDesc));
	m_pPlayerWeapons[WEAPON_CHAKRA - 1] = (CPlayerWeapon*)(g_pGameInstance->Get_GameObject_By_LayerIndex(g_pGameInstance->Get_TargetSceneNum(), TAG_LAY(Layer_PlayerWeapon), 3));
	m_pPlayerWeapons[WEAPON_CHAKRA - 1]->Dissolve_Out(0.f);

	////// Create Player Weapon Shield //
	eWeaponDesc.eAttachedDesc.Initialize_AttachedDesc(this, "skd_l_palm", _float3(1, 1, 1), _float3(0, 0, 0), _float3(0.f, 0.f, 0.0f));
	eWeaponDesc.eWeaponState = CPlayerWeapon::EWeaponState::STATE_EQUIP;
	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(g_pGameInstance->Get_TargetSceneNum(), TAG_LAY(Layer_PlayerWeapon), TAG_OP(Prototype_PlayerWeapon_Shield), &eWeaponDesc));
	m_pPlayerWeapons[WEAPON_SHIELD - 1] = (CPlayerWeapon*)(g_pGameInstance->Get_GameObject_By_LayerIndex(g_pGameInstance->Get_TargetSceneNum(), TAG_LAY(Layer_PlayerWeapon), 4));
	m_pPlayerWeapons[WEAPON_SHIELD - 1]->Dissolve_Out(0.f);

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

				m_bOncePlaySwingSound = false;
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
	m_pTextureParticleTransform = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
	m_pMeshParticleTransform = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
	NULL_CHECK_RETURN(m_pTextureParticleTransform, E_FAIL);
	NULL_CHECK_RETURN(m_pMeshParticleTransform, E_FAIL);

	CUtilityMgr* pUtil = GetSingle(CUtilityMgr);

	m_vecTextureParticleDesc.push_back(pUtil->Get_TextureParticleDesc(L"SpearNormalAttack"));
	m_vecTextureParticleDesc[m_vecTextureParticleDesc.size() - 1].FollowingTarget = m_pTextureParticleTransform;
	m_vecTextureParticleDesc[m_vecTextureParticleDesc.size() - 1].iFollowingDir = FollowingDir_Look;

	//	1
	m_vecTextureParticleDesc.push_back(pUtil->Get_TextureParticleDesc(L"FireSmallParticle"));
	m_vecTextureParticleDesc[m_vecTextureParticleDesc.size() - 1].FollowingTarget = nullptr;
	//	2
	m_vecTextureParticleDesc.push_back(pUtil->Get_TextureParticleDesc(L"FireSlamCircle"));
	m_vecTextureParticleDesc[m_vecTextureParticleDesc.size() - 1].FollowingTarget = nullptr;
	m_vecTextureParticleDesc[m_vecTextureParticleDesc.size() - 1].ParticleSize2 = _float3(2.5f);
	m_vecTextureParticleDesc[m_vecTextureParticleDesc.size() - 1].vEmissive_SBB = _float3(1, 0.5f, 0);
	//	3
	m_vecTextureParticleDesc.push_back(pUtil->Get_TextureParticleDesc(L"DistortionWaveEffect"));
	m_vecTextureParticleDesc[m_vecTextureParticleDesc.size() - 1].FollowingTarget = nullptr;

	//	4
	m_vecTextureParticleDesc.push_back(pUtil->Get_TextureParticleDesc(L"Fire_Mandara"));
	m_vecTextureParticleDesc[m_vecTextureParticleDesc.size() - 1].FollowingTarget = nullptr;

	//	5
	m_vecTextureParticleDesc.push_back(pUtil->Get_TextureParticleDesc(L"Fire_SlamEffect2"));
	m_vecTextureParticleDesc[m_vecTextureParticleDesc.size() - 1].FollowingTarget = nullptr;

	//	6
	m_vecTextureParticleDesc.push_back(pUtil->Get_TextureParticleDesc(L"Fire_Mandara_2"));
	m_vecTextureParticleDesc[m_vecTextureParticleDesc.size() - 1].FollowingTarget = nullptr;
	m_vecTextureParticleDesc[m_vecTextureParticleDesc.size() - 1].EachParticleLifeTime = 3.0f;


#pragma region NonInstMesh;


	// 0
	{
		NONINSTNESHEFTDESC tNIMEDesc;

		tNIMEDesc.vPosition = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
		tNIMEDesc.eMeshType = Prototype_Mesh_ConeMesh;
		tNIMEDesc.fMaxTime_Duration = 0.35f;
		tNIMEDesc.vLookDir = m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK);
		tNIMEDesc.fAppearTime = 0.175f;

		tNIMEDesc.noisingdir = _float2(0, 1);

		tNIMEDesc.NoiseTextureIndex = 381;
		tNIMEDesc.MaskTextureIndex = 10;
		tNIMEDesc.iDiffuseTextureIndex = 299;
		tNIMEDesc.m_iPassIndex = 19;
		tNIMEDesc.vEmissive = _float4(1, 0.5f, 1.f, 0);
		tNIMEDesc.vLimLight = _float4(1, 0, 0, 1);
		tNIMEDesc.NoiseTextureIndex = 381;
		tNIMEDesc.vColor = _float3(1.0, 0, 0);

		tNIMEDesc.RotAxis = FollowingDir_Up;
		tNIMEDesc.RotationSpeedPerSec = 1080.f;
		tNIMEDesc.vSize = _float3(1.f, 0.1f, 1.f);

		m_vecNonInstMeshDesc.push_back(tNIMEDesc);

	}
	// 1
	{
		NONINSTNESHEFTDESC tNIMEDesc;

		tNIMEDesc.vPosition = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
		tNIMEDesc.eMeshType = Prototype_Mesh_Spear_NormalEffect;
		tNIMEDesc.fMaxTime_Duration = 0.35f;
		tNIMEDesc.vLookDir = -m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK);
		tNIMEDesc.fAppearTime = 0.175f;

		tNIMEDesc.noisingdir = _float2(0, -1);

		tNIMEDesc.NoiseTextureIndex = 381;
		tNIMEDesc.MaskTextureIndex = 10;
		tNIMEDesc.iDiffuseTextureIndex = 299;
		tNIMEDesc.m_iPassIndex = 19;
		tNIMEDesc.vEmissive = _float4(1, 0.5f, 1.f, 0);
		tNIMEDesc.vLimLight = _float4(1, 0, 0, 1);
		tNIMEDesc.NoiseTextureIndex = 381;
		tNIMEDesc.vColor = _float3(1.0, 0, 0);

		tNIMEDesc.RotAxis = FollowingDir_Up;
		tNIMEDesc.RotationSpeedPerSec = 0;
		tNIMEDesc.vSize = _float3(0.5f, 1, 1.f);

		m_vecNonInstMeshDesc.push_back(tNIMEDesc);
	}

	// 2
	{

		NONINSTNESHEFTDESC tNIMEDesc;



		tNIMEDesc.eMeshType = Prototype_Mesh_Spear_UltiEffect;
		tNIMEDesc.fMaxTime_Duration = 2.5f;

		tNIMEDesc.fAppearTime = 0.75f;

		tNIMEDesc.noisingdir = _float2(0, 1);

		tNIMEDesc.NoiseTextureIndex = 381;
		tNIMEDesc.MaskTextureIndex = 1;
		tNIMEDesc.iDiffuseTextureIndex = 305;
		tNIMEDesc.m_iPassIndex = 19;
		tNIMEDesc.vEmissive = _float4(1, 0.5f, 1.f, 0);
		tNIMEDesc.vLimLight = _float4(1, 0, 0, 1);
		tNIMEDesc.NoiseTextureIndex = 381;
		tNIMEDesc.vColor = _float3(1.0, 0, 0);

		tNIMEDesc.RotAxis = FollowingDir_Look;
		tNIMEDesc.RotationSpeedPerSec = 1080.f;
		tNIMEDesc.vSize = _float3(0.4f);
		m_vecNonInstMeshDesc.push_back(tNIMEDesc);

	}


	// 3
	{
		NONINSTNESHEFTDESC tNIMEDesc;
		tNIMEDesc.eMeshType = Prototype_Mesh_JY_Tornado;
		tNIMEDesc.fMaxTime_Duration = 3.0f;

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
		tNIMEDesc.vSize = _float3(2.f, 0.2f, 2.f);
		m_vecNonInstMeshDesc.push_back(tNIMEDesc);
	}
	// 4
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
		tNIMEDesc.iDiffuseTextureIndex = 276;
		tNIMEDesc.m_iPassIndex = 19;
		tNIMEDesc.vEmissive = _float4(1, 0.5f, 1.f, 0);
		tNIMEDesc.vLimLight = _float4(1, 0, 0, 1);
		tNIMEDesc.NoiseTextureIndex = 381;
		tNIMEDesc.vColor = _float3(1.0, 0, 0);

		tNIMEDesc.RotAxis = FollowingDir_Look;
		tNIMEDesc.RotationSpeedPerSec = 360.f;
		tNIMEDesc.vSize = _float3(0.01f, 0.01f, 0.025f);

		m_vecNonInstMeshDesc.push_back(tNIMEDesc);
	}
	// 5 
	{

		NONINSTNESHEFTDESC tNIMEDesc;

		tNIMEDesc.eMeshType = Prototype_Mesh_Aura2;
		tNIMEDesc.fMaxTime_Duration = 1.f;

		tNIMEDesc.fAppearTime = 0.5f;

		tNIMEDesc.noisingdir = _float2(0, 1);

		tNIMEDesc.NoiseTextureIndex = 249;
		tNIMEDesc.MaskTextureIndex = 64;
		tNIMEDesc.iDiffuseTextureIndex = 276;
		tNIMEDesc.m_iPassIndex = 19;
		tNIMEDesc.vEmissive = _float4(1, 0.5f, 1.f, 0);
		tNIMEDesc.vLimLight = _float4(1, 0, 0, 1);
		tNIMEDesc.NoiseTextureIndex = 381;
		tNIMEDesc.vColor = _float3(1.0, 0, 0);

		tNIMEDesc.RotAxis = FollowingDir_Look;
		tNIMEDesc.RotationSpeedPerSec = 0.f;
		tNIMEDesc.vSize = _float3(0.002f);
		m_vecNonInstMeshDesc.push_back(tNIMEDesc);
	}
	// 6 
	{
		NONINSTNESHEFTDESC tNIMEDesc;

		tNIMEDesc.vPosition = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);

		tNIMEDesc.vLookDir = -(m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK));

		tNIMEDesc.eMeshType = Prototype_Mesh_Beacon_Shape;
		tNIMEDesc.fMaxTime_Duration = 4.5f;

		tNIMEDesc.fAppearTime = 0.75f;

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
		tNIMEDesc.vSize = _float3(0.06f);
		m_vecNonInstMeshDesc.push_back(tNIMEDesc);
	}
	// 7 
	{
		NONINSTNESHEFTDESC tNIMEDesc;

		tNIMEDesc.vPosition = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);

		tNIMEDesc.vLookDir = -(m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK));




		tNIMEDesc.eMeshType = Prototype_Mesh_Spark_Mesh2;
		tNIMEDesc.fMaxTime_Duration = 1.5f;

		tNIMEDesc.fAppearTime = 0.75f;

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
		tNIMEDesc.vSize = _float3(0.0155f);


		m_vecNonInstMeshDesc.push_back(tNIMEDesc);
	}
	// 8
	{
		NONINSTNESHEFTDESC tNIMEDesc;





		tNIMEDesc.eMeshType = Prototype_Mesh_Elec_effect2;
		tNIMEDesc.fMaxTime_Duration = 1.5f;

		tNIMEDesc.fAppearTime = 0.75f;

		tNIMEDesc.noisingdir = _float2(0, 1);

		tNIMEDesc.NoiseTextureIndex = 350;
		tNIMEDesc.MaskTextureIndex = 18;
		tNIMEDesc.iDiffuseTextureIndex = 378;
		tNIMEDesc.m_iPassIndex = 19;
		tNIMEDesc.vEmissive = _float4(1, 0.5f, 1.f, 0);
		tNIMEDesc.vLimLight = _float4(0.98046875f, 0.93359375f, 0.19140625f, 1.f);
		tNIMEDesc.NoiseTextureIndex = 381;
		tNIMEDesc.vColor = _float3(0.98046875f, 0.93359375f, 0.19140625f);

		tNIMEDesc.RotAxis = FollowingDir_Up;
		tNIMEDesc.RotationSpeedPerSec = -1620.f;
		tNIMEDesc.vSize = _float3(-0.02f, -0.013f, -0.02f);
		m_vecNonInstMeshDesc.push_back(tNIMEDesc);



	}
	//9
	{
		NONINSTNESHEFTDESC tNIMEDesc;




		tNIMEDesc.eMeshType = Prototype_Mesh_Big_Sword;
		tNIMEDesc.fMaxTime_Duration = 2.35f;


		tNIMEDesc.fAppearTime = 0.35f;

		tNIMEDesc.noisingdir = _float2(0, 1);

		tNIMEDesc.NoiseTextureIndex = 350;
		tNIMEDesc.MaskTextureIndex = 5;
		tNIMEDesc.iDiffuseTextureIndex = 378;
		tNIMEDesc.m_iPassIndex = 19;
		tNIMEDesc.vEmissive = _float4(1, 0.5f, 1.f, 0);
		tNIMEDesc.vLimLight = _float4(0.98046875f, 0.93359375f, 0.19140625f, 1.f);
		tNIMEDesc.NoiseTextureIndex = 381;
		tNIMEDesc.vColor = _float3(0.98046875f, 0.93359375f, 0.19140625f);

		tNIMEDesc.RotAxis = FollowingDir_Right;
		tNIMEDesc.OnceStartRot = -90.f;

		tNIMEDesc.RotationSpeedPerSec = 0.f;
		tNIMEDesc.vSize = _float3(0.08f, 0.08f, 0.08f);

		tNIMEDesc.MoveDir = FollowingDir_Look;
		tNIMEDesc.MoveSpeed = 0;
		m_vecNonInstMeshDesc.push_back(tNIMEDesc);

	}
	// 10
	{


		NONINSTNESHEFTDESC tNIMEDesc;

		tNIMEDesc.eMeshType = Prototype_Mesh_Half_Sheild;

		tNIMEDesc.fMaxTime_Duration = 2.3f;
		tNIMEDesc.fAppearTime = 0.35f;
		tNIMEDesc.noisingdir = _float2(0, -1);

		tNIMEDesc.NoiseTextureIndex = 350;
		tNIMEDesc.MaskTextureIndex = 5;
		tNIMEDesc.iDiffuseTextureIndex = 378;
		tNIMEDesc.m_iPassIndex = 19;
		tNIMEDesc.vEmissive = _float4(1, 0.5f, 1.f, 1);
		tNIMEDesc.vLimLight = _float4(0.98046875f, 0.93359375f, 0.19140625f, 1.f);
		tNIMEDesc.NoiseTextureIndex = 381;
		tNIMEDesc.vColor = _float3(0.98046875f, 0.93359375f, 0.19140625f);

		tNIMEDesc.RotAxis = FollowingDir_Up;
		tNIMEDesc.RotationSpeedPerSec = 0.f;
		tNIMEDesc.vSize = _float3(0.007f, -0.007f, 0.007f);

		tNIMEDesc.MoveDir = FollowingDir_Look;
		tNIMEDesc.MoveSpeed = 0;


		m_vecNonInstMeshDesc.push_back(tNIMEDesc);



	}
	// 11
	{
		NONINSTNESHEFTDESC tNIMEDesc;




		tNIMEDesc.eMeshType = Prototype_Mesh_Big_Sheild;
		tNIMEDesc.fMaxTime_Duration = 0.15f;

		tNIMEDesc.fAppearTime = 0.075f;

		tNIMEDesc.noisingdir = _float2(0, -3);

		tNIMEDesc.MaskTextureIndex = 16;
		tNIMEDesc.iDiffuseTextureIndex = 378;

		tNIMEDesc.m_iPassIndex = 18;
		tNIMEDesc.vEmissive = _float4(1, 0.5f, 1.f, 1);
		tNIMEDesc.NoiseTextureIndex = 381;
		tNIMEDesc.vLimLight = _float4(0.98046875f, 0.93359375f, 0.19140625f, 1.f);
		tNIMEDesc.vColor = _float3(0.98046875f, 0.93359375f, 0.19140625f);

		tNIMEDesc.RotAxis = FollowingDir_Up;
		tNIMEDesc.RotationSpeedPerSec = 0.f;
		tNIMEDesc.vSize = _float3(0.0015f, 0.001f, -0.003f);

		tNIMEDesc.MoveDir = FollowingDir_Look;
		tNIMEDesc.MoveSpeed = -10.f;

		m_vecNonInstMeshDesc.push_back(tNIMEDesc);
	}
	//12
	{

		NONINSTNESHEFTDESC tNIMEDesc;


		tNIMEDesc.eMeshType = Prototype_Mesh_Spear_NormalEffect;
		tNIMEDesc.fMaxTime_Duration = 0.35f;
		tNIMEDesc.fAppearTime = 0.175f;

		tNIMEDesc.noisingdir = _float2(0, -1);

		tNIMEDesc.NoiseTextureIndex = 381;
		tNIMEDesc.MaskTextureIndex = 10;
		tNIMEDesc.iDiffuseTextureIndex = 378;
		tNIMEDesc.m_iPassIndex = 19;
		tNIMEDesc.vEmissive = _float4(1, 0.5f, 1.f, 0);
		tNIMEDesc.NoiseTextureIndex = 381;
		tNIMEDesc.vLimLight = _float4(0.98046875f, 0.93359375f, 0.19140625f, 1.f);
		tNIMEDesc.vColor = _float3(0.98046875f, 0.93359375f, 0.19140625f);

		tNIMEDesc.RotAxis = FollowingDir_Right;
		tNIMEDesc.RotationSpeedPerSec = -180;
		tNIMEDesc.StartRot = -0.5f;
		tNIMEDesc.vSize = _float3(0.5f, -1, 1.f);
		tNIMEDesc.MoveDir = FollowingDir_Look;
		tNIMEDesc.MoveSpeed = 0.f;
		m_vecNonInstMeshDesc.push_back(tNIMEDesc);


	}
	//13
	{
		NONINSTNESHEFTDESC tNIMEDesc;


		tNIMEDesc.eMeshType = Prototype_Mesh_Spear_NormalEffect;
		tNIMEDesc.fAppearTime = 0.175f;
		tNIMEDesc.fMaxTime_Duration = 0.35f;
		tNIMEDesc.noisingdir = _float2(0, -1);

		tNIMEDesc.NoiseTextureIndex = 381;
		tNIMEDesc.MaskTextureIndex = 10;
		tNIMEDesc.iDiffuseTextureIndex = 378;
		tNIMEDesc.m_iPassIndex = 19;
		tNIMEDesc.vEmissive = _float4(1, 0.5f, 1.f, 0);
		tNIMEDesc.NoiseTextureIndex = 381;
		tNIMEDesc.vLimLight = _float4(0.98046875f, 0.93359375f, 0.19140625f, 1.f);
		tNIMEDesc.vColor = _float3(0.98046875f, 0.93359375f, 0.19140625f);

		tNIMEDesc.RotAxis = FollowingDir_Right;
		tNIMEDesc.RotationSpeedPerSec = 180;
		tNIMEDesc.StartRot = -0.5f;
		tNIMEDesc.vSize = _float3(0.5f, 1, 1.f);
		tNIMEDesc.MoveDir = FollowingDir_Look;
		tNIMEDesc.MoveSpeed = 0.f;
		m_vecNonInstMeshDesc.push_back(tNIMEDesc);
	}
	//14
	{
		NONINSTNESHEFTDESC tNIMEDesc;



		tNIMEDesc.eMeshType = Prototype_Mesh_ConeMesh;
		tNIMEDesc.fMaxTime_Duration = 0.35f;
		tNIMEDesc.fAppearTime = 0.175f;

		tNIMEDesc.noisingdir = _float2(0, -1);

		tNIMEDesc.NoiseTextureIndex = 381;
		tNIMEDesc.MaskTextureIndex = 10;
		tNIMEDesc.iDiffuseTextureIndex = 378;
		tNIMEDesc.m_iPassIndex = 19;
		tNIMEDesc.vEmissive = _float4(1, 0.5f, 1.f, 0);
		tNIMEDesc.NoiseTextureIndex = 381;
		tNIMEDesc.vLimLight = _float4(0.98046875f, 0.93359375f, 0.19140625f, 1.f);
		tNIMEDesc.vColor = _float3(0.98046875f, 0.93359375f, 0.19140625f);

		tNIMEDesc.RotAxis = FollowingDir_Up;
		tNIMEDesc.RotationSpeedPerSec = 1080.f;
		tNIMEDesc.vSize = _float3(1.f, -0.1f, 1.f);

		m_vecNonInstMeshDesc.push_back(tNIMEDesc);

	}
#pragma endregion
	return S_OK;
}

HRESULT CPlayer::Update_Partilce_Position()
{
	_Sfloat3 vec = m_pCollider->Get_ColliderPosition(2);
	_Matrix mat = m_pTransformCom->Get_WorldMatrix();
	mat.r[0] = XMVector3Normalize(mat.r[0]);
	mat.r[1] = XMVector3Normalize(mat.r[1]);
	mat.r[2] = XMVector3Normalize(mat.r[2]);

	mat.r[3] = vec;
	m_pTextureParticleTransform->Set_Matrix(mat);
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
	Safe_Release(m_pCollider_Parkur);
	Safe_Release(m_pHPUI);

	Safe_Release(m_pTextureParticleTransform);
	Safe_Release(m_pMeshParticleTransform);


	//JJB
	Safe_Release(m_pSkillUI);
}
