#include "stdafx.h"
#include "..\Public\Player.h"
#include "Camera_Main.h"

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

	return S_OK;
}

_int CPlayer::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0) return -1;
	

	// Check Player Key Input
	Check_PlayerKeyInput(fDeltaTime);


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
	m_fAttachCamPos_Offset = _float3(0.f, 3.f, -2.f);
	Update_AttachCamPos();
	//

	return _int();
}

_int CPlayer::LateUpdate(_double fDeltaTimer)
{
	if (__super::LateUpdate(fDeltaTimer) < 0)return -1;

	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
	FAILED_CHECK(m_pRendererCom->Add_ShadowGroup(CRenderer::SHADOW_ANIMMODEL, this, m_pTransformCom, m_pShaderCom, m_pModel));

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

void CPlayer::Set_State_IdleStart(_double fDeltaTime)
{
	Set_PlayerState(STATE_IDLE);
	m_pModel->Change_AnimIndex(BASE_ANIM_IDLE, 0.1f);
}

void CPlayer::Set_State_MoveStart(_double fDeltaTime)
{
	Move(m_eInputDir, fDeltaTime);
	Set_PlayerState(STATE_MOV);
	m_pModel->Change_AnimIndex(BASE_ANIM_RUN_F, 0.1f);
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

	if (true == m_bPressedDodgeKey)
	{
		Set_State_DodgeStart(fDeltaTime);
	}

	if (true == m_bPressedMainAttackKey || true == m_bPressedPowerAttackKey)
	{
		Set_State_MainAttackStart(fDeltaTime);
	}

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

		if (true == m_bPressedDodgeKey)
		{
			Set_State_DodgeStart(fDeltaTime);
		}

		if (true == m_bPressedMainAttackKey || true == m_bPressedPowerAttackKey)
		{
			Set_State_MainAttackStart(fDeltaTime);
		}
	}
	
	return _int();
}

HRESULT CPlayer::Update_State_Attack(_double fDeltaTime)
{
	if (true == m_bAttackEnd && true == m_bPressedPowerAttackKey)
	{
		m_bPlayPowerAttack = true; 
	}

	if (true == m_bAttackEnd)
	{
		Set_MainAttackAnim(m_bPlayJumpAttack);
		m_bAttackEnd = false;
	}

	Attack(fDeltaTime);

	return S_OK;
}

HRESULT CPlayer::Update_State_PowerAttack(_double fDeltaTime)
{
	Set_PowerAttackAnim(m_bPlayJumpAttack);

	Attack(fDeltaTime);

	return S_OK;
}

HRESULT CPlayer::Update_State_JumpAttack(_double fDeltaTime)
{
	return E_NOTIMPL;
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
	

	if (pGameInstance->Get_DIKeyState(DIK_SPACE) & DIS_Down)
	{
		m_bPressedDodgeKey = true;
	}
	else
	{
		m_bPressedDodgeKey = false;
	}
	
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

	RELEASE_INSTANCE(CGameInstance);
	return false;
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
		// Speed Interp
		/*m_fCurMovSpeedIncRate += 0.01f;
		if (1.f <= m_fCurMovSpeedIncRate)
		{
			m_fCurMovSpeedIncRate = 1.f;
		}
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
		m_fPlayerCurSpeed = pGameInstance->Easing(TYPE_Linear, 0.f, m_fPlayerMaxSpeed, m_fCurMovSpeedIncRate, 1.f);
		RELEASE_INSTANCE(CGameInstance);
		m_pTransformCom->Set_MoveSpeed(m_fPlayerCurSpeed);*/


		m_pTransformCom->MovetoDir(vMovDir, fMoveRate);
		m_pTransformCom->Turn_Dir(vMovDir, fTurnRate);
		//m_fAnimSpeed *= m_fCurMovSpeedIncRate;
	}

	m_fMovDir = XMVector3Normalize(vMovDir);
}

void CPlayer::Turn_Back(_double fDeltaTime)
{
	_float fRate = (_float)m_pModel->Get_PlayRate();
	
	_float fDecAcc = g_pGameInstance->Easing(TYPE_ExpoOut, 0.f, (_float)fDeltaTime, fRate, 1.f);
	m_pTransformCom->Move_Forward((fDeltaTime - fDecAcc) * 0.5f);

	if (0.2f <= fRate && 0.7f >= fRate)
	{
		m_fAnimSpeed = 1.5f;
		m_pTransformCom->Turn_Dir(m_pTransformCom->Get_MatrixState_Normalized(CTransform::TransformState::STATE_LOOK) * -1.f, 0.77f);
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
		// 1) 다음 콤보 커멘트 입력 체크
		Check_NextComboCommand();

		// 2) 끝난 다음 전환 될 콤보 행동 체크
		if (0.45f <= fAnimPlayRate && 0.52f >= fAnimPlayRate)
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
			Change_NextCombo();
		}
		/////////////////////////////////////////////////////////

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
		// 1) 다음 콤보 커멘트 입력 체크
		Check_NextComboCommand();

		// 2) 끝난 다음 전환 될 콤보 행동 체크
		if (0.5f <= fAnimPlayRate && 0.65f >= fAnimPlayRate)
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
			Change_NextCombo();
		}
		/////////////////////////////////////////////////////////

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

		if (0.5f <= fAnimPlayRate && 0.8f >= fAnimPlayRate)
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
			Change_NextCombo();
		}
		//

		break;
	}

	m_bDodging = true;
}

void CPlayer::Attack(_double fDeltaTime)
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
		// 1) 다음 콤보 커멘트 입력 체크
		Check_NextComboCommand();

		// 2) 끝난 다음 전환 될 콤보 행동 체크
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
		// 1) 다음 콤보 커멘트 입력 체크
		Check_NextComboCommand();

		// 2) 끝난 다음 전환 될 콤보 행동 체크
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
		// 1) 다음 콤보 커멘트 입력 체크
		Check_NextComboCommand();

		// 2) 끝난 다음 전환 될 콤보 행동 체크
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
		// 1) 다음 콤보 커멘트 입력 체크
		Check_NextComboCommand();

		// 2) 끝난 다음 전환 될 콤보 행동 체크
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
		// 1) 다음 콤보 커멘트 입력 체크
		Check_NextComboCommand();

		if (true == m_bPlayNextCombo)
		{
			// 2) 끝난 다음 전환 될 콤보 행동 체크
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
		//// 1) 다음 콤보 커멘트 입력 체크
		Check_NextComboCommand();

		// 2) 끝난 다음 전환 될 콤보 행동 체크
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
		//// 1) 다음 콤보 커멘트 입력 체크
		Check_NextComboCommand();

		// 2) 끝난 다음 전환 될 콤보 행동 체크
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
		if (fAnimPlayRate <= 0.425f)
		{
			m_fAnimSpeed = 2.f;
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
		//// 1) 다음 콤보 커멘트 입력 체크
		Check_NextComboCommand();

		// 2) 끝난 다음 전환 될 콤보 행동 체크
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
	}
	m_bMainAttacking = true;
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
					m_pModel->Change_AnimIndex(SPEAR_ANIM_MAIN_ATK_COMBO_0_JUMPATTACK, 0.5f, true);
				else
					m_pModel->Change_AnimIndex_ReturnTo(SPEAR_ANIM_MAIN_ATK_COMBO_0, SPEAR_ANIM_MAIN_ATK_COMBO_0_RECORVERY, 0.1f, true);
			}
			else
			{
				if (true == bJumpAttack)
					m_pModel->Change_AnimIndex(SPEAR_ANIM_POWER_ATK_COMBO_0_JUMPATTACK, 0.5f, true);
				else
					m_pModel->Change_AnimIndex(SPEAR_ANIM_POWER_ATK_COMBO_0, 0.1f, true);
			}
		}
		else if (2 == m_iCurCombo)
		{
			if (false == m_bPlayPowerAttack)
			{
				if (true == bJumpAttack)
					m_pModel->Change_AnimIndex(SPEAR_ANIM_MAIN_ATK_COMBO_1_JUMPATTACK, 0.4f, true);
				else
					m_pModel->Change_AnimIndex_ReturnTo(SPEAR_ANIM_MAIN_ATK_COMBO_1, SPEAR_ANIM_MAIN_ATK_COMBO_1_RECORVERY, 0.1f, true);
			}
			else
			{
				if (true == bJumpAttack)
					m_pModel->Change_AnimIndex(SPEAR_ANIM_POWER_ATK_COMBO_1_JUMPATTACK, 0.5f, true);
				else
					m_pModel->Change_AnimIndex(SPEAR_ANIM_POWER_ATK_COMBO_1, 0.1f, true);
			}
		}
		else if (3 == m_iCurCombo)
		{
			if (false == m_bPlayPowerAttack)
			{
				if (true == bJumpAttack)
					m_pModel->Change_AnimIndex(SPEAR_ANIM_MAIN_ATK_COMBO_2_JUMPATTACK, 0.5f, true);
				else
					m_pModel->Change_AnimIndex(SPEAR_ANIM_MAIN_ATK_COMBO_2, 0.1f, true);
			}
			else
			{
				if (true == bJumpAttack) 
					m_pModel->Change_AnimIndex(SPEAR_ANIM_POWER_ATK_COMBO_2_JUMPATTACK, 0.5f, true);
				else
					m_pModel->Change_AnimIndex(SPEAR_ANIM_POWER_ATK_COMBO_2, 0.1f, true);
			}
		}
		break;
	}
}

void CPlayer::Set_PowerAttackAnim(_bool bJumpAttack)
{
	switch (m_eCurWeapon)
	{
	case EWEAPON_TYPE::WEAPON_SPEAR:
		if (1 == m_iCurCombo)
		{
			if (true == bJumpAttack)
				m_pModel->Change_AnimIndex(SPEAR_ANIM_POWER_ATK_COMBO_0_JUMPATTACK, 0.5f, true);
			else
				m_pModel->Change_AnimIndex(SPEAR_ANIM_POWER_ATK_COMBO_0, 0.1f, true);
		}
		else if (2 == m_iCurCombo)
		{
			if (true == bJumpAttack)
				m_pModel->Change_AnimIndex(SPEAR_ANIM_POWER_ATK_COMBO_1_JUMPATTACK, 0.5f, true);
			else
				m_pModel->Change_AnimIndex(SPEAR_ANIM_POWER_ATK_COMBO_1, 0.1f, true);
		}
		else if (3 == m_iCurCombo)
		{
			if (true == bJumpAttack)
				m_pModel->Change_AnimIndex(SPEAR_ANIM_POWER_ATK_COMBO_2_JUMPATTACK, 0.5f, true);
			else
				m_pModel->Change_AnimIndex(SPEAR_ANIM_POWER_ATK_COMBO_2, 0.1f, true);
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

void CPlayer::Change_NextCombo()
{
	if (false == m_bPlayNextCombo)
		return;

	m_pModel->Set_BlockAnim(false);

	if (true == m_bReadyMainAttackCombo)
	{
		m_bPlayMainAttackCombo = true;
	}
	else if (true == m_bReadyDodgeCombo)
	{
		m_bPlayDodgeCombo = true;
	}
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


	return S_OK;
}

HRESULT CPlayer::SetUp_EtcInfo()
{
	m_eCurWeapon = EWEAPON_TYPE::WEAPON_SPEAR;
	m_eCurAnim = EPLAYERANIM_BASE::BASE_ANIM_IDLE;
	m_eCurState = EPLAYER_STATE::STATE_IDLE;

	m_fAttachCamPos_Offset = _float3(0.f, 8.f, -8.f);
	Update_AttachCamPos();
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
					}
					else											// Change to Dodge Combo
					{
						m_bPlayDodgeCombo = false;
						m_bReadyDodgeCombo = false;
 						m_iCurCombo = (m_iCurCombo % m_iMaxCombo) + 1;
						Set_TurnInputDir_CalDir();
					}
				}

			}
			else
			{
				LookAtInputDir(fDeltatime);
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
			if (false == m_pModel->Get_IsHavetoBlockAnimChange())
			{
				// If Input Next Combo Command
				if(true == m_bPlayNextCombo)
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
					m_pModel->Change_AnimIndex(BASE_ANIM_RUN_F, 0.3f);
					m_iCurCombo = 1;
					m_bMainAttacking = false;
					m_bPlayJumpAttack = false;
					m_bPlayPowerAttack = false;
				}
				else if (MOVDIR_END == m_eInputDir && true == m_bAttackEnd)			// If not input Command & not input Mov Command
				{
					Set_PlayerState(STATE_IDLE);
					m_pModel->Change_AnimIndex(BASE_ANIM_IDLE, 0.1f);
					m_iCurCombo = 1;
					m_bMainAttacking = false;
					m_bPlayJumpAttack = false;
					m_bPlayPowerAttack = false;
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
}
