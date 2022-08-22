#include "stdafx.h"
#include "..\public\Rajibalsura.h"
#include "Camera_Main.h"
#include "Scene_Loading.h"
#include "Scene_Stage5.h"

CRajibalsura::CRajibalsura(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CRajibalsura::CRajibalsura(const CRajibalsura & rhs)
	: CGameObject(rhs)
{
}

HRESULT CRajibalsura::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CRajibalsura::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components());

	FAILED_CHECK(SetUp_EtcInfo());

	return S_OK;
}

_int CRajibalsura::Update(_double fDeltaTime) 
{
	if (__super::Update(fDeltaTime) < 0) return -1; 

	switch (m_iCurState)
	{
	case 0:
	{
	}
		break;
	case 1:
	{
		m_fDelayTime -= (_float)fDeltaTime;
		_float3 fPos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
		fPos.y += 10.f;
		fPos.z -= 20.f;
		m_fAttachCamPos = fPos;
		if (0.f >= m_fDelayTime)
		{
			m_iCurState = 2;
		}
	}
		break;
	case 2:
	{
		static_cast<CScene_Stage5*>(g_pGameInstance->Get_NowScene())->Start_EndingCinematic();
		m_iCurState = 3;
		m_fDelayTime = 3.f;
	}
		break;
	case 3:
	{
		m_fDelayTime -= (_float)fDeltaTime;
		if (0.f >= m_fDelayTime)
		{
			_float3 fPos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
			fPos.y += 10.f;
			fPos.z -= 20.f;
			m_fAttachCamPos = fPos;
			m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, _float3(103.f, 68.56f, 386.f));

			g_pGameInstance->Add_GameObject_To_Layer(SCENEID::SCENE_STAGE5, TAG_LAY(Layer_SteamPad), TAG_OP(Prototype_Object_EndingPortal), nullptr);

			m_iCurState = 4;
		}
	}
		break;
	case 4:
	{
		_float3 fPos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
		fPos.y += 10.f;
		fPos.z -= 20.f;
		m_fAttachCamPos = fPos;
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, _float3(103.f, 68.56f, 386.f));
		m_fDelayTime = 8.f;
	}
		break;
	case 5:
	{
		m_fDelayTime -= (_float)fDeltaTime;
		if (0.f >= m_fDelayTime)
		{
			m_fDelayTime = 5.f;
			m_iCurState = 6;
			m_pTransformCom->Set_MoveSpeed(2.f);
			GetSingle(CUtilityMgr)->Get_MainCamera()->Set_CameraMode(CAM_MODE_FIX);
			g_pGameInstance->PlaySoundW(L"Jino_MrM_Naration_3.wav", CHANNEL_MONSTER, 1.f);
			m_pDissolveCom->Set_DissolveOn(false, 5.f);
		}

		_float3 fPos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
		fPos.y += 3.f;
		fPos.z -= 2.f;
		m_fAttachCamPos = fPos;
	}
		break;
	case 6:
	{
		m_fDelayTime -= (_float)fDeltaTime;
		if (0.f >= m_fDelayTime)
		{
			static_cast<CScene_Stage5*>(g_pGameInstance->Get_NowScene())->Set_EndGame();
		}

		m_pTransformCom->MovetoDir(XMVectorSet(0.f, 0.f, 1.f, 0.f), fDeltaTime);
	}
		break;
	}

	FAILED_CHECK(m_pModel->Update_AnimationClip(fDeltaTime * m_fAnimSpeed, m_bIsOnScreen));
	m_pDissolveCom->Update_Dissolving(fDeltaTime);

	return _int();
}

_int CRajibalsura::LateUpdate(_double fDeltaTimer)
{
	if (__super::LateUpdate(fDeltaTimer) < 0)return -1;

	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
	FAILED_CHECK(m_pRendererCom->Add_ShadowGroup(CRenderer::SHADOW_ANIMMODEL, this, m_pTransformCom, m_pShaderCom, m_pModel, nullptr, m_pDissolveCom));

	return _int();
}

_int CRajibalsura::Render()
{
	if (__super::Render() < 0)		return -1;

	NULL_CHECK_RETURN(m_pModel, E_FAIL);

	CGameInstance* pInstance = GetSingle(CGameInstance);
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));
	FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));

	FAILED_CHECK(m_pDissolveCom->Render(3));

	return _int();
}

_int CRajibalsura::LateRender()
{
	return _int();
}

void CRajibalsura::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{

}

_float CRajibalsura::Take_Damage(CGameObject * pTargetObject, _float fDamageAmount, _fVector vDamageDir, _bool bKnockback, _float fKnockbackPower)
{
	return _float();
}

_float CRajibalsura::Apply_Damage(CGameObject * pTargetObject, _float fDamageAmount, _bool bKnockback)
{
	return _float();
}

void CRajibalsura::Set_CurState(_uint iState)
{
	switch (iState)
	{
	case 0:
		m_fAnimSpeed = 0.f;
		break;
	case 1:
		m_fAnimSpeed = 0.5f;
		GetSingle(CUtilityMgr)->Get_MainCamera()->Set_CameraLookWeight(0.999f);
		GetSingle(CUtilityMgr)->Get_MainCamera()->Set_CameraMoveWeight(0.99f);

		GetSingle(CUtilityMgr)->Get_MainCamera()->Set_CameraMode(CAM_MODE_NOMAL);
		GetSingle(CUtilityMgr)->Get_MainCamera()->Set_FocusTarget(this);
		GetSingle(CUtilityMgr)->Get_MainCamera()->Lock_CamLook(true, XMVectorSet(0.f, -0.2f, 1.f, 0.f));
		
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, _float3(101.f, 34.f, 326.f));

		// Fade inout time
		m_fDelayTime = 8.f;
		break;
	}
	m_iCurState = iState;
}

HRESULT CRajibalsura::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));


	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));


	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_Boss_Mahabalasura), TAG_COM(Com_Model), (CComponent**)&m_pModel));


	FAILED_CHECK(m_pModel->Change_AnimIndex(0));


	CTransform::TRANSFORMDESC tDesc = {};
	tDesc.fMovePerSec = 0.8f;
	tDesc.fRotationPerSec = XMConvertToRadians(60);
	tDesc.fScalingPerSec = 1;
	tDesc.vPivot = _float3(0, 0, 0);
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));


	CDissolve::DISSOLVEDESC	tDissolveDesc;
	tDissolveDesc.eDissolveModelType = CDissolve::DISSOLVE_ANIM;
	tDissolveDesc.pModel = m_pModel;
	tDissolveDesc.pShader = m_pShaderCom;
	tDissolveDesc.RampTextureIndex = 1;
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Dissolve), TAG_COM(Com_Dissolve), (CComponent**)&m_pDissolveCom, &tDissolveDesc));

	return S_OK;
}

HRESULT CRajibalsura::SetUp_EtcInfo()
{
	m_pTransformCom->Rotation_CW(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(170.f));

	Set_IsOcllusion(true);

	Set_LimLight_N_Emissive(_float4(0, 0, 0, 0), _float4(0.5f, 0, 0, 0));

	m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, _float3(101.f, 33.56f, 326.f));
	m_pTransformCom->LookDir(XMVectorSet(0.f, 0.f, 1.f, 0.f));

	return S_OK;
}

CRajibalsura * CRajibalsura::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CRajibalsura*	pInstance = NEW CRajibalsura(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CRajibalsura");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CRajibalsura::Clone(void * pArg)
{
	CRajibalsura*	pInstance = NEW CRajibalsura(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CRajibalsura");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CRajibalsura::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModel);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pDissolveCom);
}
