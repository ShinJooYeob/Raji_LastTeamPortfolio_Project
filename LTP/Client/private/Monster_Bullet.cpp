#include "stdafx.h"
#include "..\public\Monster_Bullet.h"


CMonster_Bullet::CMonster_Bullet(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CMonster_Bullet::CMonster_Bullet(const CMonster_Bullet & rhs)
	: CGameObject(rhs)
{
}

HRESULT CMonster_Bullet::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CMonster_Bullet::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	return S_OK;
}

_int CMonster_Bullet::Update(_double dDeltaTime)
{
	if (__super::Update(dDeltaTime) < 0)
		return -1;

	return _int();
}

_int CMonster_Bullet::LateUpdate(_double dDeltaTime)
{
	if (__super::LateUpdate(dDeltaTime) < 0)
		return -1;

	return _int();
}

_int CMonster_Bullet::Render()
{
	if (__super::Render() < 0)
		return -1;

	return _int();
}

_int CMonster_Bullet::LateRender()
{
	if (__super::LateRender() < 0)
		return -1;

	return _int();
}

void CMonster_Bullet::Free()
{
	__super::Free();
}
