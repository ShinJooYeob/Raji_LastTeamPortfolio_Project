#pragma once

#include "TriggerObject.h"

BEGIN(Client)
class CPlayer;
class CCamera_Main;
class CEventTrigger final : public CTriggerObject
{
public:
	enum EEventType { TYPE_RANGDA_CUTSCENE, TYPE_END };


protected:
	CEventTrigger(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CEventTrigger(const CEventTrigger& rhs);
	virtual ~CEventTrigger() = default;

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
	void							EVENT_Rangda_Cutscene();

private:
	HRESULT							SetUp_Components();
	HRESULT							SetUp_PlayerAndCameraInfo();
	HRESULT							SetUp_EtcInfo();

private:
	EEventType				m_eEventType = TYPE_END;
	_bool					m_bActive = false;

	CPlayer*				m_pPlayer = nullptr;
	CCamera_Main*			m_pMainCamera = nullptr;
	CTransform*				m_pPlayerTransform = nullptr;
	
public:
	static CEventTrigger*					Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject*					Clone(void* pArg);
	virtual void							Free() override;
};

END