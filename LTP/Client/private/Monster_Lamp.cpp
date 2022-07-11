#include "stdafx.h"
#include "..\public\Monster_Lamp.h"


CMonster_Lamp::CMonster_Lamp(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CMonster(pDevice, pDeviceContext)
{
}

CMonster_Lamp::CMonster_Lamp(const CMonster_Lamp & rhs)
	: CMonster(rhs)
{
}

HRESULT CMonster_Lamp::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));


	return S_OK;
}

HRESULT CMonster_Lamp::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components());



	SetUp_Info();


	return S_OK;
}

_int CMonster_Lamp::Update(_double dDeltaTime)
{
	if (__super::Update(dDeltaTime) < 0)return -1;

	FollowMe(dDeltaTime);


	FAILED_CHECK(m_pModel->Update_AnimationClip(dDeltaTime));
	FAILED_CHECK(Adjust_AnimMovedTransform(dDeltaTime));

	return _int();
}

_int CMonster_Lamp::LateUpdate(_double dDeltaTime)
{
	if (__super::LateUpdate(dDeltaTime) < 0)return -1;


	FAILED_CHECK(m_pRendererCom->Add_ShadowGroup_InstanceModel(CRenderer::INSTSHADOW_ANIMINSTANCE, this, &m_vecInstancedTransform, m_pModelInstance, m_pShaderCom, m_pModel));

	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));

	return _int();
}

_int CMonster_Lamp::Render()
{
	if (__super::Render() < 0)		return -1;

	NULL_CHECK_RETURN(m_pModel, E_FAIL);

	CGameInstance* pInstance = GetSingle(CGameInstance);
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));

	FAILED_CHECK(m_pModelInstance->Render(m_pShaderCom, 2, &m_vecInstancedTransform));

	return _int();
}

_int CMonster_Lamp::LateRender()
{
	return _int();
}

HRESULT CMonster_Lamp::SetUp_Info()
{

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	m_pPlayerTransformCom = static_cast<CTransform*>(pGameInstance->Get_Commponent_By_LayerIndex(m_eNowSceneNum, TAG_LAY(Layer_Player), TAG_COM(Com_Transform)));

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CMonster_Lamp::FollowMe(_double dDeltaTime)
{

	for (auto& MeshInstance : m_vecInstancedTransform)
	{
		MeshInstance->LookAt(m_pPlayerTransformCom->Get_MatrixState(CTransform::STATE_POS));
		//_Vector vDistance = m_pPlayerTransformCom->Get_MatrixState(CTransform::STATE_POS) - MeshInstance->Get_MatrixState(CTransform::STATE_POS);

		MeshInstance->Move_Forward(dDeltaTime);
	}


	return S_OK;
}



HRESULT CMonster_Lamp::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VTXANIMINST), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	// MODELCOM_NAME
	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_Monster_Lamp), TAG_COM(Com_Model), (CComponent**)&m_pModel));

	for (_uint i = 0; i < 4; i++)
	{
		CTransform* pTransform = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
		NULL_CHECK_RETURN(pTransform, E_FAIL);

		pTransform->Set_MoveSpeed(1.5f);

		pTransform->Set_MatrixState(CTransform::STATE_POS, _float3(0 + _float(i)*1.f, 0, 2));
		m_vecInstancedTransform.push_back(pTransform);
	}

	CModelInstance::MODELINSTDESC tModelIntDsec;
	tModelIntDsec.m_pTargetModel = m_pModel;
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_ModelInstance_4), TAG_COM(Com_ModelInstance), (CComponent**)&m_pModelInstance, &tModelIntDsec));


	return S_OK;
}

HRESULT CMonster_Lamp::Adjust_AnimMovedTransform(_double fDeltatime)
{
	_uint iNowAnimIndex = m_pModel->Get_NowAnimIndex();
	_double PlayRate = m_pModel->Get_PlayRate();

	if (iNowAnimIndex != m_iOldAnimIndex || PlayRate > 0.98)
		m_iAdjMovedIndex = 0;


	if (PlayRate <= 0.98)
	{
		//switch (iNowAnimIndex)
		//{
		//case 1://애니메이션 인덱스마다 잡아주면 됨
		//	if (m_iAdjMovedIndex == 0 && PlayRate > 0.0) // 이렇게 되면 이전 애니메이션에서 보간되는 시간 끝나자 마자 바로 들어옴
		//	{

		//		m_iAdjMovedIndex++;
		//	}
		//	else if (m_iAdjMovedIndex == 1 && PlayRate > 0.7666666666666666) //특정 프레임 플레이 레이트이후에 들어오면실행
		//	{


		//		m_iAdjMovedIndex++;
		//	}

		//	break;
		//case 2:

		//	break;
		//}
	}


	m_iOldAnimIndex = iNowAnimIndex;
	return S_OK;
}

CMonster_Lamp * CMonster_Lamp::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CMonster_Lamp*	pInstance = new CMonster_Lamp(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CMonster_Lamp");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMonster_Lamp::Clone(void * pArg)
{
	CMonster_Lamp*	pInstance = new CMonster_Lamp(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CMonster_Lamp");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMonster_Lamp::Free()
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
