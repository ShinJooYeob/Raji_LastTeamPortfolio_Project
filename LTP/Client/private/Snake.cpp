#include "stdafx.h"
#include "..\public\Snake.h"


CSnake::CSnake(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CBoss(pDevice, pDeviceContext)
{
}

CSnake::CSnake(const CSnake & rhs)
	: CBoss(rhs)
{
}

HRESULT CSnake::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));


	return S_OK;
}

HRESULT CSnake::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components());

	if (pArg != nullptr) 
	{
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, *((_float3*)pArg));
		m_StartPos = *((_float3*)pArg);
	}
	//else
	//{
	//	m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, _float3(8.f, -110.f, 53.f));
	//	
	//}

	m_pTransformCom->Rotation_CW(XMVectorSet(0, 1, 0, 0), XMConvertToRadians(170));

	m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, _float3(8.f, -110.f, 53.f));
	m_StartPos = _float3(8.f, -110.f, 53.f);

	m_pTransformCom->Scaled_All(_float3(3.f, 3.f, 3.f));

	m_pModel->Change_AnimIndex(1);

	m_fAttackCoolTime = 5.f;
	m_fSkillCoolTime = 8.f;
	m_bIsHit = false;

	m_pPlayerObj = (CGameObject*)g_pGameInstance->Get_GameObject_By_LayerIndex(m_eNowSceneNum, TEXT("Layer_Player"));

	return S_OK;
}

_int CSnake::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)return -1;

	if (!TestBool)
	{
		TestBool = true;
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, _float3(8.f, -110.f, 53.f));
	}


	if (!m_bIsAttack)
		m_fAttackCoolTime -= (_float)fDeltaTime;
	
	if (m_pModel->Get_NowAnimIndex() == 1)
		m_fRotTime += (_float)fDeltaTime;

	CTransform* PlayerTransform = (CTransform*)m_pPlayerObj->Get_Component(TAG_COM(Com_Transform));
	_float3 PlayerPos = PlayerTransform->Get_MatrixState(CTransform::STATE_POS);

	PlayerPos.y = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS).y;

	_float3 TargetDir = XMVector3Normalize(XMLoadFloat3(&PlayerPos) - m_pTransformCom->Get_MatrixState(CTransform::STATE_POS));
	m_vAngle = XMVector3Dot(XMLoadFloat3(&TargetDir), XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK)));

	if (g_pGameInstance->Get_DIKeyState(DIK_M)& DIS_Down)
		m_bTestHodeing = !m_bTestHodeing;

	if (g_pGameInstance->Get_DIKeyState(DIK_N)& DIS_Down)
		m_pModel->Change_AnimIndex_ReturnTo(5, 1);

	if (m_bIsAttack && m_bIsLookAt)
	{
		CTransform* PlayerTransform = (CTransform*)m_pPlayerObj->Get_Component(TAG_COM(Com_Transform));
		_float3 PlayerPos = PlayerTransform->Get_MatrixState(CTransform::STATE_POS);

		PlayerPos.y = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS).y;
		_Vector Dir = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
		Dir = XMVector3Normalize(PlayerPos.XMVector() - XMVectorSetY(Dir, PlayerPos.y));
		m_pTransformCom->Turn_Dir(Dir, 0.90f);
	}

	if (XMVectorGetX(m_vAngle) > 0.94f && !m_bIsAttack && m_fAttackCoolTime <= 0.f)
	{ 
		if (m_bTestHodeing)
			m_bHiding = true;

		if (m_bHiding)
		{
			m_bIsAttack = true;
			m_pModel->Change_AnimIndex_ReturnTo(2, 1);
		}
		else
		{
			m_bIsAttack = true;

			_int iRandom =  (_int)GetSingle(CUtilityMgr)->RandomFloat(1.0f, 2.9f);

			if(iRandom == 1)
				m_pModel->Change_AnimIndex(3);
			else
				m_pModel->Change_AnimIndex(7);
		}
	}

	//m_bIsOnScreen = g_pGameInstance->IsNeedToRender(m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS), m_fFrustumRadius);
	FAILED_CHECK(m_pModel->Update_AnimationClip(fDeltaTime * (m_fAnimmultiple)));
	FAILED_CHECK(Adjust_AnimMovedTransform(fDeltaTime));

	return _int();
}

_int CSnake::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)return -1;


	FAILED_CHECK(m_pRendererCom->Add_ShadowGroup(CRenderer::SHADOW_ANIMMODEL, this, m_pTransformCom, m_pShaderCom, m_pModel));
	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
	m_vOldPos = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS);

	return _int();
}

_int CSnake::Render()
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
		for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; j++)
			FAILED_CHECK(m_pModel->Bind_OnShader(m_pShaderCom, i, j, MODLETEXTYPE(j)));
		FAILED_CHECK(m_pModel->Render(m_pShaderCom, 3, i, "g_BoneMatrices"));
	}

	return _int();
}

_int CSnake::LateRender()
{
	if (__super::LateRender() < 0)		return -1;

	return _int();
}

HRESULT CSnake::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_Boss_Snake), TAG_COM(Com_Model), (CComponent**)&m_pModel));
	FAILED_CHECK(m_pModel->Change_AnimIndex(1));


	CTransform::TRANSFORMDESC tDesc = {};

	tDesc.fMovePerSec = 5;
	tDesc.fRotationPerSec = XMConvertToRadians(60);
	tDesc.fScalingPerSec = 1;
	tDesc.vPivot = _float3(0, 0, 0);

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));


	return S_OK;
}

HRESULT CSnake::Adjust_AnimMovedTransform(_double fDeltatime)
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
		{
		}
		break;

		case 1:
		{
			m_bIsAttack = false;
			if (m_iRotationRandom == 0)
			{
				//CTransform* PlayerTransform = (CTransform*)m_pPlayerObj->Get_Component(TAG_COM(Com_Transform));
				//_float3 PlayerPos = PlayerTransform->Get_MatrixState(CTransform::STATE_POS);

				//PlayerPos.y = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS).y;
				//_float3 TargetDir = XMVector3Normalize(XMLoadFloat3(&PlayerPos) - m_pTransformCom->Get_MatrixState(CTransform::STATE_POS));
				//m_vAngle = XMVector3Dot(XMLoadFloat3(&TargetDir), XMVector3Normalize(m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK)));
				
				if (XMVectorGetX(m_vAngle) < -0.2f && !m_bIsAngleOut)
				{
					m_bIsAngleOut = true;
				}

				if(!m_bIsAngleOut)
					m_pTransformCom->Turn_CW(XMVectorSet(0.f, 1.f, 0.f, 0.f), fDeltatime * 0.2f);
				else
					m_pTransformCom->Turn_CCW(XMVectorSet(0.f, 1.f, 0.f, 0.f), fDeltatime * 0.2f);
			}
			else if (m_iRotationRandom == 1)
			{
				if (XMVectorGetX(m_vAngle) < -0.2f && !m_bIsAngleOut)
				{
					m_bIsAngleOut = true;
				}

				if (!m_bIsAngleOut)
					m_pTransformCom->Turn_CCW(XMVectorSet(0.f, 1.f, 0.f, 0.f), fDeltatime * 0.2f);
				else
					m_pTransformCom->Turn_CW(XMVectorSet(0.f, 1.f, 0.f, 0.f), fDeltatime * 0.2f);
			}
			//else if (m_iRotationRandom == 2)
			//{
			//	m_fRotTime += (_float)fDeltatime;

			//	if (m_fRotTime < 3)
			//		m_pTransformCom->Turn_CW(XMVectorSet(0.f, 1.f, 0.f, 0.f), fDeltatime * 0.5f);
			//	else
			//		m_pTransformCom->Turn_CCW(XMVectorSet(0.f, 1.f, 0.f, 0.f), fDeltatime * 0.5f);
			//}
		}
		break;

		case 2:
		{
			if (PlayRate > 0 && PlayRate < 0.5f)
			{
				if (m_iAdjMovedIndex == 0 && PlayRate > 0.499999f)
				{
					m_bIsLookAt = false;
					++m_iAdjMovedIndex;
				}
				_float3 MonsterPos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
				_float3 FindPos = (MonsterPos.XMVector() + m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK) * 12.f);

				CTransform* PlayerTransform = (CTransform*)m_pPlayerObj->Get_Component(TAG_COM(Com_Transform));
				_float3 PlayerPos = PlayerTransform->Get_MatrixState(CTransform::STATE_POS);

				PlayerPos.y = FindPos.y =  MonsterPos.y;

				_float3 vGoalDir = (PlayerPos.XMVector() - FindPos.XMVector());
				_float fLength = g_pGameInstance->Easing(TYPE_SinInOut, 0, vGoalDir.Get_Lenth(), (_float)PlayRate, 0.5f);

				m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, (MonsterPos.XMVector() + vGoalDir.Get_Nomalize() * fLength));
			}
			else if (PlayRate > 0.55f && PlayRate < 0.98f)
			{
				_float3 MonsterPos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);

				//CTransform* PlayerTransform = (CTransform*)m_pPlayerObj->Get_Component(TAG_COM(Com_Transform));
				//_float3 PlayerPos = PlayerTransform->Get_MatrixState(CTransform::STATE_POS);

				//PlayerPos.y = MonsterPos.y;

				_float3 vGoalDir = (m_StartPos.XMVector() - MonsterPos.XMVector());
				_float fLength = g_pGameInstance->Easing(TYPE_SinInOut, 0, vGoalDir.Get_Lenth(), (_float)PlayRate - 0.55f, 0.43f);

				m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, (MonsterPos.XMVector() + vGoalDir.Get_Nomalize() * fLength));
			}
		}
		break;

		case 3:
		break;

		case 4:
		break;

		case 5:
		break;

		case 6:
		break;

		}
	}
	else
	{
		if (iNowAnimIndex == 1)
		{
			m_fAttackCoolTime = 2.f;
			m_fRotTime = 0.f;
			m_iRotationRandom = (_int)GetSingle(CUtilityMgr)->RandomFloat(0.0f, 1.9f);
			//m_iRotationRandom = 2;
		}

		if (iNowAnimIndex == 2)
		{
			m_bIsAngleOut = false;
			m_bIsLookAt = true;
			m_fAttackCoolTime = 2.f;
			m_fRotTime = 0.f;
			m_iRotationRandom = (_int)GetSingle(CUtilityMgr)->RandomFloat(0.0f, 1.9f);
		}

		if (iNowAnimIndex == 3)
		{
			m_bIsAngleOut = false;
			m_fAttackCoolTime = 2.f;
			m_fRotTime = 0.f;
			m_iRotationRandom = (_int)GetSingle(CUtilityMgr)->RandomFloat(0.0f, 1.9f);
			//m_iRotationRandom = 2;

			m_pModel->Change_AnimIndex(1);
		}

		if (iNowAnimIndex == 5)
		{
			m_fAttackCoolTime = 2.f;
			m_fRotTime = 0.f;
			m_iRotationRandom = (_int)GetSingle(CUtilityMgr)->RandomFloat(0.0f, 1.9f);
		}

		if (iNowAnimIndex == 7)
		{
			m_bIsAngleOut = false;
			m_fAttackCoolTime = 2.f;
			m_fRotTime = 0.f;
			m_iRotationRandom = (_int)GetSingle(CUtilityMgr)->RandomFloat(0.0f, 1.9f);
			m_pModel->Change_AnimIndex(1);
		}
	}

	m_iOldAnimIndex = iNowAnimIndex;
	return S_OK;
}

CSnake * CSnake::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CSnake*	pInstance = new CSnake(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CSnake");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CSnake::Clone(void * pArg)
{
	CSnake*	pInstance = new CSnake(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created  Clone CSnake");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSnake::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModel);
}
