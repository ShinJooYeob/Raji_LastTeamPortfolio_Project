#include "stdafx.h"
#include "..\Public\Player.h"
#include "Camera_Main.h"
#include "PlayerWeapon_Spear.h"
#include "PlayerWeapon_Bow.h"
#include "PlayerWeapon_Shield.h"
#include "PlayerWeapon_Chakra.h"
#include "Timer.h"

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

	if (m_eNowSceneNum = 7)
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, _float3(1.f, 0.f, 0.f));

	FAILED_CHECK(SetUp_EtcInfo());

	FAILED_CHECK(SetUp_PlayerWeapons());

	Set_IsOcllusion(true);

	return S_OK;
}

_int CPlayer::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0) return -1;
	

	// Check Player Key Input
	Check_PlayerKeyInput(fDeltaTime);

	static bool TestBool = false;
	if (g_pGameInstance->Get_DIKeyState(DIK_V) & DIS_Down)
		TestBool = !TestBool;
	if (TestBool)
	{
		static double Timer = 0;
		Timer -= fDeltaTime;
		if (Timer < 0)
		{
			Timer = 0.05f;
			m_pMotionTrail->Add_MotionBuffer(m_pTransformCom->Get_WorldFloat4x4());
		}
	}



	// Reset AnimSpeed
	m_fAnimSpeed = 1.f;


	// Process Player State Logic
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
	case EPLAYER_STATE::STATE_UTILITYSKILL:
		FAILED_CHECK(Update_State_UtilitySkill(fDeltaTime));
		break;
	case EPLAYER_STATE::STATE_ULTIMATESKILL:
		FAILED_CHECK(Update_State_UltimateSkill(fDeltaTime));
		break;
	case EPLAYER_STATE::STATE_TAKE_DAMAGE:
		FAILED_CHECK(Update_State_Damage(fDeltaTime));
		break;
	case EPLAYER_STATE::STATE_EXECUTION:
		FAILED_CHECK(Update_State_Execution(fDeltaTime));
		break;
	default:
		MSGBOX("CPlayer::Update : Unknown Player Cur_State Value");
		break;
	}


	// Update Player Anim
	FAILED_CHECK(m_pModel->Update_AnimationClip(fDeltaTime * m_fAnimSpeed, m_bIsOnScreen));

	Adjust_AnimMovedTransform(fDeltaTime);

	// Debug Camera //
	if (m_eNowSceneNum == 7)
	{
		m_fAttachCamPos_Offset = _float3(0.f, 4.f, -10.f);
	}
	else
	{
		m_fAttachCamPos_Offset = _float3(0.f, 3.f, -2.f);
	}
	Update_AttachCamPos();
	//


	m_pMotionTrail->Update_MotionTrail(fDeltaTime);

	return _int();
}

_int CPlayer::LateUpdate(_double fDeltaTimer)
{
	if (__super::LateUpdate(fDeltaTimer) < 0)return -1;

	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
	FAILED_CHECK(m_pRendererCom->Add_ShadowGroup(CRenderer::SHADOW_ANIMMODEL, this, m_pTransformCom, m_pShaderCom, m_pModel));
	FAILED_CHECK(m_pRendererCom->Add_TrailGroup(CRenderer::TRAIL_MOTION, m_pMotionTrail));


	m_vOldPos = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS);
	g_pGameInstance->Set_TargetPostion(PLV_PLAYER, m_vOldPos);

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

	_uint NumMaterial = m_pModel->Get_NumMaterial();

	for (_uint i = 0; i < NumMaterial; i++)
	{
		for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; j++)
			FAILED_CHECK(m_pModel->Bind_OnShader(m_pShaderCom, i, j, MODLETEXTYPE(j)));

		FAILED_CHECK(m_pModel->Render(m_pShaderCom, 3, i, "g_BoneMatrices"));
	}

	return _int();
}

_int CPlayer::LateRender()
{

	return _int();
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

void CPlayer::Set_State_IdleStart(_double fDeltaTime)
{
	Set_PlayerState(STATE_IDLE);

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

	if(EWEAPON_TYPE::WEAPON_BOW != m_eCurWeapon)
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

	m_fAttachCamLook = XMVector3Normalize((XMVectorSetW(vBonePos.XMVector() - m_fAttachCamPos.XMVector(), 0.f)));
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
		Set_State_DodgeStart(fDeltaTime);
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

		if (false == m_bPlayTurnBackAnim && XMVectorGetX(vDot) <= -0.7)
		{
			Set_State_TurnBackStart(fDeltaTime);
			return _int();
		}


		/** Change State By KeyInput */
		if (true == m_bPressedDodgeKey)
		{
			Set_State_DodgeStart(fDeltaTime);
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
		if (true == m_bAttackEnd && true == m_bPressedPowerAttackKey)
		{
			m_bPlayPowerAttack = true;
		}

		if (true == m_bAttackEnd)
		{
			Set_MainAttackAnim(m_bPlayJumpAttack);
			m_bAttackEnd = false;
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

HRESULT CPlayer::Update_State_Damage(_double fDeltaTime)
{
	return _int();
}

HRESULT CPlayer::Update_State_Execution(_double fDeltaTime)
{
	return _int();
}

HRESULT CPlayer::Update_State_Dead(_double fDeltaTime)
{
	return _int();
}

void CPlayer::Check_PlayerKeyInput(_double fDeltaTime)
{
	Check_Mov_KeyInput(fDeltaTime);
	Check_Action_KeyInput(fDeltaTime);
}

_bool CPlayer::Check_Mov_KeyInput(_double fDeltaTime)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_int iMov_F = 0, iMov_R = 0;

	if (pGameInstance->Get_DIKeyState(DIK_W) & DIS_Press)
	{
		iMov_F += 1;
	}
	if (pGameInstance->Get_DIKeyState(DIK_S) & DIS_Press)
	{
		iMov_F -= 1;
	}
	if (pGameInstance->Get_DIKeyState(DIK_A) & DIS_Press)
	{
		iMov_R -= 1;
	}
	if (pGameInstance->Get_DIKeyState(DIK_D) & DIS_Press)
	{
		iMov_R += 1;
	}

	Set_InputDir(iMov_F, iMov_R, fDeltaTime);

	RELEASE_INSTANCE(CGameInstance);
	return false;
}

_bool CPlayer::Check_Action_KeyInput(_double fDeltaTime)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	
	m_bPressedUltimateKey = false ;

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
			return false;
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
	
	if(false == m_bPlayTurnBackAnim)
	{
		m_pTransformCom->MovetoDir(vMovDir, fMoveRate);
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
	
	/*_float fDecAcc = g_pGameInstance->Easing(TYPE_ExpoOut, 0.f, (_float)fDeltaTime, fRate, 1.f);
	m_pTransformCom->Move_Forward((fDeltaTime - fDecAcc) * 0.5f);*/

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
		// Set AnimSpeed & Move
		if (true == m_pModel->Get_IsHavetoBlockAnimChange() && 0.148148f > fAnimPlayRate)
		{
			m_fAnimSpeed = 4.f;
		}
		else if (true == m_pModel->Get_IsHavetoBlockAnimChange() && 0.148148f <= fAnimPlayRate && 0.5925925f >= fAnimPlayRate)
		{	
			_float MoveSpeed = g_pGameInstance->Easing_Return(TYPE_QuadOut, TYPE_QuarticIn,  0.f, 2.0f, fAnimPlayRate - 0.148148f, 0.4444445f);
			m_fAnimSpeed = 1.6f;

			m_pTransformCom->Move_Forward(fDeltaTime * MoveSpeed);
		}
		else if (true == m_pModel->Get_IsHavetoBlockAnimChange() && fAnimPlayRate && 0.5925925f < fAnimPlayRate)
		{
			m_fAnimSpeed = 3.5f;
			m_pModel->Set_BlockAnim(false);
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
			}
		}
		else if (0.52f <= fAnimPlayRate)
		{
			if (true == m_bReadyDodgeCombo)
			{
				m_pModel->Set_BlockAnim(false);
				m_bPlayDodgeCombo = true;
			}
		}

		break;
	case BASE_ANIM_DODGE_CARTWHEEL:
		if (true == m_pModel->Get_IsHavetoBlockAnimChange() && 0.12f > fAnimPlayRate)
		{
			m_fAnimSpeed = 4.f;
		}
		else if (true == m_pModel->Get_IsHavetoBlockAnimChange() && 0.12f <= fAnimPlayRate && 0.7f >= fAnimPlayRate)
		{
			_float MoveSpeed = g_pGameInstance->Easing_Return(TYPE_QuadOut, TYPE_QuarticIn, 0.f, 2.0f, fAnimPlayRate - 0.12f, 0.58f);
			m_fAnimSpeed = 1.6f;
			m_pTransformCom->Move_Forward(fDeltaTime * MoveSpeed);
		}
		else if (true == m_pModel->Get_IsHavetoBlockAnimChange() && fAnimPlayRate && 0.7f < fAnimPlayRate)
		{
			m_fAnimSpeed = 1.f;
			m_pModel->Set_BlockAnim(false);
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
			}
		}
		else if (0.61f <= fAnimPlayRate)
		{
			if (true == m_bReadyDodgeCombo)
			{
				m_pModel->Set_BlockAnim(false);
				m_bPlayDodgeCombo = true;
			}
		}

		break;
	case BASE_ANIM_DODGE_FLIP:
		if (true == m_pModel->Get_IsHavetoBlockAnimChange() && 0.24f > fAnimPlayRate)
		{
			m_fAnimSpeed = 4.f;
		}
		else if (true == m_pModel->Get_IsHavetoBlockAnimChange() && 0.24f <= fAnimPlayRate && 0.64f >= fAnimPlayRate)
		{
			_float MoveSpeed = g_pGameInstance->Easing_Return(TYPE_QuadOut, TYPE_QuarticIn, 0.f, 2.7f, fAnimPlayRate - 0.24f, 0.4f);
			m_fAnimSpeed = 1.4f;
			m_pTransformCom->Move_Forward(fDeltaTime * MoveSpeed);
		}
		else if (true == m_pModel->Get_IsHavetoBlockAnimChange() && fAnimPlayRate && 0.64f < fAnimPlayRate)
		{
			m_fAnimSpeed = 3.5f;
			m_pModel->Set_BlockAnim(false);
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
			}
		}
		else if (0.62f <= fAnimPlayRate)
		{
			if (true == m_bReadyDodgeCombo)
			{
				m_pModel->Set_BlockAnim(false);
				m_bPlayDodgeCombo = true;
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
			m_pTransformCom->Move_Forward(fDeltaTime * MoveSpeed);
		}


		////////////////////Next Combo Check //////////////////////
		// 1) Check to Input Next Combo Command
		Check_NextComboCommand();

		// 2) Check to Next Combo Behavior
		if (true == m_bPlayNextCombo)
		{
			if (0.92f <= fAnimPlayRate)
			{
				Change_NextCombo();
			}
			else if (0.85f <= fAnimPlayRate)
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
	case SPEAR_ANIM_MAIN_ATK_COMBO_1:
	{
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
			m_pTransformCom->Move_Forward(fDeltaTime * MoveSpeed);
		}


		////////////////////Next Combo Check //////////////////////
		// 1) Check to Input Next Combo Command
		Check_NextComboCommand();

		// 2) Check to Next Combo Behavior
		if (true == m_bPlayNextCombo)
		{
			if (0.92f <= fAnimPlayRate)
			{
				Change_NextCombo();
			}
			else if (0.85f <= fAnimPlayRate)
			{
				if (true == m_bReadyDodgeCombo)
				{
					m_pModel->Set_BlockAnim(false);
					m_bPlayDodgeCombo = true;
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
			m_pTransformCom->Move_Forward(fDeltaTime * MoveSpeed);
		}
		else if (true == m_pModel->Get_IsHavetoBlockAnimChange() && fAnimPlayRate && 0.9f < fAnimPlayRate)
		{
			m_fAnimSpeed = 1.5f;
			m_pModel->Set_BlockAnim(false);
			m_bAttackEnd = true;
		}


		////////////////////Next Combo Check //////////////////////
		// 1) Check to Input Next Combo Command
		Check_NextComboCommand();

		// 2) Check to Next Combo Behavior
		if (true == m_bPlayNextCombo)
		{
			if (0.66f <= fAnimPlayRate)
			{
				Change_NextCombo();
			}
			else if (0.6f <= fAnimPlayRate)
			{
				if (true == m_bReadyDodgeCombo)
				{
					m_pModel->Set_BlockAnim(false);
					m_bPlayDodgeCombo = true;
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
	}
		break;
	case SPEAR_ANIM_MAIN_ATK_COMBO_2_JUMPATTACK:
	{
		m_fAnimSpeed = 1.8f;

		if (0.25f >= fAnimPlayRate)
		{
			_float MoveSpeed = g_pGameInstance->Easing(TYPE_SinInOut, 1.5f, 0.f, fAnimPlayRate, 0.25f);
			m_pTransformCom->Move_Forward(fDeltaTime * MoveSpeed);
		}
		else if (true == m_pModel->Get_IsHavetoBlockAnimChange() && fAnimPlayRate && 0.9f < fAnimPlayRate)
		{
			m_fAnimSpeed = 1.8f;
			m_pModel->Set_BlockAnim(false);
			m_bAttackEnd = true;
		}


		////////////////////Next Combo Check //////////////////////
		// 1) ´ÙÀ½ ÄÞº¸ Ä¿¸àÆ® ÀÔ·Â Ã¼Å©
		Check_NextComboCommand();

		// 2) ³¡³­ ´ÙÀ½ ÀüÈ¯ µÉ ÄÞº¸ Çàµ¿ Ã¼Å©
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
	case SPEAR_ANIM_MAIN_ATK_COMBO_1_JUMPATTACK:
	{
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
		}


		////////////////////Next Combo Check //////////////////////
		// 1) ´ÙÀ½ ÄÞº¸ Ä¿¸àÆ® ÀÔ·Â Ã¼Å©
		Check_NextComboCommand();

		// 2) ³¡³­ ´ÙÀ½ ÀüÈ¯ µÉ ÄÞº¸ Çàµ¿ Ã¼Å©
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
	case SPEAR_ANIM_MAIN_ATK_COMBO_0_JUMPATTACK:
	{
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
		}


		////////////////////Next Combo Check //////////////////////
		// 1) ´ÙÀ½ ÄÞº¸ Ä¿¸àÆ® ÀÔ·Â Ã¼Å©
		Check_NextComboCommand();

		if (true == m_bPlayNextCombo)
		{
			// 2) ³¡³­ ´ÙÀ½ ÀüÈ¯ µÉ ÄÞº¸ Çàµ¿ Ã¼Å©
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
	case SPEAR_ANIM_MAIN_ATK_COMBO_0_RECORVERY:
	case SPEAR_ANIM_MAIN_ATK_COMBO_1_RECORVERY:
	{
		if (false == m_pModel->Get_IsAnimChanging())
		{
			m_bAttackEnd = true;
		}
	}
		break;


		//// Power Attack ////
	case SPEAR_ANIM_POWER_ATK_COMBO_0:
	{
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
			m_pTransformCom->Move_Forward(fDeltaTime * MoveSpeed);
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
			m_pTransformCom->Move_Forward(fDeltaTime * MoveSpeed);
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
	}
		break;
	case SPEAR_ANIM_POWER_ATK_COMBO_0_JUMPATTACK:
	{
		m_fAnimSpeed = 1.f;

		if (0.41f >= fAnimPlayRate)
		{
			_float MoveSpeed = g_pGameInstance->Easing(TYPE_SinInOut, 1.5f, 0.f, fAnimPlayRate, 0.25f);
			m_pTransformCom->Move_Forward(fDeltaTime * MoveSpeed);
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
		static_cast<CPlayerWeapon_Spear*>(m_pPlayerWeapons[WEAPON_SPEAR - 1])->Change_Pivot(CPlayerWeapon_Spear::ESpearPivot::SPEAR_PIVOT_TAKEDOWN);
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
			m_fAnimSpeed = 1.f;
			if (0.98f <= m_pModel->Get_PlayRate())
			{
				m_eCurBowMainAtkState = BOWMAINATK_LOOP;
				m_pModel->Change_AnimIndex(BOW_ANIM_MAIN_ATK_LOOP, 0.1f, false);
			}
			LookAt_MousePos();

			static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->PlayAnim_NormalAttack_Ready();
			break;
		case BOWMAINATK_LOOP:
			m_fAnimSpeed = 1.5f;
			if (false == m_bPressedMainAttackKey)
			{
				m_eCurBowMainAtkState = BOWMAINATK_SHOT;
				m_pModel->Change_AnimIndex(BOW_ANIM_MAIN_ATK_SHOT, 0.1f, false);
			}
			else
			{
				Move_NotTurn(m_eInputDir, fDeltaTime);

				if (MOVDIR_END == m_eInputDir)
					m_pModel->Change_AnimIndex(BOW_ANIM_MAIN_ATK_LOOP, 0.1f, true);
				else if (MOVDIR_F == m_eInputDir || MOVDIR_FL == m_eInputDir || MOVDIR_FR == m_eInputDir)
					m_pModel->Change_AnimIndex(BOW_ANIM_MAIN_ATK_LOOP_RUN_F, 0.1f, true);
				else if (MOVDIR_B == m_eInputDir || MOVDIR_BL == m_eInputDir || MOVDIR_BR == m_eInputDir)
					m_pModel->Change_AnimIndex(BOW_ANIM_MAIN_ATK_LOOP_RUN_B, 0.1f, true);
				else if (MOVDIR_L == m_eInputDir)
					m_pModel->Change_AnimIndex(BOW_ANIM_MAIN_ATK_LOOP_RUN_L, 0.1f, true);
				else if (MOVDIR_R == m_eInputDir)
					m_pModel->Change_AnimIndex(BOW_ANIM_MAIN_ATK_LOOP_RUN_R, 0.1f, true);
			}
			LookAt_MousePos();
			break;
		case BOWMAINATK_SHOT:
			m_fAnimSpeed = 1.f;
			static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->PlayAnim_NormalAttack_Shot();
			if (0.9f <= m_pModel->Get_PlayRate())
			{
				m_eCurBowMainAtkState = BOWMAINATK_START;
				Set_State_IdleStart(fDeltaTime);
				m_bAttackEnd = true;
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
			m_fAnimSpeed = 1.5f;


			// Bow Anim Control
			if (false == m_bAnimChangeSwitch && 0.1 > fAnimPlayRate)
			{
				static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->PlayAnim_UtilityAttack_Loop();
				static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->Set_AnimSpeed(5.f);
				m_bAnimChangeSwitch = true;
			}
			else if(true == m_bAnimChangeSwitch && 0.2 < fAnimPlayRate)
			{
				m_bAnimChangeSwitch = false;
				static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->PlayAnim_UtilityAttack_Shot();
			}
			else if(0.3 < fAnimPlayRate)
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
				}
			}

			////////////////////Next Combo Check //////////////////////
			Check_NextComboCommand();

			if (true == m_bPlayNextCombo)
			{
				if (0.86f <= fAnimPlayRate)
				{
					m_bPlayJumpAttack = false;
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
						m_bPlayJumpAttack = false;
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
		case BOW_ANIM_POWER_COMBO_1:
		case BOW_ANIM_POWER_COMBO_1_JUMP:
		{
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
			}
			else if(0.806f < fAnimPlayRate)
			{
				static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->PlayAnim_Idle();
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
					Change_NextCombo();
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
		case BOW_ANIM_POWER_COMBO_2_JUMP:
		{
			m_fAnimSpeed = 1.3f;


			// Bow Anim Control
			if (true == m_bAnimChangeSwitch && 0.525 < fAnimPlayRate)
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
					m_fAnimSpeed = 1.5f;
					m_pModel->Set_BlockAnim(false);
					m_bAttackEnd = true;
					m_bAnimChangeSwitch = false;
				}
			}

			////////////////////Next Combo Check //////////////////////
			Check_NextComboCommand();

			if (true == m_bPlayNextCombo)
			{
				if (0.86f <= fAnimPlayRate)
				{
					m_bPlayJumpAttack = false;
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
						m_bPlayJumpAttack = false;
						m_bAnimChangeSwitch = false;
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
		m_fAnimSpeed = 2.f;
		
		if (0.9f < fAnimPlayRate)
		{
			m_fAnimSpeed = 1.5f;
			m_pModel->Set_BlockAnim(false);
			m_bAttackEnd = true;
		}


		////////////////////Next Combo Check //////////////////////
		// 1) Check to Input Next Combo Command
		Check_NextComboCommand();

		// 2) Check to Next Combo Behavior
		if (true == m_bPlayNextCombo)
		{
			if (0.392f <= fAnimPlayRate)
			{
				Change_NextCombo();
			}
			else if (0.5f <= fAnimPlayRate)
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
		if (0.464f >= fAnimPlayRate)
		{
			LookAt_MousePos();
		}
		//
	}
	break;
	case SWORD_ANIM_MAIN_ATK_COMBO_1:
	{
		m_fAnimSpeed = 1.f;
		
		if (0.16f >= fAnimPlayRate)
		{
			m_fAnimSpeed = 2.f;
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
			_float MoveSpeed = g_pGameInstance->Easing_Return(TYPE_QuadOut, TYPE_QuarticIn, 0.f, 1.5f, fAnimPlayRate, 0.47f);
			m_pTransformCom->Move_Forward(fDeltaTime * MoveSpeed);
		}
		else if (0.9f < fAnimPlayRate)
		{
			m_fAnimSpeed = 1.5f;
			m_pModel->Set_BlockAnim(false);
			m_bAttackEnd = true;
		}


		////////////////////Next Combo Check //////////////////////
		// 1) Check to Input Next Combo Command
		Check_NextComboCommand();

		// 2) Check to Next Combo Behavior
		if (true == m_bPlayNextCombo)
		{
			if (0.583f <= fAnimPlayRate)
			{
				Change_NextCombo();
			}
			else if (0.55f <= fAnimPlayRate)
			{
				if (true == m_bReadyDodgeCombo)
				{
					m_pModel->Set_BlockAnim(false);
					m_bPlayDodgeCombo = true;
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
		m_fAnimSpeed = 1.f;

		if (0.16f >= fAnimPlayRate)
		{
			m_fAnimSpeed = 2.f;
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
		}


		////////////////////Next Combo Check //////////////////////
		// 1) Check to Input Next Combo Command
		Check_NextComboCommand();

		// 2) Check to Next Combo Behavior
		if (true == m_bPlayNextCombo)
		{
			if (0.66f <= fAnimPlayRate)
			{
				Change_NextCombo();
			}
			else if (0.6f <= fAnimPlayRate)
			{
				if (true == m_bReadyDodgeCombo)
				{
					m_pModel->Set_BlockAnim(false);
					m_bPlayDodgeCombo = true;
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
	case SWORD_ANIM_MAIN_ATK_COMBO_2_JUMPATTACK:
	{
		m_fAnimSpeed = 1.8f;

		if (0.25f >= fAnimPlayRate)
		{
			_float MoveSpeed = g_pGameInstance->Easing(TYPE_SinInOut, 1.5f, 0.f, fAnimPlayRate, 0.25f);
			m_pTransformCom->Move_Forward(fDeltaTime * MoveSpeed);
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
			if (0.85f <= fAnimPlayRate)
			{
				m_bPlayJumpAttack = false;
				Change_NextCombo();
			}
			else if (0.8f <= fAnimPlayRate)
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
		if (0.5f >= fAnimPlayRate)
		{
			LookAt_MousePos();
		}
		//
	}
	break;
	case SWORD_ANIM_MAIN_ATK_COMBO_1_JUMPATTACK:
	{
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
	case SWORD_ANIM_MAIN_ATK_COMBO_0_JUMPATTACK:
	{
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
		}


		////////////////////Next Combo Check //////////////////////
		// 1) ´ÙÀ½ ÄÞº¸ Ä¿¸àÆ® ÀÔ·Â Ã¼Å©
		Check_NextComboCommand();

		if (true == m_bPlayNextCombo)
		{
			// 2) ³¡³­ ´ÙÀ½ ÀüÈ¯ µÉ ÄÞº¸ Çàµ¿ Ã¼Å©
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
		_Vector vPutOnPos = (XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK)) * 1.2f) + m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
		static_cast<CPlayerWeapon_Shield*>(m_pPlayerWeapons[WEAPON_SHIELD - 1])->Start_SmashMode(vPutOnPos);
		
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
				}
			}
		}
		/////////////////////////////////////////////////////////
	}
	break;
	case SWORD_ANIM_POWER_ATK_COMBO_0_JUMPATTACK:
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

		m_fAnimSpeed = 2.5f;

		if (false == m_bAnimChangeSwitch)
		{
			m_bAnimChangeSwitch = true;
			_Vector vThrowStartPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
			vThrowStartPos = XMVectorSetY(vThrowStartPos, XMVectorGetY(vThrowStartPos) + 1.f);
			_Vector vPlayerLook = XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK));
			vThrowStartPos += vPlayerLook * 1.5f;
			static_cast<CPlayerWeapon_Shield*>(m_pPlayerWeapons[WEAPON_SHIELD - 1])->Start_ThrowMode(vThrowStartPos, 5.f);
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
					static_cast<CPlayerWeapon_Shield*>(m_pPlayerWeapons[WEAPON_SHIELD - 1])->End_SmashMode();
					m_bAnimChangeSwitch = false;
				}
			}
			else if (0.6f <= fAnimPlayRate && 0.66f > fAnimPlayRate)
			{
				if (true == m_bReadyDodgeCombo)
				{
					m_pModel->Set_BlockAnim(false);
					m_bPlayDodgeCombo = true;
					m_bPlayJumpAttack = false;
					static_cast<CPlayerWeapon_Shield*>(m_pPlayerWeapons[WEAPON_SHIELD - 1])->End_SmashMode();
					m_bAnimChangeSwitch = false;
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
		m_fAnimSpeed = 1.5f;

		if (false == m_bAnimChangeSwitch)
		{
			_Vector vPutOnPos = (XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK)) * 2.5f) + m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
			static_cast<CPlayerWeapon_Shield*>(m_pPlayerWeapons[WEAPON_SHIELD - 1])->Start_SmashMode(vPutOnPos);
			m_bAnimChangeSwitch = true;
		}

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
			m_bAnimChangeSwitch = false;
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
		break;
	case UTILITY_LOOP:
		m_fAnimSpeed = 1.5f;

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
				m_pModel->Change_AnimIndex(SPEAR_ANIM_THROW_LOOP, 0.1f, true);
			else if (MOVDIR_F == m_eInputDir || MOVDIR_FL == m_eInputDir || MOVDIR_FR == m_eInputDir)
				m_pModel->Change_AnimIndex(SPEAR_ANIM_THROW_LOOP_MOV_F, 0.1f, true);
			else if (MOVDIR_B == m_eInputDir || MOVDIR_BL == m_eInputDir || MOVDIR_BR == m_eInputDir)
				m_pModel->Change_AnimIndex(SPEAR_ANIM_THROW_LOOP_MOV_B, 0.1f, true);
			else if (MOVDIR_L == m_eInputDir)
				m_pModel->Change_AnimIndex(SPEAR_ANIM_THROW_LOOP_MOV_L, 0.1f, true);
			else if (MOVDIR_R == m_eInputDir) 
				m_pModel->Change_AnimIndex(SPEAR_ANIM_THROW_LOOP_MOV_R, 0.1f, true);
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
		}
		static_cast<CPlayerWeapon_Spear*>(m_pPlayerWeapons[WEAPON_SPEAR - 1])->Throw_End();
		m_bThrowSpear = false;
	}
	else if (false == m_bThrowSpear && 0.14f >= m_pModel->Get_PlayRate() && 0.1f <= m_pModel->Get_PlayRate())
	{
		m_bThrowSpear = true;
		_Vector vPlayerLook = XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_LOOK));
		static_cast<CPlayerWeapon_Spear*>(m_pPlayerWeapons[WEAPON_SPEAR - 1])->Throw_Start(vPlayerLook);

	}
}

void CPlayer::Spear_Ultimate(_double fDeltaTime)
{
	static_cast<CPlayerWeapon_Spear*>(m_pPlayerWeapons[WEAPON_SPEAR - 1])->Change_Pivot(CPlayerWeapon_Spear::ESpearPivot::SPEAR_PIVOT_TAKEDOWN);
	m_fAnimSpeed = 1.f;

	if (0.98f <= m_pModel->Get_PlayRate())
	{
		static_cast<CPlayerWeapon_Spear*>(m_pPlayerWeapons[WEAPON_SPEAR - 1])->Change_Pivot(CPlayerWeapon_Spear::ESpearPivot::SPEAR_PIVOT_NORMAL);
		Set_State_IdleStart(fDeltaTime);
	}
}

void CPlayer::Shelling(_double fDeltaTime)
{
	switch (m_eCurUtilityState)
	{
	case UTILITY_START:
		m_fAnimSpeed = 1.f;
		if (false == m_pModel->Get_IsHavetoBlockAnimChange())
		{
			m_eCurUtilityState = UTILITY_LOOP;
		}
		else if (false == m_bPressedUtilityKey)
		{
			m_eCurUtilityState = UTILITY_END;
			static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->PlayAnim_Idle();
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
		}
		else if (m_fCurTime_ShellingDelay >= m_fMaxTime_ShellingDelay && true == m_bPressedMainAttackKey)
		{
			m_fCurTime_ShellingDelay = 0.f;
			m_pModel->Change_AnimIndex(BOW_ANIM_UTILITY_SHOT, 0.1f, true);
			m_eCurUtilityState = UTILITY_ACTIVE;
			static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->PlayAnim_UtilityAttack_Shot();
		}
		LookAt_MousePos();
		break;
	case UTILITY_ACTIVE:
		m_fAnimSpeed = 1.5f;
		Shot_Shelling(fDeltaTime);
		break;
	case UTILITY_END: 
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
	}
	else if (true == m_bAnimChangeSwitch && 0.574f <= m_pModel->Get_PlayRate())
	{
		m_bAnimChangeSwitch = false;
		static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->PlayAnim_UtilityAttack_Shot();
		static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->Set_AnimSpeed(1.f);
	}
	else if (false == m_bAnimChangeSwitch && 0.446f <= m_pModel->Get_PlayRate() && 0.574f >= m_pModel->Get_PlayRate())
	{
		m_bAnimChangeSwitch = true;
		static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->PlayAnim_UtilityAttack_Ready();
		static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->Set_AnimSpeed(2.f);
	}
	else if (true == m_bAnimChangeSwitch && 0.191f <= m_pModel->Get_PlayRate() && 0.446f >= m_pModel->Get_PlayRate())
	{
		m_bAnimChangeSwitch = false;
		static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->PlayAnim_NormalAttack_Shot();
	}
	else if (false == m_bAnimChangeSwitch && 0.106f <= m_pModel->Get_PlayRate() && 0.191f >= m_pModel->Get_PlayRate())
	{
		m_bAnimChangeSwitch = true;
		static_cast<CPlayerWeapon_Bow*>(m_pPlayerWeapons[WEAPON_BOW - 1])->PlayAnim_NormalAttack_Ready();
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
			m_pTransformCom->Set_MoveSpeed(2.5f);
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
	}

	if (0.98f <= fAnimPlayRate)
	{
		m_bAnimChangeSwitch = false;
		Set_State_IdleStart(fDeltaTime);
		static_cast<CPlayerWeapon_Shield*>(m_pPlayerWeapons[WEAPON_SHIELD - 1])->End_UltimateMode();
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
					m_pModel->Change_AnimIndex(SWORD_ANIM_POWER_ATK_COMBO_2_JUMPATTACK, 0.1f, true);
				else
					m_pModel->Change_AnimIndex(SWORD_ANIM_POWER_ATK_COMBO_2, 0.1f, true);
			}
		}
		break;
	}
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

_fVector CPlayer::LookAt_MousePos()
{
	_Vector		vResult = XMVectorSet(0.f, 0.f, 0.f, 1.f);
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

	Safe_Delete_Array(ViewPortDesc);
	return vResult;
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

	tMotionDesc.iNumTrailCount = 6;
	tMotionDesc.pModel = m_pModel;
	tMotionDesc.pShader = m_pShaderCom;
	tMotionDesc.vLimLight = _float4(1, 0.5f, 0, 1.f);
	tMotionDesc.iPassIndex = 5;

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_MotionTrail), TAG_COM(Com_MotionTrail), (CComponent**)&m_pMotionTrail, &tMotionDesc));

	
	return S_OK;
}

HRESULT CPlayer::SetUp_EtcInfo()
{
	m_eCurWeapon = EWEAPON_TYPE::WEAPON_NONE;
	m_eCurAnim = EPLAYERANIM_BASE::BASE_ANIM_IDLE;
	m_eCurState = EPLAYER_STATE::STATE_IDLE;

	m_fAttachCamPos_Offset = _float3(0.f, 8.f, -8.f);
	Update_AttachCamPos();


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

	ZeroMemory(m_pPlayerWeapons, sizeof(CPlayerWeapon*) * (WEAPON_END - 1));

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

HRESULT CPlayer::Adjust_AnimMovedTransform(_double fDeltatime)
{
	_uint iNowAnimIndex = m_pModel->Get_NowAnimIndex();
	_double PlayRate = m_pModel->Get_PlayRate();

	if (iNowAnimIndex != m_iOldAnimIndex || PlayRate > 0.98)
	{
		m_iAdjMovedIndex = 0;
	}


	if (PlayRate <= 0.98)
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
			}
		}
			break;
		}
	}


	m_iOldAnimIndex = iNowAnimIndex;
	return S_OK;
}

CPlayer * CPlayer::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CPlayer*	pInstance = new CPlayer(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CPlayer");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CPlayer::Clone(void * pArg)
{
	CPlayer*	pInstance = new CPlayer(*this);

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

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModel);

	Safe_Release(m_pMotionTrail);
}
