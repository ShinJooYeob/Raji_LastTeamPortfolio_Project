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
	//bool isKey = false;
	//if (KEYPRESS(DIK_W))
	//{
	//	isKey = true;
	//	m_pTransformCom->Move_Forward(fDeltaTime);
	//}
	//if (KEYPRESS(DIK_S))
	//{
	//	isKey = true;
	//	m_pTransformCom->Move_Backward(fDeltaTime);
	//}
	//if (KEYPRESS(DIK_D))
	//{
	//	isKey = true;
	//	m_pTransformCom->Move_Right(fDeltaTime);
	//}
	//if (KEYPRESS(DIK_A))
	//{
	//	isKey = true;
	//	m_pTransformCom->Move_Left(fDeltaTime);
	//}


	if (m_Com_ColliderBase)
		m_Com_ColliderBase->Update_BeforeSimulation();

	return _int();
}


_int CTestObject_PhysX::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)
		return -1;

	if (m_Com_ColliderBase)
		m_Com_ColliderBase->Update_AfterSimulation();

	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
	return _int();
}

_int CTestObject_PhysX::Render()
{
	if (__super::Render() < 0)		
		return -1;

	if (mbTrigger == false)
	{

		NULL_CHECK_RETURN(m_pModel, E_FAIL);

		CGameInstance* pInstance = GetSingle(CGameInstance);
		FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
		FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));

		FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));

		_uint NumMaterial = m_pModel->Get_NumMaterial();

		for (_uint i = 0; i < NumMaterial; i++)
		{
			for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; j++)
				FAILED_CHECK(m_pModel->Bind_OnShader(m_pShaderCom, i, j, MODLETEXTYPE(j)));

			FAILED_CHECK(m_pModel->Render(m_pShaderCom, 2, i, "g_BoneMatrices"));
		}
	}

#ifdef _DEBUG
	if (m_Com_ColliderBase)
		m_Com_ColliderBase->Render();
#endif // _DEBUG
	return _int();
}

_int CTestObject_PhysX::LateRender()
{
	if (__super::LateRender() < 0)
		return -1;

	return _int();
}

HRESULT CTestObject_PhysX::Set_ColSetID(E_PHYTYPE id)
{
	// Collider Set;
	if (id == E_PHYTYPE_DYNAMIC)
	{
		FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider_PhysX_Dynamic), TAG_COM(Com_Collider_PhysX), (CComponent**)&m_Com_ColliderBase));
		return S_OK;
	}
	if (id == E_PHYTYPE_STATIC)
	{
		FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider_PhysX_Static), TAG_COM(Com_Collider_PhysX), (CComponent**)&m_Com_ColliderBase));
		return S_OK;
	}
	if (id == E_PHYTYPE_JOINT)
	{
		FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider_PhysX_Joint), TAG_COM(Com_Collider_PhysX), (CComponent**)&m_Com_ColliderBase));
		return S_OK;
	}
	return E_FAIL;
}

//HRESULT CTestObject_PhysX::SetUp_ColliderDesc(CCollider_PhysX_Base::PHYSXDESC desc)
//{
//	if (m_Com_ColliderBase)
//		return E_FAIL;
//	
//	switch (desc.ePhyType)
//	{
//	case E_PHYTYPE_DYNAMIC:
//		FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider_PhysX_Static), TAG_COM(Com_Collider), (CComponent**)&m_Com_ColliderBase));
//
//		break;
//	case E_PHYTYPE_STATIC:
//		FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider_PhysX_Dynamic), TAG_COM(Com_Collider), (CComponent**)&m_Com_ColliderBase));
//
//		break;
//	case E_PHYTYPE_JOINT:
//		FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider_PhysX_Joint), TAG_COM(Com_Collider), (CComponent**)&m_Com_ColliderBase));
//		break;
//	default:
//		break;
//
//	}
//
//	if (m_Com_ColliderBase)
//	{
//		return m_Com_ColliderBase->Setup_PhysXDesc(desc);
//	}
//		
//	return E_FAIL;
//}



HRESULT CTestObject_PhysX::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VNAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_MONSTER_BULLET(Prototype_Mesh_Monster_Bullet_Vayusura_Leader), TAG_COM(Com_Model), (CComponent**)&m_pModel));


	// Static / dynamic / 관절에 적용되는 컴포넌트들 생성
	m_Com_ColliderBase = nullptr;
	//	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider_PhysX_Static), TAG_COM(Com_Collider_PhysX), (CComponent**)&m_Com_ColliderBase));
	//	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider_PhysX_Joint), TAG_COM(Com_Collider_PhysX), (CComponent**)&m_Com_ColliderBase));
	//	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider_PhysX_Dynamic), TAG_COM(Com_Collider_PhysX), (CComponent**)&m_Com_ColliderBase));


	
	CTransform::TRANSFORMDESC tDesc = {};

	tDesc.fMovePerSec = 5;
	tDesc.fRotationPerSec = XMConvertToRadians(60);
	tDesc.fScalingPerSec = 1.f;
	tDesc.vPivot = _float3(0, 0, 0);

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));

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


	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModel);

	Safe_Release(m_Com_ColliderBase);
}
