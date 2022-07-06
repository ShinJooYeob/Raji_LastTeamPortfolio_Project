#include "stdafx.h"
#include "..\public\PlayerWeapon_Arrow.h"
#include "Timer.h"

CPlayerWeapon_Arrow::CPlayerWeapon_Arrow(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CPlayerWeapon(pDevice, pDeviceContext)
{
}

CPlayerWeapon_Arrow::CPlayerWeapon_Arrow(const CPlayerWeapon_Arrow & rhs)
	: CPlayerWeapon(rhs)
{
}

HRESULT CPlayerWeapon_Arrow::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CPlayerWeapon_Arrow::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components());

	FAILED_CHECK(SetUp_EtcInfo());

	return S_OK;
}

_int CPlayerWeapon_Arrow::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0) return -1;

	switch (m_eCurState)
	{
	case Arrow_State_NormalReady:
		UpdateState_NormalReady(fDeltaTime);
		break;
	case Arrow_State_NormalShot:
		UpdateState_NormalShot(fDeltaTime);
		break;
	case Arrow_State_UtilityReady:
		UpdateState_UtilityReady(fDeltaTime);
		break;
	case Arrow_State_UtilityShot:
		UpdateState_UtilityShot(fDeltaTime);
		break;
	case Arrow_State_PowerShot_Combo_0:
		UpdateState_PowerShot_Conbo_0(fDeltaTime);
		break;
	case Arrow_State_PowerShot_Combo_1:
		UpdateState_PowerShot_Conbo_1(fDeltaTime);
		break;
	case Arrow_State_PowerShot_Combo_2:
		UpdateState_PowerShot_Conbo_2(fDeltaTime);
		break;
	case Arrow_State_UltimateReady_Pre_Ready:
		UpdateState_Ultimate_Pre_Ready(fDeltaTime);
		break;
	case Arrow_State_UltimateShot_Pre_Shot:
		UpdateState_Ultimate_Pre_Shot(fDeltaTime);
		break;
	case Arrow_State_UltimateReady_Post_Ready:
		UpdateState_Ultimate_Post_Ready(fDeltaTime);
		break;
	case Arrow_State_UltimateShot_Post_Shot:
		UpdateState_Ultimate_Post_Shot(fDeltaTime);
		break;
	}

	if (true == m_bFired)
	{
		m_fCurTime_Destroy += (_float)g_fDeltaTime;
		if (m_fCurTime_Destroy >= m_fMaxTime_Destroy)
		{
			Set_IsDead();
		}
	}
	return _int();
}

_int CPlayerWeapon_Arrow::LateUpdate(_double fDeltaTimer)
{
	if (__super::LateUpdate(fDeltaTimer) < 0) return -1;

	// Center Pos //
	/*m_tPlayerWeaponDesc.eAttachedDesc.Set_DefaultBonePivot(_float3(1, 1, 1), _float3(-100, -180, 70), _float3(0.0, 0.0, 0.0));
	m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, _float3(0.283f, -1.04f, 0.497f));
	m_iPassNum = 8;*/
	//


	//m_iPassNum = 8;
	//m_tPlayerWeaponDesc.eAttachedDesc.Set_DefaultBonePivot(_float3(1.1f, 1.1f, 1.1f), _float3(38, -50, -32), _float3(0, 0, 0));
	//m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, _float3(-1.11f, -0.43f, -0.02f));


	m_fAttachedMatrix = m_pTransformCom->Get_WorldMatrix()  * m_tPlayerWeaponDesc.eAttachedDesc.Caculate_AttachedBoneMatrix();
	m_fAttachedMatrix = m_fAttachedMatrix.TransposeXMatrix();
	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
	return _int();
}

_int CPlayerWeapon_Arrow::Render()
{
	if (__super::Render() < 0)		return -1;

	NULL_CHECK_RETURN(m_pModel, E_FAIL);

	CGameInstance* pInstance = GetSingle(CGameInstance);

	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_AttechMatrix", &m_fAttachedMatrix, sizeof(_float4x4)));

	FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));

	_uint NumMaterial = m_pModel->Get_NumMaterial();

	for (_uint i = 0; i < NumMaterial; i++)
	{

		for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; j++)
			FAILED_CHECK(m_pModel->Bind_OnShader(m_pShaderCom, i, j, MODLETEXTYPE(j)));

		FAILED_CHECK(m_pModel->Render(m_pShaderCom, m_iPassNum, i));
	}

	return _int();
}

_int CPlayerWeapon_Arrow::LateRender()
{
	return _int();
}

void CPlayerWeapon_Arrow::Set_State(EArrowState eNewState)
{
	m_eCurState = eNewState;

	switch (m_eCurState)
	{
	case EArrowState::Arrow_State_NormalReady:
		m_iPassNum = 8;
		m_tPlayerWeaponDesc.eAttachedDesc.Set_DefaultBonePivot(_float3(1, 1, 1), _float3(-98, -80, 115), _float3(0.0, 0.0, 0.0));
		m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, _float3(-1.087f, 0.36f, 0.667f));
		break;
	case EArrowState::Arrow_State_NormalShot:
		m_iPassNum = 2;
		m_fAttachedMatrix = m_pTransformCom->Get_WorldMatrix() * m_tPlayerWeaponDesc.eAttachedDesc.Caculate_AttachedBoneMatrix();
		m_pTransformCom->Set_Matrix(m_fAttachedMatrix);
		break;
	case EArrowState::Arrow_State_UtilityReady:
		m_iPassNum = 8;
		m_tPlayerWeaponDesc.eAttachedDesc.Set_DefaultBonePivot(_float3(1.1f, 1.1f, 1.1f), _float3(48, 150.f, -95), _float3(0.0, 0.0, 0.0));
		m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, _float3(0.473f, 0.3f, 1.057f));
		break;
	case EArrowState::Arrow_State_UtilityShot:
		m_iPassNum = 2;
		m_fAttachedMatrix = m_pTransformCom->Get_WorldMatrix() * m_tPlayerWeaponDesc.eAttachedDesc.Caculate_AttachedBoneMatrix();
		m_pTransformCom->Set_Matrix(m_fAttachedMatrix);
		break;
	}

}

void CPlayerWeapon_Arrow::Set_State_PowerShot_Combo_0(_fVector vShotDir, _uint iArrowIndex)
{
	m_tPlayerWeaponDesc.eAttachedDesc.Set_DefaultBonePivot(_float3(1, 1, 1), _float3(-100, -180, 70), _float3(0.0, 0.0, 0.0));
	m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, _float3(0.283f, -0.85f, 0.497f));
	
	m_fAttachedMatrix = m_pTransformCom->Get_WorldMatrix() * m_tPlayerWeaponDesc.eAttachedDesc.Caculate_AttachedBoneMatrix();
	m_pTransformCom->Set_Matrix(m_fAttachedMatrix);

	m_iPassNum = 2;
	m_eCurState = Arrow_State_PowerShot_Combo_0;

	_Vector vFinalShotDir = XMVectorSetY(vShotDir, 0);
	vFinalShotDir = XMVectorSetY(vFinalShotDir, 0);
	m_pTransformCom->LookDir(vFinalShotDir);
	if (1 == iArrowIndex)
	{
		m_pTransformCom->Turn_Direct(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_UP), XMConvertToRadians(10.f));
	} 
	else if (2 == iArrowIndex)
	{
		m_pTransformCom->Turn_Direct(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_UP), XMConvertToRadians(-10.f));
	}

	m_bFired = true;
}

void CPlayerWeapon_Arrow::Set_State_PowerShot_Combo_1(_fVector vShotDir)
{
	m_tPlayerWeaponDesc.eAttachedDesc.Set_DefaultBonePivot(_float3(1, 1, 1), _float3(-98, -80, 115), _float3(0.0, 0.0, 0.0));
	m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, _float3(-1.087f, 0.36f, 0.667f));

	m_fAttachedMatrix = m_pTransformCom->Get_WorldMatrix() * m_tPlayerWeaponDesc.eAttachedDesc.Caculate_AttachedBoneMatrix();
	m_pTransformCom->Set_Matrix(m_fAttachedMatrix);

	m_eCurState = Arrow_State_PowerShot_Combo_1;
	m_iPassNum = 2;

	_Vector vFinalShotDir = XMVectorSetY(vShotDir, 0);
	m_pTransformCom->LookDir(vFinalShotDir);
	m_bFired = true;
}

void CPlayerWeapon_Arrow::Set_State_PowerShot_Combo_2(_fVector vShotDir, _float fSpeed)
{
	m_tPlayerWeaponDesc.eAttachedDesc.Set_DefaultBonePivot(_float3(1.1f, 1.1f, 1.1f), _float3(-110, -80, 110), _float3(0, 0, 0));
	m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, _float3(-0.957f, 0.11f, 0.667f));
	
	m_fAttachedMatrix = m_pTransformCom->Get_WorldMatrix() * m_tPlayerWeaponDesc.eAttachedDesc.Caculate_AttachedBoneMatrix();
	m_pTransformCom->Set_Matrix(m_fAttachedMatrix);

	m_eCurState = Arrow_State_PowerShot_Combo_2;
	m_iPassNum = 2;

	m_fStartPos_y = XMVectorGetY(m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS));

	_Vector vFinalShotDir = XMVectorSetY(vShotDir, 0);
	m_pTransformCom->LookDir(vFinalShotDir);

	m_pTransformCom->Set_MoveSpeed(fSpeed);
	m_bFired = true;
}

void CPlayerWeapon_Arrow::Set_State_Ultimate_Pre_Ready()
{
	m_iPassNum = 8;
	m_tPlayerWeaponDesc.eAttachedDesc.Set_DefaultBonePivot(_float3(1.1f, 1.1f, 1.1f), _float3(48.f, 150.f, -95.f), _float3(0.0, 0.0, 0.0));
	m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, _float3(0.473f, 0.3f, 1.057f));

	m_eCurState = Arrow_State_UltimateReady_Pre_Ready;
}

void CPlayerWeapon_Arrow::Set_State_Ultimate_Pre_Shot()
{
	m_iPassNum = 2;
	m_fAttachedMatrix = m_pTransformCom->Get_WorldMatrix() * m_tPlayerWeaponDesc.eAttachedDesc.Caculate_AttachedBoneMatrix();
	m_pTransformCom->Set_Matrix(m_fAttachedMatrix);

	m_pTransformCom->LookDir(XMVectorSet(0.f, 1.f, 0.f, 0.f));
	m_eCurState = Arrow_State_UltimateShot_Pre_Shot;
}

void CPlayerWeapon_Arrow::Set_State_Ultimate_Post_Ready()
{
	m_iPassNum = 8;
	m_tPlayerWeaponDesc.eAttachedDesc.Set_DefaultBonePivot(_float3(1.1f, 1.1f, 1.1f), _float3(38, -50, -32), _float3(0, 0, 0));
	m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, _float3(-1.11f, -0.43f, -0.02f));

	m_eCurState = Arrow_State_UltimateReady_Post_Ready;
}

void CPlayerWeapon_Arrow::Set_State_Ultimate_Post_Shot()
{
	m_iPassNum = 2;
	m_fAttachedMatrix = m_pTransformCom->Get_WorldMatrix() * m_tPlayerWeaponDesc.eAttachedDesc.Caculate_AttachedBoneMatrix();
	m_pTransformCom->Set_Matrix(m_fAttachedMatrix);

	m_pTransformCom->LookDir(XMVectorSet(0.f, 1.f, 0.f, 0.f));
	m_eCurState = Arrow_State_UltimateShot_Post_Shot;
}

_int CPlayerWeapon_Arrow::UpdateState_NormalReady(_double fDeltaTime)
{
	return _int();
}

_int CPlayerWeapon_Arrow::UpdateState_NormalShot(_double fDeltaTime)
{
	m_pTransformCom->Move_Forward(fDeltaTime);
	m_pTimer_Destroy->Get_DeltaTime();
	m_bFired = true;
	return _int();
}

_int CPlayerWeapon_Arrow::UpdateState_UtilityReady(_double fDeltaTime)
{
	return _int();
}

_int CPlayerWeapon_Arrow::UpdateState_UtilityShot(_double fDeltaTime)
{
	m_pTransformCom->Move_Forward(fDeltaTime);
	m_pTimer_Destroy->Get_DeltaTime();
	m_bFired = true;
	return _int();
}

_int CPlayerWeapon_Arrow::UpdateState_UltimateReady(_double fDeltaTime)
{
	return _int();
}

_int CPlayerWeapon_Arrow::UpdateState_UltimateShot(_double fDeltaTime)
{
	m_pTimer_Destroy->Get_DeltaTime();
	m_bFired = true;
	return _int();
}

_int CPlayerWeapon_Arrow::UpdateState_PowerShot_Conbo_0(_double fDeltaTime)
{
	m_pTransformCom->Move_Forward(fDeltaTime);

	return _int();
}

_int CPlayerWeapon_Arrow::UpdateState_PowerShot_Conbo_1(_double fDeltaTime)
{
	m_pTransformCom->Move_Forward(fDeltaTime);

	return _int();
}

_int CPlayerWeapon_Arrow::UpdateState_PowerShot_Conbo_2(_double fDeltaTime)
{
	 _float fPos_y = m_fStartPos_y + (0.f * m_fTimeAcc - 9.8f * m_fTimeAcc * m_fTimeAcc * 0.5f);
	 m_fTimeAcc += 0.02f;

	 _Vector vMyPos_Before = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);
	 m_pTransformCom->Move_Forward(fDeltaTime);
	 _Vector vMyPos_After = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);

	 vMyPos_After = XMVectorSetY(vMyPos_After, fPos_y);
	 m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, vMyPos_After);
	 
	 m_pTransformCom->LookDir(XMVector3Normalize(vMyPos_After - vMyPos_Before));

	return _int();
}

_int CPlayerWeapon_Arrow::UpdateState_Ultimate_Pre_Ready(_double fDeltaTime)
{
	return _int();
}

_int CPlayerWeapon_Arrow::UpdateState_Ultimate_Pre_Shot(_double fDeltaTime)
{
	m_fTimeAcc += (_float)g_fDeltaTime;
	m_pTransformCom->Move_Forward(fDeltaTime);

	if (m_fTimeAcc > 1.f)
	{
		// Spawn Core Sphere
		Set_IsDead();
	}

	return _int();
}

_int CPlayerWeapon_Arrow::UpdateState_Ultimate_Post_Ready(_double fDeltaTime)
{
	return _int();
}

_int CPlayerWeapon_Arrow::UpdateState_Ultimate_Post_Shot(_double fDeltaTime)
{
	m_pTransformCom->Move_Forward(fDeltaTime);

	/** Check Collision to CoreSphere
	if ()
	{

	}
	*/
	return _int();
}

_fVector CPlayerWeapon_Arrow::Get_BonePos(const char * pBoneName)
{
	return _fVector();
}

void CPlayerWeapon_Arrow::Update_AttachCamPos()
{
}

void CPlayerWeapon_Arrow::Update_AttachMatrix()
{
}

HRESULT CPlayerWeapon_Arrow::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VNAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_PlayerWeapon_Arrow), TAG_COM(Com_Model), (CComponent**)&m_pModel));

	CTransform::TRANSFORMDESC tDesc = {}; 

	tDesc.fMovePerSec = 30;
	tDesc.fRotationPerSec = XMConvertToRadians(60);
	tDesc.fScalingPerSec = 1;
	tDesc.vPivot = _float3(0, 0, 0);

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));


	return S_OK;
}

HRESULT CPlayerWeapon_Arrow::SetUp_EtcInfo()
{
	m_pTimer_Destroy = CTimer::Create();
	m_fMaxTime_Destroy = 2.f;
	m_fCurTime_Destroy = 0.f;

	return S_OK;
}

CPlayerWeapon_Arrow * CPlayerWeapon_Arrow::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CPlayerWeapon_Arrow*	pInstance = new CPlayerWeapon_Arrow(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CPlayerWeapon_Arrow");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CPlayerWeapon_Arrow::Clone(void * pArg)
{
	CPlayerWeapon_Arrow*	pInstance = new CPlayerWeapon_Arrow(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CPlayerWeapon_Arrow");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPlayerWeapon_Arrow::Free()
{
	__super::Free();

	Safe_Release(m_pTimer_Destroy);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModel);
}
