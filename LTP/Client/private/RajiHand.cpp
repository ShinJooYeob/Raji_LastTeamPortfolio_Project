#include "stdafx.h"
#include "..\public\RajiHand.h"
#include "Player.h"
#include "RepelWall.h"
#include "Scene_Laboratory_Jino.h"
#include "RajiMask.h"
#include "Camera_Main.h"
#include "Rajibalsura.h"

CRajiHand::CRajiHand(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CRajiHand::CRajiHand(const CRajiHand & rhs)
	: CGameObject(rhs)
{
}

HRESULT CRajiHand::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CRajiHand::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components());
	FAILED_CHECK(SetUp_Etc());

	return S_OK;
}

_int CRajiHand::Update(_double fDeltaTime)
{
	m_pTransformCom->Scaled_All(_float3(0.03f));
	m_pTransformCom->Rotation_CW(m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK), XMConvertToRadians(0.f));

	if (false == m_pDissolveCom->Get_IsFadeIn() && 1.f <= m_pDissolveCom->Get_DissolvingRate())
		return 0;
	 
	if (__super::Update(fDeltaTime) < 0) return -1;

	switch (m_iCurCutSceneState)
	{
	case 0:
		break;
	case 1:
	{
		_float fAnimPlayRate = (_float) m_pModel->Get_PlayRate();

		if (false == m_bOnceSwitch && 0.3f <= fAnimPlayRate && 0.706f > fAnimPlayRate)
		{
			CRajiMask* pRajiMask = static_cast<CRajiMask*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, Tag_Layer(Layer_RoseObj)));
			ATTACHEDESC tAttachDesc;
			tAttachDesc.Initialize_AttachedDesc(this, "skd_r_palm", _float3(1), _float3(0), _float3(-63.f, -0.554f, -115.f));
			pRajiMask->Set_AttachDesc(tAttachDesc);
			m_bOnceSwitch = true;
		}
		else if (0.48f >= fAnimPlayRate && 0.3f <= fAnimPlayRate)
		{
			GetSingle(CUtilityMgr)->Get_MainCamera()->Get_CamTransformCom()->Set_MoveSpeed(0.5f);
			GetSingle(CUtilityMgr)->Get_MainCamera()->Get_CamTransformCom()->MovetoTarget_ErrRange(XMVectorSet(101.f, 36.f, 326.7f, 1.f), fDeltaTime, 0.05f);
		}
		else if (0.266f >= fAnimPlayRate)
		{
			GetSingle(CUtilityMgr)->Get_MainCamera()->Get_CamTransformCom()->Set_MoveSpeed(1.f);
			GetSingle(CUtilityMgr)->Get_MainCamera()->Get_CamTransformCom()->MovetoTarget_ErrRange(XMVectorSet(101.f, 35.46f, 326.7f, 1.f), fDeltaTime, 0.05f);
		}
		else if (0.706f <= fAnimPlayRate && 0.806f >= fAnimPlayRate)
		{
			if (true == m_bOnceSwitch)
			{
				CRajiMask* pRajiMask = static_cast<CRajiMask*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, Tag_Layer(Layer_RoseObj)));
				pRajiMask->Set_Appear(false);
				m_bOnceSwitch = false;
			}
			m_fAnimSpeed = 0.7f;
		}
		else
		{
			m_fAnimSpeed = 1.f;
		}
		_Vector vLook = XMVector3Normalize(GetSingle(CUtilityMgr)->Get_MainCamera()->Get_CamTransformCom()->Get_MatrixState(CTransform::STATE_LOOK));
		_Vector vRight = XMVector3Normalize(GetSingle(CUtilityMgr)->Get_MainCamera()->Get_CamTransformCom()->Get_MatrixState(CTransform::STATE_RIGHT));
		_Vector vUp = XMVector3Normalize(GetSingle(CUtilityMgr)->Get_MainCamera()->Get_CamTransformCom()->Get_MatrixState(CTransform::STATE_UP));
		_Vector vPos = GetSingle(CUtilityMgr)->Get_MainCamera()->Get_Camera_Transform()->Get_MatrixState(CTransform::STATE_POS);

		vPos = vPos + vRight * 0.6f - vUp * 0.1f;
		m_pTransformCom->LookDir(vLook);
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, vPos);

		FAILED_CHECK(m_pModel->Update_AnimationClip(fDeltaTime * m_fAnimSpeed, m_bIsOnScreen));

		if (0.98f <= fAnimPlayRate)
		{
			GetSingle(CUtilityMgr)->Get_MainCamera()->Set_EndingCutSceneState(4);
			g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, Tag_Layer(Layer_RoseObj), TAG_OP(Prototype_Object_Rajibalsura));
			Set_IsDead();
		}
	}
		break;
	}

	FAILED_CHECK(m_pDissolveCom->Update_Dissolving(fDeltaTime));
	return _int();
}

_int CRajiHand::LateUpdate(_double fDeltaTimer)
{
	if (false == m_pDissolveCom->Get_IsFadeIn() && 1.f <= m_pDissolveCom->Get_DissolvingRate())
		return 0;

	if (__super::LateUpdate(fDeltaTimer) < 0) return -1;

	if (0 < m_iCurCutSceneState)
	{
		FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
		FAILED_CHECK(m_pRendererCom->Add_ShadowGroup(CRenderer::SHADOW_ANIMMODEL, this, m_pTransformCom, m_pShaderCom, m_pModel, nullptr, m_pDissolveCom));
	}
	
	return _int();
}

_int CRajiHand::Render()
{
	if (__super::Render() < 0)		return -1;

	NULL_CHECK_RETURN(m_pModel, E_FAIL);

	CGameInstance* pInstance = GetSingle(CGameInstance);

	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));
	FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));

	FAILED_CHECK(m_pDissolveCom->Render(13)); 
	return _int();
}

_int CRajiHand::LateRender()
{
	return _int();
}

void CRajiHand::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{

}

void CRajiHand::Set_Appear(_bool bAppear)
{
	if (true == bAppear && true == m_bDisappear)
	{
		m_pDissolveCom->Set_DissolveOn(true, 0.5f);		// Appear
		m_bDisappear = false;
	}
	else if(false == bAppear && false == m_bDisappear)
	{
		m_pDissolveCom->Set_DissolveOn(false, 0.5f);	// Disappear
		m_bDisappear = true;
	}
}

void CRajiHand::Set_CutSceneState(_uint iState)
{
	m_iCurCutSceneState = iState;
}

HRESULT CRajiHand::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Mesh_RajiArm), TAG_COM(Com_Model), (CComponent**)&m_pModel));

	CTransform::TRANSFORMDESC tDesc = {};
	tDesc.fMovePerSec = 0.2f;
	tDesc.fRotationPerSec = XMConvertToRadians(360);
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

HRESULT CRajiHand::SetUp_Etc()
{
	m_pTransformCom->Scaled_All(_float3(0.05f, 0.05f, 0.05f));
	m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, _float3(101.f, 34.37f, 325.2f));

	m_pDissolveCom->Set_DissolveOn(true, 0.f);

	CGameObject* pPlayer = g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, Tag_Layer(Layer_Player));
	
	Set_IsOcllusion(false);

	return S_OK;
}

CRajiHand * CRajiHand::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CRajiHand*	pInstance = NEW CRajiHand(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CRajiHand");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CRajiHand::Clone(void * pArg)
{
	CRajiHand*	pInstance = NEW CRajiHand(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CRajiHand");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRajiHand::Free()
{
	__super::Free();
	 
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pDissolveCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModel);
}
