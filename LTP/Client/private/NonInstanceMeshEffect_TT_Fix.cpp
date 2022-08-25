#include "stdafx.h"
#include "..\public\NonInstanceMeshEffect_TT_Fix.h"


CNonInstanceMeshEffect_TT_Fix::CNonInstanceMeshEffect_TT_Fix(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CNonInstanceMeshEffect_TT_Fix::CNonInstanceMeshEffect_TT_Fix(const CNonInstanceMeshEffect_TT_Fix & rhs)
	: CGameObject(rhs)
{
}

HRESULT CNonInstanceMeshEffect_TT_Fix::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));

	return S_OK;
}

HRESULT CNonInstanceMeshEffect_TT_Fix::Initialize_Clone(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	NULL_CHECK_RETURN(pArg, E_FAIL);
	memcpy(&mMeshDesc, pArg, sizeof(NONINSTNESHEFTDESC));


	if (mMeshDesc.m_iPassIndex < 16 || mMeshDesc.m_iPassIndex > 19)
		return E_FAIL;


	FAILED_CHECK(SetUp_Components());
	mIsInit = false;

	m_pTransformCom->LookDir(mMeshDesc.vLookDir.XMVector());
	m_pTransformCom->Scaled_All(mMeshDesc.vSize);
	
	

	// RotAxis ROTDIR
	switch (mMeshDesc.RotAxis)
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

	// MoveDIR
	switch (mMeshDesc.MoveDir)
	{
	case FollowingDir_Right:
		m_vMoveDir = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_RIGHT);
		break;
	case FollowingDir_Up:
		m_vMoveDir = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_UP);
		break;
	case FollowingDir_Look:
		m_vMoveDir = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_LOOK);
		break;
	default:
		__debugbreak();
		break;
	}

	Set_LimLight_N_Emissive(mMeshDesc.vLimLight ,  mMeshDesc.vEmissive );


	return S_OK;
}

_int CNonInstanceMeshEffect_TT_Fix::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0) return -1;
	
	

	if (mIsInit == false)
	{
		mIsInit = Update_INIT();
	}

	if (mIsDealy)
	{
		mDealyTime -= fDeltaTime;
		if (mDealyTime >= 0)
		{
			mDealyTime -= fDeltaTime;
			return 0;
		}
		mIsDealy = false;
	}

	bool isUpdatecheck = IsLife(fDeltaTime);

	_Sfloat3 scale = m_pTransformCom->Get_Scale();
	if (scale.x <= 0.001f || scale.y <= 0.001f || scale.z <= 0.001f)
		isUpdatecheck = false;

	if (isUpdatecheck == false)
	{
		Set_IsDead();
		return 0;
	}

	if (Update_Move(fDeltaTime))
	{
	}

	if (Update_Rot(fDeltaTime))
	{
	}
	if (Update_Scale(fDeltaTime))
	{
	}
	if (Update_Light(fDeltaTime))
	{
	}
	return _int();
}

_int CNonInstanceMeshEffect_TT_Fix::LateUpdate(_double fDeltaTimer)
{
	if (__super::LateUpdate(fDeltaTimer) < 0) return -1;

	if (mMeshDesc.m_iPassIndex > 17)
	{
		FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this));
	}
	else
	{
		FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_DISTORTION, this));
	}

	return _int();
}

_int CNonInstanceMeshEffect_TT_Fix::Render()
{
	if (__super::Render() < 0)		
		return -1;

	if (mIsDealy)
		return 0;

	NULL_CHECK_BREAK(m_pModel);

	CGameInstance* pInstance = GetSingle(CGameInstance);


	//_float4 color = _float4(1.f, 0.734375f, 0.75234375f, 1);
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_vColor", &mMeshDesc.vColor, sizeof(_float4)));
	
	
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));
	FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));

	//_float2 noisingdir = _float2(1, 1).Get_Nomalize();
	//_float	fDistortionNoisingPushPower = 0.5f;
	//_float	fAppearTime = 2.f;

	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_fTimer", &m_CurTime_Duration, sizeof(_float)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_fAppearTimer", &mMeshDesc.fAppearTime, sizeof(_float)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_fMaxTime", &mMeshDesc.fMaxTime_Duration, sizeof(_float)));
	
	FAILED_CHECK(m_pShaderCom->Set_RawValue("noisingdir", &mMeshDesc.noisingdir, sizeof(_float2)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_fDistortionNoisingPushPower", &mMeshDesc.fDistortionNoisingPushPower, sizeof(_float)));

	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_fAlphaTestValue", &mMeshDesc.fAlphaTestValue, sizeof(_float)));


	FAILED_CHECK(GetSingle(CUtilityMgr)->Bind_UtilTex_OnShader(CUtilityMgr::UTILTEX_NOISE, m_pShaderCom, "g_NoiseTexture", mMeshDesc.NoiseTextureIndex));
	FAILED_CHECK(GetSingle(CUtilityMgr)->Bind_UtilTex_OnShader(CUtilityMgr::UTILTEX_MASK, m_pShaderCom, "g_SourTexture", mMeshDesc.MaskTextureIndex));
	FAILED_CHECK(m_pShaderCom->Set_Texture("g_BackBufferTexture", g_pGameInstance->Get_SRV(L"Target_ReferenceDefferred")));

	_uint NumMaterial = m_pModel->Get_NumMaterial();

	for (_uint i = 0; i < NumMaterial; i++)
	{
		for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; j++)
		{
			if (j == 1)
			{
				FAILED_CHECK(GetSingle(CUtilityMgr)->Bind_UtilTex_OnShader(CUtilityMgr::UTILTEX_NOISE, m_pShaderCom, "g_DiffuseTexture", mMeshDesc.iDiffuseTextureIndex));
			}
			else
			{
				FAILED_CHECK(m_pModel->Bind_OnShader(m_pShaderCom, i, j, MODLETEXTYPE(j)));
			}
		}

		FAILED_CHECK(m_pModel->Render(m_pShaderCom, mMeshDesc.m_iPassIndex, i));
	}

	return _int();
}

_int CNonInstanceMeshEffect_TT_Fix::LateRender()
{
	return _int();
}

CTransform * CNonInstanceMeshEffect_TT_Fix::Get_Transform()
{
	return m_pTransformCom;

}

void CNonInstanceMeshEffect_TT_Fix::Set_Position(_float3 pos)
{


	m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, pos);
}

void CNonInstanceMeshEffect_TT_Fix::Set_Matrix(_fMatrix mat)
{
	m_pTransformCom->Set_Matrix(mat);
}

_float3 CNonInstanceMeshEffect_TT_Fix::Get_Postition()
{
	return m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS);
}


void CNonInstanceMeshEffect_TT_Fix::Set_DeadMeshparticle()
{
	m_CurTime_Duration = mMeshDesc.fMaxTime_Duration - mMeshDesc.fAppearTime;
}

HRESULT CNonInstanceMeshEffect_TT_Fix::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VNAM), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(mMeshDesc.eMeshType), TAG_COM(Com_Model), (CComponent**)&m_pModel));

	CTransform::TRANSFORMDESC tDesc = {};
	tDesc.fMovePerSec = 30;
	tDesc.fRotationPerSec = XMConvertToRadians(mMeshDesc.RotationSpeedPerSec);
	tDesc.fScalingPerSec = 1;
	tDesc.vPivot = _float3(0, 0, 0);
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));

	return S_OK;
}

bool CNonInstanceMeshEffect_TT_Fix::IsLife(_double Timer)
{
	m_CurTime_Duration += Timer;

	// Timer
	if (m_CurTime_Duration >= (_double)mMeshDesc.fMaxTime_Duration)
	{
		return false;
	}


	return true;
}

bool CNonInstanceMeshEffect_TT_Fix::Update_INIT()
{
	// Custom Init
	CTransform::TRANSFORMDESC Transspeed;
	Transspeed.fMovePerSec = mAddDesc.AccMoveSpeed;
	Transspeed.fRotationPerSec = mAddDesc.AccRotSpeed;
	Transspeed.fScalingPerSec = mAddDesc.AccScaleSpeed;

	m_pTransformCom->Set_TransformDesc(Transspeed);

	_Vector scale = m_pTransformCom->Get_Scale();

	_Vector Right = m_pTransformCom->Get_MatrixState_Normalized(CTransform::STATE_RIGHT);
	_Vector Up = m_pTransformCom->Get_MatrixState_Normalized(CTransform::STATE_UP);
	_Vector Look = m_pTransformCom->Get_MatrixState_Normalized(CTransform::STATE_LOOK);
	_Vector Pos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);


	_Matrix worldmat = m_pTransformCom->Get_WorldMatrix();

	_float3 LocalPos = mMeshDesc.vPosition;

	worldmat.r[3] =
		Pos +
		Right * LocalPos.x +
		Up * LocalPos.y +
		Look * LocalPos.z;

	m_pTransformCom->Set_Matrix(worldmat);


	// NOUSE
	mAddDesc.FollowTarget = nullptr;
	mAddDesc.ScaleReFlag = false;
	mAddDesc.bAfterApperTime = false;

	mMeshDesc.RotationSpeedPerSec = 0;
	mMeshDesc.MoveSpeed = 0;
	mMeshDesc.SizeSpeed = 0;

	if (mAddDesc.InitRot.y == 0 &&
		mAddDesc.InitRot.x == 0 &&
		mAddDesc.InitRot.z == 0)
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
			m_vLookAxis = Look;
			break;
		default:
			__debugbreak();
			break;
		}

		m_pTransformCom->LookDir(m_vLookAxis.XMVector());
		if (mAddDesc.vAddDirectAngle.x != 0)
			m_pTransformCom->Turn_Direct(Right, XMConvertToRadians(mAddDesc.vAddDirectAngle.x));
		if (mAddDesc.vAddDirectAngle.y != 0)
			m_pTransformCom->Turn_Direct(Up, XMConvertToRadians(mAddDesc.vAddDirectAngle.y));
		if (mAddDesc.vAddDirectAngle.z != 0)
			m_pTransformCom->Turn_Direct(Look, XMConvertToRadians(mAddDesc.vAddDirectAngle.z));
	}

	else
	{
		_Squternion qq = _Squternion::CreateFromYawPitchRoll(
			mAddDesc.InitRot.y,
			mAddDesc.InitRot.x,
			mAddDesc.InitRot.z);

		_Matrix rotmat = _Sfloat4x4::CreateFromQuaternion(qq);

		rotmat.r[0] *= scale.m128_f32[0];
		rotmat.r[1] *= scale.m128_f32[1];
		rotmat.r[2] *= scale.m128_f32[2];
		rotmat.r[3] = Pos;
		m_pTransformCom->Set_Matrix(rotmat);
	}

	

	return true;
}

bool CNonInstanceMeshEffect_TT_Fix::Update_Move(_double Timer)
{
	if (mAddDesc.AccMoveSpeed != 0)
	{
		m_pTransformCom->MovetoDir(m_vMoveDir.XMVector(), Timer);
	}

	return true;
}

bool CNonInstanceMeshEffect_TT_Fix::Update_Rot(_double Timer)
{
	// ROT
	if (mAddDesc.FixFlag_Rot == true)
	{

	}
	else
	{
		// 바라보는 방향 결정
		if (mAddDesc.AccRotSpeed == 0)
		{
			_Vector Right = m_pTransformCom->Get_MatrixState_Normalized(CTransform::STATE_RIGHT);
			_Vector Up = m_pTransformCom->Get_MatrixState_Normalized(CTransform::STATE_UP);
			_Vector Look = m_pTransformCom->Get_MatrixState_Normalized(CTransform::STATE_LOOK);

			switch (mAddDesc.LookRotAxis)
			{
			case FollowingDir_Right:
				m_vLookAxis = Right;
				break;
			case FollowingDir_Up:
				m_vLookAxis = Up;
				break;
			case FollowingDir_Look:
				m_vLookAxis = Look;
				break;
			default:
				__debugbreak();
				break;
			}

			m_pTransformCom->LookDir(m_vLookAxis.XMVector());

			if (mAddDesc.LookRotSpeed == 0)
			{
				

			}
			else
			{
				if (mAddDesc.vAddDirectAngle.x != 0)
					m_pTransformCom->Turn_Direct(Right, XMConvertToRadians(mAddDesc.vAddDirectAngle.x));
				if (mAddDesc.vAddDirectAngle.y != 0)
					m_pTransformCom->Turn_Direct(Up, XMConvertToRadians(mAddDesc.vAddDirectAngle.y));
				if (mAddDesc.vAddDirectAngle.z != 0)
					m_pTransformCom->Turn_Direct(Look, XMConvertToRadians(mAddDesc.vAddDirectAngle.z));
			}

		}
		else
		{
			// BUG False Code
			m_pTransformCom->Turn_CW(m_vRotAxis.XMVector(), Timer);
		}
	}

	return true;
}

bool CNonInstanceMeshEffect_TT_Fix::Update_Scale(_double Timer)
{
	// Scale

	if (mAddDesc.AccScaleSpeed == 0)
	{

	}
	else if (mAddDesc.bAfterApperTime)
	{
		{
			if (mMeshDesc.fMaxTime_Duration - mMeshDesc.fAppearTime < m_CurTime_Duration)
			{
				if (mAddDesc.ScaleMax != 0)
				{
					_float3 scale;
					XMStoreFloat3(&scale, m_pTransformCom->Get_Scale());

					if (mAddDesc.bLockScale[0])
					{
						if (mAddDesc.ScaleMax > scale.x)
						{
							m_pTransformCom->Scaling(CTransform::STATE_RIGHT, Timer);
						}
					}

					if (mAddDesc.bLockScale[1])
					{
						if (mAddDesc.ScaleMax > scale.y)
						{
							m_pTransformCom->Scaling(CTransform::STATE_UP, Timer);
						}
					}


					if (mAddDesc.bLockScale[2])
					{
						if (mAddDesc.ScaleMax > scale.z)
						{
							m_pTransformCom->Scaling(CTransform::STATE_LOOK, Timer);
						}
					}
				}
				else
				{
					if (mAddDesc.bLockScale[0])
					{
						m_pTransformCom->Scaling(CTransform::STATE_RIGHT, Timer);
					}

					if (mAddDesc.bLockScale[1])
					{
						m_pTransformCom->Scaling(CTransform::STATE_UP, Timer);
					}


					if (mAddDesc.bLockScale[2])
					{
						m_pTransformCom->Scaling(CTransform::STATE_LOOK, Timer);

					}
				}

			}

		}
	}
	else
	{
		m_pTransformCom->Scaling_All(Timer);
		if (mAddDesc.ScaleMax != 0)
		{
			_float3 scale;
			XMStoreFloat3(&scale, m_pTransformCom->Get_Scale());

			if (mAddDesc.bLockScale[0])
			{
				if (mAddDesc.ScaleMax > scale.x)
				{
					m_pTransformCom->Scaling(CTransform::STATE_RIGHT, Timer);
				}
			}

			if (mAddDesc.bLockScale[1])
			{
				if (mAddDesc.ScaleMax > scale.y)
				{
					m_pTransformCom->Scaling(CTransform::STATE_UP, Timer);
				}
			}


			if (mAddDesc.bLockScale[2])
			{
				if (mAddDesc.ScaleMax > scale.z)
				{
					m_pTransformCom->Scaling(CTransform::STATE_LOOK, Timer);
				}
			}
		}
		else
		{
			if (mAddDesc.bLockScale[0])
			{
				m_pTransformCom->Scaling(CTransform::STATE_RIGHT, Timer);
			}

			if (mAddDesc.bLockScale[1])
			{
				m_pTransformCom->Scaling(CTransform::STATE_UP, Timer);
			}


			if (mAddDesc.bLockScale[2])
			{
				m_pTransformCom->Scaling(CTransform::STATE_LOOK, Timer);

			}
		}
	}

	return true;
}

bool CNonInstanceMeshEffect_TT_Fix::Update_Light(_double Timer)
{
	_float4 Emsive = mEmissive_Origin;
	_float4 Rim = mRim_Origin;
	_float mul = 0.0f;
	if (mLightDesc.bEmsiive)
	{
		
		mTimer_Em += Timer * mLightDesc.SpeedTime_Emsive;

		mul = XMScalarSin(_float(mTimer_Em)) * 0.5f + 0.5f;
		if (mul <= mLightDesc.Ambiant)
			mul = mLightDesc.Ambiant;
		Emsive = _float4(
			mEmissive_Origin.x * mul,
			mEmissive_Origin.y * mul,
			mEmissive_Origin.z * mul,
			mEmissive_Origin.w);

	}

	if (mLightDesc.bRim)
	{
		mTimer_Rim += Timer * mLightDesc.SpeedTime_Rim;

		mul = XMScalarSin(_float(mTimer_Rim)) * 0.5f + 0.5f;
		
		if (mul <= mLightDesc.Ambiant)
			mul = mLightDesc.Ambiant;

		Rim = _float4(
			mRim_Origin.x * mul,
			mRim_Origin.y * mul,
			mRim_Origin.z * mul,
			mRim_Origin.w);

	}

	if (mLightDesc.bRim || mLightDesc.bEmsiive)
	{
		Set_LimLight_N_Emissive(Rim, Emsive);
	}


	//Set_LimLight_N_Emissive(_float4(mEmissive_origin.x*mEmissiveTimer, mEmissive_origin.y*mEmissiveTimer, mEmissive_origin.z *mEmissiveTimer, mEmissiveTimer),
	//	_float4(mEmissiveTimer, mEmissiveTimer*0.5f, mEmissiveTimer, 1.0f));



	return true;
}


CNonInstanceMeshEffect_TT_Fix * CNonInstanceMeshEffect_TT_Fix::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CNonInstanceMeshEffect_TT_Fix*	pInstance = NEW CNonInstanceMeshEffect_TT_Fix(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CNonInstanceMeshEffect_TT_Fix");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CNonInstanceMeshEffect_TT_Fix::Clone(void * pArg)
{
	CNonInstanceMeshEffect_TT_Fix*	pInstance = NEW CNonInstanceMeshEffect_TT_Fix(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CNonInstanceMeshEffect_TT_Fix");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CNonInstanceMeshEffect_TT_Fix::Free()
{
	__super::Free();
	mAddDesc.FollowTarget = nullptr;

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModel);
	Safe_Release(m_pTransformCom); 

}
