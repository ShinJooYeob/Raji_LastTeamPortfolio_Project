#include "stdafx.h"
#include "..\public\TestStaticObject.h"


CTestStaticObject::CTestStaticObject(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CTestStaticObject::CTestStaticObject(const CTestStaticObject & rhs)
	: CGameObject(rhs)
{
}
int CTestStaticObject::OBJID = 0;

HRESULT CTestStaticObject::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));


	return S_OK;
}

HRESULT CTestStaticObject::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components());

	if (pArg != nullptr)
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, *((_float3*)pArg));

//	m_pTransformCom->Rotation_CW(XMVectorSet(0, 1, 0, 0), XMConvertToRadians(170));
	m_pTransformCom->Scaled_All(_float3(1, 1, 1));

	++OBJID;
	if (OBJID == 1)
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, _float3(3, 0, 0));

	if (OBJID == 2)
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, _float3(-3, 0, 0));


	return S_OK;
}

_int CTestStaticObject::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)return -1;

	FAILED_CHECK(m_pModel->Update_AnimationClip(fDeltaTime, true));
	return _int();
}

_int CTestStaticObject::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)return -1;

	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));

	if (KEYPRESS(DIK_W))
	{
		m_pTransformCom->Move_Down(fDeltaTime);
	}

	if (KEYPRESS(DIK_S))
	{
		m_pTransformCom->Move_Up(fDeltaTime);
	}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  
	if (KEYPRESS(DIK_A))
	{
		m_pTransformCom->Move_Left(fDeltaTime);
	}
	if (KEYPRESS(DIK_D))
	{
		m_pTransformCom->Move_Right(fDeltaTime);
	}

	_float3 up = _float3(0, 1, 0);
	
	if (KEYPRESS(DIK_E))
	{
		m_pTransformCom->Turn_CW(XMLoadFloat3(&up),fDeltaTime);
	}

	if (KEYPRESS(DIK_Q))
	{
		m_pTransformCom->Turn_CCW(XMLoadFloat3(&up), fDeltaTime);
	}


	// Change_AnimIndex_UntilNReturn
	//if (KEYDOWN(DIK_W))
	//{
	//	// A~B까지 재생후에 C로 돌아간다.
	//	AniIndex = 0;
	//	FAILED_CHECK(m_pModel->Change_AnimIndex_UntilNReturn(2, 3, 0));
	//}
	// Change_AnimIndex_UntilNReturn_Must
	//if (KEYDOWN(DIK_E))
	//{
	//	// 강제 재생
	//	FAILED_CHECK(m_pModel->Change_AnimIndex_UntilNReturn_Must(0, 2, 0));
	//}
	// Change_AnimIndex_UntilTo
	//if (KEYDOWN(DIK_R))
	//{
	//	// A~B로 차례로 이동하는 애니메이션 / m_bIsBlockAnim는 애니메이션이 끝나는지 체크
	//	m_pModel->Change_AnimIndex_UntilTo(2, 6);
	//}
	// Change_AnimIndex_ReturnTo
	//if (KEYDOWN(DIK_T))
	//{
	//	m_pModel->Change_AnimIndex_UntilTo(0, 5);
	//}
	// Change_AnimIndex_ReturnTo_Must
	//if (KEYDOWN(DIK_Y))
	//{
	//	m_pModel->Change_AnimIndex_ReturnTo_Must(0, 5);
	//}


	return _int();
}

_int CTestStaticObject::Render()
{
	if (__super::Render() < 0)		return -1;

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

		FAILED_CHECK(m_pModel->Render(m_pShaderCom, 1, i, "g_BoneMatrices"));
	}

	return _int();
}

_int CTestStaticObject::LateRender()
{
	return _int();
}



HRESULT CTestStaticObject::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

//	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VNAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	

//	static int ID = 0;
	//if (ID % 2 == 0)
	//{

	//	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Mesh_TestObject), TAG_COM(Com_Model), (CComponent**)&m_pModel));
	//}
	//else
	//{
	//	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Mesh_TEST_STATIC), TAG_COM(Com_Model), (CComponent**)&m_pModel));
	//}
	//ID++;

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Mesh_Player), TAG_COM(Com_Model), (CComponent**)&m_pModel));



	CTransform::TRANSFORMDESC tDesc = {};

	tDesc.fMovePerSec = 5;
	tDesc.fRotationPerSec = XMConvertToRadians(60);
	tDesc.fScalingPerSec = 1.f;
	tDesc.vPivot = _float3(0, 0, 0);

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));


	return S_OK;
}

CTestStaticObject * CTestStaticObject::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CTestStaticObject*	pInstance = NEW CTestStaticObject(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		DEBUGBREAK;
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CTestStaticObject::Clone(void * pArg)
{
	CTestStaticObject*	pInstance = NEW CTestStaticObject(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		DEBUGBREAK;
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTestStaticObject::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModel);
}
