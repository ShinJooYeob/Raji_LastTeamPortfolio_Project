#include "stdafx.h"
#include "..\public\NonInstanceMeshEffect_TT.h"


CNonInstanceMeshEffect_TT::CNonInstanceMeshEffect_TT(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CNonInstanceMeshEffect_TT::CNonInstanceMeshEffect_TT(const CNonInstanceMeshEffect_TT & rhs)
	: CGameObject(rhs)
{
}

HRESULT CNonInstanceMeshEffect_TT::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CNonInstanceMeshEffect_TT::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	NULL_CHECK_RETURN(pArg, E_FAIL);
	memcpy(&m_tMeshDesc, pArg, sizeof(NONINSTNESHEFTDESC));


	if (m_tMeshDesc.m_iPassIndex < 16 || m_tMeshDesc.m_iPassIndex > 19)
		return E_FAIL;


	FAILED_CHECK(SetUp_Components());
	
	m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, m_tMeshDesc.vPosition);

	m_pTransformCom->LookDir(m_tMeshDesc.vLookDir.XMVector());
	m_pTransformCom->Scaled_All(m_tMeshDesc.vSize);

	// RotAxis 회전 방향 결정 
	switch (m_tMeshDesc.RotAxis)
	{
	case FollowingDir_Right:
		m_vRotAxis = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_RIGHT);
		break;
	case FollowingDir_Up:
		m_vRotAxis = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_UP);
		break;
	case FollowingDir_Look:
		m_vRotAxis = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_LOOK);
		break;
	default:
		__debugbreak();
		break;
	}
	Set_LimLight_N_Emissive( m_tMeshDesc.vLimLight ,  m_tMeshDesc.vEmissive );


	return S_OK;
}

_int CNonInstanceMeshEffect_TT::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0) return -1;

	if (m_pParentTranscom->Get_IsOwnerDead())
	{
		Set_IsDead();
		return _int();
	}


	m_fCurTime_Duration += (_float)fDeltaTime;

	m_tMeshDesc.RotationSpeedPerSec += mAddDesc.AccRotSpeed*fDeltaTime;


	if (m_pParentTranscom)
	{

	//	m_pTransformCom->LookDir(m_pParentTranscom->Get_MatrixState(CTransform::STATE_LOOK));
		_Vector Right = m_pParentTranscom->Get_MatrixState_Normalized(CTransform::STATE_RIGHT);
		_Vector Up = m_pParentTranscom->Get_MatrixState_Normalized(CTransform::STATE_UP);
		_Vector Look = m_pParentTranscom->Get_MatrixState_Normalized(CTransform::STATE_LOOK);

		_Vector Pos = m_pParentTranscom->Get_MatrixState(CTransform::STATE_POS);
		_Vector PosLocal = (Right*  m_tMeshDesc.vPosition.x) + (Up* m_tMeshDesc.vPosition.y) + (Look * m_tMeshDesc.vPosition.z);


		// 바라보는 방향 결정
		if (m_tMeshDesc.RotationSpeedPerSec ==0)
		{

			switch (mAddDesc.LookRotAxis)
			{
			case FollowingDir_Right:
				m_vLookAxis = Right;
				break;
			case FollowingDir_Up:
				m_vLookAxis = Up;
				break;
			case FollowingDir_Look:
				m_vLookAxis = Up;
				break;
			default:
				__debugbreak();
				break;
			}

			m_pTransformCom->LookDir(m_vLookAxis.XMVector());
			if (mAddDesc.vAddDirectAngle.x != 0)
				m_pTransformCom->Turn_Direct(Right, XMConvertToRadians(mAddDesc.vAddDirectAngle.x));
			else if (mAddDesc.vAddDirectAngle.y != 0)
				m_pTransformCom->Turn_Direct(Up, XMConvertToRadians(mAddDesc.vAddDirectAngle.y));
			else if (mAddDesc.vAddDirectAngle.z != 0)
				m_pTransformCom->Turn_Direct(Look, XMConvertToRadians(mAddDesc.vAddDirectAngle.z));

		}
		else
		{
			m_pTransformCom->Turn_CW(m_vRotAxis.XMVector(), fDeltaTime*m_tMeshDesc.RotationSpeedPerSec);

		}


		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, Pos + PosLocal);

	}
	else
	{
		// FIXPos

	}
	Set_LimLight_N_Emissive(m_tMeshDesc.vLimLight, m_tMeshDesc.vEmissive);


	if (m_fCurTime_Duration >= m_tMeshDesc.fMaxTime_Duration)
	{
		Set_IsDead();
	}

	return _int();
}

_int CNonInstanceMeshEffect_TT::LateUpdate(_double fDeltaTimer)
{
	if (__super::LateUpdate(fDeltaTimer) < 0) return -1;

	if (m_tMeshDesc.m_iPassIndex > 17)
	{
		FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
	}
	else
	{
		FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_DISTORTION, this));
	}

	return _int();
}

_int CNonInstanceMeshEffect_TT::Render()
{
	if (__super::Render() < 0)		return -1;

	NULL_CHECK_RETURN(m_pModel, E_FAIL);

	CGameInstance* pInstance = GetSingle(CGameInstance);


	//_float4 color = _float4(1.f, 0.734375f, 0.75234375f, 1);
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_vColor", &m_tMeshDesc.vColor, sizeof(_float4)));
	
	
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));
	FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));

	//_float2 noisingdir = _float2(1, 1).Get_Nomalize();
	//_float	fDistortionNoisingPushPower = 0.5f;
	//_float	fAppearTime = 2.f;

	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_fTimer", &m_fCurTime_Duration, sizeof(_float)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_fAppearTimer", &m_tMeshDesc.fAppearTime, sizeof(_float)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_fMaxTime", &m_tMeshDesc.fMaxTime_Duration, sizeof(_float)));
	
	FAILED_CHECK(m_pShaderCom->Set_RawValue("noisingdir", &m_tMeshDesc.noisingdir, sizeof(_float2)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_fDistortionNoisingPushPower", &m_tMeshDesc.fDistortionNoisingPushPower, sizeof(_float)));



	FAILED_CHECK(GetSingle(CUtilityMgr)->Bind_UtilTex_OnShader(CUtilityMgr::UTILTEX_NOISE, m_pShaderCom, "g_NoiseTexture", m_tMeshDesc.NoiseTextureIndex));
	FAILED_CHECK(GetSingle(CUtilityMgr)->Bind_UtilTex_OnShader(CUtilityMgr::UTILTEX_MASK, m_pShaderCom, "g_SourTexture", m_tMeshDesc.MaskTextureIndex));
	FAILED_CHECK(m_pShaderCom->Set_Texture("g_BackBufferTexture", g_pGameInstance->Get_SRV(L"Target_ReferenceDefferred")));

	_uint NumMaterial = m_pModel->Get_NumMaterial();

	for (_uint i = 0; i < NumMaterial; i++)
	{
		for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; j++)
		{
			if (j == 1)
			{
				FAILED_CHECK(GetSingle(CUtilityMgr)->Bind_UtilTex_OnShader(CUtilityMgr::UTILTEX_NOISE, m_pShaderCom, "g_DiffuseTexture", m_tMeshDesc.iDiffuseTextureIndex));
			}
			else
			{
				FAILED_CHECK(m_pModel->Bind_OnShader(m_pShaderCom, i, j, MODLETEXTYPE(j)));
			}
		}

		FAILED_CHECK(m_pModel->Render(m_pShaderCom, m_tMeshDesc.m_iPassIndex, i));
	}

	return _int();
}

_int CNonInstanceMeshEffect_TT::LateRender()
{
	return _int();
}


HRESULT CNonInstanceMeshEffect_TT::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VNAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(m_tMeshDesc.eMeshType), TAG_COM(Com_Model), (CComponent**)&m_pModel));

	CTransform::TRANSFORMDESC tDesc = {};
	tDesc.fMovePerSec = 30;
	tDesc.fRotationPerSec = XMConvertToRadians(m_tMeshDesc.RotationSpeedPerSec);
	tDesc.fScalingPerSec = 1;
	tDesc.vPivot = _float3(0, 0, 0);
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));

	return S_OK;
}


CNonInstanceMeshEffect_TT * CNonInstanceMeshEffect_TT::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CNonInstanceMeshEffect_TT*	pInstance = NEW CNonInstanceMeshEffect_TT(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CNonInstanceMeshEffect_TT");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CNonInstanceMeshEffect_TT::Clone(void * pArg)
{
	CNonInstanceMeshEffect_TT*	pInstance = NEW CNonInstanceMeshEffect_TT(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CNonInstanceMeshEffect_TT");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CNonInstanceMeshEffect_TT::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModel);
	Safe_Release(m_pTransformCom); 
	Safe_Release(m_pParentTranscom);

}
