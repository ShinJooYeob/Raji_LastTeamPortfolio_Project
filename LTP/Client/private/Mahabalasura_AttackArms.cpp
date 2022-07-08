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
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components());

	if (pArg != nullptr)
	{
		_float3 Pos = *(_float3*)pArg;
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, Pos);

	}

	m_pTransformCom->Scaled_All(_float3(3.f, 5.f, 5.f));

	m_pPlayerObj = (CGameObject*)g_pGameInstance->Get_GameObject_By_LayerIndex(m_eNowSceneNum, TEXT("Layer_Player"));
	CTransform* PlayerTransform = (CTransform*)m_pPlayerObj->Get_Component(TAG_COM(Com_Transform));
	m_PlayerPos = PlayerTransform->Get_MatrixState(CTransform::STATE_POS);

	m_BezierStartPos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);

	m_CenterPoint = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS) + XMLoadFloat3(&m_PlayerPos) * 0.5f;
	m_CenterPoint.y = 15.f;


	Set_LimLight_N_Emissive(_float4(255.f, 0.f, 20.f, 255.f), 0);
	m_pTransformCom->LookAt(XMLoadFloat3(&m_PlayerPos));

	return S_OK;
}

_int CMahabalasura_AttackArms::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0) return -1;

	
	//m_pTransformCom->BezierCurve(m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), XMLoadFloat3(&m_PlayerPos), XMLoadFloat3(&CenterPoint), m_fProgressTime, 1.0, fDeltaTime);

	m_fProgressTime += (_float)fDeltaTime;
	//if (m_pTransformCom->MovetoBezierCurve(m_fProgressTime, XMLoadFloat3(&m_BezierStartPos), XMLoadFloat3(&m_CenterPoint), XMLoadFloat3(&m_PlayerPos)))
	//{
	//	//이펙트 생성 할 곳

	//	Set_IsDead();
	//}
	_float Length = m_BezierStartPos.Get_Distance(m_PlayerPos.XMVector());
	_float Rate =max( ( 1.0f - (10.0f - Length) * 0.1f)*0.5f ,0);


	if (m_fProgressTime >= m_fTotalTime * Rate)
	{
		Set_IsDead();
	}
	_float3 Pos;



	Pos.x = g_pGameInstance->Easing(TYPE_Linear, m_BezierStartPos.x, m_PlayerPos.x, m_fProgressTime , m_fTotalTime * Rate);

	


	if (m_fProgressTime  < m_fTotalTime * 0.4 *Rate)
	{
		Pos.y = g_pGameInstance->Easing(TYPE_QuadOut, m_BezierStartPos.y, m_BezierStartPos.Get_Distance(m_PlayerPos.XMVector()) * .3875f, m_fProgressTime, m_fTotalTime *0.4f *Rate);
	}
	else
	{
		Pos.y = g_pGameInstance->Easing(TYPE_SinIn, m_BezierStartPos.Get_Distance(m_PlayerPos.XMVector())*.3875f, m_PlayerPos.y, m_fProgressTime - (m_fTotalTime *0.4f)*Rate, m_fTotalTime *0.6f * Rate);
	}

	Pos.z = g_pGameInstance->Easing(TYPE_Linear, m_BezierStartPos.z, m_PlayerPos.z, m_fProgressTime, m_fTotalTime*Rate);

	m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, Pos);


	m_bIsOnScreen = g_pGameInstance->IsNeedToRender(m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS), m_fFrustumRadius);
	FAILED_CHECK(m_pModel->Update_AnimationClip(fDeltaTime * (m_fAnimmultiple), m_bIsOnScreen));
	FAILED_CHECK(Adjust_AnimMovedTransform(fDeltaTime));

	return _int();
}

_int CMahabalasura_AttackArms::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)return -1;

	FAILED_CHECK(m_pRendererCom->Add_ShadowGroup(CRenderer::SHADOW_ANIMMODEL, this, m_pTransformCom, m_pShaderCom, m_pModel));
	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
	m_vOldPos = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS);

	return _int();
}

_int CMahabalasura_AttackArms::Render()
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

_int CMahabalasura_AttackArms::LateRender()
{
	return _int();
}

HRESULT CMahabalasura_AttackArms::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_Boss_MahabalasuraAttackArms), TAG_COM(Com_Model), (CComponent**)&m_pModel));

	CTransform::TRANSFORMDESC tDesc = {};

	tDesc.fMovePerSec = 5;
	tDesc.fRotationPerSec = XMConvertToRadians(60);
	tDesc.fScalingPerSec = 1;
	tDesc.vPivot = _float3(0, 0, 0);

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));


	return S_OK;
}

HRESULT CMahabalasura_AttackArms::Adjust_AnimMovedTransform(_double fDeltatime)
{
	_uint iNowAnimIndex = m_pModel->Get_NowAnimIndex();
	_double PlayRate = m_pModel->Get_PlayRate();

	if (iNowAnimIndex != m_iOldAnimIndex || PlayRate > 0.98)
		m_iAdjMovedIndex = 0;


	if (PlayRate <= 0.98)
	{

	}
	else
	{

	}

	m_iOldAnimIndex = iNowAnimIndex;
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

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModel);
}
