#pragma once

#include "Base.h"

BEGIN(Engine)

class CRenderTargetLayer final : public CBase
{
private:
	CRenderTargetLayer(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CRenderTargetLayer() = default;

public:
	ID3D11RenderTargetView* Get_RTV() {
		return m_pRTV;
	}

	ID3D11ShaderResourceView* Get_SRV() {
		return m_pSRV;
	}

public:
	HRESULT Initialize(_uint iSizeX, _uint iSizeY, DXGI_FORMAT eFormat, _float4 vClearColor, _bool bAutoClearing);
	HRESULT Clear();
	_bool Get_IsAutoClearing() { return m_bAutoClearing; };

#ifdef _DEBUG
public:
	HRESULT Ready_DebugDesc(_float fX, _float fY, _float fSizeX, _float fSizeY);
	HRESULT Render_DebugBuffer(class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);
#endif // _DEBUG

private:
	ID3D11Device*					m_pDevice = nullptr;
	ID3D11DeviceContext*			m_pDeviceContext = nullptr;
	ID3D11Texture2D*				m_pTexture2D = nullptr;
	ID3D11RenderTargetView*			m_pRTV = nullptr;
	ID3D11ShaderResourceView*		m_pSRV = nullptr;

private:
	_float4					m_vClearColor;
	_bool					m_bAutoClearing = true;

#ifdef _DEBUG
private:
	_float4x4				m_WorldMatrix;
	_float4x4				m_ViewMatrix;
	_float4x4				m_ProjMatrix;
#endif // _DEBUG

public:
	static CRenderTargetLayer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _uint iSizeX, _uint iSizeY, DXGI_FORMAT eFormat, _float4 vClearColor ,_bool bAutoClearing);
	virtual void Free() override;
};

END