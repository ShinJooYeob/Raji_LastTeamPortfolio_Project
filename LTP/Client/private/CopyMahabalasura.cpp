#include "stdafx.h"
#include "..\public\CopyMahabalasura.h"
#include "Mahabalasura.h"

CCopyMahabalasura::CCopyMahabalasura(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CBoss(pDevice, pDeviceContext)
{
}


CCopyMahabalasura::CCopyMahabalasura(const CCopyMahabalasura & rhs)
	: CBoss(rhs)
{
}

HRESULT CCopyMahabalasura::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));


	return S_OK;
}

HRESULT CCopyMahabalasura::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	m_pPlayerObj = (CGameObject*)g_pGameInstance->Get_GameObject_By_LayerIndex(m_eNowSceneNum, TEXT("Layer_Player"));

	m_pBossObj = (CMahabalasura*)g_pGameInstance->Get_GameObject_By_LayerIndex(m_eNowSceneNum, TAG_LAY(Layer_Boss));

	FAILED_CHECK(SetUp_Components());

	m_fAttackCoolTime = 4.f;

	m_bIsHit = false;

	//m_pTransformCom->Scaled_All(_float3(1.5f));

	m_iRandomIndex = rand() % 16;

	CTransform* PlayerTransform = (CTransform*)m_pPlayerObj->Get_Component(TAG_COM(Com_Transform));
	_float3 PlayerPos = PlayerTransform->Get_MatrixState(CTransform::STATE_POS);
	m_startPos = PlayerPos;

	return S_OK;
}

_int CCopyMahabalasura::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)return -1;

	m_fAttackCoolTime -= (_float)fDeltaTime;

	if (m_fAttackCoolTime <= 0 && !m_bIsAttack)
	{
		m_bIsAttack = true;
		m_pModel->Change_AnimIndex_ReturnTo(1, 0);

	}

	CTransform* PlayerTransform = (CTransform*)m_pPlayerObj->Get_Component(TAG_COM(Com_Transform));
	_float3 PlayerPos = PlayerTransform->Get_MatrixState(CTransform::STATE_POS);

	CTransform* BossTransform = (CTransform*)m_pBossObj->Get_Component(TAG_COM(Com_Transform));
	_float3 BossPos = BossTransform->Get_MatrixState(CTransform::STATE_POS);

	for (_int i = 0; i < m_vecInstancedTransform.size(); ++i)
	{
		m_vecInstancedTransform[i]->LookAt(XMLoadFloat3(&m_startPos));

		if (i % 2 == 0)
		{
			m_vecInstancedTransform[i]->Move_Left(fDeltaTime);
		}
		else if (i % 2 == 1)
		{
			m_vecInstancedTransform[i]->Move_Right(fDeltaTime);
		}
	}

	//나중에 충돌로 해당 인덱스일때 다 사라지게 하고 보스를 그위치에서 나오게끔한다.
	if (g_pGameInstance->Get_DIKeyState(DIK_5 & DIS_Down))
	{
		CTransform* BossTransform = (CTransform*)m_pBossObj->Get_Component(TAG_COM(Com_Transform));
		_float3 BossPos = BossTransform->Get_MatrixState(CTransform::STATE_POS);

		_float3 CopyPos = m_vecInstancedTransform[m_iRandomIndex]->Get_MatrixState(CTransform::STATE_POS);

		BossTransform->Set_MatrixState(CTransform::STATE_POS, CopyPos);
		m_pBossObj->Set_CopyOff(false);
		m_pBossObj->Set_Hit();

		Set_IsDead();
	}
	//m_bIsOnScreen = g_pGameInstance->IsNeedToRender(m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS), m_fFrustumRadius);
	FAILED_CHECK(m_pModel->Update_AnimationClip(fDeltaTime * (m_fAnimmultiple), m_bIsOnScreen));
	FAILED_CHECK(Adjust_AnimMovedTransform(fDeltaTime));

	return _int();
}

_int CCopyMahabalasura::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)return -1;


	//FAILED_CHECK(m_pRendererCom->Add_ShadowGroup(CRenderer::SHADOW_ANIMMODEL, this, m_pTransformCom, m_pShaderCom, m_pModel));
	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
	//m_vOldPos = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS);
	//g_pGameInstance->Set_TargetPostion(PLV_PLAYER, m_vOldPos);

	return _int();
}

_int CCopyMahabalasura::Render()
{
	if (__super::Render() < 0)		return -1;

	NULL_CHECK_RETURN(m_pModel, E_FAIL);

	CGameInstance* pInstance = GetSingle(CGameInstance);
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));

	CTransform* PlayerTransform = (CTransform*)m_pPlayerObj->Get_Component(TAG_COM(Com_Transform));
	_float3 PlayerPos = PlayerTransform->Get_MatrixState(CTransform::STATE_POS);

	for (_int i = 0; i < m_vecInstancedTransform.size(); ++i)
	{
		m_vecInstancedTransform[i]->LookAt(XMLoadFloat3(&PlayerPos));
	}


	FAILED_CHECK(m_pModelInstance->Render(m_pShaderCom, 2, &m_vecInstancedTransform));


	//FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));

	//_uint NumMaterial = m_pModel->Get_NumMaterial();

	//for (_uint i = 0; i < NumMaterial; i++)
	//{
	//	//if(i == 10)continue;

	//	for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; j++)
	//		FAILED_CHECK(m_pModel->Bind_OnShader(m_pShaderCom, i, j, MODLETEXTYPE(j)));

	//	FAILED_CHECK(m_pModel->Render(m_pShaderCom, 3, i, "g_BoneMatrices"));
	//}

	return _int();
}

_int CCopyMahabalasura::LateRender()
{
	if (__super::LateRender() < 0)		return -1;

	return _int();
}

HRESULT CCopyMahabalasura::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VTXANIMINST), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_Boss_MahabalasurCopy), TAG_COM(Com_Model), (CComponent**)&m_pModel));
	FAILED_CHECK(m_pModel->Change_AnimIndex(0));

	for (_uint i = 0; i < 16; i++)
	{
		CTransform* pTransform = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
		NULL_CHECK_RETURN(pTransform, E_FAIL);
		
		pTransform->Set_MoveSpeed(1.5f);

		CTransform* BossTransform = (CTransform*)m_pBossObj->Get_Component(TAG_COM(Com_Transform));
		_float3 BossPos = BossTransform->Get_MatrixState(CTransform::STATE_POS);

		pTransform->Set_MatrixState(CTransform::STATE_POS, _float3(GetSingle(CUtilityMgr)->RandomFloat(-10.5f, 10.5f), BossPos.y, GetSingle(CUtilityMgr)->RandomFloat(-10.5f, 10.5f)));
		//pTransform->Set_MatrixState(CTransform::STATE_POS, _float3(rand()& 6+1 * iTemp, BossPos.y, rand() & 6 + 1 * iTemp));
		m_vecInstancedTransform.push_back(pTransform);
	}

	CModelInstance::MODELINSTDESC tModelIntDsec;
	tModelIntDsec.m_pTargetModel = m_pModel;
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_ModelInstance_16), TAG_COM(Com_ModelInstance), (CComponent**)&m_pModelInstance, &tModelIntDsec));


	//CTransform::TRANSFORMDESC tDesc = {};

	//tDesc.fMovePerSec = 5;
	//tDesc.fRotationPerSec = XMConvertToRadians(60);
	//tDesc.fScalingPerSec = 1;
	//tDesc.vPivot = _float3(0, 0, 0);

	//FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));


	return S_OK;
}

HRESULT CCopyMahabalasura::Adjust_AnimMovedTransform(_double fDeltatime)
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

		case 5:

		break;


		}
	}
	else
	{
		if (iNowAnimIndex == 0)
		{
		}
		if (iNowAnimIndex == 1)
		{
			m_bIsAttack = false;
			m_fAttackCoolTime = 4.f;
		}
	}

	m_iOldAnimIndex = iNowAnimIndex;
	return S_OK;
}

CCopyMahabalasura * CCopyMahabalasura::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CCopyMahabalasura*	pInstance = NEW CCopyMahabalasura(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CCopyMahabalasura");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CCopyMahabalasura::Clone(void * pArg)
{
	CCopyMahabalasura*	pInstance = NEW CCopyMahabalasura(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created  Clone CCopyMahabalasura");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCopyMahabalasura::Free()
{
	__super::Free();

	//Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModel);
	Safe_Release(m_pModelInstance);

	for (auto& pTransform : m_vecInstancedTransform)
		Safe_Release(pTransform);
	m_vecInstancedTransform.clear();
}
