#include "stdafx.h"
#include "..\Public\PhysX\PhysXMgr.h"
#include "..\Public\Player.h"
#include "..\Public\Camera_Main.h"
#include "..\TestObject_PhysX.h"
#include "..\PhysX\Collider_PhysX_Static.h"



#define							PVD_HOST "127.0.0.1"
#define							MAX_NUM_ACTOR_SHAPES 128

IMPLEMENT_SINGLETON(CPhysXMgr)

PxMaterial* CPhysXMgr::gMaterial = nullptr;
PxMaterial* CPhysXMgr::gMaterial1 = nullptr;
PxMaterial* CPhysXMgr::gMaterial2 = nullptr;
PxPhysics* CPhysXMgr::gPhysics = nullptr;
PxCooking* CPhysXMgr::gCooking = nullptr;
PxFoundation* CPhysXMgr::gFoundation = nullptr;

_float3 CPhysXMgr::gDebugValue1 = _float3::Zero();
_float3 CPhysXMgr::gDebugValue2 = _float3::Zero();
_float3 CPhysXMgr::gDebugValue3 = _float3::Zero();
_float3 CPhysXMgr::gDebugValue4 = _float3::Zero();

// Scene의 충돌 감지 세팅
PxFilterFlags SampleSubmarineFilterShader(
	PxFilterObjectAttributes attributes0, PxFilterData filterData0,
	PxFilterObjectAttributes attributes1, PxFilterData filterData1,
	PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
{
	// 트리거 충돌시 세팅
	if (PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1))
	{
		pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
		return PxFilterFlag::eDEFAULT;
	}
	
//	pairFlags = PxPairFlag::eCONTACT_DEFAULT;


	// 감지할 충돌 세팅
	pairFlags = PxPairFlag::eSOLVE_CONTACT
		| PxPairFlag::eDETECT_DISCRETE_CONTACT
		| PxPairFlag::eNOTIFY_TOUCH_FOUND;
		//| PxPairFlag::eNOTIFY_TOUCH_PERSISTS
	//	| PxPairFlag::eNOTIFY_CONTACT_POINTS;


	// 충돌 필터 설정 / 나중에
	// if ((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1))

	return PxFilterFlag::eDEFAULT;
}

static CContactReportCallback gContactReportCallback; // 충돌 이벤트 콜백 Scene에 연결
static CFiterCallback gFiterCallback; // 충돌 정의 콜백


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
//	CreateBase_Plane(PxVec3(0, -10, 0));

	return S_OK;

}
static float X = 0;
static float Z = 0;

HRESULT CPhysXMgr::Update_PhysX(_double timedelta)
{
	// Tick
	if (mScene)
	{
		// Debugger에서 실행되는 거 확인
		mScene->simulate((PxReal)timedelta);
	}
	return S_OK;
}

HRESULT CPhysXMgr::LateUpdate_PhysX(_double timedelta)
{
	// 업데이트전에 콜라이더를 받아옴

	if (mScene)
	{
		// 결과 업데이트
		mScene->fetchResults(true);
	}
	// 충돌이 끝나면 메세지를 받는다.
	// 현재 컴포넌트로 검사하고 맞는 명령을 실행.
	Call_CollisionFunc_Trigger();
	Call_CollisionFunc_Contect();
	ReleasePhysXCom();

	return S_OK;
}



HRESULT CPhysXMgr::Renderer()
{
//	PxShape* shapes[MAX_NUM_ACTOR_SHAPES];

	// 모든 객체 가져오기
	// PxU32 numActor = mScene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC);
	//if (numActor)
	//{
	//	// 모든 피직스 객체 Get
	//	std::vector<PxRigidActor*> actors(numActor);
	//	mScene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC,
	//		reinterpret_cast<PxActor**>(&actors[0]), numActor);

	//	for (auto& actor : actors)
	//	{
	//		PxTransform tr =  actor->getGlobalPose();
	//		Render_Actor(actor);
	//	}
	//}
	
	return S_OK;

}


static float stackZ = 0.f;

void CPhysXMgr::KEYTEST()
{
	if (KEYDOWN(DIK_C))
	{	
		for (PxU32 i = 0; i < 5; i++)
			CreateStack_Test(PxTransform(PxVec3(0, 0, stackZ -= 10.0f)), 10, 2.0f);
	}


	if (KEYDOWN(DIK_V))
	{
		PxVec3 Box1Pos = PxVec3(10, 0, -50);
		PxVec3 Box2Pos = PxVec3(-10, 0, -50);

		PxVec3 BOXScale = PxVec3(5, 30, 5);

		PxRigidStatic* BoxActor1 = mPhysics->createRigidStatic(PxTransform(Box1Pos));
		PxRigidStatic* BoxActor2 = mPhysics->createRigidStatic(PxTransform(Box2Pos));
		CreateBox_Actor(BoxActor1, mMaterial, BOXScale);
		CreateBox_Actor(BoxActor2, mMaterial, BOXScale);
	}

	if (KEYDOWN(DIK_SPACE))
	{
		// Test
		CCamera_Main* pMainCam = ((CCamera_Main*)(g_pGameInstance->Get_GameObject_By_LayerIndex(SCENE_STATIC, TAG_LAY(Layer_Camera_Main))));
		CTransform* pCamTransform = pMainCam->Get_Camera_Transform();
		NULL_CHECK_BREAK(pCamTransform);
		PxVec3 CamPos = *(PxVec3*)&pCamTransform->Get_MatrixState_Float3(CTransform::STATE_POS);
		PxTransform trans3 = PxTransform(CamPos);
		CreateDynamic(trans3, PxSphereGeometry(3), PxVec3(0, -50, -100));
	}


}

HRESULT CPhysXMgr::ResetScene()
{
	if (mScene)
	{
		PX_RELEASE(mScene);
		FAILED_CHECK(Initialize_PhysXLib());
	}
	return S_OK;
	
}

HRESULT CPhysXMgr::CreateBase_Plane(PxVec3 point)
{
	PxRigidStatic* groundPlane = PxCreatePlane(*mPhysics, PxPlane(point, PxVec3(0,1,0)), *mMaterial);
	mScene->addActor(*groundPlane);
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
	PxRigidActor* aConvexActor = mPhysics->createRigidStatic(PxTransform(0, 0, 0));

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
	PxHeightFieldGeometry hfGeom(aHeightField, PxMeshGeometryFlags(1), PxReal(10.f), PxReal(numCols), PxReal(numCols));

	// 엑터에 지형 정보 달기
	PxRigidActor* aHieightFieldActor = mPhysics->createRigidStatic(PxTransform(0, 0, 0));

	PxShape* aHeightFieldShape = PxRigidActorExt::createExclusiveShape(*aHieightFieldActor,
		hfGeom, *mMaterial);

	mScene->addActor(*aHieightFieldActor);
	return S_OK;
}

HRESULT CPhysXMgr::Create_Plane()
{
	PxRigidStatic* Plane = PxCreatePlane(*mPhysics, PxPlane( 0, 1, 0, 0), *mMaterial);
	mScene->addActor(*Plane);
	return S_OK;

}

HRESULT CPhysXMgr::Add_TriggerMsg(PxTriggerPair* msg)
{
	// 충돌체 검사해줌
	mListPxTriggerPair.push_back(msg);

	return S_OK;
}

HRESULT CPhysXMgr::Add_ContactMsg(PxContactPair* msg)
{
	// 충돌체 검사해줌
	mListContactPair.push_back(msg);
	return S_OK;
}

HRESULT CPhysXMgr::Add_CollisionObject(CCollider_PhysX_Base * ComPhysX)
{
	// 충돌체 컴포넌트에서 추가해준다.
	if (ComPhysX)
	{
		mListPshysXComColiders.push_back(ComPhysX);
		Safe_AddRef(ComPhysX);
	}

	return S_OK;
}


HRESULT CPhysXMgr::Render_Actor(const PxRigidActor* actor )
{
	bool sleeping = actor->is<PxRigidDynamic>() ? actor->is<PxRigidDynamic>()->isSleeping() : false;

	if (sleeping)
		return S_FALSE;

	PxShape* shapes[MAX_NUM_ACTOR_SHAPES];
	const PxU32 numShapes = actor->getNbShapes();
	if (numShapes >= MAX_NUM_ACTOR_SHAPES)
		return E_FAIL;

	actor->getShapes(shapes, numShapes);

	// 모양 마다 그려준다.
	for (PxU32 j = 0; j < numShapes; j++)
	{
		const PxMat44 shpaeWorld(PxShapeExt::getGlobalPose(*shapes[j], *actor));
		const PxGeometryHolder h = shapes[j]->getGeometry();
		RenderShape(h);

		int debug = 5;
	}

	return S_OK;
}

HRESULT CPhysXMgr::RenderShape(const PxGeometryHolder & h)
{
	const PxGeometry& geom =  h.any();

	switch (geom.getType())
	{
	case PxGeometryType::eSPHERE:		
	{
		const PxSphereGeometry& sphereGeom = static_cast<const PxSphereGeometry&>(geom);
		// sphereGeom.radius;
		break;
	}

	case PxGeometryType::eBOX:
	{
		const PxBoxGeometry& boxGeom = static_cast<const PxBoxGeometry&>(geom);
		//	boxGeom.halfExtents.x;
		break;
	}
	case PxGeometryType::eCAPSULE:
	{
		const PxCapsuleGeometry& capsuleGeom = static_cast<const PxCapsuleGeometry&>(geom);
		//	const PxF32 radius = capsuleGeom.radius;
		//	const PxF32 halfHeight = capsuleGeom.halfHeight;		

	}
		break;

	//case PxGeometryType::eCONVEXMESH:
	//	PxConvexMeshGeometry
	//	break;

	//case PxGeometryType::eTRIANGLEMESH:
	//	PxTriangleMeshGeometry
	//	break;


	default:
		break;


	}


	return S_OK;
}

PxFoundation * CPhysXMgr::Get_Foundation()
{
	NULL_CHECK_BREAK(mFoundation);
	return mFoundation;
}

PxPhysics * CPhysXMgr::Get_PhysicsCreater()
{
//	NULL_CHECK_BREAK(mPhysics);
	return mPhysics;
}

PxCooking * CPhysXMgr::Get_PhysicsCooking()
{
	NULL_CHECK_BREAK(mCooking);
	return mCooking;
}

PxScene * CPhysXMgr::Get_PhysicsScene()
{
//	NULL_CHECK_BREAK(mScene);
	return mScene;
}

HRESULT CPhysXMgr::Initialize_PhysXLib()
{
	
	// Init
	mToleranceScale.length = 100;
	mToleranceScale.speed = 981;

	// 트리거 타입

	mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, 
		mAllocCallback,
		mErrorCallback);
	NULL_CHECK_BREAK(mFoundation);
	gFoundation = mFoundation;

	// 메시 베이크에 해당되는 인자 전달
	// mPhysics->getPhysicsInsertionCallback();
	mCooking = PxCreateCooking(PX_PHYSICS_VERSION, *mFoundation, PxCookingParams(mToleranceScale));
	NULL_CHECK_BREAK(mCooking);
	gCooking = mCooking;


#ifdef  _DEBUG

	mPvd = PxCreatePvd(*mFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	mPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation, mToleranceScale, true, mPvd);
	NULL_CHECK_BREAK(mPhysics);
	gPhysics = mPhysics;

#ifdef _DEBUG
	PxInitExtensions(*mPhysics, mPvd);
#endif // _DEBUG

#else
	mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation, mToleranceScale);
	NULL_CHECK_BREAK(mPhysics);
#endif


	PxSceneDesc sceneDesc(mPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0, -9.81f, 0.0f);


	sceneDesc.simulationEventCallback = &gContactReportCallback;
	sceneDesc.filterShader = SampleSubmarineFilterShader;
	sceneDesc.filterCallback = &gFiterCallback;
	
//	sceneDesc.filterShader = &contactReportFilterShader;

//	sceneDesc.contactModifyCallback = PxDefaultSimulationFilterShader;


#ifdef  _DEBUG
	mDisPatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = mDisPatcher;
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

	mMaterial = mPhysics->createMaterial(0.5f, 0.5f, 0.6f);
	gMaterial = mMaterial;

	gMaterial1 = mPhysics->createMaterial(0.3f, 0.3f, 0.5f);
	gMaterial2 = mPhysics->createMaterial(0.8f, 0.8f, 1.0f);

	return S_OK;
}

HRESULT CPhysXMgr::Call_CollisionFunc_Trigger()
{
	if (mListPxTriggerPair.empty())
		return S_OK;

	PxTriggerPair* triggerdata = mListPxTriggerPair.front();
	mListPxTriggerPair.pop_front();

	COLLIDERTYPE_PhysXID type = COLLIDER_PHYSX_END;

	if (triggerdata->status &PxPairFlag::eNOTIFY_TOUCH_FOUND)
		type = COLLIDER_PHYSX_TRIGGERIN;

	else if (triggerdata->status &PxPairFlag::eNOTIFY_TOUCH_LOST)
		type = COLLIDER_PHYSX_TRIGGEROUT;

	if (type == COLLIDER_PHYSX_END)
		return S_OK;

	CCollider_PhysX_Base* col1 = Find_ComPxCollider(triggerdata->otherActor);
	CCollider_PhysX_Base* col2 = Find_ComPxCollider(triggerdata->triggerActor);

	Safe_Delete(triggerdata);

	if (col1 == nullptr || col2 == nullptr)
		return E_FAIL;

	col1->Get_GameObject()->CollisionPhysX_Trigger(col2->Get_GameObject(), col2->Get_ObjectID(), type);
	

	return S_OK;
}

HRESULT CPhysXMgr::Call_CollisionFunc_Contect()
{

	if (mListContactPair.empty())
		return S_OK;

	// 정적 오브젝트 / 동적 오브젝트 충돌 판단
	// #TODO: 동적 오브젝트 충돌처리

	while (mListContactPair.empty() == false)
	{
		PxContactPair* contectData = mListContactPair.front();
		mListContactPair.pop_front();

		COLLIDERTYPE_PhysXID colType = COLLIDER_PHYSX_END;

		if (contectData->events &PxPairFlag::eNOTIFY_TOUCH_FOUND)
			colType = COLLIDER_PHYSX_CONECTIN;

		else if (contectData->events &PxPairFlag::eNOTIFY_TOUCH_PERSISTS)
			colType = COLLIDER_PHYSX_CONECTSTAY;

		else if (contectData->events &PxPairFlag::eNOTIFY_TOUCH_LOST)
			colType = COLLIDER_PHYSX_CONECTOUT;

		else if (colType == COLLIDER_PHYSX_END)
			return E_FAIL;

		PxRigidActor* act1 = contectData->shapes[0]->getActor();
		PxRigidActor* act2 = contectData->shapes[1]->getActor();

		CCollider_PhysX_Base* col1 = Find_ComPxCollider(act1);
		CCollider_PhysX_Base* col2 = Find_ComPxCollider(act2);

		Safe_Delete(contectData);

		if (col1 == nullptr || col2 == nullptr)
			return E_FAIL;

		col1->Get_GameObject()->CollisionPhysX_Rigid(col2->Get_GameObject(), col2->Get_ObjectID(), colType);

	}

	



	return S_OK;
}

CGameObject * CPhysXMgr::Find_GameObject(PxRigidActor * searchActor)
{
	for (auto& obj: mListPshysXComColiders)
	{
		if (searchActor == obj->Get_ColliderActor())
			return obj->Get_GameObject();
	}

	return nullptr;
}


CCollider_PhysX_Base * CPhysXMgr::Find_ComPxCollider(PxRigidActor * searchActor)
{
	for (auto& obj : mListPshysXComColiders)
	{
		if (searchActor == obj->Get_ColliderActor())
			return obj;
	}
	return nullptr;
}


HRESULT CPhysXMgr::ReleasePhysXCom()
{
	for (auto& com : mListPshysXComColiders)
		Safe_Release(com);
	mListPshysXComColiders.clear();
	return S_OK;
}


HRESULT CPhysXMgr::CreateBox_Actor(PxRigidActor* actor, PxMaterial* Material, PxVec3 halfExtent)
{
	PxShape* shape = mPhysics->createShape(PxBoxGeometry(halfExtent.x, halfExtent.y, halfExtent.z), *Material);
	shape->setContactOffset(1);

	NULL_CHECK_BREAK(shape);
	actor->attachShape(*shape);
	mScene->addActor(*actor);
	shape->release();
	return S_OK;
}

HRESULT CPhysXMgr::CreateSphere_Actor(PxRigidActor * actor, PxMaterial * Material, _float halfExtent)
{
	PxShape* shape = mPhysics->createShape(PxSphereGeometry(halfExtent), *Material);
	NULL_CHECK_BREAK(shape);
	actor->attachShape(*shape);
	mScene->addActor(*actor);
	shape->release();
	return S_OK;
}

PxShape * CPhysXMgr::CreateDemoShape(E_SHAPE_TYPE type, const PxGeometry & geom, bool isExclusive)
{
	PxShape* shape = nullptr;
	if (type == SHAPE_NONE)
	{
		const PxShapeFlags shapeFlags = PxShapeFlag::eVISUALIZATION | PxShapeFlag::eSIMULATION_SHAPE;
		shape = mPhysics->createShape(geom, *mMaterial, isExclusive, shapeFlags);
		return shape;
	}
	else if (type == SHAPE_BASE_TRIGGER)
	{
		const PxShapeFlags shapeFlags = PxShapeFlag::eVISUALIZATION | PxShapeFlag::eTRIGGER_SHAPE;
		shape = mPhysics->createShape(geom, *mMaterial, isExclusive, shapeFlags);
		return shape;

	}
	
	return nullptr;

}

PxRigidDynamic* CPhysXMgr::CreateDynamic(const PxTransform& t, const PxGeometry& geometry, const PxVec3& velocity)
{
	PxTransform testrans = PxTransform(PxVec3(0, 0, 0));

	PxRigidDynamic* dynamic = PxCreateDynamic(*mPhysics, testrans, geometry, *mMaterial, 10.0f);
	dynamic->setAngularDamping(0.5f);
	dynamic->setLinearVelocity(velocity);
	mScene->addActor(*dynamic);
	return dynamic;
}

PxRigidDynamic * CPhysXMgr::CreateDynamic_BaseActor(const PxTransform & t,const PxGeometry& geometry,PxReal density,  const PxVec3 & velocity)
{

	PxRigidDynamic* actor = PxCreateDynamic(*mPhysics, t, geometry, *gMaterial1,density);
	NULL_CHECK_BREAK(actor);
	actor->setAngularDamping(0.05f);
	actor->setLinearVelocity(velocity);
	mScene->addActor(*actor);
	return actor;
}

PxRigidStatic * CPhysXMgr::CreateStatic_BaseActor(const PxTransform & t, const PxGeometry& geometry)
{
	PxRigidStatic* actor = PxCreateStatic(*mPhysics, t, geometry, *gMaterial2);
	NULL_CHECK_BREAK(actor);
	mScene->addActor(*actor);
	return actor;
}

PxRigidStatic * CPhysXMgr::CreateStatic_Base_ShapeActor(const PxTransform & t, PxShape& shape)
{
	PxRigidStatic* actor = PxCreateStatic(*mPhysics, t, shape);
	NULL_CHECK_BREAK(actor);
	mScene->addActor(*actor);
	return actor;
}




HRESULT CPhysXMgr::CreateDemoMap()
{
	// 충돌 테스트용 데모 맵 생성
	PxTransform pxTrnas;
	PxVec3		pxScale = PxVec3(1,1,1);

	pxTrnas.p = PxVec3(0, -1, 0);
	pxScale = PxVec3(10, 1, 10);
	CreateDemoMap_StaticBox(pxTrnas,pxScale);
	
	pxTrnas.p = PxVec3(-4, 0, 0);
	pxScale = PxVec3(2, 2, 2);

	CreateDemoMap_StaticBox(pxTrnas,pxScale);
	
	pxTrnas.p = PxVec3(3, 0, 0);
	pxScale = PxVec3(1, 3, 1);
	CreateDemoMap_StaticBox(pxTrnas, pxScale);

	pxTrnas.p = PxVec3(1, 1, 3);
	pxScale = PxVec3(5, 5, 5);
	CreateDemoMap_StaticBox(pxTrnas, pxScale,true);
//	CreateDemoMap_StaticBox(pxTrnas,pxScale);
//	CreateDemoMap_StaticBox(pxTrnas,pxScale);
//
	return S_OK;
}

HRESULT CPhysXMgr::CreateDemoMap_StaticBox(PxTransform px, PxVec3 scale, _bool triger)
{
	_uint nowScene = g_pGameInstance->Get_TargetSceneNum();

	FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer
	(nowScene, TAG_LAY(Layer_ColBase), TAG_OP(Prototype_Object_Static_PhysX)));
	CTestObject_PhysX* obj =
		static_cast<CTestObject_PhysX*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(nowScene, TAG_LAY(Layer_ColBase)));
	obj->Set_ColSetID(E_PHYTYPE_STATIC);
	obj->Set_ModelSetting(CTestObject_PhysX::MODEL_GEMETRY);

	CTransform* objTrans = (CTransform*)obj->Get_Component(TAG_COM(Com_Transform));
	CCollider_PhysX_Static* colStatic = (CCollider_PhysX_Static*)obj->Get_Component(TAG_COM(Com_Collider_PhysX));
	objTrans->Set_MatrixState(CTransform::STATE_POS, PXVEC3TOFLOAT3(px.p));
	objTrans->Scaled_All(PXVEC3TOFLOAT3(scale));

	CCollider_PhysX_Base::PHYSXDESC_STATIC createStatic;
	createStatic.bTrigger = triger;
	createStatic.eShapeType = E_GEOMAT_BOX;
	createStatic.mGameObect = obj;
	createStatic.mTrnasform = objTrans;
	NULL_CHECK_BREAK(createStatic.mTrnasform);
	colStatic->Set_ColiiderDesc(createStatic);
	return S_OK;
}

void CPhysXMgr::Free()
{
	Clean_Phyics();
	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);

	ReleasePhysXCom();
}


void CContactReportCallback::onConstraintBreak(PxConstraintInfo* /*constraints*/, PxU32 /*count*/)
{
	// PxConstraintInfo 의 제약 조건이 꺠졌을때 호출
	OutputDebugStringW(L"onConstraintBreak");
	OutputDebugStringW(L"\n");

}

void CContactReportCallback::onWake(PxActor** /*actors*/, PxU32 /*count*/)
{
	// 꺠어난 엑터 호출 // eSEND_SLEEP_NOTIFIES 설정시 
	// A->wakeUp()
	OutputDebugStringW(L"onWake");
	OutputDebugStringW(L"\n");
}

void CContactReportCallback::onSleep(PxActor** /*actors*/, PxU32 /*count*/)
{
	// A->putToSleep() 시에 호출
	OutputDebugStringW(L"onSleep");
	OutputDebugStringW(L"\n");
}


void CContactReportCallback::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 count)
{
	// 접촉 이벤트 발생 시 호출
	// pair로 호출 한쌍의 액터에 대한 호출된다.
	// #PxSimulationFilterCallback 참조
//	 OutputDebugStringW(L"onContact\n");

	while (count--)
	{
		const PxContactPair& current = *pairs++;

		if (current.events & PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			OutputDebugStringW(L"Add onContact 1\n");
			PxContactPair* Currentt = NEW PxContactPair;
			memcpy(Currentt, &current, sizeof(PxContactPair));
			GetSingle(CPhysXMgr)->Add_ContactMsg(Currentt);
		}

		//else if (current.events & PxPairFlag::eNOTIFY_TOUCH_PERSISTS)
		//{
		//	OutputDebugStringW(L"Add onContact 2\n");
		//	PxContactPair* Currentt = NEW PxContactPair;
		//	memcpy(Currentt, &current, sizeof(PxContactPair));
		//	GetSingle(CPhysXMgr)->Add_ContactMsg(Currentt);

		//}
		//else if (current.events & PxPairFlag::eNOTIFY_TOUCH_LOST)
		//{
		//	OutputDebugStringW(L"Add onContact 3\n");
		//	PxContactPair* Currentt = NEW PxContactPair;
		//	memcpy(Currentt, &current, sizeof(PxContactPair));
		//	GetSingle(CPhysXMgr)->Add_ContactMsg(Currentt);
		//}
		

	}
}

void CContactReportCallback::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
	//PxShapeFlag::eTRIGGER_SHAPE 에 대한 이벤트 전달
	while (count--)
	{
		const PxTriggerPair& current = *pairs++;

		// ignore pairs when shapes have been deleted
		if (current.flags & (PxTriggerPairFlag::eREMOVED_SHAPE_TRIGGER | PxTriggerPairFlag::eREMOVED_SHAPE_OTHER))
			continue;

		if (current.status & PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			OutputDebugStringW(L"Add Trigger 1\n");
			PxTriggerPair* Currentt = NEW PxTriggerPair;
			memcpy(Currentt, &current, sizeof(PxTriggerPair));
			GetSingle(CPhysXMgr)->Add_TriggerMsg(Currentt);

		}
		if (current.status & PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			OutputDebugStringW(L"Add Trigger 2\n");
			PxTriggerPair* Currentt = NEW PxTriggerPair;
			memcpy(Currentt, &current, sizeof(PxTriggerPair));
			GetSingle(CPhysXMgr)->Add_TriggerMsg(Currentt);

		}
	}
}

void CContactReportCallback::onAdvance(const PxRigidBody*const*, const PxTransform*, const PxU32)
{
	// flush 일때만 호출
	OutputDebugStringW(L"onAdvance");
	OutputDebugStringW(L"\n");
}

static	PxFilterFlags triggersUsingFilterCallback(PxFilterObjectAttributes /*attributes0*/, PxFilterData /*filterData0*/,
	PxFilterObjectAttributes /*attributes1*/, PxFilterData /*filterData1*/,
	PxPairFlags& pairFlags, const void* /*constantBlock*/, PxU32 /*constantBlockSize*/)
{
	//	printf("contactReportFilterShader\n");

	PX_ASSERT(getImpl() == FILTER_CALLBACK);

	pairFlags = PxPairFlag::eCONTACT_DEFAULT;

	//if (usesCCD())
	//	pairFlags |= PxPairFlag::eDETECT_CCD_CONTACT | PxPairFlag::eNOTIFY_TOUCH_CCD;

	return PxFilterFlag::eCALLBACK;
}

PxFilterFlags CFiterCallback::pairFound(PxU32 pairID, PxFilterObjectAttributes attributes0, PxFilterData filterData0, const PxActor * a0, const PxShape * s0, PxFilterObjectAttributes attributes1, PxFilterData filterData1, const PxActor * a1, const PxShape * s1, PxPairFlags & pairFlags)
{
	return PxFilterFlags();
}

void CFiterCallback::pairLost(PxU32 pairID, PxFilterObjectAttributes attributes0, PxFilterData filterData0, PxFilterObjectAttributes attributes1, PxFilterData filterData1, bool objectRemoved)
{
}

bool CFiterCallback::statusChange(PxU32 & pairID, PxPairFlags & pairFlags, PxFilterFlags & filterFlags)
{
	return false;
}

