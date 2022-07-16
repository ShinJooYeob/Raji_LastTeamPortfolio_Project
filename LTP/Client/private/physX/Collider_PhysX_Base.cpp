#include "stdafx.h"
#include "..\Public\PhysX\Collider_PhysX_Base.h"
#include "DebugDraw.h"


#define  MAX_NUM_ACTOR_SHAPES 128


CCollider_PhysX_Base::CCollider_PhysX_Base(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CComponent(pDevice, pDeviceContext)
{
	mePhysX_ID = E_PHYTYPE_END;

}

CCollider_PhysX_Base::CCollider_PhysX_Base(const CCollider_PhysX_Base & rhs)
	: CComponent(rhs), mePhysX_ID(rhs.mePhysX_ID)
#ifdef _DEBUG
	,m_pBasicEffect(rhs.m_pBasicEffect),
	m_pBatch(rhs.m_pBatch),
	m_pInputLayout(rhs.m_pInputLayout)
#endif // _DEBUG
{
	mMain_Actor = nullptr;
#ifdef _DEBUG
	Safe_AddRef(m_pInputLayout);
#endif // _DEBUG
}


HRESULT CCollider_PhysX_Base::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;

#ifdef _DEBUG
	m_pBasicEffect = NEW BasicEffect(m_pDevice);
	m_pBasicEffect->SetVertexColorEnabled(true);

	const void* pShaderByteCode = nullptr;
	size_t		iShaderByteCodeLength = 0;

	m_pBasicEffect->GetVertexShaderBytecode(&pShaderByteCode, &iShaderByteCodeLength);

	FAILED_CHECK(m_pDevice->CreateInputLayout(DirectX::VertexPositionColor::InputElements, DirectX::VertexPositionColor::InputElementCount,
		pShaderByteCode, iShaderByteCodeLength, &m_pInputLayout));

	NULL_CHECK_RETURN(m_pInputLayout, E_FAIL);

	m_pBatch = NEW PrimitiveBatch<DirectX::VertexPositionColor>(m_pDeviceContext);
	NULL_CHECK_RETURN(m_pBatch, E_FAIL);

#endif // _DEBUG


	return S_OK;
}

HRESULT CCollider_PhysX_Base::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	mMain_Actor = nullptr;

	mPhysics = GetSingle(CPhysXMgr)->gPhysics;
	mNormalMaterial = GetSingle(CPhysXMgr)->gMaterial;
	mScene = GetSingle(CPhysXMgr)->Get_PhysicsScene();

	return S_OK;
}


HRESULT CCollider_PhysX_Base::Update_BeforeSimulation()
{
	// 시뮬레이션 전의 충돌체 위치
	// 현재 오브젝트 위치를 받아온다.

	if (mMainGameObject == nullptr)
		return E_FAIL;

	GetSingle(CPhysXMgr)->Add_CollisionObject(this);




	return S_OK;
}

HRESULT CCollider_PhysX_Base::Update_AfterSimulation()
{
	// 시뮬레이션 후의 충돌체 위치
	return S_OK;
}




HRESULT CCollider_PhysX_Base::Add_Shape(PxGeometry& gemo, PxTransform offset)
{
	// 메인 액터에 충돌체 추가
	if (mMain_Actor)
	{
		PxShape* shape = CPhysXMgr::gPhysics->createShape(gemo, *CPhysXMgr::gMaterial);
		shape->setLocalPose(offset);
		NULL_CHECK_BREAK(shape);
		mMain_Actor->attachShape(*shape);
	}

	return S_OK;
}

PxGeometry * CCollider_PhysX_Base::Create_Geometry(E_GEOMAT_TYPE e,_float3 scale)
{
	PxGeometry* gemo = nullptr;

	_float3 halfscale = _float3(scale.x*0.5f, scale.y*0.5f,scale.z*0.5f);

	switch (e)
	{
	case Client::E_GEOMAT_BOX:
		gemo = NEW PxBoxGeometry(FLOAT3TOPXVEC3(halfscale));
		break;
	case Client::E_GEOMAT_SPEHE:
		gemo = NEW PxSphereGeometry(PxReal(halfscale.x));
		break;
	case Client::E_GEOMAT_CAPSULE:
		gemo = NEW PxCapsuleGeometry(PxReal(halfscale.x), PxReal(halfscale.y));
		break;
	case Client::E_GEOMAT_SHAPE:

		break;
	case Client::E_GEOMAT_VERTEX:
		break;
	case Client::E_GEOMAT_TRIANGLE:
		break;
	case Client::E_GEOMAT_END:
		break;
	default:
		break;
	}

	NULL_CHECK_BREAK(gemo);
	return gemo;
}

HRESULT CCollider_PhysX_Base::Change_GeoMetry(PxShape* shape, const PxGeometry& geo, _float3 scale)
{
	shape->setGeometry(geo);
	return S_OK;
}



HRESULT CCollider_PhysX_Base::Set_GeoMatScale(PxShape* shape, PxVec3 scale)
{
	PxGeometryHolder h = shape->getGeometry();

	switch (h.getType())
	{
	case physx::PxGeometryType::eSPHERE:
	{	
		PxSphereGeometry s;
		shape->getSphereGeometry(s);
		s.radius = scale.x;
		shape->setGeometry(s);
	}
	break;
	case physx::PxGeometryType::eCAPSULE:
	{
		PxCapsuleGeometry c;
		shape->getCapsuleGeometry(c);
		c.radius = scale.x;
		c.halfHeight = scale.y;
		shape->setGeometry(c);
	}
	break;
	case physx::PxGeometryType::eBOX:
	{
		PxBoxGeometry b;
		shape->getBoxGeometry(b);
		b.halfExtents = scale;
		shape->setGeometry(b);
	}
		break;
	case physx::PxGeometryType::eCONVEXMESH:
		break;
	case physx::PxGeometryType::eTRIANGLEMESH:
		break;
	case physx::PxGeometryType::eHEIGHTFIELD:
		break;
	case physx::PxGeometryType::eGEOMETRY_COUNT:
		break;
	case physx::PxGeometryType::eINVALID:
		break;
	default:
		break;
	}

	return S_OK;
}

void CCollider_PhysX_Base::Set_Scale_MainTrans(_float4 ff)
{
	mPxMainMatrix4x4.scale(*(PxVec4*)(&ff));
}

PxVec3 CCollider_PhysX_Base::GetScale(PxMat44 mat)
{
	_float x = mat.column0.normalize();
	_float y = mat.column1.normalize();
	_float z = mat.column2.normalize();


	return PxVec3(x,y,z);
}

PxTransform CCollider_PhysX_Base::GetPxTransform(PxMat44 m)
{
	// 스케일값을 제외하고 반환
	PxVec3 p = PxVec3(m.column3.x, m.column3.y, m.column3.z);

	PxVec3 column0 = PxVec3(m.column0.x, m.column0.y, m.column0.z);
	PxVec3 column1 = PxVec3(m.column1.x, m.column1.y, m.column1.z);
	PxVec3 column2 = PxVec3(m.column2.x, m.column2.y, m.column2.z);

	PxQuat q = PxQuat(PxMat33(column0.getNormalized(), column1.getNormalized(), column2.getNormalized()));
	return PxTransform(p,q);
}



PxVec3 CCollider_PhysX_Base::Get_Scale_MainTrans()
{
	return PxVec3(mPxMainMatrix4x4.column0.normalize(), 
		mPxMainMatrix4x4.column1.normalize(),
		mPxMainMatrix4x4.column2.normalize());
}

void CCollider_PhysX_Base::Set_Transform(CTransform * trans)
{
	mMainTransform = trans;
	mPxMainMatrix4x4 = MAT4X4TOPXMAT(mMainTransform->Get_WorldFloat4x4());
	mMain_Actor->setGlobalPose(PxTransform(mPxMainMatrix4x4));
}

HRESULT CCollider_PhysX_Base::Set_ActorFlag(PxActorFlag::Enum e, bool b)
{
	// 시각화
	// eVISUALIZATION = (1 << 0),

	// 중력비활성화
	// eDISABLE_GRAVITY = (1 << 1),

	// 알림이벤트 On Off
	// eSEND_SLEEP_NOTIFIES = (1 << 2),

	// 충돌 비활성화
	// eDISABLE_SIMULATION = (1 << 3)

	mMain_Actor->setActorFlag(e, b);
	return S_OK;
}

#ifdef _DEBUG

HRESULT CCollider_PhysX_Base::Render()
{
	NULL_CHECK_RETURN(mMain_Actor, E_FAIL);

	// 모양에 따라 드로잉
	m_pDeviceContext->GSSetShader(nullptr, nullptr, 0);
	m_pDeviceContext->IASetInputLayout(m_pInputLayout);
	m_pBasicEffect->SetWorld(XMMatrixIdentity());

	m_pBasicEffect->SetView(GetSingle(CGameInstance)->Get_Transform_Matrix(PLM_VIEW));
	m_pBasicEffect->SetProjection(GetSingle(CGameInstance)->Get_Transform_Matrix(PLM_PROJ));
	m_pBasicEffect->Apply(m_pDeviceContext);


	//bool sleeping = mRigActor->is<PxRigidDynamic>() ? mRigActor->is<PxRigidDynamic>()->isSleeping() : false;

	//if (sleeping)
	//	return S_FALSE;

	m_pBatch->Begin();


	PxShape* shapes[MAX_NUM_ACTOR_SHAPES];
	const PxU32 numShapes = mMain_Actor->getNbShapes();
	if (numShapes >= MAX_NUM_ACTOR_SHAPES)
		return E_FAIL;

	mMain_Actor->getShapes(shapes, numShapes);

	// 모양 마다 그려준다.

	for (PxU32 j = 0; j < numShapes; j++)
	{
		const PxMat44 shpaeWorld(PxShapeExt::getGlobalPose(*shapes[j], *mMain_Actor));
		const PxGeometryHolder h = shapes[j]->getGeometry();
		RenderShape(h, shpaeWorld, mRenderColor);
	}
	m_pBatch->End();

	return S_OK;
}


HRESULT CCollider_PhysX_Base::RenderShape(const PxGeometryHolder & h, const PxMat44& world, XMVECTORF32 color)
{
	const PxGeometry& geom = h.any();
	PxTransform worldTrans = PxTransform(world);
	_float3 worldpos = PXVEC3TOFLOAT3(worldTrans.p);


	switch (geom.getType())
	{
	case PxGeometryType::eSPHERE:
	{
		const PxSphereGeometry& sphereGeom = static_cast<const PxSphereGeometry&>(geom);
		BoundingSphere s = BoundingSphere(worldpos, 0.05f);
		DX::Draw(m_pBatch, s, color);
		break;
	}

	case PxGeometryType::eBOX:
	{
		const PxBoxGeometry& boxGeom = static_cast<const PxBoxGeometry&>(geom);
		_float3 halfextents = _float3(boxGeom.halfExtents.x, boxGeom.halfExtents.y, boxGeom.halfExtents.z);
	//	PxQuat q = worldTrans.q;
		_float4 q = *(_float4*)&worldTrans.q;

		BoundingOrientedBox obbBox = BoundingOrientedBox(worldpos, halfextents, q);

		DX::Draw(m_pBatch, obbBox, color);
		break;
	}
	case PxGeometryType::eCAPSULE:
	{
		const PxCapsuleGeometry& capsuleGeom = static_cast<const PxCapsuleGeometry&>(geom);

		BoundingBox box = BoundingBox(worldpos, _float3(capsuleGeom.radius, capsuleGeom.halfHeight, capsuleGeom.radius));
		_float3 upSph1 = worldpos; upSph1.y += capsuleGeom.halfHeight;
		_float3 upSph2 = worldpos; upSph2.y -= capsuleGeom.halfHeight;
		
		BoundingSphere s1 = BoundingSphere(upSph1, capsuleGeom.radius);
		BoundingSphere s2 = BoundingSphere(upSph2, capsuleGeom.radius);
		DX::Draw(m_pBatch, box);
		DX::Draw(m_pBatch, s1);
		DX::Draw(m_pBatch, s2);

	}
	break;

	//case PxGeometryType::eCONVEXMESH:
	//	PxConvexMeshGeometry
	//	break;

	//case PxGeometryType::eTRIANGLEMESH:
	//	PxTriangleMeshGeometry
	//	break;

	default:
		break;

	}

	return S_OK;
}

HRESULT CCollider_PhysX_Base::RenderDebugSphere(_float4x4 mat,_float3 scale, XMVECTORF32 color, PxGeometryType::Enum type)
{
	if (type == PxGeometryType::Enum::eBOX)
	{
		_Sfloat4x4 s = _Sfloat4x4(mat);
		_float x = s.Right().Length();
		_float y = s.Up().Length();
		_float z = s.Backward().Length();

		scale = _float3(x*scale.x, y*scale.y, z*scale.z);
		_Squternion q = _Squternion::CreateFromRotationMatrix(mat);

		BoundingOrientedBox obbBox = BoundingOrientedBox(s.Translation(), scale, q);
		DX::Draw(m_pBatch, obbBox, color);
	}
	if (type == PxGeometryType::Enum::eSPHERE)
	{
		PxTransform worldTrans = PxTransform(MAT4X4TOPXMAT(mat));
		_float3 worldpos = PXVEC3TOFLOAT3(worldTrans.p);
		BoundingSphere s = BoundingSphere(worldpos, scale.x);
		DX::Draw(m_pBatch, s, color);
	}
	
	return S_OK;
}

HRESULT CCollider_PhysX_Base::RenderBuffer(E_GEOMAT_TYPE e, const PxMat44& world, XMVECTORF32 color)
{
	PxTransform pxtrans = PxTransform(world);
	PxVec3		scale = GetScale(world) * 0.5f;

	switch (e)
	{

	case Client::E_GEOMAT_BOX:
	{
		const BoundingOrientedBox& obbBox = BoundingOrientedBox(
			PXVEC3TOFLOAT3(pxtrans.p),
			PXVEC3TOFLOAT3(scale),
			PXVEC4TOFLOAT4(pxtrans.q));
		DX::Draw(m_pBatch, obbBox, color);
	}
		break;
	case Client::E_GEOMAT_SPEHE:
	{
		const BoundingSphere& s = BoundingSphere(
			PXVEC3TOFLOAT3(pxtrans.p),
			scale.x);
		DX::Draw(m_pBatch, s, color);
	}
		break;
	case Client::E_GEOMAT_CAPSULE:
	{
		const BoundingBox& box = BoundingBox(
			PXVEC3TOFLOAT3(world),
			_float3(scale.x, scale.y, scale.z));

		_float3 upSph1 = PXVEC3TOFLOAT3(pxtrans.p); upSph1.y += scale.x;
		_float3 downSph2 = PXVEC3TOFLOAT3(pxtrans.p); downSph2.y -= scale.x;
		const BoundingSphere& s1 = BoundingSphere(
			PXVEC3TOFLOAT3(pxtrans.p),
			scale.x);
		const BoundingSphere& s2 = BoundingSphere(
			PXVEC3TOFLOAT3(pxtrans.p),
			scale.x);
		DX::Draw(m_pBatch, box, color);
		DX::Draw(m_pBatch, s1, color);
		DX::Draw(m_pBatch, s2, color);
	}
		break;
	case Client::E_GEOMAT_SHAPE:
		break;
	case Client::E_GEOMAT_VERTEX:
		break;
	case Client::E_GEOMAT_TRIANGLE:
		break;
	case Client::E_GEOMAT_END:
		break;

	default:
		break;
	}

	return S_OK;


}

#endif

void CCollider_PhysX_Base::Free()
{
	__super::Free();

	if (mePhysX_ID == E_PHYTYPE_JOINT)
		return;

#ifdef _DEBUG
	if (m_bIsClone == false)
	{
		Safe_Delete(m_pBasicEffect);
		Safe_Delete(m_pBatch);
	}

	Safe_Release(m_pInputLayout);

#endif // _DEBUG

	if (m_bIsClone)
	{
		if (mScene)
		{
			if (mMain_Actor)
			{
				mMain_Actor->release();
				mMain_Actor = nullptr;
			}
		}
	}
	//PX_RELEASE(mPhysics);
	//PX_RELEASE(mNormalMaterial);
	//PX_RELEASE(mScene);
	

}
