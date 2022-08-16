#include "stdafx.h"
#include "MiniGame_Jino_Monster.h"
#include "MiniGame_Jino_Player.h"
#include "Scene_MiniGame_Jino.h"

CMiniGame_Jino_Monster::CMiniGame_Jino_Monster(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CMiniGame_Jino_Monster::CMiniGame_Jino_Monster(const CMiniGame_Jino_Monster & rhs)
	: CGameObject(rhs)
{
}

HRESULT CMiniGame_Jino_Monster::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CMiniGame_Jino_Monster::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	return S_OK;
}

_int CMiniGame_Jino_Monster::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0) return -1;

	return _int();
}

_int CMiniGame_Jino_Monster::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)return -1;

	return _int();
}

_int CMiniGame_Jino_Monster::Render()
{
	if (__super::Render() < 0)		return -1;

	return _int();
}

_int CMiniGame_Jino_Monster::LateRender()
{
	return _int();
}

void CMiniGame_Jino_Monster::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
	if (eConflictedObjCollisionType == CollisionType_Player)
	{
		static_cast<CMiniGame_Jino_Player*>(pConflictedObj)->Set_State_DeadStart();
	}
}

_float CMiniGame_Jino_Monster::Take_Damage(CGameObject * pTargetObject, _float fDamageAmount, _fVector vDamageDir, _bool bKnockback, _float fKnockbackPower)
{
	return _float();
}

_bool CMiniGame_Jino_Monster::Get_Active()
{
	return m_bActive;
}

void CMiniGame_Jino_Monster::Set_Active(_bool bActive)
{
	m_bActive = bActive;
}

void CMiniGame_Jino_Monster::Set_Position(_float3 vPos)
{
	
}

void CMiniGame_Jino_Monster::Free()
{
	__super::Free();

}
