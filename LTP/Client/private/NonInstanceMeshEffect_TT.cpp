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
	memcpy(&mMeshDesc, pArg, sizeof(NONINSTNESHEFTDESC));


	if (mMeshDesc.m_iPassIndex < 16 || mMeshDesc.m_iPassIndex > 19)
		return E_FAIL;


	FAILED_CHECK(SetUp_Components());
	
	


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

	Set_LimLight_N_Emissive( mMeshDesc.vLimLight ,  mMeshDesc.vEmissive );


	return S_OK;
}

_int CNonInstanceMeshEffect_TT::Update(_double fDeltaTime)
{
	if (__super::Update(fDeltaTime) < 0) return -1;
	if (m_pParentTranscom == nullptr)
	{
		Set_IsDead();
		return _int();
	}
	if(mAddDesc.FixFlag_Move)
	{ }
	else
	{

		if (m_pParentTranscom->Get_IsOwnerDead())
		{
			Set_IsDead();
			return _int();
		}
	}


	


	if (mIsInit == false)
	{
		// Custom Init
		m_pTransformCom->Set_ScalingSpeed(mAddDesc.AccScaleSpeed);

		_Vector scale = m_pTransformCom->Get_Scale();

		_Vector Pos = m_pParentTranscom->Get_MatrixState(CTransform::STATE_POS);

		if (mAddDesc.FollowTarget)
		{
			me_MoveType = MOVETYPE_FOLLOW;
		}

		if (mAddDesc.InitRot.y == 0 &&
			mAddDesc.InitRot.x == 0 &&
			mAddDesc.InitRot.z == 0)
		{
		

			_Vector Right = m_pParentTranscom->Get_MatrixState_Normalized(CTransform::STATE_RIGHT);
			_Vector Up = m_pParentTranscom->Get_MatrixState_Normalized(CTransform::STATE_UP);
			_Vector Look = m_pParentTranscom->Get_MatrixState_Normalized(CTransform::STATE_LOOK);


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
			
		if (me_MoveType == CNonInstanceMeshEffect_TT::MOVETYPE_EASE)
		{

			_Vector Right = m_pParentTranscom->Get_MatrixState_Normalized(CTransform::STATE_RIGHT);
			_Vector Up = m_pParentTranscom->Get_MatrixState_Normalized(CTransform::STATE_UP);
			_Vector Look = m_pParentTranscom->Get_MatrixState_Normalized(CTransform::STATE_LOOK);

			_Vector PosLocal = (Right*  mMeshDesc.vPosition.x) +
				(Up* mMeshDesc.vPosition.y) +
				(Look * mMeshDesc.vPosition.z);

			m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, Pos + PosLocal);

		}

		mIsInit = true;

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

	m_fCurTime_Duration += (_float)fDeltaTime;
	mMeshDesc.RotationSpeedPerSec += _float(mAddDesc.AccRotSpeed*fDeltaTime);

	_Vector Right = m_pParentTranscom->Get_MatrixState_Normalized(CTransform::STATE_RIGHT);
	_Vector Up = m_pParentTranscom->Get_MatrixState_Normalized(CTransform::STATE_UP);
	_Vector Look = m_pParentTranscom->Get_MatrixState_Normalized(CTransform::STATE_LOOK);
	_Vector Pos = m_pParentTranscom->Get_MatrixState(CTransform::STATE_POS);

	if (me_MoveType == CNonInstanceMeshEffect_TT::MOVETYPE_NONE)
	{

		_Vector PosLocal = (Right*  mMeshDesc.vPosition.x) +
			(Up* mMeshDesc.vPosition.y) + 
			(Look * mMeshDesc.vPosition.z);

		if (mAddDesc.AccMoveSpeed != 0)
		{
			_float Minvalue = mAddDesc.AccMoveSpeed * m_fCurTime_Duration;
			mMeshDesc.MoveSpeed += Minvalue;
		}


		_Vector MoveSpeed = m_vMoveDir.XMVector() * mMeshDesc.MoveSpeed  * _float(fDeltaTime);
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, Pos + PosLocal + MoveSpeed);


	}
	else if (me_MoveType == CNonInstanceMeshEffect_TT::MOVETYPE_EASE)
	{

		if (mbEasingStart == false)
		{
			mbEasingStart = true;
			mCurrentEasingDesc = mEasingDesc[mEasingCount];
			mStartPoint = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS);
			if (mEasingCount != 0)
			{
				mStartPoint = mEndPoint;
			}

			_float3 EndLocal = mCurrentEasingDesc.EndPos;
			_Vector PosLocal = (Right*  EndLocal.x) +
				(Up* EndLocal.y) +
				(Look * EndLocal.z);

			mEndPoint = Pos + PosLocal;

			mEasingTimer = 0;
		}

		_Vector NPos = GetSingle(CGameInstance)->Easing_Vector
		(mCurrentEasingDesc.EasingID, mStartPoint, mEndPoint, mEasingTimer, mCurrentEasingDesc.MaxTime).XMVector();

		mEasingTimer += _float(fDeltaTime);

		if(mEasingTimer >=mCurrentEasingDesc.MaxTime)
		{
			mEasingCount++;
			if (mEasingCount >= mEasingCountMAX)
			{
				Set_IsDead();
				return 0;
			}

			mbEasingStart = false;
		}

		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, NPos);


	}
	else if (me_MoveType == CNonInstanceMeshEffect_TT::MOVETYPE_FIX)
	{

		// 로컬 위치 기준으로 위치 고정


		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, mFIXPos);


	}
	else if (me_MoveType == CNonInstanceMeshEffect_TT::MOVETYPE_FOLLOW)
	{
		if (mAddDesc.FollowTarget->Get_IsOwnerDead())
		{
			Set_IsDead();
			return S_OK;
		}

		_Vector tRight = mAddDesc.FollowTarget->Get_MatrixState_Normalized(CTransform::STATE_RIGHT);
		_Vector tUp = mAddDesc.FollowTarget->Get_MatrixState_Normalized(CTransform::STATE_UP);
		_Vector tLook = mAddDesc.FollowTarget->Get_MatrixState_Normalized(CTransform::STATE_LOOK);
		_Vector tPos = mAddDesc.FollowTarget->Get_MatrixState(CTransform::STATE_POS);

		_Vector tPosLocal = (tRight*  mMeshDesc.vPosition.x) +
			(tUp* mMeshDesc.vPosition.y) +
			(tLook * mMeshDesc.vPosition.z);

		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, tPos + tPosLocal);
	}

	// ROT

	if (mAddDesc.FixFlag_Rot == true)
	{

	}
	else
	{
		// 바라보는 방향 결정
		if (mMeshDesc.RotationSpeedPerSec == 0)
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
			if (mAddDesc.LookRotSpeed == 0)
			{
				if (mAddDesc.vAddDirectAngle.x != 0)
					m_pTransformCom->Turn_Direct(Right, XMConvertToRadians(mAddDesc.vAddDirectAngle.x));
				if (mAddDesc.vAddDirectAngle.y != 0)
					m_pTransformCom->Turn_Direct(Up, XMConvertToRadians(mAddDesc.vAddDirectAngle.y));
				if (mAddDesc.vAddDirectAngle.z != 0)
					m_pTransformCom->Turn_Direct(Look, XMConvertToRadians(mAddDesc.vAddDirectAngle.z));

			}
			else
			{
				mAddRot += mAddDesc.LookRotSpeed * (_float)fDeltaTime;
				if (mAddDesc.vAddDirectAngle.x != 0)
					m_pTransformCom->Turn_Direct(Right, XMConvertToRadians(mAddDesc.vAddDirectAngle.x + mAddRot));
				if (mAddDesc.vAddDirectAngle.y != 0)
					m_pTransformCom->Turn_Direct(Up, XMConvertToRadians(mAddDesc.vAddDirectAngle.y + mAddRot));
				if (mAddDesc.vAddDirectAngle.z != 0)
					m_pTransformCom->Turn_Direct(Look, XMConvertToRadians(mAddDesc.vAddDirectAngle.z + mAddRot));
			}

		}
		else
		{
			// BUG False Code
			m_pTransformCom->Turn_CW(m_vRotAxis.XMVector(), fDeltaTime*mMeshDesc.RotationSpeedPerSec);
		}
	}
	// Scale
	if (mReScale == false)
	{
		//		m_pTransformCom->Scaling_All(fDeltaTime);
		if (mAddDesc.bAfterApperTime)
		{
			if (mMeshDesc.fMaxTime_Duration - mMeshDesc.fAppearTime < m_fCurTime_Duration)
			{
				m_pTransformCom->Set_ScalingSpeed(mAddDesc.AccScaleSpeed);

				if (mAddDesc.ScaleMax != 0)
				{
					_float3 scale;
					XMStoreFloat3(&scale, m_pTransformCom->Get_Scale());
					 
					if (mAddDesc.bLockScale[0])
					{
						if(mAddDesc.ScaleMax > scale.x)
						{
							m_pTransformCom->Scaling(CTransform::STATE_RIGHT, fDeltaTime);
						}
					}

					if (mAddDesc.bLockScale[1])
					{
						if (mAddDesc.ScaleMax > scale.y)
						{
							m_pTransformCom->Scaling(CTransform::STATE_UP, fDeltaTime);
						}
					}


					if (mAddDesc.bLockScale[2])
					{
						if (mAddDesc.ScaleMax > scale.z)
						{
							m_pTransformCom->Scaling(CTransform::STATE_LOOK, fDeltaTime);
						}
					}
				}
				else
				{
					if (mAddDesc.bLockScale[0])
					{
						m_pTransformCom->Scaling(CTransform::STATE_RIGHT, fDeltaTime);
					}

					if (mAddDesc.bLockScale[1])
					{
						m_pTransformCom->Scaling(CTransform::STATE_UP, fDeltaTime);
					}


					if (mAddDesc.bLockScale[2])
					{
						m_pTransformCom->Scaling(CTransform::STATE_LOOK, fDeltaTime);

					}
				}
	
			}

		}
		else
		{

			m_pTransformCom->Set_ScalingSpeed(mAddDesc.AccScaleSpeed);

			if (mAddDesc.ScaleMax != 0)
			{
				_float3 scale;
				XMStoreFloat3(&scale, m_pTransformCom->Get_Scale());

				if (mAddDesc.bLockScale[0])
				{
					if (mAddDesc.ScaleMax > scale.x)
					{
						m_pTransformCom->Scaling(CTransform::STATE_RIGHT, fDeltaTime);
					}
				}

				if (mAddDesc.bLockScale[1])
				{
					if (mAddDesc.ScaleMax > scale.y)
					{
						m_pTransformCom->Scaling(CTransform::STATE_UP, fDeltaTime);
					}
				}


				if (mAddDesc.bLockScale[2])
				{
					if (mAddDesc.ScaleMax > scale.z)
					{
						m_pTransformCom->Scaling(CTransform::STATE_LOOK, fDeltaTime);
					}
				}
			}
			else
			{
				if (mAddDesc.bLockScale[0])
				{
					m_pTransformCom->Scaling(CTransform::STATE_RIGHT, fDeltaTime);
				}

				if (mAddDesc.bLockScale[1])
				{
					m_pTransformCom->Scaling(CTransform::STATE_UP, fDeltaTime);
				}


				if (mAddDesc.bLockScale[2])
				{
					m_pTransformCom->Scaling(CTransform::STATE_LOOK, fDeltaTime);

				}
			}

		}


	}
	else
	{
		m_pTransformCom->Scaling_All(-fDeltaTime*2);
	}

	_Sfloat3 f = m_pTransformCom->Get_Scale();
	if (f.x <= 0.1f || f.y <= 0.1f || f.z <= 0.1f)
		Set_IsDead();

	// Emssive
	if (mAddDesc.bEmissive)
	{
		if (mbEmssive == false)
		{
			mEmissiveTimer += (_float)fDeltaTime * 3.0f;
			if (mEmissiveTimer > 1)
				mbEmssive = !mbEmssive;
		}
		else
		{
			mEmissiveTimer -= (_float)fDeltaTime * 3.0f;
			if (mEmissiveTimer < 0.1f)
				mbEmssive = !mbEmssive;
		}

		Set_LimLight_N_Emissive(_float4(0.04f, 0.18f, 0.95f, mEmissiveTimer), _float4(mEmissiveTimer, mEmissiveTimer*0.5f, mEmissiveTimer, 0.9f));
	}

	// Timer
	if (m_fCurTime_Duration >= mMeshDesc.fMaxTime_Duration)
	{
		if (mAddDesc.ScaleReFlag == false)
			Set_IsDead();
		else
		{
			mReScale = true;
			if (m_fCurTime_Duration >= mMeshDesc.fMaxTime_Duration * 50)
				Set_IsDead();
		}
	}
	

	return _int();
}

_int CNonInstanceMeshEffect_TT::LateUpdate(_double fDeltaTimer)
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

_int CNonInstanceMeshEffect_TT::Render()
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

	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_fTimer", &m_fCurTime_Duration, sizeof(_float)));
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

_int CNonInstanceMeshEffect_TT::LateRender()
{
	return _int();
}

void CNonInstanceMeshEffect_TT::Set_ParentTransform(CTransform * parentTrans)
{

	Safe_Release(m_pParentTranscom);
	m_pParentTranscom = parentTrans;
	Safe_AddRef(m_pParentTranscom);

	// 상대 취치로 지장
	if (m_pParentTranscom != nullptr)
	{
		_Vector Right = m_pParentTranscom->Get_MatrixState_Normalized(CTransform::STATE_RIGHT);
		_Vector Up = m_pParentTranscom->Get_MatrixState_Normalized(CTransform::STATE_UP);
		_Vector Look = m_pParentTranscom->Get_MatrixState_Normalized(CTransform::STATE_LOOK);
		_Vector Pos = m_pParentTranscom->Get_MatrixState(CTransform::STATE_POS);

		_Vector PosLocal = (Right*  mMeshDesc.vPosition.x) +
			(Up* mMeshDesc.vPosition.y) +
			(Look * mMeshDesc.vPosition.z);


		if (mAddDesc.FixFlag_Move == false)
		{
			m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, Pos + PosLocal);

		}
		else
		{
			mFIXPos = Pos + PosLocal;
			m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, mFIXPos);
			me_MoveType = CNonInstanceMeshEffect_TT::MOVETYPE_FIX;

		}

	}
	else
	{
		m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, mMeshDesc.vPosition);
	}

}


HRESULT CNonInstanceMeshEffect_TT::Set_EasingMoveDesc(const MESHAEASING* desc, _uint count)
{
	if (count <= 0)
		return E_FAIL;

	me_MoveType = MOVETYPE_EASE;

	mEasingDesc = NEW MESHAEASING[count];

	for (_uint i = 0; i < count; ++i)
	{
		memcpy(&mEasingDesc[i], &desc[i], sizeof(MESHAEASING));
	}
	mEasingCount = 0;
	mEasingCountMAX = count;
	mbEasingStart = false;
	return S_OK;
}

void CNonInstanceMeshEffect_TT::Set_DeadMeshparticle()
{
	m_fCurTime_Duration = mMeshDesc.fMaxTime_Duration - mMeshDesc.fAppearTime;
}

HRESULT CNonInstanceMeshEffect_TT::SetUp_Components()
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
	mAddDesc.FollowTarget = nullptr;

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pModel);
	Safe_Release(m_pTransformCom); 
	Safe_Release(m_pParentTranscom);

	Safe_Delete_Array(mEasingDesc);

}
