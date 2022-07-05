#include "stdafx.h"
#include "..\Public\PhysX\Collider_PhysX_Joint.h"


CCollider_PhysX_Joit::CCollider_PhysX_Joit(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CCollider_PhysX_Base(pDevice, pDeviceContext)
{
}

CCollider_PhysX_Joit::CCollider_PhysX_Joit(const CCollider_PhysX_Joit & rhs)
	: CCollider_PhysX_Base(rhs)
{

}


HRESULT CCollider_PhysX_Joit::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;


	return S_OK;
}

HRESULT CCollider_PhysX_Joit::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;


	return S_OK;
}


HRESULT CCollider_PhysX_Joit::Update_BeforeSimulation()
{


	return S_OK;
}

HRESULT CCollider_PhysX_Joit::Update_AfterSimulation()
{

	return S_OK;
}


#ifdef _DEBUG

HRESULT CCollider_PhysX_Joit::Render()
{
	FAILED_CHECK(__super::Render());



	return S_OK;
}

#endif

CCollider_PhysX_Joit * CCollider_PhysX_Joit::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{

	CCollider_PhysX_Joit* pInstance = new CCollider_PhysX_Joit(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Create Transform Prototype");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CCollider_PhysX_Joit::Clone(void * pArg)
{
	CCollider_PhysX_Joit* pInstance = new CCollider_PhysX_Joit((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Create Transform");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCollider_PhysX_Joit::Free()
{
	__super::Free();




}
