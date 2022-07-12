#include "stdafx.h"
#include "..\public\Mahabalasura_Arms.h"


CMahabalasura_Arms::CMahabalasura_Arms(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CMonsterWeapon(pDevice, pDeviceContext)
{
}

CMahabalasura_Arms::CMahabalasura_Arms(const CMahabalasura_Arms & rhs)
	: CMonsterWeapon(rhs)
{
}

HRESULT CMahabalasura_Arms::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));


	return S_OK;
}

HRESULT CMahabalasura_Arms::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components());

	ZeroMemory(&m_eAttachedDesc, sizeof(ATTACHEDESC));

	if (pArg != nullptr)
	{
		memcpy(&m_eAttachedDesc, pArg, sizeof(ATTACHEDESC));
	}

	Set_LimLight_N_Emissive(_float4(255.f, 0.f, 10.f, 255.f), _float4(0));


	m_fAttackTime = GetSingle(CUtilityMgr)->RandomFloat(1.f, 2.f);

	return S_OK;
}

_int CMahabalasura_Arms::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0) return -1;


	if (m_fAttackTime <= 0 && !m_bIsAttack)
	{
		m_bIsAttack = true;
		m_fAttackTime = GetSingle(CUtilityMgr)->RandomFloat(1.f, 2.f);
		m_pModel->Change_AnimIndex_ReturnTo(1, 0);
	}

	if(!m_bIsAttack)
		m_fAttackTime -= (_float)fDeltaTime;

	//m_bIsOnScreen = g_pGameInstance->IsNeedToRender(m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS), m_fFrustumRadius);
	FAILED_CHECK(m_pModel->Update_AnimationClip(fDeltaTime * (m_fAnimmultiple),true));
	FAILED_CHECK(Adjust_AnimMovedTransform(fDeltaTime));

	return _int();
}

_int CMahabalasura_Arms::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)return -1;

	Update_AttachMatrix();
	
	
	_Matrix mat = m_fAttachedMatrix.XMatrix();

	mat.r[0] = XMVector3Normalize(mat.r[0]);
	mat.r[1] = XMVector3Normalize(mat.r[1]);
	mat.r[2] = XMVector3Normalize(mat.r[2]);

	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
	FAILED_CHECK(m_pRendererCom->Add_ShadowGroup(CRenderer::SHADOW_ANIMMODEL_ATTACHED, this, m_pTransformCom, m_pShaderCom, m_pModel, &_float4x4(mat)));
	m_fAttachedMatrix = m_fAttachedMatrix.TransposeXMatrix();

	//_Matrix Testmat =
	//	XMMatrixRotationX(XMConvertToRadians(-20))*
	//	XMMatrixRotationY(XMConvertToRadians(180))*
	//	XMMatrixRotationZ(XMConvertToRadians(0));

	//m_pTransformCom->Set_Matrix(Testmat);

	return _int();
}

_int CMahabalasura_Arms::Render()
{
	if (__super::Render() < 0)		return -1;

	NULL_CHECK_RETURN(m_pModel, E_FAIL);

	CGameInstance* pInstance = GetSingle(CGameInstance);

	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_AttechMatrix", &m_fAttachedMatrix, sizeof(_float4x4)));

	FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));

	_uint NumMaterial = m_pModel->Get_NumMaterial();

	for (_uint i = 0; i < NumMaterial; i++)
	{
		for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; j++)
		{
			FAILED_CHECK(m_pModel->Bind_OnShader(m_pShaderCom, i, j, MODLETEXTYPE(j)));
		}
		FAILED_CHECK(m_pModel->Render(m_pShaderCom, 4, i, "g_BoneMatrices"));
	}

	return _int();
}

_int CMahabalasura_Arms::LateRender()
{
	return _int();
}

void CMahabalasura_Arms::Update_AttachMatrix()
{
	m_fAttachedMatrix = m_pTransformCom->Get_WorldMatrix()  * m_eAttachedDesc.Caculate_AttachedBoneMatrix();
}

HRESULT CMahabalasura_Arms::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_Boss_MahabalasuraArms), TAG_COM(Com_Model), (CComponent**)&m_pModel));

	CTransform::TRANSFORMDESC tDesc = {};

	tDesc.fMovePerSec = 5;
	tDesc.fRotationPerSec = XMConvertToRadians(60);
	tDesc.fScalingPerSec = 1;
	tDesc.vPivot = _float3(0, 0, 0);

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));


	return S_OK;
}

HRESULT CMahabalasura_Arms::Adjust_AnimMovedTransform(_double fDeltatime)
{
	_uint iNowAnimIndex = m_pModel->Get_NowAnimIndex();
	_double PlayRate = m_pModel->Get_PlayRate();

	if (iNowAnimIndex != m_iOldAnimIndex || PlayRate > 0.98)
		m_iAdjMovedIndex = 0;


	if (PlayRate <= 0.98)
	{
		if (iNowAnimIndex == 1)
		{
			if (PlayRate > 0.56f && m_iAdjMovedIndex == 0)
			{
				_float3 TempPos;
				XMStoreFloat3(&TempPos, m_eAttachedDesc.Get_AttachedBoneWorldPosition());

				//for(_int i=0; i< 20; ++i)
				FAILED_CHECK(g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE4, TEXT("Layer_AttackArms"), TAG_OP(Prototype_Object_Boss_MahabalasuraAttackArms), &TempPos));

				++m_iAdjMovedIndex;
			}
		}
	}
	else
	{
		if (iNowAnimIndex == 1)
		{
			m_bIsAttack = false;
		}
	}

	m_iOldAnimIndex = iNowAnimIndex;
	return S_OK;
}

CMahabalasura_Arms * CMahabalasura_Arms::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CMahabalasura_Arms*	pInstance = new CMahabalasura_Arms(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CMahabalasura_Arms");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMahabalasura_Arms::Clone(void * pArg)
{
	CMahabalasura_Arms*	pInstance = new CMahabalasura_Arms(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created  Clone CMahabalasura_Arms");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMahabalasura_Arms::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModel);
}
