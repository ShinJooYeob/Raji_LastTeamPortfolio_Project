#include "stdafx.h"
#include "..\public\MapObject.h"



CMapObject::CMapObject(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice,pDeviceContext)
{
}

CMapObject::CMapObject(const CMapObject & rhs)
	:CGameObject(rhs)
{
}

HRESULT CMapObject::Initialize_Prototype(void * pArg)
{
	return S_OK;
}

HRESULT CMapObject::Initialize_Clone(void * pArg)
{
	return S_OK;
}

_int CMapObject::Update(_double fDeltaTime)
{
	return _int();
}

_int CMapObject::LateUpdate(_double fDeltaTime)
{
	return _int();
}

_int CMapObject::Render()
{
	return _int();
}

_int CMapObject::LateRender()
{
	return _int();
}

HRESULT CMapObject::SetUp_ConstTable(CShader* pShader)
{
	CGameInstance* pInstance = GetSingle(CGameInstance);
	FAILED_CHECK(pShader->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(pShader->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));


	//FAILED_CHECK(pShader->Set_RawValue("g_CamPosition", &pInstance->Get_TargetPostion_float4(PLV_CAMERA), sizeof(_float4)));
	//FAILED_CHECK(pShader->Set_RawValue("g_CamLookDir", &pInstance->Get_TargetPostion_float4(PLV_CAMLOOK), sizeof(_float4)));


	//const LIGHTDESC* pLightDesc = pInstance->Get_LightDesc(LIGHTDESC::TYPE_DIRECTIONAL, 0);
	//NULL_CHECK_RETURN(pLightDesc, -1);

	//FAILED_CHECK(pShader->Set_RawValue("g_vLightVector", &(pLightDesc->vVector), sizeof(_float4)));
	//FAILED_CHECK(pShader->Set_RawValue("g_vLightDiffuse", &pLightDesc->vDiffuse, sizeof(_float4)));
	//FAILED_CHECK(pShader->Set_RawValue("g_vLightAmbient", &pLightDesc->vAmbient, sizeof(_float4)));
	//FAILED_CHECK(pShader->Set_RawValue("g_vLightSpecular", &pLightDesc->vSpecular, sizeof(_float4)));


	return S_OK;
}

void CMapObject::Free()
{
	__super::Free();

}
