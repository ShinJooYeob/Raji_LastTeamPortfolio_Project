#include "stdafx.h"
#include "..\public\EndingPortal.h"
#include "Player.h"
#include "Rajibalsura.h"
#include "Camera_Main.h"

CEndingPortal::CEndingPortal(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CEndingPortal::CEndingPortal(const CEndingPortal & rhs)
	: CGameObject(rhs)
{
}

HRESULT CEndingPortal::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CEndingPortal::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components());
	FAILED_CHECK(SetUp_Etc());

	return S_OK;
}

_int CEndingPortal::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0) return -1;

	switch (m_iCurState)
	{
	case 0:
	{
		m_fDelayTime -= (_float)fDeltaTime;
		if (0.f >= m_fDelayTime)
		{
			m_bActive = true;
		}

		if (true == m_bActive)
		{
			if (false == m_bOncePlay)
			{
				GetSingle(CUtilityMgr)->Get_MainCamera()->Start_CameraShaking_Fov(56.f, 2.f, 0.2f, false);
				g_pGameInstance->PlaySoundW(L"Jino_MrM_Teleport_01.wav", CHANNELID::CHANNEL_MONSTER, 1.f);
				m_bOncePlay = true;
			}

			m_pTransformCom->Scaling(CTransform::STATE_RIGHT, fDeltaTime);
			_float fScale = XMVectorGetX(m_pTransformCom->Get_MatrixScale(CTransform::STATE_RIGHT));
			if (fScale >= 10.f)
			{
				m_pTransformCom->Scaled(CTransform::STATE_RIGHT, 10.f);
				m_bActive = false;
				m_iCurState = 1;
				m_fDelayTime = 3.f;
			}
		}
	}
		break;
	case 1:
	{
		m_fDelayTime -= (_float)fDeltaTime;
		if (0.f >= m_fDelayTime)
		{
			CRajibalsura* pRajibalsura = static_cast<CRajibalsura*>(g_pGameInstance->Get_GameObject_By_LayerLastIndex(m_eNowSceneNum, Tag_Layer(Layer_RoseObj)));
			pRajibalsura->Set_CurState(5);
			m_iCurState = 2;
		}
	}
		break;
	}

	return _int();
}

_int CEndingPortal::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)return -1;

	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_BLEND, this));

	return _int();
}

_int CEndingPortal::Render()
{
	if (__super::Render() < 0)
		return -1;

	NULL_CHECK_RETURN(m_pVIBufferCom, E_FAIL);
	FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));
	CGameInstance* pInstance = GetSingle(CGameInstance);
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));
	if (FAILED(m_pTextureCom->Bind_OnShader(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;

	m_pVIBufferCom->Render(m_pShaderCom, 17);

	return _int();
}

_int CEndingPortal::LateRender()
{
	return _int();
}

void CEndingPortal::Set_Active(_bool bActive)
{
	m_bActive = bActive;
}

HRESULT CEndingPortal::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VT), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_VIBuffer_Rect), TAG_COM(Com_VIBuffer), (CComponent**)&m_pVIBufferCom));

	FAILED_CHECK(Add_Component(SCENE_STAGE5, TAG_CP(Prototype_Texture_EndingPortal), TAG_COM(Com_Texture), (CComponent**)&m_pTextureCom));

	CTransform::TRANSFORMDESC tDesc = {};
	tDesc.fMovePerSec = 5;
	tDesc.fRotationPerSec = XMConvertToRadians(60);
	tDesc.fScalingPerSec = 10;
	tDesc.vPivot = _float3(0, 0, 0);
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));

	return S_OK;
}

HRESULT CEndingPortal::SetUp_Etc()
{
	m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, _float3(102.86f, 70.43f, 391.19f));
	m_pTransformCom->Scaled_All(_float3(0.1f, 10.f, 10.f));

	m_fDelayTime = 15.f;

	return S_OK;
}

CEndingPortal * CEndingPortal::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CEndingPortal*	pInstance = NEW CEndingPortal(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CEndingPortal");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CEndingPortal::Clone(void * pArg)
{
	CEndingPortal*	pInstance = NEW CEndingPortal(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CEndingPortal");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEndingPortal::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
}

