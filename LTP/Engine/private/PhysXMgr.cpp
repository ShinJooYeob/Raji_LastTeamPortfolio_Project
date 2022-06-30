#include "..\Public\PhysXMgr.h"
#include "ExternFont.h"

#define PVD_HOST "127.0.0.1"

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

static PxDefaultErrorCallback gDefaultErrorCallback;
static PxDefaultAllocator gDefaultAllocatorCallback;
PxFoundation* gFoundation;

void CPhysXMgr::TestPhysX()
{
	// 초기화용 변수
//	PxDefaultAllocator			mAllocCallback;
//	PxDefaultErrorCallback		mErrorCallback;
	
	PxFoundation*				mFoundation;
	PxPvd*						mPvd;
	PxPhysics*					mPhysics;
	// Init



	mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback,
		gDefaultErrorCallback);
	NULL_CHECK_BREAK(mFoundation);

	//bool recordMemoryAllocations = true;

//	mPvd = PxCreatePvd(*gFoundation);
	//PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	//mPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);


	mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation,
		PxTolerancesScale());
	//NULL_CHECK_BREAK(mPhysics);

	int a = 5;



//	PxSceneDesc desc(mPhyscis->getTolerancesScale());
//	desc.gravity = PxVec3(0, -9.8f, 0.0f);
//	mDispatcher = PxDefaultCpuDispatcherCreate(2);
//	desc.cpuDispatcher = mDispatcher;
//	desc.filterShader = PxDefaultSimulationFilterShader;

}

void CPhysXMgr::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);
}

