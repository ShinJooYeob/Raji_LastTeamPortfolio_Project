#pragma once
#include "GameObject.h"

BEGIN(Client)

class CMiniGame_Jino_Monster abstract : public CGameObject
{
public:
	enum EMONSTER_TYPE { TYPE_WALKER, TYPE_RUNNER, TYPE_END };
	enum EANIM_STATE { ANIM_MOVE, ANIM_JUMP, STATE_END };

protected:
	explicit CMiniGame_Jino_Monster(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CMiniGame_Jino_Monster(const CMiniGame_Jino_Monster& rhs);
	virtual ~CMiniGame_Jino_Monster() = default;

public:
	virtual HRESULT	Initialize_Prototype(void* pArg)override;
	virtual HRESULT	Initialize_Clone(void* pArg)override;

public:
	virtual _int			Update(_double dDeltaTime)override;
	virtual _int			LateUpdate(_double dDeltaTime)override;
	virtual _int			Render()override;
	virtual _int			LateRender()override;

	virtual void			CollisionTriger(class CCollider* pMyCollider, _uint iMyColliderIndex, CGameObject* pConflictedObj, class CCollider* pConflictedCollider,
									_uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType) override;

	virtual _float			Take_Damage(CGameObject* pTargetObject, _float fDamageAmount, _fVector vDamageDir, _bool bKnockback = false, _float fKnockbackPower = 0.f) override;

	virtual void			SetUp_MonsterType(EMONSTER_TYPE eMonsterType) = 0;
	virtual void			Set_Position(_float3 vPos) = 0;

	virtual void			Dead() = 0;

public:
	void					Set_Active(_bool bActive);
	_bool					Get_Active();

private:
	_bool					m_bActive = false;

public:
	virtual CGameObject*				Clone(void* pArg) = 0;
	virtual void						Free() override;

};

END