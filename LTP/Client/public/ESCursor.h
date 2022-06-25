#pragma once

#include "GameObject.h"

BEGIN(Client)

class CESCursor final : public CGameObject
{
private:
	CESCursor(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CESCursor(const CESCursor& rhs);
	virtual ~CESCursor() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

public:
	virtual _int Update(_double fDeltaTime)override;
	virtual _int LateUpdate(_double fDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;

public:
	void Set_Color(_float4 vColor) { m_vColor = vColor; };
	void Set_Position(_float3 vPosition) ;

private:
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CVIBuffer_Cube*		m_pVIBufferCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;

	_float4				m_vColor = _float4(1, 0, 0, 1);
private:
	HRESULT SetUp_Components();

public:
	static CESCursor* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext,void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END