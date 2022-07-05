#include "stdafx.h"
#include "..\Public\PhysX\Collider_PhysX_Dynamic.h"


CCollider_PhysX_Dynamic ::CCollider_PhysX_Dynamic (ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CCollider_PhysX_Base(pDevice, pDeviceContext)
{
}

CCollider_PhysX_Dynamic ::CCollider_PhysX_Dynamic (const CCollider_PhysX_Dynamic  & rhs)
	: CCollider_PhysX_Base(rhs)
{

}

HRESULT CCollider_PhysX_Dynamic ::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;


	return S_OK;
}

HRESULT CCollider_PhysX_Dynamic ::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;


	return S_OK;
}


HRESULT CCollider_PhysX_Dynamic ::Update_BeforeSimulation()
{
	if (FAILED(__super::Update_BeforeSimulation()))
		return E_FAIL;


	return S_OK;
}

HRESULT CCollider_PhysX_Dynamic ::Update_AfterSimulation()
{
	if (FAILED(__super::Update_AfterSimulation()))
		return E_FAIL;

	return S_OK;
}


#ifdef _DEBUG

HRESULT CCollider_PhysX_Dynamic ::Render()
{
	FAILED_CHECK(__super::Render());



	return S_OK;
}

#endif

CCollider_PhysX_Dynamic  * CCollider_PhysX_Dynamic ::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{

	CCollider_PhysX_Dynamic * pInstance = new CCollider_PhysX_Dynamic (pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Create Transform Prototype");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CCollider_PhysX_Dynamic ::Clone(void * pArg)
{
	CCollider_PhysX_Dynamic * pInstance = new CCollider_PhysX_Dynamic ((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Create Transform");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCollider_PhysX_Dynamic ::Free()
{
	__super::Free();




}
