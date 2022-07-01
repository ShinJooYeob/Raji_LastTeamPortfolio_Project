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

//	TestPhysX();


	return S_OK;
}

// static PxDefaultErrorCallback gDefaultErrorCallback;
// static PxDefaultAllocator gDefaultAllocatorCallback;
// PxFoundation* gFoundation;

void CPhysXMgr::TestPhysX()
{
	// 초기화용 변수


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

#ifdef _DEBUG
	PxPvdSceneClient* pvdClient = mScene->getScenePvdClient();
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS,true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}


#endif // DEBUG

	
	// Create Simulation
	mMaterial = mPhysics->createMaterial(0.5f, 0.5f, 0.6f);
	// 스태틱 메시로 Plane 생성
	PxRigidStatic* groudPlane = PxCreatePlane(*mPhysics,PxPlane(0,1,0,99),*mMaterial);
	mScene->addActor(*groudPlane);

	float halfsize = 0.5f;
	const PxTransform t(PxVec3(0,0,0));
	PxU32 size = 30;
	
	CreateStack(t,size,halfsize);

	// Run
	while (1)
	{
		// Debugger에서 실행되는 거 확인
		mScene->simulate(1.0f / 60.0f);
		mScene->fetchResults(true); // 결과 업데이트
	}

}

HRESULT CPhysXMgr::CreateStack(const PxTransform & trans, PxU32 size, PxReal halfExtent)
{
	PxShape* shape = mPhysics->createShape(PxBoxGeometry(halfExtent, halfExtent, halfExtent), *mMaterial);
	for (PxU32 i = 0; i < size; i++)
	{
		for (PxU32 j = 0; j < size - i; j++)
		{
			PxTransform localTm(PxVec3(PxReal(j * 2) - PxReal(size - i), PxReal(i * 2 + 1), 0) * halfExtent);
			PxRigidDynamic* body = mPhysics->createRigidDynamic(trans.transform(localTm));
			NULL_CHECK_BREAK(body);
			body->attachShape(*shape);
			PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
			mScene->addActor(*body);
		}
	}
	shape->release();
	return S_OK;
}

void CPhysXMgr::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);
}

