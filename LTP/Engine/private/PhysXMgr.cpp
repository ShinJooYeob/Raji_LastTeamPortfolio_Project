#include "..\Public\PhysXMgr.h"
#include "ExternFont.h"

IMPLEMENT_SINGLETON(CPhysXMgr)

CPhysXMgr::CPhysXMgr()
{
}

HRESULT CPhysXMgr::Initialize_PhysX(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	m_pDevice = pDevice;
	m_pDeviceContext = pDeviceContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);

	return S_OK;
}

void CPhysXMgr::TestPhysX()
{

}

void CPhysXMgr::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);
}
