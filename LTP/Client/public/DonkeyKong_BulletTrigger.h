#pragma once

#include "GameObject.h"

BEGIN(Client)

class CDonkeyKong_BulletTrigger : public CGameObject
{
private:
	explicit CDonkeyKong_BulletTrigger(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CDonkeyKong_BulletTrigger(const CDonkeyKong_BulletTrigger& rhs);
	virtual ~CDonkeyKong_BulletTrigger() = default;

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
	static CDonkeyKong_BulletTrigger* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END