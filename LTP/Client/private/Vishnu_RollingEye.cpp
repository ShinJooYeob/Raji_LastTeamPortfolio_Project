#include "stdafx.h"
#include "..\public\Vishnu_RollingEye.h"
#include "Player.h"
#include "Camera_Main.h"

CVishnu_RollingEye::CVishnu_RollingEye(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CVishnu_RollingEye::CVishnu_RollingEye(const CVishnu_RollingEye & rhs)
	: CGameObject(rhs)
{
}

HRESULT CVishnu_RollingEye::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CVishnu_RollingEye::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components()); 

	static _uint iNumKinds = 0;



	m_iKinds = iNumKinds;

	if (m_iKinds)
	{
		m_pTransformCom->Scaled_All(_float3(78.f, 78.5f, 78.5f));
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, _float3(155.7f, 51.3f, 58.f));
	}
	else
	{
		m_pTransformCom->Scaled_All(_float3(100.f, 80.f, 80.f));
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, _float3(164.298f, 51.2f, 58.334f));
	}

	iNumKinds++;
	if (iNumKinds > 1)iNumKinds = 0;


	Set_IsOcllusion(true);
	return S_OK;
}

_int CVishnu_RollingEye::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0) return -1;

	_float3 vPlayerPos = g_pGameInstance->Get_TargetPostion_Vector(PLV_PLAYER);
	_float PassedValue = max(min(vPlayerPos.y, 56.3f), 48.f);
	vPlayerPos.y = PassedValue;
	vPlayerPos = vPlayerPos.XMVector() + XMVector3Normalize(vPlayerPos.XMVector() - XMVectorSet(164.298f, 51.2f, 58.334f, 0)) * 15.f;

	switch (m_iKinds)
	{
	case 0:
	{

		m_pTransformCom->LookAt(vPlayerPos.XMVector());
		break;
	}
	case 1:
	{


		m_pTransformCom->LookAt(vPlayerPos.XMVector());
		break;
	}

	default:
		__debugbreak();
		break;
	}


	return _int();
}

_int CVishnu_RollingEye::LateUpdate(_double fDeltaTimer)
{
	if (__super::LateUpdate(fDeltaTimer) < 0) return -1;

	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));

	return _int();
}

_int CVishnu_RollingEye::Render()
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
		FAILED_CHECK(m_pModel->Render(m_pShaderCom, 2, i));
	}

	return _int();
}

_int CVishnu_RollingEye::LateRender()
{
	return _int();
}


HRESULT CVishnu_RollingEye::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VNAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_Vishnu_Rolling_Eye), TAG_COM(Com_Model), (CComponent**)&m_pModel));

	CTransform::TRANSFORMDESC tDesc = {};
	tDesc.fMovePerSec = 30.f;
	tDesc.fRotationPerSec = XMConvertToRadians(360);
	tDesc.fScalingPerSec = 1;
	tDesc.vPivot = _float3(0, 0, 0);
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));

	return S_OK;
}


CVishnu_RollingEye * CVishnu_RollingEye::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CVishnu_RollingEye*	pInstance = NEW CVishnu_RollingEye(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CVishnu_RollingEye");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CVishnu_RollingEye::Clone(void * pArg)
{
	CVishnu_RollingEye*	pInstance = NEW CVishnu_RollingEye(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CVishnu_RollingEye");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVishnu_RollingEye::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModel);
}
