#include "stdafx.h"
#include "..\Public\Camera_Main.h"


_uint CALLBACK CameraEffectThread(void* _Prameter)
{
	THREADARG tThreadArg{};
	memcpy(&tThreadArg, _Prameter, sizeof(THREADARG));
	delete _Prameter;

	CCamera_Main::CAMERAEFFECTDESC* tCameraEffectDesc = nullptr;
	tCameraEffectDesc = (CCamera_Main::CAMERAEFFECTDESC*)tThreadArg.pArg;

	CCamera_Main* pCamemra = tCameraEffectDesc->pTargetCamera;
	switch (tCameraEffectDesc->eCameraEffectID)
	{
	case CCamera_Main::CAM_EFT_SHAKE:
		pCamemra->Progress_Shaking_Thread(tThreadArg.IsClientQuit, tThreadArg.CriSec);
		break;

	case CCamera_Main::CAM_EFT_SHAKE_DIR:
		pCamemra->Progress_DirShaking_Thread(tThreadArg.IsClientQuit, tThreadArg.CriSec, tCameraEffectDesc->tDirShakingDesc);
		break;
		
	case CCamera_Main::CAM_EFT_SHAKE_ROT:
		pCamemra->Progress_RotShaking_Thread(tThreadArg.IsClientQuit, tThreadArg.CriSec, tCameraEffectDesc->tRotShakingDesc);
		break;

	default:
		MSGBOX("worng Cam Eft");
		break;
	}

	delete tCameraEffectDesc;

	return 0;
}

CCamera_Main::CCamera_Main(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CCamera(pDevice,pDeviceContext)
{
}

CCamera_Main::CCamera_Main(const CCamera_Main & rhs)
	:CCamera(rhs)
{
}
//1
HRESULT CCamera_Main::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CCamera_Main::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (FAILED(SetUp_EtcInfo()))
		return E_FAIL;

	m_fMax_TargetArmLength = 10.f;
	m_fMin_TargetArmLength = 3.f;
	return S_OK;
}

_bool CCamera_Main::CamActionStart(CAMERAACTION Act)
{
	if (m_bCamActionStart)return false;

	m_bCamActionStart = true;
	m_tCamAction = Act;
	m_ActionPassedTime = 0;
	m_tCamAction.TotalTime = 0;

	_float TotalTime = 0;
	for (auto& iter : m_tCamAction.vecCamPos)
		TotalTime += iter.fDuration;
	if (m_tCamAction.TotalTime < TotalTime)m_tCamAction.TotalTime = TotalTime;
	TotalTime = 0;
	for (auto& iter : m_tCamAction.vecLookAt)
		TotalTime += iter.fDuration;
	if (m_tCamAction.TotalTime < TotalTime)m_tCamAction.TotalTime = TotalTime;


	m_ReturnPos = m_pTransform->Get_MatrixState_Float3(CTransform::STATE_POS);
	m_ReturnLook = m_ReturnPos.XMVector() + m_pTransform->Get_MatrixState(CTransform::STATE_LOOK);
	m_iNowPosIndex = 0;
	m_iNowLookIndex = 0;


	return true;
}

void CCamera_Main::Lock_CamLook(_bool bCamLock, _fVector vFixDir)
{
	m_bCamLock = bCamLock;
	m_fFixLookDir = vFixDir;
}

_fVector CCamera_Main::Get_CameraState(CTransform::TransformState eState)
{
	return  m_pTransform->Get_MatrixState(eState);
}

_fVector CCamera_Main::Get_CameraState_Normalize(CTransform::TransformState eState)
{
	return  XMVector3Normalize(m_pTransform->Get_MatrixState(eState));
}

void CCamera_Main::Set_TargetArmLength(_float fTargetArmLength)
{
	m_fTargetArmLength = fTargetArmLength;
}

void CCamera_Main::Set_FocusTarget(CGameObject * pFocusTarget)
{
	Safe_Release(m_pFocusTarget);
	m_pFocusTarget = pFocusTarget;
	Safe_AddRef(m_pFocusTarget);
}

void CCamera_Main::Set_CameraMode(ECameraMode eCameraMode)
{
	switch (eCameraMode)
	{
	case ECameraMode::CAM_MODE_FREE:
	{
		m_eCurCamMode = eCameraMode;
	}
		break;
	case ECameraMode::CAM_MODE_NOMAL:
	{
		m_eCurCamMode = eCameraMode;
	}
		break;
	case ECameraMode::CAM_MODE_TARGETING:
	{
		m_eCurCamMode = eCameraMode;
	}
		break;
	case ECameraMode::CAM_MODE_FIX:
	{
		m_eCurCamMode = eCameraMode;
	}
		break;
	}
}

void CCamera_Main::LookAt_Target()
{
	CTransform* pTarget_TransformCom = static_cast<CTransform*>(m_pFocusTarget->Get_Component(Tag_Component(Com_Transform)));
	_Vector vChasePos = pTarget_TransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
	m_pTransform->LookAt(vChasePos);
}

void CCamera_Main::Set_TargetingPoint(_fVector vTargetingPoint)
{
	m_fTargetingPoint = vTargetingPoint;
}

void CCamera_Main::Set_TargetingLook(_fVector vTargetingLook)
{
	m_fTargetingLook = vTargetingLook;
}

void CCamera_Main::Set_CameraLookWeight(_float fCamMoveWeight)
{
	m_fTarget_CamLookWeight = fCamMoveWeight;
}

void CCamera_Main::Set_CameraMoveWeight(_float fCamMoveWeight)
{
	m_fTarget_CamMoveWeight = fCamMoveWeight;
}

void CCamera_Main::Set_MaxTargetArmLength(_float fMaxTargetArmLength)
{
	m_fMax_TargetArmLength = fMaxTargetArmLength;
}

void CCamera_Main::Set_MinTargetArmLength(_float fMinTargetArmLength)
{
	m_fMin_TargetArmLength = fMinTargetArmLength;
}

_float CCamera_Main::Get_TargetArmLength()
{
	return m_fTargetArmLength;
}

_float CCamera_Main::Get_CameraLookWeight()
{
	return m_fCur_CamLookWeight;
}

_float CCamera_Main::Get_CameraMoveWeight()
{
	return m_fCur_CamMoveWeight;
}

void CCamera_Main::ChaseTarget_NormalMode(_double fDeltaTime)
{
	if (!m_pFocusTarget || m_pFocusTarget->Get_IsOwerDead())
	{
		Safe_Release(m_pFocusTarget);
		return;
	}
	 
	CTransform* pTarget_TransformCom =  static_cast<CTransform*>(m_pFocusTarget->Get_Component(Tag_Component(Com_Transform)));
	
	_Vector vCamPos = m_pTransform->Get_MatrixState(CTransform::TransformState::STATE_POS) * m_fCur_CamMoveWeight + m_pFocusTarget->Get_AttachCamPos() * (1.f - m_fCur_CamMoveWeight);
	m_pTransform->Set_MatrixState(CTransform::TransformState::STATE_POS, vCamPos);
	//m_pTransform->MovetoTarget_ErrRange(m_pFocusTarget->Get_AttachCamPos(), fDeltaTime, 0.1f);
	if (true == m_bCamLock) 
	{
		m_pTransform->Turn_Dir(m_fFixLookDir.XMVector(), m_fCur_CamLookWeight, 0.999f);
	}
	else
	{
		m_pTransform->Turn_Dir(m_pFocusTarget->Get_AttachCamLook(), m_fCur_CamLookWeight, 0.999f);
	}
}

_int CCamera_Main::Update(_double fDeltaTime)
{
	__super::Update(fDeltaTime);

	Update_CamMoveWeight();

	FAILED_CHECK(Update_CamAction(fDeltaTime));
	m_pTransform->Set_MoveSpeed(8.f);
	// State Update
	switch (m_eCurCamMode)
	{
	case ECameraMode::CAM_MODE_FREE:
	{
		Update_FreeMode(fDeltaTime);
	}
		break;
	case ECameraMode::CAM_MODE_NOMAL:
	{
		Update_NormalMode(fDeltaTime);
	}
		break;
	case ECameraMode::CAM_MODE_TARGETING:
	{
		Update_TargetingMode(fDeltaTime);
	}
		break;
	case ECameraMode::CAM_MODE_FIX:
	{
		// None Update
	}
	break;
	}
	//


	// Fov Shaking
	if (true == m_bFovShaking)
	{
		m_fFovCurTime_Shaking += (_float)fDeltaTime;
		if (m_fFovMaxTime_Shaking <= m_fFovCurTime_Shaking) 
		{
			m_CameraDesc.fFovy = XMConvertToRadians(60.f);
			m_fFovCurTime_Shaking = 0.f;
			m_bFovShaking = false;
		}
		else
		{
			ShakingCamera_Damage();
		}
	}
	//

	return _int();
}

_int CCamera_Main::LateUpdate(_double fDeltaTime)
{	
	__super::LateUpdate(fDeltaTime);


	return _int();
}

_int CCamera_Main::Render()
{



	return _int();
}

_int CCamera_Main::LateRender()
{


	return _int();
}


HRESULT CCamera_Main::Start_CameraShaking_Thread(_double TotalTime, _float Power, _float fChangeDirectioninterval)
{
	if (m_bIsStartedShaking) return S_FALSE;
	CAMERAEFFECTDESC* pCameraEffectDesc = NEW CAMERAEFFECTDESC();
	pCameraEffectDesc->eCameraEffectID = CAM_EFT_SHAKE;
	pCameraEffectDesc->pTargetCamera = this;

	m_eEffectID = CAM_EFT_SHAKE;
	m_TargetTime = TotalTime;
	m_fShakingPower = Power;
	m_bIsStartedShaking = true;
	m_fMaxTime_ReturnVectorInterval = fChangeDirectioninterval;

	GetSingle(CGameInstance)->PlayThread(CameraEffectThread, pCameraEffectDesc);

	return S_OK;
}

HRESULT CCamera_Main::Progress_Shaking_Thread(_bool * _IsClientQuit, CRITICAL_SECTION * _CriSec)
{
	DWORD  NowTick = GetTickCount();
	DWORD  OldTick = NowTick;

	CUtilityMgr* pUtil = GetSingle(CUtilityMgr);

	_bool	bIsReturnVector = false;
	_float3 vReturnVector = _float3(0);
	_float  vReturnPower = 0;

	_double ThreadPassedTime = 0;

	m_fCurTime_ReturnVectorInterval = m_fMaxTime_ReturnVectorInterval;

	while (true)
	{
		if (*_IsClientQuit == true)
			return S_OK;

		NowTick = GetTickCount();
		if ((NowTick - OldTick) <= g_fDeltaTime * 1000)
			continue;
		ThreadPassedTime += (NowTick - OldTick) * 0.001f;
		OldTick = NowTick;


		m_fCurTime_ReturnVectorInterval += (_float)g_fDeltaTime;
		if (m_fCurTime_ReturnVectorInterval >= m_fMaxTime_ReturnVectorInterval)
		{
			m_fCurTime_ReturnVectorInterval = 0.f;

			if (false == bIsReturnVector)
			{
				_float Rate = pUtil->RandomFloat(0, 1);
				vReturnPower = (pUtil->RandomFloat(0.8f,1.2f) * m_fShakingPower * ((rand()% 2 )?-1:1));
				EnterCriticalSection(_CriSec);
				vReturnVector = m_pTransform->Get_MatrixState(CTransform::STATE_RIGHT) * Rate + m_pTransform->Get_MatrixState(CTransform::STATE_UP) * (1 - Rate);
				LeaveCriticalSection(_CriSec);
				bIsReturnVector = true;
			}
			else
			{
				vReturnPower = -vReturnPower;
				bIsReturnVector = false;
			}
		}


		EnterCriticalSection(_CriSec);
		m_pTransform->MovetoDir_bySpeed(vReturnVector.XMVector(), vReturnPower, g_fDeltaTime);
		LeaveCriticalSection(_CriSec);

		if (m_TargetTime < ThreadPassedTime) break;

	}

	EnterCriticalSection(_CriSec);
	m_bIsStartedShaking = false;
	m_eEffectID = CAM_EFT_END;
	LeaveCriticalSection(_CriSec);


	//DWORD  NowTick = GetTickCount();
	//DWORD  OldTick = NowTick;

	//CUtilityMgr* pUtil = GetSingle(CUtilityMgr);

	//_bool	bIsReturnVector = false;
	//_float3 vReturnVector = _float3(0);
	//_float  vReturnPower = 0;

	//_double ThreadPassedTime = 0;

	//while (true)
	//{
	//	if (*_IsClientQuit == true)
	//		return S_OK;

	//	NowTick = GetTickCount();
	//	if ((NowTick - OldTick) <= g_fDeltaTime * 1000)
	//		continue;
	//	ThreadPassedTime += (NowTick - OldTick) * 0.001f;
	//	OldTick = NowTick;


	//	// 이부분 바꿔야된다
	//	// 현재는 간만큼 오고 반복인데 속도가 파워에 딱 되잇어서 딲딲딲이 되고있다
	//	// 파워에 대한 보간증가가 필요해 보임
	//	// 그러면서 지속시간을 지켜야된다
	//	if (!bIsReturnVector)
	//	{
	//		_float Rate = pUtil->RandomFloat(0, 1);
	//		vReturnPower = pUtil->RandomFloat(-m_fShakingPower, m_fShakingPower);


	//		EnterCriticalSection(_CriSec);
	//		vReturnVector = m_pTransform->Get_MatrixState(CTransform::STATE_RIGHT) * Rate + m_pTransform->Get_MatrixState(CTransform::STATE_UP) * (1 - Rate);
	//		LeaveCriticalSection(_CriSec);

	//		bIsReturnVector = true;
	//	}
	//	else
	//	{
	//		vReturnPower = -vReturnPower;
	//		bIsReturnVector = false;
	//	}
	//	//

	//	EnterCriticalSection(_CriSec);
	//	m_pTransform->MovetoDir_bySpeed(vReturnVector.XMVector(), vReturnPower, 1);
	//	LeaveCriticalSection(_CriSec);


	//	if (m_TargetTime < ThreadPassedTime) break;

	//}

	//EnterCriticalSection(_CriSec);
	//m_bIsStartedShaking = false;
	//m_eEffectID = CAM_EFT_END;
	//LeaveCriticalSection(_CriSec);

	return S_OK;
}

HRESULT CCamera_Main::Start_CameraShaking_Dir_Thread(const CAMERASHAKEDIRDESC * tDirShakingDesc)
{
	CAMERAEFFECTDESC* pCameraEffectDesc = NEW CAMERAEFFECTDESC();
	pCameraEffectDesc->eCameraEffectID = CAM_EFT_SHAKE_DIR;
	pCameraEffectDesc->pTargetCamera = this;
	pCameraEffectDesc->tDirShakingDesc = *tDirShakingDesc;

	GetSingle(CGameInstance)->PlayThread(CameraEffectThread, pCameraEffectDesc);

	return S_OK;
}

HRESULT CCamera_Main::Progress_DirShaking_Thread(_bool * _IsClientQuit, CRITICAL_SECTION * _CriSec, CAMERASHAKEDIRDESC tDirShakingDesc)
{
	DWORD  NowTick = GetTickCount();
	DWORD  OldTick = NowTick;

	CUtilityMgr* pUtil = GetSingle(CUtilityMgr);

	_bool	bIsReturnVector = false;
	_float3 vReturnVector = _float3(0);
	_float  vReturnPower = 0;

	_double ThreadPassedTime = 0;

	_float fCurTime_ReturnVectorInterval = tDirShakingDesc.fChangeDirectioninterval;

	while (true)
	{
		if (*_IsClientQuit == true)
			return S_OK;

		NowTick = GetTickCount();
		if ((NowTick - OldTick) <= g_fDeltaTime * 1000)
			continue;
		ThreadPassedTime += (NowTick - OldTick) * 0.001f;
		OldTick = NowTick;


		fCurTime_ReturnVectorInterval += (_float)g_fDeltaTime;
		if (fCurTime_ReturnVectorInterval >= tDirShakingDesc.fChangeDirectioninterval)
		{
			fCurTime_ReturnVectorInterval = 0.f;

			if (false == bIsReturnVector)
			{
				_float Rate = pUtil->RandomFloat(0, 1);
				vReturnPower = (pUtil->RandomFloat(0.8f, 1.2f) * tDirShakingDesc.fPower * ((rand() % 2) ? -1 : 1));
				EnterCriticalSection(_CriSec);
				vReturnVector = tDirShakingDesc.fShakingDir.XMVector() * Rate;
				LeaveCriticalSection(_CriSec);
				bIsReturnVector = true;
			}
			else
			{
				vReturnPower = -vReturnPower;
				bIsReturnVector = false;
			}
		}


		EnterCriticalSection(_CriSec);
		m_pTransform->MovetoDir_bySpeed(vReturnVector.XMVector(), vReturnPower, g_fDeltaTime);
		LeaveCriticalSection(_CriSec);

		if (tDirShakingDesc.fTotalTime < ThreadPassedTime) break;

	}

	return S_OK;
}

HRESULT CCamera_Main::Start_CameraShaking_Rot_Thread(const CAMERASHAKEROTDESC* tRotShakingDesc)
{
	CAMERAEFFECTDESC* pCameraEffectDesc = NEW CAMERAEFFECTDESC();
	pCameraEffectDesc->eCameraEffectID = CAM_EFT_SHAKE_ROT;
	pCameraEffectDesc->pTargetCamera = this;
	pCameraEffectDesc->tRotShakingDesc = *tRotShakingDesc;

	GetSingle(CGameInstance)->PlayThread(CameraEffectThread, pCameraEffectDesc);

	return S_OK;
}

HRESULT CCamera_Main::Progress_RotShaking_Thread(_bool* _IsClientQuit, CRITICAL_SECTION* _CriSec, CAMERASHAKEROTDESC tRotShakingDesc)
{
	DWORD  NowTick = GetTickCount();
	DWORD  OldTick = NowTick;

	CUtilityMgr* pUtil = GetSingle(CUtilityMgr);

	_bool	bIsReturnVector = false;
	_float3 vReturnVector = _float3(0);
	_float  vReturnPower = 0;

	_double ThreadPassedTime = 0;

	_float fCurTime_ReturnVectorInterval = tRotShakingDesc.fChangeDirectioninterval;

	while (true)
	{
		if (*_IsClientQuit == true)
			return S_OK;

		NowTick = GetTickCount();
		if ((NowTick - OldTick) <= g_fDeltaTime * 1000)
			continue;
		ThreadPassedTime += (NowTick - OldTick) * 0.001f;
		OldTick = NowTick;


		fCurTime_ReturnVectorInterval += (_float)g_fDeltaTime;
		if (fCurTime_ReturnVectorInterval >= tRotShakingDesc.fChangeDirectioninterval)
		{
			fCurTime_ReturnVectorInterval = 0.f;

			if (false == bIsReturnVector)
			{
				_float Rate = pUtil->RandomFloat(0, 1);
				vReturnPower = (pUtil->RandomFloat(0.8f, 1.2f) * tRotShakingDesc.fPower * ((rand() % 2) ? -1 : 1));
				
				/*
				EnterCriticalSection(_CriSec);
				vReturnVector = tRotShakingDesc.fShakingDir.XMVector() * Rate;
				LeaveCriticalSection(_CriSec);
				*/

				bIsReturnVector = true;
			}
			else
			{
				vReturnPower = -vReturnPower;
				bIsReturnVector = false;
			}
		}


		EnterCriticalSection(_CriSec);
		//m_pTransform->MovetoDir_bySpeed(vReturnVector.XMVector(), vReturnPower, g_fDeltaTime);
		m_pTransform->Turn_CCW(tRotShakingDesc.fShakingRotAxis.XMVector(), g_fDeltaTime * vReturnPower);
		LeaveCriticalSection(_CriSec);

		if (tRotShakingDesc.fTotalTime < ThreadPassedTime) break;

	}

	return S_OK;
}

void CCamera_Main::Set_EffectID(CameraEffectID eEffectID)
{
	m_eEffectID = eEffectID;
}

void CCamera_Main::Start_CameraShaking_Fov(_float fTargetFov, _float fSpeed, _float fDuraionTime)
{
	if (fTargetFov >= 60.f)
	{
		return;
	}

	m_CameraDesc.fFovy = XMConvertToRadians(60.f);
	m_fFovMaxTime_Shaking = fDuraionTime;
	m_bFovShaking_ChangeArrow = 1.f;
	m_fFovTarget = fTargetFov;
	m_fFovSpeed = fSpeed;
	m_bFovShaking = true;
}

void CCamera_Main::ShakingCamera_Damage()
{
	if (XMConvertToRadians(60.f) <= m_CameraDesc.fFovy)
	{
		m_bFovShaking_ChangeArrow = -1.f;
	}
	else if(XMConvertToRadians(m_fFovTarget) >= m_CameraDesc.fFovy)
	{
		m_bFovShaking_ChangeArrow = 1.f;
	}

	m_CameraDesc.fFovy += XMConvertToRadians(m_bFovShaking_ChangeArrow * m_fFovSpeed);
}

HRESULT CCamera_Main::Set_ViewMatrix()
{
	CGameInstance* pIsntance = GetSingle(CGameInstance);
	pIsntance->Set_Transform(PLM_VIEW, m_pTransform->Get_InverseWorldMatrix());
	pIsntance->Set_TargetPostion(PLV_CAMERA, *((_float4*)(m_pTransform->Get_WorldFloat4x4().m[3])));
	pIsntance->Set_TargetPostion(PLV_CAMLOOK, *((_float4*)(m_pTransform->Get_WorldFloat4x4().m[2])));

	m_vOldPos = m_pTransform->Get_MatrixState_Float3(CTransform::STATE_POS);
	return S_OK;
}

_int CCamera_Main::Update_FreeMode(_double fDeltaTime)
{
	if (g_pGameInstance->Get_DIKeyState(DIK_W) & DIS_Press)
	{
		m_pTransform->Move_Forward(fDeltaTime);
	}
	if (g_pGameInstance->Get_DIKeyState(DIK_S) & DIS_Press)
	{
		m_pTransform->Move_Backward(fDeltaTime);
	}
	if (g_pGameInstance->Get_DIKeyState(DIK_D) & DIS_Press)
	{
		m_pTransform->Move_Right(fDeltaTime);
	}
	if (g_pGameInstance->Get_DIKeyState(DIK_A) & DIS_Press)
	{
		m_pTransform->Move_Left(fDeltaTime);
	}
	if (g_pGameInstance->Get_DIKeyState(DIK_Q) & DIS_Press)
	{
		m_pTransform->MovetoDir(XMVectorSet(0, 1, 0, 0), fDeltaTime);
	}
	if (g_pGameInstance->Get_DIKeyState(DIK_E) & DIS_Press)
	{
		m_pTransform->MovetoDir(XMVectorSet(0, -1, 0, 0), fDeltaTime);
	}


	{
		static _bool IsWheelClicked = false;
		_byte BtnState = g_pGameInstance->Get_DIMouseButtonState(CInput_Device::MBS_WHEEL);
		if (BtnState & DIS_Press)
		{
			if (!IsWheelClicked && (BtnState & DIS_Down))
				IsWheelClicked = true;
			else if (IsWheelClicked && (BtnState & DIS_Up))
				IsWheelClicked = false;
			else if (IsWheelClicked && (BtnState & DIS_Press))
			{
				if (g_pGameInstance->Get_DIKeyState(DIK_LSHIFT)&DIS_Press)
				{

					_long fWheelMove = g_pGameInstance->Get_DIMouseMoveState(CInput_Device::MMS_Y);

					m_pTransform->Turn_CW(m_pTransform->Get_MatrixState(CTransform::STATE_RIGHT), fWheelMove*fDeltaTime* 0.1f);

					fWheelMove = g_pGameInstance->Get_DIMouseMoveState(CInput_Device::MMS_X);

					m_pTransform->Turn_CW(XMVectorSet(0, 1, 0, 0), fWheelMove* fDeltaTime * 0.1f);


				}
				else {

					_long fWheelMove = g_pGameInstance->Get_DIMouseMoveState(CInput_Device::MMS_Y);

					m_pTransform->MovetoDir_bySpeed(
						m_pTransform->Get_MatrixState(CTransform::STATE_UP), (_float)fWheelMove, fDeltaTime);

					fWheelMove = g_pGameInstance->Get_DIMouseMoveState(CInput_Device::MMS_X);

					m_pTransform->MovetoDir_bySpeed(
						m_pTransform->Get_MatrixState(CTransform::STATE_RIGHT), (_float)-fWheelMove, fDeltaTime);


				}


			}

		}
		if (g_pGameInstance->Get_DIKeyState(DIK_LSHIFT)&DIS_Press)
		{
			_long fWheelMove = g_pGameInstance->Get_DIMouseMoveState(CInput_Device::MMS_WHEEL);
			if (fWheelMove)
			{
				m_pTransform->MovetoDir_bySpeed(
					m_pTransform->Get_MatrixState(CTransform::STATE_LOOK), (_float)fWheelMove, fDeltaTime);
			}

		}

	}
	return _int();
}

_int CCamera_Main::Update_NormalMode(_double fDeltaTime)
{
	if (nullptr == m_pFocusTarget || m_eNowSceneNum == SCENE_LOADING)
		return 0;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	if (pGameInstance->Get_DIMouseMoveState(CInput_Device::MMS_WHEEL) < 0)
	{
		m_fTargetArmLength += 1.f;
		//if (m_fTargetArmLength > m_fMax_TargetArmLength)
		//{
		//	m_fTargetArmLength -= 1.5f;
		//	if (m_fTargetArmLength < m_fMax_TargetArmLength)
		//	{
		//		m_fTargetArmLength = m_fMax_TargetArmLength;
		//	}
		//}
		m_fTargetArmLength = (m_fTargetArmLength >= m_fMax_TargetArmLength ? m_fMax_TargetArmLength : m_fTargetArmLength);
	}
	else if (pGameInstance->Get_DIMouseMoveState(CInput_Device::MMS_WHEEL) > 0)
	{
		m_fTargetArmLength -= 1.f;
		//if (m_fTargetArmLength < m_fMin_TargetArmLength)
		//{
		//	m_fTargetArmLength += 1.5f;
		//	if (m_fTargetArmLength > m_fMin_TargetArmLength)
		//	{
		//		m_fTargetArmLength = m_fMin_TargetArmLength;
		//	}
		//}
		m_fTargetArmLength = (m_fTargetArmLength <= m_fMin_TargetArmLength ? m_fMin_TargetArmLength : m_fTargetArmLength);
	}


	if (m_fTargetArmLength > m_fMax_TargetArmLength)
	{
		m_fTargetArmLength -= 0.1f;
		if (m_fTargetArmLength < m_fMax_TargetArmLength)
		{
			m_fTargetArmLength = m_fMax_TargetArmLength;
		}
	}
	else if (m_fTargetArmLength < m_fMin_TargetArmLength)
	{
		m_fTargetArmLength += 0.1f;
		if (m_fTargetArmLength > m_fMin_TargetArmLength)
		{
			m_fTargetArmLength = m_fMin_TargetArmLength;
		}
	}

	ChaseTarget_NormalMode(fDeltaTime);

	RELEASE_INSTANCE(CGameInstance);

	return _int();
} 

_int CCamera_Main::Update_TargetingMode(_double fDeltaTime)
{
	_Vector vCamPos = (m_pTransform->Get_MatrixState(CTransform::TransformState::STATE_POS) * m_fCur_CamMoveWeight) + ((m_fTargetingPoint.XMVector() - m_fTargetingLook.XMVector()) * (1.f - m_fCur_CamMoveWeight));
	//m_pTransform->MovetoTarget_ErrRange((m_fTargetingPoint.XMVector() - m_fTargetingLook.XMVector()), fDeltaTime, 0.1f);
	m_pTransform->Turn_Dir(XMVector3Normalize(m_fTargetingLook.XMVector()), m_fCur_CamLookWeight, 0.99f);

	m_pTransform->Set_MatrixState(CTransform::TransformState::STATE_POS, vCamPos);
	
	return _int();
}

void CCamera_Main::Update_CamMoveWeight()
{
	if (m_fCur_CamLookWeight < m_fTarget_CamLookWeight)
	{
		m_fCur_CamLookWeight = (m_fCur_CamLookWeight * 0.001f) + (m_fTarget_CamLookWeight * 0.999f);
	}
	else
	{
		m_fCur_CamLookWeight = (m_fCur_CamLookWeight * 0.999f) + (m_fTarget_CamLookWeight * 0.001f);
	}

	if (m_fCur_CamMoveWeight < m_fTarget_CamMoveWeight)
	{
		m_fCur_CamMoveWeight = (m_fCur_CamMoveWeight * 0.001f) + (m_fTarget_CamMoveWeight * 0.999f);
	}
	else
	{
		m_fCur_CamMoveWeight = (m_fCur_CamMoveWeight * 0.999f) + (m_fTarget_CamMoveWeight * 0.001f);
	}
}

HRESULT CCamera_Main::SetUp_Components()
{

	return S_OK;
}

HRESULT CCamera_Main::SetUp_EtcInfo()
{
	m_pTransform->Set_MoveSpeed(1.f);
	return S_OK;
}

#define EASINGTYPE TYPE_SinInOut

HRESULT CCamera_Main::Update_CamAction(_double fDeltaTime)
{
	if (!m_bCamActionStart)return S_FALSE;

	_Matrix NewCamMat = XMMatrixIdentity();
	CGameInstance* pInstance = g_pGameInstance;



	m_ActionPassedTime += fDeltaTime;

	if (m_ActionPassedTime > m_tCamAction.TotalTime)
		//마지막 프레임까지 돌았다면 원래 위치로 되돌려라
	{
		m_ActionPassedTime = 0;
		m_bCamActionStart = false;

		//_float3 EasedPos;
		//_float3 EasedLookAt;


		//_float3 Old = m_tCamAction.vecCamPos[m_tCamAction.vecCamPos.size() - 1].vPosition;
		//_float3 Now = m_ReturnPos;

		//EasedPos = pInstance->Easing_Vector(EASINGTYPE, Old, Now, _float(m_ActionPassedTime - m_tCamAction.TotalTime), 0.5f);

		//Old = m_tCamAction.vecLookAt[m_tCamAction.vecLookAt.size() - 1].vPosition;
		//Now = m_ReturnLook;

		//EasedLookAt = pInstance->Easing_Vector(EASINGTYPE, Old, Now, _float(m_ActionPassedTime - m_tCamAction.TotalTime), 0.5f);

		//if (m_ActionPassedTime > m_tCamAction.TotalTime + 0.5f)
		//{
		//	EasedPos = m_ReturnPos;
		//	EasedLookAt = m_ReturnLook;
		//	m_ActionPassedTime = 0;
		//	m_bCamActionStart = false;
		//}

		//m_pTransform->Set_MatrixState(CTransform::STATE_POS, EasedPos);
		//m_pTransform->LookAt(EasedLookAt.XMVector());
	}
	else
		//마지막 프레임까지 돌지 않았다면 보간하면서 라업룩포를 구해라
	{
		_float3 EasedPos;
		_float3 EasedLookAt;


		_float3 Old = m_ReturnPos;
		_float3 Now = m_tCamAction.vecCamPos[0].vPosition;
		_float	Rate = 0;
		_float	TargetTime = 0;
		_uint	iTargetIndex = _uint(m_tCamAction.vecCamPos.size() - 1);

		for (_uint i = 0; i < m_tCamAction.vecCamPos.size(); i++)
		{
			if (TargetTime + m_tCamAction.vecCamPos[i].fDuration > m_ActionPassedTime)
			{
				iTargetIndex = i;
				break;
			}

			TargetTime += m_tCamAction.vecCamPos[i].fDuration;
			Old = m_tCamAction.vecCamPos[i].vPosition;
		}

		Now = m_tCamAction.vecCamPos[iTargetIndex].vPosition;
		_float TempPassedTime = min(_float(m_ActionPassedTime - TargetTime), m_tCamAction.vecCamPos[iTargetIndex].fDuration);

		EasedPos = pInstance->Easing_Vector(EASINGTYPE, Old, Now, TempPassedTime, m_tCamAction.vecCamPos[iTargetIndex].fDuration);


		//////////////////////////////////////////////////////////////////////////
		Old = m_ReturnLook;
		Now = m_tCamAction.vecLookAt[0].vPosition;
		Rate = 0;
		TargetTime = 0;
		iTargetIndex = _uint(m_tCamAction.vecLookAt.size() - 1);
		TempPassedTime = 0;

		for (_uint i = 0; i < m_tCamAction.vecLookAt.size(); i++)
		{
			if (TargetTime + m_tCamAction.vecLookAt[i].fDuration > m_ActionPassedTime)
			{
				iTargetIndex = i;
				break;
			}

			TargetTime += m_tCamAction.vecLookAt[i].fDuration;
			Old = m_tCamAction.vecLookAt[i].vPosition;
		}

		Now = m_tCamAction.vecLookAt[iTargetIndex].vPosition;
		TempPassedTime = min(_float(m_ActionPassedTime - TargetTime), m_tCamAction.vecLookAt[iTargetIndex].fDuration);

		EasedLookAt = pInstance->Easing_Vector(EASINGTYPE, Old, Now, TempPassedTime, m_tCamAction.vecLookAt[iTargetIndex].fDuration);



		m_pTransform->Set_MatrixState(CTransform::STATE_POS, EasedPos);
		m_pTransform->LookAt(EasedLookAt.XMVector());

	}


	return S_OK;
}



CCamera_Main * CCamera_Main::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CCamera_Main* pInstance = NEW CCamera_Main(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg))) {
		MSGBOX("Failed to Create CCamera_Main");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CGameObject * CCamera_Main::Clone(void * pArg)
{
	CCamera_Main* pInstance = NEW CCamera_Main(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg))) {
		MSGBOX("Failed to Create CCamera_Main");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CCamera_Main::Free()
{
	__super::Free();


 	Safe_Release(m_pFocusTarget);
}
