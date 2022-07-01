#include "stdafx.h"
#include "..\public\Monster_Vayusura_Leader.h"


CMonster_Vayusura_Leader::CMonster_Vayusura_Leader(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CMonster(pDevice, pDeviceContext)
{
}

CMonster_Vayusura_Leader::CMonster_Vayusura_Leader(const CMonster_Vayusura_Leader & rhs)
	: CMonster(rhs)
{
}

HRESULT CMonster_Vayusura_Leader::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));


	return S_OK;
}

HRESULT CMonster_Vayusura_Leader::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components());

	if (pArg != nullptr)
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, *((_float3*)pArg));

	m_pTransformCom->Rotation_CW(XMVectorSet(0, 1, 0, 0), XMConvertToRadians(170));

	m_pTransformCom->Scaled_All(_float3(150.f, 150.f, 150.f));

	SetUp_Info();

	return S_OK;
}

_int CMonster_Vayusura_Leader::Update(_double dDeltaTime)
{

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
	FAILED_CHECK(m_pModel->Update_AnimationClip(dDeltaTime * m_dAcceleration, m_bIsOnScreen));
	FAILED_CHECK(Adjust_AnimMovedTransform(dDeltaTime));

	return _int();
}

_int CMonster_Vayusura_Leader::LateUpdate(_double dDeltaTime)
{
	if (__super::LateUpdate(dDeltaTime) < 0)return -1;

	//////////
	if (m_bIsOnScreen)
	{
		FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
	}
	//////////

	//FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
	m_vOldPos = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS);

	return _int();
}

_int CMonster_Vayusura_Leader::Render()
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

_int CMonster_Vayusura_Leader::LateRender()
{
	if (__super::LateRender() < 0)
		return -1;

	return _int();
}

HRESULT CMonster_Vayusura_Leader::SetUp_Info()
{

	m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, _float3(2.f, 0.f, 2.f));


	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	m_pPlayerTransform = static_cast<CTransform*>(pGameInstance->Get_Commponent_By_LayerIndex(m_eNowSceneNum, TAG_LAY(Layer_Player), TAG_COM(Com_Transform)));

	RELEASE_INSTANCE(CGameInstance);


	return S_OK;
}

HRESULT CMonster_Vayusura_Leader::SetUp_Fight(_double dDeltaTime)
{
	m_fDistance = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS).Get_Distance(m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS));


	m_fPlayerPos = m_pPlayerTransform->Get_MatrixState_Float3(CTransform::STATE_POS);


	if (m_bLookAtOn)
	{
		//m_pTransformCom->LookAt(m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS));


		_Vector vTarget = XMVector3Normalize(m_pPlayerTransform->Get_MatrixState(CTransform::STATE_POS) - m_pTransformCom->Get_MatrixState(CTransform::STATE_POS));

		//m_pTransformCom->Turn_Dir(XMVector3Normalize(m_pTransformCom->Get_MatrixScale(CTransform::STATE_LOOK)*0.9 + vTarget* 0.1));
		m_pTransformCom->Turn_Dir(vTarget, 0.9f);
	}

	//CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	////평범하게 움직이기
	//if (pGameInstance->Get_DIKeyState(DIK_Y) & DIS_Press)
	//{
	//	m_pTransformCom->Move_Up(dDeltaTime);
	//}
	//if (pGameInstance->Get_DIKeyState(DIK_H) & DIS_Press)
	//{
	//	m_pTransformCom->Move_Down(dDeltaTime);
	//}

	//RELEASE_INSTANCE(CGameInstance);


	return S_OK;
}

HRESULT CMonster_Vayusura_Leader::PlayAnim(_double dDeltaTime)
{
	SetUp_Fight(dDeltaTime);

	CoolTime_Manager(dDeltaTime);

	if (true == m_bIOnceAnimSwitch)
	{
		Once_AnimMotion(dDeltaTime);
		m_pModel->Change_AnimIndex(m_iOnceAnimNumber);
	}
	else
	{
		Infinity_AnimMotion(dDeltaTime);
		m_pModel->Change_AnimIndex(m_iInfinityAnimNumber);
	}

	return S_OK;
}

HRESULT CMonster_Vayusura_Leader::CoolTime_Manager(_double dDeltaTime)
{
	//한번만 동작하는 애니메이션

	m_dOnceCoolTime += dDeltaTime;

	if (m_dOnceCoolTime > 2 && m_fDistance < 3 || m_bComboAnimSwitch == true)
	{
		m_dOnceCoolTime = 0;
		m_dInfinity_CoolTime = 0;

		if (m_bIOnceAnimSwitch == false)
		{
			Pattern_Change();


			m_bIOnceAnimSwitch = true;
		}
	}

	//반복적으로 동작하는 애니메이션
	m_dInfinity_CoolTime += dDeltaTime;
	if (m_dInfinity_CoolTime >= 1.5)
	{
		m_iInfinityPattern = rand() % 4;


		m_dInfinity_CoolTime = 0;
	}

	return S_OK;
}

HRESULT CMonster_Vayusura_Leader::Once_AnimMotion(_double dDeltaTime)
{

	switch (m_iOncePattern)
	{
	case 0:
		m_iOnceAnimNumber = 5; //Vayusura_Dive
		break;
	case 1:
		m_iOnceAnimNumber = 5; //Vayusura_Dive
		break;

	}

	return S_OK;
}

HRESULT CMonster_Vayusura_Leader::Pattern_Change()
{

	m_iOncePattern += 1;

	if (m_iOncePattern > 2)
	{
		//m_iOncePattern = rand() % 2; //OncePattern Random
	}


	return S_OK;
}

HRESULT CMonster_Vayusura_Leader::Infinity_AnimMotion(_double dDeltaTime)
{
	switch (m_iInfinityPattern)
	{
	case 0:
		if (m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS).y < m_fPlayerPos.y + 7)
		{
			m_pTransformCom->Move_Up(dDeltaTime * 0.5);
		}
		m_iInfinityAnimNumber = 0; // flapping
		break;
	case 1:
		if (m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS).y > m_fPlayerPos.y + 4)
		{
			m_pTransformCom->Move_Down(dDeltaTime * 0.5);
		}
		m_iInfinityAnimNumber = 0;
		break;
	case 2:
		if (m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS).x < m_fPlayerPos.x + 10 &&
			m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS).x > m_fPlayerPos.x - 10)
		{

			m_pTransformCom->MovetoDir(XMLoadFloat4(&_float4(1.f, 0.f, 0.f, 0.f)), dDeltaTime);
		}
		m_iInfinityAnimNumber = 1;
		break;
	case 3:
		if (m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS).x < m_fPlayerPos.x + 10 &&
			m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS).x > m_fPlayerPos.x - 10)
		{

			m_pTransformCom->MovetoDir(XMLoadFloat4(&_float4(1.f, 0.f, 0.f, 0.f)), -dDeltaTime);
		}
		m_iInfinityAnimNumber = 1;
		break;
	}

	return S_OK;
}

HRESULT CMonster_Vayusura_Leader::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_Monster_Vayusura_Minion), TAG_COM(Com_Model), (CComponent**)&m_pModel));
	FAILED_CHECK(m_pModel->Change_AnimIndex(0));


	CTransform::TRANSFORMDESC tDesc = {};

	tDesc.fMovePerSec = 5;
	tDesc.fRotationPerSec = XMConvertToRadians(60);
	tDesc.vPivot = _float3(0, 0, 0);
	tDesc.fScalingPerSec = 1;

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));


	return S_OK;
}

HRESULT CMonster_Vayusura_Leader::Adjust_AnimMovedTransform(_double dDeltaTime)
{
	_uint iNowAnimIndex = m_pModel->Get_NowAnimIndex();
	_double PlayRate = m_pModel->Get_PlayRate();

	if (iNowAnimIndex != m_iOldAnimIndex || PlayRate > 0.98)
	{
		m_iAdjMovedIndex = 0;

		m_bLookAtOn = true;

		if (PlayRate > 0.98 && m_bIOnceAnimSwitch == true)
		{
			m_bIOnceAnimSwitch = false;
			m_dOnceCoolTime = 0;
			m_dInfinity_CoolTime = 0;
		}
	}

	if (PlayRate <= 0.98) //애니메이션의 비율 즉, 0.98은 거의 끝나가는 시점
	{


		//switch (iNowAnimIndex)
		//{
		//case 1://애니메이션 인덱스마다 잡아주면 됨
		//	if (m_iAdjMovedIndex == 0 && PlayRate > 0.0) // 이렇게 되면 이전 애니메이션에서 보간되는 시간 끝나자 마자 바로 들어옴 즉, PlayRate의 0은 >= 하지말고 >로 하셈
		//	{

		//		m_iAdjMovedIndex++; //애니메이션이 동작할 때 한번만 발동시키기 위해 ++시킨다.
		//	}
		//	else if (m_iAdjMovedIndex == 1 && PlayRate > 0.7666666666666666) //특정 프레임 플레이 레이트이후에 들어오면실행
		//	{


		//		m_iAdjMovedIndex++;
		//	}

		//	break;
		//case 2:

		//	break;
		//}



		switch (iNowAnimIndex)
		{
		case 2:
		{
			if (PlayRate >= 0.2 && PlayRate <= 0.6)
			{
				m_bLookAtOn = false;
				m_pTransformCom->Move_Backward(dDeltaTime * 0.3);
			}
			break;
		}
		case 3:
		{
			if (PlayRate <= 0.48)
			{
				if (m_iAdjMovedIndex == 0)
				{
					m_TempLook = m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK);

					m_iAdjMovedIndex += 1;
				}
				_Vector vRight = XMVector3Cross(XMLoadFloat3(&_float3(0.f, 1.f, 0.f)), XMLoadFloat3(&m_TempLook));

				_float3 vDir;
				XMStoreFloat3(&vDir, XMVector3Normalize(vRight) * 3 * (_float)dDeltaTime);


				m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, m_pTransformCom->Get_MatrixState(CTransform::STATE_POS) + XMLoadFloat3(&vDir));
			}
			break;
		}
		case 4:
		{
			if (PlayRate <= 0.48)
			{
				if (m_iAdjMovedIndex == 0)
				{
					m_TempLook = m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK);

					m_iAdjMovedIndex += 1;
				}
				_Vector vRight = XMVector3Cross(XMLoadFloat3(&_float3(0.f, 1.f, 0.f)), XMLoadFloat3(&m_TempLook)); //Left

				_float3 vDir;
				XMStoreFloat3(&vDir, XMVector3Normalize(-vRight) * 3 * (_float)dDeltaTime);


				m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, m_pTransformCom->Get_MatrixState(CTransform::STATE_POS) + XMLoadFloat3(&vDir));
			}
			break;
		}
		case 18:
		{
			if (PlayRate >= 0.27272 && PlayRate <= 0.444)
			{
				m_bLookAtOn = false;

				_float EasingSpeed;
				EasingSpeed = GetSingle(CGameInstance)->Easing(TYPE_QuadIn, 1.5f, 2.2f, PlayRate - 0.27272f, 0.17128f);

				m_pTransformCom->Move_Forward(dDeltaTime * EasingSpeed);
			}
			else if (PlayRate >= 0.444 && PlayRate <= 0.6)
			{
				_float EasingSpeed;
				EasingSpeed = GetSingle(CGameInstance)->Easing(TYPE_QuadOut, 2.2f, 1.f, PlayRate - 0.444f, 0.156f);

				m_pTransformCom->Move_Forward(dDeltaTime * EasingSpeed);
			}
			break;
		}
		case 19: {
			if (PlayRate >= 0.24 && PlayRate <= 0.48)
			{
				m_bLookAtOn = false;
				m_pTransformCom->Move_Forward(dDeltaTime * 0.6);
			}
			else if (PlayRate >= 0.49 && PlayRate <= 0.84)
			{
				m_pTransformCom->Move_Forward(dDeltaTime * 1.2);
			}
			break;
		}
		case 20: {
			if (PlayRate >= 0.24 && PlayRate <= 0.6)
			{
				m_bLookAtOn = false;


				_float EasingSpeed;
				EasingSpeed = GetSingle(CGameInstance)->Easing(TYPE_CircularOut, 1.7f, 1.f, PlayRate - 0.24f, 0.36f);

				m_pTransformCom->Move_Forward(dDeltaTime * EasingSpeed);

				//m_pTransformCom->Move_Forward(dDeltaTime * 1.2);
			}
			break;
		}
		case 21: {
			if (PlayRate > 0 && PlayRate <= 0.720720)
			{
				m_dAcceleration = 1.5;
			}
			else if (PlayRate >= 0.720720 && PlayRate <= 0.875)
			{
				m_dAcceleration = 3;
			}
			else {
				m_dAcceleration = 1;
			}
			break;
		}
		case 22: {
			if (PlayRate > 0 && PlayRate >= 0.125)
			{
				m_pTransformCom->Move_Forward(dDeltaTime * 1.2);
			}
			break;
		}
		}
	}

	m_iOldAnimIndex = iNowAnimIndex;
	return S_OK;
}

CMonster_Vayusura_Leader * CMonster_Vayusura_Leader::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CMonster_Vayusura_Leader*	pInstance = new CMonster_Vayusura_Leader(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CMonster_Vayusura_Leader");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMonster_Vayusura_Leader::Clone(void * pArg)
{
	CMonster_Vayusura_Leader*	pInstance = new CMonster_Vayusura_Leader(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CMonster_Vayusura_Leader");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMonster_Vayusura_Leader::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModel);
}