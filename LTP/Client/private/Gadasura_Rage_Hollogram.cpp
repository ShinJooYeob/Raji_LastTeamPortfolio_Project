#include "stdafx.h"
#include "..\public\Gadasura_Rage_Hollogram.h"
#include "Monster_Bullet_Universal.h"
#include "Monster_Weapon_Universal.h"
#include "Monster_Gadasura_Rage.h"

CGadasura_Rage_Hollogram::CGadasura_Rage_Hollogram(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CMonster(pDevice, pDeviceContext)
{
}

CGadasura_Rage_Hollogram::CGadasura_Rage_Hollogram(const CGadasura_Rage_Hollogram & rhs)
	: CMonster(rhs)
{
}

HRESULT CGadasura_Rage_Hollogram::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));


	return S_OK;
}

HRESULT CGadasura_Rage_Hollogram::Initialize_Clone(void * pArg)
{
	if (nullptr != pArg)
	{
		memcpy(&m_HollogramDesc, pArg, sizeof(GADASURA_HOLLOGRAMDESC));
	}
	else {
		MSGBOX("CGadasura_Rage_Hollogram의 pArg가 Nullptr입니다.");
	}


	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components());

	if (pArg != nullptr)
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, *((_float3*)pArg));



	SetUp_Info();

	Set_LimLight_N_Emissive(_float4(1.f, 0.f, 0.f, 1.f), _float4(1,0.5f,1,1));

	return S_OK;
}

_int CGadasura_Rage_Hollogram::Update(_double dDeltaTime)
{ 
	m_dDeadTime += dDeltaTime;
	if (m_HollogramDesc.dDuration <= m_dDeadTime)
	{
		Set_IsDead();
	}

	if (__super::Update(dDeltaTime) < 0)return -1;

	//마지막 인자의 bBlockAnimUntilReturnChange에는 true로 시작해서 정상작동이 된다면 false가 된다.
	//m_pModel->Change_AnimIndex();
	//m_pModel->Change_AnimIndex_ReturnTo(); //어떤 애니메이션을 돌리고 특정 애니메이션으로 보냄
	//m_pModel->Change_AnimIndex_ReturnTo_Must(); //중간에 애니메이션을 캔슬하고 다른 애니메이션을 동작시킴
	//m_pModel->Change_AnimIndex_UntilTo(); //1~5까지 돌린다고 명령어를 입력하면 1~5까지 돌아감
	//m_pModel->Change_AnimIndex_UntilNReturn();//1~5까지 돌리고 난 이후 특정 애니메이션으로 Return 시킬 경우
	//m_pModel->Change_AnimIndex_UntilNReturn_Must(); //1~5까지 돌리고 난 이후 특정 애니메이션으로 Return 시킬 수 있지만 다른 애니메이션을 동작시킬 수 있음



	PlayAnim(dDeltaTime);

	m_bIsOnScreen = g_pGameInstance->IsNeedToRender(m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS), m_fFrustumRadius);
	FAILED_CHECK(m_pModel->Update_AnimationClip(dDeltaTime, m_bIsOnScreen));
	FAILED_CHECK(Adjust_AnimMovedTransform(dDeltaTime));

	//////////////Motion Test
	m_pMotionTrail->Update_MotionTrail(dDeltaTime);
	/////////////////////////
	return _int();
}

_int CGadasura_Rage_Hollogram::LateUpdate(_double dDeltaTime)
{
	if (__super::LateUpdate(dDeltaTime) < 0)return -1;

	//////////
	if (m_bIsOnScreen)
	{
		FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
	}
	//////////////Motion Test
	FAILED_CHECK(m_pRendererCom->Add_TrailGroup(CRenderer::TRAIL_MOTION, m_pMotionTrail));
	////////////////

	FAILED_CHECK(m_pRendererCom->Add_ShadowGroup(CRenderer::SHADOW_ANIMMODEL, this, m_pTransformCom, m_pShaderCom, m_pModel));
	m_vOldPos = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS);

	return _int();
}

_int CGadasura_Rage_Hollogram::Render()
{
	if (__super::Render() < 0)
		return -1;

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

_int CGadasura_Rage_Hollogram::LateRender()
{
	if (__super::LateRender() < 0)
		return -1;

	return _int();
}

HRESULT CGadasura_Rage_Hollogram::SetUp_Info()
{
	


	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	m_pPlayerTransform = static_cast<CTransform*>(pGameInstance->Get_Commponent_By_LayerIndex(m_eNowSceneNum, TAG_LAY(Layer_Player), TAG_COM(Com_Transform)));

	RELEASE_INSTANCE(CGameInstance);


	CGameObject* MonsterObject = static_cast<CGameObject*>(m_HollogramDesc.Object);

	m_pMonsterTransform = static_cast<CTransform*>(MonsterObject->Get_Component(TAG_COM(Com_Transform)));

	m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, m_pMonsterTransform->Get_MatrixState(CTransform::STATE_POS));


	m_pTransformCom->LookAt(m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS));

	return S_OK;
}

HRESULT CGadasura_Rage_Hollogram::SetUp_Fight(_double dDeltaTime)
{

	if (m_bLookAtOn && m_dDeadTime < 3)
	{
		//m_pTransformCom->LookAt(m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS));


		_Vector vTarget = XMVector3Normalize(m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS) - m_pTransformCom->Get_MatrixState(CTransform::STATE_POS));

		//m_pTransformCom->Turn_Dir(XMVector3Normalize(m_pTransformCom->Get_MatrixScale(CTransform::STATE_LOOK)*0.9 + vTarget* 0.1));
		m_pTransformCom->Turn_Dir(vTarget, 0.92f);

	}


	return S_OK;
}

HRESULT CGadasura_Rage_Hollogram::PlayAnim(_double dDeltaTime)
{
	SetUp_Fight(dDeltaTime);

	if (m_bOnceSwitch == false)
	{
		m_pModel->Change_AnimIndex_ReturnTo(0, 1);

		m_bOnceSwitch = true;
	}

	return S_OK;
}
HRESULT CGadasura_Rage_Hollogram::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_Monster_Gadasura_Rage_Hollogram), TAG_COM(Com_Model), (CComponent**)&m_pModel));
	FAILED_CHECK(m_pModel->Change_AnimIndex(0));


	CTransform::TRANSFORMDESC tDesc = {};

	tDesc.fMovePerSec = 5;
	tDesc.fRotationPerSec = XMConvertToRadians(60);
	tDesc.vPivot = _float3(0, 0, 0);
	tDesc.fScalingPerSec = 1;

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));

	////////////Motion Test
	CMotionTrail::MOTIONTRAILDESC tMotionDesc;

	tMotionDesc.iNumTrailCount = 6;
	tMotionDesc.pModel = m_pModel;
	tMotionDesc.pShader = m_pShaderCom;
	tMotionDesc.iPassIndex = 5;

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_MotionTrail), TAG_COM(Com_MotionTrail), (CComponent**)&m_pMotionTrail, &tMotionDesc));
	///////////////////////////


	return S_OK;
}

HRESULT CGadasura_Rage_Hollogram::Adjust_AnimMovedTransform(_double dDeltaTime)
{
	_uint iNowAnimIndex = m_pModel->Get_NowAnimIndex();
	_double PlayRate = m_pModel->Get_PlayRate();

	if (iNowAnimIndex != m_iOldAnimIndex || PlayRate > 0.99)
	{
		m_iAdjMovedIndex = 0;

		m_bLookAtOn = true;
	}

	if (PlayRate <= 0.98) //애니메이션의 비율 즉, 0.98은 거의 끝나가는 시점
	{
		switch (iNowAnimIndex)
		{
		case 0:
		{
			break;
		}
		case 1:
		{
			m_pTransformCom->Move_Forward(dDeltaTime * 2);
			break;
		}
		default:
			break;
		}

	}

	m_iOldAnimIndex = iNowAnimIndex;
	return S_OK;
}

CGadasura_Rage_Hollogram * CGadasura_Rage_Hollogram::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CGadasura_Rage_Hollogram*	pInstance = new CGadasura_Rage_Hollogram(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CGadasura_Rage_Hollogram");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CGadasura_Rage_Hollogram::Clone(void * pArg)
{
	CGadasura_Rage_Hollogram*	pInstance = new CGadasura_Rage_Hollogram(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CGadasura_Rage_Hollogram");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CGadasura_Rage_Hollogram::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModel);
	////////////Motion Test
	Safe_Release(m_pMotionTrail);
	////////////
}