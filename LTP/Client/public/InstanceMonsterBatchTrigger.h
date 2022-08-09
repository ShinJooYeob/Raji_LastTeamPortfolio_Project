#pragma once

#include "TriggerObject.h"

BEGIN(Client)
class CPlayer;

class CInstanceMonsterBatchTrigger final : public CTriggerObject
{
	enum MeshInstanceType{WASP, WORMGRUB,SPIDER,WOLF,INSTACNE_END};
	typedef struct tagInstanceBatchInfo
	{
		_float4x4 fValueMat;
		_float4x4 fSubValueMat;
		void*	Object;
	}INSTANCE_BATCH_INFO;
protected:
	CInstanceMonsterBatchTrigger(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CInstanceMonsterBatchTrigger(const CInstanceMonsterBatchTrigger& rhs);
	virtual ~CInstanceMonsterBatchTrigger() = default;

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
	_bool							Get_MonsterAllDie() {return m_bMonsterAllDie;}
	void							Set_MonsterAllDie(_bool bMonsterAllDie) { m_bMonsterAllDie = bMonsterAllDie; }

private:
	HRESULT							SetUp_Components();
	HRESULT							SetUp_EtcInfo();

	INSTANCE_BATCH_INFO				m_Instance_Batch_InfoDesc;
	_bool							m_bOnceSwitch = false;

	CTransform*						m_pPlayerTransform = nullptr;
	_bool							m_bMonsterAllDie = false;

	_float3							m_fPoint;
	
public:
	static CInstanceMonsterBatchTrigger*						Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject*					Clone(void* pArg);
	virtual void							Free() override;
};

END