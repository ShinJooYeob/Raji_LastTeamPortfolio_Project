#include "stdafx.h"
#include "..\public\PlayerWeapon.h"


CPlayerWeapon::CPlayerWeapon(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CPlayerWeapon::CPlayerWeapon(const CPlayerWeapon & rhs)
	: CGameObject(rhs)
{
}

HRESULT CPlayerWeapon::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CPlayerWeapon::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	ZeroMemory(&m_tPlayerWeaponDesc, sizeof(PlayerWeaponDesc));
	if (nullptr != pArg)
	{
		memcpy(&m_tPlayerWeaponDesc, pArg, sizeof(PlayerWeaponDesc));
	}
	return S_OK;
}

_int CPlayerWeapon::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0) return -1;

	return _int();
}

_int CPlayerWeapon::LateUpdate(_double fDeltaTimer)
{
	if (__super::LateUpdate(fDeltaTimer) < 0)return -1;

	return _int();
}

_int CPlayerWeapon::Render()
{
	if (__super::Render() < 0)		return -1;

	return _int();
}

_int CPlayerWeapon::LateRender()
{
	return _int();
}

void CPlayerWeapon::Dissolve_In(_double fTargetTime)
{
	
}

void CPlayerWeapon::Dissolve_Out(_double fTargetTime)
{
	
}

void CPlayerWeapon::Active_Collision()
{
	m_bActiveCollision = true;
}

void CPlayerWeapon::DeActive_Collision()
{
	m_bActiveCollision = false;
}

void CPlayerWeapon::Active_Collision_1()
{
	m_bActiveCollision_1 = true;
}

void CPlayerWeapon::DeActive_Collision_1()
{
	m_bActiveCollision_1 = false;
}

void CPlayerWeapon::Active_Collision_2()
{
	m_bActiveCollision_2 = true;
}

void CPlayerWeapon::DeActive_Collision_2()
{
	m_bActiveCollision_2 = false;
}

void CPlayerWeapon::Active_Trail(_bool bActivate)
{
	m_bActiveTrail = bActivate;
}

void CPlayerWeapon::Set_BlockUpdate(_bool bBlock)
{
	m_bBlockUpdate = bBlock;
}

_bool CPlayerWeapon::AbleToChangeWeapon()
{
	return _bool();
}

_fVector CPlayerWeapon::Get_BonePos(const char * pBoneName)
{
	return _fVector();
}

void CPlayerWeapon::Update_AttachCamPos()
{
}

void CPlayerWeapon::Free()
{
	__super::Free();

}
