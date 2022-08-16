#pragma once

#include "Camera.h"

BEGIN(Client)

class CCamera_Main :public CCamera
{
public:
	enum CameraEffectID
	{
		CAM_EFT_SHAKE,
		CAM_EFT_SHAKE_DIR,
		CAM_EFT_SHAKE_ROT,
		CAM_EFT_END
	};

	typedef struct tagCameraShakeDirDesc
	{
		_float		fTotalTime;
		_float		fPower;
		_float		fChangeDirectioninterval;
		_float3		fShakingDir;
	}CAMERASHAKEDIRDESC;

	typedef struct tagCameraShakeRotDesc
	{
		_float		fTotalTime;
		_float		fPower;
		_float		fChangeDirectioninterval;
		_float3		fShakingRotAxis;
	}CAMERASHAKEROTDESC;

	typedef struct tagCameraEffectDesc
	{
		CCamera_Main*		pTargetCamera;
		CameraEffectID		eCameraEffectID;
		CAMERASHAKEDIRDESC	tDirShakingDesc;
		CAMERASHAKEROTDESC	tRotShakingDesc;
		_bool				bOnceShake;
	}CAMERAEFFECTDESC;

private:
	explicit			CCamera_Main(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	explicit			CCamera_Main(const CCamera_Main&  rhs);
	virtual				~CCamera_Main() = default;


public:
	virtual HRESULT		Initialize_Prototype(void* pArg)override;
	virtual HRESULT		Initialize_Clone(void* pArg)override;

	virtual _int		Update(_double fDeltaTime)override;
	virtual _int		LateUpdate(_double fDeltaTime)override;
	virtual _int		Render()override;
	virtual _int		LateRender()override;

	virtual HRESULT		Set_ViewMatrix() override;


public:
	_bool				Get_IsCamAction() { return m_bCamActionStart; };
	_bool				Get_CamLock() { return m_bCamLock; }
	_float3				Get_FixLookDir() { return m_fFixLookDir; }
	CTransform*			Get_CamTransformCom() { return m_pTransform; }

public:
	_bool				CamActionStart(CAMERAACTION Act);
	void				Lock_CamLook(_bool bCamLock, _fVector vFixDir = {0.f, 0.f, 1.f, 0.f});

public:
	void				Ortho_OnOff(_bool bBool, _float fScreenYPixel = 20.f);


	
public: /* Get Camera Transform State */
	_fVector			Get_CameraState(CTransform::TransformState eState);
	_fVector			Get_CameraState_Normalize(CTransform::TransformState eState);

public: /* Set Camera Pos/Look*/
	void				Set_CameraPos(_fVector vPos);
	void				Set_CameraLookAt(_fVector vLookAt);
	void				Set_CameraLookAt_Turn(_fVector vLookAt);

public:
	ECameraMode			Get_CameraMode() { return m_eCurCamMode; }


public: /* Setting State */
	void				Set_TargetArmLength(_float fTargetArmLength);
	void				Set_FocusTarget(CGameObject* pFocusTarget);
	void				Set_CameraMode(ECameraMode eCameraMode);
	void				LookAt_Target();
	void				Set_TargetingPoint(_fVector vTargetingPoint);
	void				Set_TargetingLook(_fVector vTargetingLook);
	void				Set_CameraLookWeight(_float fCamMoveWeight);
	void				Set_CameraMoveWeight(_float fCamMoveWeight);
	void				Set_MaxTargetArmLength(_float fMaxTargetArmLength);
	void				Set_MinTargetArmLength(_float fMinTargetArmLength);
	void				Set_CameraInitState(_fVector vCamPos, _fVector vCamLook);
	void				Set_CamLock(_bool bLock);
	void				Set_StartedShaking(_bool bValue);

public: /* Getter */
	_float				Get_TargetArmLength();
	_float				Get_CameraLookWeight();
	_float				Get_CameraMoveWeight();

private: /* Chase Target Method */
	void				ChaseTarget_NormalMode(_double fDeltaTime);


public: /* Camera Shake */
	HRESULT				Start_CameraShaking_Thread(_double TotalTime, _float Power, _float fChangeDirectioninterval, _bool bOnceShake);
	HRESULT				Progress_Shaking_Thread(_bool* _IsClientQuit, CRITICAL_SECTION* _CriSec);

	HRESULT				Start_CameraShaking_Dir_Thread(const CAMERASHAKEDIRDESC* tDirShakingDesc, _bool bOnceShake);
	HRESULT				Progress_DirShaking_Thread(_bool* _IsClientQuit, CRITICAL_SECTION* _CriSec, CAMERASHAKEDIRDESC tDirShakingDesc);

	HRESULT				Start_CameraShaking_Rot_Thread(const CAMERASHAKEROTDESC* tRotShakingDesc, _bool bOnceShake);
	HRESULT				Progress_RotShaking_Thread(_bool* _IsClientQuit, CRITICAL_SECTION* _CriSec, CAMERASHAKEROTDESC tRotShakingDesc);

	void				Set_EffectID(CameraEffectID eEffectID);
	CameraEffectID		Get_EffectID() { return m_eEffectID; }
	
	void				Start_CameraShaking_Fov(_float fTargetFov, _float fSpeed, _float fDuraionTime, _bool bOnceShake);

private:
	HRESULT				SetUp_Components();
	HRESULT				SetUp_EtcInfo();
	HRESULT				Update_CamAction(_double fDeltaTime);


private:
	void				ShakingCamera_Damage();


private:
	_int				Update_FreeMode(_double fDeltaTime);
	_int				Update_NormalMode(_double fDeltaTime);
	_int				Update_TargetingMode(_double fDeltaTime);
	_int				Update_RajiGolu_MiniGameMode(_double fDeltaTime);
	_int				Update_FirstPersonView(_double fDeltaTime);

	void				Update_CamMoveWeight();

private:
	_bool						m_bIsStartedShaking = false;
	CameraEffectID				m_eEffectID = CAM_EFT_END;
	_double						m_TargetTime = 0;
	_float						m_fShakingPower = 0;

private:
	ECameraMode					m_eCurCamMode = CAM_MODE_NOMAL;

private:
	_bool						m_bCamActionStart = false;
	CAMERAACTION				m_tCamAction;
	_float3						m_ReturnPos = _float3(0);
	_float3						m_ReturnLook = _float3(0);
	_uint						m_iNowPosIndex = 0;
	_uint						m_iNowLookIndex = 0;
	_double						m_ActionPassedTime = 0;
	_bool						m_bCamLock = false;
	_float3						m_fFixLookDir = { 0.f, 0.f, 0.f };
	_float						m_fMaxTime_ReturnVectorInterval = 0.f;
	_float						m_fCurTime_ReturnVectorInterval = 0.f;
	
	_float						m_fTarget_CamLookWeight = 0.9f;
	_float						m_fCur_CamLookWeight = 0.9f;

	_float						m_fTarget_CamMoveWeight = 0.9f;
	_float						m_fCur_CamMoveWeight = 0.9f;

private: /* Target Info */
	CGameObject*				m_pFocusTarget = nullptr;
	CTransform*					m_pPlayerTramsformCom = nullptr;
	_float						m_fTargetArmLength = 0.f;
	_float						m_fMax_TargetArmLength = 100.f;
	_float						m_fMin_TargetArmLength = 0.f;

private:
	_float3						m_fTargetingPoint = _float3(0.f, 0.f, 0.f);
	_float3						m_fTargetingLook = _float3(0.f, 0.f, 0.f);
	_float						m_fTargetingArmLength = 0.f;

private: /* Fov Shaking */
	_float						m_fFovTarget = 0.f;
	_float						m_fFovMaxTime_Shaking = 0.f;
	_float						m_fFovCurTime_Shaking = 0.f;
	_float						m_fFovSpeed = 0.f;
	_bool						m_bFovShaking = false;
	_float						m_bFovShaking_ChangeArrow = 1.f;

private: /* For ETC */
	ATTACHEDESC					m_tAttachDesc;

public:
	static CCamera_Main* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg)override;
	virtual void Free()override;
};

END