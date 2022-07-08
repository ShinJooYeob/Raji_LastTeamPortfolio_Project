#pragma once

#include "Collider_PhysX_Base.h"

BEGIN(Client)



class CCollider_PhysX_Dynamic : public CCollider_PhysX_Base
{
public:
	typedef struct tag_DynamicFlags
	{
		// 충돌에 대한 임계값
		PxReal fSleepValue=1;
		PxReal fStabValue=1;
		PxReal fContactReportThreshold=1;
		PxReal fWakeCounter=1;

	}DYNAMICFLAG; 
	
	typedef struct tag_DynamicForce
	{
		PxReal fMass = 1;
		PxVec3 fLinearVelocity = PxVec3(0,0,0);
		PxVec3 fAngleVelocity = PxVec3(0, 0, 0);;
		PxReal fMaxLinearSpeed =0;
		PxReal fMaxAngleSpeed = 0;
		PxReal fDampingLinearSpeed = 0;
		PxReal fDampingAngleSpeed = 0;


	}DYNAMICFORCE;

private:
	explicit CCollider_PhysX_Dynamic (ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	explicit CCollider_PhysX_Dynamic (const CCollider_PhysX_Dynamic& rhs);
	virtual ~CCollider_PhysX_Dynamic () = default;

public:
	HRESULT Initialize_Prototype(void * pArg);
	HRESULT Initialize_Clone(void * pArg);

	virtual HRESULT Update_BeforeSimulation()override;
	virtual HRESULT Update_AfterSimulation()override;

public:
	const PHYSXDESC_DYNAMIC& Get_PhysXDesc() const { return mPhysXDesc; }
	_bool	Get_KeyDown() { return mbKeyDown; }
	void	Set_KeyDown(_bool b) { mbKeyDown = b; };
	void	Set_Kinecmatic(_bool b);

public:
#ifdef _DEBUG
	virtual HRESULT Render() override;

#endif // _DEBUG

	HRESULT Set_ColiiderDesc(PHYSXDESC_DYNAMIC desc);
	HRESULT	Set_DynamicFlag(DYNAMICFLAG flag);
	HRESULT	Set_DynamicValue(DYNAMICFORCE  flag);
	HRESULT Set_DynamicLock(PxRigidDynamicLockFlag::Enum e, bool value);
	
	HRESULT Set_RigidbodyFlag(PxRigidBodyFlag::Enum e, bool value);

	HRESULT Set_AddForce(PxVec3 foreVec, PxForceMode::Enum mode = PxForceMode::Enum::eFORCE);
	HRESULT Clear_Force(PxForceMode::Enum mode = PxForceMode::Enum::eFORCE);

private:
	void SetBaseFlag();

protected:
	PHYSXDESC_DYNAMIC	mPhysXDesc;
	PxRigidDynamic*		mPxDynamicActor = nullptr;
	PxReal				mfForce=5;

	_bool				mbKeyDown = false;

public:
	static CCollider_PhysX_Dynamic * Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void* pArg = nullptr);
	virtual CComponent* Clone(void* pArg)override;
	virtual void Free()override;

};

END