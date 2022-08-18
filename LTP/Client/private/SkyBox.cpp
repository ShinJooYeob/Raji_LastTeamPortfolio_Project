#include "stdafx.h"
#include "..\public\SkyBox.h"



CSkyBox::CSkyBox(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CSkyBox::CSkyBox(const CSkyBox & rhs)
	: CGameObject(rhs)
{
}

HRESULT CSkyBox::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));


	return S_OK;
}

HRESULT CSkyBox::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components());

	Set_LimLight_N_Emissive(_float4(0), _float4(1, 0, 0, 1));

	return S_OK;
}

_int CSkyBox::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)
		return -1;

	m_pTransformCom->Turn_CW(XMVectorSet(0, 1, 0, 0), fDeltaTime);



	return _int();
}

_int CSkyBox::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)
		return -1;

	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_PRIORITY, this));

	return _int();
}

_int CSkyBox::Render()
{
	if (__super::Render() < 0)
		return -1;



	NULL_CHECK_RETURN(m_pModel, E_FAIL);

	CGameInstance* pInstance = GetSingle(CGameInstance);

	m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, pInstance->Get_TargetPostion_float4(PLV_CAMERA));
	FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));

	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));



	_uint NumMaterial = m_pModel->Get_NumMaterial();

	if (GetSingle(CUtilityMgr)->Get_Renderer()->Get_IsOnPostPorcessing(POSTPROCESSING_DDFOG))
	{
		FAILED_CHECK(m_pShaderCom->Set_RawValue("g_vColor", &_float4(GetSingle(CUtilityMgr)->Get_Renderer()->Get_FogColor(), 1), sizeof(_float4)));
		for (_uint i = 0; i < NumMaterial; i++)
		{
			FAILED_CHECK(m_pModel->Render(m_pShaderCom, 9, i));
		}
	}
	else
	{
		if (g_pGameInstance->Get_NowSceneNum() == SCENE_EDIT)
		{
			m_pShaderCom->Set_Texture("g_DiffuseTexture", nullptr);
			for (_uint i = 0; i < NumMaterial; i++)
			{
				FAILED_CHECK(m_pModel->Render(m_pShaderCom, 6, i));
			}
		}
		else
		{
			for (_uint i = 0; i < NumMaterial; i++)
			{
				FAILED_CHECK(m_pModel->Bind_OnShader(m_pShaderCom, i, 1, MODLETEXTYPE(1)));
				FAILED_CHECK(m_pModel->Render(m_pShaderCom, 6, i));
			}
		}

	}

	return _int();
}

_int CSkyBox::LateRender()
{
	if (__super::LateRender() < 0)
		return -1;




	return _int();
}

HRESULT CSkyBox::SetUp_Components()
{


	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VNAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_SkyBox), TAG_COM(Com_Model), (CComponent**)&m_pModel));
	//FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_SkyBox), TAG_COM(Com_Model), (CComponent**)&m_pModel));



	CTransform::TRANSFORMDESC tDesc = {};

	tDesc.fMovePerSec = 5;
	tDesc.fRotationPerSec = XMConvertToRadians(1.5f);
	tDesc.fScalingPerSec = 1;
	tDesc.vPivot = _float3(0, 0, 0);

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));


	

	return S_OK;
}

CSkyBox * CSkyBox::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CSkyBox*	pInstance = NEW CSkyBox(pDevice,pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CSkyBox");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CSkyBox::Clone(void * pArg)
{
	CSkyBox*	pInstance = NEW CSkyBox(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CSkyBox");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSkyBox::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModel);
	Safe_Release(m_pShaderCom);
}
