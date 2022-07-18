#include "stdafx.h"
#include "..\public\Monster_Wasp.h"


CMonster_Wasp::CMonster_Wasp(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CMonster(pDevice, pDeviceContext)
{
}

CMonster_Wasp::CMonster_Wasp(const CMonster_Wasp & rhs)
	: CMonster(rhs)
{
}

HRESULT CMonster_Wasp::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	for (auto& p : m_pModel)	p = nullptr;
	for (auto& p : m_pModelInstance)	p = nullptr;
	


	return S_OK;
}

HRESULT CMonster_Wasp::Initialize_Clone(void * pArg)
{

	for (auto& p : m_pModel)	p = nullptr;
	for (auto& p : m_pModelInstance)	p = nullptr;


	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components());

	return S_OK;
}

_int CMonster_Wasp::Update(_double dDeltaTime)
{
	if (__super::Update(dDeltaTime) < 0)return -1;

	FollowMe(dDeltaTime);

	for (_int i = 0; i < ANIM_END; i++)
	{
		if (i >= ANIM_ATTACK_Frame1 && i <= ANIM_ATTACK_Frame5)
		{
			m_pModel[i]->Update_AnimationClip(dDeltaTime * m_dAcceleration);
		}
		else {
			m_pModel[i]->Update_AnimationClip(dDeltaTime);
		}
	}



	FAILED_CHECK(Adjust_AnimMovedTransform(dDeltaTime));
	for (size_t i = 0; i < m_vecInstancedTransform.size(); i++)
	{
		if (m_vecInstancedTransform[i].iType >= ANIM_RUN_Frame1 && m_vecInstancedTransform[i].iType <= ANIM_RUN_Frame2)
			FAILED_CHECK(g_pGameInstance->Add_RepelGroup(m_vecInstancedTransform[i].pTransform, 0.5f));

	}

	return _int();
}

_int CMonster_Wasp::LateUpdate(_double dDeltaTime)
{
	if (__super::LateUpdate(dDeltaTime) < 0)return -1;



	for (_int i = 0; i < ANIM_END; i++)
	{
		FAILED_CHECK(m_pRendererCom->Add_ShadowGroup_InstanceModel(CRenderer::INSTSHADOW_ANIMINSTANCE, this, &m_ModelTransGroup[i], m_pModelInstance[i], m_pShaderCom, m_pModel[i]));
	}


	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));

	return _int();
}

_int CMonster_Wasp::Render()
{
	if (__super::Render() < 0)		return -1;

	NULL_CHECK_RETURN(m_pModel, E_FAIL);

	CGameInstance* pInstance = GetSingle(CGameInstance);
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));

	for (_int i = 0; i < ANIM_END; i++)
	{
		if (m_ModelTransGroup[i].size() == 0)
			continue;
		FAILED_CHECK(m_pModelInstance[i]->Render(m_pShaderCom, 2, &m_ModelTransGroup[i]));
	}

	return _int();
}

_int CMonster_Wasp::LateRender()
{
	return _int();
}

HRESULT CMonster_Wasp::SetUp_Info()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	m_pPlayerTransformCom = static_cast<CTransform*>(pGameInstance->Get_Commponent_By_LayerIndex(m_eNowSceneNum, TAG_LAY(Layer_Player), TAG_COM(Com_Transform)));

	RELEASE_INSTANCE(CGameInstance);

	for (_uint i = 0; i < 64; i++)
	{
		TRANSFORM_STATE tDesc;
		tDesc.pTransform = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
		NULL_CHECK_RETURN(tDesc.pTransform, E_FAIL);
		tDesc.iType = rand() % 2;

		CUtilityMgr* pUtil = GetSingle(CUtilityMgr);

		_float fSpeed = pUtil->RandomFloat(2, 3);

		tDesc.pTransform->Set_MoveSpeed(fSpeed);

		_Vector vDis = (m_pPlayerTransformCom->Get_MatrixState(CTransform::STATE_LOOK) * pUtil->RandomFloat(-1, 1) + m_pPlayerTransformCom->Get_MatrixState(CTransform::STATE_RIGHT) * pUtil->RandomFloat(-1, 1));

		_Vector PlayerPos = m_pPlayerTransformCom->Get_MatrixState(CTransform::STATE_POS);

		PlayerPos = PlayerPos + (XMVector3Normalize(vDis) * pUtil->RandomFloat(2, 5));

		tDesc.pTransform->Set_MatrixState(CTransform::STATE_POS, PlayerPos);


		//////////Navigation
		CNavigation::NAVIDESC		NaviDesc;
		NaviDesc.iCurrentIndex = 0;

		tDesc.pNavigation = (CNavigation*)g_pGameInstance->Clone_Component(m_eNowSceneNum, TAG_CP(Prototype_Navigation), &NaviDesc);

		tDesc.pNavigation->FindCellIndex(tDesc.pTransform->Get_MatrixState(CTransform::TransformState::STATE_POS));
		///////////////

		m_vecInstancedTransform.push_back(tDesc);
	}

	for (_uint i = 0; i < ANIM_END; i++)
	{
		CModel* pModel = (CModel*)g_pGameInstance->Clone_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_Monster_Wasp));
		NULL_CHECK_RETURN(pModel, E_FAIL);
		m_pModel[i] = pModel;

		CModelInstance::MODELINSTDESC tModelIntDsec;
		tModelIntDsec.m_pTargetModel = pModel;

		CModelInstance* pModelInstance = (CModelInstance*)g_pGameInstance->Clone_Component(m_eNowSceneNum, TAG_CP(Prototype_ModelInstance_64), &tModelIntDsec);
		NULL_CHECK_RETURN(pModelInstance, E_FAIL);
		m_pModelInstance[i] = pModelInstance;
	}


	for (_int i = 0; i < ANIM_END; i++)
	{
		if (i == ANIM_RUN_Frame1 && i <= ANIM_RUN_Frame2)
		{
			m_pModel[i]->Change_AnimIndex(0);
		}
		else if (i >= ANIM_ATTACK_Frame1 && i <= ANIM_ATTACK_Frame5)
		{
			m_pModel[i]->Change_AnimIndex(2);
		}
	}

	_int	iNumber = 0;
	_double	dpercent = 0.85;

	for (_uint i = ANIM_RUN_Frame1; i <= ANIM_RUN_Frame2; i++)
	{
		m_pModel[i]->Update_AnimationClip(iNumber * dpercent);

		iNumber++;
	}

	iNumber = 0;
	dpercent = 0.2;
	for (_uint i = ANIM_ATTACK_Frame1; i <= ANIM_ATTACK_Frame5; i++)
	{
		m_pModel[i]->Update_AnimationClip(iNumber * dpercent);

		iNumber++;
	}

	return S_OK;
}

HRESULT CMonster_Wasp::FollowMe(_double dDeltaTime)
{
	for (_int i = 0; i < ANIM_END; i++)
	{
		m_ModelTransGroup[i].clear();
		m_ModelTransGroup[i].reserve(m_vecInstancedTransform.size());
	}


	m_iTempAnimNumber = ANIM_ATTACK_Frame1;
	for (_uint i = ANIM_ATTACK_Frame2; i <= ANIM_ATTACK_Frame5; i++)
	{
		if (m_pModel[m_iTempAnimNumber]->Get_PlayRate() < m_pModel[i]->Get_PlayRate())
		{
			m_iTempAnimNumber = m_iTempAnimNumber;
		}
		else {
			m_iTempAnimNumber = i;
		}
	}

	for (auto& MeshInstance : m_vecInstancedTransform)
	{

		_Vector vTarget = XMVector3Normalize(m_pPlayerTransformCom->Get_MatrixState(CTransform::STATE_POS) - MeshInstance.pTransform->Get_MatrixState(CTransform::STATE_POS));

		MeshInstance.pTransform->Turn_Dir(vTarget, 0.9f);
		_float fDistance = MeshInstance.pTransform->Get_MatrixState_Float3(CTransform::STATE_POS).Get_Distance(m_pPlayerTransformCom->Get_MatrixState(CTransform::STATE_POS));


		MeshInstance.pTransform->Set_MatrixState(CTransform::STATE_POS, MeshInstance.pNavigation->Get_NaviPosition(MeshInstance.pTransform->Get_MatrixState(CTransform::STATE_POS)));


		if (fDistance < 1.5)
		{
			if (MeshInstance.iType >= ANIM_ATTACK_Frame1&& MeshInstance.iType <= ANIM_ATTACK_Frame5)
				continue;
			MeshInstance.iType = m_iTempAnimNumber;
		}
		else {
			if (MeshInstance.iType >= ANIM_ATTACK_Frame1&& MeshInstance.iType <= ANIM_ATTACK_Frame5)
			{
				if (m_pModel[MeshInstance.iType]->Get_PlayRate() > 0.95)
				{
					MeshInstance.iType = rand() % 2;
				}
			}
			//else {
			//	MeshInstance.iType = rand() % 2;
			//}
		}

	}



	for (_uint i = 0; i < m_vecInstancedTransform.size(); i++)
	{

		switch (m_vecInstancedTransform[i].iType)
		{
		case ANIM_RUN_Frame1:
			m_ModelTransGroup[ANIM_RUN_Frame1].push_back(m_vecInstancedTransform[i].pTransform);
			break;
		case ANIM_RUN_Frame2:
			m_ModelTransGroup[ANIM_RUN_Frame2].push_back(m_vecInstancedTransform[i].pTransform);
			break;
		case ANIM_ATTACK_Frame1:
			m_ModelTransGroup[ANIM_ATTACK_Frame1].push_back(m_vecInstancedTransform[i].pTransform);
			break;
		case ANIM_ATTACK_Frame2:
			m_ModelTransGroup[ANIM_ATTACK_Frame2].push_back(m_vecInstancedTransform[i].pTransform);
			break;
		case ANIM_ATTACK_Frame3:
			m_ModelTransGroup[ANIM_ATTACK_Frame3].push_back(m_vecInstancedTransform[i].pTransform);
			break;
		case ANIM_ATTACK_Frame4:
			m_ModelTransGroup[ANIM_ATTACK_Frame4].push_back(m_vecInstancedTransform[i].pTransform);
			break;
		case ANIM_ATTACK_Frame5:
			m_ModelTransGroup[ANIM_ATTACK_Frame5].push_back(m_vecInstancedTransform[i].pTransform);
			break;
		default:
			break;
		}


	}


	return S_OK;
}



HRESULT CMonster_Wasp::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VTXANIMINST), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));


	SetUp_Info();


	return S_OK;
}

HRESULT CMonster_Wasp::Adjust_AnimMovedTransform(_double dDeltatime)
{
	for (auto & pInstance : m_vecInstancedTransform)
	{
		switch (pInstance.iType)
		{
		case ANIM_RUN_Frame1:
			pInstance.pTransform->Move_Forward(dDeltatime, pInstance.pNavigation);
			break;
		case ANIM_RUN_Frame2:
			pInstance.pTransform->Move_Forward(dDeltatime, pInstance.pNavigation);
			break;
		default:
		{
			if (m_pModel[pInstance.iType]->Get_PlayRate() > 0.4)
			{
				pInstance.pTransform->Move_Forward(dDeltatime, pInstance.pNavigation);
			}
			break;
		}
		}
		//if (pInstance.iType == ANIM_RUN_Frame1 || pInstance.iType == ANIM_RUN_Frame2)
		//{
		//	pInstance.pTransform->Move_Forward(dDeltatime, pInstance.pNavigation);
		//}
	}

	//for (_uint i = ANIM_RUN_Frame1; i <= ANIM_RUN_Frame2; i++)
	//{
	//	for (auto& pObjectTransform : m_ModelTransGroup[i])
	//	{
	//		pObjectTransform->Move_Forward(dDeltatime, );
	//	}
	//}

	//for (_uint i = ANIM_ATTACK_Frame1; i <= ANIM_ATTACK_Frame5; i++)
	//{
	//	for (auto& pObjectTransform : m_ModelTransGroup[i])
	//	{
	//		if (m_pModel[i]->Get_PlayRate() > 0.4)
	//		{
	//			pObjectTransform->Move_Forward(dDeltatime);
	//		}
	//	}
	//}

	return S_OK;
}

CMonster_Wasp * CMonster_Wasp::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CMonster_Wasp*	pInstance = NEW CMonster_Wasp(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CMonster_Wasp");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMonster_Wasp::Clone(void * pArg)
{
	CMonster_Wasp*	pInstance = NEW CMonster_Wasp(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CMonster_Wasp");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMonster_Wasp::Free()
{
	__super::Free();

	for (auto& pTransform : m_vecInstancedTransform)
	{
		Safe_Release(pTransform.pTransform);
		Safe_Release(pTransform.pNavigation);
	}
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
