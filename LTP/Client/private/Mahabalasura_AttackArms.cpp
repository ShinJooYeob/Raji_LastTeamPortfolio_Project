#include "stdafx.h"
#include "..\public\Mahabalasura_AttackArms.h"

CMahabalasura_AttackArms::CMahabalasura_AttackArms(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CMonsterWeapon(pDevice, pDeviceContext)
{
}

CMahabalasura_AttackArms::CMahabalasura_AttackArms(const CMahabalasura_AttackArms & rhs)
	: CMonsterWeapon(rhs)
{
}

HRESULT CMahabalasura_AttackArms::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));


	return S_OK;
}

HRESULT CMahabalasura_AttackArms::Initialize_Clone(void * pArg)
{

	NULL_CHECK_RETURN(pArg, E_FAIL);

	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components());

	_float3 BossPos = ((CTransform*)g_pGameInstance->Get_Commponent_By_LayerIndex(m_eNowSceneNum, TAG_LAY(Layer_Boss), TAG_COM(Com_Transform)))->Get_MatrixState(CTransform::STATE_POS);
	m_pPlayerTrs = (CTransform*)g_pGameInstance->Get_Commponent_By_LayerIndex(m_eNowSceneNum, TEXT("Layer_Player"), TAG_COM(Com_Transform));
	NULL_CHECK_RETURN(m_pPlayerTrs, E_FAIL);
	m_PlayerPos = (m_pPlayerTrs)->Get_MatrixState(CTransform::STATE_POS);
	m_BezierStartPos = BossPos;

	CUtilityMgr* pUtil =  GetSingle(CUtilityMgr);
	CGameInstance* pIs = g_pGameInstance;



	for (_int i = 0; i < AtkArmTotalCount; ++i)
	{
		TTD tDesc;

		_float Angle = (_float)i*(360.f / (_float)AtkArmTotalCount);
		_Matrix Mat = XMMatrixRotationY(XMConvertToRadians(Angle));
		_float3 Pos = XMLoadFloat3(&BossPos) + (Mat.r[2] * 5.f);
		Pos.y += 5.f;

		tDesc.pTransform = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
		NULL_CHECK_RETURN(tDesc.pTransform, E_FAIL);

		tDesc.pTransform->Set_MatrixState(CTransform::STATE_POS, Pos);
		tDesc.pTransform->Scaled_All(_float3(3.f));

		tDesc.bIsDead = false;
		tDesc.fPassedTime = -1.f+ (_float)( (_float)i / _float(AtkArmTotalCount));

		m_StartPos[i] = Pos;
		m_vecInstancedTransform.push_back(tDesc);
	}


	//Set_LimLight_N_Emissive(_float4(255.f, 0.f, 20.f, 255.f), _float4(0));

	return S_OK;
}

_int CMahabalasura_AttackArms::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0) return -1;

	
	//m_pTransformCom->BezierCurve(m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), XMLoadFloat3(&m_PlayerPos), XMLoadFloat3(&CenterPoint), m_fProgressTime, 1.0, fDeltaTime);

	//m_fProgressTime += (_float)fDeltaTime;
	//if (m_pTransformCom->MovetoBezierCurve(m_fProgressTime, XMLoadFloat3(&m_BezierStartPos), XMLoadFloat3(&m_CenterPoint), XMLoadFloat3(&m_PlayerPos)))
	//{
	//	//이펙트 생성 할 곳

	//	Set_IsDead();
	//}


	CUtilityMgr* pUtil = GetSingle(CUtilityMgr);


	for (_int i = 0; i < m_vecInstancedTransform.size(); ++i)
	{
		m_vecInstancedTransform[i].fPassedTime += (_float)fDeltaTime;
		_float Length = m_StartPos[i].Get_Distance(m_DestPos[i].XMVector());
		_float Rate = max((1.0f - (10.0f - Length) * 0.1f)*0.5f, 0);


		
		
		if (m_vecInstancedTransform[i].fPassedTime <= 0)
		{

			continue;
		}
		else
		{
			if (m_vecInstancedTransform[i].fPassedTime - (_float)fDeltaTime <= 0)
			{

				m_DestPos[i] = m_pPlayerTrs->Get_MatrixState_Float3(CTransform::STATE_POS);
				m_vecInstancedTransform[i].pTransform->LookAt(m_DestPos[i].XMVector());
				m_vecInstancedTransform[i].pTransform->Turn_CCW(XMVectorSet(0, 1.f, 0, 0), XMConvertToRadians(20.f));

			}
			else
			{
				
				if (m_vecInstancedTransform[i].fPassedTime > m_fTotalTime * Rate + 1.f)
				{

					m_vecInstancedTransform[i].bIsDead = true;
					continue;
				}
			}

		}


		_float3 Pos;

		if (m_vecInstancedTransform[i].fPassedTime < 1)
		{
			m_DestPos[i] = m_pPlayerTrs->Get_MatrixState_Float3(CTransform::STATE_POS);
			m_vecInstancedTransform[i].pTransform->LookAt(m_DestPos[i].XMVector());
			m_vecInstancedTransform[i].pTransform->Turn_CCW(XMVectorSet(0, 1.f, 0, 0), XMConvertToRadians(20.f));

		}
		else
		{



			Pos.x = g_pGameInstance->Easing(TYPE_Linear, m_StartPos[i].x, m_DestPos[i].x, m_vecInstancedTransform[i].fPassedTime -1 , m_fTotalTime * Rate);


			if (m_vecInstancedTransform[i].fPassedTime - 1 < m_fTotalTime * 0.4 *Rate)
			{
				Pos.y = g_pGameInstance->Easing(TYPE_QuadOut, m_StartPos[i].y, m_StartPos[i].Get_Distance(m_DestPos[i].XMVector()) * 0.3875f,
					m_vecInstancedTransform[i].fPassedTime - 1, m_fTotalTime *0.4f *Rate);
			}
			else
			{
				Pos.y = g_pGameInstance->Easing(TYPE_SinIn, m_StartPos[i].Get_Distance(m_DestPos[i].XMVector())*0.3875f, m_DestPos[i].y,
					m_vecInstancedTransform[i].fPassedTime - 1 - (m_fTotalTime *0.4f)*Rate, m_fTotalTime *0.6f * Rate);
			}

			Pos.z = g_pGameInstance->Easing(TYPE_Linear, m_StartPos[i].z, m_DestPos[i].z, m_vecInstancedTransform[i].fPassedTime - 1, m_fTotalTime*Rate);

			m_vecInstancedTransform[i].pTransform->Set_MatrixState(CTransform::STATE_POS, Pos);
			//m_vecInstancedTransform[i].pTransform->LookAt(m_DestPos[i].XMVector());
			//m_vecInstancedTransform[i].pTransform->Turn_CCW(XMVectorSet(0, 1.f, 0, 0), XMConvertToRadians(20.f));
		}
	}

	
	//m_bIsOnScreen = g_pGameInstance->IsNeedToRender(m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS), m_fFrustumRadius);
	FAILED_CHECK(m_pModel->Update_AnimationClip(fDeltaTime * (m_fAnimmultiple)));
	//FAILED_CHECK(Adjust_AnimMovedTransform(fDeltaTime));

	return _int();
}

_int CMahabalasura_AttackArms::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)return -1;

	m_vecForRenderTransform.clear();
	m_vecForRenderTransform.reserve(m_vecInstancedTransform.size());

	_uint iDeadCount = 0;

	for (auto& tDesc : m_vecInstancedTransform)
	{
		if (tDesc.fPassedTime <= 0)continue;

		if (tDesc.bIsDead)
		{
			iDeadCount++;
			continue;
		}

		m_vecForRenderTransform.push_back(tDesc.pTransform);

	}

	if (iDeadCount >= AtkArmTotalCount)Set_IsDead();

	FAILED_CHECK(m_pRendererCom->Add_ShadowGroup_InstanceModel(CRenderer::INSTSHADOW_ANIMINSTANCE, this, &m_vecForRenderTransform, m_pModelInstance,m_pShaderCom,m_pModel));
	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
	return _int();
}

_int CMahabalasura_AttackArms::Render()
{
	if (__super::Render() < 0)		return -1;

	NULL_CHECK_RETURN(m_pModel, E_FAIL);

	CGameInstance* pInstance = GetSingle(CGameInstance);

	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));

	FAILED_CHECK(m_pModelInstance->Render(m_pShaderCom, 2, &m_vecForRenderTransform));

	return _int();
}

_int CMahabalasura_AttackArms::LateRender()
{
	return _int();
}

HRESULT CMahabalasura_AttackArms::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VTXANIMINST), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_Boss_MahabalasuraAttackArms), TAG_COM(Com_Model), (CComponent**)&m_pModel));

	CModelInstance::MODELINSTDESC tDesc;
	tDesc.m_pTargetModel = m_pModel;

	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_ModelInstance_8), TAG_COM(Com_ModelInstance), (CComponent**)&m_pModelInstance, &tDesc));


	return S_OK;
}

HRESULT CMahabalasura_AttackArms::Adjust_AnimMovedTransform(_double fDeltatime)
{
	//_uint iNowAnimIndex = m_pModel->Get_NowAnimIndex();
	//_double PlayRate = m_pModel->Get_PlayRate();

	//if (iNowAnimIndex != m_iOldAnimIndex || PlayRate > 0.98)
	//	m_iAdjMovedIndex = 0;


	//if (PlayRate <= 0.98)
	//{

	//}
	//else
	//{

	//}

	//m_iOldAnimIndex = iNowAnimIndex;
	return S_OK;
}

CMahabalasura_AttackArms * CMahabalasura_AttackArms::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CMahabalasura_AttackArms*	pInstance = new CMahabalasura_AttackArms(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CMahabalasura_AttackArms");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMahabalasura_AttackArms::Clone(void * pArg)
{
	CMahabalasura_AttackArms*	pInstance = new CMahabalasura_AttackArms(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created  Clone CMahabalasura_AttackArms");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMahabalasura_AttackArms::Free()
{
	__super::Free();

	for (auto& Desc: m_vecInstancedTransform)
		Safe_Release(Desc.pTransform);
	m_vecInstancedTransform.clear();


	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModel);
	Safe_Release(m_pModelInstance);
}
