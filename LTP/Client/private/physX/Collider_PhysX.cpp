#include "stdafx.h"
#include "..\Public\PhysX\Collider_PhysX.h"

CCollider_PhysX::CCollider_PhysX(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CComponent(pDevice, pDeviceContext)
{
}

CCollider_PhysX::CCollider_PhysX(const CCollider_PhysX & rhs)
	: CComponent(rhs)
{
}


HRESULT CCollider_PhysX::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;

	return S_OK;
}

HRESULT CCollider_PhysX::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;


	

	return S_OK;
}



CCollider_PhysX * CCollider_PhysX::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{

	CCollider_PhysX* pInstance = new CCollider_PhysX(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Create Transform Prototype");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CCollider_PhysX::Clone(void * pArg)
{
	CCollider_PhysX* pInstance = new CCollider_PhysX((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Create Transform");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCollider_PhysX::Free()
{
	__super::Free();


}
