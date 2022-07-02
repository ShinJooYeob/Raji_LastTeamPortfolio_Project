#include "stdafx.h"
#include "..\Public\PhysX\PhysXMgr.h"

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
	FAILED_CHECK(CreateTest_Base());
//	FAILED_CHECK(Create_Cook());
	

	while (1)
	{
		mScene->simulate(1.f / 60.f);
		mScene->fetchResults(true);
	}

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


	PxRigidStatic* groundPlane = PxCreatePlane(*mPhysics, PxPlane(0, 1, 0, 0), *mMaterial);
	mScene->addActor(*groundPlane);


//	for (PxU32 i = 0; i < 5; i++)
//		createStack(PxTransform(PxVec3(0, 0, stackZ -= 10.0f)), 10, 2.0f);
//
//	if (!interactive)
//		createDynamic(PxTransform(PxVec3(0, 40, 100)), PxSphereGeometry(10), PxVec3(0, -50, -100));
//


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
#ifdef _DEBUG
	PxCloseExtensions();
#endif // _DEBUG
	PX_RELEASE(mPhysics);
	PX_RELEASE(mCooking);
	PX_RELEASE(mFoundation);

#ifdef _DEBUG
	//if (mPvd)
	//{
	//	PxPvdTransport* transport = mPvd->getTransport();
	//	PX_RELEASE(mPvd);
	//	PX_RELEASE(transport);

	//}
#endif // _DEBUG
	return S_OK;

}

HRESULT CPhysXMgr::Create_Cook()
{
	// 정점 메시 예제
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
	PxRigidActor* aConvexActor = mPhysics->createRigidStatic(PxTransform(0,0,0));
	mMaterial = mPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	PxShape* aConvexShape = PxRigidActorExt::createExclusiveShape(*aConvexActor,
		PxConvexMeshGeometry(convexMesh), *mMaterial);
	NULL_CHECK_BREAK(aConvexShape);

	mScene->addActor(*aConvexActor);


	//// Triangle Mesh Ex
	//PxTriangleMeshDesc meshDesc;
	//// 기존에 Model에서 정의하듯이 정의한다.
	//meshDesc.points.count = nbVerts;
	//meshDesc.points.stride = sizeof(PxVec3);
	//meshDesc.points.data = verts;

	//meshDesc.triangles.count = triCount;
	//meshDesc.triangles.stride = 3 * sizeof(PxU32);
	//meshDesc.triangles.data = indices32;

	//PxDefaultMemoryOutputStream writeBuffer;
	//PxTriangleMeshCookingResult::Enum result;
	//bool status = cooking.cookTriangleMesh(meshDesc, writeBuffer, result);
	//if (!status)
	//	return NULL;

	//PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());
	//return physics.createTriangleMesh(readBuffer);


	//PxTriangleMesh* aTriangleMesh = theCooking->createTriangleMesh(meshDesc,
	//	thePhysics->getPhysicsInsertionCallback());


	// 지형
	int numCols = 10;
	int numRows = 10;

	// 지형 샘플러 X*Z
	PxHeightFieldSample* samples = NEW PxHeightFieldSample[(sizeof(PxHeightFieldSample) * (numCols * numRows))];
	// 지형 정보
	PxHeightFieldDesc hfDesc;
	hfDesc.format = PxHeightFieldFormat::eS16_TM;
	hfDesc.nbColumns = numCols;
	hfDesc.nbRows = numRows;
	hfDesc.samples.data = samples;
	hfDesc.samples.stride = sizeof(PxHeightFieldSample);

	// 지형 클래스 cook 클래스로 샘플링된 지형 정보 저장 == 정점 정보저장
	PxHeightField* aHeightField = mCooking->createHeightField(hfDesc,
		mPhysics->getPhysicsInsertionCallback());


	// 지형 도형 인스턴스
	PxHeightFieldGeometry hfGeom(aHeightField, PxMeshGeometryFlags(1), 10, numCols, numCols);

	// 엑터에 지형 정보 달기
	PxRigidActor* aHieightFieldActor = mPhysics->createRigidStatic(PxTransform(0, 0, 0));

	PxShape* aHeightFieldShape = PxRigidActorExt::createExclusiveShape(*aHieightFieldActor,
		hfGeom, *mMaterial);

	mScene->addActor(*aHieightFieldActor);


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

PxCooking * CPhysXMgr::Get_PhysicsCooking()
{
	NULL_CHECK_BREAK(mCooking);
	return mCooking;
}

PxScene * CPhysXMgr::Get_PhysicsScene()
{
	NULL_CHECK_BREAK(mScene);
	return mScene;
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

//	Clean_Phyics();

	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);
}

