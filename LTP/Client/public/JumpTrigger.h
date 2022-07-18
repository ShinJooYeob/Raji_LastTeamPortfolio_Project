#pragma once

#include "TriggerObject.h"

BEGIN(Client)

class CJumpTrigger final : public CTriggerObject
{
protected:
	CJumpTrigger(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CJumpTrigger(const CJumpTrigger& rhs);
	virtual ~CJumpTrigger() = default;

public:
	virtual HRESULT		Initialize_Prototype(void* pArg) override;
	virtual HRESULT		Initialize_Clone(void* pArg) override;

public:
	virtual _int				Update(_double fDeltaTime) override;
	virtual _int				LateUpdate(_double fDeltaTimer) override;
	virtual _int				Render() override;
	virtual _int				LateRender() override;

	virtual _int				Active_Trigger(CGameObject* pTarget, _double fDeltaTime) override;
	virtual _int				DeActive_Trigger(CGameObject* pTarget, _double fDeltaTime) override;

	virtual void				CollisionTriger(class CCollider* pMyCollider, _uint iMyColliderIndex, CGameObject* pConflictedObj, class CCollider* pConflictedCollider,
		_uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType) override;

public:
	_float						Get_JumpPower();
	void						Set_JumpPower(_float fJumpPower);

private:
	HRESULT						SetUp_Components();
	HRESULT						SetUp_Collider();
	HRESULT						SetUp_EtcInfo();

private:
	_float				m_fJumpPower = 0.f;

private:
	CTransform*			m_pTransformCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CCollider*			m_pCollider = nullptr;

public:
	static CJumpTrigger*					Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, void* pArg = nullptr);
	virtual CGameObject*					Clone(void* pArg);
	virtual void							Free() override;
};

END