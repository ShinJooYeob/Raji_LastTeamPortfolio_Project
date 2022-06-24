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
	explicit CCamera_Main(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	explicit CCamera_Main(const CCamera_Main&  rhs);
	virtual ~CCamera_Main() = default;
public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

public:
	_bool Get_IsCamAction() { return m_bCamActionStart; };

public:
	_bool	CamActionStart(CAMERAACTION Act);
private:
	//_bool			m_bIsCamMoving = false;
	_bool			m_bCamActionStart = false;
	CAMERAACTION    m_tCamAction;
	_float3			m_ReturnPos = _float3(0);
	_float3			m_ReturnLook = _float3(0);
	_uint			m_iNowPosIndex = 0;
	_uint			m_iNowLookIndex = 0;
	_double			m_ActionPassedTime = 0;

public:
	virtual _int Update(_double fDeltaTime)override;
	virtual _int LateUpdate(_double fDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;

private:

	HRESULT			SetUp_Components();
	HRESULT			Update_CamAction(_double fDeltaTime);





public:
	HRESULT		Start_CameraShaking_Thread(_double TotalTime, _float Power);
	HRESULT		Progress_Shaking_Thread(_bool* _IsClientQuit, CRITICAL_SECTION* _CriSec);
	CameraEffectID Get_EffectID() { return m_eEffectID; }


	virtual HRESULT Set_ViewMatrix() override;

private:
	_bool				m_bIsStartedShaking = false;
	CameraEffectID		m_eEffectID = CAM_EFT_END;
	_double				m_TargetTime = 0;
	_float				m_fShakingPower = 0;


public:
	static CCamera_Main* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg)override;
	virtual void Free()override;
};

END