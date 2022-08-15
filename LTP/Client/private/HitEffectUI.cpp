#include "stdafx.h"
#include "..\public\HitEffectUI.h"



CHitEffectUI::CHitEffectUI(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CHitEffectUI::CHitEffectUI(const CHitEffectUI & rhs)
	: CGameObject(rhs)
{

}

HRESULT CHitEffectUI::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CHitEffectUI::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components());
	FAILED_CHECK(SetUp_UI_DefaultSetting());


	return S_OK;
}

_int CHitEffectUI::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)
		return -1;

	if (!m_bNeedToDraw) return 0;

	m_PassedTime += (_float)fDeltaTime;

	_float EasedAlpha = g_pGameInstance->Easing_Return(TYPE_SinInOut, TYPE_SinInOut, 0, m_vTargetColor.w, (_float)m_PassedTime, (_float)m_TotalTime);

	if (m_PassedTime > m_TotalTime)
	{
		m_bNeedToDraw = false;
		EasedAlpha = 0;
	}

	m_vColor.w = EasedAlpha;


	return _int();
}

_int CHitEffectUI::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)
		return -1;

	if (!m_bNeedToDraw) return 0;

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SCREENEFFECT, this);


	return _int();
}

_int CHitEffectUI::Render()
{

	if (!m_bNeedToDraw) return 0;

	//if (__super::Render() < 0)
	//	return -1;

	CGameInstance* pInstance = g_pGameInstance;

	NULL_CHECK_RETURN(m_pVIBufferCom, E_FAIL);

	FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));

	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &XMMatrixIdentity(), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4)));

	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_vColor", &m_vColor, sizeof(_float4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_fAlphaTestValue", &m_fAlphaTestValue, sizeof(_float)));
	FAILED_CHECK(m_pTextureCom->Bind_OnShader(m_pShaderCom, "g_DiffuseTexture", m_iTextureLayerIndex));

	FAILED_CHECK(pInstance->Begin_MTV(L"MRT_Defferred"));

	FAILED_CHECK(m_pVIBufferCom->Render(m_pShaderCom, 14));
	FAILED_CHECK(pInstance->End_MTV(L"MRT_Defferred"));

	FAILED_CHECK(m_pRendererCom->Copy_DeferredToReference());

	return _int();
}

_int CHitEffectUI::LateRender()
{
	return _int();
}

void CHitEffectUI::Set_HitEffect(_float Intensive, _float EffectTime,_float3 vColor )
{
	m_TotalTime = EffectTime;
	m_PassedTime = 0;
	m_bNeedToDraw = true;
	m_iTextureLayerIndex = rand() % 7 + 1;
	m_vColor = _float4(vColor, 0);
	m_vTargetColor.w = Intensive;
}

HRESULT CHitEffectUI::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VT), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_VIBuffer_Rect), TAG_COM(Com_VIBuffer), (CComponent**)&m_pVIBufferCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Texture_ScreenEffectUI), TAG_COM(Com_Texture), (CComponent**)&m_pTextureCom));
	FAILED_CHECK(m_pTextureCom->Change_TextureLayer(L"HitEffect"));

	CTransform::TRANSFORMDESC tDesc = {};

	tDesc.fMovePerSec = 5;
	tDesc.fRotationPerSec = XMConvertToRadians(60);
	tDesc.fScalingPerSec = 1;
	tDesc.vPivot = _float3(0, 0, 0);

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));


	return S_OK;
}

HRESULT CHitEffectUI::SetUp_UI_DefaultSetting()
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


	m_fRenderSortValue = -FLT_MAX + 1;

	m_bNeedToDraw = false;

	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH(_int(g_iWinCX), _int(g_iWinCY), -300.f, 300.f)));
	m_vColor = _float4(1, 0, 0, 0);

	return S_OK;
}

CHitEffectUI * CHitEffectUI::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CHitEffectUI*	pInstance = new CHitEffectUI(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CHitEffectUI");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CHitEffectUI::Clone(void * pArg)
{
	CHitEffectUI*	pInstance = new CHitEffectUI(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CHitEffectUI");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CHitEffectUI::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	

}
