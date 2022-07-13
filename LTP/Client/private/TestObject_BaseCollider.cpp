#include "stdafx.h"
#include "..\public\TestObject_BaseCollider.h"


CTestObject_BaseCollider::CTestObject_BaseCollider(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CTestObject_BaseCollider::CTestObject_BaseCollider(const CTestObject_BaseCollider & rhs)
	: CGameObject(rhs)
{
}

HRESULT CTestObject_BaseCollider::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));


	return S_OK;
}

HRESULT CTestObject_BaseCollider::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));
	
	FAILED_CHECK(SetUp_Components());

	return S_OK;
}




_int CTestObject_BaseCollider::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)
		return -1;
	
	// 이동 
	


	return _int();
}


_int CTestObject_BaseCollider::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)
		return -1;



	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
	return _int();
}

_int CTestObject_BaseCollider::Render()
{
	if (__super::Render() < 0)		
		return -1;
	






#ifdef _DEBUG
	//if (mCom_ColliderBase)
	//	mCom_ColliderBase->Render();
#endif // _DEBUG
	return _int();
}

_int CTestObject_BaseCollider::LateRender()
{
	if (__super::LateRender() < 0)
		return -1;

	return _int();
}


HRESULT CTestObject_BaseCollider::Set_ColSetID(E_TESTMODEL id)
{
	// Collider Set;
	//CCollider_PhysX_Base::CColider_PX_Desc desc;
	//desc.mObjid = Prototype_Player;
	//if (id == E_PHYTYPE_DYNAMIC)
	//{
	//	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider_PhysX_Dynamic), TAG_COM(Com_Collider_PhysX), (CComponent**)&mCom_ColliderBase, &desc));
	//	mCom_ColliderBase->Set_ObjectID((OBJECTPROTOTYPEID)1);
	//	return S_OK;
	//}
	//if (id == E_PHYTYPE_STATIC)
	//{
	//	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider_PhysX_Static), TAG_COM(Com_Collider_PhysX), (CComponent**)&mCom_ColliderBase, &desc));
	//	mCom_ColliderBase->Set_ObjectID((OBJECTPROTOTYPEID)1);
	//	return S_OK;
	//}
	//if (id == E_PHYTYPE_JOINT)
	//{
	//	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider_PhysX_Joint), TAG_COM(Com_Collider_PhysX), (CComponent**)&mCom_ColliderBase, &desc));
	//	mCom_ColliderBase->Set_ObjectID((OBJECTPROTOTYPEID)1);
	//	return S_OK;
	//}

	return E_FAIL;
}

HRESULT CTestObject_BaseCollider::Set_ModelSetting(E_TESTMODEL id)
{
	//if (mCom_Model)
	//	return E_FAIL;

	//meModelID = id;
	//switch (id)
	//{

	//case Client::CTestObject_BaseCollider::MODEL_GEMETRY:
	//	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_MONSTER_BULLET(Prototype_Mesh_Monster_Bullet_Vayusura_Leader), TAG_COM(Com_Model), (CComponent**)&mCom_Model));
	//	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VNAM), TAG_COM(Com_Shader), (CComponent**)&mCom_Shader));

	//	// For.Test
	//	mCom_ColliderBase->Set_ObjectID((OBJECTPROTOTYPEID)1);

	//	return S_OK;
	//	break;
	//case Client::CTestObject_BaseCollider::MODEL_PLAYER:
	//	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Mesh_Player), TAG_COM(Com_Model), (CComponent**)&mCom_Model));
	//	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VAM), TAG_COM(Com_Shader), (CComponent**)&mCom_Shader));
	//	mCom_ColliderBase->Set_ObjectID((OBJECTPROTOTYPEID)2);

	//	return S_OK;
	//	break;
	//case Client::CTestObject_BaseCollider::MODEL_STATICMAPOBJ:
	//	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Mesh_Player), TAG_COM(Com_Model), (CComponent**)&mCom_Model));
	//	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VNAM), TAG_COM(Com_Shader), (CComponent**)&mCom_Shader));
	//	mCom_ColliderBase->Set_ObjectID((OBJECTPROTOTYPEID)3);

	//	return S_OK;
	//	break;
	//case Client::CTestObject_BaseCollider::MODEL_MONSTER:
	//	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Mesh_Boss_Rangda_Finger), TAG_COM(Com_Model), (CComponent**)&mCom_Model));
	//	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VAM), TAG_COM(Com_Shader), (CComponent**)&mCom_Shader));
	//	mCom_ColliderBase->Set_ObjectID((OBJECTPROTOTYPEID)4);

	//	return S_OK;
	//	break;
	//case Client::CTestObject_BaseCollider::MODEL_EMPTY:
	//	mCom_Model = nullptr;
	//	return S_OK;

	//default:
	//	break;
	//}
	return E_FAIL;
}


HRESULT CTestObject_BaseCollider::Update_Player(_double fDeltaTime)
{
	//FAILED_CHECK(mCom_Model->Update_AnimationClip(fDeltaTime));

	//bool isKey = false;
	//if (KEYPRESS(DIK_W))
	//{
	//	isKey = true;
	////	((CCollider_PhysX_Dynamic*)mCom_ColliderBase)->Move(PxVec3(0, 0, 1));
	//	mCom_Transform->Move_Forward(fDeltaTime);
	//}
	//if (KEYPRESS(DIK_S))
	//{
	//	isKey = true;
	//	//	((CCollider_PhysX_Dynamic*)mCom_ColliderBase)->Move(PxVec3(0, 0, -1));
	//	mCom_Transform->Move_Backward(fDeltaTime);
	//}
	//if (KEYPRESS(DIK_D))
	//{
	//	isKey = true;
	//	//	((CCollider_PhysX_Dynamic*)mCom_ColliderBase)->Move(PxVec3(1, 0, 0));
	//	mCom_Transform->Move_Right(fDeltaTime);
	//}
	//if (KEYPRESS(DIK_A))
	//{
	//	isKey = true;
	//	//	((CCollider_PhysX_Dynamic*)mCom_ColliderBase)->Move(PxVec3(-1, 0, 0));
	//	mCom_Transform->Move_Left(fDeltaTime);
	//}

	//if (KEYPRESS(DIK_E))
	//{
	//	isKey = true;
	//	_float3 vec = _float3(0, 10, 0);
	//	mCom_Transform->Set_MatrixState(CTransform::STATE_POS, vec);;
	//}

	//((CCollider_PhysX_Dynamic*)mCom_ColliderBase)->Set_KeyDown(isKey);
	//mCom_ColliderBase->Update_BeforeSimulation();

	return S_OK;
}

HRESULT CTestObject_BaseCollider::Update_Static(_double time)
{
	return S_OK;
}

HRESULT CTestObject_BaseCollider::Update_Dynamic(_double fDeltaTime)
{
	return S_OK;
}





HRESULT CTestObject_BaseCollider::SetUp_Components()
{
	//FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&mCom_Renderer));


	//// 모델과 충돌 컴포넌트는 후 지정
	//mCom_Shader = nullptr;
	//mCom_Model = nullptr;
	//mCom_ColliderBase = nullptr;
	//CTransform::TRANSFORMDESC tDesc = {};

	//tDesc.fMovePerSec = 5;
	//tDesc.fRotationPerSec = XMConvertToRadians(60);
	//tDesc.fScalingPerSec = 1.f;
	//tDesc.vPivot = _float3(0, 0, 0);

	//FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&mCom_Transform, &tDesc));

	return S_OK;
}



CTestObject_BaseCollider * CTestObject_BaseCollider::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CTestObject_BaseCollider*	pInstance = new CTestObject_BaseCollider(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		DEBUGBREAK;
		Safe_Release(pInstance);
	}
	return pInstance;
}

CTestObject_BaseCollider * CTestObject_BaseCollider::Clone(void * pArg)
{
	CTestObject_BaseCollider*	pInstance = new CTestObject_BaseCollider(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		DEBUGBREAK;
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTestObject_BaseCollider::Free()
{
	__super::Free();


	//Safe_Release(mCom_Transform);
	//Safe_Release(mCom_Renderer);
	//Safe_Release(mCom_Shader);
	//Safe_Release(mCom_Model);

	//Safe_Release(mCom_ColliderBase);
	//Safe_Release(mCom_ColliderHair);

}
