#include "stdafx.h"
#include "..\Public\PhysX\PhysXMgr.h"
#include "..\Public\Player.h"
#include "..\Public\Camera_Main.h"

#define							PVD_HOST "127.0.0.1"
#define							MAX_NUM_ACTOR_SHAPES 128

IMPLEMENT_SINGLETON(CPhysXMgr)

PxMaterial* CPhysXMgr::gMaterial = nullptr;
PxPhysics* CPhysXMgr::gPhysics = nullptr;
PxCooking* CPhysXMgr::gCooking = nullptr;
PxFoundation* CPhysXMgr::gFoundation = nullptr;


static CDemoCallback gDemoCallback;


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



	return S_OK;

}
static float X = 0;
static float Z = 0;

HRESULT CPhysXMgr::Update_PhysX(_double timedelta)
{
	// #TEST
	// 플레이어 위치 받아서 테스트
	// KEYTEST();

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

	return S_OK;
}



HRESULT CPhysXMgr::Renderer()
{
	PxShape* shapes[MAX_NUM_ACTOR_SHAPES];

	// 모든 객체 가져오기
	PxU32 numActor = mScene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC);
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


void CPhysXMgr::KEYTEST()
{
	if (KEYDOWN(DIK_C))
	{
		// CreatePlayer
		CPlayer* pPlayer = ((CPlayer*)(g_pGameInstance->Get_GameObject_By_LayerIndex(SCENE_STAGE6, TAG_LAY(Layer_Player))));
		NULL_CHECK_BREAK(pPlayer);
		CTransform* pTransPos = pPlayer->Get_Transform();
		PxVec3 PlayerPos = *(PxVec3*)&pTransPos->Get_MatrixState_Float3(CTransform::STATE_POS);
		PxTransform PlayerTrans = PxTransform(PlayerPos);

		// mTestRigActor = mPhysics->createRigidDynamic(PlayerTrans);
		// CreateSphere_Actor(mTestRigActor, mMaterial, 1);

		mTestRigActor = CreateDynamic(PlayerTrans, PxSphereGeometry(3.0f), PxVec3(1, 1, 1));


		// CreateChain

		// CreateChain(PxTransform(PxVec3(0.0f, 20.0f, 0.0f)), 10, PxBoxGeometry(1.0f, 0.5f, 1.0f), 4.0f,
		// 	CPhysXMgr::CreateLimitedSpherical);
		// CreateChain(PxTransform(PxVec3(0.0f, 20.0f, -10.0f)), 5, PxBoxGeometry(2.0f, 0.5f, 0.5f), 4.0f, createBreakableFixed);
		// CreateChain(PxTransform(PxVec3(0.0f, 20.0f, -20.0f)), 5, PxBoxGeometry(2.0f, 0.5f, 0.5f), 4.0f, createDampedD6);
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
		CreateDynamic(trans3, PxSphereGeometry(3.0f), PxVec3(0, 0, -1) * 200);
	}


}

HRESULT CPhysXMgr::CreateTest_Base()
{
	PxRigidStatic* groundPlane = PxCreatePlane(*mPhysics, PxPlane(0, 1, 0, 0), *mMaterial);
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
	PxRigidActor* aConvexActor = mPhysics->createRigidStatic(PxTransform(0,0,0));


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
	PxHeightFieldGeometry hfGeom(aHeightField, PxMeshGeometryFlags(1), PxReal(10.f), numCols, numCols);

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
	sceneDesc.simulationEventCallback = &gDemoCallback;

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

	mMaterial = mPhysics->createMaterial(0.5f, 0.5f, 0.6f);
	gMaterial = mMaterial;

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

PxRigidDynamic* CPhysXMgr::CreateDynamic(const PxTransform& t, const PxGeometry& geometry, const PxVec3& velocity)
{
	PxTransform testrans = PxTransform(PxVec3(0, 0, 0));

	PxRigidDynamic* dynamic = PxCreateDynamic(*mPhysics, testrans, geometry, *mMaterial, 10.0f);
	dynamic->setAngularDamping(0.5f);
	dynamic->setLinearVelocity(velocity);
	mScene->addActor(*dynamic);
	return dynamic;
}

PxRigidDynamic * CPhysXMgr::CreateDynamic_BaseActor(const PxTransform & t,const PxGeometry& geometry,  const PxVec3 & velocity)
{
	PxTransform testrans = PxTransform(PxVec3(0,0,0));

	PxRigidDynamic* actor = PxCreateDynamic(*mPhysics, testrans, geometry, *mMaterial, 1.f);
	NULL_CHECK_BREAK(actor);
	actor->setAngularDamping(0.5f);
	actor->setLinearVelocity(velocity);
	mScene->addActor(*actor);
	return actor;
}

PxRigidStatic * CPhysXMgr::CreateStatic_BaseActor(const PxTransform & t, const PxGeometry& geometry)
{
	PxRigidStatic* actor = PxCreateStatic(*mPhysics, t, geometry, *mMaterial);
	NULL_CHECK_BREAK(actor);
	mScene->addActor(*actor);
	return actor;
}

HRESULT CPhysXMgr::CreateChain(const PxTransform& t, PxU32 length, const PxGeometry& g, PxReal separation, JointCreateFunction createJoint)
{
	// 관절 오브젝트 생성
	PxVec3 offset(separation / 2, 0, 0);
	PxTransform localTm(offset);
	PxRigidDynamic* prev = NULL;

	for (PxU32 i = 0; i < length; i++)
	{
		PxRigidDynamic* current = PxCreateDynamic(*mPhysics, t*localTm, g, *gMaterial, 1.0f);
		(*createJoint)(prev, prev ? PxTransform(offset) : t, current, PxTransform(-offset));
		mScene->addActor(*current);
		prev = current;
		localTm.p.x += separation;
	}

	return S_OK;
}

PxJoint * CPhysXMgr::CreateLimitedSpherical(PxRigidActor * a0, const PxTransform & t0, PxRigidActor * a1, const PxTransform & t1)
{
	// FIX
	PxFixedJoint* fix = PxFixedJointCreate(*gPhysics, a0, t0, a1, t1);
	// 파손 관절 예시
	fix->setBreakForce(1000, 100000);
	fix->setConstraintFlag(PxConstraintFlag::eDRIVE_LIMITS_ARE_FORCES, true);
	fix->setConstraintFlag(PxConstraintFlag::eDISABLE_PREPROCESSING, true);

	// Sphere
	PxSphericalJoint* spher = PxSphericalJointCreate(*gPhysics, a0, t0, a1, t1);
	spher->setLimitCone(PxJointLimitCone(PxPi / 4, PxPi / 4, 0.05f));
	spher->setSphericalJointFlag(PxSphericalJointFlag::eLIMIT_ENABLED, true);

//	spher->setConstraintFlag(PxConstraintFlag::eVISUALIZATION,true);//



	return spher;
}

void CPhysXMgr::Free()
{
	Clean_Phyics();
	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);
}

void CDemoCallback::onConstraintBreak(PxConstraintInfo * constraints, PxU32 count)
{
	OutputDebugStringW(L"onConstraintBreak_Demo");
	OutputDebugStringW(L"\n");
}

void CDemoCallback::onWake(PxActor ** actors, PxU32 count)
{
	OutputDebugStringW(L"onWake_Demo");
	OutputDebugStringW(L"\n");
}

void CDemoCallback::onSleep(PxActor ** actors, PxU32 count)
{
	OutputDebugStringW(L"onSleep_Demo");
	OutputDebugStringW(L"\n");
}

void CDemoCallback::onContact(const PxContactPairHeader & pairHeader, const PxContactPair * pairs, PxU32 nbPairs)
{
	OutputDebugStringW(L"onContact_Demo");
	OutputDebugStringW(L"\n");

	//for (PxU32 i = 0; i < nbPairs; i++)
	//{
	//	const PxContactPair& cp = pairs[i];

	//	if (cp.events & PxPairFlag::eNOTIFY_TOUCH_FOUND)
	//	{
	//		if ((pairHeader.actors[0] == mSubmarineActor) || (pairHeader.actors[1] == mSubmarineActor))
	//		{
	//			PxActor* otherActor = (mSubmarineActor == pairHeader.actors[0]) ? pairHeader.actors[1] : pairHeader.actors[0];
	//			Seamine* mine = reinterpret_cast<Seamine*>(otherActor->userData);
	//			// insert only once
	//			if (std::find(mMinesToExplode.begin(), mMinesToExplode.end(), mine) == mMinesToExplode.end())
	//				mMinesToExplode.push_back(mine);

	//			break;
	//		}
	//	}
	//}

}

void CDemoCallback::onTrigger(PxTriggerPair * pairs, PxU32 count)
{
	OutputDebugStringW(L"onTrigger_Demo");
	OutputDebugStringW(L"\n");
}

void CDemoCallback::onAdvance(const PxRigidBody * const * bodyBuffer, const PxTransform * poseBuffer, const PxU32 count)
{
	OutputDebugStringW(L"onAdvance_Demo");
	OutputDebugStringW(L"\n");
}

void CDemoConectCallback::onContactModify(PxContactModifyPair * const pairs, PxU32 count)
{
	// #TEST Contact Callback


}
