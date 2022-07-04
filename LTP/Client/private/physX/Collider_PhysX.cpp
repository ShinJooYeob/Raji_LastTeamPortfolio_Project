#include "stdafx.h"
#include "..\Public\PhysX\Collider_PhysX.h"
#include "DebugDraw.h"


#define  MAX_NUM_ACTOR_SHAPES 128
CCollider_PhysX::CCollider_PhysX(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CComponent(pDevice, pDeviceContext)
{
}

CCollider_PhysX::CCollider_PhysX(const CCollider_PhysX & rhs)
	: CComponent(rhs),
#ifdef _DEBUG
	m_pBasicEffect(rhs.m_pBasicEffect),
	m_pBatch(rhs.m_pBatch),
	m_pInputLayout(rhs.m_pInputLayout)
#endif // _DEBUG
{
#ifdef _DEBUG
	Safe_AddRef(m_pInputLayout);
#endif // _DEBUG

	// 액터 복사 

	//for (_uint i = 0; i < BOUNDING_END; ++i)
	//{
	//	switch (m_eColliderType)
	//	{
	//	case Engine::COLLIDER_AABB:
	//		if (nullptr != rhs.m_pAABB[BOUNDING_ORIGINAL])
	//			m_pAABB[i] = new BoundingBox(*rhs.m_pAABB[BOUNDING_ORIGINAL]);
	//		break;
	//	case Engine::COLLIDER_OBB:
	//		if (nullptr != rhs.m_pOBB[BOUNDING_ORIGINAL])
	//			m_pOBB[i] = new BoundingOrientedBox(*rhs.m_pOBB[BOUNDING_ORIGINAL]);
	//		break;
	//	case Engine::COLLIDER_SPHERE:
	//		if (nullptr != rhs.m_pSphere[BOUNDING_ORIGINAL])
	//			m_pSphere[i] = new BoundingSphere(*rhs.m_pSphere[BOUNDING_ORIGINAL]);
	//		break;
	//	default:
	//		break;
	//	}
	//}
}


HRESULT CCollider_PhysX::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;

	return S_OK;
}

HRESULT CCollider_PhysX::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	mRigActor = nullptr;


#ifdef _DEBUG
	m_pBasicEffect = new BasicEffect(m_pDevice);
	m_pBasicEffect->SetVertexColorEnabled(true);

	const void* pShaderByteCode = nullptr;
	size_t		iShaderByteCodeLength = 0;

	m_pBasicEffect->GetVertexShaderBytecode(&pShaderByteCode, &iShaderByteCodeLength);

	FAILED_CHECK(m_pDevice->CreateInputLayout(DirectX::VertexPositionColor::InputElements, DirectX::VertexPositionColor::InputElementCount,
		pShaderByteCode, iShaderByteCodeLength, &m_pInputLayout));

	NULL_CHECK_RETURN(m_pInputLayout, E_FAIL);

	m_pBatch = new PrimitiveBatch<DirectX::VertexPositionColor>(m_pDeviceContext);
	NULL_CHECK_RETURN(m_pBatch, E_FAIL);

#endif // _DEBUG


	return S_OK;
}


HRESULT CCollider_PhysX::Update_Trans2Px()
{
	// Transform 위치 PX로 업데이트
//	mPxTransform = PxTransform(*(PxVec3*)&mTransform->Get_MatrixState_Float3(CTransform::STATE_POS));
	return S_OK;
}

HRESULT CCollider_PhysX::Update_Px2Trans()
{
	//mPxTransform = mRigActor->getGlobalPose();
	//_float3 vec3 = *(_float3*)&mPxTransform.p;
	//mTransform->Set_MatrixState(CTransform::STATE_POS, vec3);

	return S_OK;
}

HRESULT CCollider_PhysX::Set_PxScale(PxVec3 pxvec)
{
	return S_OK;
}

HRESULT CCollider_PhysX::CreateDynamicActor()
{
	if (mRigActor)
		return S_FALSE;

	mRigActor = GetSingle(CPhysXMgr)->CreateDynamic_BaseActor(mPxTransform, PxSphereGeometry(1.0f));

	return S_OK;
}

HRESULT CCollider_PhysX::CreateStaticActor()
{
	if (mRigActor)
		return S_FALSE;

	mRigActor = GetSingle(CPhysXMgr)->CreateDynamic_BaseActor(mPxTransform, PxSphereGeometry(1.0f));

	return S_OK;

}
HRESULT CCollider_PhysX::Add_Shape(PxGeometry& gemo)
{
	if (mRigActor)
	{	
		PxShape* shape = CPhysXMgr::gPhysics->createShape(gemo, *CPhysXMgr::gMaterial);
		NULL_CHECK_BREAK(shape);
		mRigActor->attachShape(*shape);
	}

	return S_OK;
}

#ifdef _DEBUG

HRESULT CCollider_PhysX::Render()
{
	if (mRigActor)
		return E_FAIL;

	// 모양에 따라 드로잉
	m_pDeviceContext->GSSetShader(nullptr, nullptr, 0);
	m_pDeviceContext->IASetInputLayout(m_pInputLayout);
	m_pBasicEffect->SetWorld(XMMatrixIdentity());

	m_pBasicEffect->SetView(GetSingle(CGameInstance)->Get_Transform_Matrix(PLM_VIEW));
	m_pBasicEffect->SetProjection(GetSingle(CGameInstance)->Get_Transform_Matrix(PLM_PROJ));
	m_pBasicEffect->Apply(m_pDeviceContext);

	m_pBatch->Begin();

	bool sleeping = mRigActor->is<PxRigidDynamic>() ? mRigActor->is<PxRigidDynamic>()->isSleeping() : false;

	if (sleeping)
		return S_FALSE;

	PxShape* shapes[MAX_NUM_ACTOR_SHAPES];
	const PxU32 numShapes = mRigActor->getNbShapes();
	if (numShapes >= MAX_NUM_ACTOR_SHAPES)
		return E_FAIL;

	mRigActor->getShapes(shapes, numShapes);

	// 모양 마다 그려준다.
	for (PxU32 j = 0; j < numShapes; j++)
	{
		const PxMat44 shpaeWorld(PxShapeExt::getGlobalPose(*shapes[j], *mRigActor));
		const PxGeometryHolder h = shapes[j]->getGeometry();
		RenderShape(h, shpaeWorld);
	}

	//switch (m_eColliderType)
	//{
	//case Engine::COLLIDER_AABB:
	//	DX::Draw(m_pBatch, *m_pAABB[BOUNDING_TRANSFORM], vColliderColor);
	//	break;
	//case Engine::COLLIDER_OBB:
	//	DX::Draw(m_pBatch, *m_pOBB[BOUNDING_TRANSFORM], vColliderColor);
	//	break;
	//case Engine::COLLIDER_SPHERE:
	//	DX::Draw(m_pBatch, *m_pSphere[BOUNDING_TRANSFORM], vColliderColor);
	//	break;
	//default:
	//	break;
	//}

	m_pBatch->End();


	return S_OK;
}

HRESULT CCollider_PhysX::RenderShape(const PxGeometryHolder & h, const PxMat44& world, const PxVec4& color)
{
	const PxGeometry& geom = h.any();
	_float3 worldpos = *(_float3*)&world.getPosition();

	switch (geom.getType())
	{
	case PxGeometryType::eSPHERE:
	{
		const PxSphereGeometry& sphereGeom = static_cast<const PxSphereGeometry&>(geom);
		BoundingSphere s = BoundingSphere(worldpos, sphereGeom.radius);
		DX::Draw(m_pBatch, s);
		break;
	}

	case PxGeometryType::eBOX:
	{
		const PxBoxGeometry& boxGeom = static_cast<const PxBoxGeometry&>(geom);
		_float3 vec3 = _float3(boxGeom.halfExtents.x, boxGeom.halfExtents.y, boxGeom.halfExtents.z);
		BoundingBox box = BoundingBox(worldpos, vec3);
		DX::Draw(m_pBatch, box);
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
#endif

CCollider_PhysX * CCollider_PhysX::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{

	CCollider_PhysX* pInstance = new CCollider_PhysX(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Create Transform Prototype");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CCollider_PhysX::Clone(void * pArg)
{
	CCollider_PhysX* pInstance = new CCollider_PhysX((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Create Transform");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCollider_PhysX::Free()
{
	__super::Free();


}
