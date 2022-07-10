#include "stdafx.h"
#include "..\public\TestObject_PhysX.h"
#include "..\public\PhysX\Collider_PhysX_Static.h"
#include "..\public\PhysX\Collider_PhysX_Dynamic.h"
#include "..\public\PhysX\Collider_PhysX_Joint.h"


CTestObject_PhysX::CTestObject_PhysX(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CTestObject_PhysX::CTestObject_PhysX(const CTestObject_PhysX & rhs)
	: CGameObject(rhs)
{
}

HRESULT CTestObject_PhysX::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));


	return S_OK;
}

HRESULT CTestObject_PhysX::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));
	
	FAILED_CHECK(SetUp_Components());

	return S_OK;
}




_int CTestObject_PhysX::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)
		return -1;
	
	// 이동 
	if (meModelID == CTestObject_PhysX::MODEL_PLAYER)
	{
		Update_Player(fDeltaTime);
	}
	else if (mCom_ColliderBase->Get_PhysX_ID() == E_PHYTYPE_DYNAMIC)
	{

	}

	else if (mCom_ColliderBase->Get_PhysX_ID() == E_PHYTYPE_JOINT)
	{
		Update_Joint(fDeltaTime);
	}
	else
	{
		if (mCom_ColliderBase)
		{
			mCom_ColliderBase->Update_BeforeSimulation();
		}
	}


	return _int();
}


_int CTestObject_PhysX::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)
		return -1;

	if (mCom_ColliderBase)
		mCom_ColliderBase->Update_AfterSimulation();

	FAILED_CHECK(mCom_Renderer->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
	return _int();
}

_int CTestObject_PhysX::Render()
{
	if (__super::Render() < 0)		
		return -1;
	if (meModelID == MODEL_EMPTY)
	{

	}
	else
	{
		if (mbTrigger == false)
		{
			NULL_CHECK_RETURN(mCom_Model, E_FAIL);

			CGameInstance* pInstance = GetSingle(CGameInstance);
			FAILED_CHECK(mCom_Shader->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
			FAILED_CHECK(mCom_Shader->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));

			FAILED_CHECK(mCom_Transform->Bind_OnShader(mCom_Shader, "g_WorldMatrix"));

			_uint NumMaterial = mCom_Model->Get_NumMaterial();

			for (_uint i = 0; i < NumMaterial; i++)
			{
				for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; j++)
					FAILED_CHECK(mCom_Model->Bind_OnShader(mCom_Shader, i, j, MODLETEXTYPE(j)));

				FAILED_CHECK(mCom_Model->Render(mCom_Shader, 2, i, "g_BoneMatrices"));
			}
		}
	}

#ifdef _DEBUG
	if (mCom_ColliderBase)
		mCom_ColliderBase->Render();
#endif // _DEBUG
	return _int();
}

_int CTestObject_PhysX::LateRender()
{
	if (__super::LateRender() < 0)
		return -1;

	return _int();
}

void CTestObject_PhysX::Set_Postition(_float3 position)
{
	mCom_Transform->Set_MatrixState(CTransform::STATE_POS, position);

}

void CTestObject_PhysX::Set_Scale(_float3 scale)
{
	mCom_Transform->Scaled_All(scale);

}

void CTestObject_PhysX::CollisionPhysX_Trigger(CGameObject * pTriggerObj, _uint id, COLLIDERTYPE_PhysXID eConflictedObjCollisionType)
{
	wstring a = Tag_Object_Prototype((OBJECTPROTOTYPEID)id);
	OutputDebugStringW(a.c_str());
	OutputDebugStringW(L"TriggerObjectOnOnON\n");

}

void CTestObject_PhysX::CollisionPhysX_Rigid(CGameObject * pOtherObject, _uint id, COLLIDERTYPE_PhysXID eConflictedObjCollisionType)
{
	wstring a = Tag_Object_Prototype((OBJECTPROTOTYPEID)id);
	OutputDebugStringW(a.c_str());
	OutputDebugStringW(L"RigidObjectOnOnON\n");
}

HRESULT CTestObject_PhysX::Set_ColSetID(E_PHYTYPE id)
{
	// Collider Set;
	CCollider_PhysX_Base::CColider_PX_Desc desc;
	desc.mObjid = Prototype_Player;
	if (id == E_PHYTYPE_DYNAMIC)
	{
		FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider_PhysX_Dynamic), TAG_COM(Com_Collider_PhysX), (CComponent**)&mCom_ColliderBase, &desc));
		mCom_ColliderBase->Set_ObjectID((OBJECTPROTOTYPEID)1);
		return S_OK;
	}
	if (id == E_PHYTYPE_STATIC)
	{
		FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider_PhysX_Static), TAG_COM(Com_Collider_PhysX), (CComponent**)&mCom_ColliderBase, &desc));
		mCom_ColliderBase->Set_ObjectID((OBJECTPROTOTYPEID)1);
		return S_OK;
	}
	if (id == E_PHYTYPE_JOINT)
	{
		FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider_PhysX_Joint), TAG_COM(Com_Collider_PhysX), (CComponent**)&mCom_ColliderBase, &desc));
		mCom_ColliderBase->Set_ObjectID((OBJECTPROTOTYPEID)1);
		return S_OK;
	}

	return E_FAIL;
}

HRESULT CTestObject_PhysX::Set_ModelSetting(E_MODEL id)
{
	if (mCom_Model)
		return E_FAIL;

	meModelID = id;
	switch (id)
	{

	case Client::CTestObject_PhysX::MODEL_GEMETRY:
		FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_MONSTER_BULLET(Prototype_Mesh_Monster_Bullet_Vayusura_Leader), TAG_COM(Com_Model), (CComponent**)&mCom_Model));
		FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VNAM), TAG_COM(Com_Shader), (CComponent**)&mCom_Shader));

		// For.Test
		mCom_ColliderBase->Set_ObjectID((OBJECTPROTOTYPEID)1);

		return S_OK;
		break;
	case Client::CTestObject_PhysX::MODEL_PLAYER:
		FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Mesh_Player), TAG_COM(Com_Model), (CComponent**)&mCom_Model));
		FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VAM), TAG_COM(Com_Shader), (CComponent**)&mCom_Shader));
		mCom_ColliderBase->Set_ObjectID((OBJECTPROTOTYPEID)2);

		return S_OK;
		break;
	case Client::CTestObject_PhysX::MODEL_STATICMAPOBJ:
		FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Mesh_Player), TAG_COM(Com_Model), (CComponent**)&mCom_Model));
		FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VNAM), TAG_COM(Com_Shader), (CComponent**)&mCom_Shader));
		mCom_ColliderBase->Set_ObjectID((OBJECTPROTOTYPEID)3);

		return S_OK;
		break;
	case Client::CTestObject_PhysX::MODEL_MONSTER:
		FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Mesh_Boss_Rangda_Finger), TAG_COM(Com_Model), (CComponent**)&mCom_Model));
		FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VAM), TAG_COM(Com_Shader), (CComponent**)&mCom_Shader));
		mCom_ColliderBase->Set_ObjectID((OBJECTPROTOTYPEID)4);

		return S_OK;
		break;
	case Client::CTestObject_PhysX::MODEL_EMPTY:
		mCom_Model = nullptr;
		return S_OK;

	default:
		break;
	}
	return E_FAIL;
}

HRESULT CTestObject_PhysX::Set_Kinect(_bool b)
{
	((CCollider_PhysX_Dynamic*)mCom_ColliderBase)->Set_Kinecmatic(b);
	return S_OK;
}

HRESULT CTestObject_PhysX::Set_PlayerMeshair()
{
	// #HAIR
	//	Hair PhysX Test
	NULL_CHECK_BREAK(mCom_ColliderHair);

	// skd_hair01 skd_hair02 skd_hair03 skd_hair04 skd_hair05 skd_hair06 skd_hair07 skd_hairEnd
	// m_pModel->Find_HierarchyNode("skd_hair01");
	// m_pModel->Find_HierarchyNode("skd_hairEnd");


//	CCollider_PhysX_Base::PHYSXDESC_JOINT createJoint;
//	string strs[8] =
//	{
//		"skd_hair01","skd_hair02","skd_hair03","skd_hair04","skd_hair05",
//		"skd_hair06","skd_hair07","skd_hairEnd"
//	};
//
//	createJoint.mBoneNames = strs;
//	createJoint.mLength = 8;
//	createJoint.mAttachModel = m_pModel;
//	createJoint.eShapeType = E_GEOMAT_SPEHE;
//	createJoint.mGameObject = this;
//	createJoint.mScale = _Sfloat3::One * 2;
//	createJoint.mSeparation = 2;
//
//	static_cast<CCollider_PhysX_Joint*>(m_pCollider_HairPhysX)->Set_ColiiderDesc(createJoint);
//

	//#TEST Dynamic
//	CCollider_PhysX_Base::PHYSXDESC_DYNAMIC createDynamic;
//
//	createDynamic.bTrigger = false;
//	createDynamic.eShapeType = E_GEOMAT_BOX;
//	createDynamic.mTrnasform = m_pTransformCom;
//	static_cast<CCollider_PhysX_Dynamic*>(m_pCollider_HairPhysX)->Set_ColiiderDesc(createDynamic);

	return S_OK;

}

HRESULT CTestObject_PhysX::Update_Player(_double fDeltaTime)
{
	FAILED_CHECK(mCom_Model->Update_AnimationClip(fDeltaTime));

	bool isKey = false;
	if (KEYPRESS(DIK_W))
	{
		isKey = true;
	//	((CCollider_PhysX_Dynamic*)mCom_ColliderBase)->Move(PxVec3(0, 0, 1));
		mCom_Transform->Move_Forward(fDeltaTime);
	}
	if (KEYPRESS(DIK_S))
	{
		isKey = true;
		//	((CCollider_PhysX_Dynamic*)mCom_ColliderBase)->Move(PxVec3(0, 0, -1));
		mCom_Transform->Move_Backward(fDeltaTime);
	}
	if (KEYPRESS(DIK_D))
	{
		isKey = true;
		//	((CCollider_PhysX_Dynamic*)mCom_ColliderBase)->Move(PxVec3(1, 0, 0));
		mCom_Transform->Move_Right(fDeltaTime);
	}
	if (KEYPRESS(DIK_A))
	{
		isKey = true;
		//	((CCollider_PhysX_Dynamic*)mCom_ColliderBase)->Move(PxVec3(-1, 0, 0));
		mCom_Transform->Move_Left(fDeltaTime);
	}

	if (KEYPRESS(DIK_E))
	{
		isKey = true;
		_float3 vec = _float3(0, 10, 0);
		mCom_Transform->Set_MatrixState(CTransform::STATE_POS, vec);;
	}

	((CCollider_PhysX_Dynamic*)mCom_ColliderBase)->Set_KeyDown(isKey);
	mCom_ColliderBase->Update_BeforeSimulation();

	return S_OK;
}

HRESULT CTestObject_PhysX::Update_Static(_double time)
{
	return E_NOTIMPL;
}

HRESULT CTestObject_PhysX::Update_Dynamic(_double fDeltaTime)
{
	return E_NOTIMPL;
}

HRESULT CTestObject_PhysX::Update_Joint(_double fDeltaTime)
{
	bool isKey = false;
	if (KEYPRESS(DIK_W))
	{
		isKey = true;
		mCom_Transform->Move_Forward(fDeltaTime);
	}
	if (KEYPRESS(DIK_S))
	{
		isKey = true;
		mCom_Transform->Move_Backward(fDeltaTime);
	}
	if (KEYPRESS(DIK_D))
	{
		isKey = true;
		mCom_Transform->Move_Right(fDeltaTime);
	}
	if (KEYPRESS(DIK_A))
	{
		isKey = true;
		mCom_Transform->Move_Left(fDeltaTime);
	}

	mCom_ColliderBase->Update_BeforeSimulation();

	return S_OK;
}




HRESULT CTestObject_PhysX::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&mCom_Renderer));


	// 모델과 충돌 컴포넌트는 후 지정
	mCom_Shader = nullptr;
	mCom_Model = nullptr;
	mCom_ColliderBase = nullptr;
	CTransform::TRANSFORMDESC tDesc = {};

	tDesc.fMovePerSec = 5;
	tDesc.fRotationPerSec = XMConvertToRadians(60);
	tDesc.fScalingPerSec = 1.f;
	tDesc.vPivot = _float3(0, 0, 0);

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&mCom_Transform, &tDesc));

	return S_OK;
}

void CTestObject_PhysX::Set_ChainTest()
{
	// static _float3 position = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS);
	// position.y += 10;
	// m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, position);
	// 
	// // Chain Test
	// m_Com_ColliderJoint->CreateChain(PxTransform(FLOAT3TOPXVEC3(position)) , 10, PxSphereGeometry(1),2, CCollider_PhysX_Base::CreateLimitedSpherical);

}


CTestObject_PhysX * CTestObject_PhysX::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CTestObject_PhysX*	pInstance = new CTestObject_PhysX(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		DEBUGBREAK;
		Safe_Release(pInstance);
	}
	return pInstance;
}

CTestObject_PhysX * CTestObject_PhysX::Clone(void * pArg)
{
	CTestObject_PhysX*	pInstance = new CTestObject_PhysX(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		DEBUGBREAK;
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTestObject_PhysX::Free()
{
	__super::Free();


	Safe_Release(mCom_Transform);
	Safe_Release(mCom_Renderer);
	Safe_Release(mCom_Shader);
	Safe_Release(mCom_Model);

	Safe_Release(mCom_ColliderBase);
	Safe_Release(mCom_ColliderHair);

}
