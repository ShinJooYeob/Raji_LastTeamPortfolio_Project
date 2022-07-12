#include "stdafx.h"
#include "..\public\SpearWave.h"


CSpearWave::CSpearWave(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CSpearWave::CSpearWave(const CSpearWave & rhs)
	: CGameObject(rhs)
{
}

HRESULT CSpearWave::Initialize_Prototype(void * pArg)
{
	return E_NOTIMPL;
}

HRESULT CSpearWave::Initialize_Clone(void * pArg)
{
	return E_NOTIMPL;
}

_int CSpearWave::Update(_double fDeltaTime)
{
	return _int();
}

_int CSpearWave::LateUpdate(_double fDeltaTimer)
{
	return _int();
}

_int CSpearWave::Render()
{
	return _int();
}

_int CSpearWave::LateRender()
{
	return _int();
}

void CSpearWave::Active_Trail(_bool bActivate)
{
}

CSpearWave * CSpearWave::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	return nullptr;
}

CGameObject * CSpearWave::Clone(void * pArg)
{
	return nullptr;
}

void CSpearWave::Free()
{
}
