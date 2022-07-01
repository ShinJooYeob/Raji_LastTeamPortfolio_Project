#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CPhysXMgr final : public CBase
{
	DECLARE_SINGLETON(CPhysXMgr)
public:
	CPhysXMgr();
	virtual ~CPhysXMgr() = default;

public:
	HRESULT Initialize_PhysX(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	HRESULT Update_PhysX(_double timedelta);
	HRESULT LateUpdate_PhysX(_double timedelta);


private:
	HRESULT CreateTest_Base();
	HRESULT CreateStack_Test(const PxTransform& trans, PxU32 size, PxReal halfExtent);
	HRESULT Clean_Phyics();

	HRESULT Create_Cook();

	
	PxFoundation*	Get_Foundation();
	PxPhysics*		Get_PhysicsCreater();

//	void			Set_Material(_float3 vec);

private:
	HRESULT Initialize_PhysXLib();


private:
	PxDefaultAllocator			mAllocCallback;
	PxDefaultErrorCallback		mErrorCallback;

	PxDefaultCpuDispatcher*		mDisPatcher = nullptr;
	PxTolerancesScale			mToleranceScale;

	PxFoundation*				mFoundation= nullptr;
	PxPhysics*					mPhysics = nullptr;
	// 모델 메시 제어
	PxCooking*					mCooking;

	PxScene*					mScene = nullptr;
	PxMaterial*					mMaterial = nullptr;

	PxPvd*						mPvd = nullptr;


public:

	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDeviceContext = nullptr;


public:
	virtual void Free() override;
};

END