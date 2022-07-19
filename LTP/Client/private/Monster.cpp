#include "stdafx.h"
#include "..\public\Monster.h"

CMonster::CMonster(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice,pDeviceContext)
{
}

CMonster::CMonster(const CMonster & rhs)
	:CGameObject(rhs)
{
}

HRESULT CMonster::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CMonster::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));



	CNavigation::NAVIDESC NaviDesc;
	NaviDesc.iCurrentIndex = 0;
	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Navigation), TAG_COM(Com_Navaigation), (CComponent**)&m_pNavigationCom, &NaviDesc));

	return S_OK;
}

_int CMonster::Update(_double dDeltaTime)
{
	if (__super::Update(dDeltaTime) < 0)
		return -1;

	return _int();
}

_int CMonster::LateUpdate(_double dDeltaTime)
{
	if (__super::LateUpdate(dDeltaTime) < 0)
		return -1;

	return _int();
}

_int CMonster::Render()
{
	if (__super::Render() < 0)
		return -1;

	return _int();
}

_int CMonster::LateRender()
{
	if (__super::LateRender() < 0)
		return -1;

	return _int();
}

void CMonster::Free()
{
	__super::Free();

	Safe_Release(m_pNavigationCom);
}
