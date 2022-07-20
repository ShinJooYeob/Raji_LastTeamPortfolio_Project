#pragma once

#include "Component.h"


BEGIN(Engine)

class CGameObject;

class ENGINE_DLL CRenderer final :public CComponent
{
public:
	enum RENDERGROUP 
	{
		RENDER_PRIORITY, RENDER_NONBLEND, RENDER_SUBNONBLEND, RENDER_BLEND, RENDER_SUBBLEND, 
		RENDER_AFTERALLOBJ, RENDER_DISTORTION, RENDER_EFFECT, RENDER_NONBLEND_NOLIGHT,  RENDER_UI, RENDER_END
	};
	enum SHADOWGROUP
	{
		SHADOW_ANIMMODEL, SHADOW_ANIMMODEL_ATTACHED, SHADOW_NONANIMMODEL, SHADOW_NONANIMMODEL_ATTACHED, SHADOW_TERRAIN, SHADOW_END
	};
	enum INSTANCESHADOWGROUP
	{
		INSTSHADOW_ANIMINSTANCE, INSTSHADOW_NONANIMINSTANCE, INSTSHADOW_END
	};
	enum TRAILGROUP
	{
		TRAIL_MOTION, TRAIL_SWORD, TRAIL_SWORD_DISTORT, TRAIL_END
	};

private:
	typedef struct tagShdowRenderDesc
	{
		CGameObject* pGameObject = nullptr;
		class CTransform* pTransform = nullptr;
		class CShader* pShader = nullptr;
		class CModel* pModel = nullptr;
		class CDissolve* pDissolve = nullptr;
		_float fIsOcllusion = 1.f;
		_float4x4 AttacehdMatrix = XMMatrixIdentity();
	}SHADOWDESC;

	typedef struct tagInstanceShdowRenderDesc
	{
		CGameObject* pGameObject = nullptr;
		vector<class CTransform*>* pvecTransform = nullptr;
		vector<_float4x4>*		   pvecTransformfloat4x4 = nullptr;
		vector<_float4>*		   pvecTimer = nullptr;
		class CShader* pShader = nullptr;
		class CModel* pModel = nullptr;
		class CModelInstance* pModelInstance = nullptr;
		class CDissolve* pDissolve = nullptr;
		_float fIsOcllusion = 1.f;
	}INSTSHADOWDESC;


private:
	explicit CRenderer(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	virtual ~CRenderer() = default;

private:
	virtual HRESULT Initialize_Prototype(void * pArg)override;
	virtual HRESULT Initialize_Clone(void * pArg)override;


public:
	HRESULT Add_RenderGroup(RENDERGROUP eRenderID, CGameObject* pGameObject);
	HRESULT Add_ShadowGroup(SHADOWGROUP eShadowID, CGameObject* pGameObject, CTransform* pTransform, CShader* pShader, 
		class CModel* pModel = nullptr, _float4x4* AttacehdMatrix = nullptr, class CDissolve* pDissolve = nullptr);
	HRESULT Add_ShadowGroup_InstanceModel(INSTANCESHADOWGROUP eShadowID, CGameObject* pGameObject, vector<CTransform*>* pvecTransform, 
		CModelInstance* pModelInst ,CShader* pShader, class CModel* pModel, vector<_float4x4>* pvecTransformfloat4x4 = nullptr, vector<_float4>* pvecTimer = nullptr);
	HRESULT Add_TrailGroup(TRAILGROUP eTrailID, CComponent* pTrailComponent);
	HRESULT Add_DebugGroup(class CComponent* pComponent);
	HRESULT Render_RenderGroup(_double fDeltaTime);
	HRESULT Clear_RenderGroup_forSceneChaging();



private:
	list<CGameObject*>					m_RenderObjectList[RENDER_END];
	typedef list<CGameObject*>			RENDEROBJECTS;

	list<SHADOWDESC>					m_ShadowObjectList[SHADOW_END];
	typedef list<SHADOWDESC>			SHADOWOBJECTS;

	list<INSTSHADOWDESC>				m_InstanceShadowList[INSTSHADOW_END];
	typedef list<INSTSHADOWDESC>		SHADOWINSTOBJECTS;


	list<class CComponent*>					m_DebugObjectList;
	typedef list<class CComponent*>			DEBUGOBJECT;

	list<class CComponent*>					m_TrailList[TRAIL_END];
	typedef list<class CComponent*>			DEBUGOBJECT;


private:
	vector<wstring>					m_szDebugRenderTargetList;
	typedef vector<wstring>			DEBUGRENDERTARGET;

private:
	class CRenderTargetMgr*					m_pRenderTargetMgr = nullptr;
	class CLightMgr*						m_pLightMgr = nullptr;
	class CGraphic_Device*					m_pGraphicDevice= nullptr;

	class CVIBuffer_Rect*					m_pVIBuffer = nullptr;
	class CShader*							m_pShader = nullptr;
	class CTexture*							m_pTexture = nullptr;


private:
	MATRIXWVP					m_WVPmat;

	MATRIXWVP						m_LightWVPmat;
	ID3D11DepthStencilView*			m_LightDepthStencil = nullptr;
	ID3D11DepthStencilView*			m_AvgLumiDepthStencil = nullptr;
	ID3D11DepthStencilView*			m_DownScaledDepthStencil[5] = { nullptr };

	_float						m_fDofLength = 30.f;
	_float						m_fOverLuminece = 2.0f;
	_float						m_fBloomBrightnessMul = 1.5f;
	_float						m_fTexleSize = 2.f;
	_bool						m_bShadowLightMatBindedChecker = false;

	_float						m_fShadowIntensive = 0.5f;

	//_float3						m_vGodRayColor = _float3(0.71875f, 0.83984375f, 0.390625f);
	_float3						m_vGodRayColor = _float3(0.3215686274509804f, 0.3254901960784314f, 0.2823529411764706f);
	_float						m_fGodrayLength = 64.f;
	_float						m_fGodrayIntensity = 0.2f;
	_float						m_fInitDecay = 0.2f;
	_float						m_fDistDecay = 1.2f;
	_float						m_fMaxDeltaLen = 0.005f;

	_float3						m_vFogColor = _float3(0.5f, 0.5f, 0.5f);
	_float3						m_vFogHighlightColor = _float3(0.8f, 0.7f, 0.4f);
	_float						m_fFogStartDist = 5.0f;
	_float						m_fFogGlobalDensity = 0.02f;
	_float						m_fFogHeightFalloff = 0.1f;
	//_float						m_fFogStartDist = 37.0f;
	//_float						m_fFogGlobalDensity = 1.5f;
	//_float						m_fFogHeightFalloff = 0.2f;

	_float3						m_vSunAtPoint = _float3(32, -10, 32);


	_float4						m_vSunPosSS = _float4(0);


	_float						m_SunSize  = 10.f;
	_float						m_LensfalreSupportSunSize = 128.f;
	_uint						m_iLensefalreNoiseTexIndex = 352;


	_bool						m_PostProcessingOn[POSTPROCESSING_END];

private:
	HRESULT Render_Priority();
	HRESULT Render_NonBlend();
	HRESULT Render_Lights(_double fDeltaTime);

	HRESULT Render_NonBlend_NoLight();
	HRESULT Render_Blend();
	HRESULT Render_AfterObj();
	HRESULT Render_DistortionObject();
	HRESULT Render_EffectObj();
	
	HRESULT Render_UI();
	
private:
	HRESULT Render_DeferredTexture();
	HRESULT Copy_DeferredToReference();
	HRESULT Copy_DeferredToBackBuffer();
	HRESULT Ready_DepthStencilBuffer(_uint iDSVIndex, D3D11_VIEWPORT* pOutOldViewPort);
	HRESULT Render_BlurShadow();
	HRESULT Caculate_AvgLuminence();
	HRESULT Render_Bloom();
	HRESULT Make_BluredDeffered(_float TexelSize = 2.f);
	HRESULT Render_DDFog();
	HRESULT Render_GodRay();
	HRESULT Render_LesnFlare();

	HRESULT Render_MotionTrail();
	HRESULT Render_SwordTrail();
	HRESULT Render_EmissiveBlur();

private:
	HRESULT Render_DepthOfField();
	HRESULT Render_ShadowMap();
	HRESULT Render_ShadowGroup();

	HRESULT Add_DebugRenderTarget(const _tchar* szTargetTag, _float fX, _float fY, _float fCX, _float fCY);
	HRESULT Ready_For_Update(_double fDelataTimme);

public:
	void OnOff_PostPorcessing(POSTPROCESSINGID eID) { m_PostProcessingOn[eID] = !m_PostProcessingOn[eID]; };
	void OnOff_PostPorcessing_byParameter(POSTPROCESSINGID eID, _bool bBool) { m_PostProcessingOn[eID] = bBool; };
	_bool Get_IsOnPostPorcessing(POSTPROCESSINGID eID) { return m_PostProcessingOn[eID]; };

	_float Get_DofLength() { return m_fDofLength; };
	void	Set_DofLength(_float vDofValue) { m_fDofLength = vDofValue; };
	_float Get_ShadowIntensive() { return m_fShadowIntensive; };
	void	Set_ShadowIntensive(_float vShadowIntensive) { m_fShadowIntensive = vShadowIntensive; };
	_float Get_BloomOverLuminceValue() { return m_fOverLuminece * 0.5f; };
	void	Set_BloomOverLuminceValue(_float vBloomOverLuminceValue) { m_fOverLuminece = vBloomOverLuminceValue * 2.f; };

	_float Get_BloomBrightnessMul() { return m_fBloomBrightnessMul; };
	void	Set_BloomBrightnessMul(_float fValue) { m_fBloomBrightnessMul = fValue; };
	

	_float Get_GodrayLength() { return m_fGodrayLength; };
	void  Set_GodrayLength(_float fValue) {	m_fGodrayLength = fValue;	};
	_float Get_GodrayIntensity() { return m_fGodrayIntensity; };
	void  Set_GodrayIntensity(_float fValue) { m_fGodrayIntensity = fValue; };
	_float Get_StartDecay() { return m_fInitDecay; };
	void  Set_StartDecay(_float fValue) { m_fInitDecay = fValue; };
	_float Get_DistDecay() { return m_fDistDecay; };
	void  Set_DistDecay(_float fValue) { m_fDistDecay = fValue; };
	_float Get_MaxDeltaLen() { return m_fMaxDeltaLen; };
	void  Set_MaxDeltaLen(_float fValue) { m_fMaxDeltaLen = fValue; };

	_float3 Get_FogColor() { return m_vFogColor; };
	void  Set_FogColor(_float3 vValue) { m_vFogColor = vValue; };
	_float Get_FogStartDist() { return m_fFogStartDist; };
	void  Set_FogStartDist(_float fValue) { m_fFogStartDist = fValue; };
	_float3 Get_FogHighlightColor() { return m_vFogHighlightColor; };
	void  Set_FogHighlightColor(_float3 vValue) { m_vFogHighlightColor = vValue; };
	_float Get_FogGlobalDensity() { return m_fFogGlobalDensity; };
	void  Set_FogGlobalDensity(_float fValue) { m_fFogGlobalDensity = fValue; };
	_float Get_FogHeightFalloff() { return m_fFogHeightFalloff; };
	void  Set_FogHeightFalloff(_float fValue) { m_fFogHeightFalloff = fValue; };

	_uint Get_DebugRenderTargetSize() { return _uint(m_szDebugRenderTargetList.size()); };
	const _tchar* Get_DebugRenderTargetTag(_uint iIndex);

	_float3 Get_SunAtPoint() { return  m_vSunAtPoint; }
	void	Set_SunAtPoint(_float3 vVector) { m_vSunAtPoint = vVector; }

	_float3		Get_GodRayColor() { return m_vGodRayColor; }
	void		Set_GodRayColor(_float3 vVector) { m_vGodRayColor = vVector; }



	_float		Get_SunSize() { return m_SunSize; }
	void		Set_SunSize(_float vVector) { m_SunSize = vVector; }
	_float		Get_LensfalreSupportSunSize() { return m_LensfalreSupportSunSize; }
	void		Set_LensfalreSupportSunSize(_float vVector) { m_LensfalreSupportSunSize = vVector; }
	_uint		Get_LensefalreNoiseTexIndex() { return m_iLensefalreNoiseTexIndex; }
	void		Set_LensefalreNoiseTexIndex(_uint vVector) { m_iLensefalreNoiseTexIndex = vVector; }


#ifdef _DEBUG
	HRESULT Render_Debug();
#endif // _DEBUG


public:
	static CRenderer* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void* pArg = nullptr);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END	