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




	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	m_pPlayerTransformCom = static_cast<CTransform*>(pGameInstance->Get_Commponent_By_LayerIndex(m_eNowSceneNum, TAG_LAY(Layer_Player), TAG_COM(Com_Transform)));

	RELEASE_INSTANCE(CGameInstance);


	return S_OK;
}

_int CMonster_Lamp::Update(_double dDeltaTime)
{
	if (__super::Update(dDeltaTime) < 0)return -1;

	FollowMe(dDeltaTime);

	for (_int i = 0; i < ANIM_END; i++)
	{
		FAILED_CHECK(m_pModel[i]->Update_AnimationClip(dDeltaTime));
	}
	FAILED_CHECK(Adjust_AnimMovedTransform(dDeltaTime));

	return _int();
}

_int CMonster_Lamp::LateUpdate(_double dDeltaTime)
{
	if (__super::LateUpdate(dDeltaTime) < 0)return -1;



	for (_int i = 0; i < ANIM_END; i++)
	{
		FAILED_CHECK(m_pRendererCom->Add_ShadowGroup_InstanceModel(CRenderer::INSTSHADOW_ANIMINSTANCE, this, &m_ModelTransGroup[i] , m_pModelInstance[i], m_pShaderCom, m_pModel[i]));
	}


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

	for (_int i = 0; i < ANIM_END; i++)
	{
		FAILED_CHECK(m_pModelInstance[i]->Render(m_pShaderCom, 2, &m_ModelTransGroup[i]));
	}

	return _int();
}

_int CMonster_Lamp::LateRender()
{
	return _int();
}

HRESULT CMonster_Lamp::SetUp_Info()
{
	for (_uint i = 0; i < 4; i++)
	{
		TRANSFORM_STATE tDesc;
		tDesc.pTransform = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
		NULL_CHECK_RETURN(tDesc.pTransform, E_FAIL);
		tDesc.eType = ANIM_IDLE;

		CUtilityMgr* pUtil = GetSingle(CUtilityMgr);

		_float fSpeed = pUtil->RandomFloat(1, 2);

		tDesc.pTransform->Set_MoveSpeed(fSpeed);
		tDesc.pTransform->Set_MatrixState(CTransform::STATE_POS, _float3(0 + _float(i)*1.f, 0, 2));



		m_vecInstancedTransform.push_back(tDesc);
	}

	for (_uint i = 0; i < ANIM_END; i++)
	{
		m_pModel[i] = (CModel*)g_pGameInstance->Clone_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_Monster_Wasp));
		NULL_CHECK_RETURN(m_pModel[i], E_FAIL);

		CModelInstance::MODELINSTDESC tModelIntDsec;
		tModelIntDsec.m_pTargetModel = m_pModel[i];

		m_pModelInstance[i] = (CModelInstance*)g_pGameInstance->Clone_Component(m_eNowSceneNum, TAG_CP(Prototype_ModelInstance_4), &tModelIntDsec);
		NULL_CHECK_RETURN(m_pModelInstance[i], E_FAIL);
	}

	m_pModel[ANIM_IDLE]->Change_AnimIndex(0);
	m_pModel[ANIM_RUN]->Change_AnimIndex(0);
	m_pModel[ANIM_ATTACK]->Change_AnimIndex(2);
	m_pModel[ANIM_HIT]->Change_AnimIndex(3);
	m_pModel[ANIM_DIE]->Change_AnimIndex(4);

	return S_OK;
}

HRESULT CMonster_Lamp::FollowMe(_double dDeltaTime)
{
	for (_int i = 0; i < ANIM_END; i++)
	{
		m_ModelTransGroup[i].clear();
		m_ModelTransGroup[i].reserve(m_vecInstancedTransform.size());
	}

	for (auto& MeshInstance : m_vecInstancedTransform)
	{
		
		//_Vector vDistance = m_pPlayerTransformCom->Get_MatrixState(CTransform::STATE_POS) - MeshInstance.pTransform->Get_MatrixState(CTransform::STATE_POS);

		_Vector vTarget = XMVector3Normalize(m_pPlayerTransformCom->Get_MatrixState(CTransform::STATE_POS) - MeshInstance.pTransform->Get_MatrixState(CTransform::STATE_POS));

		MeshInstance.pTransform->Turn_Dir(vTarget, 0.9f);

		_float fDistance = MeshInstance.pTransform->Get_MatrixState_Float3(CTransform::STATE_POS).Get_Distance(m_pPlayerTransformCom->Get_MatrixState(CTransform::STATE_POS));

		MeshInstance.eType = ANIM_RUN;

		if (fDistance < 2)
		{
			MeshInstance.eType = ANIM_ATTACK;
		}

	}



	for (_uint i = 0; i < m_vecInstancedTransform.size(); i++)
	{

		switch (m_vecInstancedTransform[i].eType)
		{
		case ANIM_IDLE:
			m_ModelTransGroup[ANIM_IDLE].push_back(m_vecInstancedTransform[i].pTransform);
			break;
		case ANIM_RUN:
			m_ModelTransGroup[ANIM_RUN].push_back(m_vecInstancedTransform[i].pTransform);
			break;
		case ANIM_ATTACK:
			m_ModelTransGroup[ANIM_ATTACK].push_back(m_vecInstancedTransform[i].pTransform);
			break;
		case ANIM_HIT:
			m_ModelTransGroup[ANIM_HIT].push_back(m_vecInstancedTransform[i].pTransform);
			break;
		case ANIM_DIE:
			m_ModelTransGroup[ANIM_DIE].push_back(m_vecInstancedTransform[i].pTransform);
			break;
		default:
			break;
		}


	}


	return S_OK;
}



HRESULT CMonster_Lamp::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VTXANIMINST), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));


	SetUp_Info();


	return S_OK;
}

HRESULT CMonster_Lamp::Adjust_AnimMovedTransform(_double dDeltatime)
{

	
	for (auto& pObjectTransform : m_ModelTransGroup[ANIM_RUN])
	{
		pObjectTransform->Move_Forward(dDeltatime);
	}

	//_uint iNowAnimIndex = m_pModel->Get_NowAnimIndex();
	//_double PlayRate = m_pModel->Get_PlayRate();

	//if (iNowAnimIndex != m_iOldAnimIndex || PlayRate > 0.98)
	//	m_iAdjMovedIndex = 0;


	//if (PlayRate <= 0.98)
	//{
	//	//switch (iNowAnimIndex)
	//	//{
	//	//case 1://애니메이션 인덱스마다 잡아주면 됨
	//	//	if (m_iAdjMovedIndex == 0 && PlayRate > 0.0) // 이렇게 되면 이전 애니메이션에서 보간되는 시간 끝나자 마자 바로 들어옴
	//	//	{

	//	//		m_iAdjMovedIndex++;
	//	//	}
	//	//	else if (m_iAdjMovedIndex == 1 && PlayRate > 0.7666666666666666) //특정 프레임 플레이 레이트이후에 들어오면실행
	//	//	{


	//	//		m_iAdjMovedIndex++;
	//	//	}

	//	//	break;
	//	//case 2:

	//	//	break;
	//	//}
	//}


	//m_iOldAnimIndex = iNowAnimIndex;
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
		Safe_Release(pTransform.pTransform);
	m_vecInstancedTransform.clear();

	for (_int i = 0; i < ANIM_END; i++)
	{
		Safe_Release(m_pModelInstance[i]);
	}

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);

	for (_int i = 0; i < ANIM_END; i++)
	{
		Safe_Release(m_pModel[i]);
	}
}
