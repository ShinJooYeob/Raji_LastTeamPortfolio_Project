#include "stdafx.h"
#include "..\public\TestObject.h"


CTestObject::CTestObject(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CTestObject::CTestObject(const CTestObject & rhs)
	: CGameObject(rhs)
{
}

HRESULT CTestObject::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));


	return S_OK;
}

HRESULT CTestObject::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components());

	if (pArg != nullptr)
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, *((_float3*)pArg));

	m_pTransformCom->Rotation_CW(XMVectorSet(0, 1, 0, 0), XMConvertToRadians(170));

	return S_OK;
}

_int CTestObject::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)return -1;

	if (m_eNowSceneNum == SCENE_STAGE4)
	{
		if (g_pGameInstance->Get_DIKeyState(DIK_W) & DIS_Press)
		{
			m_pModel->Change_AnimIndex(12);

			if (g_pGameInstance->Get_DIKeyState(DIK_W) & DIS_Up)
			{
				m_pModel->Change_AnimIndex(11);
			}
		}
	}



	FAILED_CHECK(m_pModel->Update_AnimationClip(fDeltaTime, m_bIsOnScreen));

	return _int();
}

_int CTestObject::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)return -1;

	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND_NOLIGHT, this));


	return _int();
}

_int CTestObject::Render()
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

#ifdef _DEBUG
	if (m_pNavigationCom != nullptr)
	{
		m_pNavigationCom->Render(m_pTransformCom);
		//m_pNavigationCom->Render((CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_BackGround"), CGameObject::m_pTransformTag));

	}

#endif // _DEBUG

	return _int();
}

_int CTestObject::LateRender()
{

	return _int();
}



HRESULT CTestObject::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	if (m_eNowSceneNum == SCENE_STAGE4)
	{
		FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_TestObject_Himeko), TAG_COM(Com_Model), (CComponent**)&m_pModel));
		FAILED_CHECK(m_pModel->Change_AnimIndex(11));
	}
	else
	{
		FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_TestObject), TAG_COM(Com_Model), (CComponent**)&m_pModel));
		FAILED_CHECK(m_pModel->Change_AnimIndex(0));
	}


	CTransform::TRANSFORMDESC tDesc = {};

	tDesc.fMovePerSec = 5;
	tDesc.fRotationPerSec = XMConvertToRadians(60);
	tDesc.fScalingPerSec = 1;
	tDesc.vPivot = _float3(0, 0, 0);

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));


	if (m_eNowSceneNum == SCENE_STAGE4)
	{
		/* For.Com_Navigation */
		CNavigation::NAVIDESC		NaviDesc;
		NaviDesc.iCurrentIndex = -1;

		FAILED_CHECK(Add_Component(SCENE_STAGE4, TAG_CP(Prototype_Navigation), TAG_COM(Com_Navaigation), (CComponent**)&m_pNavigationCom, &NaviDesc));
	}
	return S_OK;
}

CTestObject * CTestObject::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CTestObject*	pInstance = new CTestObject(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CTestObject");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CTestObject::Clone(void * pArg)
{
	CTestObject*	pInstance = new CTestObject(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CTestObject");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTestObject::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModel);
	Safe_Release(m_pNavigationCom);
}
