#pragma once
#include "GameObject.h"

BEGIN(Client)

class CEnvMappedWater final : public CGameObject
{
public:
	typedef struct tagEnvMappedWaterDesc
	{
		_float3 vPosition = _float3(0,0,0); 
		_float4 vRotAxis_N_Angle = _float4(1, 0, 0, 0);
		_float3 vScale = _float4(1.f, 1.f, 1.f, 0);
		_float4 vEmissive = _float4(1.f, 0.2f, 1.f, 0);
		_float2 vNoisePushDir = _float2(0.f, 1.f);

		_float4 vMixColor = _float4(1, 1, 1, 1);
		_float	fFlowRate = 0.1f;
	}EMWDESC;

private:
	explicit CEnvMappedWater(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CEnvMappedWater(const CEnvMappedWater& rhs);
	virtual ~CEnvMappedWater() = default;

public:
	virtual HRESULT		Initialize_Prototype(void* pArg) override;
	virtual HRESULT		Initialize_Clone(void* pArg) override;

public:
	virtual _int		Update(_double fDeltaTime) override;
	virtual _int		LateUpdate(_double fDeltaTimer) override;
	virtual _int		Render() override;
	virtual _int		LateRender() override;

private:
	CShader*				m_pShaderCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CVIBuffer_Rect*			m_pVIBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;

private:
	EMWDESC					m_tEmwDesc;
	_float					m_fPassedTime = 0;

private:
	HRESULT				SetUp_Components();

public:
	static CEnvMappedWater*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject*			Clone(void* pArg);
	virtual void					Free() override;
};
END
