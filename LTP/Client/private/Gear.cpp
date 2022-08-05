#include "stdafx.h"
#include "..\public\Gear.h"


CGear::CGear(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CMapObject(pDevice, pDeviceContext)
{
}

CGear::CGear(const CGear & rhs)
	: CMapObject(rhs)
{
}

HRESULT CGear::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));


	return S_OK;
}

HRESULT CGear::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	if (nullptr != pArg)
	{
		memcpy(&m_GearStateDesc, pArg, sizeof(GEAR_STATEDESC));
	}

	SetUp_Components();


	m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, m_GearStateDesc.fPos);
	m_pTransformCom->Scaled_All(m_GearStateDesc.fScale);


	m_fFrustumRadius = 20.f;

	//Set_LimLight_N_Emissive(_float4(1.f, 0.f, 0.f, 1.f), _float4(1, 0.5f, 1, 1));

	return S_OK;
}

_int CGear::Update(_double dDeltaTime)
{
	if (__super::Update(dDeltaTime) < 0)
		return -1;



	if (m_bTurnOn == true)
	{
		switch (m_GearStateDesc.iTurnDirection)
		{
		case 0:
		{
			m_pTransformCom->Turn_CW(XMVectorSet(1.f, 0.f, 0.f, 0.f), dDeltaTime);
			break;
		}
		case 1:
		{
			m_pTransformCom->Turn_CCW(XMVectorSet(1.f, 0.f, 0.f, 0.f), dDeltaTime);
			break;
		}
		default:
			break;
		}
	}


	//FAILED_CHECK(g_pGameInstance->Add_RepelGroup(m_pTransformCom, 2.5f));

	return _int();
}

_int CGear::LateUpdate(_double dDeltaTime)
{
	if (__super::LateUpdate(dDeltaTime) < 0)
		return -1;



	FAILED_CHECK(m_pRendererCom->Add_ShadowGroup(CRenderer::SHADOW_NONANIMMODEL, this, m_pTransformCom, m_pShaderCom, m_pModel));

	if (g_pGameInstance->IsNeedToRender(m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS), m_fFrustumRadius))
	{
		FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
	}

	return _int();
}

_int CGear::Render()
{
	if (__super::Render() < 0)
		return -1;

	NULL_CHECK_RETURN(m_pModel, E_FAIL);

	FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));


	FAILED_CHECK(__super::SetUp_ConstTable(m_pShaderCom));


	_uint NumMaterial = m_pModel->Get_NumMaterial();

	for (_uint i = 0; i < NumMaterial; i++)
	{

		for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; j++)
			FAILED_CHECK(m_pModel->Bind_OnShader(m_pShaderCom, i, j, MODLETEXTYPE(j)));

		FAILED_CHECK(m_pModel->Render(m_pShaderCom, 3, i));
	}

	return 0;
}

_int CGear::LateRender()
{
	if (__super::LateRender() < 0)
		return -1;




	return _int();
}

HRESULT CGear::SetUp_Components()
{
	CTransform::TRANSFORMDESC tDesc = {};

	tDesc.fMovePerSec = m_GearStateDesc.fSpeed;
	tDesc.fRotationPerSec = XMConvertToRadians(60);
	tDesc.vPivot = _float3(0, 0, 0);
	tDesc.fScalingPerSec = 1;

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom,&tDesc));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VNAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));


	switch (m_GearStateDesc.iGearTypeNumber)
	{
	case CGear::GEARTYPE_1:
	{
		FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_SM_ENV_CC_CogMedium_03), TAG_COM(Com_Model), (CComponent**)&m_pModel));
		break;
	}
	case CGear::GEARTYPE_2:
	{
		FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_SM_ENV_CC_CogMedium_02), TAG_COM(Com_Model), (CComponent**)&m_pModel));
		break;
	}
	case CGear::GEARTYPE_3:
	{
		FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_SM_ENV_CC_CogMedium_02), TAG_COM(Com_Model), (CComponent**)&m_pModel));
		break;
	}
	case CGear::GEARTYPE_4:
	{
		FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_SM_ENV_CC_CogMedium_01), TAG_COM(Com_Model), (CComponent**)&m_pModel));
		break;
	}
	case CGear::GEARTYPE_5:
	{
		FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_SM_ENV_CC_CogMedium_03), TAG_COM(Com_Model), (CComponent**)&m_pModel));
		break;
	}
	case CGear::GEARTYPE_6:
	{
		FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_SM_ENV_CC_CogMedium_03), TAG_COM(Com_Model), (CComponent**)&m_pModel));
		break;
	}
	case CGear::GEARTYPE_7:
	{
		FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_SM_ENV_CC_CogMedium_03), TAG_COM(Com_Model), (CComponent**)&m_pModel));
		break;
	}
	case CGear::GEARTYPE_8:
	{
		FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_SM_ENV_CC_WaterWheel_01), TAG_COM(Com_Model), (CComponent**)&m_pModel));
		break;
	}
	}


	return S_OK;
}

CGear * CGear::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CGear*	pInstance = NEW CGear(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CGear");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CGear::Clone(void * pArg)
{
	CGear*	pInstance = NEW CGear(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created Clone CGear");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CGear::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModel);
}
