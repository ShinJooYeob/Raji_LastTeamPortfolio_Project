#pragma once

#include "GameObject.h"


BEGIN(Client)

class CCircusBackground :public CGameObject
{
private:
	CCircusBackground(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CCircusBackground(const CCircusBackground& rhs);
	virtual ~CCircusBackground() = default;


public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;


public:
	virtual _int Update(_double fDeltaTime)override;
	virtual _int LateUpdate(_double fDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;

public:
	void Set_Active(_bool bActive);
	_bool Get_IsActive();

	_float3 Get_AttackPoint();
	
private:
	CShader*						m_pShaderCom = nullptr;
	CRenderer*						m_pRendererCom = nullptr;
	CTransform*						m_pTransformCom = nullptr;
	CTexture*						m_pTextureCom = nullptr;
	CVIBuffer_Rect*					m_pVIBufferCom = nullptr;

private:
	HRESULT SetUp_Components();
	HRESULT SetUp_Etc();

public:
	static CCircusBackground* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};


END