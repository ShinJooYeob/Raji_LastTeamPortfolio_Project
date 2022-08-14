#pragma once

#include "UI.h"

BEGIN(Client)

class CRadialBlurUI : public CGameObject
{

protected:
	explicit CRadialBlurUI(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	explicit CRadialBlurUI(const CRadialBlurUI&  rhs);
	virtual ~CRadialBlurUI() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

public:
	virtual _int Update(_double fDeltaTime)override;
	virtual _int LateUpdate(_double fDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;


	void Set_TargetPos_ByWorldPos(_float3 vPos);
	void Set_IsRadialIn(_bool bBool, _float fZoomRadialSize = 0.25f, _float fZoomPower = 0.7f, _float TargetTimer = 1.f);

private:
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CVIBuffer_Rect*		m_pVIBufferCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;

private:
	FLOATRECT			m_UIRect;
	_float4x4		m_ProjMatrix;


private:
	_bool				m_bIsOn = false;

	_bool				m_bNeedToDraw = false;
	_float				m_PassedTime = FLT_MAX;
	_float				m_TotalTime = 0;
	_float2				m_TargetPos = _float2(0.5f, 0.5f);
	_float				m_ZoomSize = 0;
	_float				m_ZoomPower = 0;
	_float4				m_vTargetColor = _float4(0,0,0,0);

	_float				m_fTargetZoomPower = 0.25f;
	_float				m_fStartZoomPower = 0.f;
	_float				m_fZoomRadialSize = 0.7f;
	_float				m_fStartRadialSize = 1.f;
	
protected:
	HRESULT SetUp_Components();
	HRESULT SetUp_UI_DefaultSetting();

public:
	static CRadialBlurUI* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free()override;

};
END

