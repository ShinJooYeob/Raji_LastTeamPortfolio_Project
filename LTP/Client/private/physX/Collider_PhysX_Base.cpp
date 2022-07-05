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
#ifdef _DEBUG
	Safe_AddRef(m_pInputLayout);
#endif // _DEBUG
}


HRESULT CCollider_PhysX_Base::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;

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

HRESULT CCollider_PhysX_Base::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	mMain_Actor = nullptr;

	return S_OK;
}


HRESULT CCollider_PhysX_Base::Update_BeforeSimulation()
{

	// 시뮬레이션 전의 충돌체 위치
	// 현재 오브젝트 위치를 받아온다.
	if (mbPhysXUpdate == false)
		return S_OK;

	if (mMainTransform == nullptr)
		return E_FAIL;

		mPxMainTransform = PxTransform(*(PxVec3*)&mMainTransform->Get_MatrixState_Float3(CTransform::STATE_POS));

	//if (mePhysxType == E_PHYSXTYPE_JOINT)
	//{
	//	_float4x4 mat =  mAttachDesc.Caculate_AttachedBoneMatrix();
	//	mPxTransform = PxTransform(MAT4X4TOPXMAT(mat));
	//}

	return S_OK;
}

HRESULT CCollider_PhysX_Base::Update_AfterSimulation()
{
	if (mbPhysXUpdate == false)
		return S_OK;

	if (mMainTransform == nullptr)
		return E_FAIL;

	// 시뮬레이션 후 충돌체를 업데이트 해준다.
	//if (mbKeyUpdate)
	//{
	//	
	//	mPxTransform = PxTransform(*(PxVec3*)&Trans->Get_MatrixState_Float3(CTransform::STATE_POS));
	//	mRigActor->setGlobalPose(mPxTransform);
	//}

		// Pos
		mPxMainTransform = mMain_Actor->getGlobalPose();
		_float3 vec3 = *(_float3*)&mPxMainTransform.p;
		mMainTransform->Set_MatrixState(CTransform::STATE_POS, vec3);
	

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

void CCollider_PhysX_Base::Set_Postiotn(_float3 positiotn)
{
	mMain_Actor->setGlobalPose(PxTransform(FLOAT3TOPXVEC3(positiotn)));
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
	XMVECTORF32 color = DirectX::Colors::Cyan;

	for (PxU32 j = 0; j < numShapes; j++)
	{
		const PxMat44 shpaeWorld(PxShapeExt::getGlobalPose(*shapes[j], *mMain_Actor));
		const PxGeometryHolder h = shapes[j]->getGeometry();
		RenderShape(h, shpaeWorld, color);
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
		BoundingSphere s = BoundingSphere(worldpos, sphereGeom.radius);
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
#endif

void CCollider_PhysX_Base::Free()
{
	__super::Free();

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
		
		auto scene = GetSingle(CPhysXMgr)->Get_PhysicsScene();
		if (scene)
		{
			mMain_Actor->release();
			mMain_Actor = nullptr;
		}
	}


}
