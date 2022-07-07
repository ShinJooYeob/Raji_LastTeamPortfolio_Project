#include "stdafx.h"
#include "..\Public\PhysX\Collider_PhysX_Static.h"


CCollider_PhysX_Static::CCollider_PhysX_Static(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CCollider_PhysX_Base(pDevice, pDeviceContext)
{
	mePhysX_ID = E_PHYTYPE_STATIC;
}

CCollider_PhysX_Static::CCollider_PhysX_Static(const CCollider_PhysX_Static & rhs)
	: CCollider_PhysX_Base(rhs)
{

}


HRESULT CCollider_PhysX_Static::Initialize_Prototype(void * pArg)
{
	if (FAILED(__super::Initialize_Prototype(pArg)))
		return E_FAIL;


	return S_OK;
}

HRESULT CCollider_PhysX_Static::Initialize_Clone(void * pArg)
{
	if (FAILED(__super::Initialize_Clone(pArg)))
		return E_FAIL;

	
	return S_OK;
}


HRESULT CCollider_PhysX_Static::Update_BeforeSimulation()
{
	

	if (E_STATIC_BUFFER == mStaticID)
	{
		if (mMainTransform == nullptr)
			return E_FAIL;

	}
	else
	{
		if (mbTrigger)
			return S_OK;
		if (mMain_Actor == nullptr || mMainTransform == nullptr)
			return E_FAIL;


		//mPxMainMatrix4x4 = MAT4X4TOPXMAT(mMainTransform->Get_WorldMatrix());
		//mMain_Actor->setGlobalPose(PxTransform(mPxMainMatrix4x4));
	}

	return S_OK;
}

HRESULT CCollider_PhysX_Static::Update_AfterSimulation()
{
	



	if (E_STATIC_BUFFER == mStaticID)
	{
		mPxMainMatrix4x4 = MAT4X4TOPXMAT(mMainTransform->Get_WorldMatrix());

	}
	else
	{
		if (mbTrigger)
			return S_OK;

		// #TODO 모양 새로 만들어야함
		//PxVec3 newScale = Get_Scale_MainTrans();
		////	Set_GeoMatScale(mMainShape, newScale);

		//PxGeometry* gemo = Create_Geometry(mPhysXDesc.eShapeType, newScale);
		//Create_Geometry(*gemo);

		//mMain_Actor->setGlobalPose()
	}
	


	


	

//	mPxMainMatrix4x4 = mMain_Actor->getGlobalPose();
//	mMainTransform->Set_Matrix(PXMATTOMAT4x4(mPxMainMatrix4x4));

	return S_OK;
}


#ifdef _DEBUG
HRESULT CCollider_PhysX_Static::Render()
{
	if (E_STATIC_BUFFER == mStaticID)
	{
		
		// 모양에 따라 드로잉
		m_pDeviceContext->GSSetShader(nullptr, nullptr, 0);
		m_pDeviceContext->IASetInputLayout(m_pInputLayout);
		m_pBasicEffect->SetWorld(XMMatrixIdentity());

		m_pBasicEffect->SetView(GetSingle(CGameInstance)->Get_Transform_Matrix(PLM_VIEW));
		m_pBasicEffect->SetProjection(GetSingle(CGameInstance)->Get_Transform_Matrix(PLM_PROJ));
		m_pBasicEffect->Apply(m_pDeviceContext);

		m_pBatch->Begin();
		// 모양 마다 그려준다.
		XMVECTORF32 color = DirectX::Colors::White;
		RenderBuffer(mPhysXDesc.eShapeType, mPxMainMatrix4x4, color);
		m_pBatch->End();

	}
	else
	{

	FAILED_CHECK(__super::Render());
	}
	return S_OK;
}
#endif

CCollider_PhysX_Static * CCollider_PhysX_Static::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{

	CCollider_PhysX_Static* pInstance = new CCollider_PhysX_Static(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Create Transform Prototype");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CCollider_PhysX_Static::Clone(void * pArg)
{
	CCollider_PhysX_Static* pInstance = new CCollider_PhysX_Static((*this));

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Create Transform");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCollider_PhysX_Static::Free()
{
	__super::Free();

}

HRESULT CCollider_PhysX_Static::Set_ColiiderDesc(PHYSXDESC_STATIC desc)
{
	// 충돌 모델 초기화
	memcpy(&mPhysXDesc, &desc, sizeof(PHYSXDESC_STATIC));


	// 위 정보로 콜라이더 초기화
	// 위치 / 스케일 / 엑터 타입 / 모양을 지정헤서 콜라이더 컴포넌트 생성
	if (mMain_Actor)
		return E_FAIL;

	
	PxGeometry* gemo = nullptr;

	mMainTransform = mPhysXDesc.mTrnasform;

	_float3 scale = mMainTransform->Get_Scale();
	_float3 halfscale = _float3(scale.x*0.5f, scale.y*0.5f, scale.z*0.5f);
	_float3 pos = mMainTransform->Get_MatrixState(CTransform::STATE_POS);

	gemo = Create_Geometry(desc.eShapeType, scale);
	NULL_CHECK_BREAK(gemo);

	mPxMainMatrix4x4 = MAT4X4TOPXMAT(mMainTransform->Get_WorldMatrix());
	mMain_Actor = GetSingle(CPhysXMgr)->CreateStatic_BaseActor(PxTransform(mPxMainMatrix4x4), *gemo);
	NULL_CHECK_BREAK(mMain_Actor);

	PxShape* shapes[1];
	const PxU32 numShapes = 1;
	mMain_Actor->getShapes(shapes, numShapes);
	mMainShape = shapes[0];
	NULL_CHECK_BREAK(mMainShape);

	Safe_Delete(gemo);

	mMain_Actor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
	mMain_Actor->setActorFlag(PxActorFlag::eSEND_SLEEP_NOTIFIES, false);


	return S_OK;
}

HRESULT CCollider_PhysX_Static::Set_ColiiderBufferDesc(PHYSXDESC_STATIC desc)
{
	mMain_Actor = nullptr;
	mStaticID = CCollider_PhysX_Static::E_STATIC_BUFFER;
	memcpy(&mPhysXDesc, &desc, sizeof(PHYSXDESC_STATIC));
	mMainTransform = mPhysXDesc.mTrnasform;
	mPxMainMatrix4x4 = MAT4X4TOPXMAT(mMainTransform->Get_WorldMatrix());
	return S_OK;

}



HRESULT CCollider_PhysX_Static::Set_ActorFlag(PxActorFlag::Enum e, bool b)
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

HRESULT CCollider_PhysX_Static::Set_eDISABLE_SIMULATION(bool b)
{
	mMain_Actor->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, b);
	return S_OK;
}
