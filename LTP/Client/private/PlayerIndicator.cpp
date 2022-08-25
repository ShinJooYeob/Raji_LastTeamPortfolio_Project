#include "stdafx.h"
#include "..\public\PlayerIndicator.h"
#include "Player.h"
#include "Boss.h"

CPlayerIndicator::CPlayerIndicator(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CPlayerIndicator::CPlayerIndicator(const CPlayerIndicator & rhs)
	: CGameObject(rhs)
{
}

HRESULT CPlayerIndicator::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CPlayerIndicator::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg)); 

	FAILED_CHECK(Ready_Components());
	FAILED_CHECK(Ready_EtcInfo());

	return S_OK;
}

_int CPlayerIndicator::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)	return -1;
	FAILED_CHECK(m_pDissolveCom->Update_Dissolving(fDeltaTime));
	FAILED_CHECK(Check_Monster(fDeltaTime));

	if (!m_bIsOnIndicated) return 0;

	FAILED_CHECK(Update_Moving(fDeltaTime));

	
	return _int();
}

_int CPlayerIndicator::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)return -1;

	if (!m_bIsOnIndicated) return 0;

	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_PRENONBLEND, this));
	return _int();
}

_int CPlayerIndicator::Render()
{
	if (!m_bIsOnIndicated) return 0;

	if (__super::Render() < 0)		return -1;

	FAILED_CHECK(m_pRendererCom->End_RenderTarget(TEXT("MRT_Material")));
	FAILED_CHECK(m_pRendererCom->Begin_RenderTarget(TEXT("MRT_OccludedMaterial")));
	NULL_CHECK_RETURN(m_pModel, E_FAIL);

	CGameInstance* pInstance = GetSingle(CGameInstance);

	
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_vColor", &m_vColor, sizeof(_float4)));

	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));
	FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));

	FAILED_CHECK(m_pDissolveCom->Render(26));


	FAILED_CHECK(m_pRendererCom->End_RenderTarget(TEXT("MRT_OccludedMaterial")));
	FAILED_CHECK(m_pRendererCom->Begin_RenderTarget(TEXT("MRT_Material")));
	return _int();
}

_int CPlayerIndicator::LateRender()
{
	return _int();
}

HRESULT CPlayerIndicator::Ready_Components()
{
	CTransform::TRANSFORMDESC tDesc = {};
	tDesc.fMovePerSec = 5;
	tDesc.fRotationPerSec = 1.f;
	tDesc.fScalingPerSec = 1;
	tDesc.vPivot = _float3(0, 0, 0);
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VNAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));
	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_Bullet), TAG_COM(Com_Model), (CComponent**)&m_pModel));


	CDissolve::DISSOLVEDESC	tDissolveDesc;
	tDissolveDesc.eDissolveModelType = CDissolve::DISSOLVE_NONANIM_OCLD;
	tDissolveDesc.pModel = m_pModel;
	tDissolveDesc.pShader = m_pShaderCom;
	tDissolveDesc.RampTextureIndex = 1;
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Dissolve), TAG_COM(Com_Dissolve), (CComponent**)&m_pDissolveCom, &tDissolveDesc));

	return S_OK;
}

HRESULT CPlayerIndicator::Ready_EtcInfo()
{
	m_pPlayer = (CPlayer*)g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, TAG_LAY(Layer_Player));
	NULL_CHECK_RETURN(m_pPlayer, E_FAIL);

	m_tPlayerHipBoneDesc.Initialize_AttachedDesc(m_pPlayer, "skd_hip",
		_float3(1), _float3(0), _float3(-0.074084f, -0.861011f, -75.1948f));

	m_vScale = _float3(0.0025f);
	m_pTransformCom->Scaled_All(m_vScale);
	return S_OK;
}


HRESULT CPlayerIndicator::Check_Monster(_double fDeltaTime)
{
	if (m_bTurningOff)
	{
		m_fTurnOffTime += _float(fDeltaTime);

		if (m_fTurnOffTime > 1.f)
		{
			m_bTurningOff = false;
			m_bIsOnIndicated = false;
			m_fTurnOffTime = 0;
		}
	}
	else
	{
		list<CGameObject*>* pMonsterList = nullptr;
		_uint NowMonsterCount = 0;

		pMonsterList = g_pGameInstance->Get_ObjectList_from_Layer(m_eNowSceneNum, TAG_LAY(Layer_Boss));
		if (pMonsterList != nullptr)
		{
			for (auto pObj : *pMonsterList)
				if (((CBoss*)(pObj))->Get_IsCombat())
					NowMonsterCount += (_uint)pMonsterList->size();
		}
		pMonsterList = g_pGameInstance->Get_ObjectList_from_Layer(m_eNowSceneNum, TAG_LAY(Layer_Unique_Monster));
		if (pMonsterList != nullptr) NowMonsterCount += (_uint)pMonsterList->size();
		pMonsterList = g_pGameInstance->Get_ObjectList_from_Layer(m_eNowSceneNum, TAG_LAY(Layer_Monster));
		if (pMonsterList != nullptr) NowMonsterCount += (_uint)pMonsterList->size();


		if (!m_bIsOnIndicated)
		{
			if (NowMonsterCount > 0)
			{
				m_bIsOnIndicated = true;
				m_pDissolveCom->Set_DissolveOn(true, 1.f);
			}
		}
		else if (!m_bTurningOff)
		{
			if (NowMonsterCount == 0)
			{
				m_bTurningOff = true;
				m_fTurnOffTime = 0;
				m_pDissolveCom->Set_DissolveOn(false, 1.f);
			}
		}

	}

	return S_OK;
}

HRESULT CPlayerIndicator::Update_Moving(_double fDeltaTime)
{
	m_fRotAngle += (_float)fDeltaTime * 90.f;
	m_fForfloatYPassedTime += (_float)fDeltaTime;
	if (m_fForfloatYPassedTime > 1.5f) m_fForfloatYPassedTime = 0;
	_float fPivotY = g_pGameInstance->Easing_Return(TYPE_SinInOut, TYPE_SinInOut, 2, 2.5f, m_fForfloatYPassedTime, 1.5f);

	m_vColor = m_pRendererCom->Get_vOcdMaskColor();
	m_vColor.w = 0.f;
	_float fLength = pow(m_vColor.Get_Lenth() + 1.f, 2.f);
	m_vColor.w = 1.f;
	Set_LimLight_N_Emissive(m_vColor, _float4(1.f, (1.f / fLength), (1.f / fLength), 0.f));

	_Vector vPlayerHipPos = m_tPlayerHipBoneDesc.Get_AttachedBoneWorldPosition_BlenderFixed();

	m_pTransformCom->Set_Matrix(XMMatrixIdentity());
	m_pTransformCom->Scaled_All(_float3(m_vScale));
	m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, vPlayerHipPos + XMVectorSet(0, fPivotY, 0, 0));
	m_pTransformCom->LookAt(m_pTransformCom->Get_MatrixState(CTransform::STATE_POS) + +XMVectorSet(0.000001f, 1.f, 0, 0));

	m_pTransformCom->Turn_CW(m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK), XMConvertToRadians(m_fRotAngle));


	return S_OK;
}

CPlayerIndicator * CPlayerIndicator::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CPlayerIndicator*	pInstance = NEW CPlayerIndicator(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CPlayerIndicator");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CPlayerIndicator::Clone(void * pArg)
{
	CPlayerIndicator*	pInstance = NEW CPlayerIndicator(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CPlayerIndicator");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPlayerIndicator::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pModel);
	Safe_Release(m_pDissolveCom);
}
