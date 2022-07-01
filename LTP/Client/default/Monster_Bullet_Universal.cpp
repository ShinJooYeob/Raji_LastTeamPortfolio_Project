#include "stdafx.h"
#include "Monster_Bullet_Universal.h"


Monster_Bullet_Universal::Monster_Bullet_Universal(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CMonster_Bullet(pDevice, pDeviceContext)
{
}

Monster_Bullet_Universal::Monster_Bullet_Universal(const Monster_Bullet_Universal & rhs)
	: CMonster_Bullet(rhs)
{
}

HRESULT Monster_Bullet_Universal::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));


	return S_OK;
}

HRESULT Monster_Bullet_Universal::Initialize_Clone(void * pArg)
{
	return E_NOTIMPL;
}

_int Monster_Bullet_Universal::Update(_double dDeltaTime)
{
	return _int();
}

_int Monster_Bullet_Universal::LateUpdate(_double dDeltaTime)
{
	return _int();
}

_int Monster_Bullet_Universal::Render()
{
	return _int();
}

_int Monster_Bullet_Universal::LateRender()
{
	return _int();
}

HRESULT Monster_Bullet_Universal::SetUp_Components()
{
	return E_NOTIMPL;
}

Monster_Bullet_Universal * Monster_Bullet_Universal::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	return nullptr;
}

CGameObject * Monster_Bullet_Universal::Clone(void * pArg)
{
	return nullptr;
}

void Monster_Bullet_Universal::Free()
{
}
