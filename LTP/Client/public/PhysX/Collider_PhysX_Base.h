#pragma once

#include "Component.h"


BEGIN(Engine)
class CTransform;
END
BEGIN(Client)


enum E_PHYTYPE
{
	E_PHYTYPE_STATIC,
	E_PHYTYPE_DYNAMIC,
	E_PHYTYPE_JOINT,
	E_PHYTYPE_END

};

enum E_GEOMAT_TYPE
{
	E_GEOMAT_BOX,
	E_GEOMAT_SPEHE,
	E_GEOMAT_CAPSULE,
	E_GEOMAT_SHAPE,
	E_GEOMAT_VERTEX,
	E_GEOMAT_TRIANGLE,
	E_GEOMAT_END,
};



class CCollider_PhysX_Base : public CComponent
{
public:
	// 각각 생성하는 콜라이더에 따른 DESC
	typedef struct Tag_PhysXDesc_Static
	{
		CTransform*			mTrnasform = nullptr;
		E_GEOMAT_TYPE		eShapeType = E_GEOMAT_BOX;
		_bool				bTrigger = false;

	}PHYSXDESC_STATIC;


	typedef struct Tag_PhysXDesc_Dynamic
	{
		CTransform*			mTrnasform = nullptr;
		E_GEOMAT_TYPE		eShapeType = E_GEOMAT_BOX;
		_float3				mVelocity = _float3::Zero();
		_bool				bTrigger = false;

	}PHYSXDESC_DYNAMIC;


	typedef struct Tag_PhysXDesc_Joint
	{
		string*				mBoneNames = nullptr;
		_uint				mLength = 0;
		CModel*				mAttachModel = nullptr;

		E_GEOMAT_TYPE		eShapeType = E_GEOMAT_BOX;
		_float3				mScale = _float3::Zero();
		_float				mSeparation = 2;



	}PHYSXDESC_JOINT;



protected:
	explicit CCollider_PhysX_Base(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext);
	explicit CCollider_PhysX_Base(const CCollider_PhysX_Base& rhs);
	virtual ~CCollider_PhysX_Base() = default;

public:
	HRESULT Initialize_Prototype(void * pArg);
	HRESULT Initialize_Clone(void * pArg);

	virtual HRESULT Update_BeforeSimulation();
	virtual HRESULT Update_AfterSimulation();

public:
#ifdef _DEBUG
	virtual HRESULT Render() override;
	HRESULT CCollider_PhysX_Base::RenderShape(const PxGeometryHolder & h, const PxMat44& world, XMVECTORF32 color = DirectX::Colors::White);
#endif // _DEBUG

public:
	PxRigidActor*	Get_ColliderActor() const { return mMain_Actor; }
	void			Set_Postiotn(_float3 positiotn);
	void			Set_Transform(CTransform* trans) { mMainTransform = trans; };
	void			Set_PhysXUpdate(_bool b) { mbPhysXUpdate = b; };

	E_PHYTYPE		Get_PhysX_ID()const { return mePhysX_ID; }
	HRESULT			Add_Shape(PxGeometry& gemo, PxTransform trans = PxTransform());

public:
	HRESULT CreateDynamicActor(PxVec3 scale = PxVec3(1, 1, 1));
	HRESULT CreateStaticActor(PxVec3 scale = PxVec3(1, 1, 1));
	HRESULT CCollider_PhysX_Base::CreateChain(ATTACHEDESC attach, PxU32 length, const PxGeometry & g, PxReal separation, JointCreateFunction createJoint);



protected:
	//	CTransform*					mTransform = nullptr;
	PxRigidActor*					mMain_Actor= nullptr;
	PxTransform						mPxMainTransform;
	CTransform*						mMainTransform = nullptr;

protected:
	E_PHYTYPE						mePhysX_ID = E_PHYTYPE_END;
	// 물리충돌 해제
	bool							mbPhysXUpdate = true;
	// 외부 입력 업데이트
	// bool							mbKeyUpdate = false;


#ifdef _DEBUG
private:
	BasicEffect*									m_pBasicEffect = nullptr;
	ID3D11InputLayout*								m_pInputLayout = nullptr;
	PrimitiveBatch<DirectX::VertexPositionColor>*	m_pBatch = nullptr;
#endif // _DEBUG

public:
	virtual CComponent* Clone(void* pArg)=0;
	virtual void Free()override;
};


END