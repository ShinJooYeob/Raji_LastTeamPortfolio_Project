#include "stdafx.h"
#include "..\public\Mahabalasura.h"

CMahabalasura::CMahabalasura(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CBoss(pDevice, pDeviceContext)
{
}

CMahabalasura::CMahabalasura(const CMahabalasura & rhs)
	: CBoss(rhs)
{
}

HRESULT CMahabalasura::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));


	return S_OK;
}

HRESULT CMahabalasura::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components());

	m_fAttackCoolTime = 2.f;
	m_fSkillCoolTime = 5.f;
	m_bIsHit = false;

	//m_pTransformCom->Scaled_All(_float3(1.5f));
	m_pPlayerObj = (CGameObject*)g_pGameInstance->Get_GameObject_By_LayerIndex(m_eNowSceneNum,
		TEXT("Layer_Player"));

	ATTACHEDESC WeaponDesc;

	WeaponDesc.Initialize_AttachedDesc(this, "middle_metacarpal_r", XMVectorSet(100.f, 200.f, 150.f, 0.f), XMVectorSet(0, 0.f, 180.f, 0.f), XMVectorSet(-0.950 * 100.f,-0.160 * 200.f,-1.550 * 150.f,1.f));
	g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TEXT("Weapon"), TAG_OP(Prototype_Object_Boss_MahabalasuraWeapon), &WeaponDesc);
	return S_OK;
}

_int CMahabalasura::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)return -1;

	m_fAttackCoolTime -= (_float)fDeltaTime;
	m_fSkillCoolTime -= (_float)fDeltaTime;

	if (m_bIsLookAt)
	{
		CTransform* PlayerTransform = (CTransform*)m_pPlayerObj->Get_Component(TAG_COM(Com_Transform));
		_float3 PlayerPos = PlayerTransform->Get_MatrixState(CTransform::STATE_POS);

		PlayerPos.y = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS).y;
		_Vector Dir = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
		Dir = XMVector3Normalize(PlayerPos.XMVector() - XMVectorSetY(Dir, PlayerPos.y));
		m_pTransformCom->Turn_Dir(Dir, 0.90f);

		m_fRange = XMVectorGetZ(XMVector3Length(XMLoadFloat3(&PlayerPos) - m_pTransformCom->Get_MatrixState(CTransform::STATE_POS)));
	}

	
	if (!m_bIsAttack && !m_bIsHit && m_bIsWalk)
	{
		if(m_fRange < 2 && m_fAttackCoolTime > 0)
			m_pModel->Change_AnimIndex(0);
		else
		{
			m_pTransformCom->Move_Forward(fDeltaTime * 0.7);

			m_pModel->Change_AnimIndex(2);
		}
	}
	//기본공격
	if (m_fRange < 4.f && !m_bIsAttack && m_fAttackCoolTime <=0)
	{
		m_bIsAttack = true;

		m_bIsWalk = false;
		//m_bIsLookAt = false;
		m_pModel->Change_AnimIndex_ReturnTo(5, 0);
	}
	else if (!m_bIsAttack && m_fSkillCoolTime <= 0)
	{

	}

	//if (g_pGameInstance->Get_DIKeyState(DIK_M)& DIS_Down)
	//	m_bIsHit = true;


	m_bIsOnScreen = g_pGameInstance->IsNeedToRender(m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS), m_fFrustumRadius);
	FAILED_CHECK(m_pModel->Update_AnimationClip(fDeltaTime * (m_fAnimmultiple), m_bIsOnScreen));
	FAILED_CHECK(Adjust_AnimMovedTransform(fDeltaTime));

	return _int();
}

_int CMahabalasura::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)return -1;


	FAILED_CHECK(m_pRendererCom->Add_ShadowGroup(CRenderer::SHADOW_ANIMMODEL, this, m_pTransformCom, m_pShaderCom, m_pModel));
	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
	m_vOldPos = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS);
	//g_pGameInstance->Set_TargetPostion(PLV_PLAYER, m_vOldPos);

	return _int();
}

_int CMahabalasura::Render()
{
	if (__super::Render() < 0)		return -1;

	NULL_CHECK_RETURN(m_pModel, E_FAIL);

	CGameInstance* pInstance = GetSingle(CGameInstance);
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));


	FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));

	_uint NumMaterial = m_pModel->Get_NumMaterial();

	for (_uint i = 0; i < NumMaterial; i++)
	{
		//if(i == 10)continue;

		for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; j++)
			FAILED_CHECK(m_pModel->Bind_OnShader(m_pShaderCom, i, j, MODLETEXTYPE(j)));

		FAILED_CHECK(m_pModel->Render(m_pShaderCom, 3, i, "g_BoneMatrices"));
	}

	return _int();
}

_int CMahabalasura::LateRender()
{
	if (__super::LateRender() < 0)		return -1;

	return _int();
}

HRESULT CMahabalasura::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_Boss_Mahabalasura), TAG_COM(Com_Model), (CComponent**)&m_pModel));
	FAILED_CHECK(m_pModel->Change_AnimIndex(0));


	CTransform::TRANSFORMDESC tDesc = {};

	tDesc.fMovePerSec = 5;
	tDesc.fRotationPerSec = XMConvertToRadians(60);
	tDesc.fScalingPerSec = 1;
	tDesc.vPivot = _float3(0, 0, 0);

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));


	return S_OK;
}

HRESULT CMahabalasura::Adjust_AnimMovedTransform(_double fDeltatime)
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
			//IDLE
			if (PlayRate > 0.f)
			{
				m_bIsAttack = false;
				m_bIsLookAt = true;
				m_bIsWalk = true;
			}
			break;
		case 1:
			break;

		case 5:
		{
			m_bIsLookAt = false;
			m_fAnimmultiple = 1.3f;

			if (PlayRate > 0.1401869f && PlayRate < 0.20f)
			{
				m_bIsLookAt = true;
				m_pTransformCom->Move_Forward(fDeltatime * 1.5);
			}
			else if (PlayRate > 0.36448598f && PlayRate < 0.45f)
			{
				m_bIsLookAt = true;
				m_pTransformCom->Move_Forward(fDeltatime * 1.5);
			}
			else if (PlayRate > 0.551401869f && PlayRate < 0.60f)
			{
				m_bIsLookAt = true;
				m_pTransformCom->Move_Forward(fDeltatime * 1.5);
			}
		}
		break;


		}
	}
	else
	{
		if (iNowAnimIndex == 0)
		{
		}
		if (iNowAnimIndex == 5)
		{
			m_fAttackCoolTime = 2.f;
			m_fSkillCoolTime += 2.f;
			m_fAnimmultiple = 1.f;

		}
	}

	m_iOldAnimIndex = iNowAnimIndex;
	return S_OK;
}

CMahabalasura * CMahabalasura::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CMahabalasura*	pInstance = new CMahabalasura(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CMahabalasura");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMahabalasura::Clone(void * pArg)
{
	CMahabalasura*	pInstance = new CMahabalasura(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created  Clone CMahabalasura");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMahabalasura::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModel);
}
