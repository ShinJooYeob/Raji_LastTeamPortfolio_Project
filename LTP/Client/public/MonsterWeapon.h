#pragma once
#include "GameObject.h"

BEGIN(Client)
class CMonsterWeapon abstract : public CGameObject
{
protected:
	CMonsterWeapon(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CMonsterWeapon(const CMonsterWeapon& rhs);
	virtual ~CMonsterWeapon() = default;

public:
	virtual HRESULT Initialize_Prototype(void* pArg)override;
	virtual HRESULT Initialize_Clone(void* pArg)override;

public:
	virtual _int Update(_double fDeltaTime)override;
	virtual _int LateUpdate(_double fDeltaTime)override;
	virtual _int Render()override;
	virtual _int LateRender()override;

	virtual void CollisionTriger(class CCollider* pMyCollider, _uint iMyColliderIndex, CGameObject* pConflictedObj, class CCollider* pConflictedCollider,
		_uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType) override;

public:
	virtual	_bool	Get_IsAttackState() { return m_bIsAttack; }

public:
	virtual	void	Set_IsAttackState(_bool State) { m_bIsAttack = State; }

protected:
	_bool		m_bIsHit = false;
	_bool		m_bIsAttack = false;
	_float		m_fAttackCoolTime = 3.f;
	_float		m_fSkillCoolTime = 10.f;
	_float		m_fAnimmultiple = 1.f; // AnimationPlaySpeed
	_float		m_fRange = 0.f; // 오브젝트간의 거리변수

public:
	virtual void Free() override; 

};

END