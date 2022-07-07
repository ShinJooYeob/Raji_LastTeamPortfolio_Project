#pragma once

#include "Base.h"

BEGIN(Engine)


// 두 엑터를 연결해서 사용한다.
typedef PxJoint* (*JointCreateFunction)(PxRigidActor* a0, const PxTransform& t0, PxRigidActor* a1, const PxTransform& t1);


enum E_SHAPE_TYPE
{
	SHAPE_NONE,
	// 기본 트리거
	SHAPE_BASE_TRIGGER,
	// 필터 트리거
	SHAPE_FITTER_TRIGGER,
	// 콜백 트리거
	SHAPE_CALLBACK_TRIGGER,
	SHAPE_END,
};


class CPhysXMgr final : public CBase
{
	DECLARE_SINGLETON(CPhysXMgr)


public:
	CPhysXMgr();
	virtual ~CPhysXMgr() = default;

public:
	HRESULT					Initialize_PhysX(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	HRESULT					Update_PhysX(_double timedelta);
	HRESULT					LateUpdate_PhysX(_double timedelta);
	HRESULT					Renderer();


	PxFoundation*			Get_Foundation();
	PxPhysics*				Get_PhysicsCreater();
	PxCooking*				Get_PhysicsCooking();
	PxScene*				Get_PhysicsScene();
	
public:
	// Shape Test
	HRESULT					CreateBox_Actor(PxRigidActor* actor, PxMaterial* Material, PxVec3 halfExtent);
	HRESULT					CreateSphere_Actor(PxRigidActor* actor, PxMaterial* Material, _float halfExtent);
	
	// Create Shape
	PxShape* CreateDemoShape(E_SHAPE_TYPE type,const PxGeometry& geom, bool isExclusive = false);
	// Create Chain

public:
	// Actor Test
	PxRigidDynamic*			CreateDynamic(const PxTransform& t, const PxGeometry& geometry, const PxVec3& velocity = PxVec3(0));

	PxRigidDynamic*			CreateDynamic_BaseActor(const PxTransform& t, const PxGeometry& geometry, PxReal density, const PxVec3& velocity = PxVec3(0));
	PxRigidStatic*			CreateStatic_BaseActor(const PxTransform& t, const PxGeometry& geometry);

	HRESULT					Create_Plane();

	void					KEYTEST();

	HRESULT					ResetScene();

private:
	HRESULT CreateBase_Plane(PxVec3 point);
	HRESULT CreateStack_Test(const PxTransform& trans, PxU32 size, PxReal halfExtent);
	HRESULT Clean_Phyics();
	HRESULT Create_Cook();

	HRESULT Render_Actor(const PxRigidActor* actor);
	HRESULT RenderShape(const PxGeometryHolder& h);
	

private:
	HRESULT Initialize_PhysXLib();
	


private:
	ID3D11Device*				m_pDevice = nullptr;
	ID3D11DeviceContext*		m_pDeviceContext = nullptr;

	PxDefaultAllocator			mAllocCallback;
	PxDefaultErrorCallback		mErrorCallback;

	PxDefaultCpuDispatcher*		mDisPatcher = nullptr;
	PxTolerancesScale			mToleranceScale;

	PxFoundation*				mFoundation = nullptr;
	PxPhysics*					mPhysics = nullptr;
	// 모델 메시 제어
	PxCooking*					mCooking = nullptr;

	PxScene*					mScene = nullptr;
	PxMaterial*					mMaterial = nullptr;

	PxPvd*						mPvd = nullptr;

	// TESTCODE

	PxActor*						mTestActor = nullptr;
	PxRigidActor*					mTestRigActor = nullptr;
	PxTransform*					mTestTransfrom1 = nullptr;

public:
	static PxPhysics*			gPhysics;
	static PxCooking*			gCooking;
	static PxFoundation*		gFoundation;
	static PxMaterial*			gMaterial;

	static PxMaterial*			gMaterial1;
	static PxMaterial*			gMaterial2;


	static _float3 gDebugValue1;
	static _float3 gDebugValue2;
	static _float3 gDebugValue3;
	static _float3 gDebugValue4;


public:
	HRESULT	CreateDemoMap();	
	HRESULT	CreateDemoMap_StaticBox(PxTransform px,PxVec3 scale);

public:
	virtual void Free() override;
};


// 충돌 관련 콜백함수
class CContactReportCallback
	:public PxSimulationEventCallback
{
	// 충돌 콜백함수
	// 씬에연결해서 콜백을 받습니다.
	// OnAdvance를 제외하고  PxScene::fetchResults()에서 호출
	// sendPendingReports = true인  PxScene::flushSimulation() 에서 호출


public:
	// PxSimulationEventCallback을(를) 통해 상속됨
	virtual void onConstraintBreak(PxConstraintInfo * constraints, PxU32 count) override;
	virtual void onWake(PxActor ** actors, PxU32 count) override;
	virtual void onSleep(PxActor ** actors, PxU32 count) override;
	virtual void onContact(const PxContactPairHeader & pairHeader, const PxContactPair * pairs, PxU32 nbPairs) override;
	virtual void onTrigger(PxTriggerPair * pairs, PxU32 count) override;
	virtual void onAdvance(const PxRigidBody * const * bodyBuffer, const PxTransform * poseBuffer, const PxU32 count) override;
};

//class CDemoConectCallback :
//	public PxContactModifyCallback
//{
//	// PxContactModifyCallback을(를) 통해 상속됨
//	virtual void onContactModify(PxContactModifyPair * const pairs, PxU32 count) override;
//};

END
