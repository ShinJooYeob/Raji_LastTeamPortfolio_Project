#include "stdafx.h"
#include "..\public\FemaleStatue.h"
#include "Demon_Tree.h"

CFemaleStatue::CFemaleStatue(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CMapObject(pDevice, pDeviceContext)
{
}

CFemaleStatue::CFemaleStatue(const CFemaleStatue & rhs)
	: CMapObject(rhs)
{
}

HRESULT CFemaleStatue::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));
	return S_OK;
}

HRESULT CFemaleStatue::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));
	FAILED_CHECK(SetUp_Components());


	if (pArg != nullptr)
	{
		_float3 vPos = *(_float3*)pArg;
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, vPos);
	}
	m_iPassIndex = 3;

	m_pTransformCom->Rotation_CW(XMVectorSet(0, 1.f, 0, 0), XMConvertToRadians(180.f));
	m_pTransformCom->Scaled_All(_float3(4.f));


	m_pDemonTree = (CDemon_Tree*)g_pGameInstance->Get_GameObject_By_LayerIndex(m_eNowSceneNum, TAG_LAY(Layer_MapObject), 0);



	return S_OK;
}

_int CFemaleStatue::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)
		return -1;

	m_pModel->Change_AnimIndex_ReturnTo_Must(1,0);

	CUtilityMgr* pUtilityMgr = GetSingle(CUtilityMgr);

	_int iClearCount = m_pDemonTree->Get_iCompleteCount();
	if (iClearCount < 0)
		iClearCount = 0;
	_float fAnlgeRate = (_float)fabs(m_pDemonTree->Get_NowTreeMesh_Angle() / 360.f);
	_float fBeginningAngle = m_pDemonTree->Get_NowTreeMesh_BeginningAngle();
	if (fAnlgeRate < fBeginningAngle / 360.f)
	{
		m_fRate = g_pGameInstance->Easing(TYPE_SinInOut,((0.25f)*(_float)iClearCount), ((0.25f)*(_float)iClearCount) - (0.25f), fAnlgeRate, fBeginningAngle /360.f);
	}
	else
	{
		m_fRate = g_pGameInstance->Easing(TYPE_SinInOut, ((0.25f)*(_float)iClearCount) - (0.25f), ((0.25f)*(_float)iClearCount), fAnlgeRate - fBeginningAngle / 360.f,(1.f - fBeginningAngle / 360.f));
	}


	if (m_fRate >= 1.f)
		m_fRate = 0.9999999999f;

	if (m_fRate > ((0.25f)*(_float)iClearCount))
		m_fRate = ((0.25f)*(_float)iClearCount);
	
	
	FAILED_CHECK(m_pModel->Update_AnimationClip(2.25f * m_fRate, m_bIsOnScreen));
	FAILED_CHECK(Adjust_AnimMovedTransform(fDeltaTime));
	return _int();
}

_int CFemaleStatue::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)
		return -1;

	FAILED_CHECK(m_pRendererCom->Add_ShadowGroup(CRenderer::SHADOW_ANIMMODEL, this, m_pTransformCom, m_pShaderCom, m_pModel));
	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));

	return _int();
}

_int CFemaleStatue::Render()
{
	if (__super::Render() < 0)
		return -1;

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
		FAILED_CHECK(m_pModel->Render(m_pShaderCom, 3, i, "g_BoneMatrices"));
	}

	return _int();
}

_int CFemaleStatue::LateRender()
{
	if (__super::LateRender() < 0)		return -1;

	return _int();
}

HRESULT CFemaleStatue::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));
	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_FemaleStatue), TAG_COM(Com_Model), (CComponent**)&m_pModel));


	m_pModel->Change_AnimIndex(1);
	FAILED_CHECK(m_pModel->Update_AnimationClip(g_fDeltaTime, m_bIsOnScreen));

	CTransform::TRANSFORMDESC tDesc = {};

	tDesc.fMovePerSec = 5;
	tDesc.fRotationPerSec = XMConvertToRadians(60);
	tDesc.fScalingPerSec = 1;
	tDesc.vPivot = _float3(0, 0, 0);
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));
	return S_OK;
}

HRESULT CFemaleStatue::Adjust_AnimMovedTransform(_double fDeltatime)
{
	_uint iNowAnimIndex = m_pModel->Get_NowAnimIndex();
	_double PlayRate = m_pModel->Get_PlayRate();

	if (iNowAnimIndex != m_iOldAnimIndex || PlayRate > 0.98)
		m_iAdjMovedIndex = 0;

	if (PlayRate <= 0.98)
	{
		switch (iNowAnimIndex)
		{
		case 0:
			break;
		case 1:
			break;
		}
	}
	else
	{

	}

	m_iOldAnimIndex = iNowAnimIndex;
	return S_OK;
}

CFemaleStatue * CFemaleStatue::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CFemaleStatue*	pInstance = NEW CFemaleStatue(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CFemaleStatue");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CFemaleStatue::Clone(void * pArg)
{
	CFemaleStatue*	pInstance = NEW CFemaleStatue(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created  Clone CFemaleStatue");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CFemaleStatue::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModel);
}
