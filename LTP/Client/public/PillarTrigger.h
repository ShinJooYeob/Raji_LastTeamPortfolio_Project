#pragma once

#include "TriggerObject.h"

BEGIN(Client)

class CPillarTrigger final : public CTriggerObject
{
public:
	enum EPillarTriggerState { STATE_NORMAL, STATE_TOP, STATE_END };
	
protected:
	CPillarTrigger(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CPillarTrigger(const CPillarTrigger& rhs);
	virtual ~CPillarTrigger() = default;

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


public:
	void							Set_Pos(_float3 fPos);
	void							Set_PillarType(EPillarTriggerState eType);

private:
	HRESULT							SetUp_Components();
	HRESULT							SetUp_EtcInfo();

private:
	_float3						m_fLookDir = _float3(0.f, 0.f, 0.f);
	EPillarTriggerState			m_ePillarTriggerType;

public:
	static CPillarTrigger*					Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject*					Clone(void* pArg);
	virtual void							Free() override;
};

END