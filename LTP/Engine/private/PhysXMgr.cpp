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

	TestPhysX();


	return S_OK;
}

void CPhysXMgr::TestPhysX()
{
	// 초기화용 변수
	PxDefaultAllocator			mAllocCallback;
	PxDefaultErrorCallback		mErrorCallback;
	PxDefaultCpuDispatcher*		mDispatcher;
	PxTolerancesScale			mTolerancesScale;
	PxFoundation*				mFoundation;
	PxPhysics*					mPhyscis;
	PxScene*					mScene;
	PxMaterial*					mMaterial;
	PxPvd*						mPvd;
	
	// Init
	mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, mAllocCallback, mErrorCallback);
	NULL_CHECK_BREAK(mFoundation);

//	mPvd = PxCreatePvd(*mFoundation);


}

void CPhysXMgr::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);
}
