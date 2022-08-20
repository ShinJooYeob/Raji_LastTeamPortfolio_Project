#pragma once

#include "GameObject.h"

BEGIN(Client)

class CKongRajiTrigger : public CGameObject
{
private:
	explicit CKongRajiTrigger(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CKongRajiTrigger(const CKongRajiTrigger& rhs);
	virtual ~CKongRajiTrigger() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

public:
	virtual _int Update(_double dDeltaTime)override;
	virtual _int LateUpdate(_double dDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;
private:
	HRESULT SetUp_Components();
	HRESULT SetUp_Collider();
	HRESULT	SetUp_Info();

private:
	HRESULT	Update_Collider(_double dDeltaTime);

private:
	CRenderer*			m_pRendererCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CCollider*			m_pColliderCom = nullptr;


public:
	static CKongRajiTrigger* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END