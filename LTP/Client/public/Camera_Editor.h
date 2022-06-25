#pragma once

#include "Camera.h"

BEGIN(Client)

class CCamera_Editor :public CCamera
{
private:
	explicit CCamera_Editor(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	explicit CCamera_Editor(const CCamera_Editor&  rhs);
	virtual ~CCamera_Editor() = default;
public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

public:
	virtual _int Update(_double fDeltaTime)override;
	virtual _int LateUpdate(_double fDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;


public:
	_bool	CamActionStart(CAMERAACTION Act);
private:
	_bool			m_bIsCamMoving = false;

private:
	HRESULT			SetUp_Components();

	HRESULT			Update_CamAction(_double fDeltaTime);

private:
	_bool			m_bCamActionStart = false;
	CAMERAACTION    m_tCamAction;
	_float3			m_ReturnPos = _float3(0);
	_float3			m_ReturnLook = _float3(0);
	_uint			m_iNowPosIndex = 0;
	_uint			m_iNowLookIndex = 0;
	_double			m_ActionPassedTime = 0;

public:
	static CCamera_Editor* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg)override;
	virtual void Free()override;
};

END