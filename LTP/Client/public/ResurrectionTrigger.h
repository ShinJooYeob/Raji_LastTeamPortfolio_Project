#pragma once

#include "TriggerObject.h"

BEGIN(Client)
class CPlayer;

class CResurrectionTrigger final : public CTriggerObject
{
protected:
	CResurrectionTrigger(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CResurrectionTrigger(const CResurrectionTrigger& rhs);
	virtual ~CResurrectionTrigger() = default;

public:
	virtual HRESULT		Initialize_Prototype(void* pArg) override;
	virtual HRESULT		Initialize_Clone(void* pArg) override;
	virtual HRESULT		After_Initialize() override;


public:
	virtual _int					Update(_double fDeltaTime) override;
	virtual _int					LateUpdate(_double fDeltaTimer) override;
	virtual _int					Render() override;
	virtual _int					LateRender() override;

	virtual _int					Active_Trigger(CGameObject* pTarget, _double fDeltaTime) override;
	virtual _int					DeActive_Trigger(CGameObject* pTarget, _double fDeltaTime) override;

	virtual EParkourTriggerType 	Get_ParkourTriggerType();

	virtual void					CollisionTriger(class CCollider* pMyCollider, _uint iMyColliderIndex, CGameObject* pConflictedObj, class CCollider* pConflictedCollider,
		_uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType) override;

private:
	HRESULT							SetUp_Components();
	HRESULT							SetUp_EtcInfo();

private:
	_float3						m_fResurrectionPos = _float3(3.f, 3.f, 3.f);
	_uint						m_iNavIndex = 0;


public:
	static CResurrectionTrigger*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject*					Clone(void* pArg);
	virtual void							Free() override;
};

END