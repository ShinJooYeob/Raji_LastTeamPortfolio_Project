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

	FAILED_CHECK(Initialize_PhysXLib());

	// #TEST
//	FAILED_CHECK(CreateTest_Base());
	FAILED_CHECK(Create_Cook());
	

	//while (1)
	//{
	//	mScene->simulate(1.f / 60.f);
	//	mScene->fetchResults(true);
	//}

	return S_OK;

}

HRESULT CPhysXMgr::Update_PhysX(_double timedelta)
{
	// Tick
	
	if (mScene)
	{
		// Debugger에서 실행되는 거 확인
		mScene->simulate(timedelta);
	}
	
	return S_OK;
}

HRESULT CPhysXMgr::LateUpdate_PhysX(_double timedelta)
{
	if (mScene)
	{
		// 결과 업데이트
		mScene->fetchResults(true);
	}

	return S_OK;
}

HRESULT CPhysXMgr::CreateTest_Base()
{
	// Mat 객체들 생성
	mMaterial = mPhysics->createMaterial(0.5f, 0.5f, 0.6f);
	// Plane 객체 생성
	PxRigidStatic* groudPlane = PxCreatePlane(*mPhysics, PxPlane(0, 1, 0, 99), *mMaterial);
	mScene->addActor(*groudPlane);

//	사각형 박스 생성
	float halfsize = 0.5f;
	const PxTransform t(PxVec3(0, 0, 0));
	PxU32 size = 5;
	CreateStack_Test(t, size, halfsize);


	return S_OK;
}

//Test
HRESULT CPhysXMgr::CreateStack_Test(const PxTransform & trans, PxU32 size, PxReal halfExtent)
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
	PX_RELEASE(shape);

	return S_OK;
}

HRESULT CPhysXMgr::Clean_Phyics()
{

	PX_RELEASE(mDisPatcher);
	PX_RELEASE(mScene);
	PX_RELEASE(mMaterial);
	PxCloseExtensions();
	if (mPvd)
	{
		PxPvdTransport* transport = mPvd->getTransport();
		PX_RELEASE(transport);
		PX_RELEASE(mPvd);
	}
//	PX_RELEASE(mCooking);	
	PX_RELEASE(mPhysics);
	PX_RELEASE(mFoundation);
	return S_OK;

}

HRESULT CPhysXMgr::Create_Cook()
{
	// 블록 메시 예제
	// 정점 정의
	PxVec3 convexVerts[] = { PxVec3(0,1,0),PxVec3(1,0,0),PxVec3(-1,0,0),PxVec3(0,0,1),
	PxVec3(0,0,-1) };

	// 레이아웃 설정
	PxConvexMeshDesc convexDesc;
	convexDesc.points.count = 5;
	convexDesc.points.stride = sizeof(PxVec3);
	convexDesc.points.data = convexVerts;
	convexDesc.flags = PxConvexFlag::eCOMPUTE_CONVEX;

	// CookLib 사용해서 정점 설정
	PxDefaultMemoryOutputStream buf;
	PxConvexMeshCookingResult::Enum result;
	if (!mCooking->cookConvexMesh(convexDesc, buf, &result))
		return NULL;
	PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
	PxConvexMesh* convexMesh = mPhysics->createConvexMesh(input);

	// 메시 인스턴스를 엑터한테 추가하는 개념
//	PxRigidActor* aConvexActor = mPhysics->createRigidDynamic(PxTransform(0,0,0));
	PxRigidActor* aConvexActor = mPhysics->createRigidStatic(PxTransform(0,0,0));
	mMaterial = mPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	PxShape* aConvexShape = PxRigidActorExt::createExclusiveShape(*aConvexActor,
		PxConvexMeshGeometry(convexMesh), *mMaterial);
	NULL_CHECK_BREAK(aConvexShape);

	mScene->addActor(*aConvexActor);

	return S_OK;
}

PxFoundation * CPhysXMgr::Get_Foundation()
{
	NULL_CHECK_BREAK(mFoundation);
	return mFoundation;
}

PxPhysics * CPhysXMgr::Get_PhysicsCreater()
{
	NULL_CHECK_BREAK(mPhysics);
	return mPhysics;
}

HRESULT CPhysXMgr::Initialize_PhysXLib()
{

	// Init
	mToleranceScale.length = 100;
	mToleranceScale.speed = 981;

	mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, mAllocCallback,
		mErrorCallback);
	NULL_CHECK_BREAK(mFoundation);

	// 메시 베이크에 해당되는 인자 전달
	// mPhysics->getPhysicsInsertionCallback();
	mCooking = PxCreateCooking(PX_PHYSICS_VERSION, *mFoundation, PxCookingParams(mToleranceScale));
	NULL_CHECK_BREAK(mCooking);


#ifdef  _DEBUG

	mPvd = PxCreatePvd(*mFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	mPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation, mToleranceScale, true, mPvd);
	NULL_CHECK_BREAK(mPhysics);

	PxInitExtensions(*mPhysics, mPvd);

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
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}
#endif // DEBUG

	return S_OK;
}


void CPhysXMgr::Free()
{

	Clean_Phyics();

	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);
}

