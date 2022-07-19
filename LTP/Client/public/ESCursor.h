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


	_uint Get_eObjectID() { return m_eObjectID; }
	void Set_eObjectID(_uint eID) { m_eObjectID = eID; }


private:
	CShader*			m_pShaderCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CVIBuffer_Cube*		m_pVIBufferCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;

	_float4				m_vColor = _float4(1, 0, 0, 1);
	_uint				m_eObjectID = Prototype_Object_Monster_Mahinasura_Minion;
private:
	HRESULT SetUp_Components();

public:
	static CESCursor* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext,void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END