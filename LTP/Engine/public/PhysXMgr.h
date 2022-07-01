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
	void TestPhysX();


public:
	HRESULT CreateStack(const PxTransform& trans, PxU32 size, PxReal halfExtent);


private:
	PxDefaultAllocator			mAllocCallback;
	PxDefaultErrorCallback		mErrorCallback;

	PxDefaultCpuDispatcher*		mDisPatcher = nullptr;
	PxTolerancesScale			mToleranceScale;

	PxFoundation*				mFoundation;
	PxPhysics*					mPhysics = nullptr;

	PxScene*					mScene = nullptr;
	PxMaterial*					mMaterial = nullptr;

	PxPvd*						mPvd;


public:

	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDeviceContext = nullptr;


public:
	virtual void Free() override;
};

END