#include "stdafx.h"
#include "..\public\ShellingArrow.h"

CShellingArrow::CShellingArrow(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CShellingArrow::CShellingArrow(const CShellingArrow & rhs)
	: CGameObject(rhs)
{
}

HRESULT CShellingArrow::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CShellingArrow::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	memcpy(&m_tShellingArrowDesc, pArg, sizeof(SHELLINGARROWDESC));

	FAILED_CHECK(SetUp_Components());

	return S_OK;
}

_int CShellingArrow::Update(_double dDeltaTime)
{
	if (__super::Update(dDeltaTime) < 0) return -1;

	for (_uint i = 0; i < 32; i++)
	{
		if (0.5f >= XMVectorGetY(m_vecInstancedTransform[i]->Get_MatrixState(CTransform::TransformState::STATE_POS)))
		{
			continue;
		}

		m_vecInstancedTransform[i]->Move_Forward(dDeltaTime);
	}
	 

	m_fDestroy_Count -= (_float)dDeltaTime;
	if (0.f >= m_fDestroy_Count)
	{
		Set_IsDead();
	}
	return _int();
}

_int CShellingArrow::LateUpdate(_double dDeltaTime)
{
	if (__super::LateUpdate(dDeltaTime) < 0) return -1;

	FAILED_CHECK(m_pRendererCom->Add_ShadowGroup_InstanceModel(CRenderer::INSTSHADOW_NONANIMINSTANCE, this, &m_vecInstancedTransform, m_pModelInstance, m_pShaderCom, m_pModel));
	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));

	return _int();
}

_int CShellingArrow::Render()
{
	if (__super::Render() < 0)
		return -1;

	NULL_CHECK_RETURN(m_pModel, E_FAIL);

	CGameInstance* pInstance = GetSingle(CGameInstance);
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));

	FAILED_CHECK(m_pModelInstance->Render(m_pShaderCom, 2, &m_vecInstancedTransform));

	return 0;
}

_int CShellingArrow::LateRender()
{
	return _int();
}

HRESULT CShellingArrow::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VTXNONANIMINST), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));
	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_PlayerWeapon_Arrow), TAG_COM(Com_Model), (CComponent**)&m_pModel));

	_Vector vLookDir = XMVector3Normalize(m_tShellingArrowDesc.fTargetPos.XMVector() - m_tShellingArrowDesc.fStartPos.XMVector());
	for (_uint i = 0; i < 32; i++)
	{
		CTransform* pTransform = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
		NULL_CHECK_RETURN(pTransform, E_FAIL);
		
		_float3 fStartPos = GetSingle(CUtilityMgr)->RandomFloat3(
			_float3(m_tShellingArrowDesc.fStartPos.x - 1.2f, m_tShellingArrowDesc.fStartPos.y - 3.f, m_tShellingArrowDesc.fStartPos.z - 1.2f),
			_float3(m_tShellingArrowDesc.fStartPos.x + 1.2f, m_tShellingArrowDesc.fStartPos.y + 3.f, m_tShellingArrowDesc.fStartPos.z + 1.2f)
		);

		pTransform->Scaled_All(_float3(1.6f, 1.6f, 1.6f));

		pTransform->LookDir_ver2(vLookDir);
		pTransform->Set_MatrixState(CTransform::STATE_POS, _float3(fStartPos.x, fStartPos.y, fStartPos.z));
		pTransform->Set_MoveSpeed(18.f);

		m_vecInstancedTransform.push_back(pTransform);
	}

	
	CModelInstance::MODELINSTDESC tModelIntDsec;
	tModelIntDsec.m_pTargetModel = m_pModel;
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_ModelInstance_32), TAG_COM(Com_ModelInstance), (CComponent**)&m_pModelInstance, &tModelIntDsec));

	return S_OK;
}

CShellingArrow * CShellingArrow::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CShellingArrow*	pInstance = new CShellingArrow(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CShellingArrow");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CShellingArrow::Clone(void * pArg)
{
	CShellingArrow*	pInstance = new CShellingArrow(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CShellingArrow");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CShellingArrow::Free()
{
	__super::Free();

	for (auto& pTransform : m_vecInstancedTransform)
		Safe_Release(pTransform);
	m_vecInstancedTransform.clear();

	Safe_Release(m_pModelInstance);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModel);
}
