#include "stdafx.h"
#include "..\public\PM_Monster.h"
#include "PackMen.h"

vector<PMTP> CPM_Monster::m_vecTurningPoints;


CPM_Monster::CPM_Monster(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CPM_Monster::CPM_Monster(const CPM_Monster & rhs)
	: CGameObject(rhs)
{
}


HRESULT CPM_Monster::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

#pragma region ReadyTurnigPoints
	m_vecTurningPoints.clear();
	
	vector<_float3> vecTempPosition;
	vecTempPosition.clear();
	{
		vecTempPosition.push_back(_float3(-22.586998f, 0.800010f, 5.757167f));
		vecTempPosition.push_back(_float3(-17.669325f, 0.800010f, 5.757167f));
		vecTempPosition.push_back(_float3(-14.834543f, 0.800010f, 5.757167f));
		vecTempPosition.push_back(_float3(-9.166116f, 0.800010f, 5.757167f));
		vecTempPosition.push_back(_float3(-6.165494f, 0.800010f, 5.757167f));
		vecTempPosition.push_back(_float3(-1.413313f, 0.800010f, 5.757167f));
		vecTempPosition.push_back(_float3(1.339999f, 0.800010f, 5.757167f));
		vecTempPosition.push_back(_float3(6.841322f, 0.800010f, 5.757167f));
		vecTempPosition.push_back(_float3(9.847200f, 0.800010f, 5.757167f));
		vecTempPosition.push_back(_float3(14.683441f, 0.800010f, 5.757167f));
		vecTempPosition.push_back(_float3(17.768784f, 0.800010f, 5.757167f));
		vecTempPosition.push_back(_float3(23.354647f, 0.800010f, 5.757167f));
		vecTempPosition.push_back(_float3(-9.166116f, 0.800010f, -0.583812f));
		vecTempPosition.push_back(_float3(-6.165494f, 0.800010f, -0.583812f));
		vecTempPosition.push_back(_float3(-1.413313f, 0.800010f, -0.583812f));
		vecTempPosition.push_back(_float3(1.339999f, 0.800010f, -0.583812f));
		vecTempPosition.push_back(_float3(6.841322f, 0.800010f, -0.583812f));
		vecTempPosition.push_back(_float3(9.847200f, 0.800010f, -0.583812f));
		vecTempPosition.push_back(_float3(-22.586998f, 0.800010f, -6.442939f));
		vecTempPosition.push_back(_float3(-17.669325f, 0.800010f, -6.442939f));
		vecTempPosition.push_back(_float3(-14.834543f, 0.800010f, -6.442939f));
		vecTempPosition.push_back(_float3(-9.166116f, 0.800010f, -6.442939f));
		vecTempPosition.push_back(_float3(-6.165494f, 0.800010f, -6.442939f));
		vecTempPosition.push_back(_float3(-1.413313f, 0.800010f, -6.442939f));
		vecTempPosition.push_back(_float3(1.339999f, 0.800010f, -6.442939f));
		vecTempPosition.push_back(_float3(6.841322f, 0.800010f, -6.442939f));
		vecTempPosition.push_back(_float3(9.847200f, 0.800010f, -6.442939f));
		vecTempPosition.push_back(_float3(14.683441f, 0.800010f, -6.442939f));
		vecTempPosition.push_back(_float3(17.768784f, 0.800010f, -6.442939f));
		vecTempPosition.push_back(_float3(23.354647f, 0.800010f, -6.442939f));
	}




	_uint iNum = 0;;



	{
		//0
		PMTP tDesc;
		tDesc.vPosition = vecTempPosition[iNum];
		iNum++;
		tDesc.iNeighborIndexArr[0] = 1;
		tDesc.iNeighborIndexArr[1] = 18;
		tDesc.iNeighborIndexArr[2] = INT_MAX;
		tDesc.iNeighborIndexArr[3] = INT_MAX;
		m_vecTurningPoints.push_back(tDesc);
	}

	{
		//1
		PMTP tDesc;
		tDesc.vPosition = vecTempPosition[iNum];
		iNum++;
		tDesc.iNeighborIndexArr[0] = 2;
		tDesc.iNeighborIndexArr[1] = 19;
		tDesc.iNeighborIndexArr[2] = 0;
		tDesc.iNeighborIndexArr[3] = INT_MAX;
		m_vecTurningPoints.push_back(tDesc);

	}

	{
		//2
		PMTP tDesc;
		tDesc.vPosition = vecTempPosition[iNum];
		iNum++;
		tDesc.iNeighborIndexArr[0] = 3;
		tDesc.iNeighborIndexArr[1] = 20;
		tDesc.iNeighborIndexArr[2] = 1;
		tDesc.iNeighborIndexArr[3] = INT_MAX;
		m_vecTurningPoints.push_back(tDesc);
	}
	{
		//3
		PMTP tDesc;
		tDesc.vPosition = vecTempPosition[iNum];
		iNum++;
		tDesc.iNeighborIndexArr[0] = INT_MAX;
		tDesc.iNeighborIndexArr[1] = 12;
		tDesc.iNeighborIndexArr[2] = 2;
		tDesc.iNeighborIndexArr[3] = INT_MAX;
		m_vecTurningPoints.push_back(tDesc);
	}
	{
		//4
		PMTP tDesc;
		tDesc.vPosition = vecTempPosition[iNum];
		iNum++;
		tDesc.iNeighborIndexArr[0] = 5;
		tDesc.iNeighborIndexArr[1] = 13;
		tDesc.iNeighborIndexArr[2] = INT_MAX;
		tDesc.iNeighborIndexArr[3] = INT_MAX;
		m_vecTurningPoints.push_back(tDesc);
	}
	{
		//5
		PMTP tDesc;
		tDesc.vPosition = vecTempPosition[iNum];
		iNum++;
		tDesc.iNeighborIndexArr[0] = 6;
		tDesc.iNeighborIndexArr[1] = 14;
		tDesc.iNeighborIndexArr[2] = 4;
		tDesc.iNeighborIndexArr[3] = INT_MAX;
		m_vecTurningPoints.push_back(tDesc);
	}
	{
		//6
		PMTP tDesc;
		tDesc.vPosition = vecTempPosition[iNum];
		iNum++;
		tDesc.iNeighborIndexArr[0] = 7;
		tDesc.iNeighborIndexArr[1] = 15;
		tDesc.iNeighborIndexArr[2] = 5;
		tDesc.iNeighborIndexArr[3] = INT_MAX;
		m_vecTurningPoints.push_back(tDesc);
	}
	{
		//7
		PMTP tDesc;
		tDesc.vPosition = vecTempPosition[iNum];
		iNum++;
		tDesc.iNeighborIndexArr[0] = INT_MAX;
		tDesc.iNeighborIndexArr[1] = 16;
		tDesc.iNeighborIndexArr[2] = 6;
		tDesc.iNeighborIndexArr[3] = INT_MAX;
		m_vecTurningPoints.push_back(tDesc);
	}
	{
		//8
		PMTP tDesc;
		tDesc.vPosition = vecTempPosition[iNum];
		iNum++;
		tDesc.iNeighborIndexArr[0] = 9;
		tDesc.iNeighborIndexArr[1] = 17;
		tDesc.iNeighborIndexArr[2] = INT_MAX;
		tDesc.iNeighborIndexArr[3] = INT_MAX;
		m_vecTurningPoints.push_back(tDesc);
	}
	{
		//9
		PMTP tDesc;
		tDesc.vPosition = vecTempPosition[iNum];
		iNum++;
		tDesc.iNeighborIndexArr[0] = 10;
		tDesc.iNeighborIndexArr[1] = 27;
		tDesc.iNeighborIndexArr[2] = 8;
		tDesc.iNeighborIndexArr[3] = INT_MAX;
		m_vecTurningPoints.push_back(tDesc);
	}
	{
		//10
		PMTP tDesc;
		tDesc.vPosition = vecTempPosition[iNum];
		iNum++;
		tDesc.iNeighborIndexArr[0] = 11;
		tDesc.iNeighborIndexArr[1] = 28;
		tDesc.iNeighborIndexArr[2] = 9;
		tDesc.iNeighborIndexArr[3] = INT_MAX;
		m_vecTurningPoints.push_back(tDesc);
	}
	{
		//11
		PMTP tDesc;
		tDesc.vPosition = vecTempPosition[iNum];
		iNum++;
		tDesc.iNeighborIndexArr[0] = INT_MAX;
		tDesc.iNeighborIndexArr[1] = 29;
		tDesc.iNeighborIndexArr[2] = 10;
		tDesc.iNeighborIndexArr[3] = INT_MAX;
		m_vecTurningPoints.push_back(tDesc);
	}

	//////////////////////////////////////////////////////////////////////////

	{
		//12
		PMTP tDesc;
		tDesc.vPosition = vecTempPosition[iNum];
		iNum++;
		tDesc.iNeighborIndexArr[0] = 13;
		tDesc.iNeighborIndexArr[1] = 21;
		tDesc.iNeighborIndexArr[2] = INT_MAX;
		tDesc.iNeighborIndexArr[3] = 3;
		m_vecTurningPoints.push_back(tDesc);
	}
	{
		//13
		PMTP tDesc;
		tDesc.vPosition = vecTempPosition[iNum];
		iNum++;
		tDesc.iNeighborIndexArr[0] = INT_MAX;
		tDesc.iNeighborIndexArr[1] = 22;
		tDesc.iNeighborIndexArr[2] = 12;
		tDesc.iNeighborIndexArr[3] = 4;
		m_vecTurningPoints.push_back(tDesc);
	}
	{
		//14
		PMTP tDesc;
		tDesc.vPosition = vecTempPosition[iNum];
		iNum++;
		tDesc.iNeighborIndexArr[0] = 15;
		tDesc.iNeighborIndexArr[1] = 23;
		tDesc.iNeighborIndexArr[2] = INT_MAX;
		tDesc.iNeighborIndexArr[3] = 5;
		m_vecTurningPoints.push_back(tDesc);
	}
	{
		//15
		PMTP tDesc;
		tDesc.vPosition = vecTempPosition[iNum];
		iNum++;
		tDesc.iNeighborIndexArr[0] = INT_MAX;
		tDesc.iNeighborIndexArr[1] = 24;
		tDesc.iNeighborIndexArr[2] = 14;
		tDesc.iNeighborIndexArr[3] = 6;
		m_vecTurningPoints.push_back(tDesc);
	}
	{
		//16
		PMTP tDesc;
		tDesc.vPosition = vecTempPosition[iNum];
		iNum++;
		tDesc.iNeighborIndexArr[0] = 17;
		tDesc.iNeighborIndexArr[1] = 25;
		tDesc.iNeighborIndexArr[2] = INT_MAX;
		tDesc.iNeighborIndexArr[3] = 7;
		m_vecTurningPoints.push_back(tDesc);
	}
	{
		//17
		PMTP tDesc;
		tDesc.vPosition = vecTempPosition[iNum];
		iNum++;
		tDesc.iNeighborIndexArr[0] = INT_MAX;
		tDesc.iNeighborIndexArr[1] = 26;
		tDesc.iNeighborIndexArr[2] = 16;
		tDesc.iNeighborIndexArr[3] = 8;
		m_vecTurningPoints.push_back(tDesc);
	}

	//////////////////////////////////////////////////////////////////////////
	{
		//18
		PMTP tDesc;
		tDesc.vPosition = vecTempPosition[iNum];
		iNum++;
		tDesc.iNeighborIndexArr[0] = 19;
		tDesc.iNeighborIndexArr[1] = INT_MAX;
		tDesc.iNeighborIndexArr[2] = INT_MAX;
		tDesc.iNeighborIndexArr[3] = 0;
		m_vecTurningPoints.push_back(tDesc);
	}
	{
		//19
		PMTP tDesc;
		tDesc.vPosition = vecTempPosition[iNum];
		iNum++;
		tDesc.iNeighborIndexArr[0] = 20;
		tDesc.iNeighborIndexArr[1] = INT_MAX;
		tDesc.iNeighborIndexArr[2] = 18;
		tDesc.iNeighborIndexArr[3] = 1;
		m_vecTurningPoints.push_back(tDesc);
	}
	{
		//20
		PMTP tDesc;
		tDesc.vPosition = vecTempPosition[iNum];
		iNum++;
		tDesc.iNeighborIndexArr[0] = 21;
		tDesc.iNeighborIndexArr[1] = INT_MAX;
		tDesc.iNeighborIndexArr[2] = 19;
		tDesc.iNeighborIndexArr[3] = 2;
		m_vecTurningPoints.push_back(tDesc);
	}
	{
		//21
		PMTP tDesc;
		tDesc.vPosition = vecTempPosition[iNum];
		iNum++;
		tDesc.iNeighborIndexArr[0] = INT_MAX;
		tDesc.iNeighborIndexArr[1] = INT_MAX;
		tDesc.iNeighborIndexArr[2] = 20;
		tDesc.iNeighborIndexArr[3] = 12;
		m_vecTurningPoints.push_back(tDesc);
	}
	{
		//22
		PMTP tDesc;
		tDesc.vPosition = vecTempPosition[iNum];
		iNum++;
		tDesc.iNeighborIndexArr[0] = 23;
		tDesc.iNeighborIndexArr[1] = INT_MAX;
		tDesc.iNeighborIndexArr[2] = INT_MAX;
		tDesc.iNeighborIndexArr[3] = 13;
		m_vecTurningPoints.push_back(tDesc);
	}
	{
		//23
		PMTP tDesc;
		tDesc.vPosition = vecTempPosition[iNum];
		iNum++;
		tDesc.iNeighborIndexArr[0] = 24;
		tDesc.iNeighborIndexArr[1] = INT_MAX;
		tDesc.iNeighborIndexArr[2] = 22;
		tDesc.iNeighborIndexArr[3] = 14;
		m_vecTurningPoints.push_back(tDesc);
	}
	{
		//24
		PMTP tDesc;
		tDesc.vPosition = vecTempPosition[iNum];
		iNum++;
		tDesc.iNeighborIndexArr[0] = 25;
		tDesc.iNeighborIndexArr[1] = INT_MAX;
		tDesc.iNeighborIndexArr[2] = 23;
		tDesc.iNeighborIndexArr[3] = 15;
		m_vecTurningPoints.push_back(tDesc);
	}
	{
		//25
		PMTP tDesc;
		tDesc.vPosition = vecTempPosition[iNum];
		iNum++;
		tDesc.iNeighborIndexArr[0] = INT_MAX;
		tDesc.iNeighborIndexArr[1] = INT_MAX;
		tDesc.iNeighborIndexArr[2] = 24;
		tDesc.iNeighborIndexArr[3] = 16;
		m_vecTurningPoints.push_back(tDesc);
	}
	{
		//26
		PMTP tDesc;
		tDesc.vPosition = vecTempPosition[iNum];
		iNum++;
		tDesc.iNeighborIndexArr[0] = 27;
		tDesc.iNeighborIndexArr[1] = INT_MAX;
		tDesc.iNeighborIndexArr[2] = INT_MAX;
		tDesc.iNeighborIndexArr[3] = 17;
		m_vecTurningPoints.push_back(tDesc);
	}
	{
		//27
		PMTP tDesc;
		tDesc.vPosition = vecTempPosition[iNum];
		iNum++;
		tDesc.iNeighborIndexArr[0] = 28;
		tDesc.iNeighborIndexArr[1] = INT_MAX;
		tDesc.iNeighborIndexArr[2] = 26;
		tDesc.iNeighborIndexArr[3] = 9;
		m_vecTurningPoints.push_back(tDesc);
	}
	{
		//28
		PMTP tDesc;
		tDesc.vPosition = vecTempPosition[iNum];
		iNum++;
		tDesc.iNeighborIndexArr[0] = 29;
		tDesc.iNeighborIndexArr[1] = INT_MAX;
		tDesc.iNeighborIndexArr[2] = 27;
		tDesc.iNeighborIndexArr[3] = 10;
		m_vecTurningPoints.push_back(tDesc);
	}
	{
		//29
		PMTP tDesc;
		tDesc.vPosition = vecTempPosition[iNum];
		iNum++;
		tDesc.iNeighborIndexArr[0] = INT_MAX;
		tDesc.iNeighborIndexArr[1] = INT_MAX;
		tDesc.iNeighborIndexArr[2] = 28;
		tDesc.iNeighborIndexArr[3] = 11;
		m_vecTurningPoints.push_back(tDesc);
	}






#pragma endregion

	return S_OK;
}

HRESULT CPM_Monster::Initialize_Clone(void * pArg)
{
	NULL_CHECK_RETURN(pArg, E_FAIL);

	FAILED_CHECK(__super::Initialize_Clone(pArg));

	memcpy(&m_tDesc, pArg, sizeof(PMMONDESC));

	FAILED_CHECK(SetUp_Components());

	m_iPassedPointIndex = m_iNowPointIndex = m_tDesc.iIndex;
	m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, m_vecTurningPoints[m_iNowPointIndex].vPosition);
	m_pNavigationCom->FindCellIndex(m_pTransformCom->Get_MatrixState(CTransform::STATE_POS));
	FAILED_CHECK(Find_Path_to_Player(0));
	m_PassedTimer = 0;
	return S_OK;
}

_int CPM_Monster::Update(_double dDeltaTime)
{
	if (__super::Update(dDeltaTime) < 0)return -1;

	if (m_bDeadTimer > 0)
	{

		m_bDeadTimer -= (_float)dDeltaTime;

		if (m_bDeadTimer <= 0)
		{
			m_iPassedPointIndex = m_iNowPointIndex = rand() % 30;
			m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, m_vecTurningPoints[m_iNowPointIndex].vPosition);
			m_pNavigationCom->FindCellIndex(m_pTransformCom->Get_MatrixState(CTransform::STATE_POS));
			FAILED_CHECK(Find_Path_to_Player(0));
			//
			INSTMESHDESC tDesc = GetSingle(CUtilityMgr)->Get_MeshParticleDesc(L"JY_Mesh_9");
			tDesc.TotalParticleTime = 0.001f;
			tDesc.ParticleSize2 = _float3(0.2f, 0.0001f, 0.2f);
			tDesc.vFixedPosition = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);

			GetSingle(CUtilityMgr)->Create_MeshInstance(m_eNowSceneNum, tDesc);


		}

		return 0;
	}
	

	if (m_PassedTimer < 0.5f)
	{
		m_PassedTimer += (_float)dDeltaTime;
		return 1;
	}

	if (m_pPackMan->Get_IsBustTimer() > 0)
	{
		if (m_bOld == false)
		{

			_uint iChangerBuffer = m_iPassedPointIndex;
			m_iPassedPointIndex = m_iNowPointIndex;
			m_iNowPointIndex = iChangerBuffer;

			m_vMoveToNextPointDir = m_vMoveToNextPointDir.XMVector() * -1.f;

			m_pTransformCom->Set_MoveSpeed(GetSingle(CUtilityMgr)->RandomFloat(1.f, 2.5f));
		}


		m_fColorPassedTimer -= (_float)dDeltaTime;

		if (m_fColorPassedTimer < 0)
		{
			m_vOldColor = m_vNewColor;
			m_vNewColor = GetSingle(CUtilityMgr)->RandomFloat3(0, 1);
			m_fColorPassedTimer = 0.35f;
		}

		_float3 vColor = g_pGameInstance->Easing_Vector(TYPE_SinInOut, m_vOldColor, m_vNewColor, 0.35f - m_fColorPassedTimer, 0.35f);
		_float vValue = g_pGameInstance->Easing_Return(TYPE_SinInOut, TYPE_SinInOut, 0, 1, 3.f - m_pPackMan->Get_IsBustTimer(), 3.f);
		Set_LimLight_N_Emissive(_float4(vColor, vValue), _float4(1.f,0.5f,1.f,0).XMVector() * vValue);

		m_bOld = true;
	}
	else
	{
		if (m_bOld == true)
		{

			_uint iChangerBuffer = m_iPassedPointIndex;
			m_iPassedPointIndex = m_iNowPointIndex;
			m_iNowPointIndex = iChangerBuffer;

			m_vMoveToNextPointDir = m_vMoveToNextPointDir.XMVector() * -1.f;

			m_pTransformCom->Set_MoveSpeed(GetSingle(CUtilityMgr)->RandomFloat(2.5f, 5.f));
		}

		Set_LimLight_N_Emissive(_float4(0), _float4(0));
		m_bOld = false;
	}


	FAILED_CHECK(Update_MonsterPattern(dDeltaTime));





	m_bIsOnScreen = g_pGameInstance->IsNeedToRender(m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS), m_fFrustumRadius);

	FAILED_CHECK(m_pModel->Update_AnimationClip(dDeltaTime * m_dAcceleration, m_bIsOnScreen));
	FAILED_CHECK(Adjust_AnimMovedTransform(dDeltaTime));


	Update_Collider(dDeltaTime);

	return _int();
}

_int CPM_Monster::LateUpdate(_double dDeltaTime)
{
	if (m_bDeadTimer > 0)
		return 0;

	if (__super::LateUpdate(dDeltaTime) < 0)return -1;

	if (m_bIsOnScreen)
	{
		FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
	}

	FAILED_CHECK(m_pRendererCom->Add_ShadowGroup(CRenderer::SHADOW_ANIMMODEL, this, m_pTransformCom, m_pShaderCom, m_pModel, nullptr, m_pDissolve));

	FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pColliderCom));


	return _int();
}

_int CPM_Monster::Render()
{
	if (m_bDeadTimer > 0)
		return 0;

	if (__super::Render() < 0)
		return -1;

	NULL_CHECK_RETURN(m_pModel, E_FAIL);

	CGameInstance* pInstance = GetSingle(CGameInstance);
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));


	FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));

	FAILED_CHECK(m_pDissolve->Render(3)); //디졸브 내부에서 밑의 머테리얼을 찾아주고 있음

#ifdef _DEBUG
	//FAILED_CHECK(m_pNavigationCom->Render());
#endif
	return _int();
}

_int CPM_Monster::LateRender()
{
	return _int();
}

void CPM_Monster::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
	if (m_pPackMan->Get_IsBustTimer() > 0)
	{
		m_bDeadTimer = GetSingle(CUtilityMgr)->RandomFloat(2.5f, 3.5f);

		g_pGameInstance->Play3D_Sound(TEXT("PM_Attack.wav"), g_pGameInstance->Get_TargetPostion_float4(PLV_CAMERA), CHANNELID::CHANNEL_PLAYER, 1.f);
		
	}
	else
	{
		m_pPackMan->Added_Damaged();
	}


}

HRESULT CPM_Monster::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_PMMonster), TAG_COM(Com_Model), (CComponent**)&m_pModel));
	FAILED_CHECK(m_pModel->Change_AnimIndex(0));
	FAILED_CHECK(m_pModel->Update_AnimationClip(g_fDeltaTime * m_dAcceleration, true));

	CTransform::TRANSFORMDESC tDesc = {};

	tDesc.fMovePerSec = GetSingle(CUtilityMgr)->RandomFloat(2.5f,5.f);
	tDesc.fRotationPerSec = XMConvertToRadians(360);
	tDesc.vPivot = _float3(0, 0, 0);
	tDesc.fScalingPerSec = 1;

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));


	CDissolve::DISSOLVEDESC DissolveDesc;
	DissolveDesc.pModel = m_pModel;
	DissolveDesc.eDissolveModelType = CDissolve::DISSOLVE_ANIM;
	DissolveDesc.pShader = m_pShaderCom;
	DissolveDesc.RampTextureIndex = 4;
	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Dissolve), TAG_COM(Com_Dissolve), (CComponent**)&m_pDissolve, &DissolveDesc));


	CNavigation::NAVIDESC NaviDesc;
	NaviDesc.iCurrentIndex = 0;
	if (FAILED(__super::Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Navigation), TAG_COM(Com_Navaigation), (CComponent**)&m_pNavigationCom, &NaviDesc)))
		return E_FAIL;


	FAILED_CHECK(SetUp_Collider());

	m_pPlayerTransfom = (CTransform*)g_pGameInstance->Get_Commponent_By_LayerIndex(m_eNowSceneNum, TAG_LAY(Layer_Player),TAG_COM(Com_Transform));
	NULL_CHECK_RETURN(m_pPlayerTransfom, E_FAIL);

	m_pPackMan = (CPackMen*)g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_Player));
	NULL_CHECK_RETURN(m_pPackMan, E_FAIL);

	return S_OK;
}

HRESULT CPM_Monster::Adjust_AnimMovedTransform(_double dDeltatime)
{
	_uint iNowAnimIndex = m_pModel->Get_NowAnimIndex();
	_double PlayRate = m_pModel->Get_PlayRate();

	if (iNowAnimIndex != m_iOldAnimIndex || PlayRate > 0.98)
		m_iAdjMovedIndex = 0;


	if (PlayRate <= 0.98)
	{
		switch (iNowAnimIndex)
		{
		case 1://애니메이션 인덱스마다 잡아주면 됨
			if (m_iAdjMovedIndex == 0 && PlayRate > 0.0) // 이렇게 되면 이전 애니메이션에서 보간되는 시간 끝나자 마자 바로 들어옴
			{

				m_iAdjMovedIndex++;
			}
			else if (m_iAdjMovedIndex == 1 && PlayRate > 0.7666666666666666) //특정 프레임 플레이 레이트이후에 들어오면실행
			{


				m_iAdjMovedIndex++;
			}

			break;
		case 2:

			break;
		}
	}


	m_iOldAnimIndex = iNowAnimIndex;
	return S_OK;
}


HRESULT CPM_Monster::SetUp_Collider()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_Collider), (CComponent**)&m_pColliderCom));


	COLLIDERDESC			ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
	ColliderDesc.vScale = _float3(1.5f, 1.5f, 1.5f);
	ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
	ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));



	return S_OK;
}

HRESULT CPM_Monster::Update_Collider(_double dDeltaTime)
{
	m_pColliderCom->Update_ConflictPassedTime(dDeltaTime);

	//Collider
	_uint	iNumCollider = m_pColliderCom->Get_NumColliderBuffer();
	for (_uint i = 0; i < iNumCollider; i++)
	{
		_Matrix mat = XMMatrixIdentity();
		mat.r[3] = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);

		m_pColliderCom->Update_Transform(i, mat);
	}

	FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_MonsterWeapon, this, m_pColliderCom));
	return S_OK;
}

HRESULT CPM_Monster::Update_MonsterPattern(_double dDeltatime)
{
	m_pTransformCom->Turn_CCW(XMVectorSet(0, 1, 0, 0), dDeltatime);

	m_pTransformCom->MovetoDir(m_vMoveToNextPointDir.XMVector(), dDeltatime, m_pNavigationCom);

	if (m_vecTurningPoints[m_iNowPointIndex].vPosition.Get_Distance(m_pTransformCom->Get_MatrixState(CTransform::STATE_POS)) < 0.1f)
	{
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, m_vecTurningPoints[m_iNowPointIndex].vPosition);


		if (m_pPackMan->Get_IsBustTimer() > 0)
		{
			FAILED_CHECK(Find_Path_to_Evading(dDeltatime));
		}
		else
		{
			FAILED_CHECK(Find_Path_to_Player(dDeltatime));
		}

	}


	return S_OK;
}

HRESULT CPM_Monster::Find_Path_to_Player(_double dDeltatime)
{

	_Vector ToPlayerDir = XMVector3Normalize(m_pPlayerTransfom->Get_MatrixState(CTransform::STATE_POS) - m_vecTurningPoints[m_iNowPointIndex].vPosition.XMVector());

	_uint iNextIndex = INT_MAX;
	_float fLength = -FLT_MAX;

	if (m_iPassedPointIndex != m_vecTurningPoints[m_iNowPointIndex].iNeighborIndexArr[0] &&
		m_vecTurningPoints[m_iNowPointIndex].iNeighborIndexArr[0] < 999 &&
		XMVectorGetX(ToPlayerDir) > fLength)
	{
		m_vMoveToNextPointDir = _float3(1, 0, 0);
		iNextIndex = m_vecTurningPoints[m_iNowPointIndex].iNeighborIndexArr[0];
		fLength = XMVectorGetX(ToPlayerDir);
	}


	if (m_iPassedPointIndex != m_vecTurningPoints[m_iNowPointIndex].iNeighborIndexArr[1] &&
		m_vecTurningPoints[m_iNowPointIndex].iNeighborIndexArr[1] < 999 &&
		-XMVectorGetZ(ToPlayerDir) > fLength)
	{
		m_vMoveToNextPointDir = _float3(0, 0, -1.f);
		iNextIndex = m_vecTurningPoints[m_iNowPointIndex].iNeighborIndexArr[1];
		fLength = -XMVectorGetZ(ToPlayerDir);
	}


	if (m_iPassedPointIndex != m_vecTurningPoints[m_iNowPointIndex].iNeighborIndexArr[2] &&
		m_vecTurningPoints[m_iNowPointIndex].iNeighborIndexArr[2] < 999 &&
		-XMVectorGetX(ToPlayerDir) > fLength)
	{
		m_vMoveToNextPointDir = _float3(-1.f, 0, 0);
		iNextIndex = m_vecTurningPoints[m_iNowPointIndex].iNeighborIndexArr[2];
		fLength = -XMVectorGetX(ToPlayerDir);
	}

	if (m_iPassedPointIndex != m_vecTurningPoints[m_iNowPointIndex].iNeighborIndexArr[3] &&
		m_vecTurningPoints[m_iNowPointIndex].iNeighborIndexArr[3] < 999 &&
		XMVectorGetZ(ToPlayerDir) > fLength)
	{
		m_vMoveToNextPointDir = _float3(0, 0, 1.f);
		iNextIndex = m_vecTurningPoints[m_iNowPointIndex].iNeighborIndexArr[3];
		fLength = XMVectorGetZ(ToPlayerDir);
	}

	m_iPassedPointIndex = m_iNowPointIndex;
	m_iNowPointIndex = iNextIndex;

	return S_OK;
}

HRESULT CPM_Monster::Find_Path_to_Evading(_double dDeltatime)
{

	_Vector ToPlayerDir = -XMVector3Normalize(m_pPlayerTransfom->Get_MatrixState(CTransform::STATE_POS) - m_vecTurningPoints[m_iNowPointIndex].vPosition.XMVector());

	_uint iNextIndex = INT_MAX;
	_float fLength = -FLT_MAX;

	if (m_vecTurningPoints[m_iNowPointIndex].iNeighborIndexArr[0] < 999 &&
		XMVectorGetX(ToPlayerDir) > fLength)
	{
		m_vMoveToNextPointDir = _float3(1, 0, 0);
		iNextIndex = m_vecTurningPoints[m_iNowPointIndex].iNeighborIndexArr[0];
		fLength = XMVectorGetX(ToPlayerDir);
	}


	if (m_vecTurningPoints[m_iNowPointIndex].iNeighborIndexArr[1] < 999 &&
		-XMVectorGetZ(ToPlayerDir) > fLength)
	{
		m_vMoveToNextPointDir = _float3(0, 0, -1.f);
		iNextIndex = m_vecTurningPoints[m_iNowPointIndex].iNeighborIndexArr[1];
		fLength = -XMVectorGetZ(ToPlayerDir);
	}


	if (m_vecTurningPoints[m_iNowPointIndex].iNeighborIndexArr[2] < 999 &&
		-XMVectorGetX(ToPlayerDir) > fLength)
	{
		m_vMoveToNextPointDir = _float3(-1.f, 0, 0);
		iNextIndex = m_vecTurningPoints[m_iNowPointIndex].iNeighborIndexArr[2];
		fLength = -XMVectorGetX(ToPlayerDir);
	}

	if (m_vecTurningPoints[m_iNowPointIndex].iNeighborIndexArr[3] < 999 &&
		XMVectorGetZ(ToPlayerDir) > fLength)
	{
		m_vMoveToNextPointDir = _float3(0, 0, 1.f);
		iNextIndex = m_vecTurningPoints[m_iNowPointIndex].iNeighborIndexArr[3];
		fLength = XMVectorGetZ(ToPlayerDir);
	}

	m_iPassedPointIndex = m_iNowPointIndex;
	m_iNowPointIndex = iNextIndex;

	return S_OK;





	return S_OK;
}


CPM_Monster * CPM_Monster::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CPM_Monster*	pInstance = NEW CPM_Monster(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CPM_Monster");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CPM_Monster::Clone(void * pArg)
{
	CPM_Monster*	pInstance = NEW CPM_Monster(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CPM_Monster");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPM_Monster::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModel);
	Safe_Release(m_pDissolve);

	Safe_Release(m_pColliderCom);
	Safe_Release(m_pNavigationCom);
} 
