#include "stdafx.h"
#include "..\public\Phyxs\PhyxSampleTest.h"

CPhyxSampleTest::CPhyxSampleTest()
{
}


HRESULT CPhyxSampleTest::Initialize_Prototype()
{

	// 연결만 시키면 될듯


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

HRESULT CPhyxSampleTest::SnipTestCreate_Func1()
{

	PxScene*					mScene = nullptr;
	PxMaterial*					mMaterial = nullptr;
	PxTolerancesScale			mToleranceScale;
	
	
	PxFoundation*				mFoundation = g_pGameInstance->Get_Foundation();
	PxPhysics*					mPhysics = g_pGameInstance->Get_PhysicsCreater();
	PxCooking*					mCooking = g_pGameInstance->Get_PhysicsCooking();

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

void CPhyxSampleTest::Free()
{
}
