#include "stdafx.h"
#include "..\public\Golu_Bullet.h"
#include "PartilceCreateMgr.h"

const _tchar* m_pGolu_BulletTag[CGolu_Bullet::GOLU_BULLET_END]
{
	L"FireBall",
	L"BarrierBullet",
	L"BlackHole",
	L"NonTexture"
};

CGolu_Bullet::CGolu_Bullet(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice, pDeviceContext)
{
}

CGolu_Bullet::CGolu_Bullet(const CGolu_Bullet & rhs)
	: CGameObject(rhs)
{
}

HRESULT CGolu_Bullet::Initialize_Prototype(void * pArg)
{
	FAILED_CHECK(__super::Initialize_Prototype(pArg));


	return S_OK;
}

HRESULT CGolu_Bullet::Initialize_Clone(void * pArg)
{
	if (nullptr != pArg)
	{
		ZeroMemory(&m_Golu_BulletDesc, sizeof(GOLU_BULLETDESC));
		memcpy(&m_Golu_BulletDesc, pArg, sizeof(GOLU_BULLETDESC));
	}
	else {
		__debugbreak();
		MSGBOX("CGolu_Bullet의 pArg가 Nullptr입니다.");
	}
	FAILED_CHECK(__super::Initialize_Clone(pArg));

	FAILED_CHECK(SetUp_Components());

	SetUp_Info();



	Initialize_Bullet();

	return S_OK;
}

_int CGolu_Bullet::Update(_double dDeltaTime)
{
	if (__super::Update(dDeltaTime) < 0)return -1;

	m_dDurationTime += dDeltaTime;
	m_dParticleTime += dDeltaTime;
	m_SoundTime += dDeltaTime;

	if (m_dDurationTime >= m_Golu_BulletDesc.dDuration)
	{
		Set_IsDead();
	}
	Distance();

	//Billboard 순서 중요함
	PlayOn(dDeltaTime);

	if(m_Golu_BulletDesc.bColiiderOn == true)
		Update_Collider(dDeltaTime);

	return _int();
}

_int CGolu_Bullet::LateUpdate(_double dDeltaTime)
{
	if (__super::LateUpdate(dDeltaTime) < 0)return -1;

	FAILED_CHECK(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND_NOLIGHT, this));


#ifdef _DEBUG
	FAILED_CHECK(m_pRendererCom->Add_DebugGroup(m_pColliderCom));
#endif

	return _int();
}

_int CGolu_Bullet::Render()
{
	if (__super::Render() < 0)
		return -1;

	NULL_CHECK_RETURN(m_pVIBufferCom, E_FAIL);

	FAILED_CHECK(m_pTransformCom->Bind_OnShader(m_pShaderCom, "g_WorldMatrix"));

	CGameInstance* pInstance = GetSingle(CGameInstance);
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ViewMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_VIEW), sizeof(_float4x4)));
	FAILED_CHECK(m_pShaderCom->Set_RawValue("g_ProjMatrix", &pInstance->Get_Transform_Float4x4_TP(PLM_PROJ), sizeof(_float4x4)));

	if (FAILED(m_pTextureCom->Bind_OnShader(m_pShaderCom, "g_DiffuseTexture", m_Golu_BulletDesc.iTextureIndex)))
		return E_FAIL;

	m_pVIBufferCom->Render(m_pShaderCom, 0);

	return _int();
}

_int CGolu_Bullet::LateRender()
{
	if (__super::LateRender() < 0)
		return -1;


	return _int();
}

void CGolu_Bullet::CollisionTriger(CCollider * pMyCollider, _uint iMyColliderIndex, CGameObject * pConflictedObj, CCollider * pConflictedCollider, _uint iConflictedObjColliderIndex, CollisionTypeID eConflictedObjCollisionType)
{
	switch (m_Golu_BulletDesc.iGoluBulletType)
	{
	case FIREBALL:
		if (m_fDistance < 12)
		{
			if (m_SoundTime > 0.5)
			{
				g_pGameInstance->Play3D_Sound(TEXT("EH_M1_1179.mp3"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_MONSTER, 0.3f);

				m_SoundTime = 0;
			}
		}
		break;
	case BARRIERBULLET:
	{
		if (m_SoundTime > 0.5)
		{
			g_pGameInstance->Play3D_Sound(TEXT("EH_M1_2576.mp3"), m_pTransformCom->Get_MatrixState(CTransform::STATE_POS), CHANNELID::CHANNEL_MONSTER, 0.3f);

			m_SoundTime = 0;
		}
		switch (m_Golu_BulletDesc.iTextureIndex)
		{
		case 0:
		{
			if (m_dParticleTime > 0.3)
			{
				// Fragment
				INSTMESHDESC ParticleMesh = GETPARTICLE->Get_EffectSetting_Mesh(CPartilceCreateMgr::E_MESHINST_EFFECTJ::Um_MeshBase4_TurnAuto,
					Prototype_Mesh_SM_4E_IceShards_01, //FBX
					0.01f, //파티클 전체의 지속시간 한번만 재생시키기 위해 짧음
					0.8f, //파티클 지속시간
					_float4(0.28f, 0.29f, 0.95f, 0.0f), //색깔1
					_float4(0), //색깔2 색깔1~2끼리 움직이면서 함 즉, 바꾸지 않게 하려면 같은 색을 넣고
					1, //여기에 1을 넣으면 됨
					_float3(2), //사이즈
					_float3(0.1f), //사이즈2 이것도 위에랑 마찬가지
					1);
				ParticleMesh.eParticleTypeID = InstanceEffect_Ball; //퍼지는 타입
				ParticleMesh.eInstanceCount = Prototype_ModelInstance_16; //인스턴스 갯수
				ParticleMesh.ePassID = MeshPass_BrightColor; //노이즈

														 //범위
				_float val = 1.5f;
				ParticleMesh.ParticleStartRandomPosMin = _float3(-val, -0.5f, -val);
				ParticleMesh.ParticleStartRandomPosMax = _float3(val, -0.5f, val);

				//디퓨즈 텍스쳐
				ParticleMesh.TempBuffer_0.w = 301;

				ParticleMesh.iMaskingTextureIndex = NONNMASK;//노이즈 타입이 아니면 동작하지 않음
				ParticleMesh.iMaskingTextureIndex = 122;
				ParticleMesh.iNoiseTextureIndex = 289;
				ParticleMesh.vEmissive_SBB = _float3(1.f, 1.0f, 1.f);
				ParticleMesh.Particle_Power = 20.0f;

				ParticleMesh.SubPowerRandomRange_RUL = _float3(1, 1, 1);
				ParticleMesh.fRotSpeed_Radian = XMConvertToRadians(max(1080, 0));


				ParticleMesh.TempBuffer_0.z = 1; //한번에 파티클이 생성됨

											 //위치지정
											 //_Matrix mat = m_pTransformCom->Get_WorldMatrix();
											 //_Vector pos = mat.r[3] + mat.r[2] * 3;
											 //testMesh.vFixedPosition = pos;

											 //위치지정
				ParticleMesh.FollowingTarget = nullptr;
				ParticleMesh.vFixedPosition = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS);
				//ParticleMesh.FollowingTarget = m_pTransformCom;
				ParticleMesh.iFollowingDir = FollowingDir_Up;

				GETPARTICLE->Create_MeshInst_DESC(ParticleMesh, m_eNowSceneNum);

				m_dParticleTime = 0;
			}
			break;
		}
		case 1:
		{
			if (m_dParticleTime > 0.3)
			{
				// Fragment
				INSTMESHDESC ParticleMesh = GETPARTICLE->Get_EffectSetting_Mesh(CPartilceCreateMgr::E_MESHINST_EFFECTJ::Um_MeshBase4_TurnAuto,
					Prototype_Mesh_SM_4E_IceShards_01, //FBX
					0.01f, //파티클 전체의 지속시간 한번만 재생시키기 위해 짧음
					0.8f, //파티클 지속시간
					_float4(0.28f, 0.29f, 0.95f, 0.0f), //색깔1
					_float4(0), //색깔2 색깔1~2끼리 움직이면서 함 즉, 바꾸지 않게 하려면 같은 색을 넣고
					1, //여기에 1을 넣으면 됨
					_float3(2), //사이즈
					_float3(0.1f), //사이즈2 이것도 위에랑 마찬가지
					1);
				ParticleMesh.eParticleTypeID = InstanceEffect_Ball; //퍼지는 타입
				ParticleMesh.eInstanceCount = Prototype_ModelInstance_16; //인스턴스 갯수
				ParticleMesh.ePassID = MeshPass_BrightColor; //노이즈

															 //범위
				_float val = 1.5f;
				ParticleMesh.ParticleStartRandomPosMin = _float3(-val, -0.5f, -val);
				ParticleMesh.ParticleStartRandomPosMax = _float3(val, -0.5f, val);

				//디퓨즈 텍스쳐
				ParticleMesh.TempBuffer_0.w = 300;

				ParticleMesh.iMaskingTextureIndex = NONNMASK;//노이즈 타입이 아니면 동작하지 않음
				ParticleMesh.iMaskingTextureIndex = 122;
				ParticleMesh.iNoiseTextureIndex = 289;
				ParticleMesh.vEmissive_SBB = _float3(1.f, 1.0f, 1.f);
				ParticleMesh.Particle_Power = 20.0f;

				ParticleMesh.SubPowerRandomRange_RUL = _float3(1, 1, 1);
				ParticleMesh.fRotSpeed_Radian = XMConvertToRadians(max(1080, 0));


				ParticleMesh.TempBuffer_0.z = 1; //한번에 파티클이 생성됨

												 //위치지정
												 //_Matrix mat = m_pTransformCom->Get_WorldMatrix();
												 //_Vector pos = mat.r[3] + mat.r[2] * 3;
												 //testMesh.vFixedPosition = pos;

												 //위치지정
				ParticleMesh.FollowingTarget = nullptr;
				ParticleMesh.vFixedPosition = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS);
				//ParticleMesh.FollowingTarget = m_pTransformCom;
				ParticleMesh.iFollowingDir = FollowingDir_Up;

				GETPARTICLE->Create_MeshInst_DESC(ParticleMesh, m_eNowSceneNum);

				m_dParticleTime = 0;
			}
			break;
		}
		case 2:
		{
			if (m_dParticleTime > 0.3)
			{
				// Fragment
				INSTMESHDESC ParticleMesh = GETPARTICLE->Get_EffectSetting_Mesh(CPartilceCreateMgr::E_MESHINST_EFFECTJ::Um_MeshBase4_TurnAuto,
					Prototype_Mesh_SM_4E_IceShards_01, //FBX
					0.01f, //파티클 전체의 지속시간 한번만 재생시키기 위해 짧음
					0.8f, //파티클 지속시간
					_float4(0.28f, 0.29f, 0.95f, 0.0f), //색깔1
					_float4(0), //색깔2 색깔1~2끼리 움직이면서 함 즉, 바꾸지 않게 하려면 같은 색을 넣고
					1, //여기에 1을 넣으면 됨
					_float3(2), //사이즈
					_float3(0.1f), //사이즈2 이것도 위에랑 마찬가지
					1);
				ParticleMesh.eParticleTypeID = InstanceEffect_Ball; //퍼지는 타입
				ParticleMesh.eInstanceCount = Prototype_ModelInstance_16; //인스턴스 갯수
				ParticleMesh.ePassID = MeshPass_BrightColor; //노이즈

															 //범위
				_float val = 1.5f;
				ParticleMesh.ParticleStartRandomPosMin = _float3(-val, -0.5f, -val);
				ParticleMesh.ParticleStartRandomPosMax = _float3(val, -0.5f, val);

				//디퓨즈 텍스쳐
				ParticleMesh.TempBuffer_0.w = 271;

				ParticleMesh.iMaskingTextureIndex = NONNMASK;//노이즈 타입이 아니면 동작하지 않음
				ParticleMesh.iMaskingTextureIndex = 122;
				ParticleMesh.iNoiseTextureIndex = 289;
				ParticleMesh.vEmissive_SBB = _float3(1.f, 1.0f, 1.f);
				ParticleMesh.Particle_Power = 20.0f;

				ParticleMesh.SubPowerRandomRange_RUL = _float3(1, 1, 1);
				ParticleMesh.fRotSpeed_Radian = XMConvertToRadians(max(1080, 0));


				ParticleMesh.TempBuffer_0.z = 1; //한번에 파티클이 생성됨

												 //위치지정
												 //_Matrix mat = m_pTransformCom->Get_WorldMatrix();
												 //_Vector pos = mat.r[3] + mat.r[2] * 3;
												 //testMesh.vFixedPosition = pos;

												 //위치지정
				ParticleMesh.FollowingTarget = nullptr;
				ParticleMesh.vFixedPosition = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS);
				//ParticleMesh.FollowingTarget = m_pTransformCom;
				ParticleMesh.iFollowingDir = FollowingDir_Up;

				GETPARTICLE->Create_MeshInst_DESC(ParticleMesh, m_eNowSceneNum);

				m_dParticleTime = 0;
			}
			break;
		}
		case 3:
		{
			if (m_dParticleTime > 0.3)
			{
				// Fragment
				INSTMESHDESC ParticleMesh = GETPARTICLE->Get_EffectSetting_Mesh(CPartilceCreateMgr::E_MESHINST_EFFECTJ::Um_MeshBase4_TurnAuto,
					Prototype_Mesh_SM_4E_IceShards_01, //FBX
					0.01f, //파티클 전체의 지속시간 한번만 재생시키기 위해 짧음
					0.8f, //파티클 지속시간
					_float4(0.28f, 0.29f, 0.95f, 0.0f), //색깔1
					_float4(0), //색깔2 색깔1~2끼리 움직이면서 함 즉, 바꾸지 않게 하려면 같은 색을 넣고
					1, //여기에 1을 넣으면 됨
					_float3(2), //사이즈
					_float3(0.1f), //사이즈2 이것도 위에랑 마찬가지
					1);
				ParticleMesh.eParticleTypeID = InstanceEffect_Ball; //퍼지는 타입
				ParticleMesh.eInstanceCount = Prototype_ModelInstance_16; //인스턴스 갯수
				ParticleMesh.ePassID = MeshPass_BrightColor; //노이즈

															 //범위
				_float val = 1.5f;
				ParticleMesh.ParticleStartRandomPosMin = _float3(-val, -0.5f, -val);
				ParticleMesh.ParticleStartRandomPosMax = _float3(val, -0.5f, val);

				//디퓨즈 텍스쳐
				ParticleMesh.TempBuffer_0.w = 305;

				ParticleMesh.iMaskingTextureIndex = NONNMASK;//노이즈 타입이 아니면 동작하지 않음
				ParticleMesh.iMaskingTextureIndex = 122;
				ParticleMesh.iNoiseTextureIndex = 289;
				ParticleMesh.vEmissive_SBB = _float3(1.f, 1.0f, 1.f);
				ParticleMesh.Particle_Power = 20.0f;

				ParticleMesh.SubPowerRandomRange_RUL = _float3(1, 1, 1);
				ParticleMesh.fRotSpeed_Radian = XMConvertToRadians(max(1080, 0));


				ParticleMesh.TempBuffer_0.z = 1; //한번에 파티클이 생성됨

												 //위치지정
												 //_Matrix mat = m_pTransformCom->Get_WorldMatrix();
												 //_Vector pos = mat.r[3] + mat.r[2] * 3;
												 //testMesh.vFixedPosition = pos;

												 //위치지정
				ParticleMesh.FollowingTarget = nullptr;
				ParticleMesh.vFixedPosition = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS);
				//ParticleMesh.FollowingTarget = m_pTransformCom;
				ParticleMesh.iFollowingDir = FollowingDir_Up;

				GETPARTICLE->Create_MeshInst_DESC(ParticleMesh, m_eNowSceneNum);

				m_dParticleTime = 0;
			}
			break;
		}
		case 4:
		{
			if (m_dParticleTime > 0.3)
			{
				// Fragment
				INSTMESHDESC ParticleMesh = GETPARTICLE->Get_EffectSetting_Mesh(CPartilceCreateMgr::E_MESHINST_EFFECTJ::Um_MeshBase4_TurnAuto,
					Prototype_Mesh_SM_4E_IceShards_01, //FBX
					0.01f, //파티클 전체의 지속시간 한번만 재생시키기 위해 짧음
					0.8f, //파티클 지속시간
					_float4(0.28f, 0.29f, 0.95f, 0.0f), //색깔1
					_float4(0), //색깔2 색깔1~2끼리 움직이면서 함 즉, 바꾸지 않게 하려면 같은 색을 넣고
					1, //여기에 1을 넣으면 됨
					_float3(2), //사이즈
					_float3(0.1f), //사이즈2 이것도 위에랑 마찬가지
					1);
				ParticleMesh.eParticleTypeID = InstanceEffect_Ball; //퍼지는 타입
				ParticleMesh.eInstanceCount = Prototype_ModelInstance_16; //인스턴스 갯수
				ParticleMesh.ePassID = MeshPass_BrightColor; //노이즈

															 //범위
				_float val = 1.5f;
				ParticleMesh.ParticleStartRandomPosMin = _float3(-val, -0.5f, -val);
				ParticleMesh.ParticleStartRandomPosMax = _float3(val, -0.5f, val);

				//디퓨즈 텍스쳐
				ParticleMesh.TempBuffer_0.w = 309;

				ParticleMesh.iMaskingTextureIndex = NONNMASK;//노이즈 타입이 아니면 동작하지 않음
				ParticleMesh.iMaskingTextureIndex = 122;
				ParticleMesh.iNoiseTextureIndex = 289;
				ParticleMesh.vEmissive_SBB = _float3(1.f, 1.0f, 1.f);
				ParticleMesh.Particle_Power = 20.0f;

				ParticleMesh.SubPowerRandomRange_RUL = _float3(1, 1, 1);
				ParticleMesh.fRotSpeed_Radian = XMConvertToRadians(max(1080, 0));


				ParticleMesh.TempBuffer_0.z = 1; //한번에 파티클이 생성됨

												 //위치지정
												 //_Matrix mat = m_pTransformCom->Get_WorldMatrix();
												 //_Vector pos = mat.r[3] + mat.r[2] * 3;
												 //testMesh.vFixedPosition = pos;

												 //위치지정
				ParticleMesh.FollowingTarget = nullptr;
				ParticleMesh.vFixedPosition = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS);
				//ParticleMesh.FollowingTarget = m_pTransformCom;
				ParticleMesh.iFollowingDir = FollowingDir_Up;

				GETPARTICLE->Create_MeshInst_DESC(ParticleMesh, m_eNowSceneNum);

				m_dParticleTime = 0;
			}
			break;
		}
		case 5:
		{
			if (m_dParticleTime > 0.3)
			{
				// Fragment
				INSTMESHDESC ParticleMesh = GETPARTICLE->Get_EffectSetting_Mesh(CPartilceCreateMgr::E_MESHINST_EFFECTJ::Um_MeshBase4_TurnAuto,
					Prototype_Mesh_SM_4E_IceShards_01, //FBX
					0.01f, //파티클 전체의 지속시간 한번만 재생시키기 위해 짧음
					0.8f, //파티클 지속시간
					_float4(0.28f, 0.29f, 0.95f, 0.0f), //색깔1
					_float4(0), //색깔2 색깔1~2끼리 움직이면서 함 즉, 바꾸지 않게 하려면 같은 색을 넣고
					1, //여기에 1을 넣으면 됨
					_float3(2), //사이즈
					_float3(0.1f), //사이즈2 이것도 위에랑 마찬가지
					1);
				ParticleMesh.eParticleTypeID = InstanceEffect_Ball; //퍼지는 타입
				ParticleMesh.eInstanceCount = Prototype_ModelInstance_16; //인스턴스 갯수
				ParticleMesh.ePassID = MeshPass_BrightColor; //노이즈

															 //범위
				_float val = 1.5f;
				ParticleMesh.ParticleStartRandomPosMin = _float3(-val, -0.5f, -val);
				ParticleMesh.ParticleStartRandomPosMax = _float3(val, -0.5f, val);

				//디퓨즈 텍스쳐
				ParticleMesh.TempBuffer_0.w = 391;

				ParticleMesh.iMaskingTextureIndex = NONNMASK;//노이즈 타입이 아니면 동작하지 않음
				ParticleMesh.iMaskingTextureIndex = 122;
				ParticleMesh.iNoiseTextureIndex = 289;
				ParticleMesh.vEmissive_SBB = _float3(1.f, 1.0f, 1.f);
				ParticleMesh.Particle_Power = 20.0f;

				ParticleMesh.SubPowerRandomRange_RUL = _float3(1, 1, 1);
				ParticleMesh.fRotSpeed_Radian = XMConvertToRadians(max(1080, 0));


				ParticleMesh.TempBuffer_0.z = 1; //한번에 파티클이 생성됨

												 //위치지정
												 //_Matrix mat = m_pTransformCom->Get_WorldMatrix();
												 //_Vector pos = mat.r[3] + mat.r[2] * 3;
												 //testMesh.vFixedPosition = pos;

												 //위치지정
				ParticleMesh.FollowingTarget = nullptr;
				ParticleMesh.vFixedPosition = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS);
				//ParticleMesh.FollowingTarget = m_pTransformCom;
				ParticleMesh.iFollowingDir = FollowingDir_Up;

				GETPARTICLE->Create_MeshInst_DESC(ParticleMesh, m_eNowSceneNum);

				m_dParticleTime = 0;
			}
			break;
		}
		case 6:
		{
			if (m_dParticleTime > 0.3)
			{
				// Fragment
				INSTMESHDESC ParticleMesh = GETPARTICLE->Get_EffectSetting_Mesh(CPartilceCreateMgr::E_MESHINST_EFFECTJ::Um_MeshBase4_TurnAuto,
					Prototype_Mesh_SM_4E_IceShards_01, //FBX
					0.01f, //파티클 전체의 지속시간 한번만 재생시키기 위해 짧음
					0.8f, //파티클 지속시간
					_float4(0.28f, 0.29f, 0.95f, 0.0f), //색깔1
					_float4(0), //색깔2 색깔1~2끼리 움직이면서 함 즉, 바꾸지 않게 하려면 같은 색을 넣고
					1, //여기에 1을 넣으면 됨
					_float3(2), //사이즈
					_float3(0.1f), //사이즈2 이것도 위에랑 마찬가지
					1);
				ParticleMesh.eParticleTypeID = InstanceEffect_Ball; //퍼지는 타입
				ParticleMesh.eInstanceCount = Prototype_ModelInstance_16; //인스턴스 갯수
				ParticleMesh.ePassID = MeshPass_BrightColor; //노이즈

															 //범위
				_float val = 1.5f;
				ParticleMesh.ParticleStartRandomPosMin = _float3(-val, -0.5f, -val);
				ParticleMesh.ParticleStartRandomPosMax = _float3(val, -0.5f, val);

				//디퓨즈 텍스쳐
				ParticleMesh.TempBuffer_0.w = 198;

				ParticleMesh.iMaskingTextureIndex = NONNMASK;//노이즈 타입이 아니면 동작하지 않음
				ParticleMesh.iMaskingTextureIndex = 122;
				ParticleMesh.iNoiseTextureIndex = 289;
				ParticleMesh.vEmissive_SBB = _float3(1.f, 1.0f, 1.f);
				ParticleMesh.Particle_Power = 20.0f;

				ParticleMesh.SubPowerRandomRange_RUL = _float3(1, 1, 1);
				ParticleMesh.fRotSpeed_Radian = XMConvertToRadians(max(1080, 0));


				ParticleMesh.TempBuffer_0.z = 1; //한번에 파티클이 생성됨

												 //위치지정
												 //_Matrix mat = m_pTransformCom->Get_WorldMatrix();
												 //_Vector pos = mat.r[3] + mat.r[2] * 3;
												 //testMesh.vFixedPosition = pos;

												 //위치지정
				ParticleMesh.FollowingTarget = nullptr;
				ParticleMesh.vFixedPosition = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS);
				//ParticleMesh.FollowingTarget = m_pTransformCom;
				ParticleMesh.iFollowingDir = FollowingDir_Up;

				GETPARTICLE->Create_MeshInst_DESC(ParticleMesh, m_eNowSceneNum);

				m_dParticleTime = 0;
			}
			break;
		}
		case 7:
		{
			if (m_dParticleTime > 0.3)
			{
				// Fragment
				INSTMESHDESC ParticleMesh = GETPARTICLE->Get_EffectSetting_Mesh(CPartilceCreateMgr::E_MESHINST_EFFECTJ::Um_MeshBase4_TurnAuto,
					Prototype_Mesh_SM_4E_IceShards_01, //FBX
					0.01f, //파티클 전체의 지속시간 한번만 재생시키기 위해 짧음
					0.8f, //파티클 지속시간
					_float4(0.28f, 0.29f, 0.95f, 0.0f), //색깔1
					_float4(0), //색깔2 색깔1~2끼리 움직이면서 함 즉, 바꾸지 않게 하려면 같은 색을 넣고
					1, //여기에 1을 넣으면 됨
					_float3(2), //사이즈
					_float3(0.1f), //사이즈2 이것도 위에랑 마찬가지
					1);
				ParticleMesh.eParticleTypeID = InstanceEffect_Ball; //퍼지는 타입
				ParticleMesh.eInstanceCount = Prototype_ModelInstance_16; //인스턴스 갯수
				ParticleMesh.ePassID = MeshPass_BrightColor; //노이즈

															 //범위
				_float val = 1.5f;
				ParticleMesh.ParticleStartRandomPosMin = _float3(-val, -0.5f, -val);
				ParticleMesh.ParticleStartRandomPosMax = _float3(val, -0.5f, val);

				//디퓨즈 텍스쳐
				ParticleMesh.TempBuffer_0.w = 267;

				ParticleMesh.iMaskingTextureIndex = NONNMASK;//노이즈 타입이 아니면 동작하지 않음
				ParticleMesh.iMaskingTextureIndex = 122;
				ParticleMesh.iNoiseTextureIndex = 289;
				ParticleMesh.vEmissive_SBB = _float3(1.f, 1.0f, 1.f);
				ParticleMesh.Particle_Power = 20.0f;

				ParticleMesh.SubPowerRandomRange_RUL = _float3(1, 1, 1);
				ParticleMesh.fRotSpeed_Radian = XMConvertToRadians(max(1080, 0));


				ParticleMesh.TempBuffer_0.z = 1; //한번에 파티클이 생성됨

												 //위치지정
												 //_Matrix mat = m_pTransformCom->Get_WorldMatrix();
												 //_Vector pos = mat.r[3] + mat.r[2] * 3;
												 //testMesh.vFixedPosition = pos;

												 //위치지정
				ParticleMesh.FollowingTarget = nullptr;
				ParticleMesh.vFixedPosition = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS);
				//ParticleMesh.FollowingTarget = m_pTransformCom;
				ParticleMesh.iFollowingDir = FollowingDir_Up;

				GETPARTICLE->Create_MeshInst_DESC(ParticleMesh, m_eNowSceneNum);

				m_dParticleTime = 0;
			}

			break;
		}
		default:
			break;
		}
	}
		break;
	default:
		break;
	}
}

_float CGolu_Bullet::Take_Damage(CGameObject * pTargetObject, _float fDamageAmount, _fVector vDamageDir, _bool bKnockback, _float fKnockbackPower)
{
	return _float();
}

HRESULT CGolu_Bullet::SetUp_Components()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Renderer), TAG_COM(Com_Renderer), (CComponent**)&m_pRendererCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Shader_VT), TAG_COM(Com_Shader), (CComponent**)&m_pShaderCom));

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_VIBuffer_Rect), TAG_COM(Com_VIBuffer), (CComponent**)&m_pVIBufferCom));

	FAILED_CHECK(Add_Component(m_eNowSceneNum, TAG_CP(Prototype_Golu_Bullet), TAG_COM(Com_Texture), (CComponent**)&m_pTextureCom));

	m_pTextureCom->Change_TextureLayer(m_pGolu_BulletTag[m_Golu_BulletDesc.iGoluBulletType]);

	CTransform::TRANSFORMDESC tDesc = {};

	tDesc.fMovePerSec = m_Golu_BulletDesc.fSpeed;
	tDesc.fRotationPerSec = XMConvertToRadians(60);
	tDesc.vPivot = _float3(0, 0, 0);
	tDesc.fScalingPerSec = 1;

	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Transform), TAG_COM(Com_Transform), (CComponent**)&m_pTransformCom, &tDesc));


	SetUp_Collider();

	return S_OK;
}

HRESULT CGolu_Bullet::SetUp_Info()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (nullptr != m_Golu_BulletDesc.pObject)
	{
		CGameObject* pObject = static_cast<CGameObject*>(m_Golu_BulletDesc.pObject);
		m_pObjectTransform = static_cast<CTransform*>(pObject->Get_Component(TAG_COM(Com_Transform)));
		m_pTransformCom->Scaled_All(m_Golu_BulletDesc.fScale);

		_Vector vPosition = m_pObjectTransform->Get_MatrixState(CTransform::STATE_POS);

		m_vDefaultPos = XMVectorSet(0,0,0,0) + 	XMVector3Normalize(m_pObjectTransform->Get_MatrixState(CTransform::STATE_RIGHT)) *m_Golu_BulletDesc.fPositioning.x
		+XMVector3Normalize(m_pObjectTransform->Get_MatrixState(CTransform::STATE_UP)) * m_Golu_BulletDesc.fPositioning.y
		+XMVector3Normalize(m_pObjectTransform->Get_MatrixState(CTransform::STATE_LOOK)) * m_Golu_BulletDesc.fPositioning.z;



		vPosition += XMVector3Normalize(m_pObjectTransform->Get_MatrixState(CTransform::STATE_RIGHT)) *m_Golu_BulletDesc.fPositioning.x;
		vPosition += XMVector3Normalize(m_pObjectTransform->Get_MatrixState(CTransform::STATE_UP)) * m_Golu_BulletDesc.fPositioning.y;
		vPosition += XMVector3Normalize(m_pObjectTransform->Get_MatrixState(CTransform::STATE_LOOK)) * m_Golu_BulletDesc.fPositioning.z;

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

HRESULT CGolu_Bullet::SetUp_Collider()
{
	FAILED_CHECK(Add_Component(SCENE_STATIC, TAG_CP(Prototype_Collider), TAG_COM(Com_Collider), (CComponent**)&m_pColliderCom));


	switch (m_Golu_BulletDesc.iGoluBulletType)
	{
	case 10:
		break;

	default:
	{
		COLLIDERDESC			ColliderDesc;
		ZeroMemory(&ColliderDesc, sizeof(COLLIDERDESC));
		ColliderDesc.vScale = _float3(1.f,1.f,1.f);
		ColliderDesc.vRotation = _float4(0.f, 0.f, 0.f, 1.f);
		ColliderDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
		FAILED_CHECK(m_pColliderCom->Add_ColliderBuffer(COLLIDER_SPHERE, &ColliderDesc));
		break;
	}
	}

	return S_OK;
}

HRESULT CGolu_Bullet::Update_Collider(_double dDeltaTime)
{
	if (m_pColliderCom == nullptr)
		return S_OK;

	m_pColliderCom->Update_ConflictPassedTime(dDeltaTime);

	switch (m_Golu_BulletDesc.iGoluBulletType)
	{

	case 10:
		break;
	default:
	{
		_uint	iNumCollider = m_pColliderCom->Get_NumColliderBuffer();
		for (_uint i = 0; i < iNumCollider; i++)
			m_pColliderCom->Update_Transform(i, m_pTransformCom->Get_WorldMatrix());
		break;
	}
	}

	FAILED_CHECK(g_pGameInstance->Add_CollisionGroup(CollisionType_PlayerWeapon, this, m_pColliderCom));

	return S_OK;
}

HRESULT CGolu_Bullet::Billboard()
{
	//뷰 스페이스에서 역행렬을 하면 카메라의 월드 스페이스가 나옴
	_Vector vRight = m_pCameraTransform->Get_MatrixState(CTransform::STATE_RIGHT);
	_Vector vUp = m_pCameraTransform->Get_MatrixState(CTransform::STATE_UP);
	_Vector vLook = m_pCameraTransform->Get_MatrixState(CTransform::STATE_LOOK);

	vRight = m_pTransformCom->Get_MatrixScale(CTransform::STATE_RIGHT) * XMVector3Normalize(vRight);
	vUp = m_pTransformCom->Get_MatrixScale(CTransform::STATE_UP) * XMVector3Normalize(vUp);
	vLook = m_pTransformCom->Get_MatrixScale(CTransform::STATE_LOOK) * XMVector3Normalize(vLook);

	m_pTransformCom->Set_MatrixState(CTransform::STATE_RIGHT, vRight);
	m_pTransformCom->Set_MatrixState(CTransform::STATE_UP, vUp);
	m_pTransformCom->Set_MatrixState(CTransform::STATE_LOOK, vLook);

	return S_OK;
}

HRESULT CGolu_Bullet::Magnet()
{
	_Vector vPosition = m_pObjectTransform->Get_MatrixState(CTransform::STATE_POS);

	vPosition += XMVector3Normalize(m_pObjectTransform->Get_MatrixState(CTransform::STATE_RIGHT)) *m_Golu_BulletDesc.fPositioning.x;
	vPosition += XMVector3Normalize(m_pObjectTransform->Get_MatrixState(CTransform::STATE_UP)) * m_Golu_BulletDesc.fPositioning.y;
	vPosition += XMVector3Normalize(m_pObjectTransform->Get_MatrixState(CTransform::STATE_LOOK)) * m_Golu_BulletDesc.fPositioning.z;

	m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, vPosition);

	return S_OK;
}

_bool CGolu_Bullet::SrcPosToDestPos(_double dDeltaTime, _float fSpeed)
{
	_Vector vWorldMousePos,vDir;
	_float	fDistance;

	vWorldMousePos = XMLoadFloat3(&m_Golu_BulletDesc.fDestinationPos);

	vDir = XMVector3Normalize(vWorldMousePos - m_pTransformCom->Get_MatrixState(CTransform::STATE_POS));

	fDistance = m_pTransformCom->Get_MatrixState_Float3(CTransform::STATE_POS).Get_Distance(vWorldMousePos);

	if (fDistance >= 0.1)
	{
		m_pTransformCom->MovetoDir_bySpeed(vDir, fSpeed, dDeltaTime);
		return true;
	}
	else {
		return false;
	}

}

HRESULT CGolu_Bullet::PickingPosDir(_double dDeltaTime, _float fSpeed)
{
	if (m_bOnceSwitch == false)
	{
		_Vector vWorldMousePos, vDir;

		vWorldMousePos = XMLoadFloat3(&m_Golu_BulletDesc.fDestinationPos);

		vDir = XMVector3Normalize(vWorldMousePos - m_pTransformCom->Get_MatrixState(CTransform::STATE_POS));

		XMStoreFloat3(&m_fDir, vDir);
		m_bOnceSwitch = true;
	}

	m_pTransformCom->MovetoDir_bySpeed(XMLoadFloat3(&m_fDir), fSpeed, dDeltaTime);

	return S_OK;
}

HRESULT CGolu_Bullet::CreateDestPos()
{
	m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, m_Golu_BulletDesc.fDestinationPos);
	return S_OK;
}

HRESULT CGolu_Bullet::PlayOn(_double dDeltaTime)
{
	switch (m_Golu_BulletDesc.iGoluBulletType)
	{
	case CGolu_Bullet::FIREBALL:
	{
		Billboard();
		FireBall(dDeltaTime);
		break;
	}
	case CGolu_Bullet::BARRIERBULLET:
	{
		//_Vector vLookDir = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS) - m_pObjectTransform->Get_MatrixState(CTransform::STATE_POS);
		//m_pTransformCom->LookDir(vLookDir);

		BarrierBullet(dDeltaTime);

		//m_pTransformCom->Set_TurnSpeed(XMConvertToRadians(90.f));
		//m_pTransformCom->Turn_Revolution_CCW(m_pObjectTransform->Get_MatrixState(CTransform::STATE_POS), 1.f, dDeltaTime);
		//_Vector vPos = m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);
		//m_pTransformCom->Set_MatrixState(CTransform::STATE_POS, XMVectorSetY(vPos, XMVectorGetY(m_pObjectTransform->Get_MatrixState(CTransform::STATE_POS))));
		Billboard();
		break;
	}
	case CGolu_Bullet::BLACKHOLE:
	{
		Billboard();
		//BlackHole(dDeltaTime);
		Tornado(dDeltaTime);
		break;
	}
	case CGolu_Bullet::NONTEXTURE:
	{
		break;
	}
	default:
		break;
	}
	return S_OK;
}

HRESULT CGolu_Bullet::Initialize_Bullet()
{

	switch (m_Golu_BulletDesc.iGoluBulletType)
	{
	case BLACKHOLE:
	{
		CreateDestPos();
		break;
	}
	case NONTEXTURE:
	{
		CreateDestPos();
		break;
	}
	default:
		break;
	}

	return S_OK;
}

HRESULT CGolu_Bullet::FireBall(_double dDeltaTime)
{
	m_fAngle += 360.f;
	//로테이션이 초기 상태를 기준으로 돌림
	//턴이 현재 상태를 기준으로 돌림
	//m_pTransformCom->Rotation_CCW(XMVE, XMConvertToRadians(m_fAngle));
	m_pTransformCom->Set_TurnSpeed(1);
	m_pTransformCom->Turn_CCW(m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK),XMConvertToRadians(m_fAngle* (_float)dDeltaTime));
	
	PickingPosDir(dDeltaTime, m_Golu_BulletDesc.fSpeed);

	//if (SrcPosToDestPos(dDeltaTime, m_Golu_BulletDesc.fSpeed) == false)
	//	Set_IsDead();

	return S_OK;
}

HRESULT CGolu_Bullet::BarrierBullet(_double dDeltaTime)
{
	_float3 PlayerPos = m_pObjectTransform->Get_MatrixState_Float3(CTransform::STATE_POS);

	m_fAngle += 360.f*(_float)dDeltaTime;

	//스자이공부에서 자전이 필요없기 때문에 스이공부만 넣은것
	m_pTransformCom->Set_Matrix(XMMatrixScaling(m_Golu_BulletDesc.fScale.x, m_Golu_BulletDesc.fScale.y, m_Golu_BulletDesc.fScale.z) *
		XMMatrixTranslation(m_vDefaultPos.x, m_vDefaultPos.y, m_vDefaultPos.z) *
		XMMatrixRotationAxis(XMVectorSet(0, 1, 0, 0), XMConvertToRadians(m_fAngle)) *
		XMMatrixTranslation(PlayerPos.x, PlayerPos.y, PlayerPos.z));

	return S_OK;
}

HRESULT CGolu_Bullet::BlackHole(_double dDeltaTime)
{
	m_fAngle += 50.f;
	//로테이션이 초기 상태를 기준으로 돌림
	//턴이 현재 상태를 기준으로 돌림
	//m_pTransformCom->Rotation_CCW(XMVE, XMConvertToRadians(m_fAngle));
	m_pTransformCom->Set_TurnSpeed(1);
	m_pTransformCom->Turn_CW(m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK), XMConvertToRadians(m_fAngle* (_float)dDeltaTime));

	return S_OK;
}

HRESULT CGolu_Bullet::Tornado(_double dDeltaTime)
{
	if (m_bOnceSwitch == false)
	{
#pragma region Mesh


		{
			NONINSTNESHEFTDESC tNIMEDesc;
			ZeroMemory(&tNIMEDesc,sizeof(NONINSTNESHEFTDESC));

			tNIMEDesc.eMeshType = Prototype_Mesh_Tornado;
			tNIMEDesc.fAppearTime = 0.5f;
			tNIMEDesc.fMaxTime_Duration = tNIMEDesc.fAppearTime*2.f;
			tNIMEDesc.vLookDir = _float3(1, 0, 0);
			tNIMEDesc.noisingdir = _float2(0, 1);

			tNIMEDesc.fDistortionNoisingPushPower = 20.f;
			tNIMEDesc.NoiseTextureIndex = 6;
			tNIMEDesc.MaskTextureIndex = 81;
			tNIMEDesc.iDiffuseTextureIndex = 365;
			tNIMEDesc.m_iPassIndex = 19;
			tNIMEDesc.vEmissive = _float4(1, 0.5f, 1.f, 0);
			tNIMEDesc.vLimLight = _float4(0.35f, 0.85f, 0.35f, 1);
			tNIMEDesc.NoiseTextureIndex = 381;
			tNIMEDesc.vColor = _float4(1, 1, 1, 1);

			tNIMEDesc.RotAxis = FollowingDir_Up;
			tNIMEDesc.RotationSpeedPerSec = -1080.f;
			tNIMEDesc.vSize = _float3(2.f, 1.5f, 2.f);
			tNIMEDesc.vLimLight = _float4(_float3(vOldRimLightColor), 1);

			tNIMEDesc.MoveDir = FollowingDir_Look;
			tNIMEDesc.MoveSpeed = 0.f;

			m_vecJYNonMeshParticleDesc.push_back(tNIMEDesc);
		}
		{
			NONINSTNESHEFTDESC tNIMEDesc;
			ZeroMemory(&tNIMEDesc, sizeof(NONINSTNESHEFTDESC));

			tNIMEDesc.eMeshType = Prototype_Mesh_Tornado2;
			tNIMEDesc.fAppearTime = 0.5f;
			tNIMEDesc.fMaxTime_Duration = tNIMEDesc.fAppearTime*2.f;
			tNIMEDesc.vLookDir = _float3(1, 0, 0);

			tNIMEDesc.noisingdir = _float2(-1, 0);

			tNIMEDesc.fDistortionNoisingPushPower = 20.f;
			tNIMEDesc.NoiseTextureIndex = 6;
			tNIMEDesc.MaskTextureIndex = 81;
			tNIMEDesc.iDiffuseTextureIndex = 365;
			tNIMEDesc.m_iPassIndex = 19;
			tNIMEDesc.vEmissive = _float4(1, 0.5f, 1.f, 0);
			tNIMEDesc.vLimLight = _float4(0.35f, 0.85f, 0.35f, 1);
			tNIMEDesc.NoiseTextureIndex = 381;
			tNIMEDesc.vColor = _float4(1, 1, 1, 1);

			tNIMEDesc.RotAxis = FollowingDir_Up;
			tNIMEDesc.RotationSpeedPerSec = -1080.f;
			tNIMEDesc.vSize = _float3(2.f, 1.5f, 2.f);
			tNIMEDesc.vLimLight = _float4(_float3(vOldRimLightColor), 1);

			tNIMEDesc.MoveDir = FollowingDir_Look;
			tNIMEDesc.MoveSpeed = 0.f;
			m_vecJYNonMeshParticleDesc.push_back(tNIMEDesc);
		}
#pragma endregion


		m_vecJYNonMeshParticleDesc[1].vPosition = m_vecJYNonMeshParticleDesc[0].vPosition
			= m_pTransformCom->Get_MatrixState(CTransform::STATE_POS);


		g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_PlayerEffect), TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecJYNonMeshParticleDesc[0]);
		g_pGameInstance->Add_GameObject_To_Layer(m_eNowSceneNum, TAG_LAY(Layer_PlayerEffect), TAG_OP(Prototype_NonInstanceMeshEffect), &m_vecJYNonMeshParticleDesc[1]);


		m_bOnceSwitch = true;
	}
	return S_OK;
}

HRESULT CGolu_Bullet::FireRing(_double dDeltaTime)
{
	m_fAngle += 360.f;
	//로테이션이 초기 상태를 기준으로 돌림
	//턴이 현재 상태를 기준으로 돌림
	//m_pTransformCom->Rotation_CCW(XMVE, XMConvertToRadians(m_fAngle));
	m_pTransformCom->Set_TurnSpeed(1);
	m_pTransformCom->Turn_CCW(m_pTransformCom->Get_MatrixState(CTransform::STATE_LOOK), XMConvertToRadians(m_fAngle* (_float)dDeltaTime));

	if (SrcPosToDestPos(dDeltaTime, m_Golu_BulletDesc.fSpeed) == false)
		Set_IsDead();

	return S_OK;
}

HRESULT CGolu_Bullet::Distance()
{
	m_fDistance = m_pObjectTransform->Get_MatrixState_Float3(CTransform::STATE_POS).Get_Distance(m_pTransformCom->Get_MatrixState(CTransform::STATE_POS));

	return S_OK;
}

CGolu_Bullet * CGolu_Bullet::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, void * pArg)
{
	CGolu_Bullet*	pInstance = NEW CGolu_Bullet(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pArg)))
	{
		MSGBOX("Failed to Created CGolu_Bullet");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CGolu_Bullet::Clone(void * pArg)
{
	CGolu_Bullet*	pInstance = NEW CGolu_Bullet(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSGBOX("Failed to Created CGolu_Bullet");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CGolu_Bullet::Free()
{

	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pColliderCom);
}