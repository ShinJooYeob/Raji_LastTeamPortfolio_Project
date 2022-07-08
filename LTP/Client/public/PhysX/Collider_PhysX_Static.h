#pragma once

#include "Collider_PhysX_Base.h"

BEGIN(Client)



class CCollider_PhysX_Static : public CCollider_PhysX_Base
{
public:
	enum E_STATICTYPE
	{
		E_STATIC_BUFFER,
		E_STATIC_STATIC,
		E_STATIC_END,
	};


private:
	explicit CCollider_PhysX_Static(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	explicit CCollider_PhysX_Static(const CCollider_PhysX_Static& rhs);
	virtual ~CCollider_PhysX_Static() = default;

public:
	HRESULT Initialize_Prototype(void * pArg);
	HRESULT Initialize_Clone(void * pArg);

	virtual HRESULT Update_BeforeSimulation()override;
	virtual HRESULT Update_AfterSimulation()override;

public:
	const PHYSXDESC_STATIC& Get_PhysXDesc() const { return mPhysXDesc; }
	HRESULT Set_ColiiderDesc(PHYSXDESC_STATIC desc);
	HRESULT Set_ColiiderBufferDesc(PHYSXDESC_STATIC desc);
	HRESULT	Set_eDISABLE_SIMULATION(bool b = true);
	
public:
#ifdef _DEBUG
	virtual HRESULT Render() override;

#endif // _DEBUG
	// static 충돌체 생성

protected:
	E_STATICTYPE			mStaticID = E_STATIC_END;
	PHYSXDESC_STATIC		mPhysXDesc;
	PxRigidStatic*			mPxRigStaticActor;

	// 트리거 객체
	_bool					mbTrigger = false;


public:
	static CCollider_PhysX_Static* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void* pArg = nullptr);
	virtual CComponent* Clone(void* pArg)override;
	virtual void Free()override;

	// CCollider_PhysX_Base을(를) 통해 상속됨
};

END