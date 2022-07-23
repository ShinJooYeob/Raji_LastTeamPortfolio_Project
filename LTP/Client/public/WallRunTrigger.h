#pragma once

#include "TriggerObject.h"

BEGIN(Client)
class CPlayer;

class CWallRunTrigger final : public CTriggerObject
{
protected:
	CWallRunTrigger(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CWallRunTrigger(const CWallRunTrigger& rhs);
	virtual ~CWallRunTrigger() = default;

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
	_float3					m_fStartPos = _float3(0.f);
	_float3					m_fEndPos = _float3(0.f);
	_bool					m_bAnimDir = false;

public:
	static CWallRunTrigger*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject*					Clone(void* pArg);
	virtual void							Free() override;
};

END