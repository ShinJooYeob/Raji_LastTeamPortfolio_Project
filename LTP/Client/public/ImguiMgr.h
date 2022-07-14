#pragma once

#ifdef _DEBUG

#include "Base.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"


BEGIN(Client)

#define IMGUITREE(X) if (ImGui::TreeNode(X))
#define IMGUITREE_END ImGui::TreePop();

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

public: /*For Debug*/
	_int	Update_DebugWnd(_double fDeltaTime);
	_int	Update_DebugWnd_PhysX(_double fDeltaTime);
	_int	LateUpdate_DebugWnd(_double fDeltaTime);
	_int	Render_DebugWnd();


	
private:
	ID3D11Device*				m_pDevice = nullptr;
	ID3D11DeviceContext*		m_pDeviceContext = nullptr;
	ID3D11RenderTargetView*		m_pBackBufferRTV = nullptr;
	ID3D11DepthStencilView*		m_pDepthStencilView = nullptr;
	IDXGISwapChain*				m_pSwapChain = nullptr;


private:
	_bool						m_bOnDebugWnd = false;
	_bool						bArrWindowFlag[10] = { 0 };

	_ulong				m_dwNumRender = 0;
	_double				m_dTimerAcc = 0.0f;

private:
	void Make_VerticalSpacing(_uint count);


public:
	virtual void Free()override;

};

END	

#endif // DEBUG