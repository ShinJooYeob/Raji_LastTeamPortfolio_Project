#pragma once

#include "TriggerObject.h"

BEGIN(Client)

class CGoldfishTrigger final : public CTriggerObject
{
	typedef struct tagGoldfishInfo
	{
		_float4x4 fValueMat;  //m[0][0] : MeshInstanceMonsterEnum, m[0][1] : Monster Max Size, m[0][2] : Cell Max Size, m[0][3] : Cell Count,
							  //m[1][0] : Life Count, m[1][1] : Monster Hit Count, m[1][2] : TriggerDistance,
							  //m[2][0]~m[2][2] : X,Y,Z
		_float4x4 fSubValueMat;
		void*		Object = nullptr;
	}GOLDFISH_INFO;
protected:
	CGoldfishTrigger(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CGoldfishTrigger(const CGoldfishTrigger& rhs);
	virtual ~CGoldfishTrigger() = default;

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
	_bool							Get_MonsterAllDie() { return m_bMonsterAllDie; }
	void							Set_MonsterAllDie(_bool bMonsterAllDie);

private:
	HRESULT							SetUp_Components();
	HRESULT							SetUp_EtcInfo();

	GOLDFISH_INFO				m_Batch_InfoDesc;
	_bool							m_bOnceSwitch = false;

	CTransform*						m_pPlayerTransform = nullptr;
	_bool							m_bMonsterAllDie = false;
	_bool							m_bAllDieOn = false;

	_float3							m_fPoint;

public:
	static CGoldfishTrigger*						Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject*					Clone(void* pArg);
	virtual void							Free() override;
};

END