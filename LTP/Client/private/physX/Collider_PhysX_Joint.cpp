#include "stdafx.h"
#include "..\Public\PhysX\Collider_PhysX_Joint.h"


CCollider_PhysX_Joint::CCollider_PhysX_Joint(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CCollider_PhysX_Base(pDevice, pDeviceContext)
{
}

CCollider_PhysX_Joint::CCollider_PhysX_Joint(const CCollider_PhysX_Joint & rhs)
	: CCollider_PhysX_Base(rhs)
{

}


HRESULT CCollider_PhysX_Joint::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;


	return S_OK;
}

HRESULT CCollider_PhysX_Joint::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;


	return S_OK;
}


HRESULT CCollider_PhysX_Joint::Update_BeforeSimulation()
{


	return S_OK;
}

HRESULT CCollider_PhysX_Joint::Update_AfterSimulation()
{

	return S_OK;
}


#ifdef _DEBUG

HRESULT CCollider_PhysX_Joint::Render()
{
	FAILED_CHECK(__super::Render());



	return S_OK;
}

#endif

CCollider_PhysX_Joint * CCollider_PhysX_Joint::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{

	CCollider_PhysX_Joint* pInstance = new CCollider_PhysX_Joint(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Create Transform Prototype");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CCollider_PhysX_Joint::Clone(void * pArg)
{
	CCollider_PhysX_Joint* pInstance = new CCollider_PhysX_Joint((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Create Transform");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCollider_PhysX_Joint::Free()
{
	__super::Free();




}
