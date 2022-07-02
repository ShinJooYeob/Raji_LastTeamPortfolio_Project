#include "stdafx.h"
#include "..\public\PhysX\PhyxSampleTest.h"
#include "..\public\PhysX\PhysXMgr.h"

CPhyxSampleTest::CPhyxSampleTest()
{
}


HRESULT CPhyxSampleTest::Initialize_Prototype()
{

	// 연결만 시키면 될듯

	mFoundation = GetSingle(CPhysXMgr)->Get_Foundation();
	mPhysics = GetSingle(CPhysXMgr)->Get_PhysicsCreater();
	mCooking = GetSingle(CPhysXMgr)->Get_PhysicsCooking();
//	mScene = g_pGameInstance->Get_PhysicsScene();
	mMaterial = mPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	FAILED_CHECK(SnipTestCreate_Func1());

	
	return S_OK;
}



_int CPhyxSampleTest::Update(_double dDeltaTime)
{

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
	PxRigidStatic* groundPlane = PxCreatePlane(*mPhysics, PxPlane(0, 1, 0, 0), *mMaterial);
	mScene->addActor(*groundPlane);

	for (PxU32 i = 0; i < 5; i++)
		CreateStack(PxTransform(PxVec3(0, 0, stackZ -= 10.0f)), 10, 2.0f);

	//if (!interactive)
	//	CreateDynamic(PxTransform(PxVec3(0, 40, 100)), PxSphereGeometry(10), PxVec3(0, -50, -100));

	return S_OK;
}

HRESULT CPhyxSampleTest::SnipTestCreate_Func2()
{
	return S_OK;
}

HRESULT CPhyxSampleTest::SnipTestCreate_Func3()
{
	return S_OK;
}

HRESULT CPhyxSampleTest::CreateStack(const PxTransform& t, PxU32 size, PxReal halfExtent)
{
	//PxShape* shape = mPhysics->createShape(PxBoxGeometry(halfExtent, halfExtent, halfExtent), *mMaterial);
	//for (PxU32 i = 0; i < size; i++)
	//{
	//	for (PxU32 j = 0; j < size - i; j++)
	//	{
	//		PxTransform localTm(PxVec3(PxReal(j * 2) - PxReal(size - i), PxReal(i * 2 + 1), 0) * halfExtent);
	//		PxRigidDynamic* body = mPhysics->createRigidDynamic(t.transform(localTm));
	//		body->attachShape(*shape);
	//		PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
	//		mScene->addActor(*body);
	//	}
	//}
	//shape->release();
	return S_OK;
}

CPhyxSampleTest * CPhyxSampleTest::Create()
{
	CPhyxSampleTest*	pInstance = new CPhyxSampleTest();

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
