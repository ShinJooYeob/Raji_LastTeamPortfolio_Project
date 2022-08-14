#include "stdafx.h"
#include "..\public\RadialBlurUI.h"



CRadialBlurUI::CRadialBlurUI(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CRadialBlurUI::CRadialBlurUI(const CRadialBlurUI & rhs)
	: CGameObject(rhs)
{

}

HRESULT CRadialBlurUI::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CRadialBlurUI::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components());
	FAILED_CHECK(SetUp_UI_DefaultSetting());

	m_ZoomSize = 1.f;
	m_fStartZoomPower = 0.f;

	return S_OK;
}

_int CRadialBlurUI::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)
		return -1;

	if (m_PassedTime < m_TotalTime)
	{
		if (m_bIsOn)
		{
			m_PassedTime += (_float)fDeltaTime;

			m_ZoomSize = g_pGameInstance->Easing(TYPE_Linear, m_fStartRadialSize, m_fZoomRadialSize,(_float)m_PassedTime, m_TotalTime);
			m_ZoomPower = g_pGameInstance->Easing(TYPE_Linear, m_fStartZoomPower, m_fTargetZoomPower,(_float)m_PassedTime, m_TotalTime);

		}
		else
		{
			m_PassedTime += (_float)fDeltaTime;
			m_ZoomSize = g_pGameInstance->Easing(TYPE_Linear, m_fStartRadialSize, m_fZoomRadialSize, (_float)m_PassedTime, m_TotalTime);
			m_ZoomPower = g_pGameInstance->Easing(TYPE_Linear, m_fStartZoomPower, m_fTargetZoomPower, (_float)m_PassedTime, m_TotalTime);

			if (m_PassedTime >= m_TotalTime)
			{
				m_ZoomSize = 1.f;
				m_ZoomPower = 0.f;
				m_bNeedToDraw = false;
			}
		}

	}


	return _int();
}

_int CRadialBlurUI::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)
		return -1;


	if (m_bNeedToDraw)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SCREENEFFECT, this);


	return _int();
}

_int CRadialBlurUI::Render()
{
	//if (__super::Render() < 0)
	//	return -1;

	CGameInstance* pInstance = g_pGameInstance;

	NULL_CHECK_RETURN(m_pVIBufferCom, E_FAIL);

	FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));

	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &XMMatrixIdentity(), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4)));

	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_vCenter", &m_TargetPos, sizeof(_float2)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_fZoomPower", &m_ZoomPower, sizeof(_float)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_fZoomSize", &m_ZoomSize, sizeof(_float)));


	FAILED_CHECK(pInstance->Begin_MTV(L"MRT_Defferred"));
	FAILED_CHECK(m_pShaderCom->Set_Texture("g_SourTexture", pInstance->Get_SRV(L"Target_ReferenceDefferred")));
	FAILED_CHECK(m_pVIBufferCom->Render(m_pShaderCom, 13));
	FAILED_CHECK(pInstance->End_MTV(L"MRT_Defferred"));

	FAILED_CHECK(m_pRendererCom->Copy_DeferredToReference());

	return _int();
}

_int CRadialBlurUI::LateRender()
{
	return _int();
}

void CRadialBlurUI::Set_TargetPos_ByWorldPos(_float3 vPos)
{
	CGameInstance* pInstace = g_pGameInstance;
	_float3 ProjSpacePos = XMVector3TransformCoord(vPos.Multiply_Matrix_AsPosVector(pInstace->Get_Transform_Matrix(PLM_VIEW)), pInstace->Get_Transform_Matrix(PLM_PROJ));
	m_TargetPos = _float2(ProjSpacePos.x * 0.5f + 0.5f, ProjSpacePos.y * -0.5f + 0.5f);
}

void CRadialBlurUI::Set_IsRadialIn(_bool bBool,_float fZoomRadialSize , _float fZoomPower,_float TargetTimer)
{
	m_bIsOn = bBool;
	m_bNeedToDraw = true;
	m_PassedTime = 0;
	m_TotalTime = TargetTimer;


	m_fStartRadialSize = m_ZoomSize;
	m_fStartZoomPower = m_ZoomPower;

	if (m_bIsOn)
	{
		m_fTargetZoomPower = fZoomPower;
		m_fZoomRadialSize = fZoomRadialSize;
	}
	else
	{
		m_fTargetZoomPower = 0;
		m_fZoomRadialSize = 1.f;
	}
}

HRESULT CRadialBlurUI::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VT), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_VIBuffer_Rect), TAG_COM(Com_VIBuffer), (CComponent**)&m_pVIBufferCom));


	CTransform::TRANSFORMDESC tDesc = {};

	tDesc.fMovePerSec = 5;
	tDesc.fRotationPerSec = XMConvertToRadians(60);
	tDesc.fScalingPerSec = 1;
	tDesc.vPivot = _float3(0, 0, 0);

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));


	return S_OK;
}

HRESULT CRadialBlurUI::SetUp_UI_DefaultSetting()
{

	{
		m_UIRect.left = 0;
		m_UIRect.top = 0;
		m_UIRect.right = g_iWinCX;
		m_UIRect.bottom = g_iWinCY;

		NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);

		m_pTransformCom->Set_Matrix(XMMatrixIdentity());
		m_pTransformCom->Scaled(CTransform::STATE_RIGHT, m_UIRect.right - m_UIRect.left);
		m_pTransformCom->Scaled(CTransform::STATE_UP, m_UIRect.bottom - m_UIRect.top);

		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS,
			_float3((m_UIRect.right + m_UIRect.left) * 0.5f - g_iWinCX * 0.5f,
				-(m_UIRect.bottom + m_UIRect.top) * 0.5f + g_iWinCY * 0.5f, 0.f));
		//if (m_fAngle != 0)
		//	m_pTransformCom->Rotation_CCW(XMVectorSet(0, 0, 1, 0), XMConvertToRadians(m_fAngle));
		//if (m_fYTurnAngle)
		//	m_pTransformCom->Turn_CW(m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_UP).Get_Nomalize(), XMConvertToRadians(m_fYTurnAngle));

	}


	m_fRenderSortValue = -FLT_MAX;

	m_bNeedToDraw = false;

	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH(_int(g_iWinCX), _int(g_iWinCY), -300.f, 300.f)));

	return S_OK;
}

CRadialBlurUI * CRadialBlurUI::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CRadialBlurUI*	pInstance = new CRadialBlurUI(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CRadialBlurUI");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CRadialBlurUI::Clone(void * pArg)
{
	CRadialBlurUI*	pInstance = new CRadialBlurUI(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CRadialBlurUI");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CRadialBlurUI::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	

}
