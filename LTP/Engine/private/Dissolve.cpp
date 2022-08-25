#include "..\public\Dissolve.h"
#include "Shader.h"
#include "Texture.h"
#include "GameInstance.h"




CDissolve::CDissolve(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CComponent(pDevice, pDeviceContext)
{
}

CDissolve::CDissolve(const CDissolve & rhs)
	: CComponent(rhs),
	m_szTexturePrototypeTag(rhs.m_szTexturePrototypeTag)
{
}


HRESULT CDissolve::Initialize_Prototype(const _tchar* szDissolvePrototypeTag)
{
	m_szTexturePrototypeTag = szDissolvePrototypeTag;


	return S_OK;
}

HRESULT CDissolve::Initialize_Clone(void * pArg)
{
	NULL_CHECK_RETURN(pArg, E_FAIL);
	
	memcpy(&m_tDissolveDesc, pArg, sizeof(DISSOLVEDESC));

	NULL_CHECK_RETURN(m_tDissolveDesc.pModel,E_FAIL);
	NULL_CHECK_RETURN(m_tDissolveDesc.pShader, E_FAIL);

	switch (m_tDissolveDesc.eDissolveModelType)
	{
	case Engine::CDissolve::DISSOLVE_ANIM:
		m_iDissolvePassedIndex = 7;
		m_iShadowPassIndex = 10;
		break;
	case Engine::CDissolve::DISSOLVE_ANIM_ATTACHED:
		m_iDissolvePassedIndex = 9;
		m_iShadowPassIndex = 12;
		break;
	case Engine::CDissolve::DISSOLVE_NONANIM:
		m_iDissolvePassedIndex = 10;
		m_iShadowPassIndex = 12;

		break;
	case Engine::CDissolve::DISSOLVE_NONANIM_ATTACHED:
		m_iDissolvePassedIndex = 11;
		m_iShadowPassIndex = 13;
		break;

	case Engine::CDissolve::DISSOLVE_ANIM_WEIGHTW:
		m_iDissolvePassedIndex = 8;
		m_iShadowPassIndex = 10;
		break;
	case Engine::CDissolve::DISSOLVE_NONANIM_OCLD:
		m_iDissolvePassedIndex = 27;
		m_iShadowPassIndex = 12;
		break;

	case Engine::CDissolve::DISSOLVE_ANIM_ATTACHED_OLCD:
		m_iDissolvePassedIndex = 20;
		m_iShadowPassIndex = 12;
		break;
		

	default:
		MSGBOX("Wrong Dissolve Type");
		__debugbreak();
		break;
	}
	m_pDissolveTexture = (CTexture*) GetSingle(CGameInstance)->Clone_Component(0, m_szTexturePrototypeTag.c_str());
	NULL_CHECK_RETURN(m_pDissolveTexture, E_FAIL);

	return S_OK;
}

_int CDissolve::Render(_uint iPassIndex)
{
	if (m_bIsDissolving)
	{
		FAILED_CHECK(m_pDissolveTexture->Bind_OnShader(m_tDissolveDesc.pShader, "g_DissolveNoiseTexture", 0));
		FAILED_CHECK(m_pDissolveTexture->Bind_OnShader(m_tDissolveDesc.pShader, "g_BurnRampTexture", m_tDissolveDesc.RampTextureIndex));
		

		_float DissolveValue = (m_bIsFadeIn) ? 
			(_float(m_TargetTime - m_PassedTime) / (_float)m_TargetTime) :
			(_float(m_PassedTime) / (_float)m_TargetTime) ;

		DissolveValue = max(min(DissolveValue, 1.f), 0);

		FAILED_CHECK(m_tDissolveDesc.pShader->Set_RawValue("g_fDissolveValue", &(DissolveValue), sizeof(_float)));
	

		_uint NumMaterial = m_tDissolveDesc.pModel->Get_NumMaterial();

		for (_uint i = 0; i < NumMaterial; i++)
		{
			for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; j++)
				FAILED_CHECK(m_tDissolveDesc.pModel->Bind_OnShader(m_tDissolveDesc.pShader, i, j, MODLETEXTYPEFORENGINE(j)));

			FAILED_CHECK(m_tDissolveDesc.pModel->Render(m_tDissolveDesc.pShader, m_iDissolvePassedIndex, i, "g_BoneMatrices"));

		}
	}
	else
	{

		_uint NumMaterial = m_tDissolveDesc.pModel->Get_NumMaterial();

		for (_uint i = 0; i < NumMaterial; i++)
		{
			for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; j++)
				FAILED_CHECK(m_tDissolveDesc.pModel->Bind_OnShader(m_tDissolveDesc.pShader, i, j, MODLETEXTYPEFORENGINE(j)));

			FAILED_CHECK(m_tDissolveDesc.pModel->Render(m_tDissolveDesc.pShader, iPassIndex, i, "g_BoneMatrices"));
		}

	}



	return _int();
}

_int CDissolve::Render_SkipMtrl(_uint iPassIndex, vector<_uint>* vecSkipMtrlIndexs)
{

	_uint vectorIndex = 0;

	if (vecSkipMtrlIndexs->size() == 0)
	{

		if (m_bIsDissolving)
		{
			FAILED_CHECK(m_pDissolveTexture->Bind_OnShader(m_tDissolveDesc.pShader, "g_DissolveNoiseTexture", 0));
			FAILED_CHECK(m_pDissolveTexture->Bind_OnShader(m_tDissolveDesc.pShader, "g_BurnRampTexture", m_tDissolveDesc.RampTextureIndex));


			_float DissolveValue = (m_bIsFadeIn) ?
				(_float(m_TargetTime - m_PassedTime) / (_float)m_TargetTime) :
				(_float(m_PassedTime) / (_float)m_TargetTime);

			DissolveValue = max(min(DissolveValue, 1.f), 0);

			FAILED_CHECK(m_tDissolveDesc.pShader->Set_RawValue("g_fDissolveValue", &(DissolveValue), sizeof(_float)));


			_uint NumMaterial = m_tDissolveDesc.pModel->Get_NumMaterial();

			for (_uint i = 0; i < NumMaterial; i++)
			{

				for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; j++)
					FAILED_CHECK(m_tDissolveDesc.pModel->Bind_OnShader(m_tDissolveDesc.pShader, i, j, MODLETEXTYPEFORENGINE(j)));

				FAILED_CHECK(m_tDissolveDesc.pModel->Render(m_tDissolveDesc.pShader, m_iDissolvePassedIndex, i, "g_BoneMatrices"));

			}
		}
		else
		{

			_uint NumMaterial = m_tDissolveDesc.pModel->Get_NumMaterial();

			for (_uint i = 0; i < NumMaterial; i++)
			{

				for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; j++)
					FAILED_CHECK(m_tDissolveDesc.pModel->Bind_OnShader(m_tDissolveDesc.pShader, i, j, MODLETEXTYPEFORENGINE(j)));

				FAILED_CHECK(m_tDissolveDesc.pModel->Render(m_tDissolveDesc.pShader, iPassIndex, i, "g_BoneMatrices"));
			}

		}


	}

	else
	{
		if (m_bIsDissolving)
		{
			FAILED_CHECK(m_pDissolveTexture->Bind_OnShader(m_tDissolveDesc.pShader, "g_DissolveNoiseTexture", 0));
			FAILED_CHECK(m_pDissolveTexture->Bind_OnShader(m_tDissolveDesc.pShader, "g_BurnRampTexture", m_tDissolveDesc.RampTextureIndex));


			_float DissolveValue = (m_bIsFadeIn) ?
				(_float(m_TargetTime - m_PassedTime) / (_float)m_TargetTime) :
				(_float(m_PassedTime) / (_float)m_TargetTime);

			DissolveValue = max(min(DissolveValue, 1.f), 0);

			FAILED_CHECK(m_tDissolveDesc.pShader->Set_RawValue("g_fDissolveValue", &(DissolveValue), sizeof(_float)));


			_uint NumMaterial = m_tDissolveDesc.pModel->Get_NumMaterial();

			for (_uint i = 0; i < NumMaterial; i++)
			{
				if (vectorIndex < vecSkipMtrlIndexs->size() && (*vecSkipMtrlIndexs)[vectorIndex] == i)
				{
					vectorIndex++;
					continue;
				}

				for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; j++)
					FAILED_CHECK(m_tDissolveDesc.pModel->Bind_OnShader(m_tDissolveDesc.pShader, i, j, MODLETEXTYPEFORENGINE(j)));

				FAILED_CHECK(m_tDissolveDesc.pModel->Render(m_tDissolveDesc.pShader, m_iDissolvePassedIndex, i, "g_BoneMatrices"));

			}
		}
		else
		{

			_uint NumMaterial = m_tDissolveDesc.pModel->Get_NumMaterial();

			for (_uint i = 0; i < NumMaterial; i++)
			{
				if (vectorIndex < vecSkipMtrlIndexs->size() &&(*vecSkipMtrlIndexs)[vectorIndex] == i)
				{
					vectorIndex++;
					continue;
				}

				for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; j++)
					FAILED_CHECK(m_tDissolveDesc.pModel->Bind_OnShader(m_tDissolveDesc.pShader, i, j, MODLETEXTYPEFORENGINE(j)));

				FAILED_CHECK(m_tDissolveDesc.pModel->Render(m_tDissolveDesc.pShader, iPassIndex, i, "g_BoneMatrices"));
			}

		}
	}




	return _int();
}

_int CDissolve::Render_Shadow(_uint iPassIndex)
{

	if (m_bIsDissolving)
	{
		FAILED_CHECK(m_pDissolveTexture->Bind_OnShader(m_tDissolveDesc.pShader, "g_DissolveNoiseTexture", 0));
		FAILED_CHECK(m_pDissolveTexture->Bind_OnShader(m_tDissolveDesc.pShader, "g_BurnRampTexture", m_tDissolveDesc.RampTextureIndex));


		_float DissolveValue = (m_bIsFadeIn) ?
			(_float(m_TargetTime - m_PassedTime) / (_float)m_TargetTime) :
			(_float(m_PassedTime) / (_float)m_TargetTime);

		DissolveValue = max(min(DissolveValue, 1.f), 0);

		FAILED_CHECK(m_tDissolveDesc.pShader->Set_RawValue("g_fDissolveValue", &(DissolveValue), sizeof(_float)));


		_uint NumMaterial = m_tDissolveDesc.pModel->Get_NumMaterial();

		for (_uint i = 0; i < NumMaterial; i++)
		{
			for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; j++)
				FAILED_CHECK(m_tDissolveDesc.pModel->Bind_OnShader(m_tDissolveDesc.pShader, i, j, MODLETEXTYPEFORENGINE(j)));

			FAILED_CHECK(m_tDissolveDesc.pModel->Render(m_tDissolveDesc.pShader, m_iShadowPassIndex, i, "g_BoneMatrices"));

		}
	}
	else
	{
		_uint NumMaterial = m_tDissolveDesc.pModel->Get_NumMaterial();

		for (_uint i = 0; i < NumMaterial; i++)
		{
			for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; j++)
				FAILED_CHECK(m_tDissolveDesc.pModel->Bind_OnShader(m_tDissolveDesc.pShader, i, j, MODLETEXTYPEFORENGINE(j)));

			FAILED_CHECK(m_tDissolveDesc.pModel->Render(m_tDissolveDesc.pShader, iPassIndex, i, "g_BoneMatrices"));
		}

	}



	return _int();
}

HRESULT CDissolve::Set_DissolveOn(_bool bFadeIn, _double TargetTime)
{
	if (m_PassedTime < m_TargetTime)
		return S_FALSE;

	m_bIsDissolving = true;
	m_bIsFadeIn = bFadeIn;
	m_TargetTime = TargetTime;
	m_PassedTime = 0;

	return _int();
}

HRESULT CDissolve::Update_Dissolving(_double fDeltaTimme)
{
	if (!m_bIsDissolving) return S_FALSE;

	m_PassedTime += fDeltaTimme;

	if (m_PassedTime >= m_TargetTime) m_bIsDissolving = (m_bIsFadeIn)? false : true;
	

	return S_OK;
}


CDissolve * CDissolve::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, const _tchar* szDissolvePrototypeTag)
{
	CDissolve*	pInstance = new CDissolve(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(szDissolvePrototypeTag)))
	{
		MSGBOX("Failed to Creating CDissolve");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CDissolve::Clone(void * pArg)
{
	CDissolve*	pInstance = new CDissolve(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Creating CDissolve");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDissolve::Free()
{
	__super::Free();
	Safe_Release(m_pDissolveTexture);
}