#include "stdafx.h"
#include "..\public\UI.h"

CUI::CUI(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CUI::CUI(const CUI & rhs)
	: CGameObject(rhs)
{
}

HRESULT CUI::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CUI::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	if (nullptr != pArg)
	{

		memcpy(&m_SettingUIDesc, pArg, sizeof(SETTING_UI));

		SetUp_UIInfo(m_SettingUIDesc);
	}

	if (FAILED(SetUp_Components()))
		return E_FAIL;

	if (m_SettingUIDesc.eUIType == UI_2D)
	{
		XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH((_float)g_iWinCX, (_float)g_iWinCY, 0.f, 1.f)));

		m_pTransformCom->Rotation_CW(XMVectorSet(0, 0, 1.f, 0), XMConvertToRadians(m_fAngle));
		m_pTransformCom->Scaled_All(XMVectorSet(m_fSizeX, m_fSizeY, 1.f, 0.0f));
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, XMVectorSet(m_fX - ((_float)g_iWinCX * 0.5f), -m_fY + ((_float)g_iWinCY * 0.5f), 0.f, 1.f));
	}
	else
	{
		//m_pTransformCom->Rotation_CW(XMVectorSet(0, 0, 1.f, 0), XMConvertToRadians(m_fAngle));
		m_pTransformCom->Rotation_CCW(XMVectorSet(0, 1.f, 0.f, 0), XMConvertToRadians(m_fAngle));
		m_pTransformCom->Scaled_All(m_SettingUIDesc.v3DUIScaled);
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, m_SettingUIDesc.v3DUIPosition);
	}

	

	m_bIsRevolutionCW = false;
	return S_OK;
}

_int CUI::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)return -1;


	m_PassedTimer += (_float)fDeltaTime;

	if (m_bIsCahngeUV_Y)
	{
		m_fY_EsingTime = 0.f;
		EsingUV_Y(fDeltaTime);
	}

	//if (m_SettingUIDesc.pUI_Name == TEXT("Skill_3_Sprout1"))
	//{
	//	//m_SettingUIDesc.v3DUIPosition = _float3(0.5f, 0.25f, 5.f);
	//	//m_SettingUIDesc.v3DUIScaled = _float3(10.f, 6.2f, 7.f);
	//	//m_SettingUIDesc.fAngle = 30.f;
	//	//m_pTransformCom->Turn_CCW(XMVectorSet(0.f, 1.f, 0.f, 0.f), fDeltaTime);
	//	m_fSizeX = 50.f;
	//	m_fSizeY = 50.f;
	//	m_fX = 962.f;
	//	m_fY = 507.f;

	//}

	if (m_bIsRevolutionCCW)
	{
		UI_RevolutionCCW();
		//m_pTransformCom->Turn_CCW(XMVectorSet(0.f, 1.f, 0.f, 0.f), fDeltaTime);
	}
	if (m_bIsRevolutionCW)
	{
		UI_RevolutionCW();
	}


	if (m_SettingUIDesc.eUIType == UI_2D)
		Update_Rect();
	else
		Update_Rect3D();

	if (m_bIsDrawTimeOn)
		TimeDraw(fDeltaTime);

	return _int();
}

_int CUI::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)return -1;

	if (m_SettingUIDesc.eUIKindsID == UIID_MINIGAME_TAIKODRUM)
	{
		FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
	}
	else if (m_SettingUIDesc.eUIKindsID == UIID_MINIGAME_TAIKO)
	{
		FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_JJBMINIGAME, this));
	}
	else
	{
		FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this));
	}


	return _int();
}

_int CUI::Render()
{
	if (__super::Render() < 0)return -1;

	 
	if (!m_bDraw)
		return S_OK;

	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix");

	if (m_SettingUIDesc.eUIType == UITYPE::UI_3D)
	{
		CGameInstance* pInstance = GetSingle(CGameInstance);
		FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
		FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));
	}
	else
	{
		m_pShaderCom->Set_RawValue("g_ViewMatrix", &XMMatrixIdentity(), sizeof(_float4x4));
		m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4));
	}

	m_pShaderCom->Set_RawValue("g_Alpha", &m_SettingUIDesc.fAlpha, sizeof(_float));


	if (m_SettingUIDesc.bSettingOtherTexture)
		SettingTexture();

	if (m_iPassIndex == 19 || m_iPassIndex == 20)
	{
		CUtilityMgr* pUtil = GetSingle(CUtilityMgr);
		FAILED_CHECK(pUtil->Bind_UtilTex_OnShader(CUtilityMgr::UTILTEX_NOISE, m_pShaderCom, "g_NoiseTexture", 52));
		FAILED_CHECK(pUtil->Bind_UtilTex_OnShader(CUtilityMgr::UTILTEX_MASK, m_pShaderCom, "g_SourTexture", 36));

		_float2 vNoiseDir = _float2(0, 0.2f);
		FAILED_CHECK(m_pShaderCom->Set_RawValue("noisingdir", &vNoiseDir, sizeof(_float2)));
		FAILED_CHECK(m_pShaderCom->Set_RawValue("g_fTimer", &m_PassedTimer, sizeof(_float)));
	}

	switch (m_SettingUIDesc.eUIKindsID)
	{
	case Client::CUI::UIID_JB:
	{

		if (FAILED(m_pTextureCom->Bind_OnShader(m_pShaderCom, "g_DiffuseTexture", m_iTextureIndex)))
		return E_FAIL;

		break;
	}
	case Client::CUI::UIID_JY_Lobby:
	{


		FAILED_CHECK(m_pTextureCom->Bind_OnShader_AutoFrame(m_pShaderCom, "g_DiffuseTexture", g_fDeltaTime * 2.5f));
	}

		break;

	case Client::CUI::UIID_MINIGAME_TAIKO:
		FAILED_CHECK(m_pTextureCom->Bind_OnShader(m_pShaderCom, "g_DiffuseTexture", m_iTextureIndex));
		break;

	case Client::CUI::UIID_MINIGAME_TAIKODRUM:
		FAILED_CHECK(m_pTextureCom->Bind_OnShader_AutoFrame(m_pShaderCom, "g_DiffuseTexture", g_fDeltaTime * 1.5f));
		break;

	case Client::CUI::UIID_EFFECT:
	{
		CUtilityMgr* pUtil = GetSingle(CUtilityMgr);

		FAILED_CHECK(pUtil->Bind_UtilTex_OnShader(CUtilityMgr::UTILTEX_NOISE, m_pShaderCom, "g_NoiseTexture", m_SettingUIDesc.iNoiseTextureIndex));
		FAILED_CHECK(pUtil->Bind_UtilTex_OnShader(CUtilityMgr::UTILTEX_MASK, m_pShaderCom, "g_SourTexture", m_SettingUIDesc.iMaskTextureIndex));
		FAILED_CHECK(pUtil->Bind_UtilTex_OnShader(CUtilityMgr::UTILTEX_NOISE, m_pShaderCom, "g_DiffuseTexture", m_SettingUIDesc.iTextureIndex));

		///FAILED_CHECK(pUtil->Bind_UtilTex_OnShader(CUtilityMgr::UTILTEX_NOISE, m_pShaderCom, "g_DiffuseTexture", 271));
		///FAILED_CHECK(pUtil->Bind_UtilTex_OnShader(CUtilityMgr::UTILTEX_NOISE, m_pShaderCom, "g_NoiseTexture", 381));
		///FAILED_CHECK(pUtil->Bind_UtilTex_OnShader(CUtilityMgr::UTILTEX_MASK, m_pShaderCom, "g_SourTexture", 96));

		_float2 vNoiseDir = _float2(0, 0.2f);
		FAILED_CHECK(m_pShaderCom->Set_RawValue("noisingdir", &vNoiseDir, sizeof(_float2)));
		FAILED_CHECK(m_pShaderCom->Set_RawValue("g_fTimer", &m_PassedTimer, sizeof(_float)));
		
	}
		break;
	case Client::CUI::UIID_END:
		break;
	default:
		break;
	}




	//if (m_pUIName == TEXT("UI_Fade"))
	//	m_pShaderCom->Set_RawValue("g_Alpha", &m_fFadeTime, sizeof(_float));

	//if (m_pUIName == TEXT("UI_Fade"))
	//	m_pVIBufferCom->Render(m_pShaderCom, 5);
	//else
	//	m_pVIBufferCom->Render(m_pShaderCom, 0);

	m_pVIBufferCom->Render(m_pShaderCom, m_iPassIndex);


	return S_OK;
}

_int CUI::LateRender()
{
	if (__super::LateRender() < 0)return -1;

	return _int();
}

void CUI::Set_Angle(_float fAngle)
{
	m_pTransformCom->Rotation_CW(XMVectorSet(0, 0, 1.f, 0), XMConvertToRadians(fAngle));
}

void CUI::Set_UV_Y(_float fY)
{
	m_bIsCahngeUV_Y = true;

	m_fGoalUV_Y = fY;
}

HRESULT CUI::Set_ChangeTextureLayer(_tchar * LayerName)
{
	FAILED_CHECK(m_pTextureCom->Change_TextureLayer(LayerName));

	return S_OK;
}

HRESULT CUI::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VT), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_VIBuffer_Rect), TAG_COM(Com_VIBuffer), (CComponent**)&m_pVIBufferCom));


	CTransform::TRANSFORMDESC tDesc = {};

	tDesc.fMovePerSec = 5;
	tDesc.fRotationPerSec = 1.f;
	tDesc.vPivot = _float3(0, 0, 0);
	tDesc.fScalingPerSec = 1;

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(SCENE_STATIC, TAG_CP(Prototype_Texture_UI), TAG_COM(Com_UI), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	switch (m_SettingUIDesc.eUIKindsID)
	{
	case Client::CUI::UIID_JB:
		FAILED_CHECK(m_pTextureCom->Change_TextureLayer(L"UI"));
		break;
	case Client::CUI::UIID_JY_Lobby:
		FAILED_CHECK(m_pTextureCom->Change_TextureLayer(L"LobbyUI"));
		break;
	case Client::CUI::UIID_MINIGAME_TAIKO:
		FAILED_CHECK(m_pTextureCom->Change_TextureLayer(L"TaikoUI"));
		break;

	case Client::CUI::UIID_MINIGAME_TAIKODRUM:
		FAILED_CHECK(m_pTextureCom->Change_TextureLayer(L"TaikoDrumUI"));
		break;

	case Client::CUI::UIID_EFFECT:
		m_iPassIndex = 18;
		break;
	case Client::CUI::UIID_END:
		break;
	default:
		break;
	}

	return S_OK;
}

void CUI::UI_RevolutionCCW()
{
	//m_fRevolutionAngle += (_float)fDeltaTime;

	m_fRevolutionStartAngle += 3.f;

	if (m_fRevolutionStartAngle > m_fRevolutionGoalAngle)
	{
		m_bIsRevolutionCCW = false;
		m_fRevolutionStartAngle = 0.f;
		m_fRevolutionGoalAngle = 0.f;
		return;
	}

	_float3 Pos = m_SettingUIDesc.v3DUIPosition;
	
	_Matrix mat = XMMatrixInverse(nullptr, g_pGameInstance->Get_Transform_Matrix(PLM_VIEW));

	_float3 vCamPos = mat.r[3];
	

	m_pTransformCom->Set_Matrix(XMMatrixScaling(m_SettingUIDesc.v3DUIScaled.x, m_SettingUIDesc.v3DUIScaled.y, m_SettingUIDesc.v3DUIScaled.z) *
		XMMatrixRotationAxis(XMVectorSet(0,1,0,0), XMConvertToRadians(m_SettingUIDesc.fAngle * -1.f))*
		XMMatrixTranslation(Pos.x, Pos.y, Pos.z) *
		XMMatrixRotationAxis(-mat.r[1], XMConvertToRadians(m_fRevolutionStartAngle)) *
		XMMatrixTranslation(vCamPos.x, vCamPos.y, vCamPos.z));

	//m_pTransformCom->Turn_Revolution_CCW(_float3(0, Pos.y, 0).XMVector(), Pos.z + 0.8f, XMConvertToRadians(3.f));
}

void CUI::UI_RevolutionCW()
{
	m_fRevolutionStartAngle += 3.f;

	if (m_fRevolutionStartAngle > m_fRevolutionGoalAngle)
	{
		m_bIsRevolutionCW = false;
		m_fRevolutionStartAngle = 0.f;
		m_fRevolutionGoalAngle = 0.f;
		return;
	}

	_float3 Pos = m_SettingUIDesc.v3DUIPosition;
	_Matrix mat = XMMatrixInverse(nullptr, g_pGameInstance->Get_Transform_Matrix(PLM_VIEW));
	_float3 vCamPos = mat.r[3];

	m_pTransformCom->Set_Matrix(XMMatrixScaling(m_SettingUIDesc.v3DUIScaled.x, m_SettingUIDesc.v3DUIScaled.y, m_SettingUIDesc.v3DUIScaled.z) *
		XMMatrixRotationAxis(XMVectorSet(0, 1, 0, 0), XMConvertToRadians(m_SettingUIDesc.fAngle* -1.f))*
		XMMatrixTranslation(Pos.x, Pos.y, Pos.z) *
		XMMatrixRotationAxis(mat.r[1], XMConvertToRadians(m_fRevolutionStartAngle)) *
		XMMatrixTranslation(vCamPos.x, vCamPos.y, vCamPos.z));

	//m_pTransformCom->Turn_Revolution_CW(_float3(0, Pos.y, 0).XMVector(), Pos.z + 0.8f, XMConvertToRadians(3.f));
}

void CUI::UI_RevolutionCCWInitialization(_float Angle)
{
	//_float3 Pos = m_SettingUIDesc.v3DUIPosition;
	//m_pTransformCom->Turn_Revolution_CCW(_float3(0, Pos.y, 0).XMVector(), Pos.z + 0.8f, XMConvertToRadians(Angle));

	_float3 Pos = m_SettingUIDesc.v3DUIPosition;

	_Matrix mat = XMMatrixInverse(nullptr, g_pGameInstance->Get_Transform_Matrix(PLM_VIEW));

	_float3 vCamPos = mat.r[3];


	m_pTransformCom->Set_Matrix(XMMatrixScaling(m_SettingUIDesc.v3DUIScaled.x, m_SettingUIDesc.v3DUIScaled.y, m_SettingUIDesc.v3DUIScaled.z) *
		XMMatrixRotationAxis(XMVectorSet(0, 1, 0, 0), XMConvertToRadians(m_SettingUIDesc.fAngle* -1.f))*
		XMMatrixTranslation(Pos.x, Pos.y, Pos.z) *
		XMMatrixRotationAxis(-mat.r[1], XMConvertToRadians(Angle)) *
		XMMatrixTranslation(vCamPos.x, vCamPos.y, vCamPos.z));
}

void CUI::UI_RevolutionCWInitialization(_float Angle)
{
	/*_float3 Pos = m_SettingUIDesc.v3DUIPosition;
	m_pTransformCom->Turn_Revolution_CW(_float3(0, Pos.y, 0).XMVector(), Pos.z + 0.8f, XMConvertToRadians(Angle));*/

	_float3 Pos = m_SettingUIDesc.v3DUIPosition;
	_Matrix mat = XMMatrixInverse(nullptr, g_pGameInstance->Get_Transform_Matrix(PLM_VIEW));
	_float3 vCamPos = mat.r[3];

	m_pTransformCom->Set_Matrix(XMMatrixScaling(m_SettingUIDesc.v3DUIScaled.x, m_SettingUIDesc.v3DUIScaled.y, m_SettingUIDesc.v3DUIScaled.z) *
		XMMatrixRotationAxis(XMVectorSet(0, 1, 0, 0), XMConvertToRadians(m_SettingUIDesc.fAngle* -1.f))*
		XMMatrixTranslation(Pos.x, Pos.y, Pos.z) *
		XMMatrixRotationAxis(mat.r[1], XMConvertToRadians(Angle)) *
		XMMatrixTranslation(vCamPos.x, vCamPos.y, vCamPos.z));
}

void CUI::TimeDraw(_double TimeDelta)
{
	m_fDrawTime -= (_float)TimeDelta;

	if (m_fDrawTime > 0.f)
		m_bDraw = true;

	if (m_fDrawTime <= 0.f)
	{
		m_bIsDrawTimeOn = false;
		m_fDrawTime = 0.f;
		m_bDraw = false;
	}
}

void CUI::SetUp_UIInfo(SETTING_UI & pStruct)
{
	m_fX = pStruct.m_fX;
	m_fY = pStruct.m_fY;
	m_fSizeX = pStruct.m_fSizeX;
	m_fSizeY = pStruct.m_fSizeY;

	m_pUIName = pStruct.pUI_Name;
	m_iTextureIndex = pStruct.iTextureIndex;

	m_bColl = pStruct.bColl;
	m_bMove = pStruct.bMove;
	m_bDraw = pStruct.bDraw;
	m_bClick = pStruct.bClick;
	m_iLevelIndex = pStruct.iLevelIndex;
	//m_pPrototypeTag = pStruct.pPrototypeTag;
	//m_pComponentTag = pStruct.pComponentTag;
	m_fAngle = pStruct.fAngle;
}

void CUI::Update_Rect()
{
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH((_float)g_iWinCX, (_float)g_iWinCY, 0.f, 1.f)));

	m_pTransformCom->Scaled_All(XMVectorSet(m_fSizeX, m_fSizeY, 1.f, 0.0f));
	m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, XMVectorSet(m_fX - (g_iWinCX * 0.5f), -m_fY + (g_iWinCY * 0.5f), 0.f, 1.f));
}

void CUI::Update_Rect3D()
{
	m_pTransformCom->Scaled_All(m_SettingUIDesc.v3DUIScaled);
	//m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, m_SettingUIDesc.v3DUIPosition);
	//m_pTransformCom->Rotation_CCW(XMVectorSet(0.f, 1.f, 0.f, 0), XMConvertToRadians(m_SettingUIDesc.fAngle));
}

HRESULT CUI::SettingTexture()
{
	CUtilityMgr* pUtil = GetSingle(CUtilityMgr);


	FAILED_CHECK(pUtil->Bind_UtilTex_OnShader(CUtilityMgr::UTILTEX_NOISE, m_pShaderCom, "g_NoiseTexture", m_SettingUIDesc.iNoiseTextureIndex));
	FAILED_CHECK(pUtil->Bind_UtilTex_OnShader(CUtilityMgr::UTILTEX_MASK, m_pShaderCom, "g_SourTexture", m_SettingUIDesc.iMaskTextureIndex));


	//m_fUV_Y = 0.2f;
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_UV_Y", &m_fUV_Y, sizeof(_float)));

	_float4 g_vColor = m_vColor;
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_vColor", &g_vColor, sizeof(_float4)));
	
	if (m_iPassIndex == 16)
	{
		_float2 noisingdir = _float2(0.0f, 0.075f);
		FAILED_CHECK(m_pShaderCom->Set_RawValue("noisingdir", &noisingdir, sizeof(_float2)));

	}
	else
	{

		_float2 noisingdir = _float2(0.0f, 0.25f);
		FAILED_CHECK(m_pShaderCom->Set_RawValue("noisingdir", &noisingdir, sizeof(_float2)));
	}

	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_fTimer", &m_PassedTimer, sizeof(_float)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_fMaxTime", &(m_fTotalTargetTime), sizeof(_float)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_fPaperCurlIntensive", &(m_fApearTime), sizeof(_float)));

	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_fAppearTimer", &(m_fDisApearTime), sizeof(_float)));




	return S_OK;
}

void CUI::EsingUV_Y(_double fDeltaTime)
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

CUI * CUI::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void* pArg)
{
	CUI*	pInstance = NEW CUI(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CUI");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CUI::Clone(void * pArg)
{
	CUI*	pInstance = NEW CUI(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created  Clone CUI");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}
