#pragma once

#include "Component.h"


BEGIN(Engine)

class CGameObject;

class ENGINE_DLL CRenderer final :public CComponent
{
public:
	enum RENDERGROUP 
	{
		RENDER_PRIORITY, RENDER_NONBLEND, RENDER_SUBNONBLEND, RENDER_NONBLEND_NOLIGHT, 
		RENDER_BLEND, RENDER_SUBBLEND, RENDER_AFTERALLOBJ, RENDER_UI, RENDER_END
	};
	enum SHADOWGROUP
	{
		SHADOW_ANIMMODEL, SHADOW_ANIMMODEL_ATTACHED, SHADOW_NONANIMMODEL, SHADOW_NONANIMMODEL_ATTACHED, SHADOW_TERRAIN, SHADOW_END
	};

private:
	typedef struct tagShdowRenderDesc
	{
		CGameObject* pGameObject = nullptr;
		class CTransform* pTransform = nullptr;
		class CShader* pShader = nullptr;
		class CModel* pModel = nullptr;
		_float4x4 AttacehdMatrix = XMMatrixIdentity();
	}SHADOWDESC;

private:
	explicit CRenderer(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	virtual ~CRenderer() = default;

private:
	virtual HRESULT Initialize_Prototype(void * pArg)override;
	virtual HRESULT Initialize_Clone(void * pArg)override;


public:
	HRESULT Add_RenderGroup(RENDERGROUP eRenderID, CGameObject* pGameObject);
	HRESULT Add_ShadowGroup(SHADOWGROUP eShadowID, CGameObject* pGameObject, CTransform* pTransform, CShader* pShader, class CModel* pModel = nullptr, _float4x4* AttacehdMatrix = nullptr);
	HRESULT Add_DebugGroup(class CComponent* pComponent);
	HRESULT Render_RenderGroup(_double fDeltaTime);
	HRESULT Clear_RenderGroup_forSceneChaging();


private:
	list<CGameObject*>				m_RenderObjectList[RENDER_END];
	typedef list<CGameObject*>		RENDEROBJECTS;

private:
	list<SHADOWDESC>				m_ShadowObjectList[SHADOW_END];
	typedef list<SHADOWDESC>		SHADOWOBJECTS;

private:
	list<class CComponent*>					m_DebugObjectList;
	typedef list<class CComponent*>			DEBUGOBJECT;

private:
	class CRenderTargetMgr*					m_pRenderTargetMgr = nullptr;
	class CLightMgr*						m_pLightMgr = nullptr;
	class CGraphic_Device*					m_pGraphicDevice= nullptr;

	class CVIBuffer_Rect*					m_pVIBuffer = nullptr;
	class CShader*							m_pShader = nullptr;


private:
	MATRIXWVP					m_WVPmat;

	MATRIXWVP						m_LightWVPmat;
	ID3D11DepthStencilView*			m_LightDepthStencil = nullptr;
	ID3D11DepthStencilView*			m_AvgLumiDepthStencil = nullptr;
	ID3D11DepthStencilView*			m_DownScaledDepthStencil[5] = { nullptr };

	_float						m_fDofLength = 10.f;
	_float						m_fTexleSize = 2.f;
	_bool						m_bShadowLightMatBindedChecker = false;

private:
	HRESULT Render_Priority();
	HRESULT Render_NonBlend();
	HRESULT Render_Lights(_double fDeltaTime);

	HRESULT Render_NonBlend_NoLight();
	HRESULT Render_Blend();
	HRESULT Render_AfterObj();

	HRESULT Render_UI();
	
private:
	HRESULT Render_DeferredTexture();
	HRESULT Copy_DeferredToReference();
	HRESULT Copy_DeferredToBackBuffer();
	HRESULT Ready_DepthStencilBuffer(_uint iDSVIndex, D3D11_VIEWPORT* pOutOldViewPort);
	HRESULT Render_BlurShadow();
	HRESULT Caculate_AvgLuminence();
	HRESULT Render_Bloom();
	HRESULT Make_BluredDeffered();

private:
	HRESULT Render_DepthOfField();
	HRESULT Render_ShadowMap();
	HRESULT Render_ShadowGroup();




#ifdef _DEBUG
	HRESULT Render_Debug();
#endif // _DEBUG


public:
	static CRenderer* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void* pArg = nullptr);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END	