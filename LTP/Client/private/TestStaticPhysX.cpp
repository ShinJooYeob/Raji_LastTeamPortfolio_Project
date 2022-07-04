#include "stdafx.h"
#include "..\public\TestStaticPhysX.h"
#include "..\public\PhysX\Collider_PhysX.h"


CTestStaticPhysX::CTestStaticPhysX(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CTestStaticPhysX::CTestStaticPhysX(const CTestStaticPhysX & rhs)
	: CGameObject(rhs)
{
}

HRESULT CTestStaticPhysX::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));


	return S_OK;
}

HRESULT CTestStaticPhysX::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));
	
	FAILED_CHECK(SetUp_Components());

	if (pArg != nullptr)
	{
		TESTPHYSXDESC desc;
		memcpy(&desc, pArg, sizeof(TESTPHYSXDESC));

		memcpy(&desc, pArg, sizeof(TESTPHYSXDESC));
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS,desc.pos);

		Set_InitPhyType(desc.ePhyType);
		Set_Trigger(desc.bTrigger);
	}



	return S_OK;
}

HRESULT CTestStaticPhysX::Set_InitPhyType(E_PHYTYPE e)
{
	mePhyType = e;

	switch (mePhyType)
	{
	case Client::CTestStaticPhysX::E_PHYTYPE_TESTBOX:
		Set_StaticBox();
		break;
	case Client::CTestStaticPhysX::E_PHYTYPE_BULLET:
		Set_ChainTest();

		break;
	case Client::CTestStaticPhysX::E_PHYTYPE_END:
		break;
	default:
		break;
	}
	return S_OK;
}

_int CTestStaticPhysX::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)
		return -1;

	// 이동 
	bool isKey = false;
	if (mePhyType == E_PHYTYPE_BULLET)
	{
		if (KEYPRESS(DIK_W))
		{
			isKey = true;
			m_pTransformCom->Move_Forward(fDeltaTime);
		}
		if (KEYPRESS(DIK_S))
		{
			isKey = true;
			m_pTransformCom->Move_Backward(fDeltaTime);
		}
		if (KEYPRESS(DIK_D))
		{
			isKey = true;
			m_pTransformCom->Move_Right(fDeltaTime);
		}
		if (KEYPRESS(DIK_A))
		{
			isKey = true;
			m_pTransformCom->Move_Left(fDeltaTime);
		}

	}
	m_pPhysX->Set_KeyUpdate(isKey);
	m_pPhysX->Update_BeforeSimulation(m_pTransformCom);


	return _int();
}


_int CTestStaticPhysX::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)
		return -1;

	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));

	m_pPhysX->Update_AfterSimulation(m_pTransformCom);

	return _int();
}

_int CTestStaticPhysX::Render()
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
	m_pPhysX->Render();

#endif // _DEBUG
	return _int();
}

_int CTestStaticPhysX::LateRender()
{
	if (__super::LateRender() < 0)
		return -1;

	return _int();
}



HRESULT CTestStaticPhysX::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VNAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_MONSTER_BULLET(Prototype_Mesh_Monster_Bullet_Vayusura_Leader), TAG_COM(Com_Model), (CComponent**)&m_pModel));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider_PhysX), TAG_COM(Com_Collider), (CComponent**)&m_pPhysX));

	
	CTransform::TRANSFORMDESC tDesc = {};

	tDesc.fMovePerSec = 5;
	tDesc.fRotationPerSec = XMConvertToRadians(60);
	tDesc.fScalingPerSec = 1.f;
	tDesc.vPivot = _float3(0, 0, 0);

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));

	return S_OK;
}

void CTestStaticPhysX::Set_StaticBox()
{
	// 충돌체 달기

	_float3 scale = _float3(1,5,5);
	m_pPhysX->CreateStaticActor(FLOAT3TOPXVEC3(scale));


	m_pTransformCom->Scaled_All(scale);
	m_pPhysX->Set_Postiotn((m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS)));

}

void CTestStaticPhysX::Set_DynamicBullet()
{
	_float3 scale = _float3(0.5f, 0.5f, 0.5f);
	m_pPhysX->CreateDynamicActor(FLOAT3TOPXVEC3(scale));
	m_pTransformCom->Scaled_All(scale);

	_float3 position = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS);
	m_pPhysX->Set_Postiotn(position);




}


void CTestStaticPhysX::Set_ChainTest()
{
	_float offsetX = 2.5f;
	static _float3 position = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS);
	position.z += 1;
	position.y += 10;
	m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, position);

	// Chain Test
	m_pPhysX->CreateChain(PxTransform(FLOAT3TOPXVEC3(position)) , 10, PxSphereGeometry(1),2, CCollider_PhysX::CreateLimitedSpherical);
}


CTestStaticPhysX * CTestStaticPhysX::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CTestStaticPhysX*	pInstance = new CTestStaticPhysX(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		DEBUGBREAK;
		Safe_Release(pInstance);
	}
	return pInstance;
}

CTestStaticPhysX * CTestStaticPhysX::Clone(void * pArg)
{
	CTestStaticPhysX*	pInstance = new CTestStaticPhysX(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		DEBUGBREAK;
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTestStaticPhysX::Free()
{
	__super::Free();


	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModel);
	Safe_Release(m_pPhysX);

	
}
