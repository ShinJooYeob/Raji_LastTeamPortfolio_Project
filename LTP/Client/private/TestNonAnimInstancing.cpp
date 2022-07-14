#include "stdafx.h"
#include "..\public\TestNonAnimInstancing.h"



CTestNonAnimInstancing::CTestNonAnimInstancing(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CMapObject(pDevice, pDeviceContext)
{
}

CTestNonAnimInstancing::CTestNonAnimInstancing(const CTestNonAnimInstancing & rhs)
	: CMapObject(rhs)
{
}

HRESULT CTestNonAnimInstancing::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));
	return S_OK;
}

HRESULT CTestNonAnimInstancing::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));
	FAILED_CHECK(SetUp_Components());



	Set_IsOcllusion(true);

	m_iPassIndex = 3;

	return S_OK;

}

_int CTestNonAnimInstancing::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)
		return -1;

	if (g_pGameInstance->Get_DIKeyState(DIK_V) & DIS_Down)
	{
		for (_uint i = 0; i < m_vecTimer.size(); i++)
		{
			m_vecTimer[i] = _float4(-1.f * i - 3.f, 3.f, 0, 0);
		}

	}

	for (_uint i = 0; i < m_vecTimer.size(); i++)
	{
		m_vecTimer[i].x += (_float)fDeltaTime;

		if (m_vecTimer[i].x > 0) m_vecTimer[i].w = 2;

	}


	return _int();
}

_int CTestNonAnimInstancing::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)
		return -1;





	FAILED_CHECK(m_pRendererCom->Add_ShadowGroup_InstanceModel(CRenderer::INSTSHADOW_NONANIMINSTANCE, this,
		&m_vecInstancedTransform, m_pModelInstance, m_pShaderCom, m_pModel,nullptr,&m_vecTimer));
	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
	




	return _int();
}

_int CTestNonAnimInstancing::Render()
{
	if (__super::Render() < 0)
		return -1;


	NULL_CHECK_RETURN(m_pModel, E_FAIL);


	FAILED_CHECK(__super::SetUp_ConstTable(m_pShaderCom));

	FAILED_CHECK(GetSingle(CUtilityMgr)->Bind_DissolveTex_OnShader(m_pShaderCom, 1));
	FAILED_CHECK(m_pModelInstance->Render(m_pShaderCom, 2, &m_vecInstancedTransform,0,nullptr,nullptr, &m_vecTimer));

	return 0;
}

_int CTestNonAnimInstancing::LateRender()
{
	if (__super::LateRender() < 0)
		return -1;




	return _int();
}

HRESULT CTestNonAnimInstancing::SetUp_Components()
{

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VTXNONANIMINST), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_AlgaeRock_Ledge), TAG_COM(Com_Model), (CComponent**)&m_pModel));

	for (_uint i =0; i < 4; i++)
	{
		CTransform* pTransform = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
		NULL_CHECK_RETURN(pTransform, E_FAIL);
		pTransform->Set_MatrixState(CTransform::STATE_POS, _float3(10, 0, 0 + _float(i)*2.f));
		m_vecInstancedTransform.push_back(pTransform);	


		m_vecTimer.push_back(_float4(-1.f * i - 3.f, 3.f, 0, 0));
	}



	CModelInstance::MODELINSTDESC tModelIntDsec;

	tModelIntDsec.m_pTargetModel = m_pModel;

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_ModelInstance_64), TAG_COM(Com_ModelInstance), (CComponent**)&m_pModelInstance,&tModelIntDsec));

	

	return S_OK;
}

CTestNonAnimInstancing * CTestNonAnimInstancing::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CTestNonAnimInstancing*	pInstance = NEW CTestNonAnimInstancing(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CTestNonAnimInstancing");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CTestNonAnimInstancing::Clone(void * pArg)
{
	CTestNonAnimInstancing*	pInstance = NEW CTestNonAnimInstancing(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CTestNonAnimInstancing");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTestNonAnimInstancing::Free()
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

	
