#include "stdafx.h"
#include "..\public\MandalaMesh.h"

CMandalaMesh::CMandalaMesh(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CMapObject(pDevice, pDeviceContext)
{
}

CMandalaMesh::CMandalaMesh(const CMandalaMesh & rhs)
	: CMapObject(rhs)
{
}

HRESULT CMandalaMesh::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));
	return S_OK;
}

HRESULT CMandalaMesh::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	if (pArg != nullptr)
	{
		m_MandalaDesc = *(MANDALADESC*)pArg;
	}

	FAILED_CHECK(SetUp_Components());
	m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, m_MandalaDesc.m_Pos);
	m_iPassIndex = 3;

	m_fFrustumRadius = 20.f;

	switch (m_MandalaDesc.m_eMandalaNum)
	{
	case CMandalaMesh::MANDALA_00:
	{
		m_pTransformCom->Rotation_CW(XMVectorSet(0, 1.f, 0, 0), XMConvertToRadians(72.f));
		m_BeginningAngle = 72.f;
		m_fRotationAngle = 72.f;
	}
	break;

	case CMandalaMesh::MANDALA_01:
	{
		m_pTransformCom->Rotation_CW(XMVectorSet(0, 1.f, 0, 0), XMConvertToRadians(144.f));
		m_BeginningAngle = 144.f;
		m_fRotationAngle = 144.f;
	}
	break;

	case CMandalaMesh::MANDALA_02:
	{
		m_pTransformCom->Rotation_CW(XMVectorSet(0, 1.f, 0, 0), XMConvertToRadians(72.f));
		m_BeginningAngle = 72.f;
		m_fRotationAngle = 72.f;
	}
	break;

	case CMandalaMesh::MANDALA_03:
	{
		m_pTransformCom->Rotation_CW(XMVectorSet(0, 1.f, 0, 0), XMConvertToRadians(288.f));
		m_BeginningAngle = 288.f;
		m_fRotationAngle = 288.f;
	}
	break;

	case CMandalaMesh::MANDALA_04:
	{
		m_pTransformCom->Rotation_CW(XMVectorSet(0, 1.f, 0, 0), XMConvertToRadians(0.f));
		m_BeginningAngle = 0.f;
		m_fRotationAngle = 0.f;
		m_bIsAngleZero = true;
	}
	break;

	}

	return S_OK;
}

_int CMandalaMesh::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)
		return -1;


	return _int();
}

_int CMandalaMesh::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)
		return -1;

	FAILED_CHECK(m_pRendererCom->Add_ShadowGroup(CRenderer::SHADOW_NONANIMMODEL, this, m_pTransformCom, m_pShaderCom, m_pModel));

	if (g_pGameInstance->IsNeedToRender(m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS), m_fFrustumRadius))
	{
		FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
	}

	return _int();
}

_int CMandalaMesh::Render()
{
	if (__super::Render() < 0)
		return -1;


	NULL_CHECK_RETURN(m_pModel, E_FAIL);


	_Matrix Oldmat = m_pTransformCom->Get_WorldMatrix();
	_float OldTurnSpeed = m_pTransformCom->Get_TurnSpeed();
	
	m_pTransformCom->Set_TurnSpeed(1.f);
	m_pTransformCom->Turn_CCW(XMVectorSet(0, 1, 0, 0), XMConvertToRadians(-45));

	FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));

	m_pTransformCom->Set_TurnSpeed(OldTurnSpeed);
	m_pTransformCom->Set_Matrix(Oldmat);

	FAILED_CHECK(__super::SetUp_ConstTable(m_pShaderCom));


	_uint NumMaterial = m_pModel->Get_NumMaterial();

	for (_uint i = 0; i < NumMaterial; i++)
	{

		for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; j++)
			FAILED_CHECK(m_pModel->Bind_OnShader(m_pShaderCom, i, j, MODLETEXTYPE(j)));

		FAILED_CHECK(m_pModel->Render(m_pShaderCom, m_iPassIndex, i));
	}

	return 0;
}

_int CMandalaMesh::LateRender()
{
	if (__super::LateRender() < 0)
		return -1;




	return _int();
}

void CMandalaMesh::Turn_CCW()
{
	--m_fRotationAngle;
	if (m_fRotationAngle <= -360.f)
		m_fRotationAngle = 0;
	m_pTransformCom->Rotation_CW(XMVectorSet(0, 1.f, 0, 0), XMConvertToRadians(m_fRotationAngle));
}

void CMandalaMesh::Turn_CW()
{
	++m_fRotationAngle;

	if (m_fRotationAngle >= 360.f)
		m_fRotationAngle = 0;

	m_pTransformCom->Rotation_CW(XMVectorSet(0, 1.f, 0, 0), XMConvertToRadians(m_fRotationAngle));
}

_bool CMandalaMesh::CheckAngle()
{
	if ((m_fRotationAngle >= 0.f && m_fRotationAngle < 2.f) || (m_fRotationAngle >= 358.f && m_fRotationAngle <= 360.f) ||
		(m_fRotationAngle <= 0.f && m_fRotationAngle >= -2.f) || (m_fRotationAngle <= -358.f && m_fRotationAngle >= -360.f)
		|| m_fRotationAngle == 0.f)
	{
		m_fRotationAngle = 0;
		m_pTransformCom->Rotation_CW(XMVectorSet(0, 1.f, 0, 0), m_fRotationAngle);
		m_bIsAngleZero = true;
	}

	return m_bIsAngleZero;
}

HRESULT CMandalaMesh::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VNAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	switch (m_MandalaDesc.m_eMandalaNum)
	{
	case CMandalaMesh::MANDALA_00:
		FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_MandalaCircle_0), TAG_COM(Com_Model), (CComponent**)&m_pModel));
		break;
	case CMandalaMesh::MANDALA_01:
		FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_MandalaCircle_1), TAG_COM(Com_Model), (CComponent**)&m_pModel));
		break;
	case CMandalaMesh::MANDALA_02:
		FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_MandalaCircle_2), TAG_COM(Com_Model), (CComponent**)&m_pModel));
		break;
	case CMandalaMesh::MANDALA_03:
		FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_MandalaCircle_3), TAG_COM(Com_Model), (CComponent**)&m_pModel));
		break;
	case CMandalaMesh::MANDALA_04:
		FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_MandalaCircle_4), TAG_COM(Com_Model), (CComponent**)&m_pModel));
		break;
	}

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom));

	return S_OK;
}

CMandalaMesh * CMandalaMesh::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CMandalaMesh*	pInstance = NEW CMandalaMesh(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CMandalaMesh");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMandalaMesh::Clone(void * pArg)
{
	CMandalaMesh*	pInstance = NEW CMandalaMesh(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created Clone CMandalaMesh");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMandalaMesh::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModel);
}
