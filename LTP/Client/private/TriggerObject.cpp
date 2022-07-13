#include "stdafx.h"
#include "..\public\TriggerObject.h"


CTriggerObject::CTriggerObject(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CTriggerObject::CTriggerObject(const CTriggerObject & rhs)
	: CGameObject(rhs)
{
}

HRESULT CTriggerObject::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CTriggerObject::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	return S_OK;
}

_int CTriggerObject::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0) return -1;

	return _int();
}

_int CTriggerObject::LateUpdate(_double fDeltaTimer)
{
	if (__super::LateUpdate(fDeltaTimer) < 0)return -1;

	return _int();
}

_int CTriggerObject::Render()
{
	if (__super::Render() < 0)		return -1;

	return _int();
}

_int CTriggerObject::LateRender()
{
	return _int();
}

_int CTriggerObject::Active_Trigger(_double fDeltaTime)
{
	return _int();
}

CTriggerObject::EParkourTriggerType  CTriggerObject::Get_ParkourTriggerType()
{
	return EParkourTriggerType();
}

void CTriggerObject::Free()
{
	__super::Free();

}

