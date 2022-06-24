#pragma once

#include "Base.h"

BEGIN(Engine)

class CRenderTargetMgr final : public CBase
{
	DECLARE_SINGLETON(CRenderTargetMgr)
private:
	CRenderTargetMgr();
	virtual ~CRenderTargetMgr() = default;

public:
	ID3D11ShaderResourceView* Get_SRV(const _tchar* pTargetTag) const;

public:
	HRESULT Initialize_RenderTargetMgr(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	
	HRESULT Add_RenderTarget(const _tchar* pRenderTargetTag, _uint iSizeX, _uint iSizeY, DXGI_FORMAT eFormat, _float4 vClearColor);
	HRESULT Add_MRT(const _tchar* pMRTTag, const _tchar* pRenderTargetTag);

	HRESULT Begin(const _tchar* pMRTTag);
	HRESULT Begin_WithBackBuffer(const _tchar* pMRTTag);
	HRESULT Begin(const _tchar* pMRTTag, ID3D11DepthStencilView* DepthStencil);
	HRESULT End(const _tchar* pMRTTag);

	HRESULT Clear_All_RenderTargetColor();

#ifdef _DEBUG
public:
	HRESULT Ready_DebugDesc(const _tchar* pTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY);
	HRESULT Render_DebugBuffer(const _tchar* pMRTTag);
#endif // _DEBUG


private:
	map<const _tchar*, class CRenderTargetLayer*>			m_mapRenderTarget;
	typedef map<const _tchar*, class CRenderTargetLayer*>	RENDERTARGETS;

private:
	map<const _tchar*, list<class CRenderTargetLayer*>>		m_mapMRTGroup;
	typedef map<const _tchar*, class CRenderTargetLayer*>	MRTs;

private:
	ID3D11Device*					m_pDevice = nullptr;
	ID3D11DeviceContext*			m_pDeviceContext = nullptr;

	ID3D11RenderTargetView*			m_pOldRTV = nullptr;
	ID3D11DepthStencilView*			m_pOriginalDSV = nullptr;

#ifdef _DEBUG
private:
	class CVIBuffer_Rect*					m_pVIBuffer = nullptr;
	class CShader*							m_pShader = nullptr;
#endif // _DEBUG


public:
	class CRenderTargetLayer* Find_RenderTarget(const _tchar* pRenderTargetTag);
	list<class CRenderTargetLayer*>* Find_MRT(const _tchar* pMRTTag);

public:
	virtual void Free() override;
};

END