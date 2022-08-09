#pragma once

#include "TriggerObject.h"

BEGIN(Client)
class CPlayer;

class CFogTrigger final : public CTriggerObject
{
public:
	typedef struct tagFogTriggerDesc
	{
		_float3			fMain_Pos;
		_float3			fSub_Pos;

		_float			fMain_FogDensity;
		_float			fSub_FogDensity;
	}FOGTRIGGERDESC;


protected:
	CFogTrigger(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CFogTrigger(const CFogTrigger& rhs);
	virtual ~CFogTrigger() = default;

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
	HRESULT							SetUp_PlayerAndCameraInfo();
	HRESULT							SetUp_EtcInfo();

private:
	FOGTRIGGERDESC			m_tFogTriggerDesc;

	CPlayer*				m_pPlayer = nullptr;
	CCamera_Main*			m_pMainCamera = nullptr;
	CTransform*				m_pPlayerTransform = nullptr;
	_float					m_fDist_MainToSub = 0.f;

public:
	static CFogTrigger*						Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject*					Clone(void* pArg);
	virtual void							Free() override;
};

END