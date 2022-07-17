#include "stdafx.h"
#include "..\public\MonsterWeapon.h"

CMonsterWeapon::CMonsterWeapon(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CMonsterWeapon::CMonsterWeapon(const CMonsterWeapon & rhs)
	: CGameObject(rhs)
{
}
HRESULT CMonsterWeapon::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));


	return S_OK;
}

HRESULT CMonsterWeapon::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	return S_OK;
}

_int CMonsterWeapon::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)return -1;

	return _int();
}

_int CMonsterWeapon::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)return -1;

	return _int();
}

_int CMonsterWeapon::Render()
{
	if (__super::Render() < 0)		return -1;

	return _int();
}

_int CMonsterWeapon::LateRender()
{
	if (__super::LateRender() < 0)		return -1;
	return _int();
}

void CMonsterWeapon::CollisionTriger(_uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
}

void CMonsterWeapon::Free()
{
	__super::Free();
}
