#pragma once

#include "UI.h"

BEGIN(Client)

class CHitEffectUI : public CGameObject
{

protected:
	explicit CHitEffectUI(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	explicit CHitEffectUI(const CHitEffectUI&  rhs);
	virtual ~CHitEffectUI() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

public:
	virtual _int Update(_double fDeltaTime)override;
	virtual _int LateUpdate(_double fDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;

	void	Set_HitEffect(_float Intensive, _float EffectTime = 0.5f, _float3 vColor = _float3(1,0,0));
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

	_float4				m_vTargetColor = _float4(0, 0, 0, 0);
	_float4				m_vColor = _float4(0, 0, 0, 0);
	_float				m_fAlphaTestValue = 0.1f;
	_uint				m_iTextureLayerIndex = 1;
protected:
	HRESULT SetUp_Components();
	HRESULT SetUp_UI_DefaultSetting();

public:
	static CHitEffectUI* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free()override;

};
END

