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

//enum CollisionTypeID
//{
//	CollisionType_Player,
//	CollisionType_PlayerWeapon,
//	CollisionType_Monster,
//	CollisionType_MonsterWeapon,
//	CollisionType_NPC,
//	CollisionType_DynaicObject,
//	CollisionType_Terrain,
//	CollisionType_END
//};




class CCollider_PhysX_Base : public CComponent
{
public:
	struct CColider_PX_Desc
	{
		OBJECTPROTOTYPEID mObjid = Object_Prototype_End;

	};

public:
	// 각각 생성하는 콜라이더에 따른 DESC
	typedef struct Tag_PhysXDesc_Static
	{
		CTransform*			mTrnasform = nullptr;
		CGameObject*		mGameObect = nullptr;
		E_GEOMAT_TYPE		eShapeType = E_GEOMAT_BOX;
		_bool				bTrigger = false;

	}PHYSXDESC_STATIC;


	typedef struct Tag_PhysXDesc_Dynamic
	{
		CTransform*			mTrnasform = nullptr;
		CGameObject*		mGameObect = nullptr;
		E_GEOMAT_TYPE		eShapeType = E_GEOMAT_BOX;
		_float3				mVelocity = _float3::Zero();

	}PHYSXDESC_DYNAMIC;


	typedef struct Tag_PhysXDesc_Joint
	{
		string*				mBoneNames = nullptr;
		_uint				mLength = 0;
		CModel*				mAttachModel = nullptr;
		CGameObject*		mGameObject = nullptr;

		E_GEOMAT_TYPE		eShapeType = E_GEOMAT_BOX;
		_float3				mScale = _float3::One();
		_float				mSeparation = 2;

	}PHYSXDESC_JOINT;

	//typedef struct Tag_PhysXDesc_JointHAIR
	//{
	//	string				mActorBone;
	//	string*				mBones = nullptr;
	//	_uint				mLength = 0;
	//	CModel*				mAttachModel = nullptr;
	//	CGameObject*		mGameObject = nullptr;

	//	E_GEOMAT_TYPE		eShapeType = E_GEOMAT_BOX;
	//	_float3				mScale = _float3::One();
	//	_float				mSeparation = 2;

	//}PHYSXDESC_JOINT_HAIR;



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
	HRESULT RenderShape(const PxGeometryHolder & h, const PxMat44& world, XMVECTORF32 color = DirectX::Colors::White);
	HRESULT RenderDebugSphere(_float4x4 mat, _float scale, XMVECTORF32 color);
	HRESULT RenderBuffer(E_GEOMAT_TYPE e, const PxMat44& world, XMVECTORF32 color = DirectX::Colors::White);
#endif // _DEBUG

public:
	PxRigidActor*	Get_ColliderActor() const { return mMain_Actor; }
	void			Set_Transform(CTransform* trans);
	HRESULT			Set_ActorFlag(PxActorFlag::Enum e, bool b);

	// 테스트용 
	void			Set_ObjectID(OBJECTPROTOTYPEID e) { mColDesc.mObjid = e; };

	E_PHYTYPE		Get_PhysX_ID()const { return mePhysX_ID; }
	HRESULT			Add_Shape(PxGeometry& gemo, PxTransform trans = PxTransform());

	PxGeometry*			Create_Geometry(E_GEOMAT_TYPE e,_float3 scale);
	HRESULT				Change_GeoMetry(PxShape* shape, const PxGeometry& geo, _float3 scale);
	HRESULT				Set_GeoMatScale(PxShape* shape, PxVec3 scale);
	PxVec3				Get_Scale_MainTrans();
	void				Set_Scale_MainTrans(_float4 f);
	CGameObject*		Get_GameObject() { return mMainGameObject; };
	OBJECTPROTOTYPEID	Get_ObjectID() { return mColDesc.mObjid; };

	HRESULT			CreateNewShape(PxGeometry* gemo);

	PxVec3			GetScale(PxMat44 mat);
	PxTransform		GetPxTransform(PxMat44 mat);

protected:
	// 충돌체 메인 Actor
	PxRigidActor*					mMain_Actor= nullptr; 
	// 충돌체 위치 / 연결된 위치 저장
	PxMat44							mPxMainMatrix4x4;
	PxVec3							mScale = PxVec3(1,1,1);
	CTransform*						mMainTransform = nullptr;
	CGameObject*					mMainGameObject = nullptr;
	PxShape*						mMainShape = nullptr;

protected:
	E_PHYTYPE						mePhysX_ID = E_PHYTYPE_END;
	// 콜라이더의 오브젝트 아이디
	CColider_PX_Desc				mColDesc;


	PxPhysics*						mPhysics = nullptr;
	PxMaterial*						mNormalMaterial = nullptr;
	PxScene*						mScene = nullptr;


#ifdef _DEBUG
protected:
	BasicEffect*									m_pBasicEffect = nullptr;
	ID3D11InputLayout*								m_pInputLayout = nullptr;
	PrimitiveBatch<DirectX::VertexPositionColor>*	m_pBatch = nullptr;
	XMVECTORF32 mRenderColor = DirectX::Colors::Black;

#endif // _DEBUG

public:
	virtual CComponent* Clone(void* pArg)=0;
	virtual void Free()override;
};


END