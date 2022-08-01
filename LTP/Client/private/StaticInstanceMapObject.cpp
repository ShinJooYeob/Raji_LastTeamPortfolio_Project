#include "stdafx.h"
#include "..\public\StaticInstanceMapObject.h"

#define FrustumCutlineNum 99999999999

CStaticInstanceMapObject::CStaticInstanceMapObject(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CMapObject(pDevice, pDeviceContext)
{
}

CStaticInstanceMapObject::CStaticInstanceMapObject(const CStaticInstanceMapObject & rhs)
	: CMapObject(rhs)
{
}

HRESULT CStaticInstanceMapObject::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));
	return S_OK;
}

HRESULT CStaticInstanceMapObject::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));
	FAILED_CHECK(SetUp_Components());


	return S_OK;

}

_int CStaticInstanceMapObject::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)
		return -1;






	return _int();
}

_int CStaticInstanceMapObject::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)
		return -1;



	CGameInstance* pInstance = GetSingle(CGameInstance);


	m_vecForRender.clear();
	m_vecForRender.resize(m_mapInstancMapObjects.size());

	_int iForRenderVectorIndex = -1;
	vector<CTransform*>*	pForRenderingObj = nullptr;

	for (auto& InstancMapObject : m_mapInstancMapObjects)
	{
		pForRenderingObj = &InstancMapObject.second.pvecTransform;
		iForRenderVectorIndex++;

		if (InstancMapObject.second.pvecTransform.size() < FrustumCutlineNum)
		{
			m_vecForRender[iForRenderVectorIndex].clear();
			m_vecForRender[iForRenderVectorIndex].reserve(InstancMapObject.second.pvecTransform.size());
			 
			for (auto& pTransform : InstancMapObject.second.pvecTransform)
			{
				if (pInstance->IsNeedToLightRender(pTransform->Get_MatrixState_Float3(CTransform::STATE_POS), InstancMapObject.second.fFrustumRange))
				{
					m_vecForRender[iForRenderVectorIndex].push_back(pTransform);
				}

			}
			pForRenderingObj = &m_vecForRender[iForRenderVectorIndex];

		}


		if (pForRenderingObj->size() > 0)
		{
			Set_IsOcllusion(InstancMapObject.second.bIsOcllusion);

			m_pRendererCom->Add_ShadowGroup_InstanceModel(CRenderer::INSTSHADOW_NONANIMINSTANCE,
				this, pForRenderingObj, InstancMapObject.second.pModelInstance,
				m_pShaderCom, InstancMapObject.second.pModel);

		}
	}


	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));





	return _int();
}

_int CStaticInstanceMapObject::Render()
{
	if (__super::Render() < 0)
		return -1;

	CGameInstance* pInstance = GetSingle(CGameInstance);
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));


	m_vecForRender.clear();
	m_vecForRender.resize(m_mapInstancMapObjects.size());
	_int iForRenderVectorIndex = -1;
	vector<CTransform*>*	pForRenderingObj = nullptr;

	for (auto& InstancMapObject : m_mapInstancMapObjects)
	{
		pForRenderingObj = &InstancMapObject.second.pvecTransform;
		iForRenderVectorIndex++;

		if (InstancMapObject.second.pvecTransform.size() < FrustumCutlineNum)
		{
			m_vecForRender[iForRenderVectorIndex].clear();
			m_vecForRender[iForRenderVectorIndex].reserve(InstancMapObject.second.pvecTransform.size());

			for (auto& pTransform : InstancMapObject.second.pvecTransform)
			{
				if (pInstance->IsNeedToRender(pTransform->Get_MatrixState_Float3(CTransform::STATE_POS), InstancMapObject.second.fFrustumRange))
				{
					m_vecForRender[iForRenderVectorIndex].push_back(pTransform);
				}

			}
			pForRenderingObj = &m_vecForRender[iForRenderVectorIndex];
		}


		if (pForRenderingObj->size() > 0)
		{
			InstancMapObject.second.pModelInstance->Render(m_pShaderCom, InstancMapObject.second.iPassIndex, pForRenderingObj);
		}
	}



	return 0;
}

_int CStaticInstanceMapObject::LateRender()
{
	if (__super::LateRender() < 0)
		return -1;

	return _int();
}

HRESULT CStaticInstanceMapObject::Add_InstanceMapObject(OBJELEMENT& tElements)
{

	auto iter = find_if(m_mapInstancMapObjects.begin(), m_mapInstancMapObjects.end(), CTagStringFinder(tElements.MeshID));


	CGameInstance* pInstance = g_pGameInstance;

	if (iter == m_mapInstancMapObjects.end())
	{
		INSTMAP InstanceMapDesc;

		InstanceMapDesc.iPassIndex = 13;

		if (tElements.PassIndex == 4) InstanceMapDesc.iPassIndex = 14;
		if (tElements.PassIndex == 14) InstanceMapDesc.iPassIndex = 15;


		//InstanceMapDesc.iPassIndex = tElements.PassIndex;
		InstanceMapDesc.bIsOcllusion = tElements.bIsOcllsuion;
		InstanceMapDesc.fFrustumRange = tElements.FrustumRange;

		InstanceMapDesc.pModel = (CModel*)(pInstance->Clone_Component(m_eNowSceneNum, tElements.MeshID));
		NULL_CHECK_RETURN(InstanceMapDesc.pModel, E_FAIL);

		CModelInstance::MODELINSTDESC ModelInstanceDesc;
		ModelInstanceDesc.m_pTargetModel = InstanceMapDesc.pModel;

		InstanceMapDesc.pModelInstance = (CModelInstance*)(pInstance->Clone_Component(m_eNowSceneNum, TAG_CP(Prototype_ModelInstance_128), &ModelInstanceDesc));
		NULL_CHECK_RETURN(InstanceMapDesc.pModelInstance, E_FAIL);

		CTransform* pTransform = (CTransform*)(pInstance->Clone_Component(m_eNowSceneNum, TAG_CP(Prototype_Transform)));
		NULL_CHECK_RETURN(pTransform, E_FAIL);
		pTransform->Set_Matrix(tElements.matTransform);

		InstanceMapDesc.pvecTransform.push_back(pTransform);


		m_mapInstancMapObjects.emplace(tElements.MeshID, InstanceMapDesc);
	}
	else
	{
		CTransform* pTransform = (CTransform*)(pInstance->Clone_Component(m_eNowSceneNum, TAG_CP(Prototype_Transform)));
		NULL_CHECK_RETURN(pTransform, E_FAIL);
		pTransform->Set_Matrix(tElements.matTransform);

		if (iter->second.fFrustumRange < tElements.FrustumRange)
			iter->second.fFrustumRange = tElements.FrustumRange;

		iter->second.pvecTransform.push_back(pTransform);
	}

	return S_OK;
}

HRESULT CStaticInstanceMapObject::SetUp_Components()
{

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VTXNONANIMINST), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));




	return S_OK;
}

CStaticInstanceMapObject * CStaticInstanceMapObject::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CStaticInstanceMapObject*	pInstance = NEW CStaticInstanceMapObject(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CStaticInstanceMapObject");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CStaticInstanceMapObject::Clone(void * pArg)
{
	CStaticInstanceMapObject*	pInstance = NEW CStaticInstanceMapObject(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CStaticInstanceMapObject");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CStaticInstanceMapObject::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);


	for (auto& InstancMapObject : m_mapInstancMapObjects)
	{
		Safe_Release(InstancMapObject.second.pModel);
		Safe_Release(InstancMapObject.second.pModelInstance);

		for (auto& Transforms : InstancMapObject.second.pvecTransform)
			Safe_Release(Transforms);
		InstancMapObject.second.pvecTransform.clear();
	}
	m_mapInstancMapObjects.clear();
}
