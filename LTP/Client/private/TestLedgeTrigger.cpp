#include "stdafx.h"
#include "..\public\TestLedgeTrigger.h"
#include "Player.h"

CTestLedgeTrigger::CTestLedgeTrigger(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CTriggerObject(pDevice, pDeviceContext)
{
}

CTestLedgeTrigger::CTestLedgeTrigger(const CTestLedgeTrigger & rhs)
	: CTriggerObject(rhs)
{
}

HRESULT CTestLedgeTrigger::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CTestLedgeTrigger::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(nullptr));

	FAILED_CHECK(SetUp_Components());

	FAILED_CHECK(SetUp_EtcInfo());

	//FAILED_CHECK(SetUp_PlayerAndCameraInfo());

	//LEDGETRIGGERDESC			tLedgeTriggerDesc;
	//memcpy(&tLedgeTriggerDesc, pArg, sizeof(LEDGETRIGGERDESC));

	//m_pTransformCom->Set_MatrixState(CTransform::TransformState::STATE_POS, tLedgeTriggerDesc.fSpawnPos);
	//m_eLedgeTriggerType = tLedgeTriggerDesc.eLedgeTriggerState;
	return S_OK;
}

_int CTestLedgeTrigger::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0) return -1;

	//if (true == Check_CollisionToPlayer())
	//{
	//	//Active_Trigger(fDeltaTime);
	//}
	//else
	//{
	//	//DeActive_Trigger(fDeltaTime);
	//}

	return _int();
}

_int CTestLedgeTrigger::LateUpdate(_double fDeltaTimer)
{
	if (__super::LateUpdate(fDeltaTimer) < 0)return -1;

	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_BLEND, this));

	return _int();
}

_int CTestLedgeTrigger::Render()
{
	if (__super::Render() < 0)		return -1;

	//CGameInstance* pInstance = GetSingle(CGameInstance);
	//NULL_CHECK_RETURN(m_pVIBufferCom, E_FAIL);

	//FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));

	//FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	//FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));

	//if (FAILED(m_pTextureCom->Bind_OnShader(m_pShaderCom, "g_DiffuseTexture", 0)))
	//	return E_FAIL;

	//m_pVIBufferCom->Render(m_pShaderCom, 3);


	return _int();
}

_int CTestLedgeTrigger::LateRender()
{
	return _int();
}

_int CTestLedgeTrigger::Active_Trigger(CGameObject* pTarget, _double fDeltaTime)
{
	switch (m_eLedgeTriggerType)
	{
	case ELedgeTriggerState::STATE_START:
		Ledge_Start();
		break;
	case ELedgeTriggerState::STATE_LEDGE:
		Ledge_Hanging();
		break;
	case ELedgeTriggerState::STATE_LAST_LEDGE:
		Ledge_LastHanging();
		break;
	case ELedgeTriggerState::STATE_END:
		Ledge_End();
		break;
	}

	return _int();
}

_int CTestLedgeTrigger::DeActive_Trigger(CGameObject* pTarget, _double fDeltaTime)
{
	switch (m_eLedgeTriggerType)
	{
	case ELedgeTriggerState::STATE_START:
		if (true == m_bOnTriggered)
		{
			m_bOnTriggered = false;
			m_pPlayer->Set_CurParkourTrigger(nullptr, this);
		}
		break;
	case ELedgeTriggerState::STATE_LEDGE:
		if (true == m_bOnTriggered)
		{
			m_bOnTriggered = false;
			//m_pPlayer->Set_CurParkourTrigger(nullptr, this);
		}
		break;
	case ELedgeTriggerState::STATE_LAST_LEDGE:
		if (true == m_bOnTriggered)
		{
			m_bOnTriggered = false;
		}
		break;
	case ELedgeTriggerState::STATE_END:
	
		break;
	}

	return _int();
}

CTestLedgeTrigger::EParkourTriggerType  CTestLedgeTrigger::Get_ParkourTriggerType()
{
	return EParkourTriggerType::PACUR_LEDGE;
}

CTestLedgeTrigger::ELedgeTriggerState CTestLedgeTrigger::Get_LedgeType()
{
	return m_eLedgeTriggerType;
}

_bool CTestLedgeTrigger::Is_Cornor()
{
	return m_bCornor;
}

CTestLedgeTrigger::ELedgeCornorType CTestLedgeTrigger::Get_CornorType()
{
	return m_eConorType;
}

void CTestLedgeTrigger::Ledge_Start()
{
	m_pPlayer->Set_CurParkourTrigger(this, this);
	m_bOnTriggered = true;
}

void CTestLedgeTrigger::Ledge_Hanging()
{
	if (false == m_bOnTriggered)
	{
		m_pPlayer->Set_CurParkourTrigger(this, this);
		m_bOnTriggered = true;
	}
}

void CTestLedgeTrigger::Ledge_LastHanging()
{
	if (false == m_bOnTriggered)
	{
		m_pPlayer->Set_CurParkourTrigger(this, this);
		m_bOnTriggered = true;
	}
}

void CTestLedgeTrigger::Ledge_End()
{
}

_bool CTestLedgeTrigger::Check_CollisionToPlayer()
{
	_Vector vPlayerPos = m_pPlayerTransform->Get_MatrixState(CTransform::TransformState::STATE_POS);
	_Vector vTriggerPos = m_pTransformCom->Get_MatrixState(CTransform::TransformState::STATE_POS);

	_float fDist = XMVectorGetX(XMVector3Length(vTriggerPos - vPlayerPos));
	if (fDist <= 1.f)
	{
		return true;
	}
	//
	return false;

	return _bool();
}

HRESULT CTestLedgeTrigger::SetUp_Components()
{
	//// Main Transform
	//CTransform::TRANSFORMDESC tDesc = {};
	//tDesc.fMovePerSec = 5;
	//tDesc.fRotationPerSec = XMConvertToRadians(60);
	//tDesc.fScalingPerSec = 1;
	//tDesc.vPivot = _float3(0, 0, 0);
	//FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));

	// For Debug
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_VIBuffer_Rect), TAG_COM(Com_VIBuffer), (CComponent**)&m_pVIBufferCom));
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Texture_ShellingPoint), TAG_COM(Com_Texture), (CComponent**)&m_pTextureCom));
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VT), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));
	//

	return S_OK;
}

HRESULT CTestLedgeTrigger::SetUp_PlayerAndCameraInfo()
{
	//m_pPlayer = (CPlayer*)(g_pGameInstance->Get_GameObject_By_LayerIndex(m_eNowSceneNum, TAG_LAY(Layer_Player)));
	//m_pPlayerTransform = static_cast<CTransform*>(m_pPlayer->Get_Component(TAG_COM(Com_Transform)));

	return S_OK;
}

HRESULT CTestLedgeTrigger::SetUp_EtcInfo()
{
	return S_OK;
}

CTestLedgeTrigger * CTestLedgeTrigger::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CTestLedgeTrigger*	pInstance = NEW CTestLedgeTrigger(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CTestLedgeTrigger");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CTestLedgeTrigger::Clone(void * pArg)
{
	CTestLedgeTrigger*	pInstance = NEW CTestLedgeTrigger(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CTestLedgeTrigger");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTestLedgeTrigger::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
}
