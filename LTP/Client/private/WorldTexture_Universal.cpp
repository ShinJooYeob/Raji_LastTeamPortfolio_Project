#include "stdafx.h"
#include "..\public\WorldTexture_Universal.h"

const _tchar* m_pWorldTexture_UniversalTag[CWorldTexture_Universal::WORLDTEXTURE_UNIVERSAL_END]
{
	L"Alpabet",
	L"DonkeyKong_Map"
};

CWorldTexture_Universal::CWorldTexture_Universal(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CWorldTexture_Universal::CWorldTexture_Universal(const CWorldTexture_Universal & rhs)
	: CGameObject(rhs)
{
}

HRESULT CWorldTexture_Universal::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));


	return S_OK;
}

HRESULT CWorldTexture_Universal::Initialize_Clone(void * pArg)
{
	if (nullptr != pArg)
	{
		ZeroMemory(&m_WorldTexture_UniversalDesc, sizeof(WORLDTEXTURE_UNIVERSALDESC));
		memcpy(&m_WorldTexture_UniversalDesc, pArg, sizeof(WORLDTEXTURE_UNIVERSALDESC));
	}
	else {
		__debugbreak();
		MSGBOX("CWorldTexture_Universal의 pArg가 Nullptr입니다.");
	}
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components());

	SetUp_Info();

	return S_OK;
}

_int CWorldTexture_Universal::Update(_double dDeltaTime)
{
	if (__super::Update(dDeltaTime) < 0)return -1;

	m_dDurationTime += dDeltaTime;

	if (m_dDurationTime >= m_WorldTexture_UniversalDesc.dDuration)
	{
		Set_IsDead();
	}

	if (m_WorldTexture_UniversalDesc.bBillboardOn == true)
	{
		Billboard();
	}
	if (m_WorldTexture_UniversalDesc.bMagnetOn == true)
	{
		Magnet();
	}

	PlayOn(dDeltaTime);
	
	return _int();
}

_int CWorldTexture_Universal::LateUpdate(_double dDeltaTime)
{
	if (__super::LateUpdate(dDeltaTime) < 0)return -1;

	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND_NOLIGHT, this));


	return _int();
}

_int CWorldTexture_Universal::Render()
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

	m_pVIBufferCom->Render(m_pShaderCom, 0);

	return _int();
}

_int CWorldTexture_Universal::LateRender()
{
	if (__super::LateRender() < 0)
		return -1;


	return _int();
}

HRESULT CWorldTexture_Universal::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VT), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_VIBuffer_Rect), TAG_COM(Com_VIBuffer), (CComponent**)&m_pVIBufferCom));

	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_WorldTexture_Universal), TAG_COM(Com_Texture), (CComponent**)&m_pTextureCom));

	m_pTextureCom->Change_TextureLayer(m_pWorldTexture_UniversalTag[m_WorldTexture_UniversalDesc.iWorldTextureNumber]);

	CTransform::TRANSFORMDESC tDesc = {};

	tDesc.fMovePerSec = 5;
	tDesc.fRotationPerSec = XMConvertToRadians(60);
	tDesc.vPivot = _float3(0, 0, 0);
	tDesc.fScalingPerSec = 1;

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));


	return S_OK;
}

HRESULT CWorldTexture_Universal::SetUp_Info()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (nullptr != m_WorldTexture_UniversalDesc.pObject)
	{
		CGameObject* pObject = static_cast<CGameObject*>(m_WorldTexture_UniversalDesc.pObject);
		m_pObjectTransform = static_cast<CTransform*>(pObject->Get_Component(TAG_COM(Com_Transform)));

		m_pTransformCom->Scaled_All(m_WorldTexture_UniversalDesc.fScale);

		_Vector vPosition = m_pObjectTransform->Get_MatrixState(CTransform::STATE_POS);

		vPosition += XMVector3Normalize(m_pObjectTransform->Get_MatrixState(CTransform::STATE_RIGHT)) *m_WorldTexture_UniversalDesc.fPositioning.x;
		vPosition += XMVector3Normalize(m_pObjectTransform->Get_MatrixState(CTransform::STATE_UP)) * m_WorldTexture_UniversalDesc.fPositioning.y;
		vPosition += XMVector3Normalize(m_pObjectTransform->Get_MatrixState(CTransform::STATE_LOOK)) * m_WorldTexture_UniversalDesc.fPositioning.z;

		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, vPosition);

	}
	else {
		MSGBOX("CWorldTexture_Universal의 m_WorldTexture_UniversalDesc.pObject가 nullptr입니다.");
	}

	m_pCamera = static_cast<CCamera_Main*>(pGameInstance->Get_GameObject_By_LayerIndex(SCENE_STATIC, TAG_LAY(Layer_Camera_Main)));

	m_pCameraTransform = m_pCamera->Get_CamTransformCom();

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CWorldTexture_Universal::Billboard()
{
	//뷰 스페이스에서 역행렬을 하면 카메라의 월드 스페이스가 나옴
	_Vector vRight = m_pCameraTransform->Get_MatrixState(CTransform::STATE_RIGHT);
	_Vector vUp = m_pCameraTransform->Get_MatrixState(CTransform::STATE_UP);
	_Vector vLook = m_pCameraTransform->Get_MatrixState(CTransform::STATE_LOOK);
	
	vRight = m_pTransformCom->Get_MatrixScale(CTransform::STATE_RIGHT) * XMVector3Normalize(vRight);
	vUp = m_pTransformCom->Get_MatrixScale(CTransform::STATE_UP) * XMVector3Normalize(vUp);
	vLook = m_pTransformCom->Get_MatrixScale(CTransform::STATE_LOOK) * XMVector3Normalize(vLook);

	m_pTransformCom->Set_MatrixState(CTransform::STATE_RIGHT,vRight);
	m_pTransformCom->Set_MatrixState(CTransform::STATE_UP, vUp);
	m_pTransformCom->Set_MatrixState(CTransform::STATE_LOOK, vLook);

	return S_OK;
}

HRESULT CWorldTexture_Universal::Magnet()
{
	_Vector vPosition = m_pObjectTransform->Get_MatrixState(CTransform::STATE_POS);

	vPosition += XMVector3Normalize(m_pObjectTransform->Get_MatrixState(CTransform::STATE_RIGHT)) *m_WorldTexture_UniversalDesc.fPositioning.x;
	vPosition += XMVector3Normalize(m_pObjectTransform->Get_MatrixState(CTransform::STATE_UP)) * m_WorldTexture_UniversalDesc.fPositioning.y;
	vPosition += XMVector3Normalize(m_pObjectTransform->Get_MatrixState(CTransform::STATE_LOOK)) * m_WorldTexture_UniversalDesc.fPositioning.z;

	m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, vPosition);

	return S_OK;
}

HRESULT CWorldTexture_Universal::PlayOn(_double dDeltaTime)
{
	switch (m_WorldTexture_UniversalDesc.iWorldTextureNumber)
	{
	case CWorldTexture_Universal::ALPABET:
	{
		Alpabet(dDeltaTime);
		break;
	}
	default:
		break;
	}
	return S_OK;
}

HRESULT CWorldTexture_Universal::Alpabet(_double dDeltaTime)
{
	if (*m_WorldTexture_UniversalDesc.pSwitch == false)
	{
		Set_IsDead();
	}
	return S_OK;
}

CWorldTexture_Universal * CWorldTexture_Universal::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CWorldTexture_Universal*	pInstance = NEW CWorldTexture_Universal(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CWorldTexture_Universal");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CWorldTexture_Universal::Clone(void * pArg)
{
	CWorldTexture_Universal*	pInstance = NEW CWorldTexture_Universal(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CWorldTexture_Universal");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CWorldTexture_Universal::Free()
{

	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}