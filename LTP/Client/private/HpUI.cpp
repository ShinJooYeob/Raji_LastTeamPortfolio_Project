#include "stdafx.h"
#include "..\public\HpUI.h"

CHpUI::CHpUI(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CHpUI::CHpUI(const CHpUI & rhs)
	: CGameObject(rhs)
{
}

HRESULT CHpUI::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));


	return S_OK;
}

HRESULT CHpUI::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	ZeroMemory(&m_HpDesc, sizeof(HPDESC));
	if (nullptr != pArg)
	{
		memcpy(&m_HpDesc, pArg, sizeof(HPDESC));
	}

	FAILED_CHECK(SetUp_Components());

	m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, m_HpDesc.m_vPos);

	if (m_HpDesc.m_HPType == CHpUI::HP_RAJI)
	{
		for (_int i = 0; i < 9; ++i)
		{
			_float Angle = (_float)i*(360.f / 9.f);
			_Matrix Mat = XMMatrixRotationY(XMConvertToRadians(Angle));
			_float3 Pos = _float3(0.f, 0, 0).XMVector() + (Mat.r[2] * 0.5f);

			Pos.y += 0.1f;

			CTransform* pTransform = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
			NULL_CHECK_RETURN(pTransform, E_FAIL);

			pTransform->Turn_CW(XMVectorSet(1.f, 0, 0.f, 0), XMConvertToRadians(90.f));
			//pTransform->Turn_CW(XMVectorSet(0.f, 1.f, 0.f, 0), XMConvertToRadians(Angle + 180.f));
			pTransform->Set_MatrixState(CTransform::STATE_POS, Pos);

			pTransform->Scaled_All(_float3(0.2f));

			m_vPosTransforms.push_back(pTransform);
		}
	}
	else if (m_HpDesc.m_HPType == CHpUI::HP_MONSTER)
	{
		for (_int i = 0; i < (_int)m_HpDesc.m_pObjcect->Get_MaxHP(); ++i)
		{
			_float Angle = (_float)i*(360.f / m_HpDesc.m_pObjcect->Get_MaxHP());
			_Matrix Mat = XMMatrixRotationY(XMConvertToRadians(Angle));
			_float3 Pos = _float3(0.f, 0, 0).XMVector() + (Mat.r[2] * 0.8f);

			Pos.y += 0.1f;

			CTransform* pTransform = (CTransform*)g_pGameInstance->Clone_Component(SCENE_STATIC, TAG_CP(Prototype_Transform));
			NULL_CHECK_RETURN(pTransform, E_FAIL);

			pTransform->Turn_CW(XMVectorSet(1.f, 0, 0.f, 0), XMConvertToRadians(90.f));
			//pTransform->Turn_CW(XMVectorSet(0.f, 1.f, 0.f, 0), XMConvertToRadians(Angle + 180.f));
			pTransform->Set_MatrixState(CTransform::STATE_POS, Pos);

			pTransform->Scaled_All(_float3(0.2f));

			m_vPosTransforms.push_back(pTransform);
		}
	}


	return S_OK;
}

_int CHpUI::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0)return -1;

	Set_ObjectPos();

	if (m_iHitCount < 0)
		m_iHitCount = 0;

	if (m_HpDesc.m_HPType == CHpUI::HP_RAJI)
	{
		if (m_iHitCount >(_int)m_HpDesc.m_pObjcect->Get_MaxHP())
		{
			m_iHitCount = (_int)m_HpDesc.m_pObjcect->Get_MaxHP();
			//다이!!!!!
		}

		for (_int i = 0; i < 9; ++i)
		{
			_float Angle = (_float)i*(360.f / 9.f);
			_Matrix Mat = XMMatrixRotationY(XMConvertToRadians(Angle));
			_float3 Pos = _float3(0.f, 0, 0).XMVector() + (Mat.r[2] * m_HpDesc.m_Dimensions);

			Pos.y += 0.1f;

			_Matrix Mat2 = XMMatrixRotationX(XMConvertToRadians(90))
				*XMMatrixRotationY(XMConvertToRadians(Angle + 180.f));

			Mat2.r[3] = _float4(Pos, 1).XMVector();

			m_vPosTransforms[i]->Set_Matrix(Mat2);


			m_vPosTransforms[i]->Scaled_All(_float3(0.2f));
		}

		for (_int i = 0; i < m_iHitCount; ++i)
		{
			m_vecInstancedEmptyHPTransform.push_back(m_vPosTransforms[i]);
		}
		for (_int i = m_iHitCount; i < m_vPosTransforms.size(); ++i)
		{
			m_vecInstancedHPTransform.push_back(m_vPosTransforms[i]);
		}
	}
	else if (m_HpDesc.m_HPType == CHpUI::HP_MONSTER)
	{

		if (m_iHitCount > (_int)m_HpDesc.m_pObjcect->Get_MaxHP())
		{
			m_iHitCount = (_int)m_HpDesc.m_pObjcect->Get_MaxHP();
			//다이!!!!!
		}

		_int iMaxHP = (_int)m_HpDesc.m_pObjcect->Get_MaxHP();

		for (_int i = 0; i < iMaxHP; ++i)
		{
			_float Angle = (_float)i*(360.f / (_float)iMaxHP);
			_Matrix Mat = XMMatrixRotationY(XMConvertToRadians(Angle));
			_float3 Pos = _float3(0.f, 0, 0).XMVector() + (Mat.r[2] * m_HpDesc.m_Dimensions);

			Pos.y += 0.1f;

			_Matrix Mat2 = XMMatrixRotationX(XMConvertToRadians(90))
				*XMMatrixRotationY(XMConvertToRadians(Angle + 180.f));

			Mat2.r[3] = _float4(Pos, 1).XMVector();

			m_vPosTransforms[i]->Set_Matrix(Mat2);


			m_vPosTransforms[i]->Scaled_All(_float3(1.1f - (m_HpDesc.m_pObjcect->Get_MaxHP() *0.01f), 0.5f, 0.5f));
		}

		for (_int i = 0; i < m_vPosTransforms.size(); ++i)
		{
			if(i < m_iHitCount)
				continue;

			m_vecInstancedHPTransform.push_back(m_vPosTransforms[i]);
		}
	}


	return _int();
}

_int CHpUI::LateUpdate(_double fDeltaTime)
{
	if (__super::LateUpdate(fDeltaTime) < 0)return -1;

	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SUBBLEND, this));

	return _int();
}

_int CHpUI::Render()
{
	if (__super::Render() < 0)		return -1;

	if (nullptr == m_pVIBufferCom)
		return -1;

	CGameInstance* pInstance = GetSingle(CGameInstance);
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));

	_float ATV = .1f;
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_vTextureFigureUVSize", &_float2(1, 1), sizeof(_float2)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_fAlphaTestValue", &ATV, sizeof(_float)));




	//FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));

	D3D11_MAPPED_SUBRESOURCE SubResource;

	if (m_HpDesc.m_HPType == CHpUI::HP_RAJI)
	{
		FAILED_CHECK(m_pTextureCom->Bind_OnShader(m_pShaderCom, "g_DiffuseTexture", 1));

		FAILED_CHECK(m_pVIBufferCom->Lock(&SubResource));

		for (_int i = 0; i < m_vecInstancedEmptyHPTransform.size(); ++i)
		{
			XMStoreFloat4(&(((VTXINSTMAT*)SubResource.pData)[i].vRight), ((m_vecInstancedEmptyHPTransform[i]->Get_MatrixState(CTransform::STATE_RIGHT))));
			XMStoreFloat4(&(((VTXINSTMAT*)SubResource.pData)[i].vUp), ((m_vecInstancedEmptyHPTransform[i]->Get_MatrixState(CTransform::STATE_UP))));
			XMStoreFloat4(&(((VTXINSTMAT*)SubResource.pData)[i].vLook), ((m_vecInstancedEmptyHPTransform[i]->Get_MatrixState(CTransform::STATE_LOOK))));

			XMStoreFloat4(&(((VTXINSTMAT*)SubResource.pData)[i].vTranslation),
				((m_vecInstancedEmptyHPTransform[i]->Get_MatrixState(CTransform::STATE_POS) + m_pObjectPos.XMVector())));
			((VTXINSTMAT*)SubResource.pData)[i].vTranslation.w = 1;

			((VTXINSTMAT*)SubResource.pData)[i].vColor = (_float4)XMVectorSet(1.f, 1.f, 1.f, 1.f);

			((VTXINSTMAT*)SubResource.pData)[i].vUV_WHSize = _float4(0, 0, 0, 1.f);

			((VTXINSTMAT*)SubResource.pData)[i].vTimer = _float4(0, 0, 0, 0);

		}
		m_pVIBufferCom->UnLock();

		FAILED_CHECK(m_pVIBufferCom->Render(m_pShaderCom, 24, (_int)m_vecInstancedEmptyHPTransform.size()));


		ATV = .1f;
		FAILED_CHECK(m_pShaderCom->Set_RawValue("g_vTextureFigureUVSize", &_float2(1, 1), sizeof(_float2)));
		FAILED_CHECK(m_pShaderCom->Set_RawValue("g_fAlphaTestValue", &ATV, sizeof(_float)));


		FAILED_CHECK(m_pTextureCom->Bind_OnShader(m_pShaderCom, "g_DiffuseTexture", 0));

		FAILED_CHECK(m_pVIBufferCom->Lock(&SubResource));

		for (_int i = 0; i < m_vecInstancedHPTransform.size(); ++i)
		{
			XMStoreFloat4(&(((VTXINSTMAT*)SubResource.pData)[i].vRight), ((m_vecInstancedHPTransform[i]->Get_MatrixState(CTransform::STATE_RIGHT))));
			XMStoreFloat4(&(((VTXINSTMAT*)SubResource.pData)[i].vUp), ((m_vecInstancedHPTransform[i]->Get_MatrixState(CTransform::STATE_UP))));
			XMStoreFloat4(&(((VTXINSTMAT*)SubResource.pData)[i].vLook), ((m_vecInstancedHPTransform[i]->Get_MatrixState(CTransform::STATE_LOOK))));

			XMStoreFloat4(&(((VTXINSTMAT*)SubResource.pData)[i].vTranslation),
				((m_vecInstancedHPTransform[i]->Get_MatrixState(CTransform::STATE_POS) + m_pObjectPos.XMVector())));
			((VTXINSTMAT*)SubResource.pData)[i].vTranslation.w = 1;

			((VTXINSTMAT*)SubResource.pData)[i].vColor = (_float4)XMVectorSet(1.f, 1.f, 1.f, 1.f);

			((VTXINSTMAT*)SubResource.pData)[i].vUV_WHSize = _float4(0, 0, 0, 1.f);

			((VTXINSTMAT*)SubResource.pData)[i].vTimer = _float4(0, 0, 0, 0);

		}
		m_pVIBufferCom->UnLock();

		FAILED_CHECK(m_pVIBufferCom->Render(m_pShaderCom, 24, (_int)m_vecInstancedHPTransform.size()));

		m_vecInstancedEmptyHPTransform.clear();
		m_vecInstancedHPTransform.clear();
	}
	else if (m_HpDesc.m_HPType == CHpUI::HP_MONSTER)
	{
		// #RICK MergeOff
		//FAILED_CHECK(m_pTextureCom->Bind_OnShader(m_pShaderCom, "g_DiffuseTexture", 0));

		//ATV = .1f;
		//FAILED_CHECK(m_pShaderCom->Set_RawValue("g_vTextureFigureUVSize", &_float2(1, 1), sizeof(_float2)));
		//FAILED_CHECK(m_pShaderCom->Set_RawValue("g_fAlphaTestValue", &ATV, sizeof(_float)));

		//FAILED_CHECK(m_pVIBufferCom->Lock(&SubResource));

		//for (_int i = 0; i < m_vecInstancedHPTransform.size(); ++i)
		//{
		//	XMStoreFloat4(&(((VTXINSTMAT*)SubResource.pData)[i].vRight), ((m_vecInstancedHPTransform[i]->Get_MatrixState(CTransform::STATE_RIGHT))));
		//	XMStoreFloat4(&(((VTXINSTMAT*)SubResource.pData)[i].vUp), ((m_vecInstancedHPTransform[i]->Get_MatrixState(CTransform::STATE_UP))));
		//	XMStoreFloat4(&(((VTXINSTMAT*)SubResource.pData)[i].vLook), ((m_vecInstancedHPTransform[i]->Get_MatrixState(CTransform::STATE_LOOK))));

		//	XMStoreFloat4(&(((VTXINSTMAT*)SubResource.pData)[i].vTranslation),
		//		((m_vecInstancedHPTransform[i]->Get_MatrixState(CTransform::STATE_POS) + m_pObjectPos.XMVector())));
		//	((VTXINSTMAT*)SubResource.pData)[i].vTranslation.w = 1;

		//	((VTXINSTMAT*)SubResource.pData)[i].vColor = (_float4)XMVectorSet(1.f, 1.f, 1.f, 1.f);

		//	((VTXINSTMAT*)SubResource.pData)[i].vUV_WHSize = _float4(0, 0, 0, 1.f);

		//	((VTXINSTMAT*)SubResource.pData)[i].vTimer = _float4(0, 0, 0, 0);

		//}
		//m_pVIBufferCom->UnLock();

		//FAILED_CHECK(m_pVIBufferCom->Render(m_pShaderCom, 24, (_int)m_vecInstancedHPTransform.size()));

		m_vecInstancedEmptyHPTransform.clear();
		m_vecInstancedHPTransform.clear();
	}


	return _int();
}

_int CHpUI::LateRender()
{
	return _int();
}

void CHpUI::Set_ObjectPos()
{
	CTransform* ObjTransform = (CTransform*)m_HpDesc.m_pObjcect->Get_Component(TAG_COM(Com_Transform));

	m_pObjectPos = ObjTransform->Get_MatrixState(CTransform::STATE_POS);
}

HRESULT CHpUI::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VTXPOINTINST), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_VIBuffer_Point_Instance_128), TAG_COM(Com_VIBuffer), (CComponent**)&m_pVIBufferCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom));

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(SCENE_STATIC, TAG_CP(Prototype_Texture_HPUI), TAG_COM(Com_HPUI), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (m_HpDesc.m_HPType == CHpUI::HP_RAJI)
	{
		FAILED_CHECK(m_pTextureCom->Change_TextureLayer(L"RajiHP"));
	}
	else if (m_HpDesc.m_HPType == CHpUI::HP_MONSTER)
	{
		FAILED_CHECK(m_pTextureCom->Change_TextureLayer(L"MonsterHP"));
	}

	return S_OK;
}

CHpUI * CHpUI::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CHpUI*	pInstance = NEW CHpUI(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CHpUI");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CHpUI::Clone(void * pArg)
{
	CHpUI*	pInstance = NEW CHpUI(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created Clone CHpUI");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CHpUI::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);

	for (_int i = 0; i < m_vPosTransforms.size(); ++i)
		Safe_Release(m_vPosTransforms[i]);

	m_vPosTransforms.clear();

	// #RICK MergeOff
	//for (_int i = 0; i < m_vecInstancedHPTransform.size(); ++i)
	//	Safe_Release(m_vecInstancedHPTransform[i]);

	m_vecInstancedHPTransform.clear();

	for (_int i = 0; i < m_vecInstancedEmptyHPTransform.size(); ++i)
		Safe_Release(m_vecInstancedEmptyHPTransform[i]);

	m_vecInstancedEmptyHPTransform.clear();
}
