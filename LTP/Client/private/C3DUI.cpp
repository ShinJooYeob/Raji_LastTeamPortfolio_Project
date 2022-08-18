#include "stdafx.h"
#include "..\public\C3DUI.h"

C3DUI::C3DUI(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

C3DUI::C3DUI(const C3DUI & rhs)
	: CGameObject(rhs)
{
}

HRESULT C3DUI::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));


	return S_OK;
}

HRESULT C3DUI::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	if (nullptr != pArg)
	{
		memcpy(&m_SettingUIDesc, pArg, sizeof(SETTING_UI));
	}

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	return S_OK;
}

_int C3DUI::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)return -1;

	m_pTransformCom->Scaled_All(XMVectorSet(2.f, 0.5f, 1.f, 0.0f));
	//m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, XMVectorSet(m_fX - (g_iWinCX * 0.5f), -m_fY + (g_iWinCY * 0.5f), 1.f, 1.f));
	m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, XMVectorSet(-3.f, -0.5f, 5.f, 1.f));
	_float3 test = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
	m_pTransformCom->Rotation_CCW(XMVectorSet(0.f, 1.f, 0.f, 0), XMConvertToRadians(40.f));

	return _int();
}

_int C3DUI::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)return -1;

	return _int();
}

_int C3DUI::Render()
{
	if (__super::Render() < 0)		return -1;

	if (!m_SettingUIDesc.bDraw)
		return S_OK;

	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix");

	CGameInstance* pInstance = GetSingle(CGameInstance);
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));

	m_pShaderCom->Set_RawValue("g_Alpha", &m_SettingUIDesc.fAlpha, sizeof(_float));

	if (m_SettingUIDesc.bSettingOtherTexture)
		SettingTexture();

	m_pVIBufferCom->Render(m_pShaderCom, m_iPassIndex);

	return _int();
}

_int C3DUI::LateRender()
{
	if (__super::LateRender() < 0)		return -1;

	return _int();
}

_tchar * C3DUI::Get_UIName()
{
	return m_SettingUIDesc.pUI_Name;
}

_int C3DUI::Get_TextureIndex()
{
	return m_SettingUIDesc.iTextureIndex;
}

_float3 C3DUI::Get_Pos()
{
	return m_SettingUIDesc.Position;
}

_float3 C3DUI::Get_Scaled()
{
	return m_SettingUIDesc.Scaled;
}

_bool C3DUI::Get_IsDraw()
{
	return m_SettingUIDesc.bDraw;
}

_float C3DUI::Get_Alpha()
{
	return m_SettingUIDesc.fAlpha;
}

_float4 C3DUI::Get_Color()
{
	return m_vColor;
}

void C3DUI::Set_UV_Y(_float fY)
{
	m_bIsCahngeUV_Y = true;

	m_fGoalUV_Y = fY;
}

void C3DUI::Set_Angle(_float3 vAxis, _float fAngle)
{
	m_pTransformCom->Rotation_CW(vAxis.XMVector(), XMConvertToRadians(fAngle));
}

HRESULT C3DUI::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom));
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VT), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_VIBuffer_Rect), TAG_COM(Com_VIBuffer), (CComponent**)&m_pVIBufferCom));
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(SCENE_STATIC, TAG_CP(Prototype_Texture_HPUI), TAG_COM(Com_HPUI), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT C3DUI::SettingTexture()
{
	CUtilityMgr* pUtil = GetSingle(CUtilityMgr);


	FAILED_CHECK(pUtil->Bind_UtilTex_OnShader(CUtilityMgr::UTILTEX_NOISE, m_pShaderCom, "g_NoiseTexture", m_SettingUIDesc.iNoiseTextureIndex));
	FAILED_CHECK(pUtil->Bind_UtilTex_OnShader(CUtilityMgr::UTILTEX_MASK, m_pShaderCom, "g_SourTexture", m_SettingUIDesc.iMaskTextureIndex));


	//m_fUV_Y = 0.2f;
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_UV_Y", &m_fUV_Y, sizeof(_float)));

	_float4 g_vColor = m_vColor;
	_float2 noisingdir = _float2(0.0f, 0.25f);
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_vColor", &g_vColor, sizeof(_float4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("noisingdir", &noisingdir, sizeof(_float2)));

	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_fTimer", &m_PassedTimer, sizeof(_float)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_fMaxTime", &(m_fTotalTargetTime), sizeof(_float)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_fPaperCurlIntensive", &(m_fApearTime), sizeof(_float)));

	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_fAppearTimer", &(m_fDisApearTime), sizeof(_float)));
	return S_OK;
}

void C3DUI::EsingUV_Y(_double fDeltaTime)
{
	CGameInstance* pGameInstacnce = GetSingle(CGameInstance);

	m_fY_EsingTime += (_float)fDeltaTime;
	m_fUV_Y = pGameInstacnce->Easing(TYPE_Linear, m_fUV_Y, m_fGoalUV_Y, m_fY_EsingTime, 1.f);

	if (m_fY_EsingTime >= 1.f)
	{
		m_bIsCahngeUV_Y = false;
		m_fY_EsingTime = 0;
	}
}

C3DUI * C3DUI::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	C3DUI*	pInstance = NEW C3DUI(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created C3DUI");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * C3DUI::Clone(void * pArg)
{
	C3DUI*	pInstance = NEW C3DUI(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created Clone C3DUI");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void C3DUI::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
}
