#pragma once

#ifdef USE_IMGUI

#include "Base.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"


BEGIN(Client)

class CImguiMgr :public CBase
{
	DECLARE_SINGLETON(CImguiMgr)

private:
	explicit CImguiMgr();
	virtual ~CImguiMgr() = default;

public:
	HRESULT Initialize_ImguiMgr(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, ID3D11RenderTargetView* pBackBufferRTV, ID3D11DepthStencilView* pDepthStencilView, IDXGISwapChain* pSwapChain);
	_int	Update_ImguiMgr(_double fDeltatime);
	_int	Begin_Update_Frame(_double fDeltatime, const char* szManuBarName = "ImguiBar", _uint iFlag = 0);
	_int	End_Update_Frame();
	HRESULT	Render_ImguiMgr();

	
private:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDeviceContext = nullptr;
	// ID3D11ShaderResourceView*
	ID3D11RenderTargetView*		m_pBackBufferRTV = nullptr;
	ID3D11DepthStencilView*		m_pDepthStencilView = nullptr;
	IDXGISwapChain*				m_pSwapChain = nullptr;


public:
	virtual void Free()override;

};

END	

#endif // DEBUG