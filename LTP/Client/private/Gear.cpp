#include "stdafx.h"
#include "..\public\Gear.h"


CGear::CGear(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CMapObject(pDevice, pDeviceContext)
{
}

CGear::CGear(const CGear & rhs)
	: CMapObject(rhs)
{
}

HRESULT CGear::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));


	return S_OK;
}

HRESULT CGear::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	if (nullptr != pArg)
	{
		memcpy(&m_GearStateDesc, pArg, sizeof(GEAR_STATEDESC));
	}

	SetUp_Components();


	return S_OK;
}

_int CGear::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)
		return -1;

	return _int();
}

_int CGear::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)
		return -1;


	return _int();
}

_int CGear::Render()
{
	if (__super::Render() < 0)
		return -1;


	return 0;
}

_int CGear::LateRender()
{
	if (__super::LateRender() < 0)
		return -1;




	return _int();
}

HRESULT CGear::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VNAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));


	switch (m_GearStateDesc.iGearTypeNumber)
	{
	case GEARTYPE_1:
	{
		break;
	}
	}


	return S_OK;
}

CGear * CGear::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CGear*	pInstance = NEW CGear(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CGear");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CGear::Clone(void * pArg)
{
	CGear*	pInstance = NEW CGear(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created Clone CGear");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CGear::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModel);
}
