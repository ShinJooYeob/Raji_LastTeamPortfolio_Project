#include "stdafx.h"
#include "Camera_Main.h"

#include "..\public\PhysX\PhyxSampleTest.h"
#include "..\public\PhysX\PhysXMgr.h"

/*
typedef int64_t PxI64;
typedef uint64_t PxU64;
typedef int32_t PxI32;
typedef uint32_t PxU32;
typedef int16_t PxI16;
typedef uint16_t PxU16;
typedef int8_t PxI8;
typedef uint8_t PxU8;
typedef float PxF32;
typedef double PxF64;
typedef float PxReal;
*/


CPhyxSampleTest::CPhyxSampleTest()
{
}


HRESULT CPhyxSampleTest::Initialize_Prototype()
{

	// 연결만 시키면 될듯

//	mFoundation = GetSingle(CPhysXMgr)->Get_Foundation();
//	mPhysics = GetSingle(CPhysXMgr)->Get_PhysicsCreater();
//	mCooking = GetSingle(CPhysXMgr)->Get_PhysicsCooking();
//	mScene = GetSingle(CPhysXMgr)->Get_PhysicsScene();
//	mMaterial = mPhysics->createMaterial(0.5f, 0.5f, 0.6f);

//	FAILED_CHECK(SnipTestCreate_Func1());

	
	return S_OK;
}



_int CPhyxSampleTest::Update(_double dDeltaTime)
{

	//if (KEYDOWN(DIK_SPACE))
	//{
	//	CCamera_Main* pMainCam = ((CCamera_Main*)(g_pGameInstance->Get_GameObject_By_LayerIndex(SCENE_STATIC, TAG_LAY(Layer_Camera_Main))));
	//	CTransform* pCamTransform = pMainCam->Get_Camera_Transform();
	//	NULL_CHECK_BREAK(pCamTransform);
	//	_float3 trans3 = pCamTransform->Get_MatrixState_Float3(CTransform::STATE_POS);
	//	createDynamic(PxTransform(trans3.x, trans3.y, trans3.z), PxSphereGeometry(3.0f),PxVec3(0, 0, -1) * 200);
	//}

	return _int();
}

_int CPhyxSampleTest::LateUpdate(_double dDeltaTime)
{


	return _int();
}

_int CPhyxSampleTest::Render()
{


	return _int();
}

PxReal stackZ = 10.0f;

HRESULT CPhyxSampleTest::SnipTestCreate_Func1()
{
	// 스텍 생성 / 공과 충돌
	PxRigidStatic* groundPlane = PxCreatePlane(*mPhysics, PxPlane(0, 1, 0, 0), *mMaterial);
	mScene->addActor(*groundPlane);

	for (PxU32 i = 0; i < 5; i++)
		CreateStack(PxTransform(PxVec3(0, 0, stackZ -= 10.0f)), 10, 2.0f);
	return S_OK;
}

HRESULT CPhyxSampleTest::SnipTestCreate_Func2()
{
	// 메시 쿠킹 옵션
	PxTolerancesScale ToleranceScale;
	ToleranceScale.length = 100;
	ToleranceScale.speed = 981;

	// eBVH33 : 변형메시 사용
	PxCookingParams cookingParams = PxCookingParams(ToleranceScale);
	cookingParams.midphaseDesc.setToDefault(PxMeshMidPhase::eBVH33);
	cookingParams.meshPreprocessParams = PxMeshPreprocessingFlag::eDISABLE_CLEAN_MESH;


	//// get vertex array
	//PxVec3* verts = mesh->getVerticesForModification();

	//// update the vertices here


	//	// tell PhysX to update the mesh structure
	//	PxBounds3 newBounds = mesh->refitBVH();


	//	// created earlier
	//	PxRigidActor* myActor;
	//	PxTriangleMesh* myTriMesh;
	//	PxMaterial* myMaterial;

	//	// create a shape instancing a triangle mesh at the given scale
	//	PxMeshScale scale(PxVec3(x, y, z), PxQuat(PxIdentity));
	//	PxTriangleMeshGeometry geom(myTriMesh, scale);
	//	PxShape* myTriMeshShape = PxRigidActorExt::createExclusiveShape(*myActor, geom, *myMaterial);


	return S_OK;
}

HRESULT CPhyxSampleTest::SnipTestCreate_Func3()
{
	return S_OK;
}

HRESULT CPhyxSampleTest::CreateStack(const PxTransform& t, PxU32 size, PxReal halfExtent)
{
	PxShape* shape = mPhysics->createShape(PxBoxGeometry(halfExtent, halfExtent, halfExtent), *mMaterial);
	for (PxU32 i = 0; i < size; i++)
	{
		for (PxU32 j = 0; j < size - i; j++)
		{
			PxTransform localTm(PxVec3(PxReal(j * 2) - PxReal(size - i), PxReal(i * 2 + 1), 0) * halfExtent);
			PxRigidDynamic* body = mPhysics->createRigidDynamic(t.transform(localTm));
			body->attachShape(*shape);
			PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
			mScene->addActor(*body);
		}
	}
	shape->release();
	return S_OK;
}

PxRigidDynamic * CPhyxSampleTest::createDynamic(const PxTransform & t, const PxGeometry & geometry, const PxVec3 & velocity)
{
	PxRigidDynamic* dynamic = PxCreateDynamic(*mPhysics, t, geometry, *mMaterial, 10.0f);
	dynamic->setAngularDamping(0.5f);
	dynamic->setLinearVelocity(velocity);
	mScene->addActor(*dynamic);
	return dynamic;
}


CPhyxSampleTest * CPhyxSampleTest::Create()
{
	CPhyxSampleTest*	pInstance = NEW CPhyxSampleTest();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSGBOX("Failed to Created CPhyxSampleTest");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPhyxSampleTest::Free()
{
	// Free

}
