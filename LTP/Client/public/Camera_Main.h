#pragma once

#include "Camera.h"

BEGIN(Client)

class CCamera_Main :public CCamera
{
public:
	enum CameraEffectID
	{
		CAM_EFT_SHAKE,
		CAM_EFT_END
	};

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


public:
	_bool				CamActionStart(CAMERAACTION Act);
	void				Lock_CamLook(_bool bCamLock, _fVector vFixDir = {0.f, 0.f, 1.f, 0.f});





public: /* Get Camera Transform State */
	_fVector			Get_CameraState(CTransform::TransformState eState);
	_fVector			Get_CameraState_Normalize(CTransform::TransformState eState);


public:
	ECameraMode			Get_CameraMode() { return m_eCurCamMode; }


public: /* Setting State */
	void				Set_TargetArmLength(_float fTargetArmLength);
	void				Set_FocusTarget(CGameObject* pFocusTarget);
	void				Set_CameraMode(ECameraMode eCameraMode);
	void				LookAt_Target();


public: /* Getter */
	_float				Get_TargetArmLength();


private: /* Chase Target Method */
	void				ChaseTarget_NormalMode(_double fDeltaTime);


private:
	HRESULT				SetUp_Components();
	HRESULT				SetUp_EtcInfo();
	HRESULT				Update_CamAction(_double fDeltaTime);


public:
	HRESULT				Start_CameraShaking_Thread(_double TotalTime, _float Power);
	HRESULT				Progress_Shaking_Thread(_bool* _IsClientQuit, CRITICAL_SECTION* _CriSec);
	CameraEffectID		Get_EffectID() { return m_eEffectID; }

	void				Start_CameraShaking_Fov(_float fTargetFov, _float fSpeed, _float fDuraionTime);

private:
	void				ShakingCamera_Damage();

private:
	_int				Update_NormalMode(_double fDeltaTime);




private:
	_bool			m_bIsStartedShaking = false;
	CameraEffectID	m_eEffectID = CAM_EFT_END;
	_double			m_TargetTime = 0;
	_float			m_fShakingPower = 0;

private:
	ECameraMode		m_eCurCamMode = CAM_MODE_NOMAL;

private:
	_bool			m_bCamActionStart = false;
	CAMERAACTION    m_tCamAction;
	_float3			m_ReturnPos = _float3(0);
	_float3			m_ReturnLook = _float3(0);
	_uint			m_iNowPosIndex = 0;
	_uint			m_iNowLookIndex = 0;
	_double			m_ActionPassedTime = 0;
	_bool			m_bCamLock = false;
	_float3			m_fFixLookDir = { 0.f, 0.f, 0.f };

private: /* Target Info */
	CGameObject*	m_pFocusTarget = nullptr;
	_float			m_fTargetArmLength = 0.f;
	_float			m_fMax_TargetArmLength = 100.f;
	_float			m_fMin_TargetArmLength = 0.f;

private: /* Fov Shaking */
	_float			m_fFovTarget = 0.f;
	_float			m_fFovMaxTime_Shaking = 0.f;
	_float			m_fFovCurTime_Shaking = 0.f;
	_float			m_fFovSpeed = 0.f;
	_bool			m_bFovShaking = false;
	_float			m_bFovShaking_ChangeArrow = 1.f;

public:
	static CCamera_Main* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg)override;
	virtual void Free()override;
};

END