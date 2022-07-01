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

// static PxDefaultErrorCallback gDefaultErrorCallback;
// static PxDefaultAllocator gDefaultAllocatorCallback;
// PxFoundation* gFoundation;

void CPhysXMgr::TestPhysX()
{
	// 초기화용 변수
	PxDefaultAllocator			mAllocCallback;
	PxDefaultErrorCallback		mErrorCallback;
	
	PxDefaultCpuDispatcher*		mDisPatcher = nullptr;
	PxTolerancesScale			mToleranceScale;

	PxFoundation*				mFoundation;
	PxPhysics*					mPhysics = nullptr;

	PxScene*					mScene = nullptr;
	PxMaterial*					mMaterial = nullptr;

	PxPvd*						mPvd;


	// Init

	mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, mAllocCallback,
		mErrorCallback);
	NULL_CHECK_BREAK(mFoundation);

#ifdef  _DEBUG

	mPvd = PxCreatePvd(*mFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	mPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

#endif //  _DEBUG

	
	mToleranceScale.length = 100;
	mToleranceScale.speed = 981;

#ifdef  _DEBUG
	mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation, mToleranceScale, true, mPvd);
	NULL_CHECK_BREAK(mPhysics);
	
#else
	mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation, mToleranceScale);
	NULL_CHECK_BREAK(mPhysics);
#endif

	PxSceneDesc sceneDesc(mPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0, -9.81f, 0.0f);

#ifdef  _DEBUG
	mDisPatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = mDisPatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
#endif

	mScene = mPhysics->createScene(sceneDesc);



	// Create Simulation


	// Run




	PxSceneDesc desc(mPhysics->getTolerancesScale());
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

