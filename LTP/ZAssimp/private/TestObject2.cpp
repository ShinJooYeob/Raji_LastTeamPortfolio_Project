#include "stdafx.h"
#include "..\public\TestObject2.h"


CTestObject2::CTestObject2(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CTestObject2::CTestObject2(const CTestObject2 & rhs)
	: CGameObject(rhs)
{
}

HRESULT CTestObject2::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));


	return S_OK;
}

HRESULT CTestObject2::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components());

	if (pArg != nullptr)
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, *((_float3*)pArg));

	m_pTransformCom->Rotation_CW(XMVectorSet(0, 1, 0, 0), XMConvertToRadians(170));
//	m_pTransformCom->Scaled_All(_float3(0.5f, 0.5f, 0.5f));


	return S_OK;
}

_int CTestObject2::Update(_double fDeltaTime)
{

	if (__super::Update(fDeltaTime) < 0)return -1;

	FAILED_CHECK(m_pModel->Update_AnimationClip(fDeltaTime, m_bIsOnScreen));

	return _int();
}

_int CTestObject2::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)return -1;

	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));

	if (KEYPRESS(DIK_DOWNARROW))
	{
		m_pTransformCom->Move_Backward(fDeltaTime);
	}
	if (KEYPRESS(DIK_UPARROW))
	{
		m_pTransformCom->Move_Forward(fDeltaTime);
	}

	// 애니메이션 테스트
	// Change_AnimIndex

	static int AniIndex = 0;

	if (KEYDOWN(DIK_Q))
	{
		// 단순한 애니메이션 재생
		AniIndex++;		
		FAILED_CHECK(m_pModel->Change_AnimIndex(AniIndex));
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

_int CTestObject2::Render()
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

_int CTestObject2::LateRender()
{
	return _int();
}



HRESULT CTestObject2::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_TestObject), TAG_COM(Com_Model), (CComponent**)&m_pModel));
	FAILED_CHECK(m_pModel->Change_AnimIndex(0));


	CTransform::TRANSFORMDESC tDesc = {};

	tDesc.fMovePerSec = 5;
	tDesc.fRotationPerSec = XMConvertToRadians(60);
	tDesc.fScalingPerSec = 1.f;
	tDesc.vPivot = _float3(0, 0, 0);

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));


	return S_OK;
}

CTestObject2 * CTestObject2::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CTestObject2*	pInstance = new CTestObject2(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		DEBUGBREAK;
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CTestObject2::Clone(void * pArg)
{
	CTestObject2*	pInstance = new CTestObject2(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		DEBUGBREAK;
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTestObject2::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModel);
}
